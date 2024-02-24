/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2011 CaH4e3
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
 * NES 2.0 Mapper 522 - UNL-LH10
 *
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg[8], cmd;

static SFORMAT StateRegs[] =
{
	{ &cmd, 1, "CMD" },
	{ reg, 8, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x6000, ~1);
	setprg8(0x8000, reg[6]);
	setprg8(0xA000, reg[7]);
	setprg8r(0x10, 0xC000, 0);
	setprg8(0xE000, ~0);
	setchr8(0);
	setmirror(0);
}

static DECLFW(M522Write) {
	if (A & 0x0001) {
		reg[cmd & 0x07] = V;
		Sync();
	} else {
		cmd = V;
	}
}

static void M522Power(void) {
	FDSSound_Power();
	reg[0] = reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = reg[6] = reg[7] = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0x9FFF, M522Write);
	SetWriteHandler(0xC000, 0xDFFF, CartBW);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M522Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper522_Init(CartInfo *info) {
	info->Power = M522Power;
	info->Close = M522Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
