/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
 *  Copyright (C) 2003 Xodnizel
 *  Mapper 12 code Copyright (C) 2003 CaH4e3
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

/*  Code for emulating iNES mappers 4,12,44,45,47,49,52,74,114,115,116,118,
 119,165,205,245,249,250,254
*/

#include "mapinc.h"
#include "mmc3.h"

MMC3 mmc3;

static uint8 IRQCount, IRQLatch, IRQa;
static uint8 IRQReload;

static SFORMAT MMC3_StateRegs[] =
{
	{ mmc3.reg, 8, "REGS" },
	{ &mmc3.cmd, 1, "CMD" },
	{ &mmc3.mirr, 1, "A000" },
	{ &mmc3.wram, 1, "A001" },
	{ &IRQReload, 1, "IRQR" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQa, 1, "IRQA" },
	{ 0 }
};

static void GENFIXPRG(void);
static void GENFIXCHR(void);

static void GENPWRAP(uint16 A, uint16 V);
static void GENCWRAP(uint16 A, uint16 V);

int isRevB = 1;

void (*MMC3_FixPRG)(void);
void (*MMC3_FixCHR)(void);
void (*MMC3_FixMIR)(void);

void (*MMC3_pwrap)(uint16 A, uint16 V);
void (*MMC3_cwrap)(uint16 A, uint16 V);

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V & 0xFF);
}

static void GENPWRAP(uint16 A, uint16 V) {
	setprg8(A, (V & 0x7F));
}

/* ----------------------------------------------------------------------
 * ------------------------- Generic MM3 Code ---------------------------
 * ----------------------------------------------------------------------
 */

uint8 MMC3_GetPRGBank(int V) {
	if ((~V & 0x01) && (mmc3.cmd & 0x40)) {
		V ^= 0x02;
	}
	if (V & 0x02) {
		return (0xFE | (V & 0x01));
	}
	return mmc3.reg[6 | (V & 0x01)];
}

uint8 MMC3_GetCHRBank(int V) {
	if (mmc3.cmd & 0x80) {
		V ^= 0x04;
	}
	if (V & 0x04) {
		return mmc3.reg[V - 2];
	}
	return ((mmc3.reg[V >> 1] & ~0x01) | (V & 0x01));
}

int MMC3_WramIsWritable(void) {
	return ((mmc3.wram & 0x80) && !(mmc3.wram & 0x40)) ? TRUE : FALSE;
}

static void GENFIXPRG(void) {
	MMC3_pwrap(0x8000, MMC3_GetPRGBank(0));
	MMC3_pwrap(0xA000, MMC3_GetPRGBank(1));
	MMC3_pwrap(0xC000, MMC3_GetPRGBank(2));
	MMC3_pwrap(0xE000, MMC3_GetPRGBank(3));
}

static void GENFIXCHR(void) {
	MMC3_cwrap(0x0000, MMC3_GetCHRBank(0));
	MMC3_cwrap(0x0400, MMC3_GetCHRBank(1));
	MMC3_cwrap(0x0800, MMC3_GetCHRBank(2));
	MMC3_cwrap(0x0C00, MMC3_GetCHRBank(3));

	MMC3_cwrap(0x1000, MMC3_GetCHRBank(4));
	MMC3_cwrap(0x1400, MMC3_GetCHRBank(5));
	MMC3_cwrap(0x1800, MMC3_GetCHRBank(6));
	MMC3_cwrap(0x1C00, MMC3_GetCHRBank(7));
}

static void GENFIXMIR(void) {
	setmirror((mmc3.mirr & 0x01) ^ 0x01);
}

void MMC3_Reset(void) {
	IRQCount = IRQLatch = IRQa = mmc3.cmd = 0;
	mmc3.mirr = mmc3.wram = 0;

	mmc3.reg[0] = 0;
	mmc3.reg[1] = 2;
	mmc3.reg[2] = 4;
	mmc3.reg[3] = 5;
	mmc3.reg[4] = 6;
	mmc3.reg[5] = 7;
	mmc3.reg[6] = 0;
	mmc3.reg[7] = 1;

	MMC3_FixPRG();
	MMC3_FixCHR();
	MMC3_FixMIR();
}

