/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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

static uint8 prg[4];
static uint8 chr[4];
static uint8 mirr;
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 4, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xa000, prg[1]);
	setprg8(0xc000, prg[2]);
	setprg8(0xe000, prg[3]);

	setchr2(0x0000, chr[0]);
	setchr2(0x0800, chr[1]);
	setchr2(0x1000, chr[2]);
	setchr2(0x1800, chr[3]);

	setmirror((mirr & 0x01) ^ 0x01);
}

static DECLFW(M286Write) {
	switch (A & 0xF000) {
	case 0x8000:
	case 0x9000:
		chr[(A & 0xC00) >> 10] = A & 0x1F;
		Sync();
		break;
	case 0xA000:
	case 0xB000:
		if (A & (1 << (dipsw + 4))) {
			prg[(A & 0xC00) >> 10] = A & 0x0F;
			Sync();
		}
		break;
	case 0xC000:
		mirr = V;
		Sync();
		break;
	}
}

static void M286Reset(void) {
	dipsw++;
	dipsw &= 3;
	prg[0] = prg[1] = prg[2] = prg[3] = ~0;
	mirr = 0;
	Sync();
}

static void M286Power(void) {
	dipsw = 0;
	prg[0] = prg[1] = prg[2] = prg[3] = ~0;
	mirr = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M286Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper286_Init(CartInfo *info) {
	info->Power = M286Power;
	info->Reset = M286Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
