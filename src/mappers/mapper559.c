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

/* NES 2.0 Mapper 559 - VRC4 clone
 * Subor 0102
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 nt[4];
static uint8 cpuC;

static SFORMAT StateRegs[] = {
	{ &cpuC, 1, "CPUC" },
    { nt, 4, "NTBL" },
	{ 0 },
};

static void M559PW(uint16 A, uint16 V) {
    V &= 0x1F;
    if (A == 0xC000) {
        V = cpuC;
    }
    setprg8(A, V);
}

static void M559CW(uint16 A, uint16 V) {
    setchr1(A, V & 0x1FF);
}

static void M559MIRR(void) {
    setmirrorw(nt[0], nt[1], nt[2], nt[3]);
}

static DECLFW(M559WriteMisc) {
	if (A & 0x04) {
		nt[A & 0x03] = V & 0x01;
		VRC24_FixMIR();
	} else {
		cpuC = V;
		VRC24_FixPRG();
	}
}

static DECLFW(M559WriteNibble) {
    /* nibblize address */
    if (A & 0x400) {
        V >>= 4;
    }
    VRC24_Write(A, V);
}

static void M559Power(void) {
    nt[0] = 0;
    nt[1] = 0;
    nt[2] = 1;
    nt[3] = 1;
    cpuC = ~1;
    VRC24_Power();
    setprg8(0xC000, cpuC);
    setmirrorw(nt[0], nt[1], nt[2], nt[3]);
    SetWriteHandler(0xB000, 0xFFFF, M559WriteNibble);
}

void Mapper559_Init(CartInfo *info) {
    VRC24_Init(info, VRC4, 0x400, 0x800, 1, 1);
    info->Power = M559Power;
    VRC24_FixMIR = M559MIRR;
    VRC24_pwrap = M559PW;
    VRC24_cwrap = M559CW;
    VRC24_miscWrite = M559WriteMisc;
	AddExState(StateRegs, ~0, 0, NULL);
}
