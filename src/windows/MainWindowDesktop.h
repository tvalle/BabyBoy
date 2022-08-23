#pragma once

#include "../SDL/SDLWindow.h"
#include "../Window.h"
#include "../WindowManager.h"
#include "../SoC.h"
#include "../Rom.h"

#include "VRAMWindow.h"
#include "DebugWindow.h"

class MainWindowDesktop : public Window
{
public:
    MainWindowDesktop(WindowManager *windowManager);

    void init() override;
    void update() override;
    void updateEvent(SDL_Event e) override;
    void destroy() override;
    bool markedForDeletion() override;

private:
    WindowManager *m_WindowManager;
    SDL_Event e;
    SDLWindow window;

    Rom *m_Rom;
    SoC *m_Soc;

    VRAMWindow *m_VRAMWindow;
    DebugWindow *m_DebugWindow;
};