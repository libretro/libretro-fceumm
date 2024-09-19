/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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

/* NES 2.0 Mapper 329 - UNL-EDU2000 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	setprg8r(0x10, 0x6000, latch.data >> 6);
	setprg32(0x8000, latch.data & 0x1F);
	setchr8(0);
	setmirror(((latch.data >> 5) & 0x01) ^ 0x01);
}

void Mapper329_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
}
