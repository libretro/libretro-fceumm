/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2013 CaH4e3
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

static uint16 latche;

static SFORMAT StateRegs[] =
{
	{ &latche, 2, "LATC" },
	{ 0 }
};

static void UNLCC21Sync(void) {
	setprg32(0x8000, 0);
	if(CHRsize[0] == 8192) {
		setchr4(0x0000, latche & 1);
		setchr4(0x1000, latche & 1);
	} else {
		setchr8(latche & 1);    /* actually, my bad, overdumped roms, the real CHR size if 8K */
	}
	setmirror(MI_0 + (latche & 1));
}

static void UNLCC21Write1(uint32 A, uint8 V) {
	latche = A;
	UNLCC21Sync();
}

static void UNLCC21Write2(uint32 A, uint8 V) {
	latche = V;
	UNLCC21Sync();
}

static void UNLCC21Power(void) {
	latche = 0;
	UNLCC21Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8001, 0xFFFF, UNLCC21Write1);
	SetWriteHandler(0x8000, 0x8000, UNLCC21Write2); /* another one many-in-1 mapper, there is a lot of similar carts with little different wirings */
}

static void UNLCC21StateRestore(int version) {
	UNLCC21Sync();
}

void UNLCC21_Init(CartInfo *info) {
	info->Power = UNLCC21Power;
	GameStateRestore = UNLCC21StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
