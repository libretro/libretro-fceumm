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

/* NES 2.0 Mapper 448
 * VRC4-based 830768C multicart circuit board used by a Super 6-in-1 multicart.
 */

#include "mapinc.h"
#include "latch.h"
#include "vrc2and4.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 },
};

static void Sync(void) {
	if (reg & 0x08) { /* AOROM */
		setprg32(0x8000, ((reg << 2) & ~0x07) | (latch.data & 0x07));
		setmirror(MI_0 + ((latch.data >> 4) & 0x01));
	} else {
		if (reg & 0x04) { /* UOROM */
			setprg16(0x8000, ((reg << 3) & ~0x0F) | (vrc24.prg[0] & 0x0F));
			setprg16(0xC000, ((reg << 3) & ~0x0F) | 0x0F);
		} else { /* UNROM */
			setprg16(0x8000, (reg << 3) | (vrc24.prg[0] & 0x07));
			setprg16(0xC000, (reg << 3) | 0x07);
		}
		switch (vrc24.mirr & 0x03) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
	}
	setchr8(0);
}

static DECLFW(M448WriteReg) {
	if (vrc24.cmd & 0x01) {
		reg = A & 0xFF;
		Sync();
	}
}

static DECLFW(M448WriteASIC) {
	Latch_Write(A, V);
    VRC24_Write(A, V);
	Sync();
}

static void M448Reset(void) {
	reg = 0;
	Sync();
}

static void M448Power(void) {
	reg = 0;
	Latch_Power();
	VRC24_Power();
	Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, M448WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M448WriteASIC);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper448_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	VRC24_Init(info, VRC4, 0x04, 0x08, 0, 1);
	info->Reset = M448Reset;
	info->Power = M448Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
