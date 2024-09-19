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

#include "mapinc.h"
#include "n118.h"

static void M088FixCHR(void) {
	setchr2(0x0000, (n118.reg[0] & 0x3F) >> 1);
	setchr2(0x0800, (n118.reg[1] & 0x3F) >> 1);
	setchr1(0x1000, 0x40 | (n118.reg[2] & 0x3F));
	setchr1(0x1400, 0x40 | (n118.reg[3] & 0x3F));
	setchr1(0x1800, 0x40 | (n118.reg[4] & 0x3F));
	setchr1(0x1C00, 0x40 | (n118.reg[5] & 0x3F));
}

void Mapper088_Init(CartInfo *info) {
	N118_Init(info, 0, 0);
	N118_FixCHR = M088FixCHR;
}
