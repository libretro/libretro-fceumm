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

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	if (latch.addr & 0x20) {
		setprg32(0x8000, (latch.addr >> 1) & 0x0F);
	} else {
		setprg16(0x8000, latch.addr & 0x1E);
		setprg16(0xC000, latch.addr & 0x1E);
	}
	setchr8(0);
    switch ((latch.addr >> 6) & 0x03) {
    case 0: setmirror(MI_0); break;
    case 1: setmirror(MI_V); break;
    case 2: setmirror(MI_H); break;
    case 3: setmirrorw(0, 1, 1, 1); break;
    }
}

void Mapper231_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
}
