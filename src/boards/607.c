/* FCE Ultra - NES/Famicom Emulator
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

static uint8 reg;

static DECLFR (readOB) {
	return X.DB;
}

static void sync () {
	if (reg &0x10) { /* UNROM mode */
		setprg16(0x8000, reg <<3 &0x08 | Latch_data &0x07);
		setprg16(0xC000, reg <<3 &0x08 |             0x07);
	} else /* NROM-256 mode */
		setprg32(0x8000, reg <<3 &0x04 | Latch_data >>1 &0x03);
	SetReadHandler(0x8000, 0xFFFF, reg &0x0C? readOB: CartBR); /* 6000.2 and 6000.3 seem to select additional UNROM games. Without a cartridge that has them, the exact workings remain unknown for now. */
	setchr8(0);
	setmirror(reg &0x20? MI_H: MI_V);
}

static DECLFW (writeReg) {
	reg = V;
	sync();
}

static void reset () {
	reg = 0;
	Latch_clear();
}

static void power () {
	Latch_power();
	SetWriteHandler(0x6000, 0x7FFF, writeReg);
	reset();
}

void Mapper607_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "REGS");
}
