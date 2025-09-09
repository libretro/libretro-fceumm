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
	if (Latch_address &0x40) /* AOROM */
		setprg32(0x8000, Latch_address >>2 &~0x07 | Latch_data &0x07);
	else
	if (Latch_address &0x20) /* ANROM */
		setprg32(0x8000, Latch_address >>2 &~0x03 | Latch_data &0x03);
	else /* NROM-256 */
		setprg32(0x8000, Latch_address >>2 &~0x03 | Latch_address &0x03);
	setchr8(0);
	if (Latch_address &0x60) /* AxROM */
		setmirror(Latch_data &0x10? MI_1: MI_0);
	else
		setmirror(Latch_data &0x10? MI_H: MI_V);
}

static void trapLatchWrite (uint16 *newAddress, uint8 *newValue, uint8 romValue) { /* After the lock bit is set, only update the data bits and the lowest two address bits. */
	if (Latch_address &0x80) *newAddress = Latch_address &~0x03 | *newAddress &0x03;
}

void Mapper574_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, trapLatchWrite);
	info->Reset = Latch_clear;
}
