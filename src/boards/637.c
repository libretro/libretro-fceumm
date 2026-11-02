/* FCEUmm - NES/Famicom Emulator: KL003
 *
 * Copyright notice for this file:
 *  Copyright (C) 2026 NewRisingSun
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

static uint8_t reg;
static uint8_t pad;

static void sync () {
	int prgAND = 0x07;
	int chrAND = 0x1F;
	int prgOR = reg <<3;
	int chrOR = reg <<5;
	MMC1_syncPRG(prgAND, prgOR &~prgAND);
	MMC1_syncCHR(chrAND, chrOR &~chrAND);
	MMC1_syncMirror();
}

static int getPRGBank (uint8_t bank) {
	if (reg &0x04) {
		if (reg &0x02)
			return MMC1_getPRGBank(0);
		else
			return MMC1_getPRGBank(0) &~1 | bank &1;
	} else
		return MMC1_getPRGBank(bank);
}

static DECLFR (interceptPRGRead) {
	return reg &0x08? CartBR(A &~0x3 | pad &0x3): CartBR(A);
}

static DECLFW (writeReg) {
	reg = A &0xFF;
	sync();
}

static void reset () {
	reg = 0;
	pad++;
	MMC1_clear();
}

static void power () {
	reg = 0;
	pad = 0;
	MMC1_power();
	SetReadHandler(0x8000, 0xFFFF, interceptPRGRead);
}

void Mapper637_Init (CartInfo *info) {
	MMC1_init(info, sync, MMC1_TYPE_MMC1A, getPRGBank, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&pad, 1, 0, "DIPS");
}
