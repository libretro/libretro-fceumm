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

#include "mapinc.h"
#include "txc.h"

static uint8 chr = 0;

static void M036Sync(void) {
	setprg32(0x8000, txc.output & 0x03);
	setchr8(chr & 0x0F);
}

static DECLFW(M036Write) {
	if ((A & 0xF200) == 0x4200) {
		chr = V;
	}
	TXC_Write(A, (V >> 4) & 0x03);
}

static DECLFR(M036Read) {
	return (cpu.openbus & ~0x30) | ((TXC_Read(A) << 4) & 0x30);
}

static void M036Power(void) {
	chr = 0;
	TXC_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, M036Read);
	SetWriteHandler(0x4100, 0xFFFF, M036Write);
}

void Mapper036_Init(CartInfo *info) {
	TXC_Init(info, M036Sync);
	info->Power = M036Power;
	AddExState(&chr, 1, 0, "CREG");
}
