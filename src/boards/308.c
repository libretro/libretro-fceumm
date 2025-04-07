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

static uint8  irqEnabled;
static uint16 irqCounterLow;
static uint8  irqCounterHigh;

static SFORMAT UNLTH21311_stateRegs[] ={
	{ &irqEnabled, 1, "IRQE" },
	{ &irqCounterLow, 2 | FCEUSTATE_RLSB, "CNTL" },
	{ &irqCounterHigh, 1, "CNTH" },
	{ 0 }
};

static void sync () {
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

DECLFW(UNLTH21311_writeIRQ) {
	switch(A &3) {
	case 0:	X6502_IRQEnd(FCEU_IQEXT);
		irqEnabled =0;
		irqCounterLow =0;
		break;
	case 1:	irqEnabled =1;
		break;
	case 3:	irqCounterHigh =V >>4;
		break;
	}
}

void FP_FASTAPASS(1) UNLTH21311_cpuCycle(int a) {
	while (a--) if (irqEnabled) {
		if ((++irqCounterLow &4095) ==2048) irqCounterHigh--;
		if (!irqCounterHigh && (irqCounterLow &4095) <2048) X6502_IRQBegin(FCEU_IQEXT);
	}
}

void UNLTH21311_power(void) {
	irqEnabled =irqCounterLow =irqCounterHigh =0;
	VRC24_power();
	SetWriteHandler(0xF000, 0xFFFF, UNLTH21311_writeIRQ);
}

void UNLTH21311_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 0, 0, 0);
	AddExState(UNLTH21311_stateRegs, ~0, 0, 0);
	info->Power =UNLTH21311_power;
	MapIRQHook =UNLTH21311_cpuCycle;
}
