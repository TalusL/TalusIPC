
#include <cstdlib>
#include <random>

#include "Network/TcpServer.h"
#include "Network/Socket.h"
#include "Http/HttpSession.h"
#include "onvifxml.h"
#include <regex>
#include <Util/base64.h>
#include <Util/SHA1.h>
#include <Common/MediaSource.h>

using namespace toolkit;
using namespace mediakit;

std::shared_ptr<toolkit::TcpServer> httpSrv;
Socket::Ptr onvifDiscoverSrv;

#define ONVIF_DISCOVERY_PORT 3702
#define ONVIF_DISCOVERY_MULTI_CAST_ADDR "239.255.255.250"
#define ONVIF_HTTP_PORT 8181
#define ONVIF_USER "talus"
#define ONVIF_PASS "talus"
#define MODEL "TalusIPC-Model"
#define VENDOR "TalusIPC"
#define FIRMWARE_VERSION "1.0.0"
#define SERIAL_NUMBER "112233445566778899"


#define INFO(...)
#define WARNING(...)
#define INFO(...)


static uint32_t msgSeq = 0;

// soap
static StrCaseMap soapDefaultHeader;

std::string uuid_generate() {
    const char *chars = "0123456789abcdef";
    char uuid[37];

    int i, j = 0;
    for (i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            uuid[i] = '-';
        } else {
            std::mt19937 rng(std::random_device{}());
            uuid[i] = chars[rng() % 16];
        }
    }
    uuid[36] = '\0';
    return std::string(uuid);
}

std::string removeXmlWhitespace(const std::string& s) {
    // 正则表达式匹配：
    // 1. >\s+<  - 标签之间的空白
    // 2. ^\s+    - 开头的空白
    // 3. \s+$    - 结尾的空白
    // 4. \s{2,}  - 连续的多个空白

    std::string result = s;

    // 移除标签之间的空白
    result = std::regex_replace(result, std::regex(R"(>\s+<)"), "><");

    // 移除开头和结尾的空白
    result = std::regex_replace(result, std::regex(R"(^\s+)"), "");
    result = std::regex_replace(result, std::regex(R"(\s+$)"), "");

    // 将多个连续空白替换为单个空格（保留内容中的必要空格）
    result = std::regex_replace(result, std::regex(R"([\s]{2,})"), " ");

    return result;
}


std::string regexSearch(const std::string& str,const std::regex& regex) {
    auto s = removeXmlWhitespace(str);
    std::smatch matches;
    if (std::regex_search(s, matches, regex)) {
        if (matches.size() > 1) {
            return matches[1].str();
        }
    }
    return {};
}




bool onvif_validate_soap_auth(const std::string& xml) {
    // 提取字段
    auto userName = regexSearch(xml,std::regex(R"(Header[\s\S]+?UsernameToken[\s\S]+?Username[^>]{0,}>([^<]+)<)"));

    if (userName!=ONVIF_USER) {
        return false;
    }

    auto password = regexSearch(xml,std::regex(R"(Header[\s\S]+?UsernameToken[\s\S]+?Password[^>]{0,}>([^<]+)<)"));
    auto nonce = regexSearch(xml,std::regex(R"(Header[\s\S]+?UsernameToken[\s\S]+?Nonce[^>]{0,}>([^<]+)<)"));
    auto created = regexSearch(xml,std::regex(R"(Header[\s\S]+?UsernameToken[\s\S]+?Created[^>]{0,}>([^<]+)<)"));

    auto n_dec = decodeBase64(nonce);
    SHA1 ctx;
    ctx.update(n_dec);
    ctx.update(created);
    ctx.update(ONVIF_PASS);
    auto rs = ctx.final_bin();
    auto p_b64 = encodeBase64(rs);

    if (p_b64 == password) {
        return true;
    }
    return false;
}




