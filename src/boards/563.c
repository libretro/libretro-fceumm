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

static uint8 irqEnabled;
static uint8 irqCounter;

static SFORMAT Mapper563_stateRegs[] ={
	{ &irqEnabled, 1, "IRQE" },
	{ &irqCounter, 1, "CNTL" },
	{ 0 }
};

static void sync () {
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

DECLFW(Mapper563_writeIRQ) {
	X6502_IRQEnd(FCEU_IQEXT);
	switch(A &0x1C) {
		case 0x0C: irqEnabled =0; break;
		case 0x08: irqEnabled =1; break;
		case 0x1C: irqCounter =0; break;
	}
}

static void Mapper563_scanline (void) { /* Actually, a sixteen-stage counter triggering on falling PA12. FCEU's PPU emulation isn't accurate enough for that, however, so just raise IRQ every two scanlines. */
	if (!(++irqCounter &1) && irqEnabled) X6502_IRQBegin(FCEU_IQEXT);
}

void Mapper563_power(void) {
	irqEnabled =irqCounter =0;
	VRC24_power();
	SetWriteHandler(0xF000, 0xFFFF, Mapper563_writeIRQ);
}

void Mapper563_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 0, 0, 0);
	AddExState(Mapper563_stateRegs, ~0, 0, 0);
	info->Power =Mapper563_power;
	GameHBIRQHook = Mapper563_scanline;
}
