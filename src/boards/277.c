/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
    if (!(latch.data & 1) && (latch.data & 8)) {
        setprg16(0x8000, latch.data & ~1);
        setprg16(0xC000, latch.data |  1);
    } else {
        if (latch.data & 8) {
            setprg16(0x8000, latch.data);
            setprg16(0xC000, latch.data);
        } else {
            setprg16(0x8000, latch.data);
            setprg16(0xC000, latch.data | 7);
        }
    }
	setchr8(0);
	setmirror(((latch.data >> 4) & 1) ^ 1);
}

static DECLFW(M277Write) {
	if (!(latch.data & 0x20)) {
		LatchWrite(A, V);
	}
}

static void M277Power(void) {
    LatchPower();
    latch.data = 8;
    Sync();
	SetWriteHandler(0x8000, 0xFFFF, M277Write);
}

static void M277Reset() {
    latch.data = 0x08;
    Sync();
}

void Mapper277_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
	info->Power = M277Power;
	info->Reset = M277Reset;
}
