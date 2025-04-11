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
#include "vrc2and4.h"

static void sync () {
	VRC24_syncWRAM(0);
	VRC24_syncPRG(0x03F, 0x000);  /* A real VRC4 has a PRG-ROM maximum of 256 KiB, but some hacks use more than that. */
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
	if (ROM_size &1) setprg16(0xC000, ROM_size -1); /* A number of Contra hacks have a PRG-ROM size of 400 KiB, expecting the last 16 KiB to be determined via subtraction. They would not work on real hardware. */
}

void Mapper21_Init (CartInfo *info) {
	switch(info->submapper) {
		case 1:  VRC24_init(info, sync, 0x02, 0x04, 1, 1, 8); break;
		case 2:  VRC24_init(info, sync, 0x40, 0x80, 1, 1, 8); break;
		default: VRC24_init(info, sync, 0x42, 0x84, 1, 1, 8); break;
	}
}

int Mapper22_getCHRBank(int bank) {
	return VRC24_chr[bank &7] >>1;
}

void Mapper22_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x02, 0x01, 0, 0, 8);
	VRC24_GetCHRBank =Mapper22_getCHRBank;
}

DECLFR(Mapper23_readProtection) {
	return VRC2_pins;
}

void Mapper23_Init (CartInfo *info) {
	switch(info->submapper) {
		case 1:  VRC24_init(info, sync, 0x01, 0x02, 1, 1, 8); break;
		case 2:  VRC24_init(info, sync, 0x04, 0x08, 1, 1, 8); break;
		case 3:  VRC24_init(info, sync, 0x01, 0x02, 0, 0, 8);
		         VRC24_WRAMRead =Mapper23_readProtection;
		         break;
		default: VRC24_init(info, sync, 0x05, 0x0A, 1, 1, 8); break;
	}
}

void Mapper25_Init (CartInfo *info) {
	switch(info->submapper) {
		case 1:  VRC24_init(info, sync, 0x02, 0x01, 1, 1, 8); break;
		case 2:  VRC24_init(info, sync, 0x08, 0x04, 1, 1, 8); break;
		case 3:  VRC24_init(info, sync, 0x02, 0x01, 0, 0, 8); break;
		default: VRC24_init(info, sync, 0x0A, 0x05, 1, 1, 8); break;
	}
}
