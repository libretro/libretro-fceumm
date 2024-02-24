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

/* Mapper 370 - F600
 * Golden Mario Party II - Around the World (6-in-1 multicart)
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;
static uint8 dipsw;

static void M370CW(uint16 A, uint16 V) {
	uint16 mask = (reg & 0x04) ? 0xFF : 0x7F;
	uint16 base = reg << 7;

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M370PW(uint16 A, uint16 V) {
	uint16 mask = reg & 0x20 ? 0x0F : 0x1F;
	uint16 base = reg << 1;

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M370MIR(void) {
	if ((reg & 0x07) == 1) {
		if (mmc3.cmd & 0x80) {
			setntamem(NTARAM + 0x400 * ((mmc3.reg[2] >> 7) & 0x01), 1, 0);
			setntamem(NTARAM + 0x400 * ((mmc3.reg[3] >> 7) & 0x01), 1, 1);
			setntamem(NTARAM + 0x400 * ((mmc3.reg[4] >> 7) & 0x01), 1, 2);
			setntamem(NTARAM + 0x400 * ((mmc3.reg[5] >> 7) & 0x01), 1, 3);
		} else {
			setntamem(NTARAM + 0x400 * ((mmc3.reg[0] >> 7) & 0x01), 1, 0);
			setntamem(NTARAM + 0x400 * ((mmc3.reg[0] >> 7) & 0x01), 1, 1);
			setntamem(NTARAM + 0x400 * ((mmc3.reg[1] >> 7) & 0x01), 1, 2);
			setntamem(NTARAM + 0x400 * ((mmc3.reg[1] >> 7) & 0x01), 1, 3);
		}
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static DECLFR(M370Read) {
	return (((dipsw << 7) & 0x80) | (cpu.openbus & 0x7F));
}

static DECLFW(M370Write) {
	reg = (A & 0xFF);
	MMC3_FixPRG();
	MMC3_FixCHR();
	MMC3_FixMIR();
}

static DECLFW(M370WriteCMD) {
	uint8 oldcmd = mmc3.cmd;

	switch (A & 0xE001) {
	case 0x8000:
		mmc3.cmd = V;
		if ((oldcmd & 0x40) != (mmc3.cmd & 0x40)) {
			MMC3_FixPRG();
		}
		if ((oldcmd & 0x80) != (mmc3.cmd & 0x80)) {
			MMC3_FixCHR();
			MMC3_FixMIR();
		}
		break;
	case 0x8001:
		switch (mmc3.cmd & 0x07) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			mmc3.reg[mmc3.cmd & 0x07] = V;
			MMC3_FixCHR();
			MMC3_FixMIR();
			break;
		default:
			MMC3_CMDWrite(A, V);
			break;
		}
		break;
	default:
		MMC3_CMDWrite(A, V);
		break;
	}
}

static void M370Reset(void) {
	reg = 0;
	dipsw ^= 1;
	FCEU_printf("solderpad=%02x\n", dipsw);
	MMC3_Reset();
}

static void M370Power(void) {
	reg = 0;
	dipsw = 1; /* start off with the 6-in-1 menu */
	MMC3_Power();
	SetReadHandler(0x5000, 0x5FFF, M370Read);
	SetWriteHandler(0x5000, 0x5FFF, M370Write);
	SetWriteHandler(0x8000, 0x9FFF, M370WriteCMD);
}

void Mapper370_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_FixMIR = M370MIR;
	MMC3_cwrap = M370CW;
	MMC3_pwrap = M370PW;
	info->Power = M370Power;
	info->Reset = M370Reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&dipsw, 1, 0, "DPSW");
}
