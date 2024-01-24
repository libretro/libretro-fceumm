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

#include "fceu.h"
#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "sound.h"

X6502 cpu;

uint32 timestamp;
uint32 sound_timestamp;
void (*MapIRQHook)(int a);

#define ADDCYC(x) {									\
	int __x = x;									\
	cpu.tcount += __x;									\
	cpu.count -= __x * 48;								\
	timestamp += __x;  \
	if (!overclocked) sound_timestamp +=  __x; \
}

static INLINE uint8 RdMemNorm(uint32 A) {
	return ((cpu.openbus = ARead[A](A)));
}

static INLINE void WrMemNorm(uint32 A, uint8 V) {
	BWrite[A](A, V);
}

static INLINE uint8 RdRAMFast(uint32 A) {
	return ((cpu.openbus = RAM[A]));
}

static INLINE void WrRAMFast(uint32 A, uint8 V) {
	RAM[A] = V;
	cpu.openbus = V;
}

uint8 X6502_DMR(uint32 A) {
	ADDCYC(1);
	return(cpu.openbus = ARead[A](A));
}

void X6502_DMW(uint32 A, uint8 V) {
	ADDCYC(1);
	BWrite[A](A, V);
}

#define PUSH(V) {									\
	uint8 VTMP = V;									\
	WrRAM(0x100 + cpu.S, VTMP);						\
	cpu.S--;											\
}

#define POP() RdRAM(0x100 + (++cpu.S))

static uint8 ZNTable[256];
/* Some of these operations will only make sense if you know what the flag constants are. */

#define X_ZN(zort)  cpu.P &= ~(Z_FLAG | N_FLAG); cpu.P |= ZNTable[zort]
#define X_ZNT(zort) cpu.P |= ZNTable[zort]

#define JR(cond) {									\
	if (cond)										\
	{												\
		uint32 tmp;									\
		int32 disp;									\
		disp = (int8)RdMem(cpu.PC);					\
		cpu.PC++;										\
		ADDCYC(1);									\
		tmp = cpu.PC;									\
		cpu.PC += disp;								\
		if ((tmp ^ cpu.PC) & 0x100)					\
			ADDCYC(1);								\
	} else cpu.PC++;									\
}

#define LDA     cpu.A = x; X_ZN(cpu.A)
#define LDX     cpu.X = x; X_ZN(cpu.X)
#define LDY     cpu.Y = x; X_ZN(cpu.Y)

/* All of the freaky arithmetic operations. */
#define AND     cpu.A &= x; X_ZN(cpu.A)
#define BIT     cpu.P &= ~(Z_FLAG | V_FLAG | N_FLAG); cpu.P |= ZNTable[x & cpu.A] & Z_FLAG; cpu.P |= x & (V_FLAG | N_FLAG)
#define EOR     cpu.A ^= x; X_ZN(cpu.A)
#define ORA     cpu.A |= x; X_ZN(cpu.A)

#define ADC {										\
	uint32 l = cpu.A + x + (cpu.P & 1);					\
	cpu.P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);		\
	cpu.P |= ((((cpu.A ^ x) & 0x80) ^ 0x80) & ((cpu.A ^ l) & 0x80)) >> 1;  \
	cpu.P |= (l >> 8) & C_FLAG;						\
	cpu.A = l;											\
	X_ZNT(cpu.A);										\
}

#define SBC {										\
	uint32 l = cpu.A - x - ((cpu.P & 1) ^ 1);				\
	cpu.P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);		\
	cpu.P |= ((cpu.A ^ l) & (cpu.A ^ x) & 0x80) >> 1;		\
	cpu.P |= ((l >> 8) & C_FLAG) ^ C_FLAG;				\
	cpu.A = l;											\
	X_ZNT(cpu.A);										\
}

#define CMPL(a1, a2) {								\
	uint32 t = a1 - a2;								\
	X_ZN(t & 0xFF);									\
	cpu.P &= ~C_FLAG;									\
	cpu.P |= ((t >> 8) & C_FLAG) ^ C_FLAG;				\
}

/* Special undocumented operation.  Very similar to CMP. */
#define AXS {										\
	uint32 t = (cpu.A & cpu.X) - x;						\
	X_ZN(t & 0xFF);									\
	cpu.P &= ~C_FLAG;									\
	cpu.P |= ((t >> 8) & C_FLAG) ^ C_FLAG;				\
	cpu.X = t;											\
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

