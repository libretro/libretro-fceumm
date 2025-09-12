/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 *
 */

#include "mapinc.h"
#include "asic_n118.h"
#include "cartram.h"

static void sync() {
	setprg8r(0x10, 0x6000, 0);
	N118_syncPRG(0x0F, 0x00);
	setchr8(0);
	setmirror(N118_getCHRBank(7) &0x20? MI_H: MI_V);
}

void Mapper557_Init(CartInfo *info) {
	N118_init(info, sync, NULL, NULL);
	WRAM_init(info, 8);
}
