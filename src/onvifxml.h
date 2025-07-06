//
// Created by Talus on 2025/7/5.
//

#ifndef ONVIFXML_H
#define ONVIFXML_H


const std::string capabilitiesxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:tds="http://www.onvif.org/ver10/device/wsdl" xmlns:tt="http://www.onvif.org/ver10/schema">
  <SOAP-ENV:Body>
    <tds:GetCapabilitiesResponse>
      <tds:Capabilities>
        <tt:Analytics>
          <tt:XAddr>http://%s:%d/onvif/analytics_service</tt:XAddr>
          <tt:RuleSupport>false</tt:RuleSupport>
          <tt:AnalyticsModuleSupport>false</tt:AnalyticsModuleSupport>
        </tt:Analytics>
        <tt:Device>
          <tt:XAddr>http://%s:%d/onvif/device_service</tt:XAddr>
          <tt:Network>
            <tt:IPFilter>false</tt:IPFilter>
            <tt:ZeroConfiguration>false</tt:ZeroConfiguration>
            <tt:IPVersion6>false</tt:IPVersion6>
            <tt:DynDNS>false</tt:DynDNS>
          </tt:Network>
          <tt:System>
            <tt:DiscoveryResolve>true</tt:DiscoveryResolve>
            <tt:DiscoveryBye>true</tt:DiscoveryBye>
            <tt:RemoteDiscovery>false</tt:RemoteDiscovery>
            <tt:SystemBackup>false</tt:SystemBackup>
            <tt:SystemLogging>false</tt:SystemLogging>
            <tt:FirmwareUpgrade>false</tt:FirmwareUpgrade>
            <tt:SupportedVersions>
              <tt:Major>2</tt:Major>
              <tt:Minor>40</tt:Minor>
            </tt:SupportedVersions>
          </tt:System>
          <tt:IO>
            <tt:InputConnectors>0</tt:InputConnectors>
            <tt:RelayOutputs>0</tt:RelayOutputs>
          </tt:IO>
          <tt:Security>
            <tt:TLS1.1>false</tt:TLS1.1>
            <tt:TLS1.2>false</tt:TLS1.2>
            <tt:OnboardKeyGeneration>false</tt:OnboardKeyGeneration>
            <tt:AccessPolicyConfig>false</tt:AccessPolicyConfig>
            <tt:DefaultAccessPolicy>false</tt:DefaultAccessPolicy>
            <tt:X.509Token>false</tt:X.509Token>
            <tt:RELToken>false</tt:RELToken>
            <tt:SAMLToken>false</tt:SAMLToken>
            <tt:KerberosToken>false</tt:KerberosToken>
            <tt:UsernameToken>true</tt:UsernameToken>
            <tt:HttpDigest>true</tt:HttpDigest>
          </tt:Security>
        </tt:Device>
        <tt:Events>
          <tt:XAddr>http://%s:%d/onvif/event_service</tt:XAddr>
          <tt:WSSubscriptionPolicySupport>false</tt:WSSubscriptionPolicySupport>
          <tt:WSPullPointSupport>false</tt:WSPullPointSupport>
          <tt:WSPausableSubscriptionManagerInterfaceSupport>false</tt:WSPausableSubscriptionManagerInterfaceSupport>
        </tt:Events>
        <tt:Imaging>
          <tt:XAddr>http://%s:%d/onvif/imaging_service</tt:XAddr>
        </tt:Imaging>
        <tt:Media>
          <tt:XAddr>http://%s:%d/onvif/media_service</tt:XAddr>
          <tt:StreamingCapabilities>
            <tt:RTPMulticast>false</tt:RTPMulticast>
            <tt:RTP_TCP>true</tt:RTP_TCP>
            <tt:RTP_RTSP_TCP>true</tt:RTP_RTSP_TCP>
          </tt:StreamingCapabilities>
          <tt:ProfileCapabilities>
            <tt:MaximumNumberOfProfiles>2</tt:MaximumNumberOfProfiles>
          </tt:ProfileCapabilities>
        </tt:Media>
        <tt:PTZ>
          <tt:XAddr>http://%s:%d/onvif/ptz_service</tt:XAddr>
        </tt:PTZ>
      </tds:Capabilities>
    </tds:GetCapabilitiesResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";
