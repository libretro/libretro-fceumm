/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

static void M206PW(uint16 A, uint16 V) {
	if (iNESCart.submapper == 1) {
		/* 3407, 3417 and 3451 PCBs */
		setprg32(0x8000, 0);
	} else {
		setprg8(A, V & 0x0F);
	}
}

void Mapper206_Init(CartInfo *info) {
	N118_Init(info, 0, 0);
	N118_pwrap = M206PW;
}
