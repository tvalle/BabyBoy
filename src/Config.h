#pragma once
#include <string>

class Config
{
protected:
    Config();

    static Config* instance;

public:

    static Config* GetInstance();

    bool HasBoot();

    std::string GetBootFileName();
};