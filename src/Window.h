#pragma once

#include <SDL.h>

class Window
{
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void updateEvent(SDL_Event e) = 0;
    virtual void destroy() = 0;
    virtual bool markedForDeletion() = 0;
};