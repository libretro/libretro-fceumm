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

static uint8 half;
static uint8 reg;

static void sync () {
	setprg16(0x8000, half <<5 &0x20 | reg <<3 &0x18 | Latch_data &0x07);
	setprg16(0xC000, half <<5 &0x20 | reg <<3 &0x18 |             0x07);
	setchr8(0);
	setmirror(reg &0x04? MI_H: MI_V);
}

static DECLFW (writeReg) {
	reg = V;
	sync();
}

static void power () {
	half = 0;
	reg = 0;
	Latch_power();
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
}

static void reset () {
	half ^= 1;
	reg = 0;
	RAM[0x100] =0;
	Latch_clear();
}

void Mapper301_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&half, 1, 0, "HALF");
	AddExState(&reg, 1, 0, "EXPR");
}
