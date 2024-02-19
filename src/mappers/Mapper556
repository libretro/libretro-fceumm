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

/* NES 2.0 Mapper 556
 * Used for the for the 超强小新2+瑪莉家族 7-in-1 (JY-215) multicart.
 */

#include "mapinc.h"
#include "mmc3.h"
#include "vrc2and4.h"

static uint8 cmd;
static uint8 reg[4];

static SFORMAT StateRegs[] = {
	{ reg, 5, "REGS" },
	{ &cmd, 1, "CMD0" },
	{ 0 },
};

static uint32 M556PRGMask(void) {
	return (~reg[3] & 0x3F);
}

static uint32 M556PRGBase(void) {
	return (((reg[3] & 0x40) << 2) | reg[1]);
}

static uint32 M556CHRMask(void) {
	return (0xFF >> (~reg[2] & 0x0F));
}

static uint32 M556CHRBase(void) {
	return (((reg[3] & 0x40) << 6) | ((reg[2] & 0xF0) << 4) | reg[0]);
}

static void M556MMC3PW(uint16 A, uint16 V) {
	uint32 mask = M556PRGMask();
	uint32 base = M556PRGBase();

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M556MMC3CW(uint16 A, uint16 V) {
	uint32 mask = M556CHRMask();
	uint32 base = M556CHRBase();

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M556VRC4PW(uint16 A, uint16 V) {
	uint32 mask = M556PRGMask();
	uint32 base = M556PRGBase();

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M556VRC4CW(uint16 A, uint16 V) {
	uint32 mask = M556CHRMask();
	uint32 base = M556CHRBase();

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M556Sync(void) {
	if (reg[2] & 0x80) {
		VRC24_FixPRG();
		VRC24_FixCHR();
		VRC24_FixMIR();
	} else {
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	}
}

static DECLFW(M556WriteREG) {
	if (!(reg[3] & 0x80)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 0x03;
		M556Sync();		
	}
}

static DECLFW(M556WriteASIC) {
	if (reg[2] & 0x80) {
		VRC24_Write(A, V);
	} else {
		MMC3_Write(A, V);
	}
}

static void M556CPUIRQHook(int a) {
	if (reg[2] & 0x80) {
		VRC24_IRQCPUHook(a);
	}
}

static void M556HBIRQHook(void) {
	if (!(reg[2] & 0x80)) {
		MMC3_IRQHBHook();
	}
}

static void M556Reset(void) {
	memset(reg, 0, sizeof(reg));
	reg[2] = 0x0F;
	M556Sync();
}

static void M556Power(void) {
	memset(reg, 0, sizeof(reg));
	reg[2] = 0x0F;

	MMC3_Reset();
	VRC24_Reset();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, M556WriteREG);
	SetWriteHandler(0x8000, 0xFFFF, M556WriteASIC);

	if (WRAM) {
		setprg8r(0x10, 0x6000, 0);
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
	}
}

static void M556Close(void) {
}

static void StateRestore(int version) {
	M556Sync();
}

void Mapper556_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x05, 0x0A, 0, TRUE);
	VRC24_pwrap = M556VRC4PW;
	VRC24_cwrap = M556VRC4CW;

	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M556MMC3PW;
	MMC3_cwrap = M556MMC3CW;

	info->Reset = M556Reset;
	info->Power = M556Power;
	info->Close = M556Close;
	MapIRQHook = M556CPUIRQHook;
	GameHBIRQHook = M556HBIRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		if (info->battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
	}
}
