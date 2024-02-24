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

/* NES 2.0 Mapper 433 denotes the NC-20MB PCB, used for the 20-in-1 (CA-006) multicart. It is almost identical to INES Mapper 433, except that mirroring is selected just by single bit 6 (1=Horizontal).
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint8 prg = latch.data & 0x1F;

	if (latch.data & 0x20) {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	} else {
		setprg32(0x8000, prg >> 1);
	}
	setmirror(((latch.data >> 6) & 0x01) ^ 0x01);
	setchr8(0);
}

void Mapper433_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Reset = Latch_RegReset;
}