#define ROL {										\
	uint8 l = x >> 7;								\
	x <<= 1;										\
	x |= cpu.P & C_FLAG;								\
	cpu.P &= ~(Z_FLAG | N_FLAG | C_FLAG);				\
	cpu.P |= l;										\
	X_ZNT(x);										\
}

#define ROR {										\
	uint8 l = x & 1;								\
	x >>= 1;										\
	x |= (cpu.P & C_FLAG) << 7;						\
	cpu.P &= ~(Z_FLAG | N_FLAG | C_FLAG);				\
	cpu.P |= l;										\
	X_ZNT(x);										\
}

/* Icky icky thing for some undocumented instructions.  Can easily be
 * broken if names of local variables are changed.
 */

/* Absolute */
#define GetAB(target) {								\
	target = RdMem(cpu.PC);							\
	cpu.PC++;											\
	target |= RdMem(cpu.PC) << 8;						\
	cpu.PC++;											\
}

/* Absolute Indexed(for reads) */
#define GetABIRD(target, i) {						\
	uint32 tmp;										\
	GetAB(tmp);										\
	target = tmp;									\
	target += i;									\
	if ((target ^ tmp) & 0x100) {					\
		target &= 0xFFFF;							\
		RdMem(target ^ 0x100);						\
		ADDCYC(1);									\
	}												\
}

/* Absolute Indexed(for writes and rmws) */
#define GetABIWR(target, i) {						\
	uint32 rt;										\
	GetAB(rt);										\
	target = rt;									\
	target += i;									\
	target &= 0xFFFF;								\
	RdMem((target & 0x00FF) | (rt & 0xFF00));		\
}

/* Zero Page */
#define GetZP(target) {								\
	target = RdMem(cpu.PC);							\
	cpu.PC++;											\
}

/* Zero Page Indexed */
#define GetZPI(target, i) {							\
	target = i + RdMem(cpu.PC);						\
	cpu.PC++;											\
}

/* Indexed Indirect */
#define GetIX(target) {								\
	uint8 tmp;										\
	tmp = RdMem(cpu.PC);								\
	cpu.PC++;											\
	tmp += cpu.X;										\
	target = RdRAM(tmp);							\
	tmp++;											\
	target |= RdRAM(tmp) << 8;						\
}

/* Indirect Indexed(for reads) */
#define GetIYRD(target) {							\
	uint32 rt;										\
	uint8 tmp;										\
	tmp = RdMem(cpu.PC);								\
	cpu.PC++;											\
	rt = RdRAM(tmp);								\
	tmp++;											\
	rt |= RdRAM(tmp) << 8;							\
	target = rt;									\
	target += cpu.Y;									\
	if ((target ^ rt) & 0x100) {					\
		target &= 0xFFFF;							\
		RdMem(target ^ 0x100);						\
		ADDCYC(1);									\
	}												\
}

/* Indirect Indexed(for writes and rmws) */
#define GetIYWR(target) {							\
	uint32 rt;										\
	uint8 tmp;										\
	tmp = RdMem(cpu.PC);								\
	cpu.PC++;											\
	rt = RdRAM(tmp);								\
	tmp++;											\
	rt |= RdRAM(tmp) << 8;							\
	target = rt;									\
	target += cpu.Y;									\
	target &= 0xFFFF;								\
	RdMem((target & 0x00FF) | (rt & 0xFF00));		\
}

/* Now come the macros to wrap up all of the above stuff addressing mode functions
and operation macros.  Note that operation macros will always operate(redundant
redundant) on the variable "x".
*/

#define RMWCPU_A(op)       { uint8 x = cpu.A; op; cpu.A = x; break; }	/* Meh... */
#define RMWCPU_AB(op)      { uint32 A; uint8 x; GetAB(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMWCPU_ABI(reg, op) { uint32 A; uint8 x; GetABIWR(A, reg); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMWCPU_ABX(op)     RMWCPU_ABI(cpu.X, op)
#define RMWCPU_ABY(op)     RMWCPU_ABI(cpu.Y, op)
#define RMW_IX(op)      { uint32 A; uint8 x; GetIX(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_IY(op)      { uint32 A; uint8 x; GetIYWR(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ZP(op)      { uint8 A; uint8 x; GetZP(A); x = RdRAM(A); op; WrRAM(A, x); break; }
#define RMW_ZPX(op)     { uint8 A; uint8 x; GetZPI(A, cpu.X); x = RdRAM(A); op; WrRAM(A, x); break; }

