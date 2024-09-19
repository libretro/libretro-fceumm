/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2002 Xodnizel
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

static uint8 prg[3], chr[6], mirr;
static uint8 wram[256];

static SFORMAT StateRegs[] = {
	{ prg, 3, "PREG" },
	{ chr, 6, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, ~0);

	setchr2(0x0000, chr[0] >> 1);
	setchr2(0x0800, chr[1] >> 1);
	setchr1(0x1000, chr[2]);
	setchr1(0x1400, chr[3]);
	setchr1(0x1800, chr[4]);
	setchr1(0x1C00, chr[5]);

	if (iNESCart.mapper == 207) {
		setmirrorw(chr[0] >>7, chr[0] >>7, chr[1] >>7, chr[1] >>7);
	} else {
		setmirror(mirr & 0x01);
	}
}

static DECLFW(M080RamWrite) {
	wram[A & 0xFF] = V;
}

static DECLFR(M080RamRead) {
	return wram[A & 0xFF];
}

static DECLFW(M080Write) {
	switch (A) {
	case 0x7EF0:
	case 0x7EF1:
	case 0x7EF2:
	case 0x7EF3:
	case 0x7EF4:
	case 0x7EF5:
		chr[A & 0x07] = V;
		Sync();
		break;
	case 0x7EF6:
		mirr = V;
		Sync();
		break;
	case 0x7EF8:
		break;
	case 0x7EFA:
	case 0x7EFB:
	case 0x7EFC:
	case 0x7EFD:
	case 0x7EFE:
	case 0x7EFF:
		prg[(A - 0x7EFA) >> 1] = V;
		Sync();
		break;
	}
}

static void M080Power(void) {
	Sync();
	SetReadHandler(0x7F00, 0x7FFF, M080RamRead);
	SetWriteHandler(0x7F00, 0x7FFF, M080RamWrite);
	SetWriteHandler(0x7EF0, 0x7EFF, M080Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void M207Power(void) {
	Sync();
	SetWriteHandler(0x7EF0, 0x7EFF, M080Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper080_Init(CartInfo *info) {
	info->Power = M080Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
	if (info->battery) {
		info->SaveGame[0] = wram;
		info->SaveGameLen[0] = 256;
		AddExState(wram, sizeof(wram), 0, "WRAM");
	}
}

void Mapper207_Init(CartInfo *info) {
	info->Power = M207Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
