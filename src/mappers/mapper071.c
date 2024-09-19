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

static uint8 prg, mirr;

static SFORMAT StateRegs[] = {
	{ &prg, 1, "PREG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, prg);
	setprg16(0xC000, ~0);
	setchr8(0);
	/* Fire Hawk or submapper 1, otherwise hard-mirroring */
	if (mirr) {
		setmirror(mirr);
	}
}

static DECLFW(M071Write) {
	switch (A & 0xF000) {
	case 0x9000:
		mirr = MI_0 + ((V >> 4) & 0x01);
		Sync();
		break;
	case 0xC000:
	case 0xD000:
	case 0xE000:
	case 0xF000:
		prg = V;
		Sync();
		break;
	}
}

static void M071Power(void) {
	prg = 0;
	mirr = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x9000, 0xFFFF, M071Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper071_Init(CartInfo *info) {
	info->Power = M071Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
