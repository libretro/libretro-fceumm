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

#include <string.h>

#include "fceu-types.h"
#include "x6502.h"
#include "sound.h"
#include "ppu.h"

X6502 cpu;
uint32 timestamp;
uint32 sound_timestamp;

void (*MapIRQHook)(int a);

static INLINE DECLFR(RdMem);
static INLINE DECLFW(WrMem);
static INLINE DECLFW(WrRAM);
static INLINE DECLFR(RdRAM);

#define ADDCYC(x)                                                              \
	{                                                                          \
		int __x = x;                                                           \
		cpu.tcount += __x;                                                     \
		cpu.count -= __x * 48;                                                 \
		timestamp += __x;                                                      \
		if (!ppu.overclock.overclocked_state) {                                \
			sound_timestamp += __x;                                            \
		}                                                                      \
	}

#if 0
#define POP() RdRAM(0x100 + (++cpu.S))
#define PUSH(V) { uint8 VTMP = V; WrRAM(0x100 + (cpu.S--), VTMP); }
#else
static INLINE uint8 POP(void) {
	cpu.S++;
	return RdRAM(0x100 + cpu.S);
}

static INLINE void PUSH(uint8 V) {
	WrRAM(0x100 + cpu.S, V);
	cpu.S--;
}
#endif

/* Some of these operations will only make sense if you know what the flag constants are. */

#define X_ZN(zort)  cpu.P &= ~(Z_FLAG | N_FLAG); cpu.P |= ZNTable[zort]
#define X_ZNT(zort) cpu.P |= ZNTable[zort]

#define JR(cond)                                                               \
	{                                                                          \
		if ((cond) != 0) {                                                     \
			uint16 tmp;                                                        \
			uint8 disp = RdMem(cpu.PC);                                        \
			cpu.PC++;                                                          \
			ADDCYC(1);                                                         \
			tmp = cpu.PC;                                                      \
			cpu.PC += (int8)disp;                                              \
			if ((tmp ^ cpu.PC) & 0x100) {                                      \
				ADDCYC(1);                                                     \
			}                                                                  \
		} else {                                                               \
			cpu.PC++;                                                          \
		}                                                                      \
	}

#define LDA     cpu.A = x; X_ZN(cpu.A)
#define LDX     cpu.X = x; X_ZN(cpu.X)
#define LDY     cpu.Y = x; X_ZN(cpu.Y)

/* All of the freaky arithmetic operations. */
#define AND     cpu.A &= x; X_ZN(cpu.A)
#define BIT     cpu.P &= ~(Z_FLAG | V_FLAG | N_FLAG); cpu.P |= ZNTable[x & cpu.A] & Z_FLAG; cpu.P |= x & (V_FLAG | N_FLAG)
#define EOR     cpu.A ^= x; X_ZN(cpu.A)
#define ORA     cpu.A |= x; X_ZN(cpu.A)

#define ADC                                                                    \
	{                                                                          \
		uint16 l = cpu.A + x + (cpu.P & 1);                                    \
		cpu.P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);                         \
		cpu.P |= ((((cpu.A ^ x) & 0x80) ^ 0x80) & ((cpu.A ^ l) & 0x80)) >> 1;  \
		cpu.P |= (l >> 8) & C_FLAG;                                            \
		cpu.A = (uint8)l;                                                             \
		X_ZNT(cpu.A);                                                          \
	}

#define SBC                                                                    \
	{                                                                          \
		uint16 l = cpu.A - x - ((cpu.P & 1) ^ 1);                              \
		cpu.P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);                         \
		cpu.P |= ((cpu.A ^ l) & (cpu.A ^ x) & 0x80) >> 1;                      \
		cpu.P |= ((l >> 8) & C_FLAG) ^ C_FLAG;                                 \
		cpu.A = (uint8)l;                                                      \
		X_ZNT(cpu.A);                                                          \
	}

#define CMPL(a1, a2)                                                           \
	{                                                                          \
		uint16 t = a1 - a2;                                                    \
		X_ZN(t & 0xFF);                                                        \
		cpu.P &= ~C_FLAG;                                                      \
		cpu.P |= ((t >> 8) & C_FLAG) ^ C_FLAG;                                 \
	}

/* Special undocumented operation.  Very similar to CMP. */
#define AXS                                                                    \
	{                                                                          \
		uint16 t = (cpu.A & cpu.X) - x;                                        \
		X_ZN(t & 0xFF);                                                        \
		cpu.P &= ~C_FLAG;                                                      \
		cpu.P |= ((t >> 8) & C_FLAG) ^ C_FLAG;                                 \
		cpu.X = (uint8)t;                                                      \
	}

