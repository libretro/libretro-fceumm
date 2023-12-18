/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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

/* Forward declarations */
extern uint16 latche;

/*------------------ Map 212 ---------------------------*/

static uint8 M212Read(uint32 A) {
	uint8 ret = CartBROB(A);
	if ((A & 0xE010) == 0x6000)
		ret |= 0x80;
	return ret;
}

static void M212Sync(void) {
	if (latche & 0x4000) {
		setprg32(0x8000, (latche >> 1) & 3);
	} else {
		setprg16(0x8000, latche & 7);
		setprg16(0xC000, latche & 7);
	}
	setchr8(latche & 7);
	setmirror(((latche >> 3) & 1) ^ 1);
}

void Mapper212_Init(CartInfo *info) {
	Latch_Init(info, M212Sync, M212Read, 0x0000, 0x8000, 0xFFFF, 0);
}
