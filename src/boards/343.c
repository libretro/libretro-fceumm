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

static void sync () {
	setprg32(0x8000, Latch_data);
	setchr8(0);
	setmirror(reg &0x08? MI_H: MI_V);
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
	reg = 0;
	Latch_power();
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
}

void Mapper343_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "REGS");
}
