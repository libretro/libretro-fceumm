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
 */

#include "mapinc.h"

static uint8 regs[4];
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ regs, 4, "EXPR" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
	uint8 prg  = regs[1];
	uint8 chr  = regs[2];
	uint8 mirr = (regs[0] & 0x01) ^ 0x01;

	if (regs[0] & 0x04) {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	} else {
		setprg32(0x8000, prg >> 1);
	}
	setchr8(chr);
	setmirror(mirr);
}

static DECLFW(M463Write5000) {
	if (A & (0x10 << dipsw)) {
		regs[A & 0x03] = V;
		Sync();
	}
}

static void M463Reset(void) {
	dipsw = (dipsw + 1) & 0x07;
	regs[0] = regs[1] = regs[2] = regs[3] = 0;
	Sync();
}

static void M463Power(void) {
	dipsw = 0;
	regs[0] = regs[1] = regs[2] = regs[3] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, M463Write5000);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper463_Init(CartInfo *info) {
	info->Power = M463Power;
	info->Reset = M463Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
