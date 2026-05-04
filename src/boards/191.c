/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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
#include "cartram.h"

static uint8_t submapper;
static uint8_t reg;

static void sync () {
	int bank;
	MMC3_syncWRAM(0);
	MMC3_syncPRG(0x3F, 0);
	if ((reg &0x03) == 0x03) for (bank = 0; bank < 8; bank++) {
		int val = MMC3_getCHRBank(bank);
		setchr1r(val &0x80? 0x10: 0x00, bank <<10, val &0x7F);
	} else
		MMC3_syncCHR(0x7F, 0x80);
	MMC3_syncMirror();
}

static int getPRGBank (uint8_t bank) {
	if (bank &2)
		return MMC3_getPRGBank(bank) &~6 | reg <<1 &6;
	else
		return MMC3_getPRGBank(bank);
}

static DECLFW (writeReg) {
	reg = V;
	sync();
}

static void reset () {
	RAM[0x110] =0;
	reg = submapper == 1? 0x00: 0x03;
	MMC3_clear();
}

static void power () {
	MMC3_power();
	SetWriteHandler(0x90AA, 0x90AA, writeReg);
	reset();
}

void Mapper191_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC3_init(info, sync, MMC3_TYPE_SHARP, getPRGBank, NULL, NULL, NULL);
	CartRAM_init(info, 8, 2);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
