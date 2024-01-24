/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _FCEU_OPS_H
#define _FCEU_OPS_H

case 0x00:  /* BRK */
			cpu.PC++;
			PUSH(cpu.PC>>8);
			PUSH(cpu.PC);
			PUSH(cpu.P|U_FLAG|B_FLAG);
			cpu.P     |= I_FLAG;
			cpu.mooPI |= I_FLAG;
			cpu.PC     = RdMem(0xFFFE);
			cpu.PC    |= RdMem(0xFFFF)<<8;
			break;

case 0x40:  /* RTI */
			cpu.P     = POP();
			cpu.mooPI = cpu.P;
			cpu.PC    = POP();
			cpu.PC   |= POP() << 8;
			break;

case 0x60:  /* RTS */
			cpu.PC=POP();
			cpu.PC|=POP()<<8;
			cpu.PC++;
			break;

case 0x48: /* PHA */
			PUSH(cpu.A);
			break;
case 0x08: /* PHP */
			PUSH(cpu.P|U_FLAG|B_FLAG);
			break;
case 0x68: /* PLA */
			cpu.A=POP();
			X_ZN(cpu.A);
			break;
case 0x28: /* PLP */
			cpu.P=POP();
			break;
case 0x4C:
		{
			uint16 ptmp=cpu.PC;
			uint32 npc;

			npc=RdMem(ptmp);
			ptmp++;
			npc|=RdMem(ptmp)<<8;
			cpu.PC=npc;
		}
		break; /* JMP ABSOLUTE */
case 0x6C:
			{
			uint32 tmp;
			GetAB(tmp);
			cpu.PC=RdMem(tmp);
			cpu.PC|=RdMem( ((tmp+1)&0x00FF) | (tmp&0xFF00))<<8;
			}
			break;
case 0x20: /* JSR */
			{
			uint8 npc;
			npc=RdMem(cpu.PC);
			cpu.PC++;
			PUSH(cpu.PC>>8);
			PUSH(cpu.PC);
			cpu.PC=RdMem(cpu.PC)<<8;
			cpu.PC|=npc;
			}
			break;

case 0xAA: /* TAX */
			cpu.X=cpu.A;
			X_ZN(cpu.A);
			break;

case 0x8A: /* TXA */
			cpu.A=cpu.X;
			X_ZN(cpu.A);
			break;

case 0xA8: /* TAY */
			cpu.Y=cpu.A;
			X_ZN(cpu.A);
			break;
case 0x98: /* TYA */
			cpu.A=cpu.Y;
			X_ZN(cpu.A);
			break;

case 0xBA: /* TSX */
			cpu.X=cpu.S;
			X_ZN(cpu.X);
			break;
case 0x9A: /* TXS */
			cpu.S=cpu.X;
			break;

case 0xCA: /* DEX */
			cpu.X--;
			X_ZN(cpu.X);
			break;
case 0x88: /* DEY */
			cpu.Y--;
			X_ZN(cpu.Y);
			break;

case 0xE8: /* INX */
			cpu.X++;
			X_ZN(cpu.X);
			break;
case 0xC8: /* INY */
			cpu.Y++;
			X_ZN(cpu.Y);
			break;

case 0x18: /* CLC */
			cpu.P &=~C_FLAG;
			break;
case 0xD8: /* CLD */
			cpu.P &=~D_FLAG;
			break;
case 0x58: /* CLI */
			cpu.P &=~I_FLAG;
			break;
case 0xB8: /* CLV */
			cpu.P &=~V_FLAG;
			break;

case 0x38: /* SEC */
			cpu.P |= C_FLAG;
			break;
case 0xF8: /* SED */
			cpu.P |= D_FLAG;
			break;
case 0x78: /* SEI */
			cpu.P |= I_FLAG;
			break;

case 0xEA: /* NOP */
			break;

case 0x0A: RMWCPU_A(ASL);
case 0x06: RMW_ZP(ASL);
case 0x16: RMW_ZPX(ASL);
case 0x0E: RMWCPU_AB(ASL);
case 0x1E: RMWCPU_ABX(ASL);

