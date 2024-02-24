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
 *
 */

/* iNES Mapper 59 - BMCD1038 */

#include "mapinc.h"
#include "latch.h"

static uint8 dipsw;

static void Sync(void) {
	if (latch.addr & 0x80) {
		setprg16(0x8000, (latch.addr & 0x70) >> 4);
		setprg16(0xC000, (latch.addr & 0x70) >> 4);
	} else {
		setprg32(0x8000, (latch.addr & 0x60) >> 5);
	}
	setchr8(latch.addr & 0x07);
	setmirror(((latch.addr & 0x08) >> 3) ^ 0x01);
}

static DECLFR(M059Read) {
	if (latch.addr & 0x100) {
		return (cpu.openbus & ~0x03) | (dipsw & 0x03);
	}
	return CartBR(A);
}

static DECLFW(M059Write) {
	/* Only recognize the latch write if the lock bit has not been set. */
	/* Needed for NT-234 "Road Fighter" */
	if (!(latch.addr & 0x200)) {
		Latch_Write(A, V);
	}
}

static void M059Reset(void) {
	dipsw++;
	/* Always reset to menu */
	latch.addr = 0;
	Sync();
}

static void M059Power(void) {
	Latch_Power();
	/* Trap latch writes to enforce the "Lock" bit */
	SetWriteHandler(0x8000, 0xFFFF, M059Write);
}

void Mapper059_Init(CartInfo *info) {
	Latch_Init(info, Sync, M059Read, FALSE, FALSE);
	info->Reset = M059Reset;
	info->Power = M059Power;
	AddExState(&dipsw, 1, 0, "DIPSW");
}
