/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

/* NES 2.0 Mapper 453 - Realtec 8042
 * 3-in-1 1995 World Competitive Champion Edition (DG-002)
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	setchr8(0);
	if (latch.data & 0x40) {
		setprg32(0x8000, ((latch.data >> 3) & 0x18) |  (latch.data & 7));
	} else {
		setprg16(0x8000, (((latch.data >> 2) & 0x38) | latch.data & 7));
		setprg16(0xC000, (latch.data >> 2 & 0x38) | 7);
	}
    setmirror(((latch.data >> 5) & 2) + (((latch.data >> 4) & 1) ^ 1));
}

static DECLFW(M453WriteLatch) {
    if (latch.data & 0xE0)
		latch.data = (latch.data & 0xE0) | (V & ~0xE0);
    else
		latch.data = V;
    Sync();
}

static void M453Power(void) {
    LatchPower();
    SetWriteHandler(0x8000, 0xFFFF, M453WriteLatch);
}

void Mapper453_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
	info->Power = M453Power;
	info->Reset = LatchHardReset;
}
