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
 */

/* NES 2.0 Mapper 354 denotes three different PCBs:
 * - FAM250 for the 1992 巨作 250-in-1 劃面選關 鑽石巨王 multicart (iNESCart.submapper 0),
 * - 810139C for the 1992 劃面選關 400-in-1 創新版 as well as an undumped 650-in-1 multicart (also iNESCart.submapper 0),
 * - 810331C/SCHI-24 for the 1992 劃面選關 1050-in-1 multicart (iNESCart.submapper 1).
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint16 prg = ((latch.addr >> 5) & 0x80) | ((latch.addr << 2) & 0x40) | (latch.data & 0x3F);

	switch (latch.addr & 0x07) {
	case 0:
	case 4:
		setprg32(0x8000, prg >> 1);
		break;
	case 1:
		setprg16(0x8000, prg);
		setprg16(0xC000, prg | 0x07);
		break;
	case 2:
	case 6:
		setprg8(0x8000, (prg << 1) | (latch.data >> 7));
		setprg8(0xA000, (prg << 1) | (latch.data >> 7));
		setprg8(0xC000, (prg << 1) | (latch.data >> 7));
		setprg8(0xE000, (prg << 1) | (latch.data >> 7));
		break;
	case 3:
	case 7:
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
		break;
	case 5:
		setprg8(0x6000, (prg << 1) | (latch.data >> 7));
		setprg32(0x8000, (prg >> 1) | 0x03);
		break;
	}
	/* CHR-RAM write protect */
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], ((latch.addr >> 3) & 0x01) ^ 0x01);
	setchr8(0);
	setmirror(((latch.data >> 6) & 0x01) ^ 0x01);
}

static DECLFW(M354Write) {
	uint32 addr = 0xE000 | (iNESCart.submapper == 0) ? 0x1000 : 0;

	if (A < addr) {
		return;
	}

	latch.addr = A;
	latch.data = V;
	Sync();
}

static void M354_Power(void) {
	Latch_Power();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M354Write);
}

void Mapper354_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M354_Power;
	info->Reset = Latch_RegReset;
}