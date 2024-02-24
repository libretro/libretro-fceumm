/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *	Copyright (C) 2015 Cluster
 *	http://clusterrr.com
 *	clusterrr@clusterrr.com
 *
 *  Copyright (C) 2023-2024 negativeExponent
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
    NES 2.0 mapper 327 is used for a 6-in-1 multicart. Its UNIF board name is BMC-10-24-C-A1.

	MMC3-based multicart mapper with CHR RAM, CHR ROM and PRG RAM
	
	$6000-7FFF:	A~[011xxxxx xxMRSBBB]	Multicart reg
		This register can only be written to if PRG-RAM is enabled and writable (see $A001)
		and BBB = 000 (power on state)
	
	BBB = CHR+PRG block select bits (A19, A18, A17 for both PRG and CHR)
	S = PRG block size (0=256k	 1=128k)
	R = CHR mode (0=CHR ROM	 1=CHR RAM)
	M = CHR block size (0=256k	 1=128k)
		ignored when S is 0 for some reason
		
 Example Game:
 --------------------------
 6 in 1 multicart (SMB3, TMNT2, Contra, Ninja Cat, Ninja Crusaders, Rainbow Islands 2)
*/

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSize;

static void M327PW(uint16 A, uint16 V) {
	uint8 base = (reg << 4) & 0x70;
	uint8 mask = (reg & 0x08) ? 0x1F : 0x0F;

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M327CW(uint16 A, uint16 V) {
	if (reg & 0x10) {
		setchr8r(0x10, 0);
	} else {
		uint16 base = (reg << 7) & 0x380;
		uint16 mask = (reg & 0x20) ? 0xFF : 0x7F;
		
		setchr1(A, base | (V & mask));
	}
}

static DECLFW(M327Write) {
	if (MMC3_WramIsWritable()) {
		CartBW(A, V);
		if ((reg & 7) == 0) {
			reg = A & 0x3F;
			MMC3_FixPRG();
			MMC3_FixCHR();
		}
	}
}

static void M327Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M327Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M327Write);
}

static void M327Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

void Mapper327_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M327PW;
	MMC3_cwrap = M327CW;

	info->Power = M327Power;
	info->Reset = M327Reset;
	info->Close = M327Close;
	AddExState(&reg, 1, 0, "EXPR");

	CHRRAMSize = 8192;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSize);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);
	AddExState(CHRRAM, CHRRAMSize, 0, "CHRR");
}
