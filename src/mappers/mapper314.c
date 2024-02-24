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
 * BMC 42-in-1 "reset switch" type
 */

#include "mapinc.h"
#include "latch.h"

static uint8 regs[4];

static SFORMAT StateRegs[] = {
	{ regs, 4, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint8 prg = regs[1] & 0x3F;

	if (regs[0] & 0x80) { /* NROM mode */
		if (regs[1] & 0x80) {
			setprg32(0x8000, prg);
		} else {
			setprg16(0x8000, (prg << 1) | ((regs[1] >> 6) & 0x01));
			setprg16(0xC000, (prg << 1) | ((regs[1] >> 6) & 0x01));
		}
	} else { /* UNROM mode */
		setprg16(0x8000, (prg << 1) | (latch.data & 0x07));
		setprg16(0xC000, (prg << 1) | 0x07);
	}
	setchr8((regs[2] << 2) | ((regs[0] >> 1) & 0x03));
	setmirror(((regs[0] >> 5) & 0x01) ^ 0x01);
	
}

static DECLFW(M314Write) {
	A &= 0x03;
	if (A == 0x03) A = 0x01; /* K-42001's "Aladdin III" */
	regs[A] = V;
	Sync();
}

static void M314Reset(void) {
	/* Reset returns to menu */
	regs[0] = 0x80;
	regs[1] = 0x43;
	regs[2] = regs[3] = 0;
	Sync();
}

static void M314Power(void) {
	regs[0] = 0x80;
	regs[1] = 0x43;
	regs[2] = regs[3] = 0;
	Latch_Power();
	SetWriteHandler(0x5000, 0x5FFF, M314Write);
}

void Mapper314_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, TRUE);
	info->Power = M314Power;
	info->Reset = M314Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
