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
    window.init("BabyBoy", 160, 144, true);

    SDLWindow bgMap = SDLWindow();
    bgMap.init("BabyBoy", 256, 256, false);

    SDLWindow vramWindow = SDLWindow();
    vramWindow.init("BabyBoy", 128, 192, false);

    soc.ram.write8(0xFF47, 0xE4);

    //auto vramMatrix = soc.ram.getVRAM_Tiles();
    auto bgMatrix = soc.ram.getBGTileMapMatrix();


    /*while (soc.isRunning)
    {
        soc.step();
    }*/

    SDL_Event e;

    uint8_t scx = 0x00;
    uint8_t scy = 0x00;

    bool exit = false;
    while (!exit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                exit = true;
            }

            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    scx++;
                    break;
                case SDLK_LEFT:
                    scx--;
                    break;
                case SDLK_UP:
                    scy--;
                    break;
                case SDLK_DOWN:
                    scy++;
                    break;
                default:
                    break;
                }
            }
        }

        /*vramWindow.handleEvent(e);
        vramWindow.render(vramMatrix, 128, 192);*/

        window.handleEvent(e);
        window.renderWrapping(bgMatrix, scx, scy);
        //bgMap.render(bgMatrix, 256, 256);
    }
    window.free();
    bgMap.free();
    vramWindow.free();
    SDL_Quit();
    return 0;
}