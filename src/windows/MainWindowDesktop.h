#pragma once

#include "../SDL/SDLWindow.h"
#include "../Window.h"
#include "../State.h"
#include "../SoC.h"
#include "../Rom.h"

class MainWindowDesktop : public Window
{
public:
    MainWindowDesktop(State *state);

    void init() override;
    void update() override;
    void destroy() override;

private:
    State *m_State;
    SDL_Event e;
    SDLWindow window;

    Rom *m_Rom;
    SoC *m_Soc;
};