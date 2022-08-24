#pragma once
#include <string>

struct Opcode
{
public:

	std::string mnemonic;
	uint8_t opcode;
	short int bytes;
	uint16_t address;

	Opcode()
	{
		
	}

	Opcode(std::string _mnemonic, uint8_t _opcode, short int _bytes, uint16_t _address)
	{
		mnemonic = _mnemonic;
		opcode = _opcode;
		bytes = _bytes;
		address = _address;
	}
};
