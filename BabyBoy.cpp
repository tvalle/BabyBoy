#include "SDL.h"
#include "Rom.h"
#include "CPU.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "BabyBoy",
        100, 100,
        160, 144,
        SDL_WINDOW_OPENGL);

    Rom rom = Rom("dmg0_rom.bin");
    CPU gbCpu = CPU(rom);

    SDL_Event e;

    bool isDebuggingMode = false;

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

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_ESCAPE])
        {
            exit = true;
        }

        gbCpu.ExecuteInstruction(gbCpu.RAM[gbCpu.PC]);
        if (!isDebuggingMode)
        {
            /*SDL_Delay(1);*/
        }
        else
        {
            while (SDL_PollEvent(&e) == 0 || e.type != SDL_KEYDOWN || (e.key.keysym.sym != SDLK_F10 && e.key.repeat == 0))
            {
                
            }
        }

    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}