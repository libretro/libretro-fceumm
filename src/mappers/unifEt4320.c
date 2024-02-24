/* FCE Ultra - NES/Famicom Emulator
*
* Copyright notice for this file:
*	Copyright (C) 2016 Cluster
*	http://clusterrr.com
*	clusterrr@clusterrr.com
*   Copyright (C) 2023
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA	02110-1301	USA
*/

/*
MMC3-based multicart mapper with CHR RAM, CHR ROM and PRG RAM

$6000-7FFF:	A~[011xxxxx xxMRSBBB]	Multicart reg
This register can only be written to if PRG-RAM is enabled and writable (see $A001)
and BBB = 000 (power on state)

BBB = CHR+PRG block select bits (A19, A18, A17 for both PRG and CHR)
S = PRG block size & mirroring mode (0=128k with normal MMC3, 1=256k with TxSROM-like single-screen mirroring)
R = CHR mode (0=CHR ROM	 1=CHR RAM)
M = CHR block size (0=256k	 1=128k)
ignored when S is 0 for some reason

Example Game:
--------------------------
7 in 1 multicart (Amarello, TMNT2, Contra, Ninja Cat, Ninja Crusaders, Rainbow Islands 2)
*/

/* NOTE:
 * Appears similar to Mapper 327 but with mirroring similar to Mapper 118?
 * Cannot find a cart to confirm this though
*/

#include "mapinc.h"
#include "mmc3.h"

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSize;
static uint8 reg;

static void BMC810131C_PW(uint16 A, uint16 V) {
	uint16 mask = (reg & 0x08) ? 0x1F : 0x0F;
	uint16 base = (reg << 4) & 0x70;

	setprg8(A, (base | (V & mask)));
}

static void BMC810131C_CW(uint16 A, uint16 V) {
	uint16 base = (reg << 7) & 0x380;

	if (reg & 0x10) {
		setchr1r(0x10, A, V);
	} else if ((reg & 0x20) && (reg & 0x08)) {
		setchr1(A, (base | (V & 0xFF)));
	} else {
		setchr1(A, (base | (V & 0x7F)));
	}
}

static void BMC810131C_SyncMIRR(void) {
	if (reg & 0x08) {
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

static DECLFW(BMC810131C_Write) {
	if (((mmc3.wram & 0xC0) == 0x80) && !(reg & 0x07)) {
		reg = A & 0x3F;
		MMC3_FixPRG();
		MMC3_FixCHR();
	} else {
		CartBW(A, V);
	}
}

static DECLFW(BMC810131C_WriteCMD) {
	switch (A & 0xE001) {
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

static void BMC810131C_Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void BMC810131C_Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, BMC810131C_Write);
	SetWriteHandler(0x8000, 0x9FFF, BMC810131C_WriteCMD);
}

static void BMC810131C_Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

void BMC810131C_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_FixMIR = BMC810131C_SyncMIRR;
	MMC3_pwrap = BMC810131C_PW;
	MMC3_cwrap = BMC810131C_CW;
	info->Power = BMC810131C_Power;
	info->Reset = BMC810131C_Reset;
	info->Close = BMC810131C_Close;
	AddExState(&reg, 1, 0, "EXPR");

	CHRRAMSize = 8192;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSize);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);
	AddExState(CHRRAM, CHRRAMSize, 0, "CHRR");
}
