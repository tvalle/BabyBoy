#include "Rom.h"
#include <fstream>

Rom::Rom(std::string fileName)
{
    romBuffer = 0;

    std::ifstream romFile;

    romFile.open(fileName, std::ios::binary);

    if (romFile.is_open())
    {
        romFile.seekg(0, romFile.end);
        int size = romFile.tellg();
        romFile.seekg(0, romFile.beg);

        romBuffer = new char[size];

        romFile.read(romBuffer, size);
        romFile.close();
    }
}
