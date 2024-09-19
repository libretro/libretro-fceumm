/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 403 denotes the 89433 circuit board with up to 1 MiB PRG-ROM and 32 KiB of CHR-RAM, bankable with 8
 * KiB granularity.
 *
 * Tetris Family - 玩家 19-in-1 智瑟實典 (NO-1683)
 * Sachen Superpack (versions A-C)
 */

#include "mapinc.h"

static uint8 reg[3];

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
	{ 0 }
};

static void Sync(void) {
	if (reg[2] & 0x01) { /* NROM-128 */
		setprg16(0x8000, reg[0] >> 1);
		setprg16(0xC000, reg[0] >> 1);
	} else { /* NROM-256 */
		setprg32(0x8000, reg[0] >> 2);
	}
	setchr8(reg[1]);
	setmirror(((reg[2] >> 4) & 0x01) ^ 0x01);
}

static DECLFW(M403Write4) {
	if (A & 0x100) {
		reg[A & 0x03] = V;
		Sync();
	}
}

static DECLFW(M403Write8) {
	if (reg[2] & 0x04) {
		reg[1] = V;
		Sync();
	}
}

static void M403Reset(void) {
	reg[0] = reg[1] = reg[2] = 0;
	Sync();
}

static void M403Power(void) {
	reg[0] = reg[1] = reg[2] = 0;
	Sync();
	SetReadHandler(0x6000, 0x7FFF, CartBR); /* For TetrisA (Tetris Family 19-in-1 NO 1683) */
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x5FFF, M403Write4);
	SetWriteHandler(0x8000, 0xFFFF, M403Write8);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper403_Init(CartInfo *info) {
	info->Reset = M403Reset;
	info->Power = M403Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
