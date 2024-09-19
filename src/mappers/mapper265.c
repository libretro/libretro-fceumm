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
 */

/* NES 2.0 Mapper 265 - BMC-T-262 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint8 bank  = ((latch.addr >> 3) & 0x60) | ((latch.addr >> 2) & 0x18) | (latch.data & 0x07);

	if (latch.addr & 0x80) {
		if (!bank) {
			/* NOTE: Unofficial support but Tetris II in 11-in-1 variant
			works as if its NROM-256 */
			setprg32(0x8000, bank >> 1);
		} else {
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	} else {
		setprg16(0x8000, bank);
		setprg16(0xC000, bank | 0x07);
	}
	setchr8(0);
	setmirror(((latch.addr >> 1) & 0x01) ^ 0x01);
}

static DECLFW(M265Write) {
	if (!(latch.addr & 0x2000)) {
		latch.addr = A;
	}
	latch.data = V;
	Sync();
}

static void M265Power(void) {
	Latch_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M265Write);
}

void Mapper265_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M265Power;
	info->Reset = Latch_RegReset;
}
