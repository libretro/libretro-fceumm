/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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

/*
Mapper 95 represents NAMCOT-3425, a board used only for the game Dragon Buster (J).

It is to the ordinary Namco 108 family boards (mapper 206) as TKSROM and TLSROM
(mapper 118) is to ordinary MMC3 boards. Instead of having hardwired mirroring
like mapper 206, it has CHR A15 directly controlling CIRAM A10, just as CHR A17
controls CIRAM A10 on TxSROM. Only horizontal mirroring and 1-screen mirroring
are possible because the Namco 108 lacks the C bit of MMC3.
*/

#include "mapinc.h"
#include "n118.h"

static uint8 mcache[8];
static uint32 lastppu;

static SFORMAT StateRegs[] = {
    { mcache, 8, "MCCH" },
	{ &lastppu, 4, "LPPU" },
    { 0 }
};

static void M095FixCHR(void) {
    setchr2(0x0000, n118.reg[0] >> 1);
	setchr2(0x0800, n118.reg[1] >> 1);
	setchr1(0x1000, n118.reg[2]);
	setchr1(0x1400, n118.reg[3]);
	setchr1(0x1800, n118.reg[4]);
	setchr1(0x1C00, n118.reg[5]);
	setmirror(MI_0 + mcache[lastppu]);
}

static DECLFW(M095Write) {
	switch (A & 0xE001) {
	case 0x8001:
        n118.reg[n118.cmd & 0x07] = V & 0x1F;
		switch (n118.cmd & 0x07) {
		case 0:
			mcache[0] = mcache[1] = (V >> 5) & 0x01;
            N118_FixCHR();
			break;
		case 1:
			mcache[2] = mcache[3] = (V >> 5) & 0x01;
            N118_FixCHR();
			break;
		case 2:
			mcache[4] = (V >> 5) & 0x01;
            N118_FixCHR();
			break;
		case 3:
			mcache[5] = (V >> 5) & 0x01;
            N118_FixCHR();
			break;
		case 4:
			mcache[6] = (V >> 5) & 0x01;
            N118_FixCHR();
			break;
		case 5:
			mcache[7] = (V >> 5) & 0x01;
            N118_FixCHR();
			break;
        case 6:
        case 7:
            N118_FixPRG();
            break;
		}
        break;
    default:
        N118_Write(A, V);
        break;
	}
}

static void MExMirrPPU(uint32 A) {
	static int8 lastmirr = -1, curmirr;
	if (A < 0x2000) {
		lastppu = A >> 10;
		curmirr = mcache[lastppu];
		if (curmirr != lastmirr) {
			setmirror(MI_0 + curmirr);
			lastmirr = curmirr;
		}
	}
}

static void M095Power(void) {
    lastppu = 0;
    memset(mcache, 0, sizeof(mcache));
    
    N118_Power();
    SetWriteHandler(0x8000, 0x9FFF, M095Write);
}

void Mapper095_Init(CartInfo *info) {
	N118_Init(info, 0, 0);
    info->Power = M095Power;
	N118_FixCHR = M095FixCHR;
    PPU_hook = MExMirrPPU;
    AddExState(StateRegs, ~0, 0, NULL);
}