const std::string deviceinfoxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:tds="http://www.onvif.org/ver10/device/wsdl" xmlns:tt="http://www.onvif.org/ver10/schema">
  <SOAP-ENV:Body>
    <tds:GetDeviceInformationResponse>
      <tds:Manufacturer>%s</tds:Manufacturer>
      <tds:Model>%s</tds:Model>
      <tds:FirmwareVersion>%s</tds:FirmwareVersion>
      <tds:SerialNumber>%s</tds:SerialNumber>
      <tds:HardwareId>%s</tds:HardwareId>
    </tds:GetDeviceInformationResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>
)";
const std::string discoveryxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope"
                   xmlns:SOAP-ENC="http://www.w3.org/2003/05/soap-encoding"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xmlns:xsd="http://www.w3.org/2001/XMLSchema"
                   xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing"
                   xmlns:wsdd="http://schemas.xmlsoap.org/ws/2005/04/discovery"
                   xmlns:tdn="http://www.onvif.org/ver10/network/wsdl">
    <SOAP-ENV:Header>
        <wsa:MessageID>uuid:%s</wsa:MessageID>
        <wsa:RelatesTo>%s</wsa:RelatesTo>
        <wsa:ReplyTo SOAP-ENV:mustUnderstand="true">
            <wsa:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:Address>
        </wsa:ReplyTo>
        <wsa:To SOAP-ENV:mustUnderstand="true">http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>
        <wsa:Action SOAP-ENV:mustUnderstand="true">http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action>
        <wsdd:AppSequence InstanceId="0"
                          MessageNumber="%s"/>
    </SOAP-ENV:Header>
    <SOAP-ENV:Body>
        <wsdd:ProbeMatches>
            <wsdd:ProbeMatch>
                <wsa:EndpointReference>
                    <wsa:Address>urn:uuid:%s</wsa:Address>
                </wsa:EndpointReference>
                <wsdd:Types>tdn:NetworkVideoTransmitter</wsdd:Types>
                <wsdd:Scopes>onvif://www.onvif.org/type/video_encoder onvif://www.onvif.org/type/audio_encoder onvif://www.onvif.org/type/ptz onvif://www.onvif.org/hardware/%s onvif://www.onvif.org/name/%s onvif://www.onvif.org/location/anywhere</wsdd:Scopes>
                <wsdd:XAddrs>%s</wsdd:XAddrs>
                <wsdd:MetadataVersion>0</wsdd:MetadataVersion>
            </wsdd:ProbeMatch>
        </wsdd:ProbeMatches>
    </SOAP-ENV:Body>
</SOAP-ENV:Envelope>
)";
const std::string mediaprofilexml = R"(      <trt:Profiles fixed="true" token="%s">
        <tt:Name>%s</tt:Name>
        <tt:VideoSourceConfiguration token="VSC_%d">
          <tt:Name>VideoSourceConfig_%d</tt:Name>
          <tt:UseCount>1</tt:UseCount>
          <tt:SourceToken>VideoSource_1</tt:SourceToken>
          <tt:Bounds height="%d" width="%d" y="0" x="0"/>
        </tt:VideoSourceConfiguration>
        <tt:VideoEncoderConfiguration token="VEC_%d">
          <tt:Name>VideoEncoderConfig_%d</tt:Name>
          <tt:UseCount>1</tt:UseCount>
          <tt:Encoding>%s</tt:Encoding>
          <tt:Resolution>
            <tt:Width>%d</tt:Width>
            <tt:Height>%d</tt:Height>
          </tt:Resolution>
          <tt:Quality>5</tt:Quality>
          <tt:RateControl>
            <tt:FrameRateLimit>%d</tt:FrameRateLimit>
            <tt:EncodingInterval>1</tt:EncodingInterval>
            <tt:BitrateLimit>%d</tt:BitrateLimit>
          </tt:RateControl>
        </tt:VideoEncoderConfiguration>
      </trt:Profiles>
)";
const std::string mediaprofilesxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:trt="http://www.onvif.org/ver10/media/wsdl" xmlns:tt="http://www.onvif.org/ver10/schema">
  <SOAP-ENV:Body>
    <trt:GetProfilesResponse>
%s
    </trt:GetProfilesResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";
const std::string snapshotxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:trt="http://www.onvif.org/ver10/media/wsdl" xmlns:tt="http://www.onvif.org/ver10/schema">
  <SOAP-ENV:Body>
    <trt:GetSnapshotUriResponse>
      <trt:MediaUri>
        <tt:Uri>%s</tt:Uri>
        <tt:InvalidAfterConnect>false</tt:InvalidAfterConnect>
        <tt:InvalidAfterReboot>false</tt:InvalidAfterReboot>
        <tt:Timeout>PT0S</tt:Timeout>
      </trt:MediaUri>
    </trt:GetSnapshotUriResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";
const std::string streamxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:trt="http://www.onvif.org/ver10/media/wsdl" xmlns:tt="http://www.onvif.org/ver10/schema">
  <SOAP-ENV:Body>
    <trt:GetStreamUriResponse>
      <trt:MediaUri>
        <tt:Uri>%s</tt:Uri>
        <tt:InvalidAfterConnect>false</tt:InvalidAfterConnect>
        <tt:InvalidAfterReboot>false</tt:InvalidAfterReboot>
        <tt:Timeout>PT0S</tt:Timeout>
      </trt:MediaUri>
    </trt:GetStreamUriResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";
const std::string systemtimexml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:tds="http://www.onvif.org/ver10/device/wsdl">
  <SOAP-ENV:Body>
    <tds:GetSystemDateAndTimeResponse>
      <tds:SystemDateAndTime>
        <tds:DateTimeType>NTP</tds:DateTimeType>
        <tds:DaylightSavings>false</tds:DaylightSavings>
        <tds:TimeZone>
          <tds:TZ>UTC</tds:TZ>
        </tds:TimeZone>
        <tds:UTCDateTime>
          <tds:Time>
            <tds:Hour>%02d</tds:Hour>
            <tds:Minute>%02d</tds:Minute>
            <tds:Second>%02d</tds:Second>
          </tds:Time>
          <tds:Date>
            <tds:Year>%d</tds:Year>
            <tds:Month>%02d</tds:Month>
            <tds:Day>%02d</tds:Day>
          </tds:Date>
        </tds:UTCDateTime>
      </tds:SystemDateAndTime>
    </tds:GetSystemDateAndTimeResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";
