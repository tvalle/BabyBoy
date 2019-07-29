#include "SDL.h"
#include "Rom.h"
#include "CPU.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    Rom rom = Rom("Tetris (World).gb");
    CPU gbCpu = CPU(rom);

    bool exit = false;
    while (!exit)
    {
        /*printf("%x ", rom.fetchByte(i));
        if (i % 16 == 0)
            printf("\n");*/

        gbCpu.ExecuteInstruction(gbCpu.RAM[gbCpu.PC]);
    }


    SDL_Window* window = SDL_CreateWindow
    ("An SDL2 window", // window's title
        10, 25, // coordinates on the screen, in pixels, of the window's upper left corner
        640, 480, // window's length and height in pixels  
        SDL_WINDOW_OPENGL);

    SDL_Delay(500); // window lasts 3 seconds
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}