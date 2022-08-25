#pragma once

#include <vector>
#include <string>

#include "Window.h"

class WindowManager
{
public:
    bool exit;

    WindowManager(std::string romName);
    void run();
    void add(Window *window);
    void remove(Window *window);

private:
    std::vector<Window *> m_Windows;
    std::string m_RomName;

    void initializeWindows(std::vector<Window *> windows);
    void updateWindows(std::vector<Window *> windows);
    void destroyWindows(std::vector<Window *> windows);
};