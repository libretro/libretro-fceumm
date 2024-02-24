/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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

static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
	if (latch.addr & 0x2000) { /* NROM-256 */
		setprg32(0x8000, latch.addr >> 2);
	} else { /* NROM-128 */
		setprg16(0x8000, latch.addr >> 1);
		setprg16(0xC000, latch.addr >> 1);
	}
	setchr8(latch.data);
	setmirror((latch.addr & 0x01) ^ 0x01);
}

static DECLFR(M414Read) {
	if ((A >= 0xC000) && !(latch.addr & 0x100) && (latch.addr & (dipsw << 4))) {
		return cpu.openbus;
	}
	return CartBR(A);
}

static void M414Reset(void) {
	dipsw++;
	dipsw &= 0x0F;
	Sync();
}

void Mapper414_Init(CartInfo *info) {
	Latch_Init(info, Sync, M414Read, FALSE, TRUE);
	info->Reset = M414Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
