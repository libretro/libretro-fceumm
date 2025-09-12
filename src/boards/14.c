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

#include "mapinc.h"
#include "asic_mmc3.h"
#include "asic_vrc2and4.h"

static uint8 reg;
static uint8 init; /* Games switch between ASICs expecting registers to keep their value, so initialize each ASIC only on the first switch and use this bitfield to track it */
static void applyMode (uint8);

static SFORMAT StateRegs[] = {
	{ &reg, 1, "MODE" },
	{ &init, 1, "INIT" },
	{ 0 },
};

static void sync (void) {
	if (reg &0x02) {
		MMC3_syncPRG(0x3F, 0x00);
		MMC3_syncCHR(0x1FF, 0x00);
		MMC3_syncMirror();
	} else {
		VRC24_syncPRG(0x3F, 0x00);
		VRC24_syncCHR(0x1FF, 0x00);
		VRC24_syncMirror();
	}
}

int getCHRBank_MMC3 (uint8 bank) {
	return MMC3_getCHRBank(bank) | reg <<(~bank &4? 5: ~bank &2? 3: 1) &0x100;
}

int getCHRBank_VRC2 (uint8 bank) {
	return VRC24_getCHRBank(bank) | reg <<(~bank &4? 5: ~bank &2? 3: 1) &0x100;
}

static DECLFW (writeReg) {
	uint8 previousReg = reg;
	reg = V;
	if ((previousReg ^V) &2)
		applyMode(1);
	else
		sync();
}

static void applyMode (uint8 clear) {
	PPU_hook = NULL;
	MapIRQHook = NULL;
	GameHBIRQHook = NULL;
	if (reg &0x02) {
		MMC3_activate(clear && init &1, sync, MMC3_TYPE_AX5202P, NULL, getCHRBank_MMC3, NULL, NULL);
		init &= ~1;
	} else {
		VRC2_activate(clear && init &2, sync, 0x01, 0x02, NULL, getCHRBank_VRC2, NULL, NULL);
		init &= ~2;
	}
	SetWriteHandler(0xA131, 0xA131, writeReg);
}

static void reset (void) {
	reg = 0;
	init = 3;
	applyMode(1);
}

static void power (void) {
	reg = 0;
	init = 3;
	applyMode(1);
}

static void restore (int version) {
	applyMode(0);
}

void UNLSL1632_Init (CartInfo *info) {
	MMC3_addExState();
	VRC24_addExState();
	info->Reset = reset;
	info->Power = power;
	GameStateRestore = restore;
	AddExState(StateRegs, ~0, 0, 0);
}
