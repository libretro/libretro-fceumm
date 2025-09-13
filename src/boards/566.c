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
	int prgAND = reg &0x04? 0x0F: 0x1F;
	int chrAND = reg &0x02? 0x7F: 0xFF;
	int prgOR = reg <<4 &0x10 | reg <<2 &0x20 | reg <<1 &0xC0;
	int chrOR = reg <<7 &0x80 | reg <<4 &0x700;
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	if (reg &0x80)
		setmirror(MMC3_getCHRBank(0) &0x80? MI_1: MI_0);
	else
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

void Mapper566_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
