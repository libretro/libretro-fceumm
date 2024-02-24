/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE = 0;

static void M165CW(uint16 A, uint16 V) {
	if (V == 0) {
		setchr4r(0x10, A, 0);
	} else {
		setchr4(A, V >> 2);
	}
}

static void M165PPUFD(void) {
	if (reg == 0xFD) {
		M165CW(0x0000, mmc3.reg[0]);
		M165CW(0x1000, mmc3.reg[2]);
	}
}

static void M165PPUFE(void) {
	if (reg == 0xFE) {
		M165CW(0x0000, mmc3.reg[1]);
		M165CW(0x1000, mmc3.reg[4]);
	}
}

static void M165CWM(uint16 A, uint16 V) {
	if (((mmc3.cmd & 0x7) == 0) || ((mmc3.cmd & 0x7) == 2)) {
		M165PPUFD();
	}
	if (((mmc3.cmd & 0x7) == 1) || ((mmc3.cmd & 0x7) == 4)) {
		M165PPUFE();
	}
}

static void M165PPU(uint32 A) {
	if ((A & 0x1FF0) == 0x1FD0) {
		reg = 0xFD;
		M165PPUFD();
	} else if ((A & 0x1FF0) == 0x1FE0) {
		reg = 0xFE;
		M165PPUFE();
	}
}

static void M165Close(void) {
	MMC3_Close();
    if (CHRRAM) {
        FCEU_free(CHRRAM);
        CHRRAM = NULL;
    }
}

static void M165Power(void) {
	reg = 0xFD;
	MMC3_Power();
}

void Mapper165_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
    info->Close = M165Close;
	MMC3_cwrap = M165CWM;
	PPU_hook = M165PPU;
	info->Power = M165Power;
	CHRRAMSIZE = 4096;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
	AddExState(&reg, 1, 0, "EXPR");
}