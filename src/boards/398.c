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
#include "asic_vrc2and4.h"

static uint8 reg;

static SFORMAT stateRegs[] ={
	{ &reg, 1, "EXP0" },
	{ 0 }
};

static void sync () {
	if (reg &0x80) {
		setprg32(0x8000, reg >>5 &6 | VRC24_getCHRBank(0) >>2 &1);
		setchr8(0x40 | reg >>3 &8 | VRC24_getCHRBank(0) &7);
	} else {
		VRC24_syncPRG(0x0F, 0x00);
		VRC24_syncCHR(0x1FF, 0x000);
	}
	VRC24_syncMirror();
}

static DECLFW (writeReg) {
	reg =A &0xFF;	
	VRC24_writeReg(A, V);
}

static void power (void) {
	reg = 0xC0;
	VRC24_power();
	SetWriteHandler(0x8000, 0xFFFF, writeReg);
}

static void reset (void) {
	reg = 0xC0;
	VRC24_clear();
}	

void Mapper398_Init (CartInfo *info) {
	VRC4_init(info, sync, 0x01, 0x02, 1, NULL, NULL, NULL, NULL, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(stateRegs, ~0, 0, 0);
}
