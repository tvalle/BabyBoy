#include "WindowManager.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Usage: BabyBoy rom.gb\n");
        return 0;
    }

    WindowManager windowManager = WindowManager(std::string(argv[1]));

    windowManager.run();

    return 0;
}
