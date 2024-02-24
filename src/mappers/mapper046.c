/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, ((reg & 0x0F) << 1) | (latch.data & 0x01));
	setchr8(((reg & 0xF0) >> 1) | ((latch.data >> 4) & 0x07));
}

static DECLFW(M046WriteReg) {
	reg = V;
	Sync();
}

static void M046Power(void) {
	reg = 0;
	Latch_Power();
	SetWriteHandler(0x6000, 0x7FFF, M046WriteReg);
}

static void M046Reset(void) {
	reg = 0;
	Sync();
}

void Mapper046_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, TRUE);
	info->Power = M046Power;
	info->Reset = M046Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
