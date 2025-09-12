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

static uint8 reg[4];

static void sync () {
	int prgAND = reg[1] &0x20? 0x1F: 0x0F;
	int chrAND = reg[1] &0x40? 0xFF: 0x7F;
	int prgOR = (reg[1] &0x02? 0x10: 0x00) | (reg[1] &0x10? 0x020: 0x00);
	int chrOR = (reg[1] &0x02? 0x80: 0x00) | (reg[1] &0x10? 0x100: 0x00);
	if (reg[0] &0x80) {
		if (reg[0] &0x20)
			setprg32(0x8000, reg[0] >>1);
		else {
			setprg16(0x8000, reg[0]);
			setprg16(0xC000, reg[0]);
		}
	} else
		MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	if (~reg[1] &0x01) {
		reg[A &3] = V;
		sync();
	}
}

static DECLFW (unscramble) {
	static const uint16 lutAddress[8] = { 0xA001, 0xA000, 0x8000, 0xC000, 0x8001, 0xC001, 0xE000, 0xE001 }; /* i <5? 4-i: i */
	static const uint8 lutIndex[8] = { 0, 3, 1, 5, 6, 7, 2, 4 }; /* i <6? (i^3)-1: i */
	MMC3_writeReg(lutAddress[A >>12 &6 | A &1], (A &0xE001) == 0xA000? lutIndex[V &7]: V);
}

static void reset () {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_clear();
}

static void power () {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_power();
	SetWriteHandler(0x8000, 0xFFFF, unscramble);
}

void Mapper182_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(reg, 4, 0, "EXPR");
}
