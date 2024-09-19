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
 *
 * NTDEC, ASDER games
 *
 */

#include "mapinc.h"

static uint8 reg[8];
static uint8 mirror, cmd, chrBase;

static SFORMAT StateRegs[] = {
	{ &cmd, 1, "CMD" },
	{ &mirror, 1, "MIRR" },
	{ &chrBase, 1, "CHRB" },
	{ reg, 8, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);

	setprg8(0x8000, reg[0]);
	setprg8(0xA000, reg[1]);
	setprg16(0xC000, ~0);

	setchr2(0x0000, (reg[2] >> 1));
	setchr2(0x0800, (reg[3] >> 1));
	setchr1(0x1000, ((chrBase << 4) & 0x100) | reg[4]);
	setchr1(0x1400, ((chrBase << 3) & 0x100) | reg[5]);
	setchr1(0x1800, ((chrBase << 2) & 0x100) | reg[6]);
	setchr1(0x1C00, ((chrBase << 1) & 0x100) | reg[7]);

	setmirror((mirror & 0x01) ^ 0x01);
}

static DECLFW(M112Write) {
	switch (A & 0xE000) {
	case 0x8000:
		cmd = V;
		break;
	case 0xA000:
		reg[cmd & 0x07] = V;
		Sync();
		break;
	case 0xC000:
		chrBase = V;
		Sync();
		break;
	case 0xE000:
		mirror = V;
		Sync();
		break;
	}
}

static void M112Close(void) {
}

static void M112Power(void) {
	reg[0] = 0;
	reg[1] = 1;
	reg[2] = 0;
	reg[3] = 2;
	reg[4] = 4;
	reg[5] = 5;
	reg[6] = 6;
	reg[7] = 7;
	chrBase = 0;
	Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M112Write);

	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	FCEU_CheatAddRAM(8, 0x6000, WRAM);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper112_Init(CartInfo *info) {
	info->Power = M112Power;
	info->Close = M112Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAM = (uint8 *)FCEU_gmalloc(8192);
	SetupCartPRGMapping(0x10, WRAM, 8192, 1);
	AddExState(WRAM, 8192, 0, "WRAM");
}