case 0xC6: RMW_ZP(DEC);
case 0xD6: RMW_ZPX(DEC);
case 0xCE: RMWCPU_AB(DEC);
case 0xDE: RMWCPU_ABX(DEC);

case 0xE6: RMW_ZP(INC);
case 0xF6: RMW_ZPX(INC);
case 0xEE: RMWCPU_AB(INC);
case 0xFE: RMWCPU_ABX(INC);

case 0x4A: RMWCPU_A(LSR);
case 0x46: RMW_ZP(LSR);
case 0x56: RMW_ZPX(LSR);
case 0x4E: RMWCPU_AB(LSR);
case 0x5E: RMWCPU_ABX(LSR);

case 0x2A: RMWCPU_A(ROL);
case 0x26: RMW_ZP(ROL);
case 0x36: RMW_ZPX(ROL);
case 0x2E: RMWCPU_AB(ROL);
case 0x3E: RMWCPU_ABX(ROL);

case 0x6A: RMWCPU_A(ROR);
case 0x66: RMW_ZP(ROR);
case 0x76: RMW_ZPX(ROR);
case 0x6E: RMWCPU_AB(ROR);
case 0x7E: RMWCPU_ABX(ROR);

case 0x69: LD_IM(ADC);
case 0x65: LD_ZP(ADC);
case 0x75: LD_ZPX(ADC);
case 0x6D: LDCPU_AB(ADC);
case 0x7D: LDCPU_ABX(ADC);
case 0x79: LDCPU_ABY(ADC);
case 0x61: LD_IX(ADC);
case 0x71: LD_IY(ADC);

case 0x29: LD_IM(AND);
case 0x25: LD_ZP(AND);
case 0x35: LD_ZPX(AND);
case 0x2D: LDCPU_AB(AND);
case 0x3D: LDCPU_ABX(AND);
case 0x39: LDCPU_ABY(AND);
case 0x21: LD_IX(AND);
case 0x31: LD_IY(AND);

case 0x24: LD_ZP(BIT);
case 0x2C: LDCPU_AB(BIT);

case 0xC9: LD_IM(CMP);
case 0xC5: LD_ZP(CMP);
case 0xD5: LD_ZPX(CMP);
case 0xCD: LDCPU_AB(CMP);
case 0xDD: LDCPU_ABX(CMP);
case 0xD9: LDCPU_ABY(CMP);
case 0xC1: LD_IX(CMP);
case 0xD1: LD_IY(CMP);

case 0xE0: LD_IM(CPX);
case 0xE4: LD_ZP(CPX);
case 0xEC: LDCPU_AB(CPX);

case 0xC0: LD_IM(CPY);
case 0xC4: LD_ZP(CPY);
case 0xCC: LDCPU_AB(CPY);

case 0x49: LD_IM(EOR);
case 0x45: LD_ZP(EOR);
case 0x55: LD_ZPX(EOR);
case 0x4D: LDCPU_AB(EOR);
case 0x5D: LDCPU_ABX(EOR);
case 0x59: LDCPU_ABY(EOR);
case 0x41: LD_IX(EOR);
case 0x51: LD_IY(EOR);

case 0xA9: LD_IM(LDA);
case 0xA5: LD_ZP(LDA);
case 0xB5: LD_ZPX(LDA);
case 0xAD: LDCPU_AB(LDA);
case 0xBD: LDCPU_ABX(LDA);
case 0xB9: LDCPU_ABY(LDA);
case 0xA1: LD_IX(LDA);
case 0xB1: LD_IY(LDA);

case 0xA2: LD_IM(LDX);
case 0xA6: LD_ZP(LDX);
case 0xB6: LD_ZPY(LDX);
case 0xAE: LDCPU_AB(LDX);
case 0xBE: LDCPU_ABY(LDX);

case 0xA0: LD_IM(LDY);
case 0xA4: LD_ZP(LDY);
case 0xB4: LD_ZPX(LDY);
case 0xAC: LDCPU_AB(LDY);
case 0xBC: LDCPU_ABX(LDY);

