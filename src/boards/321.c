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

static void sync () {
	if (reg &0x08)
		setprg32(0x8000, reg &0x04 | reg >>4 &0x03);
	else
		MMC3_syncPRG(0x0F, reg <<2 &~0x0F);
	MMC3_syncCHR(0x7F, reg <<5 &~0x7F);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	reg = V;
	sync();
}

static void power () {
	reg = 0;
	MMC3_power();
}

static void reset () {
	reg = 0;
	MMC3_clear();
}

void Mapper321_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
