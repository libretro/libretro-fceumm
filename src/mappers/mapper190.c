/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2017 FCEUX Team
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
 *
 * Magic Kid GooGoo
 */

#include "mapinc.h"

static uint8 prg, chr[4];

static SFORMAT StateRegs[] =  {
	{ &prg, 1, "PREG" },
	{ chr, 4, "CREG" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, prg);
	setprg16(0xC000, 0);
	setchr2(0x0000, chr[0]);
	setchr2(0x0800, chr[1]);
	setchr2(0x1000, chr[2]);
	setchr2(0x1800, chr[3]);
}

static DECLFW(M190Write) {
	switch (A & 0xE000) {
	case 0x8000:
	case 0xC000:
		prg = (A << 11) | (V & 0x07);
		Sync();
		break;
	case 0xA000:
		chr[A & 0x03] = V;
		Sync();
		break;
	}
}

static void M190Power(void) {
	chr[0] = chr[1] = chr[2] = chr[3] = 0;
	prg = 0;
	setmirror(MI_V);
	Sync();

	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M190Write);
	FCEU_CheatAddRAM(0x2000 >> 10, 0x6000, WRAM);
}

static void M190Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper190_Init(CartInfo *info) {
	info->Power = M190Power;
	info->Close = M190Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAM = (uint8 *)FCEU_gmalloc(0x2000);
	SetupCartPRGMapping(0x10, WRAM, 0x2000, 1);
	AddExState(WRAM, 0x2000, 0, "WRAM");
}
