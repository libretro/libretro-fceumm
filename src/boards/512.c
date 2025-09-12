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
#include "cartram.h"

static uint8 reg;

static void sync () {
	MMC3_syncPRG(0x3F, 0x00);
	MMC3_syncWRAM(0);
	
	if (reg &0x02) {
		setchr4r(0x10, 0x0000, 0);
		setchr4r(0x10, 0x1000, 0);
	} else
		MMC3_syncCHR(0xFF, 0x000);
	
	if (reg == 1) { /* The game uses six nametables - the two console-internal ones, plus four occupying the second half of 8 KiB CHR-RAM.*/
		setntamem(CHRptr[0x10] +0x1000, 1, 0);
		setntamem(CHRptr[0x10] +0x1400, 1, 1);
		setntamem(CHRptr[0x10] +0x1800, 1, 2);
		setntamem(CHRptr[0x10] +0x1C00, 1, 3);
	} else
		MMC3_syncMirror();
}

static DECLFW (writeReg) {
	if (A &0x100) {
		reg = V;
		sync();
	}
}

static void reset () {
	reg = 0;
	MMC3_clear();
}

static void power () {
	reg = 0;
	MMC3_power();
	SetWriteHandler(0x4020, 0x5FFF, writeReg);
}

void Mapper512_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, NULL);
	CartRAM_init(info, 8, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
