/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2016 CaH4e3
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
 * Test Ver. 1.01 Dlya Proverki TV Pristavok (RT-01, by SS aka Snake)
 * A simple board with 16K PRG ROM + 2K CHR ROM with no mapper, hardwired mirroring
 * PRG EPROM has copy protected areas with "weak bits", which is tested at some
 * points of the program. Trying to simalate "weak bits" behaviour
 * 
 */

/* NES 2.0 Mapper 328 - UNL-RT-01 */

#include <stdlib.h>
#include "mapinc.h"

static DECLFR(M328Read) {
	if (((A >= 0xCE80) && (A < 0xCF00)) || ((A >= 0xFE80) && (A < 0xFF00))) {
		return 0xF2 | (rand() & 0x0D);
	}
	return CartBR(A);
}

static void M328Power(void) {
	setprg16(0x8000, 0);
	setprg16(0xC000, 0);
	setchr2(0x0000,0);
	setchr2(0x0800,0);
	setchr2(0x1000,0);
	setchr2(0x1800,0);
	SetReadHandler(0x8000, 0xFFFF, M328Read);
}

void Mapper328_Init(CartInfo *info) {
	info->Power = M328Power;
}
