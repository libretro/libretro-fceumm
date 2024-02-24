/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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
 * NES 2.0 Mapper 396 - BMC-830752C
 * 1995 Super 8-in-1 (JY-050 rev0)
 * Super 8-in-1 Gold Card Series (JY-085)
 * Super 8-in-1 Gold Card Series (JY-086)
 * 2-in-1 (GN-51)
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	if ((latch.addr & 0x6000) == 0x2000) {
		reg = latch.data;
	}
	setprg16(0x8000, (reg << 3) | (latch.data & 0x07));
	setprg16(0xC000, (reg << 3) | 0x07);
	setchr8(0);
	setmirror((reg & 0x60) ? MI_H : MI_V);
}

static void M396Reset(void) {
	reg = 0;
	Latch_RegReset();
}

void Mapper396_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Reset = M396Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
