/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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
#include "cartram.h"

static uint8_t unrom;

static void sync0 () {
	if (Latch_address &0x01)
		setprg32(0x8000, Latch_address >>2);
	else {
		setprg16(0x8000, Latch_address >>1);
		setprg16(0xC000, Latch_address >>1);
	}
	setchr8(Latch_data >>1);
	setmirror(Latch_data &0x01? MI_H: MI_V);
}

static void sync1 () {
	int mask = ROM_size -9;
	if (unrom) {
		mask++;
		setprg16(0x8000, Latch_data &7 | mask);
		setprg16(0xC000,             7 | mask);
		setchr8r(0x10, 0);
		setmirror(MI_V);
	} else {
		if (Latch_address &0x01)
			setprg32(0x8000, Latch_address >>2 &(mask >>1));
		else {
			setprg16(0x8000, Latch_address >>1 &mask);
			setprg16(0xC000, Latch_address >>1 &mask);
		}
		setchr8(Latch_data >>1);
		setmirror(Latch_data &0x01? MI_H: MI_V);
	}
}

static void power () {
	unrom = 0;
	Latch_power();
}

static void reset () {
	unrom = !unrom;
	Latch_clear();
}

void Mapper438_Init (CartInfo *info) {
	if (info->submapper == 1) {
		Latch_init(info, sync1, 0x8000, 0xFFFF, NULL);
		CHRRAM_init(info, 8);
		info->Power = power;
		info->Reset = reset;
		AddExState(&unrom, 1, 0, "UNRO");
	} else {
		Latch_init(info, sync0, 0x8000, 0xFFFF, NULL);
		info->Reset = Latch_clear;
	}
}
