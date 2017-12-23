/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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
 */

#include "mapinc.h"

static uint8 *WRAM = NULL;
static uint32 WRAMSIZE;

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);	/* Famili BASIC (v3.0) need it (uses only 4KB), FP-BASIC uses 8KB */
	setprg16(0x8000, 0);
	setprg16(0xC000, ~0);
	setchr8(0);
}

static void StateRestore(int version) {
	Sync();
}

static void M0Power(void) {
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	Sync();
}

static void M0Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void Mapper0_Init(CartInfo *info) {
	info->Power = M0Power;
	info->Close = M0Close;
	GameStateRestore = StateRestore;
	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