#define CMP     CMPL(cpu.A, x)
#define CPX     CMPL(cpu.X, x)
#define CPY     CMPL(cpu.Y, x)

/* The following operations modify the byte being worked on. */
#define DEC     x--; X_ZN(x)
#define INC     x++; X_ZN(x)

#define ASL     cpu.P &= ~C_FLAG; cpu.P |= x >> 7; x <<= 1; X_ZN(x)
#define LSR     cpu.P &= ~(C_FLAG | N_FLAG | Z_FLAG); cpu.P |= x & 1; x >>= 1; X_ZNT(x)

/* For undocumented instructions, maybe for other things later... */
#define LSRA    cpu.P &= ~(C_FLAG | N_FLAG | Z_FLAG); cpu.P |= cpu.A & 1; cpu.A >>= 1; X_ZNT(cpu.A)

#define ROL                                                                    \
	{                                                                          \
		uint8 l = x >> 7;                                                      \
		x <<= 1;                                                               \
		x |= cpu.P & C_FLAG;                                                   \
		cpu.P &= ~(Z_FLAG | N_FLAG | C_FLAG);                                  \
		cpu.P |= l;                                                            \
		X_ZNT(x);                                                              \
	}

#define ROR                                                                    \
	{                                                                          \
		uint8 l = x & 1;                                                       \
		x >>= 1;                                                               \
		x |= (cpu.P & C_FLAG) << 7;                                            \
		cpu.P &= ~(Z_FLAG | N_FLAG | C_FLAG);                                  \
		cpu.P |= l;                                                            \
		X_ZNT(x);                                                              \
	}

/* Icky icky thing for some undocumented instructions.  Can easily be
 * broken if names of local variables are changed.
 */

/* Absolute */
#define GetAB(target)                                                          \
	{                                                                          \
		target = RdMem(cpu.PC);                                                \
		cpu.PC++;                                                              \
		target |= RdMem(cpu.PC) << 8;                                          \
		cpu.PC++;                                                              \
	}

/* Absolute Indexed(for reads) */
#define GetABIRD(target, i)                                                    \
	{                                                                          \
		uint16 tmp;                                                            \
		GetAB(tmp);                                                            \
		target = tmp;                                                          \
		target += i;                                                           \
		if ((target ^ tmp) & 0x100) {                                          \
			RdMem(target ^ 0x100);                                             \
			ADDCYC(1);                                                         \
		}                                                                      \
	}

/* Absolute Indexed(for writes and rmws) */
#define GetABIWR(target, i)                                                    \
	{                                                                          \
		uint16 rt;                                                             \
		GetAB(rt);                                                             \
		target = rt;                                                           \
		target += i;                                                           \
		RdMem((target & 0x00FF) | (rt & 0xFF00));                              \
	}

/* Zero Page */
#define GetZP(target)                                                          \
	{                                                                          \
		target = RdMem(cpu.PC);                                                \
		cpu.PC++;                                                              \
	}

/* Zero Page Indexed */
#define GetZPI(target, i)                                                      \
	{                                                                          \
		target = i + RdMem(cpu.PC);                                            \
		cpu.PC++;                                                              \
	}

/* Indexed Indirect */
#define GetIX(target)                                                          \
	{                                                                          \
		uint8 tmp = RdMem(cpu.PC);                                             \
		cpu.PC++;                                                              \
		tmp += cpu.X;                                                          \
		target = RdRAM(tmp);                                                   \
		tmp++;                                                                 \
		target |= RdRAM(tmp) << 8;                                             \
	}

/* Indirect Indexed(for reads) */
#define GetIYRD(target)                                                        \
	{                                                                          \
		uint16 rt;                                                             \
		uint8 tmp = RdMem(cpu.PC);                                             \
		cpu.PC++;                                                              \
		rt = RdRAM(tmp);                                                       \
		tmp++;                                                                 \
		rt |= RdRAM(tmp) << 8;                                                 \
		target = rt;                                                           \
		target += cpu.Y;                                                       \
		if ((target ^ rt) & 0x100) {                                           \
			RdMem(target ^ 0x100);                                             \
			ADDCYC(1);                                                         \
		}                                                                      \
	}

