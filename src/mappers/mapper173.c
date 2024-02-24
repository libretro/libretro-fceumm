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

static void M173Sync(void) {
	setprg32(0x8000, 0);
	if (iNESCart.CHRRomSize >= (16 * 1024)) {
		setchr8(((txc.output & 0x01) | (txc.Y ? 0x02 : 0x00) | ((txc.output & 2) << 0x01)));
    } else {
		setchr8(0);
    }
}

static DECLFW(M173Write) {
	TXC_Write(A, V & 0x0F);
}

static DECLFR(M173Read) {
	return ((cpu.openbus & 0xF0) | (TXC_Read(A) & 0x0F));
}

static void M173Power(void) {
	TXC_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, M173Read);
	SetWriteHandler(0x4100, 0xFFFF, M173Write);
}

void Mapper173_Init(CartInfo *info) {
	TXC_Init(info, M173Sync);
	info->Power = M173Power;
}
