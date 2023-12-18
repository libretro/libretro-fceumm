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

/* forward declarations */
extern uint16 latche;
void Latch_Init(CartInfo *info, void (*proc)(void), readfunc func, uint16 linit, uint16 adr0, uint16 adr1, uint8 wram);

/*------------------ Map 464 ---------------------------*/
static void M464Sync(void) {
	int p =latche >>7;
	int c =latche &0x1F;
	if (latche &0x40) {
		setprg32(0x8000, p >> 1);
	} else {
		setprg16(0x8000, p);
		setprg16(0xC000, p);
	}
	setchr8(c);
	setmirror(latche &0x20? MI_H: MI_V);
}

void Mapper464_Init(CartInfo *info) {
	Latch_Init(info, M464Sync, NULL, 0x0000, 0x8000, 0xFFFF, 1);
}
