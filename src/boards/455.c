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

static uint8 reg[2];

static void sync () {
	if (reg[0] &0x01) {
		if (reg[0] &0x02)
			setprg32(0x8000, reg[0] >>3);
		else {
			setprg16(0x8000, reg[0] >>2);
			setprg16(0xC000, reg[0] >>2);
		}
		MMC3_syncCHR(0xFF, 0x00);
	} else {
		int prgAND = 0x1F;
		int chrAND = reg[1] &0x02? 0x7F: 0xFF;
		int prgOR  = reg[0] >>1;
		int chrOR  = reg[1] <<6;
		MMC3_syncPRG(prgAND, prgOR &~prgAND);
		MMC3_syncCHR(chrAND, chrOR &~chrAND);
	}
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	if (A &0x100) {
		reg[0] = V;
		reg[1] = A &0xFF;
		sync();
	}
}

static void reset () {
	reg[0] = reg[1] = 0;
	MMC3_clear();
}

static void power () {
	reg[0] = reg[1] = 0;
	MMC3_power();
	SetWriteHandler(0x4020, 0x5FFF, writeReg);
}

void Mapper455_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(reg, 2, 0, "EXPR");
}
