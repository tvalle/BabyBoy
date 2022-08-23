#include "VRAMWindow.h"

VRAMWindow::VRAMWindow(SoC *soc, WindowManager *windowManager)
{
    m_Soc = soc;
    m_WindowManager = windowManager;
}

void VRAMWindow::init()
{
    window = SDLWindow();
    window.init("VRAM", 128, 192, true);
}

void VRAMWindow::update()
{
    auto vramMatrix = m_Soc->ram.getVRAM_Tiles();
    window.render(vramMatrix, 128, 192);
}

void VRAMWindow::updateEvent(SDL_Event e)
{
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == window.getWindowId())
    {
        destroy();
        m_WindowManager->remove(this);
        return;
    }

    window.handleEvent(e);
}

void VRAMWindow::destroy()
{
    window.free();
}