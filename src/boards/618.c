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
static uint8_t reg[2];

static void sync0 () {
	if (reg[0] &0x02)
		MMC3_syncPRG(0x0F, reg[0] >>1 &0x20 | reg[0] <<2 &0x10);
	else
		setprg32(0x8000, reg[0] >>3 &0x08 | reg[0] &0x04 | reg[0] >>4 &0x01 | reg[0] <<1 &0x02);
	if (reg[0] &0x80)
		MMC3_syncCHR(0x1FF, 0x200);
	else
		MMC3_syncCHR(0x0FF, reg[0] <<6 &0x100);	
	MMC3_syncMirror();
}

static void sync1 () {
	MMC3_syncPRG(0x0F, reg[0] <<4);
	if (reg[0] &0x80)
		MMC3_syncCHR(0x1FF, 0x200);
	else
		MMC3_syncCHR(0x0FF, reg[0] <<7 &0x100);	
	MMC3_syncMirror();
}

static void sync2 (void) {
	int chrAND = reg[1] &0x04? 0xFF: reg[0] &0x80? 0x3F: 0x7F;
	int chrOR = reg[0] <<5 &0x100 | reg[0] <<1 &0x80 | reg[0] >>1 &0x40;
	int prgOR = reg[0] <<3 &0x30;
	if (reg[1] &0x04)
		setprg32(0x8000, prgOR >>2 | reg[0] >>4 &0x01 | reg[0] <<1 &0x02);
	else
		MMC3_syncPRG(0x0F, prgOR);
	
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	if (reg[1] &0x08)
		setmirror(MMC3_getCHRBank(0) &0x80? MI_1: MI_0);
	else
		MMC3_syncMirror();
}

static int getCHRBank (uint8_t bank) {
	if (reg[0] &0x80)
		return MMC3_getCHRBank(bank &4 | bank >>1 &1) <<1 | bank &1;
	else
		return MMC3_getCHRBank(bank);
}


static DECLFW (writeReg) {
	reg[0] = V;
	reg[1] = A &0xFF;
	MMC3_restore(FCEU_VERSION_NUMERIC); /* Calls MMC3_cbSync() */
}

static void reset () {
	reg[0] = submapper == 2? 0x00: submapper == 1? 0xC3: 0xC2;
	reg[1] = 0x01;
	MMC3_clear();
}

static void power () {
	reg[0] = submapper == 2? 0x00: submapper == 1? 0xC3: 0xC2;
	reg[1] = 0x01;
	MMC3_power();
}

void Mapper618_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC3_init(info, submapper == 2? sync2: submapper == 1? sync1: sync0, MMC3_TYPE_SHARP, NULL, submapper < 2? getCHRBank: NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, submapper < 2? 1: 2, 0, "EXPR");
}
