/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
 * 	Copyright (C) 2023
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

/* NES 2.0 Mapper 262 - UNL-SHERO */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;
static uint8 dipsw;

static uint8 *CHRRAM;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M262CW(uint16 A, uint16 V) {
	if (reg & 0x40) {
		setchr8r(0x10, 0);
	} else {
		uint8 lsh[] = { 3, 2, 0, 1 };
		uint8 bank = (A >> 11) & 0x03;

		setchr1(A, (((reg >> lsh[bank]) << 8) & 0x100) | (V & 0xFF));
	}
}

static DECLFW(M262Write) {
	if (A & 0x100) {
		reg = V;
		MMC3_FixCHR();
	}
}

static DECLFR(M262Read) {
	if (A & 0x100) {
		return(dipsw);
	}
	return cpu.openbus;
}

static void M262Reset(void) {
	MMC3_Reset();
	dipsw ^= 0xFF;
}

static void M262Power(void) {
	dipsw = 0x00;
	MMC3_Power();
	SetWriteHandler(0x4100, 0x4FFF, M262Write);
	SetReadHandler(0x4100, 0x4FFF, M262Read);
}

static void M262Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

void Mapper262_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M262CW;
	info->Power = M262Power;
	info->Reset = M262Reset;
	info->Close = M262Close;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAM = (uint8*)FCEU_gmalloc(8192);
	SetupCartCHRMapping(0x10, CHRRAM, 8192, 1);
	AddExState(CHRRAM, 8192, 0, "CHRR");
}
