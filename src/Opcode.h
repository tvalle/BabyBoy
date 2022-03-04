#pragma once
#include <string>

struct Opcode
{
public:

	std::string mnemonic;
	uint8_t opcode;
	short int bytes;
	std::string operands;

	Opcode(std::string _mnemonic, uint8_t _opcode, short int _bytes, std::string _operands)
	{
		mnemonic = _mnemonic;
		opcode = _opcode;
		bytes = _bytes;
		operands = _operands;
	}
};
