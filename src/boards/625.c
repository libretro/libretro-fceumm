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

static void sync () {
	int prg = Latch_data >>1 &0x40 | Latch_address <<5 &0x20 | Latch_data &0x1F;
	if (prg &0x40) prg ^= 0x20; /* 0,1,2,3 -> 0,1,3,2 so that the UNROM game comes at the end */
	if (Latch_address &0x08) { /* UNROM mode */
		setprg16(0x8000, prg);
		setprg16(0xC000, prg |7);
	} else
	if (Latch_data &0x20) { /* NROM-128 mode */
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	} else /* NROM-256 mode */
		setprg32(0x8000, prg >>1);
	setchr8(0);
	setmirror(Latch_data &0x40? MI_V: MI_H);
}

static void trapLatchWrite (uint16_t *newAddress, uint8_t *newValue, uint8_t romValue) { /* Only update PRG A14-A17 in UNROM mode. */
	if (Latch_address &0x08) {
		*newAddress = Latch_address;
		*newValue = Latch_data &~0x07 | *newValue &0x07;
	}
}

void Mapper625_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, trapLatchWrite);
	info->Reset = Latch_clear;
}
