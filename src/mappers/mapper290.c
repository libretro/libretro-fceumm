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

/* NES 2.0 Mapper 290 - BMC-NTD-03 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint32 prg = ((latch.addr >> 10) & 0x1E) | ((latch.addr >> 6) & 0x01);
	uint32 chr = ((latch.addr >> 5) & 0x18) | (latch.addr & 0x07);

	if (latch.addr & 0x80) {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	} else {
		setprg32(0x8000, prg >> 1);
	}
	setchr8(chr);
	setmirror(((latch.addr >> 10) & 0x01) ^ 0x01);
}

void Mapper290_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Reset = Latch_RegReset;
}
