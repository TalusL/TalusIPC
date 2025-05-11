#include <iostream>

#include "Util/logger.h"
#include "media.h"
#include "Network/TcpServer.h"
#include "Rtsp/RtspSession.h"


using namespace std;
using namespace toolkit;


int main() {

    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel", LTrace));
    Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());


    hal_identify();

    if (!*family)
        ErrorL<<"Unsupported chip family! Quitting...";


    auto rtspSrv = std::make_shared<TcpServer>();
    rtspSrv->start<mediakit::RtspSession>(554);


    if (start_sdk()) {
        ErrorL<< "Failed to start SDK!";
        return -1;
    }



    static toolkit::semaphore sem;
    signal(SIGINT, [](int) {
        InfoL << "SIGINT:exit";
        signal(SIGINT, SIG_IGN); // 设置退出信号
        sem.post();
    }); // 设置退出信号

    sem.wait();

    stop_sdk();

    sleep(2);
    return 0;
}
