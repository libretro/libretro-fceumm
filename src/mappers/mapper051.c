/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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

static uint8 prg, mode;

static SFORMAT StateRegs[] = {
	{ &prg, 1, "PREG" },
	{ &mode, 1, "MODE" },
	{ 0 }
};

static void Sync(void) {
	if (iNESCart.submapper == 1) {
		setprg8(0x6000, (prg << 1) | 0x23);
		if (mode & 0x022)
			setprg32(0x8000, prg >> 1);
		else {
			setprg16(0x8000, ((prg >> 2) & 0x10) | ((prg >> 1) & 0x08) | (prg & 0x07));
			setprg16(0xC000, ((prg >> 2) & 0x10) | ((prg >> 1) & 0x08) | 0x07);
		}
	} else {
		if (mode & 0x02) {
			setprg8(0x6000, ((prg << 2) & 0x1C) | 0x23);
			setprg32(0x8000, prg);
		} else {
			setprg8(0x6000, ((prg << 2) & 0x10) | 0x2F);
			setprg16(0x8000, (prg << 1) | (prg >> 4));
			setprg16(0xC000, (prg << 1) | 0x07);
		}
	}
	setchr8(0);
	setmirror(((mode >> 4) & 1) ^ 1);
}

static DECLFW(M051WriteMode) {
	mode = V;
	Sync();
}

static DECLFW(M051WriteBank) {
	prg = V;
	Sync();
}

static void M051Power(void) {
	prg = 0;
	mode = 2;
	Sync();
	SetWriteHandler(0x6000, 0x7FFF, M051WriteMode);
	SetWriteHandler(0x8000, 0xFFFF, M051WriteBank);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
}

static void M051Reset(void) {
	prg = 0;
	mode = 2;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper051_Init(CartInfo *info) {
	info->Power = M051Power;
	info->Reset = M051Reset;
	AddExState(StateRegs, ~0, 0, NULL);
	GameStateRestore = StateRestore;

	if (!UNIFchrrama && (ROM.chr.size == 1)) {
		/* at least 1 variant has 8K CHR-ROM which should be treated as CHR-RAM */
		SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 1);
		AddExState(CHRptr[0], CHRsize[0], 0, "CHRR");
	}
}
