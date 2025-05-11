#include <iostream>

#include "Util/logger.h"
#include "media.h"


using namespace std;
using namespace toolkit;


int main() {

    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel", LTrace));
    Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

    hal_identify();

    if (!*family)
        HAL_ERROR("hal", "Unsupported chip family! Quitting...\n");
    if (start_sdk())
        HAL_ERROR("hal", "Failed to start SDK!\n");

    static toolkit::semaphore sem;
    signal(SIGINT, [](int) {
        InfoL << "SIGINT:exit";
        signal(SIGINT, SIG_IGN); // 设置退出信号
        sem.post();
    }); // 设置退出信号

    sem.wait();
    return 0;
}
