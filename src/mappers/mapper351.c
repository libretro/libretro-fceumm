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
#include "mmc1.h"
#include "mmc3.h"
#include "vrc2and4.h"

static uint8 reg[4], dipsw;

static uint8 *CHRRAM = NULL;
static uint8 *PRGCHR = NULL;

static SFORMAT stateRegs[] = {
	{ reg, 4, "REGS" },
	{ &dipsw, 1, "DIPS" },
	{ 0 },
};

static uint32 GetPRGChip(void) {
	return (((reg[2] & 0x01) && CHRRAM) ? 0x10 : 0x00);
}

static uint32 GetPRGMask(void) {
	return ((reg[2] & 0x04) ? 0x0F : 0x1F);
}

static uint32 GetPRGBase(void) {
	return (reg[1] >> 1);
}

static uint32 GetCHRMask(void) {
	if ((reg[2] & 0x10) && !(reg[2] & 0x20)) {
		return 0x1F;
	}
	return ((reg[2] & 0x20) ? 0x7F : 0xFF);
}

static uint32 GetCHRBase(void) {
	return (reg[0] << 1);
}

static void M351MMC1PW(uint16 A, uint16 V) {
	uint8 mask = GetPRGMask() >> 1;
	uint8 bank = GetPRGBase() >> 1;

	setprg16r(GetPRGChip(), A, (bank & ~mask) | (V & mask));
}

static void M351MMC1CW(uint16 A, uint16 V) {
	uint16 mask = GetCHRMask() >> 2;
	uint16 bank = GetCHRBase() >> 2;

	setchr4(A, (bank & ~mask) | (V & mask));
}

static void M351MMC3PW(uint16 A, uint16 V) {
	uint8 mask = GetPRGMask();
	uint8 bank = GetPRGBase();

	setprg8r(GetPRGChip(), A, (bank & ~mask) | (V & mask));
}

static void M351MMC3CW(uint16 A, uint16 V) {
	uint16 mask = GetCHRMask();
	uint16 bank = GetCHRBase();

	setchr1(A, (bank & ~mask) | (V & mask));
}

static void M351VRC24PW(uint16 A, uint16 V) {
	uint8 mask = GetPRGMask();
	uint8 bank = GetPRGBase();

	setprg8r(GetPRGChip(), A, (bank & ~mask) | (V & mask));
}

static void M351VRC24CW(uint16 A, uint16 V) {
	uint16 mask = GetCHRMask();
	uint16 bank = GetCHRBase();

	setchr1(A, (bank & ~mask) | (V & mask));
}

static void SyncPRG(void) {
	if (reg[2] & 0x10) { /* NROM mode */
		uint32 bank = GetPRGBase();
		uint32 chip = GetPRGChip();

		if (reg[2] & 0x08) { /* NROM-64 */
			setprg8r(chip, 0x8000, bank);
			setprg8r(chip, 0xA000, bank);
			setprg8r(chip, 0xC000, bank);
			setprg8r(chip, 0xE000, bank);
		} else {
			if (reg[2] & 0x04) { /* NROM-128 */
				setprg16r(chip, 0x8000, bank >> 1);
				setprg16r(chip, 0xC000, bank >> 1);
			} else { /* NROM-256 */
				setprg32r(chip, 0x8000, bank >> 2);
			}
		}
	} else {
		switch (reg[0] & 0x03) {
		default:
		case 1: MMC3_FixPRG(); break;
		case 2: MMC1_FixPRG(); break;
		case 3: VRC24_FixPRG(); break;
		}
	}
}

static void SyncCHR(void) {
	if (reg[2] & 0x01) { /* CHR RAM mode */
		setchr8r(0x10, 0);
	} else if (reg[2] & 0x40) { /* CNROM mode */
		setchr8(GetCHRBase() >> 3);
	} else {
		switch (reg[0] & 0x03) {
		default:
		case 1: MMC3_FixCHR(); break;
		case 2: MMC1_FixCHR(); break;
		case 3: VRC24_FixCHR(); break;
		}
	}
}

