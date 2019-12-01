#include <stdio.h>
#include "SDL.h"
#include "Rom.h"
#include "CPU.h"
#include "SDLWindow.h"

#define TOTAL_WINDOWS 2

SDLWindow windows[TOTAL_WINDOWS];

int oldValue = -1;
int cpuCycleCount = 0;

uint8_t debuggerMatrix[32][128] = { 1 };

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	windows[0].init("BabyBoy", 160, 144);
	windows[1].init("VRam Viewer", 128, 344, false);

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

			//Handle window events 
			for (int i = 0; i < TOTAL_WINDOWS; ++i)
			{
				windows[i].handleEvent(e);
			}

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_1:
					windows[1].focus();
					break;
				default:
					break;
				}
			}
		}

		// TODO: Move this somewhere (after this is working separate debugger into a class)
		// also separate SDL from CPU/Memory/Debugger architecture
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				uint8_t pixel = (gbCpu.RAM[0x8010 + (i * 2)] >> (7 - j)) & 1;
				pixel = pixel << 1;
				pixel = pixel | ((gbCpu.RAM[0x8011 + (i * 2)] >> (7 - j)) & 1);
				debuggerMatrix[i][j] = pixel;
			}
		}

		//Update game window
		windows[0].render(nullptr, 0, 0);
		//Update debug window
		windows[1].render(*debuggerMatrix, 128, 32);

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			exit = true;
		}

		if (gbCpu.RAM[256] != oldValue)
		{
			oldValue = gbCpu.RAM[256];
		}

		gbCpu.ExecuteInstruction(gbCpu.RAM[gbCpu.PC]);
		cpuCycleCount++;
		if (!isDebuggingMode)
		{
			/*SDL_Delay(1);*/
		}
		else if (gbCpu.PC >= 0x0C)
		{
			while (SDL_PollEvent(&e) == 0 || e.type != SDL_KEYDOWN || (e.key.keysym.sym != SDLK_F10 && e.key.repeat == 0))
			{

			}
		}

	}

	windows[0].free();
	windows[1].free();
	SDL_Quit();
	return 0;
}