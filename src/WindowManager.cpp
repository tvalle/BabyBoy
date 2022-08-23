#include <vector>
#include <chrono>
#include <algorithm>
#include <SDL.h>

#include "WindowManager.h"
#include "windows/MainWindowDesktop.h"

WindowManager::WindowManager()
{
    exit = false;
}

void WindowManager::run()
{
    MainWindowDesktop mainWindow = MainWindowDesktop(this);
    m_Windows.push_back(&mainWindow);

    initializeWindows(m_Windows);

    auto last_timepoint = std::chrono::steady_clock::now();

    while (!exit)
    {
        auto current_timepoint = std::chrono::steady_clock::now();

        updateWindows(m_Windows);

        while (current_timepoint - last_timepoint < std::chrono::milliseconds(16))
        {
            current_timepoint = std::chrono::steady_clock::now();
        }
        last_timepoint = current_timepoint;
    }

    destroyWindows(m_Windows);
}

void WindowManager::add(Window *window)
{
    if (std::find(m_Windows.begin(), m_Windows.end(), window) == m_Windows.end())
    {
        window->init();
        m_Windows.push_back(window);
    }
}

void WindowManager::remove(Window *window)
{
   m_Windows.erase(std::remove(m_Windows.begin(), m_Windows.end(), window), m_Windows.end());
}

void WindowManager::initializeWindows(std::vector<Window *> windows)
{
    for (int i = 0; i < windows.size(); i++)
    {
        windows[i]->init();
    }
}

void WindowManager::updateWindows(std::vector<Window *> windows)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        for (int i = 0; i < windows.size(); i++)
        {
            windows[i]->updateEvent(event);
        }
    }

    for (int i = 0; i < windows.size(); i++)
    {
        windows[i]->update();
    }
}

void WindowManager::destroyWindows(std::vector<Window *> windows)
{
    for (int i = 0; i < windows.size(); i++)
    {
        windows[i]->destroy();
    }
}