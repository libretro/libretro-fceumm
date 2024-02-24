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

/* iNES Mapper 33 - Taito TC0190/TC0350 */

#include "mapinc.h"

static uint8 prg[2], chr[6];

static SFORMAT StateRegs[] = {
	{ prg, 2, "PREG" },
	{ chr, 6, "CREG" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg16(0xC000, ~0);

	setchr2(0x0000, chr[0]);
	setchr2(0x0800, chr[1]);
	setchr1(0x1000, chr[2]);
	setchr1(0x1400, chr[3]);
	setchr1(0x1800, chr[4]);
	setchr1(0x1C00, chr[5]);

	setmirror(((prg[0] >> 6) & 0x01) ^ 0x01);
}

static DECLFW(M033Write) {
	switch (A & 0xE003) {
	case 0x8000:
	case 0x8001:
		prg[A & 0x01] = V;
		Sync();
		break;
	case 0x8002:
	case 0x8003:
		chr[A & 0x01] = V;
		Sync();
		break;
	case 0xA000:
	case 0xA001:
	case 0xA002:
	case 0xA003:
		chr[2 + (A & 0x03)] = V;
		Sync();
		break;
	}
}

static void M33Power(void) {
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xBFFF, M033Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper033_Init(CartInfo *info) {
	info->Power = M33Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}

