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
#include "cartram.h"

static uint8 chr;

static void sync () {
	setprg8r(0x10, 0x6000, 0);
	setprg32(0x8000, Latch_data);
	setchr4(0x0000, Latch_data &0x80? chr: 0);
	setchr4(0x1000, 1);
	setmirror(Latch_data &0x40? MI_H: MI_V);
}

static void FP_FASTAPASS(1) trapPPUAddressChange (uint32 A) {
	if (A &0x2000 && (A &0x23C0) < 0x23C0) {
		chr = A >>(10 +(Latch_data &0x40? 1: 0)) &1;
		setchr4(0x0000, Latch_data &0x80? chr: 0);
	}
}

void Mapper514_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	WRAM_init(info, 8);
	info->Reset = Latch_clear;
	PPU_hook = trapPPUAddressChange;
	AddExState(&chr, 1, 0, "CHRB");
}
