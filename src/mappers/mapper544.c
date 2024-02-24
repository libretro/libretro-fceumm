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

/* NES 2.0 Mapper 544 - Waixing FS306 */
#include "mapinc.h"
#include "vrc2and4.h"

static uint8 nt[4];
static uint8 cpuC;

static uint8 chrRamMask;
static uint8 chrRamCompare;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE;

static writefunc writePPU;
extern uint32 RefreshAddr;

static SFORMAT StateRegs[] = {
    { nt, 4, "NTBL" },
	{ &cpuC, 1, "CPUC" },
	{ &chrRamMask, 1, "CHRM" },
	{ &chrRamCompare, 1, "CHRB" },
	{ 0 },
};

static void M544PW(uint16 A, uint16 V) {
    V &= 0x1F;
    if (A == 0xC000) {
        V = cpuC;
    }
    setprg8(A, V);
}

static void M544CW(uint16 A, uint16 V) {
	if ((V & chrRamMask) == chrRamCompare) {
		setchr1r(0x10, A, V);
    } else {
		setchr1(A, V);
    }
}

static DECLFW(M544WriteExtra) {
	if (A & 0x04) {
		nt[A & 0x03] = V & 0x01;
		setmirrorw(nt[0], nt[1], nt[2], nt[3]);
	} else {
		cpuC = V;
		VRC24_FixPRG();
	}
}

static const uint8 compareMasks[8] = {
    0x28, 0x00, 0x4C, 0x64, 0x46, 0x7C, 0x04, 0xFF
};

static DECLFW(M544PPUWrite) {
	if (RefreshAddr < 0x2000) {
        uint8 reg = RefreshAddr >> 10;
        uint8 chrBank = vrc24.chr[reg];
		if (chrBank & 0x80) {
            if (chrBank & 0x10) {
                chrRamMask = 0x00;
                chrRamCompare = 0xFF;
            } else {
                chrRamMask = (chrBank & 0x40) ? 0xFE : 0xFC;
                chrRamCompare = compareMasks[((chrBank >> 1) & 0x01) | ((chrBank >> 2) & 0x02) | ((chrBank >> 4) & 0x04)];
            }
			VRC24_FixCHR();
		}
	}
	writePPU(A, V);
}

static void M544Power(void) {
	chrRamMask = 0xFC;
	chrRamCompare = 0x28;
    nt[0] = 0;
    nt[1] = 0;
    nt[2] = 1;
    nt[3] = 1;
    cpuC = ~1;
	VRC24_Power();
	writePPU = GetWriteHandler(0x2007);
	SetWriteHandler(0x2007, 0x2007, M544PPUWrite);
}

static void M544Close(void) {
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
    }
	CHRRAM = NULL;
}

void Mapper544_Init(CartInfo *info) {
    VRC24_Init(info, VRC4, 0x400, 0x800, 1, 1);
	info->Power = M544Power;
	info->Close = M544Close;
    VRC24_pwrap = M544PW;
	VRC24_cwrap = M544CW;
    VRC24_miscWrite = M544WriteExtra;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAMSIZE = 2048;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}
