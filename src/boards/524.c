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
static uint16 irqCounter;

static SFORMAT BTL900218_stateRegs[] ={
	{ &irqEnabled, 1, "IRQE" },
	{ &irqCounter, 2 | FCEUSTATE_RLSB, "CNTL" },
	{ 0 }
};

static void sync () {
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

DECLFW(BTL900218_writeIRQ) {
	if (A &8) {
		if (A &4) {
			irqEnabled =0;
			irqCounter =0;
			X6502_IRQEnd(FCEU_IQEXT);
		} else
			irqEnabled =1;	
	}
}

void FP_FASTAPASS(1) BTL900218_cpuCycle(int a) {
	while (a--) if (irqEnabled && ++irqCounter &1024) X6502_IRQBegin(FCEU_IQEXT);
}

void BTL900218_power(void) {
	irqEnabled =irqCounter =0;
	VRC24_power();
	SetWriteHandler(0xF000, 0xFFFF, BTL900218_writeIRQ);
}

void BTL900218_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 0, 0, 0);
	AddExState(BTL900218_stateRegs, ~0, 0, 0);
	info->Power =BTL900218_power;
	MapIRQHook =BTL900218_cpuCycle;
}
