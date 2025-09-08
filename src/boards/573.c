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

static DECLFR(readOB) {
	return X.DB;
}

static void sync () {
	if (Latch_address &0x02) {
		setprg16(0x8000, Latch_address <<3 &0x10 | Latch_data &0x0F);
		setprg16(0xC000, Latch_address <<3 &0x10 |             0x0F);
	} else {
		setprg16(0x8000, Latch_address <<3 &0x18 | Latch_data &0x07);
		setprg16(0xC000, Latch_address <<3 &0x18 |             0x07);
	}
	SetReadHandler(0x8000, 0xFFFF, Latch_address &0x10 && ROM_size >8? readOB: CartBR);
	setchr8(0);
}

static void trapLatchWrite (uint16 *newAddress, uint8 *newValue, uint8 romValue) {
	if (Latch_address &0x20) *newAddress = Latch_address; /* Once bit 5 is set, only the inner bank bits can be modified. */
	*newValue &= romValue; /* AND-type bus conflicts */
}

void Mapper573_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, trapLatchWrite);
	info->Reset = Latch_power;
}
