case 0x00: {
    instruction = Opcode("NOP", 0x00, 1, n);
}
break;
case 0x01: {
    instruction = Opcode("LD", 0x01, 3, n);
    char addr[5 + 0];
    sprintf(addr, " BC %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x02: {
    instruction = Opcode("LD", 0x02, 1, n);
    instruction.mnemonic.append(" BC A");
}
break;
case 0x03: {
    instruction = Opcode("INC", 0x03, 1, n);
    instruction.mnemonic.append(" BC");
}
break;
case 0x04: {
    instruction = Opcode("INC", 0x04, 1, n);
    instruction.mnemonic.append(" B");
}
break;
case 0x05: {
    instruction = Opcode("DEC", 0x05, 1, n);
    instruction.mnemonic.append(" B");
}
break;
case 0x06: {
    instruction = Opcode("LD", 0x06, 2, n);
    char addr[3 + 0];
    sprintf(addr, " B %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x07: {
    instruction = Opcode("RLCA", 0x07, 1, n);
}
break;
case 0x08: {
    instruction = Opcode("LD", 0x08, 3, n);
    char addr[5 + 2];
    sprintf(addr, "  %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x09: {
    instruction = Opcode("ADD", 0x09, 1, n);
    instruction.mnemonic.append(" HL BC");
}
break;
case 0x0A: {
    instruction = Opcode("LD", 0x0A, 1, n);
    instruction.mnemonic.append(" A BC");
}
break;
case 0x0B: {
    instruction = Opcode("DEC", 0x0B, 1, n);
    instruction.mnemonic.append(" BC");
}
break;
case 0x0C: {
    instruction = Opcode("INC", 0x0C, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0x0D: {
    instruction = Opcode("DEC", 0x0D, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0x0E: {
    instruction = Opcode("LD", 0x0E, 2, n);
    char addr[3 + 0];
    sprintf(addr, " C %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x0F: {
    instruction = Opcode("RRCA", 0x0F, 1, n);
}
break;
case 0x10: {
    instruction = Opcode("STOP", 0x10, 2, n);
    char addr[3 + 0];
    sprintf(addr, "  %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x11: {
    instruction = Opcode("LD", 0x11, 3, n);
    char addr[5 + 0];
    sprintf(addr, " DE %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x12: {
    instruction = Opcode("LD", 0x12, 1, n);
    instruction.mnemonic.append(" DE A");
}
break;
case 0x13: {
    instruction = Opcode("INC", 0x13, 1, n);
    instruction.mnemonic.append(" DE");
}
break;
case 0x14: {
    instruction = Opcode("INC", 0x14, 1, n);
    instruction.mnemonic.append(" D");
}
break;
case 0x15: {
    instruction = Opcode("DEC", 0x15, 1, n);
    instruction.mnemonic.append(" D");
}
break;
case 0x16: {
    instruction = Opcode("LD", 0x16, 2, n);
    char addr[3 + 0];
    sprintf(addr, " D %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x17: {
    instruction = Opcode("RLA", 0x17, 1, n);
}
break;
case 0x18: {
    instruction = Opcode("JR", 0x18, 2, n);
    instruction.mnemonic.append(" r8");
}
break;
case 0x19: {
    instruction = Opcode("ADD", 0x19, 1, n);
    instruction.mnemonic.append(" HL DE");
}
break;
case 0x1A: {
    instruction = Opcode("LD", 0x1A, 1, n);
    instruction.mnemonic.append(" A DE");
}
break;
case 0x1B: {
    instruction = Opcode("DEC", 0x1B, 1, n);
    instruction.mnemonic.append(" DE");
}
break;
case 0x1C: {
    instruction = Opcode("INC", 0x1C, 1, n);
    instruction.mnemonic.append(" E");
}
break;
case 0x1D: {
    instruction = Opcode("DEC", 0x1D, 1, n);
    instruction.mnemonic.append(" E");
}
break;
case 0x1E: {
    instruction = Opcode("LD", 0x1E, 2, n);
    char addr[3 + 0];
    sprintf(addr, " E %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x1F: {
    instruction = Opcode("RRA", 0x1F, 1, n);
}
break;
case 0x20: {
    instruction = Opcode("JR", 0x20, 2, n);
    instruction.mnemonic.append(" NZ r8");
}
break;
case 0x21: {
    instruction = Opcode("LD", 0x21, 3, n);
    char addr[5 + 0];
    sprintf(addr, " HL %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x22: {
    instruction = Opcode("LD", 0x22, 1, n);
    instruction.mnemonic.append(" HL A");
}
break;
case 0x23: {
    instruction = Opcode("INC", 0x23, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0x24: {
    instruction = Opcode("INC", 0x24, 1, n);
    instruction.mnemonic.append(" H");
}
break;
case 0x25: {
    instruction = Opcode("DEC", 0x25, 1, n);
    instruction.mnemonic.append(" H");
}
break;
case 0x26: {
    instruction = Opcode("LD", 0x26, 2, n);
    char addr[3 + 0];
    sprintf(addr, " H %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x27: {
    instruction = Opcode("DAA", 0x27, 1, n);
}
break;
case 0x28: {
    instruction = Opcode("JR", 0x28, 2, n);
    instruction.mnemonic.append(" Z r8");
}
break;
case 0x29: {
    instruction = Opcode("ADD", 0x29, 1, n);
    instruction.mnemonic.append(" HL HL");
}
break;
case 0x2A: {
    instruction = Opcode("LD", 0x2A, 1, n);
    instruction.mnemonic.append(" A HL");
}
break;
case 0x2B: {
    instruction = Opcode("DEC", 0x2B, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0x2C: {
    instruction = Opcode("INC", 0x2C, 1, n);
    instruction.mnemonic.append(" L");
}
break;
case 0x2D: {
    instruction = Opcode("DEC", 0x2D, 1, n);
    instruction.mnemonic.append(" L");
}
break;
case 0x2E: {
    instruction = Opcode("LD", 0x2E, 2, n);
    char addr[3 + 0];
    sprintf(addr, " L %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x2F: {
    instruction = Opcode("CPL", 0x2F, 1, n);
}
break;
case 0x30: {
    instruction = Opcode("JR", 0x30, 2, n);
    instruction.mnemonic.append(" NC r8");
}
break;
case 0x31: {
    instruction = Opcode("LD", 0x31, 3, n);
    char addr[5 + 0];
    sprintf(addr, " SP %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x32: {
    instruction = Opcode("LD", 0x32, 1, n);
    instruction.mnemonic.append(" HL A");
}
break;
case 0x33: {
    instruction = Opcode("INC", 0x33, 1, n);
    instruction.mnemonic.append(" SP");
}
break;
case 0x34: {
    instruction = Opcode("INC", 0x34, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0x35: {
    instruction = Opcode("DEC", 0x35, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0x36: {
    instruction = Opcode("LD", 0x36, 2, n);
    char addr[3 + 0];
    sprintf(addr, " (HL) %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x37: {
    instruction = Opcode("SCF", 0x37, 1, n);
}
break;
case 0x38: {
    instruction = Opcode("JR", 0x38, 2, n);
    instruction.mnemonic.append(" C r8");
}
break;
case 0x39: {
    instruction = Opcode("ADD", 0x39, 1, n);
    instruction.mnemonic.append(" HL SP");
}
break;
case 0x3A: {
    instruction = Opcode("LD", 0x3A, 1, n);
    instruction.mnemonic.append(" A HL");
}
break;
case 0x3B: {
    instruction = Opcode("DEC", 0x3B, 1, n);
    instruction.mnemonic.append(" SP");
}
break;
case 0x3C: {
    instruction = Opcode("INC", 0x3C, 1, n);
    instruction.mnemonic.append(" A");
}
break;
case 0x3D: {
    instruction = Opcode("DEC", 0x3D, 1, n);
    instruction.mnemonic.append(" A");
}
break;
case 0x3E: {
    instruction = Opcode("LD", 0x3E, 2, n);
    char addr[3 + 0];
    sprintf(addr, " A %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0x3F: {
    instruction = Opcode("CCF", 0x3F, 1, n);
}
break;
case 0x40: {
    instruction = Opcode("LD", 0x40, 1, n);
    instruction.mnemonic.append(" B B");
}
break;
case 0x41: {
    instruction = Opcode("LD", 0x41, 1, n);
    instruction.mnemonic.append(" B C");
}
break;
case 0x42: {
    instruction = Opcode("LD", 0x42, 1, n);
    instruction.mnemonic.append(" B D");
}
break;
case 0x43: {
    instruction = Opcode("LD", 0x43, 1, n);
    instruction.mnemonic.append(" B E");
}
break;
case 0x44: {
    instruction = Opcode("LD", 0x44, 1, n);
    instruction.mnemonic.append(" B H");
}
break;
case 0x45: {
    instruction = Opcode("LD", 0x45, 1, n);
    instruction.mnemonic.append(" B L");
}
break;
case 0x46: {
    instruction = Opcode("LD", 0x46, 1, n);
    instruction.mnemonic.append(" B HL");
}
break;
case 0x47: {
    instruction = Opcode("LD", 0x47, 1, n);
    instruction.mnemonic.append(" B A");
}
break;
case 0x48: {
    instruction = Opcode("LD", 0x48, 1, n);
    instruction.mnemonic.append(" C B");
}
break;
case 0x49: {
    instruction = Opcode("LD", 0x49, 1, n);
    instruction.mnemonic.append(" C C");
}
break;
case 0x4A: {
    instruction = Opcode("LD", 0x4A, 1, n);
    instruction.mnemonic.append(" C D");
}
break;
case 0x4B: {
    instruction = Opcode("LD", 0x4B, 1, n);
    instruction.mnemonic.append(" C E");
}
break;
case 0x4C: {
    instruction = Opcode("LD", 0x4C, 1, n);
    instruction.mnemonic.append(" C H");
}
break;
case 0x4D: {
    instruction = Opcode("LD", 0x4D, 1, n);
    instruction.mnemonic.append(" C L");
}
break;
case 0x4E: {
    instruction = Opcode("LD", 0x4E, 1, n);
    instruction.mnemonic.append(" C HL");
}
break;
case 0x4F: {
    instruction = Opcode("LD", 0x4F, 1, n);
    instruction.mnemonic.append(" C A");
}
break;
case 0x50: {
    instruction = Opcode("LD", 0x50, 1, n);
    instruction.mnemonic.append(" D B");
}
break;
case 0x51: {
    instruction = Opcode("LD", 0x51, 1, n);
    instruction.mnemonic.append(" D C");
}
break;
case 0x52: {
    instruction = Opcode("LD", 0x52, 1, n);
    instruction.mnemonic.append(" D D");
}
break;
case 0x53: {
    instruction = Opcode("LD", 0x53, 1, n);
    instruction.mnemonic.append(" D E");
}
break;
case 0x54: {
    instruction = Opcode("LD", 0x54, 1, n);
    instruction.mnemonic.append(" D H");
}
break;
case 0x55: {
    instruction = Opcode("LD", 0x55, 1, n);
    instruction.mnemonic.append(" D L");
}
break;
case 0x56: {
    instruction = Opcode("LD", 0x56, 1, n);
    instruction.mnemonic.append(" D HL");
}
break;
case 0x57: {
    instruction = Opcode("LD", 0x57, 1, n);
    instruction.mnemonic.append(" D A");
}
break;
case 0x58: {
    instruction = Opcode("LD", 0x58, 1, n);
    instruction.mnemonic.append(" E B");
}
break;
case 0x59: {
    instruction = Opcode("LD", 0x59, 1, n);
    instruction.mnemonic.append(" E C");
}
break;
case 0x5A: {
    instruction = Opcode("LD", 0x5A, 1, n);
    instruction.mnemonic.append(" E D");
}
break;
case 0x5B: {
    instruction = Opcode("LD", 0x5B, 1, n);
    instruction.mnemonic.append(" E E");
}
break;
case 0x5C: {
    instruction = Opcode("LD", 0x5C, 1, n);
    instruction.mnemonic.append(" E H");
}
break;
case 0x5D: {
    instruction = Opcode("LD", 0x5D, 1, n);
    instruction.mnemonic.append(" E L");
}
break;
case 0x5E: {
    instruction = Opcode("LD", 0x5E, 1, n);
    instruction.mnemonic.append(" E HL");
}
break;
case 0x5F: {
    instruction = Opcode("LD", 0x5F, 1, n);
    instruction.mnemonic.append(" E A");
}
break;
case 0x60: {
    instruction = Opcode("LD", 0x60, 1, n);
    instruction.mnemonic.append(" H B");
}
break;
case 0x61: {
    instruction = Opcode("LD", 0x61, 1, n);
    instruction.mnemonic.append(" H C");
}
break;
case 0x62: {
    instruction = Opcode("LD", 0x62, 1, n);
    instruction.mnemonic.append(" H D");
}
break;
case 0x63: {
    instruction = Opcode("LD", 0x63, 1, n);
    instruction.mnemonic.append(" H E");
}
break;
case 0x64: {
    instruction = Opcode("LD", 0x64, 1, n);
    instruction.mnemonic.append(" H H");
}
break;
case 0x65: {
    instruction = Opcode("LD", 0x65, 1, n);
    instruction.mnemonic.append(" H L");
}
break;
case 0x66: {
    instruction = Opcode("LD", 0x66, 1, n);
    instruction.mnemonic.append(" H HL");
}
break;
case 0x67: {
    instruction = Opcode("LD", 0x67, 1, n);
    instruction.mnemonic.append(" H A");
}
break;
case 0x68: {
    instruction = Opcode("LD", 0x68, 1, n);
    instruction.mnemonic.append(" L B");
}
break;
case 0x69: {
    instruction = Opcode("LD", 0x69, 1, n);
    instruction.mnemonic.append(" L C");
}
break;
case 0x6A: {
    instruction = Opcode("LD", 0x6A, 1, n);
    instruction.mnemonic.append(" L D");
}
break;
case 0x6B: {
    instruction = Opcode("LD", 0x6B, 1, n);
    instruction.mnemonic.append(" L E");
}
break;
case 0x6C: {
    instruction = Opcode("LD", 0x6C, 1, n);
    instruction.mnemonic.append(" L H");
}
break;
case 0x6D: {
    instruction = Opcode("LD", 0x6D, 1, n);
    instruction.mnemonic.append(" L L");
}
break;
case 0x6E: {
    instruction = Opcode("LD", 0x6E, 1, n);
    instruction.mnemonic.append(" L HL");
}
break;
case 0x6F: {
    instruction = Opcode("LD", 0x6F, 1, n);
    instruction.mnemonic.append(" L A");
}
break;
case 0x70: {
    instruction = Opcode("LD", 0x70, 1, n);
    instruction.mnemonic.append(" HL B");
}
break;
case 0x71: {
    instruction = Opcode("LD", 0x71, 1, n);
    instruction.mnemonic.append(" HL C");
}
break;
case 0x72: {
    instruction = Opcode("LD", 0x72, 1, n);
    instruction.mnemonic.append(" HL D");
}
break;
case 0x73: {
    instruction = Opcode("LD", 0x73, 1, n);
    instruction.mnemonic.append(" HL E");
}
break;
case 0x74: {
    instruction = Opcode("LD", 0x74, 1, n);
    instruction.mnemonic.append(" HL H");
}
break;
case 0x75: {
    instruction = Opcode("LD", 0x75, 1, n);
    instruction.mnemonic.append(" HL L");
}
break;
case 0x76: {
    instruction = Opcode("HALT", 0x76, 1, n);
}
break;
case 0x77: {
    instruction = Opcode("LD", 0x77, 1, n);
    instruction.mnemonic.append(" HL A");
}
break;
case 0x78: {
    instruction = Opcode("LD", 0x78, 1, n);
    instruction.mnemonic.append(" A B");
}
break;
case 0x79: {
    instruction = Opcode("LD", 0x79, 1, n);
    instruction.mnemonic.append(" A C");
}
break;
case 0x7A: {
    instruction = Opcode("LD", 0x7A, 1, n);
    instruction.mnemonic.append(" A D");
}
break;
case 0x7B: {
    instruction = Opcode("LD", 0x7B, 1, n);
    instruction.mnemonic.append(" A E");
}
break;
case 0x7C: {
    instruction = Opcode("LD", 0x7C, 1, n);
    instruction.mnemonic.append(" A H");
}
break;
case 0x7D: {
    instruction = Opcode("LD", 0x7D, 1, n);
    instruction.mnemonic.append(" A L");
}
break;
case 0x7E: {
    instruction = Opcode("LD", 0x7E, 1, n);
    instruction.mnemonic.append(" A HL");
}
break;
case 0x7F: {
    instruction = Opcode("LD", 0x7F, 1, n);
    instruction.mnemonic.append(" A A");
}
break;
case 0x80: {
    instruction = Opcode("ADD", 0x80, 1, n);
    instruction.mnemonic.append(" A B");
}
break;
case 0x81: {
    instruction = Opcode("ADD", 0x81, 1, n);
    instruction.mnemonic.append(" A C");
}
break;
case 0x82: {
    instruction = Opcode("ADD", 0x82, 1, n);
    instruction.mnemonic.append(" A D");
}
break;
case 0x83: {
    instruction = Opcode("ADD", 0x83, 1, n);
    instruction.mnemonic.append(" A E");
}
break;
case 0x84: {
    instruction = Opcode("ADD", 0x84, 1, n);
    instruction.mnemonic.append(" A H");
}
break;
case 0x85: {
    instruction = Opcode("ADD", 0x85, 1, n);
    instruction.mnemonic.append(" A L");
}
break;
case 0x86: {
    instruction = Opcode("ADD", 0x86, 1, n);
    instruction.mnemonic.append(" A HL");
}
break;
case 0x87: {
    instruction = Opcode("ADD", 0x87, 1, n);
    instruction.mnemonic.append(" A A");
}
break;
case 0x88: {
    instruction = Opcode("ADC", 0x88, 1, n);
    instruction.mnemonic.append(" A B");
}
break;
case 0x89: {
    instruction = Opcode("ADC", 0x89, 1, n);
    instruction.mnemonic.append(" A C");
}
break;
case 0x8A: {
    instruction = Opcode("ADC", 0x8A, 1, n);
    instruction.mnemonic.append(" A D");
}
break;
case 0x8B: {
    instruction = Opcode("ADC", 0x8B, 1, n);
    instruction.mnemonic.append(" A E");
}
break;
case 0x8C: {
    instruction = Opcode("ADC", 0x8C, 1, n);
    instruction.mnemonic.append(" A H");
}
break;
case 0x8D: {
    instruction = Opcode("ADC", 0x8D, 1, n);
    instruction.mnemonic.append(" A L");
}
break;
case 0x8E: {
    instruction = Opcode("ADC", 0x8E, 1, n);
    instruction.mnemonic.append(" A HL");
}
break;
case 0x8F: {
    instruction = Opcode("ADC", 0x8F, 1, n);
    instruction.mnemonic.append(" A A");
}
break;
case 0x90: {
    instruction = Opcode("SUB", 0x90, 1, n);
    instruction.mnemonic.append(" B");
}
break;
case 0x91: {
    instruction = Opcode("SUB", 0x91, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0x92: {
    instruction = Opcode("SUB", 0x92, 1, n);
    instruction.mnemonic.append(" D");
}
break;
case 0x93: {
    instruction = Opcode("SUB", 0x93, 1, n);
    instruction.mnemonic.append(" E");
}
break;
case 0x94: {
    instruction = Opcode("SUB", 0x94, 1, n);
    instruction.mnemonic.append(" H");
}
break;
case 0x95: {
    instruction = Opcode("SUB", 0x95, 1, n);
    instruction.mnemonic.append(" L");
}
break;
case 0x96: {
    instruction = Opcode("SUB", 0x96, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0x97: {
    instruction = Opcode("SUB", 0x97, 1, n);
    instruction.mnemonic.append(" A");
}
break;
case 0x98: {
    instruction = Opcode("SBC", 0x98, 1, n);
    instruction.mnemonic.append(" A B");
}
break;
case 0x99: {
    instruction = Opcode("SBC", 0x99, 1, n);
    instruction.mnemonic.append(" A C");
}
break;
case 0x9A: {
    instruction = Opcode("SBC", 0x9A, 1, n);
    instruction.mnemonic.append(" A D");
}
break;
case 0x9B: {
    instruction = Opcode("SBC", 0x9B, 1, n);
    instruction.mnemonic.append(" A E");
}
break;
case 0x9C: {
    instruction = Opcode("SBC", 0x9C, 1, n);
    instruction.mnemonic.append(" A H");
}
break;
case 0x9D: {
    instruction = Opcode("SBC", 0x9D, 1, n);
    instruction.mnemonic.append(" A L");
}
break;
case 0x9E: {
    instruction = Opcode("SBC", 0x9E, 1, n);
    instruction.mnemonic.append(" A HL");
}
break;
case 0x9F: {
    instruction = Opcode("SBC", 0x9F, 1, n);
    instruction.mnemonic.append(" A A");
}
break;
case 0xA0: {
    instruction = Opcode("AND", 0xA0, 1, n);
    instruction.mnemonic.append(" B");
}
break;
case 0xA1: {
    instruction = Opcode("AND", 0xA1, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0xA2: {
    instruction = Opcode("AND", 0xA2, 1, n);
    instruction.mnemonic.append(" D");
}
break;
case 0xA3: {
    instruction = Opcode("AND", 0xA3, 1, n);
    instruction.mnemonic.append(" E");
}
break;
case 0xA4: {
    instruction = Opcode("AND", 0xA4, 1, n);
    instruction.mnemonic.append(" H");
}
break;
case 0xA5: {
    instruction = Opcode("AND", 0xA5, 1, n);
    instruction.mnemonic.append(" L");
}
break;
case 0xA6: {
    instruction = Opcode("AND", 0xA6, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0xA7: {
    instruction = Opcode("AND", 0xA7, 1, n);
    instruction.mnemonic.append(" A");
}
break;
case 0xA8: {
    instruction = Opcode("XOR", 0xA8, 1, n);
    instruction.mnemonic.append(" B");
}
break;
case 0xA9: {
    instruction = Opcode("XOR", 0xA9, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0xAA: {
    instruction = Opcode("XOR", 0xAA, 1, n);
    instruction.mnemonic.append(" D");
}
break;
case 0xAB: {
    instruction = Opcode("XOR", 0xAB, 1, n);
    instruction.mnemonic.append(" E");
}
break;
case 0xAC: {
    instruction = Opcode("XOR", 0xAC, 1, n);
    instruction.mnemonic.append(" H");
}
break;
case 0xAD: {
    instruction = Opcode("XOR", 0xAD, 1, n);
    instruction.mnemonic.append(" L");
}
break;
case 0xAE: {
    instruction = Opcode("XOR", 0xAE, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0xAF: {
    instruction = Opcode("XOR", 0xAF, 1, n);
    instruction.mnemonic.append(" A");
}
break;
case 0xB0: {
    instruction = Opcode("OR", 0xB0, 1, n);
    instruction.mnemonic.append(" B");
}
break;
case 0xB1: {
    instruction = Opcode("OR", 0xB1, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0xB2: {
    instruction = Opcode("OR", 0xB2, 1, n);
    instruction.mnemonic.append(" D");
}
break;
case 0xB3: {
    instruction = Opcode("OR", 0xB3, 1, n);
    instruction.mnemonic.append(" E");
}
break;
case 0xB4: {
    instruction = Opcode("OR", 0xB4, 1, n);
    instruction.mnemonic.append(" H");
}
break;
case 0xB5: {
    instruction = Opcode("OR", 0xB5, 1, n);
    instruction.mnemonic.append(" L");
}
break;
case 0xB6: {
    instruction = Opcode("OR", 0xB6, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0xB7: {
    instruction = Opcode("OR", 0xB7, 1, n);
    instruction.mnemonic.append(" A");
}
break;
case 0xB8: {
    instruction = Opcode("CP", 0xB8, 1, n);
    instruction.mnemonic.append(" B");
}
break;
case 0xB9: {
    instruction = Opcode("CP", 0xB9, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0xBA: {
    instruction = Opcode("CP", 0xBA, 1, n);
    instruction.mnemonic.append(" D");
}
break;
case 0xBB: {
    instruction = Opcode("CP", 0xBB, 1, n);
    instruction.mnemonic.append(" E");
}
break;
case 0xBC: {
    instruction = Opcode("CP", 0xBC, 1, n);
    instruction.mnemonic.append(" H");
}
break;
case 0xBD: {
    instruction = Opcode("CP", 0xBD, 1, n);
    instruction.mnemonic.append(" L");
}
break;
case 0xBE: {
    instruction = Opcode("CP", 0xBE, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0xBF: {
    instruction = Opcode("CP", 0xBF, 1, n);
    instruction.mnemonic.append(" A");
}
break;
case 0xC0: {
    instruction = Opcode("RET", 0xC0, 1, n);
    instruction.mnemonic.append(" NZ");
}
break;
case 0xC1: {
    instruction = Opcode("POP", 0xC1, 1, n);
    instruction.mnemonic.append(" BC");
}
break;
case 0xC2: {
    instruction = Opcode("JP", 0xC2, 3, n);
    char addr[5 + 0];
    sprintf(addr, " NZ %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xC3: {
    instruction = Opcode("JP", 0xC3, 3, n);
    char addr[5 + 0];
    sprintf(addr, "  %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xC4: {
    instruction = Opcode("CALL", 0xC4, 3, n);
    char addr[5 + 0];
    sprintf(addr, " NZ %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xC5: {
    instruction = Opcode("PUSH", 0xC5, 1, n);
    instruction.mnemonic.append(" BC");
}
break;
case 0xC6: {
    instruction = Opcode("ADD", 0xC6, 2, n);
    char addr[3 + 0];
    sprintf(addr, " A %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xC7: {
    instruction = Opcode("RST", 0xC7, 1, n);
    instruction.mnemonic.append(" 00H");
}
break;
case 0xC8: {
    instruction = Opcode("RET", 0xC8, 1, n);
    instruction.mnemonic.append(" Z");
}
break;
case 0xC9: {
    instruction = Opcode("RET", 0xC9, 1, n);
}
break;
case 0xCA: {
    instruction = Opcode("JP", 0xCA, 3, n);
    char addr[5 + 0];
    sprintf(addr, " Z %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xCC: {
    instruction = Opcode("CALL", 0xCC, 3, n);
    char addr[5 + 0];
    sprintf(addr, " Z %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xCD: {
    instruction = Opcode("CALL", 0xCD, 3, n);
    char addr[5 + 0];
    sprintf(addr, "  %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xCE: {
    instruction = Opcode("ADC", 0xCE, 2, n);
    char addr[3 + 0];
    sprintf(addr, " A %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xCF: {
    instruction = Opcode("RST", 0xCF, 1, n);
    instruction.mnemonic.append(" 08H");
}
break;
case 0xD0: {
    instruction = Opcode("RET", 0xD0, 1, n);
    instruction.mnemonic.append(" NC");
}
break;
case 0xD1: {
    instruction = Opcode("POP", 0xD1, 1, n);
    instruction.mnemonic.append(" DE");
}
break;
case 0xD2: {
    instruction = Opcode("JP", 0xD2, 3, n);
    char addr[5 + 0];
    sprintf(addr, " NC %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xD3: {
    instruction = Opcode("ILLEGAL_D3", 0xD3, 1, n);
}
break;
case 0xD4: {
    instruction = Opcode("CALL", 0xD4, 3, n);
    char addr[5 + 0];
    sprintf(addr, " NC %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xD5: {
    instruction = Opcode("PUSH", 0xD5, 1, n);
    instruction.mnemonic.append(" DE");
}
break;
case 0xD6: {
    instruction = Opcode("SUB", 0xD6, 2, n);
    char addr[3 + 0];
    sprintf(addr, "  %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xD7: {
    instruction = Opcode("RST", 0xD7, 1, n);
    instruction.mnemonic.append(" 10H");
}
break;
case 0xD8: {
    instruction = Opcode("RET", 0xD8, 1, n);
    instruction.mnemonic.append(" C");
}
break;
case 0xD9: {
    instruction = Opcode("RETI", 0xD9, 1, n);
}
break;
case 0xDA: {
    instruction = Opcode("JP", 0xDA, 3, n);
    char addr[5 + 0];
    sprintf(addr, " C %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xDB: {
    instruction = Opcode("ILLEGAL_DB", 0xDB, 1, n);
}
break;
case 0xDC: {
    instruction = Opcode("CALL", 0xDC, 3, n);
    char addr[5 + 0];
    sprintf(addr, " C %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xDD: {
    instruction = Opcode("ILLEGAL_DD", 0xDD, 1, n);
}
break;
case 0xDE: {
    instruction = Opcode("SBC", 0xDE, 2, n);
    char addr[3 + 0];
    sprintf(addr, " A %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xDF: {
    instruction = Opcode("RST", 0xDF, 1, n);
    instruction.mnemonic.append(" 18H");
}
break;
case 0xE0: {
    instruction = Opcode("LDH", 0xE0, 2, n);
    instruction.mnemonic.append(" a8 A");
}
break;
case 0xE1: {
    instruction = Opcode("POP", 0xE1, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0xE2: {
    instruction = Opcode("LD", 0xE2, 1, n);
    instruction.mnemonic.append(" C A");
}
break;
case 0xE3: {
    instruction = Opcode("ILLEGAL_E3", 0xE3, 1, n);
}
break;
case 0xE4: {
    instruction = Opcode("ILLEGAL_E4", 0xE4, 1, n);
}
break;
case 0xE5: {
    instruction = Opcode("PUSH", 0xE5, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0xE6: {
    instruction = Opcode("AND", 0xE6, 2, n);
    char addr[3 + 0];
    sprintf(addr, "  %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xE7: {
    instruction = Opcode("RST", 0xE7, 1, n);
    instruction.mnemonic.append(" 20H");
}
break;
case 0xE8: {
    instruction = Opcode("ADD", 0xE8, 2, n);
    instruction.mnemonic.append(" SP r8");
}
break;
case 0xE9: {
    instruction = Opcode("JP", 0xE9, 1, n);
    instruction.mnemonic.append(" HL");
}
break;
case 0xEA: {
    instruction = Opcode("LD", 0xEA, 3, n);
    char addr[5 + 2];
    sprintf(addr, "  %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xEB: {
    instruction = Opcode("ILLEGAL_EB", 0xEB, 1, n);
}
break;
case 0xEC: {
    instruction = Opcode("ILLEGAL_EC", 0xEC, 1, n);
}
break;
case 0xED: {
    instruction = Opcode("ILLEGAL_ED", 0xED, 1, n);
}
break;
case 0xEE: {
    instruction = Opcode("XOR", 0xEE, 2, n);
    char addr[3 + 0];
    sprintf(addr, "  %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xEF: {
    instruction = Opcode("RST", 0xEF, 1, n);
    instruction.mnemonic.append(" 28H");
}
break;
case 0xF0: {
    instruction = Opcode("LDH", 0xF0, 2, n);
    instruction.mnemonic.append(" A a8");
}
break;
case 0xF1: {
    instruction = Opcode("POP", 0xF1, 1, n);
    instruction.mnemonic.append(" AF");
}
break;
case 0xF2: {
    instruction = Opcode("LD", 0xF2, 1, n);
    instruction.mnemonic.append(" A C");
}
break;
case 0xF3: {
    instruction = Opcode("DI", 0xF3, 1, n);
}
break;
case 0xF4: {
    instruction = Opcode("ILLEGAL_F4", 0xF4, 1, n);
}
break;
case 0xF5: {
    instruction = Opcode("PUSH", 0xF5, 1, n);
    instruction.mnemonic.append(" AF");
}
break;
case 0xF6: {
    instruction = Opcode("OR", 0xF6, 2, n);
    char addr[3 + 0];
    sprintf(addr, "  %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xF7: {
    instruction = Opcode("RST", 0xF7, 1, n);
    instruction.mnemonic.append(" 30H");
}
break;
case 0xF8: {
    instruction = Opcode("LD", 0xF8, 2, n);
    instruction.mnemonic.append(" HL SP r8");
}
break;
case 0xF9: {
    instruction = Opcode("LD", 0xF9, 1, n);
    instruction.mnemonic.append(" SP HL");
}
break;
case 0xFA: {
    instruction = Opcode("LD", 0xFA, 3, n);
    char addr[5 + 2];
    sprintf(addr, " A %02X%02X", m_Soc->ram.read(n+2), m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xFB: {
    instruction = Opcode("EI", 0xFB, 1, n);
}
break;
case 0xFC: {
    instruction = Opcode("ILLEGAL_FC", 0xFC, 1, n);
}
break;
case 0xFD: {
    instruction = Opcode("ILLEGAL_FD", 0xFD, 1, n);
}
break;
case 0xFE: {
    instruction = Opcode("CP", 0xFE, 2, n);
    char addr[3 + 0];
    sprintf(addr, "  %02X", m_Soc->ram.read(n+1));
    instruction.mnemonic.append(addr);
}
break;
case 0xFF: {
    instruction = Opcode("RST", 0xFF, 1, n);
    instruction.mnemonic.append(" 38H");
}
break;
