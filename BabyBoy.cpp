#include <stdio.h>
#include "Rom.h"
#include "SoC.h"
#include "SDLWindow.h"

int main(int argc, char* argv[])
{
    Rom rom = Rom("tetris.gb");

    SoC soc = SoC(rom);

    SDL_Init(SDL_INIT_VIDEO);

    
    
    SDLWindow window= SDLWindow();
    window.init("BabyBoy", 160, 144, false);
    
    SDLWindow bgMap = SDLWindow();
    bgMap.init("BabyBoy", 256, 256, true);
    
    SDLWindow vramWindow = SDLWindow();
    vramWindow.init("BabyBoy", 128, 192, false);

    //auto vramMatrix = soc.ram.getVRAM_Tiles();
    auto bgMatrix = soc.ram.getBGTileMapMatrix();


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

        /*vramWindow.handleEvent(e);
        vramWindow.render(vramMatrix, 128, 192);*/

        bgMap.handleEvent(e);
        bgMap.render(bgMatrix, 256, 256);
    }
    window.free();
    bgMap.free();
    vramWindow.free();
    SDL_Quit();
    return 0;
}