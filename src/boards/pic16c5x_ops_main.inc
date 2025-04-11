case 0x00: nop(); break;
case 0x01: illegal(); break;
case 0x02: movwf(); break;
case 0x03: movwf(); break;
case 0x04: clrw(); break;
case 0x05: illegal(); break;
case 0x06: clrf(); break;
case 0x07: clrf(); break;
/*08*/
case 0x08: subwf(); break;
case 0x09: subwf(); break;
case 0x0A: subwf(); break;
case 0x0B: subwf(); break;
case 0x0C: decf(); break;
case 0x0D: decf(); break;
case 0x0E: decf(); break;
case 0x0F: decf(); break;
/*10*/
case 0x10: iorwf(); break;
case 0x11: iorwf(); break;
case 0x12: iorwf(); break;
case 0x13: iorwf(); break;
case 0x14: andwf(); break;
case 0x15: andwf(); break;
case 0x16: andwf(); break;
case 0x17: andwf(); break;
/*18*/
case 0x18: xorwf(); break;
case 0x19: xorwf(); break;
case 0x1A: xorwf(); break;
case 0x1B: xorwf(); break;
case 0x1C: addwf(); break;
case 0x1D: addwf(); break;
case 0x1E: addwf(); break;
case 0x1F: addwf(); break;
/*20*/
case 0x20: movf(); break;
case 0x21: movf(); break;
case 0x22: movf(); break;
case 0x23: movf(); break;
case 0x24: comf(); break;
case 0x25: comf(); break;
case 0x26: comf(); break;
case 0x27: comf(); break;
/*28*/
case 0x28: incf(); break;
case 0x29: incf(); break;
case 0x2A: incf(); break;
case 0x2B: incf(); break;
case 0x2C: decfsz(); break;
case 0x2D: decfsz(); break;
case 0x2E: decfsz(); break;
case 0x2F: decfsz(); break;
/*30*/
case 0x30: rrf(); break;
case 0x31: rrf(); break;
case 0x32: rrf(); break;
case 0x33: rrf(); break;
case 0x34: rlf(); break;
case 0x35: rlf(); break;
case 0x36: rlf(); break;
case 0x37: rlf(); break;
/*38*/
case 0x38: swapf(); break;
case 0x39: swapf(); break;
case 0x3A: swapf(); break;
case 0x3B: swapf(); break;
case 0x3C: incfsz(); break;
case 0x3D: incfsz(); break;
case 0x3E: incfsz(); break;
case 0x3F: incfsz(); break;
/*40*/
case 0x40: bcf(); break;
case 0x41: bcf(); break;
case 0x42: bcf(); break;
case 0x43: bcf(); break;
case 0x44: bcf(); break;
case 0x45: bcf(); break;
case 0x46: bcf(); break;
case 0x47: bcf(); break;
/*48*/
case 0x48: bcf(); break;
case 0x49: bcf(); break;
case 0x4A: bcf(); break;
case 0x4B: bcf(); break;
case 0x4C: bcf(); break;
case 0x4D: bcf(); break;
case 0x4E: bcf(); break;
case 0x4F: bcf(); break;
/*50*/
case 0x50: bsf(); break;
case 0x51: bsf(); break;
case 0x52: bsf(); break;
case 0x53: bsf(); break;
case 0x54: bsf(); break;
case 0x55: bsf(); break;
case 0x56: bsf(); break;
case 0x57: bsf(); break;
/*58*/
case 0x58: bsf(); break;
case 0x59: bsf(); break;
case 0x5A: bsf(); break;
case 0x5B: bsf(); break;
case 0x5C: bsf(); break;
case 0x5D: bsf(); break;
case 0x5E: bsf(); break;
case 0x5F: bsf(); break;
/*60*/
case 0x60: btfsc(); break;
case 0x61: btfsc(); break;
case 0x62: btfsc(); break;
case 0x63: btfsc(); break;
case 0x64: btfsc(); break;
case 0x65: btfsc(); break;
case 0x66: btfsc(); break;
case 0x67: btfsc(); break;
/*68*/
case 0x68: btfsc(); break;
case 0x69: btfsc(); break;
case 0x6A: btfsc(); break;
case 0x6B: btfsc(); break;
case 0x6C: btfsc(); break;
case 0x6D: btfsc(); break;
case 0x6E: btfsc(); break;
case 0x6F: btfsc(); break;
/*70*/
case 0x70: btfss(); break;
case 0x71: btfss(); break;
case 0x72: btfss(); break;
case 0x73: btfss(); break;
case 0x74: btfss(); break;
case 0x75: btfss(); break;
case 0x76: btfss(); break;
case 0x77: btfss(); break;
/*78*/
case 0x78: btfss(); break;
case 0x79: btfss(); break;
case 0x7A: btfss(); break;
case 0x7B: btfss(); break;
case 0x7C: btfss(); break;
case 0x7E: btfss(); break;
case 0x7D: btfss(); break;
case 0x7F: btfss(); break;
/*80*/
case 0x80: retlw(); break;
case 0x81: retlw(); break;
case 0x82: retlw(); break;
case 0x83: retlw(); break;
case 0x84: retlw(); break;
case 0x85: retlw(); break;
case 0x86: retlw(); break;
case 0x87: retlw(); break;
/*88*/
case 0x88: retlw(); break;
case 0x89: retlw(); break;
case 0x8A: retlw(); break;
case 0x8B: retlw(); break;
case 0x8C: retlw(); break;
case 0x8D: retlw(); break;
case 0x8E: retlw(); break;
case 0x8F: retlw(); break;
/*90*/
case 0x90: call(); break;
case 0x91: call(); break;
case 0x92: call(); break;
case 0x93: call(); break;
case 0x94: call(); break;
case 0x95: call(); break;
case 0x96: call(); break;
case 0x97: call(); break;
/*98*/
case 0x98: call(); break;
case 0x99: call(); break;
case 0x9A: call(); break;
case 0x9B: call(); break;
case 0x9C: call(); break;
case 0x9D: call(); break;
case 0x9E: call(); break;
case 0x9F: call(); break;
/*A0*/
case 0xA0: goto_op(); break;
case 0xA1: goto_op(); break;
case 0xA2: goto_op(); break;
case 0xA3: goto_op(); break;
case 0xA4: goto_op(); break;
case 0xA5: goto_op(); break;
case 0xA6: goto_op(); break;
case 0xA7: goto_op(); break;
/*A8*/
case 0xA8: goto_op(); break;
case 0xA9: goto_op(); break;
case 0xAA: goto_op(); break;
case 0xAB: goto_op(); break;
case 0xAC: goto_op(); break;
case 0xAD: goto_op(); break;
case 0xAE: goto_op(); break;
case 0xAF: goto_op(); break;
/*B0*/
case 0xB0: goto_op(); break;
case 0xB1: goto_op(); break;
case 0xB2: goto_op(); break;
case 0xB3: goto_op(); break;
case 0xB4: goto_op(); break;
case 0xB5: goto_op(); break;
case 0xB6: goto_op(); break;
case 0xB7: goto_op(); break;
/*B8*/
case 0xB8: goto_op(); break;
case 0xB9: goto_op(); break;
case 0xBA: goto_op(); break;
case 0xBB: goto_op(); break;
case 0xBC: goto_op(); break;
case 0xBD: goto_op(); break;
case 0xBE: goto_op(); break;
case 0xBF: goto_op(); break;
/*C0*/
case 0xC0: movlw(); break;
case 0xC1: movlw(); break;
case 0xC2: movlw(); break;
case 0xC3: movlw(); break;
case 0xC4: movlw(); break;
case 0xC5: movlw(); break;
case 0xC6: movlw(); break;
case 0xC7: movlw(); break;
/*C8*/
case 0xC8: movlw(); break;
case 0xC9: movlw(); break;
case 0xCA: movlw(); break;
case 0xCB: movlw(); break;
case 0xCC: movlw(); break;
case 0xCD: movlw(); break;
case 0xCE: movlw(); break;
case 0xCF: movlw(); break;
/*D0*/
case 0xD0: iorlw(); break;
case 0xD1: iorlw(); break;
case 0xD2: iorlw(); break;
case 0xD3: iorlw(); break;
case 0xD4: iorlw(); break;
case 0xD5: iorlw(); break;
case 0xD6: iorlw(); break;
case 0xD7: iorlw(); break;
/*D8*/
case 0xD8: iorlw(); break;
case 0xD9: iorlw(); break;
case 0xDA: iorlw(); break;
case 0xDB: iorlw(); break;
case 0xDC: iorlw(); break;
case 0xDD: iorlw(); break;
case 0xDE: iorlw(); break;
case 0xDF: iorlw(); break;
/*E0*/
case 0xE0: andlw(); break;
case 0xE1: andlw(); break;
case 0xE2: andlw(); break;
case 0xE3: andlw(); break;
case 0xE4: andlw(); break;
case 0xE5: andlw(); break;
case 0xE6: andlw(); break;
case 0xE7: andlw(); break;
/*E8*/
case 0xE8: andlw(); break;
case 0xE9: andlw(); break;
case 0xEA: andlw(); break;
case 0xEB: andlw(); break;
case 0xEC: andlw(); break;
case 0xED: andlw(); break;
case 0xEE: andlw(); break;
case 0xEF: andlw(); break;
/*F0*/
case 0xF0: xorlw(); break;
case 0xF1: xorlw(); break;
case 0xF2: xorlw(); break;
case 0xF3: xorlw(); break;
case 0xF4: xorlw(); break;
case 0xF5: xorlw(); break;
case 0xF6: xorlw(); break;
case 0xF7: xorlw(); break;
/*F8*/
case 0xF8: xorlw(); break;
case 0xF9: xorlw(); break;
case 0xFA: xorlw(); break;
case 0xFB: xorlw(); break;
case 0xFC: xorlw(); break;
case 0xFD: xorlw(); break;
case 0xFE: xorlw(); break;
case 0xFF: xorlw(); break;
