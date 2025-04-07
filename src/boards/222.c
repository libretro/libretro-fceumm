/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 clockMode;
static uint8 pending;
static uint8 counter1;
static uint8 counter2;
static uint8 prescaler;

static SFORMAT Mapper222_stateRegs[] ={
	{ &clockMode, 1, "CLKM" },
	{ &pending, 1, "PEND" },
	{ &counter1, 1, "CNT1" },
	{ &counter2, 1, "CNT2" },
	{ &prescaler, 1, "PRES" },
	{ 0 }
};

static void sync () {
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

DECLFW(Mapper222_nibblizeCHR) {
	if (~A &1) {
		VRC24_writeReg(A,    V);
		VRC24_writeReg(A |1, V >>4);
	}
}

DECLFW(Mapper222_writeIRQ) {
	switch(A &3) {
		case 0: clockMode =0;
			break;
		case 1: pending =false;
		        if (!clockMode) {
				counter1 =V &0xF;
				counter2 =V >>4;
			}
			break;
		case 2:	clockMode =1;
			break;
	}
}

void FP_FASTAPASS(1) Mapper222_cpuCycle(int a) {
	while (a--) {
		uint8 previousPrescaler =prescaler;
		if (pending)
			prescaler =0;
		else
			prescaler++;
		if (clockMode && ~previousPrescaler &0x40 && prescaler &0x40) {
			if (++counter1 ==0xF && ++counter2 ==0xF) pending =1;
			counter1 &=0xF;
			counter2 &=0xF;
		}
		if (pending)
			X6502_IRQBegin(FCEU_IQEXT);
		else
			X6502_IRQEnd(FCEU_IQEXT);
	}
}

void Mapper222_power(void) {
	clockMode =pending =counter1 =counter2 =prescaler =0;
	VRC24_power();
	SetWriteHandler(0xB000, 0xEFFF, Mapper222_nibblizeCHR);
	SetWriteHandler(0xF000, 0xFFFF, Mapper222_writeIRQ);
}

void Mapper222_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 0, 0, 0);
	AddExState(Mapper222_stateRegs, ~0, 0, 0);
	info->Power =Mapper222_power;
	MapIRQHook =Mapper222_cpuCycle;
}
