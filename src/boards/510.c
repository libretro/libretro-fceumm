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
#include "wram.h"

static uint8 reg;

static void sync () {
	int prgAND = reg &0x20? 0x0F: 0x1F;
	int chrAND = reg &0x01? 0xFF: 0x7F;
	int prgOR = (reg &0x08? 0x020: 0x000) | (reg &0x40? 0x010: 0x000);
	int chrOR = (reg &0x04? 0x100: 0x000) | (reg &0x02? 0x080: 0x000);
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncWRAM(0);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	if (reg == 0) {
		reg = V;
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

void Mapper510_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
