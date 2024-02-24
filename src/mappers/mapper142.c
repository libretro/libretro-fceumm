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

/* Mapper 142 - UNL KS7032 */

 #include "mapinc.h"
 #include "ks202.h"

 static void Sync(void) {
	setprg8(0x6000, ks202.reg[4] & 0x0F);
	setprg8(0x8000, ks202.reg[1] & 0x0F);
	setprg8(0xA000, ks202.reg[2] & 0x0F);
	setprg8(0xC000, ks202.reg[3] & 0x0F);
	setprg8(0xE000, ~0 & 0x0F);
	setchr8(0);
}

void Mapper142_Init(CartInfo *info) {
	KS202_Init(info, Sync, 0, 0);
}
