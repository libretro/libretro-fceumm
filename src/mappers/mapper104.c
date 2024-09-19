/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012
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
 * Pegasus 5-in-1 (Golden Five) (Unl)
 */

#include "mapinc.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, (reg[1] << 4) | (reg[0] & 0x0F));
	setprg16(0xC000, (reg[1] << 4) | 0x0F);
	setchr8(0);
}

static DECLFW(M104Write) {
	switch (A & 0xF000) {
	case 0x8000:
	case 0x9000:
	case 0xA000:
	case 0xB000:
		if (!(reg[1] & 0x08)) {
			reg[1] = V;
			Sync();
		}
		break;
	case 0xC000:
	case 0xD000:
	case 0xE000:
	case 0xF000:
		reg[0] = V;
		Sync();
		break;
	}
}

static void M104Close(void) {
}

static void M104Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M104Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper104_Init(CartInfo *info) {
	info->Power = M104Power;
	info->Close = M104Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