int ug_start_discovery_server() {
    // 初始化 onvif 发现服务
    onvifDiscoverSrv = Socket::createSocket();
    if (!onvifDiscoverSrv->bindUdpSock(ONVIF_DISCOVERY_PORT)) {
        return EXIT_FAILURE;
    }

    std::string localIp = SockUtil::get_local_ip();
    ip_mreq mr{};
    mr.imr_multiaddr.s_addr = inet_addr(ONVIF_DISCOVERY_MULTI_CAST_ADDR);
    mr.imr_interface.s_addr = inet_addr(localIp.c_str());
    if (setsockopt(onvifDiscoverSrv->rawFD(), IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mr, sizeof(mr)) == -1) {
        return EXIT_FAILURE;
    }

    std::string uuid = uuid_generate();
    std::string msg_uuid = uuid_generate();
    std::string xaddr = StrPrinter<<"http://"<<localIp<<":"<<ONVIF_HTTP_PORT<<"/onvif/device_service";

    char hello[4096] = {};
    sprintf(hello,helloxml.c_str(),msg_uuid.c_str(),std::to_string(++msgSeq).c_str(),uuid.c_str(),VENDOR "/" MODEL,MODEL,xaddr.c_str());

    // 发送Hello
    auto target = SockUtil::make_sockaddr(ONVIF_DISCOVERY_MULTI_CAST_ADDR,ONVIF_DISCOVERY_PORT);
    onvifDiscoverSrv->send(removeXmlWhitespace(hello),(sockaddr*)&target);

    onvifDiscoverSrv->setOnRead([uuid,xaddr](const Buffer::Ptr &buf, struct sockaddr *addr , int len){
        // 接收到onvif发现请求、回复
        if (std::string(buf->data()).find("XAddrs") != std::string::npos) {
            return;
        }
        if (!std::regex_match(buf->data(), std::regex(R"([\s\S]+Body[\s\S]+Probe[\s\S]+)"))) {
            return;
        }
        ++msgSeq;
        auto msg_uuid = uuid_generate();
        std::string relates_to_uuid = regexSearch(buf->data(),std::regex(R"([\s\S]+Header[\s\S]+MessageID[\s\S]+([a-f0-9\-]{36})[\s\S]+MessageID[\s\S]+Header[\s\S]+)"));
        char match[4096] = {};
        sprintf(match,discoveryxml.c_str(),
            msg_uuid.c_str(),
            relates_to_uuid.c_str(),
            std::to_string(++msgSeq).c_str(),
            uuid.c_str(),
            VENDOR "/" MODEL,
            MODEL,
            xaddr.c_str());
        auto resp = removeXmlWhitespace(match);
        InfoL<<"Probe from "<<SockUtil::inet_ntoa(addr)<<":"<<SockUtil::inet_port(addr)<<" resp:\n"<<resp;
        onvifDiscoverSrv->send(resp,addr);
        auto target = SockUtil::make_sockaddr(ONVIF_DISCOVERY_MULTI_CAST_ADDR,ONVIF_DISCOVERY_PORT);
        onvifDiscoverSrv->send(removeXmlWhitespace(match),(sockaddr*)&target);
    });
    return EXIT_SUCCESS;
}

void handle_GetCapabilities(BroadcastHttpRequestArgs){
    char resp[4096] = {};
    std::string localIp = SockUtil::get_local_ip();
    sprintf(resp,capabilitiesxml.c_str(),
        localIp.c_str(),ONVIF_HTTP_PORT,
        localIp.c_str(),ONVIF_HTTP_PORT,
        localIp.c_str(),ONVIF_HTTP_PORT,
        localIp.c_str(),ONVIF_HTTP_PORT,
        localIp.c_str(),ONVIF_HTTP_PORT,
        localIp.c_str(),ONVIF_HTTP_PORT
    );
    invoker(200,soapDefaultHeader,removeXmlWhitespace(resp));
}

void handle_GetDeviceInformation(BroadcastHttpRequestArgs){
    char resp[4096] = {};
    sprintf(resp,deviceinfoxml.c_str(),
        VENDOR, MODEL, FIRMWARE_VERSION, SERIAL_NUMBER, MODEL
    );
    invoker(200,soapDefaultHeader,removeXmlWhitespace(resp));
}

void handle_GetSystemDateAndTime(BroadcastHttpRequestArgs){

    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = gmtime(&now);

    char resp[4096] = {};
    sprintf(resp,systemtimexml.c_str(),
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
        tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday
    );
    invoker(200,soapDefaultHeader,removeXmlWhitespace(resp));
}

