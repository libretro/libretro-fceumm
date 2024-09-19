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

/* NES 2.0 Mapper 453 - Realtec 8042
 * 3-in-1 1995 World Competitive Champion Edition (DG-002)
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	if (latch.data & 0x40) {
		setprg32(0x8000, ((latch.data >> 3) & 0x18) |  (latch.data & 0x07));
		setmirror(MI_0 + (((latch.data >> 4) & 0x01) ^ 0x01));
	} else {
		setprg16(0x8000, ((latch.data >> 2) & 0x38) | (latch.data & 0x07));
		setprg16(0xC000, (latch.data >> 2 & 0x38) | 0x07);
		setmirror(((latch.data >> 4) & 0x01) ^ 0x01);
	}
    setchr8(0);
}

static DECLFW(M453Write) {
	if (latch.data & 0xE0) {
		latch.data = (latch.data & 0xE0) | (V & ~0xE0);
    } else {
		latch.data = V;
    }
	Sync();
}

static void M453Power(void) {
    Latch_Power();
    SetWriteHandler(0x8000, 0xFFFF, M453Write);
}

void Mapper453_Init(CartInfo *info) {
    Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M453Power;
	info->Reset = Latch_RegReset;
}
