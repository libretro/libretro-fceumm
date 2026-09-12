/* FCEUmm - NES/Famicom Emulator
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
#include "asic_mmc3.h"

static uint8_t submapper;
static uint8_t reg;
static uint8_t pad;

static void sync () {
	int prgAND = reg &0x02? 0x0F: 0x1F;
	int chrAND = reg &0x02? 0x7F: 0xFF;
	int prgOR = reg <<4;
	int chrOR = reg <<7;
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
}

static int getPRGBank_submapper1 (uint8_t bank) {
	if ((reg &0x03) == 0x03)
		return MMC3_getPRGBank(bank) &~0x0C | reg >>2 &0x0C;
	else
	if (reg &0x01 && pad &0x01)
		return MMC3_getPRGBank(bank) &~0x04;
	else
		return MMC3_getPRGBank(bank);
}

static DECLFW (writeReg) {
	reg = A &0xFF;
	if (submapper == 0 && reg &0x01 && pad &0x01) reg |= 0x02;
	sync();
}

static void reset () {
	reg = 0;
	pad++;
	MMC3_clear();
}

static void power () {
	reg = 0;
	pad = 0;
	MMC3_power();
}

void Mapper367_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC3_init(info, sync, MMC3_TYPE_SHARP, submapper == 1? getPRGBank_submapper1: NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&pad, 1, 0, "DIPS");
}
