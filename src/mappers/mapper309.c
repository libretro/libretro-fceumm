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

/* FDS Conversion
 * NES 2.0 Mapper 309 is used for Whirlwind Manu's ROM cartridge conversion
 * of game 愛戦士ニコル (Ai Senshi Nicol, cartridge code M309).
 * Its UNIF board name is UNL-LH51.
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_309
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg, mirr;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg8(0x8000, reg);
	setprg8(0xA000, ~2);
	setprg8(0xC000, ~1);
	setprg8(0xE000, ~0);
	setchr8(0);
	setmirror(((mirr >> 3) & 0x01) ^ 0x01);
}

static DECLFW(M309Write) {
	switch (A & 0xF000) {
	case 0x8000: reg = V; Sync(); break;
	case 0xF000: mirr = V; Sync(); break;
	}
}

static void M309Power(void) {
	FDSSound_Power();
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M309Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M309Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper309_Init(CartInfo *info) {
	info->Power = M309Power;
	info->Close = M309Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