DECLFW(MMC3_CMDWrite) {
	uint8 oldcmd = mmc3.cmd;
	/*	FCEU_printf("bs %04x %02x\n",A,V); */
	switch (A & 0xE001) {
	case 0x8000:
		mmc3.cmd = V;
		if ((oldcmd & 0x40) != (mmc3.cmd & 0x40)) {
			MMC3_FixPRG();
		}
		if ((oldcmd & 0x80) != (mmc3.cmd & 0x80)) {
			MMC3_FixCHR();
		}
		break;
	case 0x8001: {
		int cbase = (mmc3.cmd & 0x80) << 5;
		mmc3.reg[mmc3.cmd & 0x7] = V;
		switch (mmc3.cmd & 0x07) {
		case 0:
			MMC3_cwrap((cbase ^ 0x000), V & (~1));
			MMC3_cwrap((cbase ^ 0x400), V | 1);
			break;
		case 1:
			MMC3_cwrap((cbase ^ 0x800), V & (~1));
			MMC3_cwrap((cbase ^ 0xC00), V | 1);
			break;
		case 2:
			MMC3_cwrap(cbase ^ 0x1000, V);
			break;
		case 3:
			MMC3_cwrap(cbase ^ 0x1400, V);
			break;
		case 4:
			MMC3_cwrap(cbase ^ 0x1800, V);
			break;
		case 5:
			MMC3_cwrap(cbase ^ 0x1C00, V);
			break;
		case 6:
			if (mmc3.cmd & 0x40) {
				MMC3_pwrap(0xC000, V);
			} else {
				MMC3_pwrap(0x8000, V);
			}
			break;
		case 7:
			MMC3_pwrap(0xA000, V);
			break;
		}
		break;
	}
	case 0xA000:
		mmc3.mirr = V;
		MMC3_FixMIR();
		break;
	case 0xA001:
		mmc3.wram = V;
		break;
	}
}

DECLFW(MMC3_IRQWrite) {
	/*	FCEU_printf("%04x:%04x\n",A,V); */
	switch (A & 0xE001) {
	case 0xC000:
		IRQLatch = V;
		break;
	case 0xC001:
		IRQCount = 0;
		IRQReload = 1;
		break;
	case 0xE000:
		X6502_IRQEnd(FCEU_IQEXT);
		IRQa = 0;
		break;
	case 0xE001:
		IRQa = 1;
		break;
	}
}

DECLFW(MMC3_Write) {
	/*	FCEU_printf("bs %04x %02x\n",A,V); */
	switch (A & 0xE000) {
	case 0x8000:
	case 0xA000:
		MMC3_CMDWrite(A, V);
		break;
	case 0xC000:
	case 0xE000:
		MMC3_IRQWrite(A, V);
		break;
	}
}

void MMC3_IRQHBHook(void) {
	int count = IRQCount;
	if (!count || IRQReload) {
		IRQCount = IRQLatch;
		IRQReload = 0;
	} else {
		IRQCount--;
	}
	if ((count | isRevB) && !IRQCount && IRQa) {
		X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void MMC3_hb(void) {
	MMC3_IRQHBHook();
}

static void MMC3_hb_KickMasterHack(void) {
	if (scanline == 238) {
		MMC3_IRQHBHook();
	}
	MMC3_IRQHBHook();
}

static void MMC3_hb_PALStarWarsHack(void) {
	if (scanline == 240) {
		MMC3_IRQHBHook();
	}
	MMC3_IRQHBHook();
}

static void StateRestore(int version) {
	MMC3_FixPRG();
	MMC3_FixCHR();
	MMC3_FixMIR();
}

void MMC3_Power(void) {
	if (UNIFchrrama) {
		setchr8(0);
	}

	SetWriteHandler(0x8000, 0xBFFF, MMC3_CMDWrite);
	SetWriteHandler(0xC000, 0xFFFF, MMC3_IRQWrite);
	SetReadHandler(0x8000, 0xFFFF, CartBR);

	setmirror(1);
	if (mmc3.opts & 1) {
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
		SetWriteHandler(0x6000, 0x6000 + ((WRAMSIZE - 1) & 0x1fff), CartBW);
		SetReadHandler(0x6000, 0x6000 + ((WRAMSIZE - 1) & 0x1fff), CartBR);
		setprg8r(0x10, 0x6000, 0);
		if (!(mmc3.opts & 2)) {
			FCEU_MemoryRand(WRAM, WRAMSIZE);
		}
	}
	MMC3_Reset();
}

void MMC3_Close(void) {
}

void MMC3_Init(CartInfo *info, int wram, int battery) {
	MMC3_FixPRG = GENFIXPRG;
	MMC3_FixCHR = GENFIXCHR;
	MMC3_FixMIR = GENFIXMIR;

	MMC3_pwrap = GENPWRAP;
	MMC3_cwrap = GENCWRAP;

	WRAMSIZE = wram << 10;

	if (wram) {
		mmc3.opts |= 1;
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}

	if (battery) {
		mmc3.opts |= 2;
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}

	AddExState(MMC3_StateRegs, ~0, 0, 0);

	info->Power = MMC3_Power;
	info->Reset = MMC3_Reset;
	info->Close = MMC3_Close;

	if (info->CRC32 == 0x5104833e) { /* Kick Master */
		GameHBIRQHook = MMC3_hb_KickMasterHack;
	} else if (info->CRC32 == 0x5a6860f1 || info->CRC32 == 0xae280e20) { /* Shougi Meikan '92/'93 */
		GameHBIRQHook = MMC3_hb_KickMasterHack;
	} else if (info->CRC32 == 0xfcd772eb) { /* PAL Star Wars, similar problem as Kick Master. */
		GameHBIRQHook = MMC3_hb_PALStarWarsHack;
	} else {
		GameHBIRQHook = MMC3_hb;
	}
	GameStateRestore = StateRestore;
}
