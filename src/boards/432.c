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

static uint8 submapper;
static uint8 reg[2];
static uint8 pad;

static DECLFR(readPad) {
	return pad;
}

static void sync() {
	int prgAND =reg[1] &0x02? 0x0F: 0x1F;
	int chrAND =reg[1] &0x20 && submapper == 3? 0x1FF: reg[1] &0x04? 0x7F: 0xFF;
	int prgOR  =reg[1] <<4 &0x10 | reg[1] <<1 &0x60;
	int chrOR  =reg[1] <<7 &0x80 | reg[1] <<5 &0x100 | reg[1] <<4 &0x200;
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
	SetReadHandler(0x8000, 0xFFFF, submapper == 1 && reg[1] &0x20 || submapper != 1 && reg[0] &0x01? readPad: CartBR);
}

static int getPRGBank(uint8 bank) {
	if (reg[1] &0x40) {
		int mask = reg[1] &(submapper == 2? 0x20: 0x80)? 3: 1;
		return MMC3_getPRGBank(bank & 1) &~mask | bank &mask;
	} else
		return MMC3_getPRGBank(bank);
}

static int getCHRBank(uint8 bank) {
	if (reg[1] &0x20 && submapper == 3)
		return MMC3_getCHRBank(bank &6 | bank >>1 &1) <<1 | bank &1;
	else
		return MMC3_getCHRBank(bank);
}

static DECLFW(writeReg) {
	if (submapper == 3 && reg[1] &0x80)
		;
	else {
		reg[A &1] = V;
		sync();
	}
}

static void reset() {
	reg[0] = reg[1] = 0;
	++pad;
	sync();
}

static void power() {
	reg[0] = reg[1] = 0;
	pad = 0;
	MMC3_power();
}

void Mapper432_Init(CartInfo *info) {
	submapper =info->submapper;
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, getPRGBank, getCHRBank, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(reg, 2, 0, "EXPR");
}
