#include <stdio.h>
#include "Rom.h"
#include "SoC.h"
#include "SDLWindow.h"

int main(int argc, char* argv[])
{
    Rom rom = Rom("tetris.gb");

    SoC soc = SoC(rom);

    SDL_Init(SDL_INIT_VIDEO);

    SDLWindow window = SDLWindow();
    window.init("BabyBoy", 256, 256);

    auto matrix = soc.ram.getVRAM_Tiles();

    /*while (soc.isRunning)
    {
        soc.step();
    }*/

    SDL_Event e;

    bool exit = false;
    while (!exit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                exit = true;
            }
        }

        window.handleEvent(e);

        window.render(matrix, 128, 192);
    }

    window.free();
    SDL_Quit();
    return 0;
}