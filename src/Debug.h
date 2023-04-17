#pragma once
#include <vector>

#define BREAKPOINT_SIZE 0xFFFF

class Debug
{
protected:
    Debug();

    static Debug* instance;

public:

    bool isPaused = false;

    static Debug* GetInstance();
    std::vector<int> addrWatchlist;

    bool breakpoints[BREAKPOINT_SIZE];
};