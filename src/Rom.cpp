#include "Rom.h"
#include <fstream>
#include <iterator>
#include <map>

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

        // Get ROM name
        std::string romName;
        for (int i = 0x0134; i <= 0x143; i++) {
            romName.insert(i-0x134, 1, romBuffer[i]);
        }

        // Get cartridge type (what it supports, mappers, ram, battery)
        std::map<uint8_t, std::string> cartridgeType;
        cartridgeType[0x00] = "ROM ONLY";
        cartridgeType[0x01] = "MBC1";
        cartridgeType[0x02] = "MBC1+RAM";
        cartridgeType[0x03] = "MBC1+RAM+BATTERY";
        cartridgeType[0x05] = "MBC2";
        cartridgeType[0x06] = "MBC2+BATTERY";
        cartridgeType[0x08] = "ROM+RAM 1";
        cartridgeType[0x09] = "ROM+RAM+BATTERY 1";
        cartridgeType[0x0B] = "MMM01";
        cartridgeType[0x0C] = "MMM01+RAM";
        cartridgeType[0x0D] = "MMM01+RAM+BATTERY";
        cartridgeType[0x0F] = "MBC3+TIMER+BATTERY";
        cartridgeType[0x10] = "MBC3+TIMER+RAM+BATTERY 2";
        cartridgeType[0x11] = "MBC3";
        cartridgeType[0x12] = "MBC3+RAM 2";
        cartridgeType[0x13] = "MBC3+RAM+BATTERY 2";
        cartridgeType[0x19] = "MBC5";
        cartridgeType[0x1A] = "MBC5+RAM";
        cartridgeType[0x1B] = "MBC5+RAM+BATTERY";
        cartridgeType[0x1C] = "MBC5+RUMBLE";
        cartridgeType[0x1D] = "MBC5+RUMBLE+RAM";
        cartridgeType[0x1E] = "MBC5+RUMBLE+RAM+BATTERY";
        cartridgeType[0x20] = "MBC6";
        cartridgeType[0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
        cartridgeType[0xFC] = "POCKET CAMERA";
        cartridgeType[0xFD] = "BANDAI TAMA5";
        cartridgeType[0xFE] = "HuC3";
        cartridgeType[0xFF] = "HuC1+RAM+BATTERY";

        // Print ROM info
        printf("Loaded ROM\n");
        printf("ROM name: %s\n", romName.c_str());
        printf("Cartridge type: %s\n", cartridgeType[romBuffer[0x147]].c_str());
    }
}
