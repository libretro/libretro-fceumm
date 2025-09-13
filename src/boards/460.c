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

#include "mapinc.h"
#include "asic_mmc3.h"
#include "cartram.h"

static uint8 submapper;
static uint8 reg;
static uint8 pad;

static DECLFR (readPad) {
	return CartBR(A &~3 | pad &3);
}

static void sync () {
	MMC3_syncPRG(0x0F, reg <<4 &~0x0F);
	if (reg &0x04)
		MMC3_syncCHR(0x1FF, 0x000);
	else
		setchr8r(0x10, 0);
	MMC3_syncMirror();
	SetReadHandler(0x8000, 0xFFFF, submapper == 0 && reg &0x80 || submapper == 1 && reg &0x20? readPad: CartBR);
}

static int getPRGBank (uint8 bank) {
	if (reg &0x08) {
		int mask = reg &0x10? 3: 1;
		return MMC3_getPRGBank(bank &1) &~mask | bank &mask;
	} else
		return MMC3_getPRGBank(bank);
}

static int getCHRBank (uint8 bank) {
	if (reg &0x04)
		return MMC3_getCHRBank(bank &6 | bank >>1 &1) <<1 | bank &1;
	else
		return MMC3_getCHRBank(bank);
}

static DECLFW (writeReg) {
	reg = A &0xFF;
	sync();
}

static void reset () {
	reg = 0;
	pad++;
	MMC3_clear();
}

static void power () {
	reg = 0;
	pad = 0;
	MMC3_power();
}

void Mapper460_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC3_init(info, sync, MMC3_TYPE_SHARP, getPRGBank, getCHRBank, NULL, writeReg);
	CHRRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&reg, 1, 0, "DIPS");
}
