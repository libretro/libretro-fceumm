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

static uint8 reg;

static void sync () {
	MMC1_syncPRG(0x07, reg <<3);
	MMC1_syncCHR(0x1F, reg <<5);
	MMC1_syncMirror();
}

static DECLFW (writeReg) {
	if (~reg &8) {
		reg = A &0xFF;
		sync();
	}
}

static void reset () {
	reg = 0;
	MMC1_clear();
}

static void power () {
	reg = 0;
	MMC1_power();
}

void Mapper499_Init (CartInfo *info) {
	MMC1_init(info, sync, MMC1_TYPE_MMC1A, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
