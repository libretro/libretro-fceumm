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
 * NES 2.0 Mapper 312 - Kaiser 7013B
 * UNIF UNL-KS7013B
 * Just another pirate cart with pirate mapper, instead of original MMC1
 * Kaiser Highway Star
 *
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, reg);
	setprg16(0xc000, ~0);
	setmirror((latch.data & 0x01) ^ 0x01);
	setchr8(0);
}

static DECLFW(M312LoWrite) {
	reg = V;
	Sync();
}

static void M312Power(void) {
	reg = 0;
	Latch_Power();
	SetWriteHandler(0x6000, 0x7FFF, M312LoWrite);
}

void Mapper312_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M312Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
