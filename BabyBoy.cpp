#include <stdio.h>
#include "Rom.h"
#include "SoC.h"
#include "SDLWindow.h"

int main(int argc, char* argv[])
{
    Rom rom = Rom("tetris.gb");

    SoC soc = SoC(rom);

    SDLWindow window = SDLWindow();
    window.init("BabyBoy", 160, 144);

    /*soc.ram.getVRAM_Tiles();

    window.render(matrix, 128, 128);*/

    while (soc.isRunning)
    {
        soc.step();
    }

    return 0;
}