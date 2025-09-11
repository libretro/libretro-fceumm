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
	MMC3_syncWRAM(0);
	MMC3_syncPRG(0x7F, 0x00);
	MMC3_syncCHR(0x7F, reg <<7 | ~reg <<4 &0x40);
	MMC3_syncMirror();
}

static int getPRGBank (uint8 bank) {
	int result = MMC3_getPRGBank(bank);
	if (reg &0x08 && ~result &0x10)
		result =          0x40 | result &0x0F;
	else
		result = reg <<4 &0x30 | result &0x0F;
	return result;
}

static DECLFW (writeReg) {
	if (A &0x020 && A &0x100) {
		reg = V;
		sync();
	} else
		MMC3_writeReg(A, V);
}

static void reset () {
	reg = 0;
	MMC3_clear();
}

static void power () {
	reg = 0;
	MMC3_power();
	SetWriteHandler(0xF000, 0xFFFF, writeReg);
}

void Mapper545_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, getPRGBank, NULL, NULL, NULL);
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
