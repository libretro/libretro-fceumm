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

static uint8 reg;

static void sync() {
	MMC3_syncPRG(0x0F, reg <<4);
	if (reg == 0x0F)
		MMC3_syncCHR(0x1FF, 0x400);
	else
		MMC3_syncCHR(0x07F, reg <<7 &0x380);
	MMC3_syncMirror();
}

static int getPRGBank(uint8 bank) {
	if (reg == 0x08) {
		return MMC3_getPRGBank(bank &1) &~3 | bank &3;
	} else
		return MMC3_getPRGBank(bank);
}

static int getCHRBank(uint8 bank) {
	if (reg == 0x0F)
		return MMC3_getCHRBank(bank &6 | bank >>1 &1) <<1 | bank &1;
	else
		return MMC3_getCHRBank(bank);
}

static DECLFW(writeReg) {
	reg = A &0xFF;
	sync();
}

static void reset() {
	reg = 0;
	sync();
}

static void power() {
	reg = 0;
	MMC3_power();
}

void Mapper567_Init(CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_SHARP, getPRGBank, getCHRBank, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
