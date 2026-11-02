/* FCE Ultra - NES/Famicom Emulator: 125
 *
 * Copyright notice for this file:
 *  Copyright (C) 2026 NewRisingSun
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
#include "asic_latch.h"

static void sync () {
	if (Latch_data &0x80) 
		setprg32(0x8000, Latch_data >>4);
	else {
		setprg16(0x8000, Latch_data >>4);
		setprg16(0xC000, Latch_data >>4);
	}
	setchr8(Latch_data);
	setmirror(Latch_data &1? MI_H: MI_V);
}

static void reset () {
	Latch_clear();
	Latch_data = 0x80;
	sync();
}

static void power () {
	Latch_power();
	Latch_data = 0x80;
	sync();
}

void Mapper634_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
}
