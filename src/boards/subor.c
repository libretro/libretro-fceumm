/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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

static SFORMAT StateRegs[] =
{
	{ regs, 4, "DREG" },
	{ 0 }
};

static void M166Sync(void) {
	int base = ((regs[0] ^ regs[1]) & 0x10) << 1;
	int bank = (regs[2] ^ regs[3]) & 0x1f;

	if (regs[1] & 0x08) {
		bank &= 0xFE;
		setprg16(0x8000, base + bank + 0);
		setprg16(0xC000, base + bank + 1);
	} else {
		if (regs[1] & 0x04) {
			setprg16(0x8000, 0x1F);
			setprg16(0xC000, base + bank);
		} else {
			setprg16(0x8000, base + bank);
			setprg16(0xC000, 0x07);
		}
	}
	setchr8(0);
}

static void M167Sync(void) {
	int base = ((regs[0] ^ regs[1]) & 0x10) << 1;
	int bank = (regs[2] ^ regs[3]) & 0x1f;

	if (regs[1] & 0x08) {
		bank &= 0xFE;
		setprg16(0x8000, base + bank + 1);
		setprg16(0xC000, base + bank + 0);
	} else {
		if (regs[1] & 0x04) {
			setprg16(0x8000, 0x1F);
			setprg16(0xC000, base + bank);
		} else {
			setprg16(0x8000, base + bank);
			setprg16(0xC000, 0x20);
		}
	}
	setchr8(0);
}

static void M166Write(uint32 A, uint8 V) {
	regs[(A >> 13) & 0x03] = V;
	M166Sync();
}

static void M167Write(uint32 A, uint8 V) {
	regs[(A >> 13) & 0x03] = V;
	M167Sync();
}

static void M166Power(void) {
	regs[0] = regs[1] = regs[2] = regs[3] = 0;
	M166Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M166Write);
}

static void M167Power(void) {
	regs[0] = regs[1] = regs[2] = regs[3] = 0;
	M167Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M167Write);
}

static void M166StateRestore(int version) {
	M167Sync();
}

static void M167StateRestore(int version) {
	M167Sync();
}

void Mapper166_Init(CartInfo *info) {
	info->Power      = M166Power;
	GameStateRestore = M166StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

void Mapper167_Init(CartInfo *info) {
	info->Power      = M167Power;
	GameStateRestore = M167StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
