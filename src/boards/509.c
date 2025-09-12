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

static uint8 reg;

static void sync () {
	int prgAND = reg &0x10? 0x1F: 0x0F;
	int chrAND = reg &0x10? 0xFF: 0x7F;
	int prgOR = reg <<1;
	int chrOR = reg <<4;
	if (reg &0x20)
		MMC3_syncPRG(prgAND, prgOR &~prgAND);
	else
	if (reg &0x06)
		setprg32(0x8000, reg >>1);
	else {
		setprg16(0x8000, reg);
		setprg16(0xC000, reg);
	}
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncWRAM(0);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	if (~reg &0x20) {
		reg = A &0xFF;
		sync();
	}
}

static void reset () {
	reg = 0;
	MMC3_clear();
}

static void power () {
	reg = 0;
	MMC3_power();
}

void Mapper509_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
