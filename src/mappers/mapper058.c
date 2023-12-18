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

/*------------------ Map 058 ---------------------------*/

static void M58Sync(void) {
	if (latche & 0x40) {
		setprg16(0x8000, latche & 7);
		setprg16(0xC000, latche & 7);
	} else
		setprg32(0x8000, (latche >> 1) & 3);
	setchr8((latche >> 3) & 7);
	setmirror(((latche & 0x80) >> 7) ^ 1);
}

void Mapper58_Init(CartInfo *info) {
	Latch_Init(info, M58Sync, NULL, 0x0000, 0x8000, 0xFFFF, 0);
}

