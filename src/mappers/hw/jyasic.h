/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

#ifndef _JYASIC_H
#define _JYASIC_H

typedef struct __JYASIC {
	uint8 mode[4];
	uint8 prg[4];
	uint8 mul[2];
	uint8 adder;
	uint8 test;
	uint8 latch[2];
	uint16 chr[8];
	uint16 nt[4];
	struct irq {
		uint8 control;
		uint8 enable;
		uint8 prescaler;
		uint8 counter;
		uint8 xor;
	} irq;
} JYASIC;

extern JYASIC jyasic;

extern uint8     JYASIC_CPUWriteHandlersSet;
extern writefunc JYASIC_cpuWrite[0x10000];

DECLFR(JYASIC_ReadALU_DIP);
DECLFW(JYASIC_trapCPUWrite);
DECLFW(JYASIC_WriteALU);
DECLFW(JYASIC_WritePRG);
DECLFW(JYASIC_WriteCHRLow);
DECLFW(JYASIC_WriteCHRHigh);
DECLFW(JYASIC_WriteNT);
DECLFW(JYASIC_WriteIRQ);
DECLFW(JYASIC_WriteMode);

void JYASIC_restoreWriteHandlers(void);
void JYASIC_RegReset(void);
void JYASIC_Reset(void);
void JYASIC_Close(void);
void JYASIC_Power(void);
void JYASIC_Init(CartInfo * info, int extended_mirr);

void JYASIC_FixPRG(void);
void JYASIC_FixCHR(void);
void JYASIC_FixMIR(void);

extern void (*JYASIC_pwrap)(uint16 A, uint16 V);
extern void (*JYASIC_wwrap)(uint16 A, uint16 V);
extern void (*JYASIC_cwrap)(uint16 A, uint16 V);

extern uint32 (*JYASIC_GetPRGBank)(uint32 V);
extern uint32 (*JYASIC_GetCHRBank)(uint32 V);

#endif /* _JYASIC_H */
