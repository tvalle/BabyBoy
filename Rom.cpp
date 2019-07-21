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

uint8_t Rom::fetchByte(int position)
{
    //TODO: control length maybe

    if (romBuffer == 0)
        return 0;

    return romBuffer[position];
}
