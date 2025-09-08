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
	if (Latch_address &0x20) { /* First chip: 256 KiB */
		setprg16(0x8000, Latch_address >>1 &0x08 | Latch_data &0x07);
		setprg16(0xC000, Latch_address >>1 &0x08 | 0x07);
	} else { /* Second chip: 128 KiB (boot) */
		setprg16(0x8000, 0x10 | Latch_address >>1 &0x07);
		setprg16(0xC000, 0x10 | Latch_address >>1 &0x07);
	}
	setchr8(0);
	setmirror(Latch_address &1? MI_H: MI_V);
}

static void trapLatchWrite (uint16 *newAddress, uint8 *newValue, uint8 romValue) { /* The address bits are only updated on a falling edge of D3. */
	if (!(Latch_data &0x08 && ~*newValue &0x08)) *newAddress = Latch_address;
}

void Mapper589_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, trapLatchWrite);
	info->Reset = Latch_power;
}
