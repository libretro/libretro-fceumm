/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

/* NES 2.0 Mapper 520 - VRC4 clone
 * Datach Dragon Ball Z/Datach Yu Yu Hakusho
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 PPUCHRBus;

static SFORMAT StateRegs[] = {
	{ &PPUCHRBus, 1, "PPUC" },
	{ 0 },
};

static void M520PW(uint16 A, uint16 V) {
	setprg8(A, ((vrc24.chr[PPUCHRBus] << 2) & 0x20) | (V & 0x1F));
}

static void M520CW(uint16 A, uint16 V) {
	setchr1(A, V & 0x07);
}

static void M520PPUHook(uint32 A) {
	uint8 bank = (A & 0x1FFF) >> 10;
	if ((PPUCHRBus != bank) && ((A & 0x3000) != 0x2000)) {
		PPUCHRBus = bank;
		VRC24_FixPRG();
	}
}

void Mapper520_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x04, 0x08, 0, 1);
	PPU_hook = M520PPUHook;
	VRC24_pwrap = M520PW;
	VRC24_cwrap = M520CW;
	AddExState(StateRegs, ~0, 0, NULL);
}
