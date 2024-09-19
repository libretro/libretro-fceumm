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

/* NES 2.0 Mapper 261 - BMC810544-C-A1 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	if (latch.addr & 0x40)
		setprg32(0x8000, latch.addr >> 7);
	else {
		setprg16(0x8000, ((latch.addr >> 6) & 0x0E) | ((latch.addr >> 5) & 0x01));
		setprg16(0xC000, ((latch.addr >> 6) & 0x0E) | ((latch.addr >> 5) & 0x01));
	}
	setchr8(latch.addr & 0x0F);
	setmirror(((latch.addr >> 4) & 0x01) ^ 0x01);
}

void Mapper261_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
}
