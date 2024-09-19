/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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

static uint8 reg[4];

static SFORMAT StateRegs[] = {
	{ reg, 4, "DREG" },
	{ 0 }
};

static void Sync(void) {
	uint16 base = ((reg[0] ^ reg[1]) & 0x10) << 1;
	uint16 bank = (reg[2] ^ reg[3]) & 0x1f;

	if (reg[1] & 0x08) {
		bank &= 0xFE;
		setprg16(0x8000, base + bank + 1);
		setprg16(0xC000, base + bank + 0);
	} else {
		if (reg[1] & 0x04) {
			setprg16(0x8000, 0x1F);
			setprg16(0xC000, base + bank);
		} else {
			setprg16(0x8000, base + bank);
			setprg16(0xC000, 0x20);
		}
	}
	setchr8(0);
}

static DECLFW(M167Write) {
	reg[(A >> 13) & 0x03] = V;
	Sync();
}

static void M167Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M167Write);
	if (WRAMSIZE) {
	    setprg8r(0x10, 0x6000, 0);
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper167_Init(CartInfo *info) {
	info->Power = M167Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8 * 1024;
	if (info->iNES2) {
		WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	}
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_malloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}
}
