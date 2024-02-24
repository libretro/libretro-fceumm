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

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint8 prg   = latch.data & 0x3F;

	setprg8r(0x10, 0x6000, 0);
	switch (latch.addr & 0x03) {
	case 0:
		setprg32(0x8000, prg >> 1);
		break;
	case 1:
		setprg16(0x8000, prg);
		setprg16(0xC000, prg | 0x07);
		break;
	case 2:
		setprg8(0x8000, (prg << 1) | (latch.data >> 7));
		setprg8(0xA000, (prg << 1) | (latch.data >> 7));
		setprg8(0xC000, (prg << 1) | (latch.data >> 7));
		setprg8(0xE000, (prg << 1) | (latch.data >> 7));
		break;
	case 3:
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
		break;
	}
	setchr8(0);
	setmirror(((latch.data >> 6) & 0x01) ^ 0x01);
}

void Mapper015_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, TRUE, FALSE);
	info->Reset = Latch_RegReset;
}
