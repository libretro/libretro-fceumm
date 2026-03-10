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
	MMC3_syncPRG(0x0F, reg <<4);
	if (reg &0x04)
		setchr8r(0x10, 0);
	else
		MMC3_syncCHR(0x7F, reg <<7);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	reg = A &0xFF;
	sync();
}

static void reset () {
	reg = 0;
	MMC3_clear();
}

static void power () {
	reg = 0;
	MMC3_power();
}

void Mapper616_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, writeReg);
	CHRRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