/* Indirect Indexed(for writes and rmws) */
#define GetIYWR(target)                                                        \
	{                                                                          \
		uint16 rt;                                                             \
		uint8 tmp = RdMem(cpu.PC);                                             \
		cpu.PC++;                                                              \
		rt = RdRAM(tmp);                                                       \
		tmp++;                                                                 \
		rt |= RdRAM(tmp) << 8;                                                 \
		target = rt;                                                           \
		target += cpu.Y;                                                       \
		RdMem((target & 0x00FF) | (rt & 0xFF00));                              \
	}

/* Now come the macros to wrap up all of the above stuff addressing mode functions
and operation macros.  Note that operation macros will always operate(redundant
redundant) on the variable "x".
*/

#define RMW_A(op)       { uint8 x = cpu.A; op; cpu.A = x; break; }	/* Meh... */
#define RMW_AB(op)      { uint16 A; uint8 x; GetAB(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ABI(reg, op) { uint16 A; uint8 x; GetABIWR(A, reg); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ABX(op)     RMW_ABI(cpu.X, op)
#define RMW_ABY(op)     RMW_ABI(cpu.Y, op)
#define RMW_IX(op)      { uint16 A; uint8 x; GetIX(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_IY(op)      { uint16 A; uint8 x; GetIYWR(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ZP(op)      { uint8 A; uint8 x; GetZP(A); x = RdRAM(A); op; WrRAM(A, x); break; }
#define RMW_ZPX(op)     { uint8 A; uint8 x; GetZPI(A, cpu.X); x = RdRAM(A); op; WrRAM(A, x); break; }

#define LD_IM(op)       { uint8 x; x = RdMem(cpu.PC); cpu.PC++; op; break; }
#define LD_ZP(op)       { uint8 A; uint8 x; GetZP(A); x = RdRAM(A); op; break; }
#define LD_ZPX(op)      { uint8 A; uint8 x; GetZPI(A, cpu.X); x = RdRAM(A); op; break; }
#define LD_ZPY(op)      { uint8 A; uint8 x; GetZPI(A, cpu.Y); x = RdRAM(A); op; break; }
#define LD_AB(op)       { uint16 A; uint8 x; FCEU_UNUSED(x); GetAB(A); x = RdMem(A); op; break; }
#define LD_ABI(reg, op) { uint16 A; uint8 x; FCEU_UNUSED(x); GetABIRD(A, reg); x = RdMem(A); op; break; }
#define LD_ABX(op)      LD_ABI(cpu.X, op)
#define LD_ABY(op)      LD_ABI(cpu.Y, op)
#define LD_IX(op)       { uint16 A; uint8 x; GetIX(A); x = RdMem(A); op; break; }
#define LD_IY(op)       { uint16 A; uint8 x; GetIYRD(A); x = RdMem(A); op; break; }

#define ST_ZP(r)        { uint8 A; GetZP(A); WrRAM(A, r); break; }
#define ST_ZPX(r)       { uint8 A; GetZPI(A, cpu.X); WrRAM(A, r); break; }
#define ST_ZPY(r)       { uint8 A; GetZPI(A, cpu.Y); WrRAM(A, r); break; }
#define ST_AB(r)        { uint16 A; GetAB(A); WrMem(A, r); break; }
#define ST_ABI(reg, r)  { uint16 A; GetABIWR(A, reg); WrMem(A, r); break; }
#define ST_ABX(r)       ST_ABI(cpu.X, r)
#define ST_ABY(r)       ST_ABI(cpu.Y, r)
#define ST_IX(r)        { uint16 A; GetIX(A); WrMem(A, r); break; }
#define ST_IY(r)        { uint16 A; GetIYWR(A); WrMem(A, r); break; }

static const uint8 ZNTable[256] = {
/*0x00*/ Z_FLAG,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x10*/      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x20*/      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x30*/      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x40*/      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x50*/      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x60*/      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x70*/      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
/*0x80*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG,
/*0x90*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG,
/*0xA0*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG,
/*0xB0*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG,
/*0xC0*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG,
/*0xD0*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG,
/*0xE0*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG,
/*0xF0*/ N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG, N_FLAG
};

