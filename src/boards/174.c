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
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
    if (latch.addr & 0x80) {
        setprg32(0x8000, (latch.addr >> 5) & 3);
    } else {
        setprg16(0x8000, (latch.addr >> 4) & 7);
        setprg16(0xC000, (latch.addr >> 4) & 7);
    }
	setchr8((latch.addr >> 1) & 7);
    setmirror((latch.addr & 1) ^ 1);
}

void Mapper174_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
}
