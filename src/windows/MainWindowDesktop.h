#pragma once

#include "../SDL/SDLWindow.h"
#include "../Window.h"
#include "../WindowManager.h"
#include "../SoC.h"
#include "../Rom.h"
#include "VRAMWindow.h"

class MainWindowDesktop : public Window
{
public:
    MainWindowDesktop(WindowManager *windowManager);

    void init() override;
    void update() override;
    void destroy() override;

private:
    WindowManager *m_WindowManager;
    SDL_Event e;
    SDLWindow window;

    Rom *m_Rom;
    SoC *m_Soc;

    VRAMWindow *m_VRAMWindow;
};