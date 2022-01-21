#pragma once

class Window
{
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void destroy() = 0;
};