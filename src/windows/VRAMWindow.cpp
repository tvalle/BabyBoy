#include "VRAMWindow.h"

VRAMWindow::VRAMWindow(SoC *soc)
{
    m_Soc = soc;
}

void VRAMWindow::init()
{
    window = SDLWindow();
    window.init("VRAM", 128, 192, true);
}

void VRAMWindow::update()
{
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            window.free();
            return;
        }
    }

    window.handleEvent(e);
    auto vramMatrix = m_Soc->ram.getVRAM_Tiles();
    window.handleEvent(e);
    window.render(vramMatrix, 128, 192);
}

void VRAMWindow::destroy()
{
    // TODO: check if needed to free here
}