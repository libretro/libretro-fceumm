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
#include "latch.h"

static uint8 reg[4];
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
	{ reg, 4, "REGS" },
	{ 0 }
};

static void Sync(void) {
	int mask = reg[2] >> 6; /* There is an CNROM mode that takes either two or four inner CHR banks from a CNROM-like
	                            latch register at $8000-$FFFF. */

	if (reg[1] & 0x10) {
		setprg32(0x8000, reg[1] >> 6);
	} else {
		setprg16(0x8000, reg[1] >> 5);
		setprg16(0xC000, reg[1] >> 5);
	}

	setchr8(((reg[1] & 0x07) & ~mask) | (latch.data & mask));

	setmirror(((reg[1] >> 3) & 0x01) ^ 0x01);
}

static DECLFW(M428Write) {
	reg[A & 0x03] = V;
	Sync();
}

static DECLFR(M428Read) {
	return (cpu.openbus & ~0x03) | (dipsw & 0x03);
}

static void M428Power(void) {
	dipsw = 0;
	reg[0] = 0;
	reg[1] = 0;
	reg[2] = 0;
	reg[3] = 0;
	Latch_Power();
	SetWriteHandler(0x6000, 0x7FFF, M428Write);
	SetReadHandler(0x6000, 0x7FFF, M428Read);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void M428Reset(void) {
	dipsw++;
	reg[0] = 0;
	reg[1] = 0;
	reg[2] = 0;
	reg[3] = 0;
	Sync();
}

void Mapper428_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M428Power;
	info->Reset = M428Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