case 0x09: LD_IM(ORA);
case 0x05: LD_ZP(ORA);
case 0x15: LD_ZPX(ORA);
case 0x0D: LDCPU_AB(ORA);
case 0x1D: LDCPU_ABX(ORA);
case 0x19: LDCPU_ABY(ORA);
case 0x01: LD_IX(ORA);
case 0x11: LD_IY(ORA);

case 0xEB:  /* (undocumented) */
case 0xE9: LD_IM(SBC);
case 0xE5: LD_ZP(SBC);
case 0xF5: LD_ZPX(SBC);
case 0xED: LDCPU_AB(SBC);
case 0xFD: LDCPU_ABX(SBC);
case 0xF9: LDCPU_ABY(SBC);
case 0xE1: LD_IX(SBC);
case 0xF1: LD_IY(SBC);

case 0x85: ST_ZP(cpu.A);
case 0x95: ST_ZPX(cpu.A);
case 0x8D: STCPU_AB(cpu.A);
case 0x9D: STCPU_ABX(cpu.A);
case 0x99: STCPU_ABY(cpu.A);
case 0x81: ST_IX(cpu.A);
case 0x91: ST_IY(cpu.A);

case 0x86: ST_ZP(cpu.X);
case 0x96: ST_ZPY(cpu.X);
case 0x8E: STCPU_AB(cpu.X);

case 0x84: ST_ZP(cpu.Y);
case 0x94: ST_ZPX(cpu.Y);
case 0x8C: STCPU_AB(cpu.Y);

/* BCC */
case 0x90: JR(!(cpu.P & C_FLAG)); break;

/* BCS */
case 0xB0: JR(cpu.P & C_FLAG); break;

/* BEQ */
case 0xF0: JR(cpu.P & Z_FLAG); break;

/* BNE */
case 0xD0: JR(!(cpu.P & Z_FLAG)); break;

/* BMI */
case 0x30: JR(cpu.P & N_FLAG); break;

/* BPL */
case 0x10: JR(!(cpu.P & N_FLAG)); break;

/* BVC */
case 0x50: JR(!(cpu.P & V_FLAG)); break;

/* BVS */
case 0x70: JR(cpu.P & V_FLAG); break;

/* AAC */
case 0x2B:
case 0x0B: LD_IM(AND;cpu.P &= ~C_FLAG; cpu.P |= cpu.A>>7);

/* AAX */
case 0x87: ST_ZP(cpu.A&cpu.X);
case 0x97: ST_ZPY(cpu.A&cpu.X);
case 0x8F: STCPU_AB(cpu.A&cpu.X);
case 0x83: ST_IX(cpu.A&cpu.X);

/* ARR - ARGH, MATEY! */
case 0x6B: {
				uint8 arrtmp;
				LD_IM(AND;cpu.P &= ~V_FLAG;cpu.P|=(cpu.A^(cpu.A>>1))&0x40;arrtmp=cpu.A>>7;cpu.A>>=1;cpu.A|=(cpu.P&C_FLAG)<<7;cpu.P&=~C_FLAG;cpu.P|=arrtmp;X_ZN(cpu.A));
			}
/* ASR */
case 0x4B: LD_IM(AND;LSRA);

/* ATX(OAL) Is this(OR with $EE) correct? */
case 0xAB: LD_IM(cpu.A|=0xEE;AND;cpu.X=cpu.A);

/* AXS */
case 0xCB: LD_IM(AXS);

/* DCP */
case 0xC7: RMW_ZP(DEC;CMP);
case 0xD7: RMW_ZPX(DEC;CMP);
case 0xCF: RMWCPU_AB(DEC;CMP);
case 0xDF: RMWCPU_ABX(DEC;CMP);
case 0xDB: RMWCPU_ABY(DEC;CMP);
case 0xC3: RMW_IX(DEC;CMP);
case 0xD3: RMW_IY(DEC;CMP);

/* ISB */
case 0xE7: RMW_ZP(INC;SBC);
case 0xF7: RMW_ZPX(INC;SBC);
case 0xEF: RMWCPU_AB(INC;SBC);
case 0xFF: RMWCPU_ABX(INC;SBC);
case 0xFB: RMWCPU_ABY(INC;SBC);
case 0xE3: RMW_IX(INC;SBC);
case 0xF3: RMW_IY(INC;SBC);

/* DOP */

