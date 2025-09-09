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

static DECLFR(readOB) {
	return X.DB;
}

static void sync () {
	if (Latch_address &0x100) {
		if (Latch_address &0x001) {
			setprg16(0x8000, Latch_address >>2);
			setprg16(0xC000, Latch_address >>2);
		} else
			setprg32(0x8000, Latch_address >>3);
	} else {
		setprg16(0x8000, Latch_address >>2);
		setprg16(0xC000, Latch_address >>2 |7);
	}
	setchr8(Latch_address >>5 &7 | Latch_address >>1 &8);
	setmirror(Latch_address &0x002? MI_H: MI_V);
	SetReadHandler(0x8000, 0xFFFF, ~Latch_address &0x100 && Latch_address &0x001 &pad? readOB: CartBR);
}

static void power() {
	pad = 0;
	Latch_power();
}

static void reset() {
	pad++;
	Latch_clear();
}

void Mapper494_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&pad, 1, 0, "DIPS");
}
