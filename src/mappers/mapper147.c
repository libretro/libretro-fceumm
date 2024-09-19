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

static void M147Sync(void) {
	setprg32(0x8000, ((jv001.output >> 4) & 0x02) | (jv001.output & 0x01));
	setchr8((jv001.output >> 1) & 0x0F);
}

static DECLFW(M147Write) {
	JV001_Write(A, ((V >> 2) & 0x3F) | ((V << 6) & 0xC0));
}

static DECLFR(M147Read) {
	uint8 ret = JV001_Read(A);
	return ((ret << 2) | ((ret >> 6) & 0x03));
}

static void M147Power(void) {
	JV001_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, M147Read);
	SetWriteHandler(0x4100, 0xFFFF, M147Write);
}

void Mapper147_Init(CartInfo *info) {
	JV001_Init(info, M147Sync);
	info->Power = M147Power;
}
