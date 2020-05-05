#include <stdio.h>
#include <SDL.h>
#include "Rom.h"
#include "SoC.h"
#include "SDLWindow.h"


int main(int argc, char* argv[])
{
    Rom rom = Rom("tetris.gb");

    SoC soc = SoC(rom);

    SDL_Init(SDL_INIT_VIDEO);

    SDLWindow window = SDLWindow();
    window.init("BabyBoy", 160, 144, true);

    SDLWindow bgMap = SDLWindow();
    bgMap.init("BabyBoy", 256, 256, false);

    SDLWindow vramWindow = SDLWindow();
    vramWindow.init("BabyBoy", 128, 192, false);

    soc.ram.write8(0xFF47, 0xE4);

    SDL_Event e;

    Uint32 timer = 0;

    int frames = 0;
    int seconds = 0;

    bool exit = false;
    while (!exit)
    {
        timer = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                exit = true;
            }
        }

        soc.step();

        //auto vramMatrix = soc.ram.getVRAM_Tiles();

        //vramWindow.handleEvent(e);
        //vramWindow.render(vramMatrix, 128, 192);

        /*auto bgMatrix = soc.ram.getBGTileMapMatrix();

        window.handleEvent(e);
        window.renderWrapping(bgMatrix, 0x00, 0x00);*/
        //bgMap.render(bgMatrix, 256, 256);

        while (SDL_GetTicks() - timer < 16.666)
        {
        }

        frames++;
        if (frames >= 60)
        {
            frames = 0;
            printf("%d\n", ++seconds);
        }

        exit = !soc.isRunning;
    }
    window.free();
    bgMap.free();
    vramWindow.free();
    SDL_Quit();
    return 0;
}