void handle_GetProfiles(BroadcastHttpRequestArgs){
    char profile[4096]{};
    char profileCnt = 0;
    int profileLen = 0;

    // 主码流
    if (auto mainStream = MediaSource::find(RTSP_SCHEMA,DEFAULT_VHOST,"live","MainStream",false)) {
        if (auto vTrack = mainStream->getTrack(TrackVideo)) {
            auto vHeight = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoHeight();
            auto vWidth = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoWidth();
            auto vCodec = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getCodecId() == CodecH264 ? "H264" : "H265";
            auto fps = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoFps();
            auto bitrate = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getBitRate();

            profileLen += sprintf(&profile[profileLen], mediaprofilexml.c_str(),
                "MainStream", "profile_1",
                profileCnt + 1, profileCnt + 1,
                vHeight,
                vWidth,
                profileCnt + 1, profileCnt + 1,
                vCodec,
                vWidth, vHeight,
                fps, bitrate);
            profileCnt++;
        }
    }

    // 子码流1
    if (auto subStream1 = MediaSource::find(RTSP_SCHEMA,DEFAULT_VHOST,"live","SubStream1",false)) {
        if (auto vTrack = subStream1->getTrack(TrackVideo)) {
            auto vHeight = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoHeight();
            auto vWidth = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoWidth();
            auto vCodec = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getCodecId() == CodecH264 ? "H264" : "H265";
            auto fps = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoFps();
            auto bitrate = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getBitRate();

            profileLen += sprintf(&profile[profileLen], mediaprofilexml.c_str(),
                "SubStream1", "profile_2",
                profileCnt + 1, profileCnt + 1,
                vHeight,
                vWidth,
                profileCnt + 1, profileCnt + 1,
                vCodec,
                vWidth, vHeight,
                fps, bitrate);
            profileCnt++;
        }
    }

    // 子码流2
    if (auto subStream2 = MediaSource::find(RTSP_SCHEMA,DEFAULT_VHOST,"live","SubStream2",false)) {
        if (auto vTrack = subStream2->getTrack(TrackVideo)) {
            auto vHeight = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoHeight();
            auto vWidth = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoWidth();
            auto vCodec = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getCodecId() == CodecH264 ? "H264" : "H265";
            auto fps = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoFps();
            auto bitrate = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getBitRate();

            profileLen += sprintf(&profile[profileLen], mediaprofilexml.c_str(),
                "SubStream2", "profile_2",
                profileCnt + 1, profileCnt + 1,
                vHeight,
                vWidth,
                profileCnt + 1, profileCnt + 1,
                vCodec,
                vWidth, vHeight,
                fps, bitrate);
            profileCnt++;
        }
    }

    char resp[8192] = {};
    sprintf(resp,mediaprofilesxml.c_str(),profile);
    invoker(200,soapDefaultHeader,removeXmlWhitespace(resp));
}

void handle_GetSnapshotUri(BroadcastHttpRequestArgs){
    char snapshot_url[256]{};
    std::string localIp = SockUtil::get_local_ip();
    snprintf(snapshot_url, sizeof(snapshot_url), "http:///%s:%d/image.jpg",
    localIp.c_str(),ONVIF_HTTP_PORT);
    char resp[4096] = {};
    sprintf(resp,snapshotxml.c_str(),snapshot_url);

    invoker(200,soapDefaultHeader,removeXmlWhitespace(resp));
}

void handle_GetStreamUri(BroadcastHttpRequestArgs){
    char resp[4096] = {};
    std::string localIp = SockUtil::get_local_ip();

    char stream_url[256]{};
    auto profile = regexSearch(parser.content(),std::regex(R"(Body[\s\S]+?ProfileToken[^>]{0,}>([^<]+)<)"));
    if (profile.empty()) {
        profile = "MainStream";
    }
    snprintf(stream_url, sizeof(stream_url), ("rtsp://%s:%d/live/"+profile).c_str(),
    localIp.c_str(), 554);

    sprintf(resp,streamxml.c_str(),stream_url);
    invoker(200,soapDefaultHeader,removeXmlWhitespace(resp));
}

void handle_GetVideoSources(BroadcastHttpRequestArgs){

    char resp[4096] = {};

    // 主码流
    if (auto mainStream = MediaSource::find(RTSP_SCHEMA,DEFAULT_VHOST,"live","MainStream",false)) {
        if (auto vTrack = mainStream->getTrack(TrackVideo)) {
            auto vHeight = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoHeight();
            auto vWidth = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoWidth();
            auto fps = std::dynamic_pointer_cast<VideoTrack>(vTrack)->getVideoFps();

            std::string localIp = SockUtil::get_local_ip();
            sprintf(resp,videosourcesxml.c_str(),fps, vWidth, vHeight);

            invoker(200,soapDefaultHeader,removeXmlWhitespace(resp));
            return;
        }
    }


    invoker(404,{},removeXmlWhitespace(resp));
}

void handle_GetEndpointReference(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}
void handle_GetServices(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}
void handle_GetConfigurations(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}
void handle_GetAudioOutputConfigurations(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}
void handle_GetMoveOptions(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}
void handle_SetSynchronizationPoint(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}

