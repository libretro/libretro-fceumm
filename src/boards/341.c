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

static uint8 submapper;

static void sync () {
	setprg32(0x8000, Latch_address >>8);
	setchr8(Latch_address >>8);
	setmirror(Latch_address &(submapper == 1? 0x800: 0x200)? MI_H: MI_V);
}

static void trapLatchWrite (uint16 *newAddress, uint8 *newValue, uint8 romValue) {
	if (!((*newAddress &0xF0) == 0xA0)) *newAddress = Latch_address; /* Update address only if new A7..A4 = $A */
}

void Mapper341_Init (CartInfo *info) {
	submapper = info->submapper;
	Latch_init(info, sync, 0x8000, 0xFFFF, submapper == 1? trapLatchWrite: NULL);
	info->Reset = Latch_clear;
}
