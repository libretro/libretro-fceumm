/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2002 Xodnizel
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

/* S-009. UNROM plus outerbank register at $6000-$7FFF. */

#include "mapinc.h"
#include "latch.h"

static uint8 reg;

static void Sync(void) {
	setprg16(0x8000, (reg << 3) | (latch.data & 0x07));
	setprg16(0xC000, (reg << 3) | 0x07);
	setchr8(0);
	setmirror((reg >> 5) & 0x01);
}

static DECLFW(M434WriteOuterBank) {
	reg = V;
	Sync();
}

static void M434Reset(void) {
	reg = 0;
	Sync();
}

static void M434Power(void) {
	reg = 0;
	Latch_Power();
	SetWriteHandler(0x6000, 0x7FFF, M434WriteOuterBank);
}

void Mapper434_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, TRUE);
	info->Reset = M434Reset;
	info->Power = M434Power;
	AddExState(&reg, 1, 0, "REGS");
}
