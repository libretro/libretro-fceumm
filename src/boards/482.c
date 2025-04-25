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
 *
 */

#include "mapinc.h"
#include "latch.h"

static uint8 dip;

static SFORMAT StateRegs[] = {
    { &dip, 1, "DIPS" },
    { 0 }
};

static void Sync(void) {
	if (latch.data &0x80) {
		setprg16(0x8000, latch.data &0x3F);
		setprg16(0xC000, latch.data &0x3F);
	} else
		setprg32(0x8000, latch.data >>1 &0x1F);
	setchr8(0);
	setmirror(latch.data &0x40? MI_H: MI_V);	
}

static DECLFR(interceptRead) {
	if ((latch.addr &3) ==3)
		return dip &7;
	else
		return CartBR(A);
}

static void Power() {
	dip =0;
	LatchPower();
	SetReadHandler(0x8000, 0xFFFF, interceptRead);
}

static void Reset() {
	dip++;
	latch.data =0;
	latch.addr =0;
	Sync();
}

void Mapper482_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
	info->Power = Power;
	info->Reset = Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