static void SyncMIR(void) {
	switch (reg[0] & 0x03) {
	default:
	case 1: MMC3_FixMIR(); break;
	case 2: MMC1_FixMIR(); break;
	case 3: VRC24_FixMIR(); break;
	}
}

static void Sync(void) {
	SyncPRG();
	SyncCHR();
	SyncMIR();
}

static void M351CPUHook(int a) {
	if ((reg[0] & 0x03) == 0x03) {
		VRC24_IRQCPUHook(a);
	}
}

static void M351HBHook(void) {
	if (!(reg[0] & 0x02)) { /* MMC3 mode */
		MMC3_IRQHBHook();
	}
}

static DECLFR(M351ReadDIP) {
	return dipsw;
}

static DECLFW(M351WriteMirr) {
	/* FDS mirroring */
	mmc3.mirr = (V >> 3) & 0x01;
	SyncMIR();
}

static DECLFW(M351WriteReg) {
	reg[A & 0x03] = V;
	Sync();
}

static DECLFW(M351Write) {
	switch (reg[0] & 0x03) {
	default:
	case 1:
		MMC3_Write(A, V);
		Sync();
		break;
	case 2:
		MMC1_Write(A, V);
		break;
	case 3:
		if (A & 0x800) {
			A = (A & 0xFFF3) | ((A << 1) & 0x08) | ((A >> 1) & 0x04);
		}
		VRC24_Write(A, V);
		break;
	}
}

static void M351Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dipsw = 0;

	VRC24_Reset();
	MMC1_Reset();
	MMC3_Reset();

	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetReadHandler(0x5000, 0x5FFF, M351ReadDIP);
	SetWriteHandler(0x4025, 0x4025, M351WriteMirr);
	SetWriteHandler(0x5000, 0x5FFF, M351WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M351Write);
}

static void M531Reset(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dipsw++;

	VRC24_Reset();
	MMC1_Reset();
	MMC3_Reset();

	FCEU_printf(" Mapper Reset! dpsw:%d\n", dipsw);
}

static void M351Close(void) {
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	if (PRGCHR) {
		FCEU_gfree(PRGCHR);
	}
	CHRRAM = NULL;
	PRGCHR = NULL;
}

static void StateRestore(int version) {
	Sync();
}

void Mapper351_Init(CartInfo *info) {
	int CHRRAMSIZE = info->CHRRamSize + info->CHRRamSaveSize;

	VRC24_Init(info, VRC2, 0x04, 0x08, FALSE, TRUE);
	VRC24_pwrap = M351VRC24PW;
	VRC24_cwrap = M351VRC24CW;

	MMC1_Init(info, FALSE, FALSE);
	MMC1_pwrap = M351MMC1PW;
	MMC1_cwrap = M351MMC1CW;

	MMC3_Init(info, FALSE, FALSE);
	MMC3_pwrap = M351MMC3PW;
	MMC3_cwrap = M351MMC3CW;

	info->Reset = M531Reset;
	info->Power = M351Power;
	info->Close = M351Close;

	MapIRQHook = M351CPUHook;
	GameHBIRQHook = M351HBHook;

	GameStateRestore = StateRestore;
	AddExState(stateRegs, ~0, 0, 0);

	if (!UNIFchrrama && CHRRAMSIZE) {
		CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");

		/* This crazy thing can map CHR-ROM into CPU address space. Allocate a combined PRG+CHR address space and treat
		 * it a second "chip". */
		PRGCHR = (uint8 *)FCEU_gmalloc(PRGsize[0] + CHRsize[0]);
		memcpy(PRGCHR, PRGptr[0], PRGsize[0]);
		memcpy(PRGCHR + PRGsize[0], CHRptr[0], CHRsize[0]);
		SetupCartPRGMapping(0x10, PRGCHR, PRGsize[0] + CHRsize[0], 0);
	}
}