void handle_PtzContinuousMove(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}
void handle_PtzStop(BroadcastHttpRequestArgs) {
    WarnL<<__FUNCTION__<<" Not Implemented";
    invoker(501,{},"");
}

// Action 映射表
using request_handler = void(*)(BroadcastHttpRequestArgs);
static std::unordered_map<std::string, request_handler> s_cmd_functions;
static onceToken token([]() {
    s_cmd_functions.emplace("GetCapabilities", handle_GetCapabilities);
    s_cmd_functions.emplace("GetDeviceInformation", handle_GetDeviceInformation);
    s_cmd_functions.emplace("GetSystemDateAndTime", handle_GetSystemDateAndTime);
    s_cmd_functions.emplace("GetProfiles", handle_GetProfiles);
    s_cmd_functions.emplace("GetSnapshotUri", handle_GetSnapshotUri);
    s_cmd_functions.emplace("GetStreamUri", handle_GetStreamUri);
    s_cmd_functions.emplace("GetVideoSources", handle_GetVideoSources);
    s_cmd_functions.emplace("GetEndpointReference", handle_GetEndpointReference);
    s_cmd_functions.emplace("GetServices", handle_GetServices);
    s_cmd_functions.emplace("GetConfigurations", handle_GetConfigurations);
    s_cmd_functions.emplace("GetAudioOutputConfigurations", handle_GetAudioOutputConfigurations);
    s_cmd_functions.emplace("GetMoveOptions", handle_GetMoveOptions);
    s_cmd_functions.emplace("SetSynchronizationPoint", handle_SetSynchronizationPoint);
    s_cmd_functions.emplace("tptz:ContinuousMove", handle_PtzContinuousMove);
    s_cmd_functions.emplace("tptz:Stop", handle_PtzStop);
});

std::string onvif_extract_soap_action(const std::string& xml) {
    static std::string actions;
    if (actions.empty()) {
        int count = 0;
        for (const auto& s_cmd_function : s_cmd_functions) {
            actions+=s_cmd_function.first;
            count++;
            if (count < s_cmd_functions.size()) {
                actions+="|";
            }
        }
    }
    static auto regStr = R"(Body[^>]{0,}>[\s\S]+()" + actions + R"()[\s\S]+>)";
    return regexSearch(xml,std::regex(regStr));
}


int ug_start_http_server() {

    // 关闭http服务器的目录浏览功能
    mINI::Instance()[Http::kDirMenu] = false;
    // 启动http服务器
    httpSrv = std::make_shared<TcpServer>();
    httpSrv->start<HttpSession>(ONVIF_HTTP_PORT);
    // 监听请求
    NoticeCenter::Instance().addListener(nullptr,Broadcast::kBroadcastHttpRequest,[](BroadcastHttpRequestArgs) {
        consumed = true;
        if ( parser.method() != "GET" && parser.method() != "POST" ) {
            invoker(405,{},"");
            ErrorL<<"HTTP METHOD not support! "<<sender.get_peer_ip();
            return;
        }
        // 收到HTTP请求
        auto url = parser.url();
        if (!start_with(url,"/onvif")) {
            invoker(404,{},"");
            ErrorL<<"HTTP URL not support! "<<sender.get_peer_ip();
            return;
        }
        // 解出 action
        std::string action = onvif_extract_soap_action(parser.content());
        if (action.empty()) {
            invoker(400,{},"");
            InfoL<<"HTTP ACTION not support! "<< parser.content() <<" "<<sender.get_peer_ip();
            return;
        }
        InfoL<<"Action:"<<action<<" from "<<sender.get_peer_ip();
        // soap 密码鉴权
        StrCaseMap headers;
        if (!onvif_validate_soap_auth(parser.content().c_str())) {
            headers["WWW-Authenticate"] = "Digest realm=\"TalusIPC\"";
            headers["Content-Type"] = "application/soap+xml";

            invoker(401,headers,removeXmlWhitespace(badauthxml));
            InfoL<<"Onvif Authorization fail! "<<sender.get_peer_ip();
            return;
        }
        // 按 Action 分发请求
        auto it = s_cmd_functions.find(action);
        if (it == s_cmd_functions.end()) {
            invoker(400,{},"");
            InfoL<<"HTTP ACTION not support! "<<sender.get_peer_ip();
            return;
        }
        it->second(parser, invoker, consumed, sender);

    });

    return EXIT_SUCCESS;
}

int start_onvif_server() {

    soapDefaultHeader["Content-Type"] = "application/soap+xml";

    if (ug_start_discovery_server()) {
        return EXIT_FAILURE;
    }

    if (ug_start_http_server()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}