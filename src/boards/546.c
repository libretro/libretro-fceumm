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
#include "asic_mmc1.h"
#include "wram.h"

static uint8 reg;

static void sync () {
	MMC1_syncWRAM(0);
	if (reg &0x10)
		MMC1_syncPRG(0x0F, 0x10);
	else
	if (reg &0x20)
		setprg32(0x8000, reg >>1);
	else {
		setprg16(0x8000, reg);
		setprg16(0xC000, reg);
	}
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], reg &0x80? 0: 1);
	setchr8(0);
	MMC1_syncMirror();
}

static DECLFW (writeReg) {
	if (!(A &0x0F00)) {
		reg = A &0xFF;
		sync();
	}
	MMC1_writeReg(A, V);
}

static void reset () {
	reg = 0;
	MMC1_clear();
}

static void power () {
	reg = 0;
	MMC1_power();
	SetWriteHandler(0x8000, 0x9FFF, writeReg);
}

void Mapper546_Init (CartInfo *info) {
	MMC1_init(info, sync, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
