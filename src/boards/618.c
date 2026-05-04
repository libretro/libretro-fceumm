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

static uint8_t reg;

static void sync () {
	if (reg &0x02)
		MMC3_syncPRG(0x0F, reg >>1 &0x20 | reg <<2 &0x10);
	else
		setprg32(0x8000, reg >>3 &0x08 | reg &0x04 | reg >>4 &0x01 | reg <<1 &0x02);
	if (reg &0x80)
		MMC3_syncCHR(0x1FF, 0x200);
	else
		MMC3_syncCHR(0x0FF, reg <<6 &0x100);	
	MMC3_syncMirror();
}

static int getCHRBank (uint8_t bank) {
	if (reg &0x80)
		return MMC3_getCHRBank(bank &4 | bank >>1 &1) <<1 | bank &1;
	else
		return MMC3_getCHRBank(bank);
}


static DECLFW (writeReg) {
	reg = V;
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

void Mapper618_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_SHARP, NULL, getCHRBank, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