#define LD_IM(op)       { uint8 x; x = RdMem(cpu.PC); cpu.PC++; op; break; }
#define LD_ZP(op)       { uint8 A; uint8 x; GetZP(A); x = RdRAM(A); op; break; }
#define LD_ZPX(op)      { uint8 A; uint8 x; GetZPI(A, cpu.X); x = RdRAM(A); op; break; }
#define LD_ZPY(op)      { uint8 A; uint8 x; GetZPI(A, cpu.Y); x = RdRAM(A); op; break; }
#define LDCPU_AB(op)       { uint32 A; uint8 x; GetAB(A); x = RdMem(A); op; break; }
#define LDCPU_ABI(reg, op) { uint32 A; uint8 x; GetABIRD(A, reg); x = RdMem(A); op; break; }
#define LDCPU_ABX(op)      LDCPU_ABI(cpu.X, op)
#define LDCPU_ABY(op)      LDCPU_ABI(cpu.Y, op)
#define LD_IX(op)       { uint32 A; uint8 x; GetIX(A); x = RdMem(A); op; break; }
#define LD_IY(op)       { uint32 A; uint8 x; GetIYRD(A); x = RdMem(A); op; break; }

#define ST_ZP(r)        { uint8 A; GetZP(A); WrRAM(A, r); break; }
#define ST_ZPX(r)       { uint8 A; GetZPI(A, cpu.X); WrRAM(A, r); break; }
#define ST_ZPY(r)       { uint8 A; GetZPI(A, cpu.Y); WrRAM(A, r); break; }
#define STCPU_AB(r)        { uint32 A; GetAB(A); WrMem(A, r); break; }
#define STCPU_ABI(reg, r)  { uint32 A; GetABIWR(A, reg); WrMem(A, r); break; }
#define STCPU_ABX(r)       STCPU_ABI(cpu.X, r)
#define STCPU_ABY(r)       STCPU_ABI(cpu.Y, r)
#define ST_IX(r)        { uint32 A; GetIX(A); WrMem(A, r); break; }
#define ST_IY(r)        { uint32 A; GetIYWR(A); WrMem(A, r); break; }

static uint8 CycTable[256] =
{
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
/*0xF0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
};

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
	int x;

	memset((void*)&cpu, 0, sizeof(cpu));
	for (x = 0; x < 256; x++)
		if (!x)
			ZNTable[x] = Z_FLAG;
		else if (x & 0x80)
			ZNTable[x] = N_FLAG;
		else
			ZNTable[x] = 0;
}

void X6502_Power(void) {
	cpu.count = cpu.tcount = cpu.IRQlow = cpu.PC = cpu.A = cpu.X = cpu.Y = cpu.P = cpu.mooPI = cpu.openbus = cpu.jammed = 0;
	cpu.S     = 0xFD;
	timestamp = sound_timestamp = 0;
	X6502_Reset();
}

void X6502_Run(int32 cycles)
{
	#define RdRAM RdMemNorm
	#define WrRAM WrMemNorm
	#define RdMem RdMemNorm
	#define WrMem WrMemNorm

	if (PAL)
		cycles *= 15;	/* 15*4=60 */
	else
		cycles *= 16;	/* 16*4=64 */

	cpu.count += cycles;

	while (cpu.count > 0) {
		int32 temp;
		uint8 b1;

		if (cpu.IRQlow) {
			if (cpu.IRQlow & FCEU_IQRESET) {
				cpu.PC = RdMem(0xFFFC);
				cpu.PC |= RdMem(0xFFFD) << 8;
				cpu.jammed = 0;
				cpu.mooPI = cpu.P = I_FLAG;
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
			if (cpu.count <= 0) {
				cpu.mooPI = cpu.P;
				return;
			}	/* Should increase accuracy without a
				 * major speed hit.
				 */
		}

		cpu.mooPI = cpu.P;
		b1 = RdMem(cpu.PC);

		ADDCYC(CycTable[b1]);

		temp = cpu.tcount;
		cpu.tcount = 0;
		if (MapIRQHook) MapIRQHook(temp);
		if (!overclocked)
			FCEU_SoundCPUHook(temp);
		cpu.PC++;
		switch (b1) {
			#include "x6502ops.inc"
		}
	}

	#undef RdRAM
	#undef WrRAM
}
