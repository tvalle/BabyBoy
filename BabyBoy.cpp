#include <stdio.h>
#include "Rom.h"
#include "SoC.h"

int main(int argc, char* argv[])
{
    Rom rom = Rom("tetris.gb");

    SoC soc = SoC(rom);

    while (soc.isRunning)
    {
        soc.step();
    }

    return 0;
}