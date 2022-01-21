#include <vector>
#include <chrono>

#include "State.h"
#include "windows/MainWindowDesktop.h"

State::State()
{
    exit = false;
}

void State::run()
{
    MainWindowDesktop mainWindow = MainWindowDesktop(this);
    m_Windows.push_back(&mainWindow);

    initializeWindows(m_Windows);

    auto last_timepoint = std::chrono::steady_clock::now();

    while (!exit)
    {
        auto current_timepoint = std::chrono::steady_clock::now();

        while (current_timepoint - last_timepoint < std::chrono::milliseconds(16))
        {
            updateWindows(m_Windows);
            current_timepoint = std::chrono::steady_clock::now();
        }
        last_timepoint = current_timepoint;
    }

    destroyWindows(m_Windows);
}

void State::initializeWindows(std::vector<Window *> windows)
{
    for (int i = 0; i < windows.size(); i++)
    {
        windows[i]->init();
    }
}

void State::updateWindows(std::vector<Window *> windows)
{
    for (int i = 0; i < windows.size(); i++)
    {
        windows[i]->update();
    }
}

void State::destroyWindows(std::vector<Window *> windows)
{
    for (int i = 0; i < windows.size(); i++)
    {
        windows[i]->destroy();
    }
}