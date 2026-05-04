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

/* Submapper 0 - K-3008, K-3032, K-3036 PCBs: H mirroring via A2 in NROM mode, and via A6 in UNROM mdoe.
   Submapper 1 - K-3055 PCB:                  H mirroring via A2 in NROM mode, and via A3=0,A4=1 in UNROM mode (used by Thundercade).
*/

static uint8_t submapper;
static void sync () {
	int prg = Latch_address >>2 &0x20 | Latch_address &0x1F;
	if (Latch_address &0x20) { /* NROM mode */
		if (Latch_address &0x01) {
			setprg16(0x8000, prg);
			setprg16(0xC000, prg);		
		} else
			setprg32(0x8000, prg >>1);
		setmirror(Latch_address &0x04? MI_H: MI_V);
	} else { /* UNROM mode */
		setprg16(0x8000, prg);
		setprg16(0xC000, prg | 7);
		setmirror(Latch_address &0x40 && submapper != 1 || ~Latch_address &0x08 && Latch_address &0x10 && submapper == 1? MI_H: MI_V);
	}
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, Latch_address &0x20? 0: 1);
	setchr8(0);
}

void Mapper340_Init (CartInfo *info) {
	submapper = info->submapper;
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Reset = Latch_clear;
}
