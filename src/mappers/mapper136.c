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

static void M136Sync(void) {
	setprg32(0x8000, (jv001.output >> 4) & 0x01);
	setchr8(jv001.output & 0x07);
}

static DECLFW(M136Write) {
	JV001_Write(A, V & 0x3F);
}

static DECLFR(M136Read) {
	return ((cpu.openbus & 0xC0) | (JV001_Read(A) & 0x3F));
}

static void M136Power(void) {
	JV001_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, M136Read);
	SetWriteHandler(0x4100, 0xFFFF, M136Write);
}

void Mapper136_Init(CartInfo *info) {
	JV001_Init(info, M136Sync);
	info->Power = M136Power;
}
