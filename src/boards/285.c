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

static uint8 pad;

static void sync_submapper0 () {
	if (Latch_data &0x40)
		setprg32(0x8000, Latch_data >>1);
	else {
		setprg16(0x8000, Latch_data);
		setprg16(0xC000, Latch_data |7);
	}
	setchr8(0);
	if (Latch_data &0x80)
		setmirror(Latch_data &0x20? MI_1: MI_0);
	else
		setmirror(Latch_data &0x20? MI_H: MI_V);
}

static void sync_submapper1 () {
	if (Latch_data &0x40)
		setprg32(0x8000, Latch_data >>1 &0x03 | Latch_data >>2 &~0x03);
	else {
		setprg16(0x8000, Latch_data >>1 &~0x07 | Latch_data &0x07);
		setprg16(0xC000, Latch_data >>1        |             0x07);
	}
	setchr8(0);
	if (Latch_data &0x80)
		setmirror(Latch_data &0x20? MI_1: MI_0);
	else
		setmirror(Latch_data &0x08? MI_H: MI_V);
}

static DECLFR (readPad) {
	if (A &0x80) return pad >= 20? (4 | pad % 20): 0; else
	if (A &0x40) return pad >= 16? (4 | pad % 16): 0; else
	if (A &0x20) return pad >= 12? (4 | pad % 12): 0; else
	if (A &0x10) return pad >=  8? (4 | pad % 8):  0; else
	return pad >= 8? 0: pad &7;
}

static void power() {
	pad = 0;
	Latch_power();
	SetReadHandler(0x5000, 0x5FFF, readPad);
}

static void reset() {
	pad = ++pad %24;
	Latch_clear();
}

void Mapper285_Init (CartInfo *info) {
	Latch_init(info, info->submapper ==1? sync_submapper1: sync_submapper0, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&pad, 1, 0, "DIPS");
}
