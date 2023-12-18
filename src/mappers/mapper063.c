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
extern uint32 submapper;
void Latch_Init(CartInfo *info, void (*proc)(void), readfunc func, uint16 linit, uint16 adr0, uint16 adr1, uint8 wram);

/*------------------ Map 063 ---------------------------*/
/* added 2019-5-23
 * Mapper 63 NTDEC-Multicart
 * http://wiki.nesdev.com/w/index.php/INES_Mapper_063
 * - Powerful 250-in-1
 * - Hello Kitty 255-in-1
 * Submapper 1:
 * - NTDEC 82-in-1 */

static uint16 openBus;

static uint8 M63Read(uint32 A) {
	if (openBus)
		return X.DB;
	return CartBR(A);
}

static void M63Sync(void) {
	uint16 prg =latche >>2 &(submapper ==1? 0x7F: 0xFF);
	if (latche &2)
		setprg32(0x8000, prg >>1);
	else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	}
	openBus = prg >= ROM_size;
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, latche &(submapper ==1? 0x200: 0x400)? 0: 1);
	setchr8(0);
	setmirror(latche &1? MI_H: MI_V);
}

void Mapper63_Init(CartInfo *info) {
	submapper = info->submapper;
	Latch_Init(info, M63Sync, M63Read, 0x0000, 0x8000, 0xFFFF, 0);
}

