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

#include "mapinc.h"
#include "mmc3.h"

static uint8 cmd;
static uint8 prg[4];
static uint8 chr[8];

static SFORMAT StateRegs[] = {
	{ &cmd, 1, "CMD0" },
	{ prg,  4, "PREG" },
	{ chr,  8, "CREG" },
	{ 0 }
};

static void M100PRG(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, prg[3]);
}

static void M100CHR(void) {
	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);
}

static DECLFW(M100WriteCMD) {
	switch (A & 0xE001) {
	case 0x8000:
		cmd = V;
		break;
	case 0x8001:
		switch (cmd) {
		case 0x00:
			chr[0] = V & 0xFE;
			chr[1] = V | 0x01;
			break;
		case 0x01:
			chr[2] = V & 0xFE;
			chr[3] = V | 0x01;
			break;
		case 0x02:
			chr[4] = V;
			break;
		case 0x03:
			chr[5] = V;
			break;
		case 0x04:
			chr[6] = V;
			break;
		case 0x05:
			chr[7] = V;
			break;
		case 0x06:
			prg[0] = V;
			break;
		case 0x07:
			prg[1] = V;
			break;
		case 0x46:
			prg[2] = V;
			break;
		case 0x47:
			prg[1] = V;
			break;
		case 0x80:
			chr[4] = V & 0xFE;
			chr[5] = V | 0x01;
			break;
		case 0x81:
			chr[6] = V & 0xFE;
			chr[7] = V | 0x01;
			break;
		case 0x82:
			chr[0] = V;
			break;
		case 0x83:
			chr[1] = V;
			break;
		case 0x84:
			chr[2] = V;
			break;
		case 0x85:
			chr[3] = V;
			break;
		}
		MMC3_FixPRG();
		MMC3_FixCHR();
		break;
	}
}

static void M100Reset(void) {
	cmd = 0;
	prg[0] = 0;
	prg[1] = 1;
	prg[2] = ~1;
	prg[3] = ~0;
	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	chr[4] = 4;
	chr[5] = 5;
	chr[6] = 6;
	chr[7] = 7;
	MMC3_Reset();
}

static void M100Power(void) {
	cmd = 0;
	prg[0] = 0;
	prg[1] = 1;
	prg[2] = ~1;
	prg[3] = ~0;
	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	chr[4] = 4;
	chr[5] = 5;
	chr[6] = 6;
	chr[7] = 7;
	MMC3_Power();
	SetWriteHandler(0x8000, 0x9FFF, M100WriteCMD);

	if (iNESCart.trainer && MISC_ROM_SIZE) {
		if (MISC_ROM_PTR[0] == 0x4C) {
			X6502_SetNewPC(0x7000);
		}
	}
}

void Mapper100_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_FixPRG = M100PRG;
	MMC3_FixCHR = M100CHR;
	info->Power = M100Power;
	info->Reset = M100Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
