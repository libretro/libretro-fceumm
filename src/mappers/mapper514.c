/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
 */

#include "mapinc.h"

static uint8 mode;
static uint32 lastnt;

static SFORMAT StateRegs[] = {
	{ &mode, 1, "MODE" },
	{ &lastnt, 4, "LSNT" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg32(0x8000, mode & 0x3F);
	setchr4(0x0000, lastnt);
	setchr4(0x1000, 1);
	setmirror(((mode >> 6) & 0x01) ^ 0x01);
}

static DECLFW(M514Write8) {
	if ((A & 0xFFF) == 0) {
		mode = V;
		Sync();
	}
}

static void M514PPUHook(uint32 A) {
	if ((A & 0x3000) == 0x2000) {
		uint32 mask = (mode & 0x40) ? 0x02 : 0x01;
		uint32 bank = A >> 10;
		if ((mode & 0x80) && (bank & mask)) {
			setchr4(0, 1);
			lastnt = 1;
		} else {
			lastnt = 0;
			setchr4(0, 0);
		}
	}
}

static void M514Reset(void) {
	lastnt = 0;
	mode = 0;
	Sync();
}

static void M514Power(void) {
	lastnt = 0;
	mode = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0x8FFF, M514Write8);
}

static void M514Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper514_Init(CartInfo *info) {
	info->Power = M514Power;
	info->Reset = M514Reset;
	info->Close = M514Close;
	PPU_hook = M514PPUHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, TRUE);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
