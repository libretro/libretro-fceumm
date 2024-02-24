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
#include "jv001.h"

static void M172Sync(void) {
	setprg32(0x8000, 0);
	setchr8(jv001.output);
	setmirror((jv001.X & 1) ^ 1);
}

static uint8 GetVal(uint8 V) {
	return (((V << 5) & 0x20) | ((V << 3) & 0x10) | ((V << 1) & 0x08) | ((V >> 1) & 0x04) |
	        ((V >> 3) & 0x02) | ((V >> 5) & 0x01));
}

static DECLFW(M172Write) {
	JV001_Write(A, GetVal(V));
}

static DECLFR(M172Read) {
	return (cpu.openbus & 0xC0) | GetVal(JV001_Read(A));
}

static void M172Power(void) {
	JV001_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, M172Read);
	SetWriteHandler(0x4100, 0xFFFF, M172Write);
}

void Mapper172_Init(CartInfo *info) {
	JV001_Init(info, M172Sync);
	info->Power = M172Power;
}
