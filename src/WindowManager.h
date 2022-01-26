#pragma once

#include <vector>

#include "Window.h"

class WindowManager
{
public:
    bool exit;

    WindowManager();
    void run();
    void add(Window *window);

private:
    std::vector<Window *> m_Windows;

    void initializeWindows(std::vector<Window *> windows);
    void updateWindows(std::vector<Window *> windows);
    void destroyWindows(std::vector<Window *> windows);
};