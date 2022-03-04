#!/usr/bin/env python3

import json
from lib2to3.pytree import convert

### Variables

opcodes_file_name = "opcodes.json"
output_opcodes_file_name = "Opcode.h"
output_array_file_name = "OpcodesArray.h"

###


def getOperands(operands):
    return ' '.join(list(map(lambda x: x["name"], operands)))

json_file = open(opcodes_file_name)
opcodes_data = json.load(json_file)
json_file.close()

unprefixed = opcodes_data["unprefixed"]
prefixed = opcodes_data["cbprefixed"]

with open(output_opcodes_file_name, "w") as output_opcodes_file:
    output_opcodes_file.write("#pragma once\n")
    output_opcodes_file.write("#include <string>\n\n")
    output_opcodes_file.write("struct Opcode\n")
    output_opcodes_file.write("{\n")
    output_opcodes_file.write("public:\n\n")
    output_opcodes_file.write("\tstd::string mnemonic;\n")
    output_opcodes_file.write("\tuint8_t opcode;\n")
    output_opcodes_file.write("\tshort int bytes;\n")
    output_opcodes_file.write("\tstd::string operands;\n\n")
    output_opcodes_file.write("\tOpcode(std::string _mnemonic, uint8_t _opcode, short int _bytes, std::string _operands)\n")
    output_opcodes_file.write("\t{\n")
    output_opcodes_file.write("\t\tmnemonic = _mnemonic;\n")
    output_opcodes_file.write("\t\topcode = _opcode;\n")
    output_opcodes_file.write("\t\tbytes = _bytes;\n")
    output_opcodes_file.write("\t\toperands = _operands;\n")
    output_opcodes_file.write("\t}\n")
    output_opcodes_file.write("};\n")

with open("OpcodesArray.h", "w") as output_array_file:
    output_array_file.write("#pragma once\n")
    output_array_file.write("#include <unordered_map>\n\n")
    output_array_file.write("std::unordered_map<uint8_t, Opcode> opcodes =\n")
    output_array_file.write("{\n")

    for opcode in unprefixed:
        output_array_file.write(f'{{ {opcode}, Opcode("{unprefixed[opcode]["mnemonic"]}", {opcode}, {int(unprefixed[opcode]["bytes"])}, "{getOperands(unprefixed[opcode]["operands"])}")}},\n')

    output_array_file.write("};\n")

