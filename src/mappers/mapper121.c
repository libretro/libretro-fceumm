/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007-2008 Mad Dumper, CaH4e3
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
 *
 * Panda prince pirate.
 * MK4, MK6, A9711/A9713 board
 * 6035052 seems to be the same too, but with prot array in reverse
 * A9746  seems to be the same too, check
 * 187 seems to be the same too, check (A98402 board)
 *
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 prg[3];
static uint8 reg;
static uint8 readIndex;
static uint8 protIndex;
static uint8 protLatch;

static SFORMAT StateRegs[] = {
	{ prg, 3, "PREG" },
	{ &reg, 2, "REGS" },
	{ &readIndex, 1, "PRRD" },
	{ &protIndex, 1, "PRID" },
	{ &protLatch, 1, "PRLT" },
	{ 0 }
};

static void M121CW(uint16 A, uint16 V) {
	if ((ROM.prg.size * 16) > 256) {
		setchr1(A, ((reg & 0x80) << 1) | V);
	} else {
		if ((A & 0x1000) == (uint32)((mmc3.cmd & 0x80) << 5)){
			setchr1(A, 0x100 | V);
		} else {
			setchr1(A, V);
		}
	}
}

static void M121PW(uint16 A, uint16 V) {
	uint8 mask = 0x1F;
	uint8 base = ((reg & 0x80) >> 2);

	if ((protIndex & 0x20) && (A > 0x8000)) {
		mask = 0xFF;
		V = prg[((A >> 13) & 0x03) - 1];
	}

	setprg8(A, base | (V & mask));
}

static const uint8 prot_array[] = { 0x83, 0x83, 0x42, 0x00, 0x00, 0x02, 0x02, 0x03 };
static DECLFR(M121ReadLUT) {
	return prot_array[readIndex];
}

static DECLFW(M121WriteLUT) {
	readIndex = ((A >> 6) & 0x04) | (V & 0x03);
	if (A & 0x0100) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static DECLFW(M121Write) {
	switch (A & 0xE001) {
	case 0x8001:
		switch (A & 0x03) {
		case 0x01:
			protLatch = ((V & 0x01) << 5) | ((V & 0x02) << 3) |
			            ((V & 0x04) << 1) | ((V & 0x08) >> 1) |
			            ((V & 0x10) >> 3) | ((V & 0x20) >> 5);
			if ((protIndex == 0x26) || (protIndex == 0x28) || (protIndex == 0x2A)) {
				prg[0x15 - (protIndex >> 1)] = protLatch;
			}
			break;
		case 0x03:
			protIndex = V & 0x3F;
			if ((protIndex & 0x20) && protLatch) {
				prg[2] = protLatch;
			}
			break;
		}
		mmc3.reg[mmc3.cmd & 0x07] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		break;
	default:
		MMC3_CMDWrite(A, V);
		break;
	}
}

static void M121Power(void) {
	memset(prg, 0, sizeof(prg));
	reg = readIndex = protIndex = protLatch = 0;
	MMC3_Power();
	SetReadHandler(0x5000, 0x5FFF, M121ReadLUT);
	SetWriteHandler(0x5000, 0x5FFF, M121WriteLUT);
	SetWriteHandler(0x8000, 0x9FFF, M121Write);
}

void Mapper121_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M121PW;
	MMC3_cwrap = M121CW;
	info->Power = M121Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
