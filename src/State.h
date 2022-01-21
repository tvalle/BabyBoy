#pragma once

#include <vector>

#include "Window.h"

class State
{
public:
    bool exit;

    State();
    void run();

private:
    std::vector<Window *> m_Windows;

    void initializeWindows(std::vector<Window *> windows);
    void updateWindows(std::vector<Window *> windows);
    void destroyWindows(std::vector<Window *> windows);
};