#pragma once

#include "../SDL/SDLWindow.h"
#include "../Window.h"
#include "../SoC.h"

class VRAMWindow : public Window {
public:
    VRAMWindow(SoC *soc);

    void init() override;
    void update() override;
    void destroy() override;

private:
    SDL_Event e;
    SDLWindow window;

    SoC *m_Soc;
};