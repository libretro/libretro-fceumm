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
 * FDS Conversion - Metroid - Jin Ji Zhi Ling (Kaiser)(KS7037)[U][!]
 * NES 2.0 Mapper 307 - UNL-KS7037
 *
 */

#include "mapinc.h"
#include "n118.h"
#include "fdssound.h"

static void M307FixPRG(void) {
	setprg4r(0x10, 0x6000, 0);
	setprg4(0x7000, 15);
	setprg8(0x8000, n118.reg[6]);
	setprg4(0xA000, ~3);
	setprg4r(0x10, 0xB000, 0x01);
	setprg8(0xC000, n118.reg[7]);
	setprg8(0xE000, ~0);
}

static void M307FixCHR(void) {
	setchr8(0);
	setmirrorw(n118.reg[2] & 0x01, n118.reg[4] & 0x01, n118.reg[3] & 0x01, n118.reg[5] & 0x01);
}

static void M307Power(void) {
	FDSSound_Power();
	N118_Power();
	SetWriteHandler(0xB000, 0xBFFF, CartBW);
}

void Mapper307_Init(CartInfo *info) {
	N118_Init(info, 8, info->battery);
	info->Power = M307Power;
	N118_FixPRG = M307FixPRG;
	N118_FixCHR = M307FixCHR;
}
