#include <iostream>

#include "MainWindowDesktop.h"

MainWindowDesktop::MainWindowDesktop(WindowManager *windowManager)
{
    m_WindowManager = windowManager;
    m_VRAMWindow = nullptr;
}

void MainWindowDesktop::init()
{
    // SDL
    SDL_Init(SDL_INIT_VIDEO);

    window = SDLWindow();
    window.init("BabyBoy", 160, 144, true);

    // Emulator
    m_Rom = new Rom("cpu_instrs.gb");

    m_Soc = new SoC(*m_Rom);
    m_Soc->ram.write8(0xFF47, 0xE4);
}

void MainWindowDesktop::update()
{
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            m_WindowManager->exit = true;
        }

        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_2:
            {
                m_VRAMWindow = new VRAMWindow(m_Soc);
                m_WindowManager->add(m_VRAMWindow);
            }
            break;
            case SDLK_1:
            {
                m_DebugWindow = new DebugWindow(m_Soc);
                m_WindowManager->add(m_DebugWindow);
            }
            break;
            default:
                break;
            }
        }
    }

    while (m_Soc->cpu.cycles < 69905)
    {
        m_Soc->step();
    }
    m_Soc->cpu.cycles = 0;

    window.handleEvent(e);
    auto bgMatrix = m_Soc->ram.getBGTileMapMatrix();
    window.render(bgMatrix, 160, 144);
}

void MainWindowDesktop::destroy()
{
    window.free();
    SDL_Quit();
}