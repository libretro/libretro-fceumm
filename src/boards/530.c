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
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

int UNLAX5705_getPRGBank(int bank) {
	int result =VRC24_getPRGBank(bank);
	return result <<2 &0x8 | result >>2 &0x2 | result &~0xA;
}

int UNLAX5705_getCHRBank(int bank) {
	int result =VRC24_getCHRBank(bank);
	return result <<1 &0x40 | result >>1 &0x20 | result &~0x60;
}

DECLFW(UNLAX5705_unscrambleAddress) {
	VRC24_writeReg(A &~0x1000 | A <<9 &0x1000, V);
}


void UNLAX5705_power (void) {
	VRC24_power();
	SetWriteHandler(0x8000, 0xFFFF, UNLAX5705_unscrambleAddress);
}

void UNLAX5705_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 1, 1, 0);
	info->Power =UNLAX5705_power;
	VRC24_GetPRGBank =UNLAX5705_getPRGBank;
	VRC24_GetCHRBank =UNLAX5705_getCHRBank;
}
