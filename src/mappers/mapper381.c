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

/* Map 381 - 2-in-1 High Standard Game (BC-019), reset-based */

#include "mapinc.h"
#include "latch.h"

static uint8 reset = 0;

static void Sync(void) {
	setprg16(0x8000, (reset << 4) | ((latch.data & 0x07) << 1) | ((latch.data >> 4) & 0x0F));
	setprg16(0xC000, (reset << 4) | 0x0F);
	setchr8(0);
}

static void M381Reset(void) {
	reset++;
	Sync();
}

void Mapper381_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
    info->Reset = M381Reset;
	AddExState(&reset, 1, 0, "RST0");
}
