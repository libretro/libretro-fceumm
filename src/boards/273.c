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
#include "asic_vrc2and4.h"

static uint8 irqEnabled;
static uint8 irqCounter;
static uint8 irqPrescaler;
static uint8 irqMask;

static SFORMAT stateRegs[] ={
	{ &irqEnabled, 1, "IRQE" },
	{ &irqCounter, 1, "CNTR" },
	{ &irqPrescaler, 1, "IRQP" },
	{ &irqMask, 1, "IRQM" },
	{ 0 }
};

static void sync () {
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

static DECLFW (writeIRQ) {
	switch(A &8) {
		case 0:
			irqCounter = V;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 8:
			irqEnabled = V;
			if (~irqEnabled &1) {
				irqPrescaler = 0;
				irqMask = 0x7F;
				X6502_IRQEnd(FCEU_IQEXT);
			}
			break;
	}
}

static void FP_FASTAPASS(1) cpuCycle (int a) {
	while (a--) {
		if (irqEnabled &1 && !(++irqPrescaler &irqMask)) {
			irqMask = 0xFF;
			if (!++irqCounter)
				X6502_IRQBegin(FCEU_IQEXT);
			else
				X6502_IRQEnd(FCEU_IQEXT);
		}
	}
}

static void power (void) {
	irqEnabled = irqCounter = irqPrescaler = irqMask = 0;
	VRC24_power();
	SetWriteHandler(0xF000, 0xFFFF, writeIRQ);
}

void Mapper273_Init (CartInfo *info) {
	VRC2_init(info, sync, 0x04, 0x08, NULL, NULL, NULL, NULL);
	AddExState(stateRegs, ~0, 0, 0);
	info->Power = power;
	MapIRQHook = cpuCycle;
}
