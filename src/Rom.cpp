#include "Rom.h"
#include <fstream>
#include <iterator>

Rom::Rom()
{
}

Rom::Rom(std::string fileName)
{
    std::ifstream romFile;

    romFile.open(fileName, std::ios::binary);
    romFile.unsetf(std::ios::skipws);

    if (romFile.is_open())
    {
        romFile.seekg(0, romFile.end);
        int size = romFile.tellg();
        romFile.seekg(0, romFile.beg);

        romBuffer.reserve(size);

        romBuffer.insert(romBuffer.begin(),
                         std::istream_iterator<uint8_t>(romFile),
                         std::istream_iterator<uint8_t>());

        romFile.close();
    }
}
