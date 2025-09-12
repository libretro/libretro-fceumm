/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

/* NES 2.0 Mapper 556
 * Used for the for the 餓狼傳說 激鬥篇 HiK 7-in-1 (JY-215) multicart.
 */

#include "mapinc.h"
#include "asic_mmc3.h"
#include "asic_vrc2and4.h"
#include "cartram.h"

static uint8 reg[4];
static uint8 index;

static SFORMAT StateRegs[] = {
	{ reg,            5, "REGS" },
	{ &index,         1, "INDX" },
	{ 0 },
};

static void sync (void) {
	int prgAND = ~reg[3] &0x3F;
	int chrAND = 0xFF >>(~reg[2] &0xF);
	int prgOR  = (reg[1] | reg[3] <<2 &0x100) &~prgAND;
	int chrOR  = (reg[0] | reg[2] <<4 &0x0F00 | reg[3] <<6 &0x1000) &~chrAND;
	if (reg[2] &0x80) {
		VRC24_syncWRAM(0);
		VRC24_syncPRG(prgAND, prgOR);
		VRC24_syncCHR(chrAND, chrOR);
		VRC24_syncMirror();
	} else {
		MMC3_syncWRAM(0);
		MMC3_syncPRG(prgAND, prgOR);
		MMC3_syncCHR(chrAND, chrOR);
		MMC3_syncMirror();
	}
}

static void applyMode (uint8 clear) {
	if (reg[2] &0x80)
		VRC4_activate(clear, sync, 0x05, 0x0A, 1, NULL, NULL, NULL, NULL, NULL);
	else
		MMC3_activate(clear, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, NULL);
}

static DECLFW(writeReg) {
	if (~reg[3] &0x80) {
		reg[index++ &3] = V;
		if (index == 3)
			applyMode(1);
		else
			sync();
	}
}

static void reset (void) {
	reg[0] = reg[1] = reg[3] = 0;
	reg[2] = 0x0F;
	index = 0;
	applyMode(1);
}

static void power (void) {
	reg[0] = reg[1] = reg[3] = 0;
	reg[2] = 0x0F;
	index = 0;
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	applyMode(1);
}

static void restore (int version) {
	applyMode(0);
}

void Mapper556_Init (CartInfo *info) {
	MMC3_addExState();
	VRC24_addExState();
	WRAM_init(info, 8);
	info->Reset = reset;
	info->Power = power;
	GameStateRestore = restore;
	AddExState(StateRegs, ~0, 0, 0);
}
