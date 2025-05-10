#include <iostream>
extern "C"{
#include "media.h"
}

int main() {
    hal_identify();

    if (!*family)
        HAL_ERROR("hal", "Unsupported chip family! Quitting...\n");
    if (start_sdk())
        HAL_ERROR("hal", "Failed to start SDK!\n");


    sleep(10000);
    return 0;
}
