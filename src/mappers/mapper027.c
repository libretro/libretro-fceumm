/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2013 CaH4e3
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

static void Sync(void) {
	setprg32(0x8000, 0);
	setchr4(0x0000, ((latch.addr << 1) & 0x02) | (latch.addr & 0x01));
	setchr4(0x1000, ((latch.addr << 1) & 0x02) | (latch.addr & 0x01));
	setmirror(MI_0 + (latch.addr & 0x01));
}

void Mapper027_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
}
