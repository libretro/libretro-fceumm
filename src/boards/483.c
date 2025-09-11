/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 *
 *
 */

/* 3927 PCB, reset-based MMC1/CNROM multicart. The common dump with mapper number 3927 has an unrealistic bank order. */

#include "mapinc.h"
#include "asic_latch.h"
#include "asic_mmc1.h"

static uint8 game;

static void sync_SLROM () {
	MMC1_syncPRG(0x07, game <<3);
	MMC1_syncCHR(0x1F, game <<5);
	MMC1_syncMirror();
}

static void sync_CNROM () {
	setprg32(0x8000, 0x0C +game -3);
	setchr8(0x30 | (game -3) <<2 | Latch_data &0x03);
	setmirror(MI_V);
}

static void sync_SEROM () {
	setprg32(0x8000, 0x0F);
	MMC1_syncCHR(0x07, 0x78);
	MMC1_syncMirror();
}

static void applyMode (uint8 clear) {
	if (game <= 2)
		MMC1_activate(clear, sync_SLROM, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
	else
	if (game <= 5)
		Latch_activate(clear, sync_CNROM, 0x8000, 0xFFFF, NULL);
	else
		MMC1_activate(clear, sync_SEROM, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
}

static void reset () {
	if (++game >=7) game = 0;
	applyMode(1);
}

static void power () {
	game = 0;
	applyMode(1);
}

static void stateRestore(int version) {
	applyMode(0);
}
void Mapper483_Init (CartInfo *info) {
	AddExState(&game, 1, 0, "GAME");
	Latch_addExState();
	MMC1_addExState();
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
}