const std::string videosourcesxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:tt="http://www.onvif.org/ver10/schema" xmlns:tds="http://www.onvif.org/ver10/device/wsdl" xmlns:trt="http://www.onvif.org/ver10/media/wsdl">
  <SOAP-ENV:Body>
    <trt:GetVideoSourcesResponse>
      <trt:VideoSources>
        <tt:VideoSource token="VideoSource_1">
          <tt:Framerate>%d</tt:Framerate>
          <tt:Resolution>
            <tt:Width>%d</tt:Width>
            <tt:Height>%d</tt:Height>
          </tt:Resolution>
          <tt:Imaging>
            <tt:BacklightCompensation>
              <tt:Mode>OFF</tt:Mode>
              <tt:Level>0</tt:Level>
            </tt:BacklightCompensation>
            <tt:Brightness>50</tt:Brightness>
            <tt:ColorSaturation>50</tt:ColorSaturation>
            <tt:Contrast>50</tt:Contrast>
            <tt:Exposure>
              <tt:Mode>AUTO</tt:Mode>
              <tt:MinExposureTime>10</tt:MinExposureTime>
              <tt:MaxExposureTime>30000</tt:MaxExposureTime>
              <tt:Gain>0</tt:Gain>
              <tt:MinGain>0</tt:MinGain>
              <tt:MaxGain>100</tt:MaxGain>
            </tt:Exposure>
            <tt:Focus>
              <tt:AutoFocusMode>AUTO</tt:AutoFocusMode>
              <tt:DefaultSpeed>1</tt:DefaultSpeed>
              <tt:NearLimit>0</tt:NearLimit>
              <tt:FarLimit>0</tt:FarLimit>
            </tt:Focus>
            <tt:WhiteBalance>
              <tt:Mode>AUTO</tt:Mode>
              <tt:CrGain>0</tt:CrGain>
              <tt:CbGain>0</tt:CbGain>
            </tt:WhiteBalance>
          </tt:Imaging>
        </tt:VideoSource>
      </trt:VideoSources>
    </trt:GetVideoSourcesResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";

const std::string helloxml = R"(<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope"
                   xmlns:SOAP-ENC="http://www.w3.org/2003/05/soap-encoding"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xmlns:xsd="http://www.w3.org/2001/XMLSchema"
                   xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing"
                   xmlns:wsdd="http://schemas.xmlsoap.org/ws/2005/04/discovery"
                   xmlns:tdn="http://www.onvif.org/ver10/network/wsdl">
    <SOAP-ENV:Header>
        <wsa:MessageID>uuid:%s</wsa:MessageID>
        <wsa:ReplyTo SOAP-ENV:mustUnderstand="true">
            <wsa:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:Address>
        </wsa:ReplyTo>
        <wsa:To SOAP-ENV:mustUnderstand="true">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>
        <wsa:Action SOAP-ENV:mustUnderstand="true">http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello</wsa:Action>
        <wsdd:AppSequence InstanceId="0"
                          MessageNumber="%s"/>
    </SOAP-ENV:Header>
    <SOAP-ENV:Body>
        <wsdd:Hello>
            <wsa:EndpointReference>
                <wsa:Address>urn:uuid:%s</wsa:Address>
            </wsa:EndpointReference>
            <wsdd:Types>tdn:NetworkVideoTransmitter</wsdd:Types>
            <wsdd:Scopes>onvif://www.onvif.org/type/video_encoder onvif://www.onvif.org/type/audio_encoder onvif://www.onvif.org/type/ptz onvif://www.onvif.org/hardware/%s onvif://www.onvif.org/name/%s onvif://www.onvif.org/location/anywhere</wsdd:Scopes>
            <wsdd:XAddrs>%s</wsdd:XAddrs>
            <wsdd:MetadataVersion>0</wsdd:MetadataVersion>
        </wsdd:Hello>
    </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";
const std::string badauthxml = R"(<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:SOAP-ENC="http://www.w3.org/2003/05/soap-encoding" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">
    <SOAP-ENV:Header></SOAP-ENV:Header>
    <SOAP-ENV:Body>
        <SOAP-ENV:Fault>
            <SOAP-ENV:Code>
                <SOAP-ENV:Value>SOAP-ENV:Sender</SOAP-ENV:Value>
            </SOAP-ENV:Code>
            <SOAP-ENV:Reason>
                <SOAP-ENV:Text>Not Authorized</SOAP-ENV:Text>
            </SOAP-ENV:Reason>
        </SOAP-ENV:Fault>
    </SOAP-ENV:Body>
</SOAP-ENV:Envelope>)";

#endif //ONVIFXML_H