static const uint8 CycTable[256] = {
/*0x00*/ 7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
/*0x10*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x20*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
/*0x30*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x40*/ 6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
/*0x50*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x60*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
/*0x70*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x80*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
/*0x90*/ 2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
/*0xA0*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
/*0xB0*/ 2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
/*0xC0*/ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
/*0xD0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0xE0*/ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
/*0xF0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7
};

static INLINE DECLFR(RdMem) {
	cpu.openbus = ARead[A](A);
	return (cpu.openbus);
}

static INLINE DECLFW(WrMem) {
	BWrite[A](A, V);
	cpu.openbus = V;
}

static INLINE DECLFR(RdRAM) {
	cpu.openbus = RAM[A];
	return (cpu.openbus);
}

static INLINE DECLFW(WrRAM) {
	RAM[A] = V;
	cpu.openbus = V;
}

DECLFR(X6502_DMR) {
	ADDCYC(1);
	return(cpu.openbus = ARead[A](A));
}

DECLFW(X6502_DMW) {
	ADDCYC(1);
	BWrite[A](A, V);
}

void X6502_IRQBegin(int w) {
	cpu.IRQlow |= w;
}

void X6502_IRQEnd(int w) {
	cpu.IRQlow &= ~w;
}

void TriggerNMI(void) {
	cpu.IRQlow |= FCEU_IQNMI;
}

void TriggerNMI2(void) {
	cpu.IRQlow |= FCEU_IQNMI2;
}

void X6502_Reset(void) {
	cpu.IRQlow = FCEU_IQRESET;
}

void X6502_Init(void) {
	memset((void*)&cpu, 0, sizeof(cpu));
	#ifdef FCEUDEF_DEBUGGER
	X6502_Debug(0, 0, 0);
	#endif
}

void X6502_Power(void) {
	cpu.count = cpu.tcount = cpu.IRQlow = cpu.PC = cpu.A = cpu.X = cpu.Y = cpu.P = cpu.mooPI = cpu.openbus = cpu.jammed = 0;
	cpu.P = 0x34;
	timestamp = sound_timestamp = 0;
	X6502_Reset();
}

void X6502_Run(int32 cycles) {
	if (isPAL) {
		cycles *= 15;	/* 15*4=60 */
	} else {
		cycles *= 16;	/* 16*4=64 */
	}

	cpu.count += cycles;

	while (cpu.count > 0) {
		int32 temp;

		if (cpu.IRQlow) {
			if (cpu.IRQlow & FCEU_IQRESET) {
				cpu.PC = RdMem(0xFFFC);
				cpu.PC |= RdMem(0xFFFD) << 8;
				cpu.S -= 3;
				cpu.jammed = 0;
				cpu.P |= I_FLAG;
				cpu.mooPI = cpu.P;
				cpu.IRQlow &= ~FCEU_IQRESET;
			} else if (cpu.IRQlow & FCEU_IQNMI2) {
				cpu.IRQlow &= ~FCEU_IQNMI2;
				cpu.IRQlow |= FCEU_IQNMI;
			} else if (cpu.IRQlow & FCEU_IQNMI) {
				if (!cpu.jammed) {
					ADDCYC(7);
					PUSH(cpu.PC >> 8);
					PUSH(cpu.PC);
					PUSH((cpu.P & ~B_FLAG) | (U_FLAG));
					cpu.P |= I_FLAG;
					cpu.PC = RdMem(0xFFFA);
					cpu.PC |= RdMem(0xFFFB) << 8;
					cpu.IRQlow &= ~FCEU_IQNMI;
				}
			} else {
				if (!(cpu.mooPI & I_FLAG) && !cpu.jammed) {
					ADDCYC(7);
					PUSH(cpu.PC >> 8);
					PUSH(cpu.PC);
					PUSH((cpu.P & ~B_FLAG) | (U_FLAG));
					cpu.P |= I_FLAG;
					cpu.PC = RdMem(0xFFFE);
					cpu.PC |= RdMem(0xFFFF) << 8;
				}
			}
			cpu.IRQlow &= ~(FCEU_IQTEMP);
			/* Should increase accuracy without a
			 * major speed hit.
			 */
			if (cpu.count <= 0) {
				cpu.mooPI = cpu.P;
				return;
			}
		}

		cpu.mooPI = cpu.P;
		cpu.opcode_PC = cpu.PC;
		cpu.opcode = RdMem(cpu.opcode_PC);
		cpu.opcode_cycles = CycTable[cpu.opcode];
		ADDCYC(cpu.opcode_cycles);
		temp = cpu.tcount;
		cpu.tcount = 0;
		cpu.PC++;

		if (MapIRQHook) MapIRQHook(temp);
		if (!ppu.overclock.overclocked_state) FCEU_SoundCPUHook(temp);

		switch (cpu.opcode) {
		/* BRK */
		case 0x00:
			cpu.PC++;
			PUSH(cpu.PC >> 8);
			PUSH(cpu.PC);
			PUSH(cpu.P | U_FLAG | B_FLAG);
			cpu.P |= I_FLAG;
			cpu.mooPI |= I_FLAG;
			cpu.PC = RdMem(0xFFFE);
			cpu.PC |= RdMem(0xFFFF) << 8;
			break;

		/* RTI */
		case 0x40:
			cpu.P = POP();
			/* cpu.mooPI=cpu.P; NOTE: This is probably incorrect, so it's
			 * commented out. */
			cpu.mooPI = cpu.P;
			cpu.PC = POP();
			cpu.PC |= POP() << 8;
			break;

		/* RTS */
		case 0x60:
			cpu.PC = POP();
			cpu.PC |= POP() << 8;
			cpu.PC++;
			break;

		/* PHA, PHP, PLA, PLP */
		case 0x48: PUSH(cpu.A); break;
		case 0x08: PUSH(cpu.P | U_FLAG | B_FLAG); break;
		case 0x68: cpu.A = POP(); X_ZN(cpu.A); break;
		case 0x28: cpu.P = POP(); break;

		/* JUMP ABSOLUTE */
		case 0x4C: {
			uint16 ptmp = cpu.PC;
			uint16 npc = RdMem(ptmp);
			ptmp++;
			npc |= RdMem(ptmp) << 8;
			cpu.PC = npc;
			break;
		}

		case 0x6C: {
			uint16 tmp;
			GetAB(tmp);
			cpu.PC = RdMem(tmp);
			cpu.PC |= RdMem(((tmp + 1) & 0x00FF) | (tmp & 0xFF00)) << 8;
			break;
		}
		/* JSR */
		case 0x20: {
			uint8 npc = RdMem(cpu.PC);
			cpu.PC++;
			PUSH(cpu.PC >> 8);
			PUSH(cpu.PC);
			cpu.PC = RdMem(cpu.PC) << 8;
			cpu.PC |= npc;
			break;
		}

		/* TAX */
		case 0xAA:
			cpu.X = cpu.A;
			X_ZN(cpu.A);
			break;

		/* TXA */
		case 0x8A:
			cpu.A = cpu.X;
			X_ZN(cpu.A);
			break;

		/* TAY */
		case 0xA8:
			cpu.Y = cpu.A;
			X_ZN(cpu.A);
			break;

		/* TYA */
		case 0x98:
			cpu.A = cpu.Y;
			X_ZN(cpu.A);
			break;

		/* TSX */
		case 0xBA:
			cpu.X = cpu.S;
			X_ZN(cpu.X);
			break;

		/* TXS */
		case 0x9A: cpu.S = cpu.X; break;

		/* DEX */
		case 0xCA:
			cpu.X--;
			X_ZN(cpu.X);
			break;

		/* DEY */
		case 0x88:
			cpu.Y--;
			X_ZN(cpu.Y);
			break;

		/* INX */
		case 0xE8:
			cpu.X++;
			X_ZN(cpu.X);
			break;

		/* INY */
		case 0xC8:
			cpu.Y++;
			X_ZN(cpu.Y);
			break;

		/* CLC */
		case 0x18: cpu.P &= ~C_FLAG; break;

		/* CLD */
		case 0xD8: cpu.P &= ~D_FLAG; break;

		/* CLI */
		case 0x58: cpu.P &= ~I_FLAG; break;

		/* CLV */
		case 0xB8: cpu.P &= ~V_FLAG; break;

		/* SEC */
		case 0x38: cpu.P |= C_FLAG; break;

		/* SED */
		case 0xF8: cpu.P |= D_FLAG; break;

		/* SEI */
		case 0x78: cpu.P |= I_FLAG; break;

		/* NOP */
		case 0xEA: break;

		case 0x0A: RMW_A(ASL);
		case 0x06: RMW_ZP(ASL);
		case 0x16: RMW_ZPX(ASL);
		case 0x0E: RMW_AB(ASL);
		case 0x1E: RMW_ABX(ASL);

		case 0xC6: RMW_ZP(DEC);
		case 0xD6: RMW_ZPX(DEC);
		case 0xCE: RMW_AB(DEC);
		case 0xDE: RMW_ABX(DEC);

		case 0xE6: RMW_ZP(INC);
		case 0xF6: RMW_ZPX(INC);
		case 0xEE: RMW_AB(INC);
		case 0xFE: RMW_ABX(INC);

		case 0x4A: RMW_A(LSR);
		case 0x46: RMW_ZP(LSR);
		case 0x56: RMW_ZPX(LSR);
		case 0x4E: RMW_AB(LSR);
		case 0x5E: RMW_ABX(LSR);

		case 0x2A: RMW_A(ROL);
		case 0x26: RMW_ZP(ROL);
		case 0x36: RMW_ZPX(ROL);
		case 0x2E: RMW_AB(ROL);
		case 0x3E: RMW_ABX(ROL);

		case 0x6A: RMW_A(ROR);
		case 0x66: RMW_ZP(ROR);
		case 0x76: RMW_ZPX(ROR);
		case 0x6E: RMW_AB(ROR);
		case 0x7E: RMW_ABX(ROR);

		case 0x69: LD_IM(ADC);
		case 0x65: LD_ZP(ADC);
		case 0x75: LD_ZPX(ADC);
		case 0x6D: LD_AB(ADC);
		case 0x7D: LD_ABX(ADC);
		case 0x79: LD_ABY(ADC);
		case 0x61: LD_IX(ADC);
		case 0x71: LD_IY(ADC);

		case 0x29: LD_IM(AND);
		case 0x25: LD_ZP(AND);
		case 0x35: LD_ZPX(AND);
		case 0x2D: LD_AB(AND);
		case 0x3D: LD_ABX(AND);
		case 0x39: LD_ABY(AND);
		case 0x21: LD_IX(AND);
		case 0x31: LD_IY(AND);

		case 0x24: LD_ZP(BIT);
		case 0x2C: LD_AB(BIT);

		case 0xC9: LD_IM(CMP);
		case 0xC5: LD_ZP(CMP);
		case 0xD5: LD_ZPX(CMP);
		case 0xCD: LD_AB(CMP);
		case 0xDD: LD_ABX(CMP);
		case 0xD9: LD_ABY(CMP);
		case 0xC1: LD_IX(CMP);
		case 0xD1: LD_IY(CMP);

		case 0xE0: LD_IM(CPX);
		case 0xE4: LD_ZP(CPX);
		case 0xEC: LD_AB(CPX);

		case 0xC0: LD_IM(CPY);
		case 0xC4: LD_ZP(CPY);
		case 0xCC: LD_AB(CPY);

		case 0x49: LD_IM(EOR);
		case 0x45: LD_ZP(EOR);
		case 0x55: LD_ZPX(EOR);
		case 0x4D: LD_AB(EOR);
		case 0x5D: LD_ABX(EOR);
		case 0x59: LD_ABY(EOR);
		case 0x41: LD_IX(EOR);
		case 0x51: LD_IY(EOR);

		case 0xA9: LD_IM(LDA);
		case 0xA5: LD_ZP(LDA);
		case 0xB5: LD_ZPX(LDA);
		case 0xAD: LD_AB(LDA);
		case 0xBD: LD_ABX(LDA);
		case 0xB9: LD_ABY(LDA);
		case 0xA1: LD_IX(LDA);
		case 0xB1: LD_IY(LDA);

		case 0xA2: LD_IM(LDX);
		case 0xA6: LD_ZP(LDX);
		case 0xB6: LD_ZPY(LDX);
		case 0xAE: LD_AB(LDX);
		case 0xBE: LD_ABY(LDX);

		case 0xA0: LD_IM(LDY);
		case 0xA4: LD_ZP(LDY);
		case 0xB4: LD_ZPX(LDY);
		case 0xAC: LD_AB(LDY);
		case 0xBC: LD_ABX(LDY);

		case 0x09: LD_IM(ORA);
		case 0x05: LD_ZP(ORA);
		case 0x15: LD_ZPX(ORA);
		case 0x0D: LD_AB(ORA);
		case 0x1D: LD_ABX(ORA);
		case 0x19: LD_ABY(ORA);
		case 0x01: LD_IX(ORA);
		case 0x11: LD_IY(ORA);

		case 0xEB: LD_IM(SBC); /* (undocumented) */
		case 0xE9: LD_IM(SBC);
		case 0xE5: LD_ZP(SBC);
		case 0xF5: LD_ZPX(SBC);
		case 0xED: LD_AB(SBC);
		case 0xFD: LD_ABX(SBC);
		case 0xF9: LD_ABY(SBC);
		case 0xE1: LD_IX(SBC);
		case 0xF1: LD_IY(SBC);

		case 0x85: ST_ZP(cpu.A);
		case 0x95: ST_ZPX(cpu.A);
		case 0x8D: ST_AB(cpu.A);
		case 0x9D: ST_ABX(cpu.A);
		case 0x99: ST_ABY(cpu.A);
		case 0x81: ST_IX(cpu.A);
		case 0x91: ST_IY(cpu.A);

		case 0x86: ST_ZP(cpu.X);
		case 0x96: ST_ZPY(cpu.X);
		case 0x8E: ST_AB(cpu.X);

		case 0x84: ST_ZP(cpu.Y);
		case 0x94: ST_ZPX(cpu.Y);
		case 0x8C: ST_AB(cpu.Y);

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
		case 0x0B: LD_IM(AND; cpu.P &= ~C_FLAG; cpu.P |= cpu.A >> 7);

		/* AAX */
		case 0x87: ST_ZP(cpu.A & cpu.X);
		case 0x97: ST_ZPY(cpu.A & cpu.X);
		case 0x8F: ST_AB(cpu.A & cpu.X);
		case 0x83: ST_IX(cpu.A & cpu.X);

		/* ARR - ARGH, MATEY! */
		case 0x6B: {
			uint8 arrtmp;
			LD_IM(AND; cpu.P &= ~V_FLAG; cpu.P |= (cpu.A ^ (cpu.A >> 1)) & 0x40;
			      arrtmp = cpu.A >> 7; cpu.A >>= 1;
			      cpu.A |= (cpu.P & C_FLAG) << 7; cpu.P &= ~C_FLAG;
			      cpu.P |= arrtmp; X_ZN(cpu.A));
		}

		/* ASR */
		case 0x4B: LD_IM(AND; LSRA);

		/* ATX(OAL) Is this(OR with $EE) correct? Blargg did some test
		   and found the constant to be OR with is $FF for NES */
		case 0xAB: LD_IM(cpu.A |= 0xFF; AND; cpu.X = cpu.A);

		/* AXS */
		case 0xCB: LD_IM(AXS);

		/* DCP */
		case 0xC7: RMW_ZP(DEC; CMP);
		case 0xD7: RMW_ZPX(DEC; CMP);
		case 0xCF: RMW_AB(DEC; CMP);
		case 0xDF: RMW_ABX(DEC; CMP);
		case 0xDB: RMW_ABY(DEC; CMP);
		case 0xC3: RMW_IX(DEC; CMP);
		case 0xD3: RMW_IY(DEC; CMP);

		/* ISB */
		case 0xE7: RMW_ZP(INC; SBC);
		case 0xF7: RMW_ZPX(INC; SBC);
		case 0xEF: RMW_AB(INC; SBC);
		case 0xFF: RMW_ABX(INC; SBC);
		case 0xFB: RMW_ABY(INC; SBC);
		case 0xE3: RMW_IX(INC; SBC);
		case 0xF3: RMW_IY(INC; SBC);

			/* DOP */

		case 0x04:
		case 0x44:
		case 0x64:
		case 0x14:
		case 0x34:
		case 0x54:
		case 0x74:
		case 0xD4:
		case 0xF4:
		case 0x80:
		case 0x82:
		case 0x89:
		case 0xC2:
		case 0xE2:
			cpu.PC++;
			break;

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
		case 0xF2:
			ADDCYC(0xFF);
			cpu.jammed = 1;
			cpu.PC--;
			break;

		/* LAR */
		case 0xBB: RMW_ABY(cpu.S &= x; cpu.A = cpu.X = cpu.S; X_ZN(cpu.X));

		/* LAX */
		case 0xA7: LD_ZP(LDA; LDX);
		case 0xB7: LD_ZPY(LDA; LDX);
		case 0xAF: LD_AB(LDA; LDX);
		case 0xBF: LD_ABY(LDA; LDX);
		case 0xA3: LD_IX(LDA; LDX);
		case 0xB3: LD_IY(LDA; LDX);

		/* NOP */
		case 0x1A:
		case 0x3A:
		case 0x5A:
		case 0x7A:
		case 0xDA:
		case 0xFA: break;

		/* RLA */
		case 0x27: RMW_ZP(ROL; AND);
		case 0x37: RMW_ZPX(ROL; AND);
		case 0x2F: RMW_AB(ROL; AND);
		case 0x3F: RMW_ABX(ROL; AND);
		case 0x3B: RMW_ABY(ROL; AND);
		case 0x23: RMW_IX(ROL; AND);
		case 0x33: RMW_IY(ROL; AND);

		/* RRA */
		case 0x67: RMW_ZP(ROR; ADC);
		case 0x77: RMW_ZPX(ROR; ADC);
		case 0x6F: RMW_AB(ROR; ADC);
		case 0x7F: RMW_ABX(ROR; ADC);
		case 0x7B: RMW_ABY(ROR; ADC);
		case 0x63: RMW_IX(ROR; ADC);
		case 0x73: RMW_IY(ROR; ADC);

		/* SLO */
		case 0x07: RMW_ZP(ASL; ORA);
		case 0x17: RMW_ZPX(ASL; ORA);
		case 0x0F: RMW_AB(ASL; ORA);
		case 0x1F: RMW_ABX(ASL; ORA);
		case 0x1B: RMW_ABY(ASL; ORA);
		case 0x03: RMW_IX(ASL; ORA);
		case 0x13: RMW_IY(ASL; ORA);

		/* SRE */
		case 0x47: RMW_ZP(LSR; EOR);
		case 0x57: RMW_ZPX(LSR; EOR);
		case 0x4F: RMW_AB(LSR; EOR);
		case 0x5F: RMW_ABX(LSR; EOR);
		case 0x5B: RMW_ABY(LSR; EOR);
		case 0x43: RMW_IX(LSR; EOR);
		case 0x53: RMW_IY(LSR; EOR);

		/* AXA - SHA */
		case 0x93: ST_IY(cpu.A & cpu.X & (((A - cpu.Y) >> 8) + 1));
		case 0x9F: ST_ABY(cpu.A & cpu.X & (((A - cpu.Y) >> 8) + 1));

		/* SYA */
		case 0x9C: {
			/* Can't reuse existing ST_ABI macro here, due to addressing
			 * weirdness. */
			uint16 A;
			GetABIWR(A, cpu.X);
			A = ((cpu.Y & ((A >> 8) + 1)) << 8) | (A & 0xff);
			WrMem(A, A >> 8);
			break;
		}

		/* SXA */
		case 0x9E: {
			/* Can't reuse existing ST_ABI macro here, due to addressing
			 * weirdness. */
			uint16 A;
			GetABIWR(A, cpu.Y);
			A = ((cpu.X & ((A >> 8) + 1)) << 8) | (A & 0xff);
			WrMem(A, A >> 8);
			break;
		}

		/* XAS */
		case 0x9B:
			cpu.S = cpu.A & cpu.X;
			ST_ABY(cpu.S & (((A - cpu.Y) >> 8) + 1));

		/* TOP */
		case 0x0C: LD_AB(;);
		case 0x1C:
		case 0x3C:
		case 0x5C:
		case 0x7C:
		case 0xDC:
		case 0xFC: LD_ABX(;);

		/* XAA - BIG QUESTION MARK HERE */
		case 0x8B:
			cpu.A |= 0xEE;
			cpu.A &= cpu.X;
			LD_IM(AND);
		}
	}
}

