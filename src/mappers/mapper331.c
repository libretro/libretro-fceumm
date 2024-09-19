/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
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
 * 7-in-1  Darkwing Duck, Snake, MagicBlock (PCB marked as "12 in 1")
 * 12-in-1 1991 New Star Co. Ltd.
 *
 */

#include "mapinc.h"

static uint8 reg[3];
static uint8 PPUCHRBus;

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint8 base = (reg[2] & 0x03) << 3;

	if (reg[2] & 8) {
		setprg16(0x8000, base | ((reg[PPUCHRBus] & 0x06) & ~0x01)); /* actually, both 0 and 1 registers used, but they will switch each PA12 transition */
		setprg16(0xc000, base | ((reg[PPUCHRBus] & 0x06) |  0x01)); /* if bits are different for both registers, so they must be programmed strongly the same! */
	} else {
		setprg16(0x8000, base | (reg[PPUCHRBus] & 0x07));
		setprg16(0xc000, base | 0x07);
	}
	setchr4(0x0000, (base << 2) | (reg[0] >> 3));
	setchr4(0x1000, (base << 2) | (reg[1] >> 3));
	setmirror(((reg[2] & 4) >> 2) ^ 1);
}

static DECLFW(M331Write) {
	switch (A & 0xE000) {
	case 0xA000: reg[0] = V; Sync(); break;
	case 0xC000: reg[1] = V; Sync(); break;
	case 0xE000: reg[2] = V; Sync(); break;
	}
}

static void M331Power(void) {
	reg[0] = reg[1] = reg[2] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M331Write);
}

static void StateRestore(int version) {
	Sync();
}

static void M331PPUHook(uint32 A) {
	uint8 bank = (A & 0x1000) >> 12;

	if ((PPUCHRBus != bank) && ((A & 0x3000) != 0x2000)) {
		PPUCHRBus = bank;
		Sync();
	}
}

void Mapper331_Init(CartInfo *info) {
	info->Power = M331Power;
	PPU_hook = M331PPUHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
