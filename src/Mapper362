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

/* NES 2.0 Mapper 362 - PCB 830506C
 * 1995 Super HiK 4-in-1 (JY-005)
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 game;
static uint8 PPUCHRBus;

static SFORMAT StateRegs[] = {
    { &game,          1, "GAME" },
    { &PPUCHRBus,     1, "PPUC" },
	{ 0 },
};

static void M362PW(uint16 A, uint16 V) {
	uint16 prgBase = (game == 0) ? ((vrc24.chr[PPUCHRBus] & 0x180) >> 3) : 0x40;

	setprg8(A, prgBase | (V & 0x0F));
}

static void M362CW(uint16 A, uint16 V) {
	uint16 chrBase = (game == 0) ? (vrc24.chr[PPUCHRBus] & 0x180) : 0x200;
	uint16 chrMask = (game == 0) ? 0x7F : 0x1FF;

	setchr1(A, chrBase | (V & chrMask));
}

static DECLFW(M362CHRWrite) {
	VRC24_Write(A, V);
	if (A & 0x01) {
		/* NOTE: Because the lst higher 2 CHR-ROM bits are repurposed as PRG/CHR outer bank, 
	 	an extra PRG sync after a CHR write. */
		VRC24_FixPRG();
	}
}

static void M362PPUHook(uint32 A) {
    uint8 bank = (A & 0x1FFF) >> 10;
	if ((game == 0) && (PPUCHRBus != bank) && ((A & 0x3000) != 0x2000)) {
		PPUCHRBus = bank;
		VRC24_FixCHR();
		VRC24_FixPRG();
	}
}

static void M362Reset(void) {
	if (PRGsize[0] <= (512 * 1024)) {
        game = 0;
    } else {
        game = (game + 1) & 0x01;
    }
	VRC24_FixCHR();
	VRC24_FixPRG();
}

static void M362Power(void) {	
	PPUCHRBus = game = 0;
    VRC24_Power();
	SetWriteHandler(0xB000, 0xEFFF, M362CHRWrite);
}

void Mapper362_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x01, 0x02, 0, 0);
	info->Reset = M362Reset;
	info->Power = M362Power;
    PPU_hook = M362PPUHook;
	VRC24_pwrap = M362PW;
	VRC24_cwrap = M362CW;
	AddExState(StateRegs, ~0, 0, NULL);
}
