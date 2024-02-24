/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

/* added 2019-5-23
 * Mapper 31 - custom mapper by infiniteneslives
 * https://wiki.nesdev.com/w/index.php/INES_Mapper_031 */

#include "mapinc.h"

static uint8 prg[8];

static SFORMAT StateRegs[] = {
	{ prg, 8, "PREG" },
	{ 0 }
};

static void Sync(void) {
	setprg4(0x8000, prg[0]);
	setprg4(0x9000, prg[1]);
	setprg4(0xA000, prg[2]);
	setprg4(0xB000, prg[3]);
	setprg4(0xC000, prg[4]);
	setprg4(0xD000, prg[5]);
	setprg4(0xE000, prg[6]);
	setprg4(0xF000, prg[7]);
	setchr8(0);
}

static DECLFW(M031Write) {
	prg[A & 0x07] = V;
	Sync();
}

static void M031Power(void) {
	prg[0] = ~7;
	prg[1] = ~6;
	prg[2] = ~5;
	prg[3] = ~4;
	prg[4] = ~3;
	prg[5] = ~2;
	prg[6] = ~1;
	prg[7] = ~0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, M031Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper031_Init(CartInfo *info) {
	info->Power = M031Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