case 0x04:
case 0x14:
case 0x34:
case 0x44:
case 0x54:
case 0x64:
case 0x74:
case 0x80:
case 0x82:
case 0x89:
case 0xC2:
case 0xD4:
case 0xE2:
case 0xF4:
cpu.PC++;break;

/* KIL */

case 0x02:
case 0x12:
case 0x22:
case 0x32:
case 0x42:
case 0x52:
case 0x62:
case 0x72:
case 0x92:
case 0xB2:
case 0xD2:
case 0xF2:ADDCYC(0xFF);
		cpu.jammed = 1;
		cpu.PC--;
		break;

/* LAR */
case 0xBB: RMWCPU_ABY(cpu.S&=x;cpu.A=cpu.X=cpu.S;X_ZN(cpu.X));

/* LAX */
case 0xA7: LD_ZP(LDA;LDX);
case 0xB7: LD_ZPY(LDA;LDX);
case 0xAF: LDCPU_AB(LDA;LDX);
case 0xBF: LDCPU_ABY(LDA;LDX);
case 0xA3: LD_IX(LDA;LDX);
case 0xB3: LD_IY(LDA;LDX);

/* NOP */
case 0x1A:
case 0x3A:
case 0x5A:
case 0x7A:
case 0xDA:
case 0xFA: break;

/* RLA */
case 0x27: RMW_ZP(ROL;AND);
case 0x37: RMW_ZPX(ROL;AND);
case 0x2F: RMWCPU_AB(ROL;AND);
case 0x3F: RMWCPU_ABX(ROL;AND);
case 0x3B: RMWCPU_ABY(ROL;AND);
case 0x23: RMW_IX(ROL;AND);
case 0x33: RMW_IY(ROL;AND);

/* RRA */
case 0x67: RMW_ZP(ROR;ADC);
case 0x77: RMW_ZPX(ROR;ADC);
case 0x6F: RMWCPU_AB(ROR;ADC);
case 0x7F: RMWCPU_ABX(ROR;ADC);
case 0x7B: RMWCPU_ABY(ROR;ADC);
case 0x63: RMW_IX(ROR;ADC);
case 0x73: RMW_IY(ROR;ADC);

/* SLO */
case 0x07: RMW_ZP(ASL;ORA);
case 0x17: RMW_ZPX(ASL;ORA);
case 0x0F: RMWCPU_AB(ASL;ORA);
case 0x1F: RMWCPU_ABX(ASL;ORA);
case 0x1B: RMWCPU_ABY(ASL;ORA);
case 0x03: RMW_IX(ASL;ORA);
case 0x13: RMW_IY(ASL;ORA);

/* SRE */
case 0x47: RMW_ZP(LSR;EOR);
case 0x57: RMW_ZPX(LSR;EOR);
case 0x4F: RMWCPU_AB(LSR;EOR);
case 0x5F: RMWCPU_ABX(LSR;EOR);
case 0x5B: RMWCPU_ABY(LSR;EOR);
case 0x43: RMW_IX(LSR;EOR);
case 0x53: RMW_IY(LSR;EOR);

/* AXA - SHA */
case 0x93: ST_IY(cpu.A&cpu.X&(((A-cpu.Y)>>8)+1));
case 0x9F: STCPU_ABY(cpu.A&cpu.X&(((A-cpu.Y)>>8)+1));

/* SYA */
case 0x9C: STCPU_ABX(cpu.Y&(((A-cpu.X)>>8)+1));

/* SXA */
case 0x9E: STCPU_ABY(cpu.X&(((A-cpu.Y)>>8)+1));

/* XAS */
case 0x9B: cpu.S=cpu.A&cpu.X;STCPU_ABY(cpu.S& (((A-cpu.Y)>>8)+1) );

/* TOP */
case 0x0C: LDCPU_AB(;);
case 0x1C:
case 0x3C:
case 0x5C:
case 0x7C:
case 0xDC:
case 0xFC: LDCPU_ABX(;);

/* XAA - BIG QUESTION MARK HERE */
case 0x8B: cpu.A|=0xEE; cpu.A&=cpu.X; LD_IM(AND);
/* endif */
#endif
