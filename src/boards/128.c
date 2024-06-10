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

static uint16 outerbank = 0;

static void Sync(void) {
	setprg16(0x8000, (outerbank >> 2) | (latch.data & 7));
    setprg16(0xC000, (outerbank >> 2) | 7);
	setchr8(0);
    setmirror(((outerbank >> 1) & 1) ^ 1);
}

static DECLFW(M128Write) {
    if (outerbank < 0xF000) {
        outerbank = A;
    }
    LatchWrite(A, V);
}

static void M128Reset() {
    outerbank = 0;
    LatchHardReset();
}

static void M128Power() {
    outerbank = 0;
    LatchPower();
    SetWriteHandler(0x8000, 0xFFFF, M128Write);
}

void Mapper128_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
    info->Power = M128Power;
    info->Reset = M128Reset;
    AddExState(&outerbank, 2, 0, "BANK");
}
