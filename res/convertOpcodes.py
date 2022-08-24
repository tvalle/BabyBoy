#!/usr/bin/env python3

import json
from lib2to3.pytree import convert
from operator import le

### Variables

opcodes_file_name = "opcodes.json"
unprefixed_file_name = "unprefixed.h"
cbprefixed_file_name = "cb_prefixed.h"

###

def intersection(lst1, lst2):
    return list(set(lst1) & set(lst2))

def getOperands(operands):
    return ' '.join(list(map(lambda x: x["name"], operands)))

def writeToFile(file, opcodes_list):
    mnemonic = opcodes_list[opcode]["mnemonic"]
    bytes = opcodes_list[opcode]["bytes"]
    operands = opcodes_list[opcode]["operands"]

    file.write("case " + opcode + ": {\n")
    file.write(f'    instruction = Opcode("{mnemonic}", {opcode}, {bytes}, n);\n')

    opstring = ""
    # append operand
    if len(operands) > 0:
        operands_as_string = list(map(lambda x: x["name"], operands))
        if intersection(["d8", "d16", "a16"], operands_as_string) == []:
            opstring = ' '.join(operands_as_string)
            file.write(f'    instruction.mnemonic.append(" {opstring}");\n')
        else:
            for o in operands:
                par_start =  "" if "immediate" in o and o["immediate"] == True else "("
                par_end =  "" if "immediate" in o and o["immediate"] == True else ")"

                if o["name"] not in ["d8", "d16", "a16"]:
                    opstring += o["name"]
                    opstring = par_start + opstring
                    opstring += par_end
                else:
                    file.write(f'    char addr[{3 if o["name"] == "d8" else 5} + {len(par_start)+len(par_end)}];\n')
                    if o["name"] == "d8":
                        file.write(f'    sprintf(addr, " {opstring} %02X", m_Soc->ram.read(n+1));\n')
                    else:
                        file.write(f'    sprintf(addr, " {opstring} %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));\n')
                    file.write('    instruction.mnemonic.append(addr);\n')

    file.write("}\n")
    file.write("break;\n")


json_file = open(opcodes_file_name)
opcodes_data = json.load(json_file)
json_file.close()

unprefixed = opcodes_data["unprefixed"]
prefixed = opcodes_data["cbprefixed"]

with open(unprefixed_file_name, "w") as unprefixed_file:
    for opcode in unprefixed:
        if opcode == "0xCB":
            continue

        writeToFile(unprefixed_file, unprefixed)

with open(cbprefixed_file_name, "w") as cbprefixed_file:
    for opcode in prefixed:
        writeToFile(cbprefixed_file, prefixed)