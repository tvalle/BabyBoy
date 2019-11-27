#include "SDL.h"
#include "Rom.h"
#include "CPU.h"
#include <stdio.h>

int oldValue = -1;
int cpuCycleCount = 0;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "BabyBoy",
        100, 100,
        160, 144,
        SDL_WINDOW_OPENGL);

    Rom rom = Rom("tetris.gb");
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

		if (gbCpu.RAM[256] != oldValue)
		{
			oldValue = gbCpu.RAM[256];
		}

		//if (cpuCycleCount == 12)

        gbCpu.ExecuteInstruction(gbCpu.RAM[gbCpu.PC]);
		cpuCycleCount++;
        if (!isDebuggingMode)
        {
            /*SDL_Delay(1);*/
        }
        else if(gbCpu.PC >= 0x0C)
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