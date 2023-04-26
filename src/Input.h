#pragma once
#include <SDL.h>

class Input
{
private:
    uint8_t m_keyState;

protected:
    Input();

    static Input* instance;

public:

    static Input* GetInstance();

    bool isAction;
    bool isDirection;

    void handleKeyPress(SDL_Keycode key, bool isKeyDown);

    uint8_t getState();
};