#ifdef FCEUDEF_DEBUGGER
X6502 XSave;	/* This is getting ugly. */

static INLINE uint8 RdMemHook(uint16 A) {
	if (cpu.ReadHook)
		return(cpu.openbus = cpu.ReadHook(&X, A));
	else
		return(cpu.openbus = ARead[A](A));
}

static INLINE void WrMemHook(uint16 A, uint8 V) {
	if (cpu.WriteHook)
		cpu.WriteHook(&X, A, V);
	else
		BWrite[A](A, V);
	cpu.openbus = V;
}
/* Called from debugger. */
void FCEUI_NMI(void) {
	cpu.IRQlow |= FCEU_IQNMI;
}

void FCEUI_IRQ(void) {
	cpu.IRQlow |= FCEU_IQTEMP;
}

void FCEUI_GetIVectors(uint16 *reset, uint16 *irq, uint16 *nmi) {
	fceuindbg = 1;

	*reset = RdMemNorm(0xFFFC);
	*reset |= RdMemNorm(0xFFFD) << 8;
	*nmi = RdMemNorm(0xFFFA);
	*nmi |= RdMemNorm(0xFFFB) << 8;
	*irq = RdMemNorm(0xFFFE);
	*irq |= RdMemNorm(0xFFFF) << 8;
	fceuindbg = 0;
}
static int debugmode;

void X6502_Debug(void (*CPUHook)(X6502 *), uint8 (*ReadHook)(X6502 *, uint32), void (*WriteHook)(X6502 *, uint32, uint8)) {
	debugmode = (ReadHook || WriteHook || CPUHook) ? 1 : 0;
	cpu.ReadHook = ReadHook;
	cpu.WriteHook = WriteHook;
	cpu.CPUHook = CPUHook;

	if (!debugmode)
		X6502_Run = X6502_RunNormal;
	else
		X6502_Run = X6502_RunDebug;
}

#endif
