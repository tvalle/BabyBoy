#include <iostream>

#include "MainWindowDesktop.h"

MainWindowDesktop::MainWindowDesktop(WindowManager *windowManager, std::string romName)
{
    m_WindowManager = windowManager;
    m_VRAMWindow = nullptr;

    m_RomName = romName;
}

void MainWindowDesktop::init()
{
    // SDL
    SDL_Init(SDL_INIT_VIDEO);

    window = SDLWindow();
    window.init("BabyBoy", 160, 144, true);

    // Emulator
    m_Rom = new Rom(m_RomName);

    m_Soc = new SoC(*m_Rom);
    m_Soc->ram.write8(0xFF47, 0xE4);
}

void MainWindowDesktop::update()
{
    if (!m_Soc->isPaused)
    {
        while (m_Soc->cpu.cycles < 69905)
        {
            m_Soc->step();
        }
    }

    // This is outside in order to step through in the debugger, otherwise it would be just after the while loop
    if (m_Soc->cpu.cycles >= 69905)
    {
        m_Soc->cpu.cycles = 0;
    }

    window.handleEvent(e);
    auto bgMatrix = m_Soc->ram.getBGTileMapMatrix();
    window.render(bgMatrix, 160, 144);
}

void MainWindowDesktop::updateEvent(SDL_Event e)
{
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == window.getWindowId())
    {
        m_WindowManager->exit = true;
    }

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
            m_VRAMWindow = new VRAMWindow(m_Soc, m_WindowManager);
            m_WindowManager->add(m_VRAMWindow);
        }
        break;
        case SDLK_1:
        {
            m_DebugWindow = new DebugWindow(m_Soc, m_WindowManager);
            m_WindowManager->add(m_DebugWindow);
        }
        break;
        default:
            break;
        }
    }
}

void MainWindowDesktop::destroy()
{
    window.free();
    SDL_Quit();
}

bool MainWindowDesktop::markedForDeletion()
{
    return false;
}