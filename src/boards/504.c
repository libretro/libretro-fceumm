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
	int prgAND = reg &0x02? 0x0F: 0x1F;
	int chrAND = reg &0x02? 0x7F: 0xFF;
	int prgOR = reg <<4;
	int chrOR = reg <<7;
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
}

static int getPRGBank(uint8 bank) {
	if ((reg &0x03) == 0x03 && ~reg &0x08)
		return MMC3_getPRGBank(bank &1) &~1 | bank &1;
	else
		return MMC3_getPRGBank(bank);
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

void Mapper504_Init(CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, getPRGBank, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
