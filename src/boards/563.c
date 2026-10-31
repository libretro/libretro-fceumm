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

static uint8_t submapper;
static uint8_t irqEnabled;
static uint8_t irqCounter;
static uint8_t reg;

static SFORMAT Mapper563_stateRegs[] ={
	{ &irqEnabled, 1, "IRQE" },
	{ &irqCounter, 1, "CNTL" },
	{ &reg,        1, "REGS" },
	{ 0 }
};

static void sync () {
	int prgAND = submapper == 0 || reg &0x02? 0x1F: 0x0F;
	int chrAND = submapper == 0 || reg &0x02? 0xFF: 0x7F;
	int prgOR = reg <<4;
	int chrOR = reg <<7;
	VRC24_syncPRG(prgAND, prgOR &~prgAND);
	VRC24_syncCHR(chrAND, chrOR &~chrAND);
	VRC24_syncMirror();
}

static DECLFW (Mapper563_writeIRQ) {
	X6502_IRQEnd(FCEU_IQEXT);
	switch(A &0x1C) {
		case 0x0C: irqEnabled =0; break;
		case 0x08: irqEnabled =1; irqCounter = 0; break;
		case 0x18: reg = V; sync(); break;
		case 0x1C: irqCounter =0; break;
	}
}

static void Mapper563_scanline (void) { /* Actually, a sixteen-stage counter triggering on falling PA12. FCEU's PPU emulation isn't accurate enough for that, however, so just raise IRQ every two scanlines. */
	if (!(++irqCounter &1) && irqEnabled) X6502_IRQBegin(FCEU_IQEXT);
}

static void Mapper563_reset (void) {
	irqEnabled = irqCounter = reg = 0;
	VRC24_clear();
}

static void Mapper563_power (void) {
	irqEnabled = irqCounter = reg = 0;
	VRC24_power();
	SetWriteHandler(0xF000, 0xFFFF, Mapper563_writeIRQ);
}

void Mapper563_Init (CartInfo *info) {
	submapper = info->submapper;
	VRC2_init(info, sync, 0x01, 0x02, NULL, NULL, NULL, NULL);
	AddExState(Mapper563_stateRegs, ~0, 0, 0);
	info->Reset = Mapper563_reset;
	info->Power = Mapper563_power;
	GameHBIRQHook = Mapper563_scanline;
}
