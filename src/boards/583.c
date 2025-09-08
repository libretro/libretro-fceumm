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
	int prg = Latch_address &~0xF | Latch_address <<3 &0x08;
	if (Latch_address &0x20) {
		setprg32(0x8000, prg >>1 &~7 | Latch_data &7);
		setmirror(Latch_data &0x10? MI_1: MI_0);
	} else {
		if (Latch_address &0x10 && Latch_address &0x08) {
			setprg16(0x8000, prg &~0x03 | Latch_data &0x03);
			setprg16(0xC000, prg        |             0x03);
			setmirror(MI_H);
		} else {
			setprg16(0x8000, prg &~0x07 | Latch_data &0x07);
			setprg16(0xC000, prg        |             0x07);
			setmirror(MI_V);
		}
	}
	setchr8(0);
}

static void trapLatchWrite (uint16 *newAddress, uint8 *newValue, uint8 romValue) { /* The address bits are only updated on a rising edge of A7. */
	if (!(~Latch_address &0x80 && *newAddress &0x80)) *newAddress = Latch_address;
}

void Mapper583_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, trapLatchWrite);
	info->Reset = Latch_power;
}
