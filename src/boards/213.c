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
	if (Latch_address &0x08) {
		setprg16(0x8000, Latch_address >>1 &~1 | Latch_address &1);
		setprg16(0xC000, Latch_address >>1 &~1 | Latch_address &1);
	} else
		setprg32(0x8000, Latch_address >>2);
	setchr8(Latch_address >>1 &~1 | Latch_address &1);
	setmirror(Latch_address &0x02? MI_H: MI_V);
}

void Mapper213_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Reset = Latch_clear;
}
