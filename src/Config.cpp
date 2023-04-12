#include "Config.h"

// TODO: Implement the configuration from a config file

Config* Config::instance = nullptr;;

Config::Config()
{
}

Config* Config::GetInstance()
{
    if (instance == nullptr) {
        instance = new Config();
    }
    
    return instance;
}

bool Config::HasBoot()
{
    return false;
}

std::string Config::GetBootFileName()
{
    return "dmg0_rom.gb";
}
