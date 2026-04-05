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

static uint8 pad;

static DECLFR (readOB) {
	return X.DB;
}

static void sync () {
	/* There are two PRG-ROM chips, the first can be either 256 KiB or 512 KiB in size, the second is always 128 KiB in size */
	int prgAND, prgOR;
	if (ROM_size >=32) {
		prgAND = Latch_address &0x400? 0x07: 0x1F;
		prgOR  = Latch_address &0x400? 0x20: 0x00;
	} else {
		prgAND = Latch_address &0x400? 0x07: 0x0F;
		prgOR  = Latch_address &0x400? 0x10: 0x00;
	}
	if (Latch_address &0x080) { /* NROM mode */
		if (Latch_address &0x001)
			setprg32(0x8000,(prgOR | Latch_address >>2 &prgAND) >>1);
		else {
			setprg16(0x8000, prgOR | Latch_address >>2 &prgAND);
			setprg16(0xC000, prgOR | Latch_address >>2 &prgAND);
		}
	} else { /* Inverse UNROM mode */
		setprg16(0x8000, prgOR | Latch_address >>2 &prgAND);
		setprg16(0xC000, prgOR);
	}
	SetReadHandler(0x8000, 0xFFFF, ROM_size >=32 && Latch_address &0x200 && pad &1? readOB: CartBR);
	if (Latch_address &0x400)
		setchr8(Latch_address >>6 &~0x03 | Latch_data &0x03);
	else
		setchr8r(0x10, 0);
	setmirror(Latch_address &0x002? MI_H: MI_V);
}

static void trapLatchWrite (uint16 *newAddress, uint8 *newValue, uint8 romValue) { /* Latch address bit 14 is an address lock bit */
	if (Latch_address &0x4000) *newAddress = Latch_address;
}

static void power () {
	pad = 0;
	Latch_power();
}

static void reset () {
	pad++;
	Latch_clear();
}

void Mapper599_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, trapLatchWrite);
	CHRRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&pad, 1, 0, "DIPS");
}
