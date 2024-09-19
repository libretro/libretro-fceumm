/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2015 CaH4e3
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

/* NES 2.0 Mapper 519
 * UNIF board name UNL-EH8813A
 */

#include "mapinc.h"
#include "latch.h"

static uint8 lock, dipsw, chr, ram[4];

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
	{ &lock, 1, "LOCK" },
	{ &chr, 1, "CREG" },
	{ 0 }
};

static void Sync(void) {
	if (lock == 0) {
		if (latch.addr & 0x80) {
			setprg16(0x8000, latch.addr);
			setprg16(0xC000, latch.addr);
		} else {
			setprg32(0x8000, latch.addr >> 1);
		}
		setmirror(((latch.data >> 7) & 0x01) ^ 0x01);
		lock = (latch.addr & 0x100) == 0x100;
		chr = latch.data & 0x7C;
	}
	setchr8(chr | (latch.data & 0x03));
}

static DECLFR(M519ReadRAM) {
	return ram[A & 0x03];
}

static DECLFW(M519WriteRAM) {
	ram[A & 0x03] = V & 0x0F;
}

static DECLFR(M519Read) {
	if (latch.addr & 0x40) {
		return CartBR((A & 0xFFF0) | (dipsw & 0x0F));
	}
	return CartBR(A);
}

static void M519Power(void) {
	dipsw = lock = 0;
	Latch_Power();
	SetReadHandler(0x5800, 0x5FFF, M519ReadRAM);
	SetWriteHandler(0x5800, 0x5FFF, M519WriteRAM);
}

static void M519Reset(void) {
	lock = 0;
	dipsw++;
	Latch_RegReset();
}

void Mapper519_Init(CartInfo *info) {
	Latch_Init(info, Sync, M519Read, FALSE, FALSE);
	info->Reset = M519Reset;
	info->Power = M519Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
