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

static void sync () {
	int prg = Latch_address >>2 &0x1F;
	if (Latch_address &0x200) {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg |7);
	} else {
		if (Latch_address &0x001)
			setprg32(0x8000, prg >>1);
		else {
			setprg16(0x8000, prg);
			setprg16(0xC000, prg);
		}
	}
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], Latch_address &0x80? 0: 1);
	setchr8(0);
	setmirror(Latch_address &0x02? MI_H: MI_V);
}

void protectLatchBits (uint16 *address, uint8 *newValue, uint8 romValue) { /* Once bit 9 is set, only the inner bank bits can be modified. */
	if (Latch_address &0x200) *address = *address &0x01C | Latch_address &~0x01C;
}

void Mapper579_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, protectLatchBits);
	info->Reset = Latch_power;
}
