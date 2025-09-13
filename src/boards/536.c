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

/* The UNIF dump of "(JY-103) 3-in-1" (JY4M4 MAPR) has a strange bank order and will not run with this emulation. */

#include "mapinc.h"
#include "asic_mmc3.h"

static uint8 reg[2];

static void sync () {
	int prg = reg[0] &0x1F | reg[0] >>1 &0x20;
	if (reg[0] &0x80)
		MMC3_syncPRG(0x0F, prg <<2);
	else
	if (reg[0] &0x20)
		setprg32(0x8000, prg >>1);
	else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	}
	setchr8(0);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	reg[A &1] = V;
	sync();
}

static void reset () {
	reg[0] = 0x60;
	reg[1] = 0x00;
	MMC3_clear();
}

static void power () {
	reg[0] = 0x60;
	reg[1] = 0x00;
	MMC3_power();
}

void Mapper536_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(reg, 2, 0, "EXPR");
}
