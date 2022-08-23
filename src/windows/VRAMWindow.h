#pragma once

#include "../SDL/SDLWindow.h"
#include "../Window.h"
#include "../WindowManager.h"
#include "../SoC.h"

class VRAMWindow : public Window {
public:
    VRAMWindow(SoC *soc, WindowManager *windowManager);

    void init() override;
    void update() override;
    void updateEvent(SDL_Event e) override;
    void destroy() override;

private:
    SDLWindow window;

    SoC *m_Soc;
    WindowManager *m_WindowManager;
};