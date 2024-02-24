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
#include "mmc6.h"

typedef struct __MMC6 {
	uint8 cmd;
	uint8 opts;
	uint8 mirr;
	uint8 wram;
	uint8 reg[8];
} MMC6;

static MMC6 mmc6;

static uint8 IRQCount, IRQLatch, IRQa;
static uint8 IRQReload;

static SFORMAT MMC6_StateRegs[] =
{
	{ mmc6.reg, 8, "REGS" },
	{ &mmc6.cmd, 1, "CMD" },
	{ &mmc6.mirr, 1, "A000" },
	{ &mmc6.wram, 1, "A001" },
	{ &IRQReload, 1, "IRQR" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQa, 1, "IRQA" },
	{ 0 }
};

static void MMC6_FixPRG(void) {
	if (!(mmc6.cmd & 0x40)) {
		setprg8(0x8000, mmc6.reg[6]);
		setprg8(0xA000, mmc6.reg[7]);
		setprg8(0xC000, ~1);
		setprg8(0xE000, ~0);
	} else {
		setprg8(0x8000, ~1);
		setprg8(0xA000, mmc6.reg[7]);
		setprg8(0xC000, mmc6.reg[6]);
		setprg8(0xE000, ~0);
	}
}

static void MMC6_FixCHR(void) {
	if (!(mmc6.cmd & 0x80)) {
		setchr1(0x0000, mmc6.reg[0] & 0xFE);
		setchr1(0x0400, mmc6.reg[0] | 0x01);
		setchr1(0x0800, mmc6.reg[1] & 0xFE);
		setchr1(0x0C00, mmc6.reg[1] | 0x01);

		setchr1(0x1000, mmc6.reg[2]);
		setchr1(0x1400, mmc6.reg[3]);
		setchr1(0x1800, mmc6.reg[4]);
		setchr1(0x1C00, mmc6.reg[5]);
	} else {
		setchr1(0x0000, mmc6.reg[2]);
		setchr1(0x0400, mmc6.reg[3]);
		setchr1(0x0800, mmc6.reg[4]);
		setchr1(0x0C00, mmc6.reg[5]);

		setchr1(0x1000, mmc6.reg[0] & 0xFE);
		setchr1(0x1400, mmc6.reg[0] | 0x01);
		setchr1(0x1800, mmc6.reg[1] & 0xFE);
		setchr1(0x1C00, mmc6.reg[1] | 0x01);
	}
}

static void MMC6_FixMIR(void) {
	setmirror((mmc6.mirr & 0x01) ^ 0x01);
}

static void MMC6_Reset(void) {
	IRQCount = IRQLatch = IRQa = mmc6.cmd = 0;
	mmc6.mirr = mmc6.wram = 0;

	mmc6.reg[0] = 0;
	mmc6.reg[1] = 2;
	mmc6.reg[2] = 4;
	mmc6.reg[3] = 5;
	mmc6.reg[4] = 6;
	mmc6.reg[5] = 7;
	mmc6.reg[6] = 0;
	mmc6.reg[7] = 1;

	MMC6_FixPRG();
	MMC6_FixCHR();
	MMC6_FixMIR();
}


static DECLFW(MBWRAMMMC6) {
	if (!(mmc6.cmd & 0x20)) {
		/* wram disabled */
		return;
	}

	A &= 0x3FF;
	if (!(A & 0x200)) {
		/* 1st bank writes are disabled */
		if ((mmc6.wram & 0x30) == 0x30) WRAM[A] = V;
	} else {
		/* 2nd bank writes are disabled */
		if ((mmc6.wram & 0xC0) == 0xC0) WRAM[A] = V;
	}
}

static DECLFR(MAWRAMMMC6) {
	if (!(mmc6.cmd & 0x20)) {
		/* wram disabled */
		return cpu.openbus;
	}

	A &= 0x3FF;
	if (!(A & 0x200)) {
		/* 1st bank */
		if (mmc6.wram & 0x20) return WRAM[A];
		if (mmc6.wram & 0x80) return 0x00;
	} else {
		/* 2nd bank */
		if (mmc6.wram & 0x80) return WRAM[A];
		if (mmc6.wram & 0x20) return 0x00;
	}
	return cpu.openbus;
}

static DECLFW(MMC6_CMDWrite) {
	uint8 oldcmd = mmc6.cmd;
	/*	FCEU_printf("bs %04x %02x\n",A,V); */
	switch (A & 0xE001) {
	case 0x8000:
		mmc6.cmd = V;
		if ((oldcmd & 0x40) != (mmc6.cmd & 0x40))
			MMC6_FixPRG();
		if ((oldcmd & 0x80) != (mmc6.cmd & 0x80))
			MMC6_FixCHR();
		break;
	case 0x8001: {
		int cbase = (mmc6.cmd & 0x80) << 5;
		mmc6.reg[mmc6.cmd & 0x7] = V;
		switch (mmc6.cmd & 0x07) {
		case 0:
			setchr1((cbase ^ 0x000), V & (~1));
			setchr1((cbase ^ 0x400), V | 1);
			break;
		case 1:
			setchr1((cbase ^ 0x800), V & (~1));
			setchr1((cbase ^ 0xC00), V | 1);
			break;
		case 2:
			setchr1(cbase ^ 0x1000, V);
			break;
		case 3:
			setchr1(cbase ^ 0x1400, V);
			break;
		case 4:
			setchr1(cbase ^ 0x1800, V);
			break;
		case 5:
			setchr1(cbase ^ 0x1C00, V);
			break;
		case 6:
			if (mmc6.cmd & 0x40)
				setprg8(0xC000, V);
			else
				setprg8(0x8000, V);
			break;
		case 7:
			setprg8(0xA000, V);
			break;
		}
		break;
	}
	case 0xA000:
		mmc6.mirr = V;
		MMC6_FixMIR();
		break;
	case 0xA001:
		mmc6.wram = V;
		break;
	}
}

static DECLFW(MMC6_IRQWrite) {
	/*	FCEU_printf("%04x:%04x\n",A,V); */
	switch (A & 0xE001) {
	case 0xC000:
		IRQLatch = V;
		break;
	case 0xC001:
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

static void MMC6_IRQHBHook(void) {
	int count = IRQCount;

	if (!count || IRQReload) {
		IRQCount = IRQLatch;
		IRQReload = 0;
	} else
		IRQCount--;
	if (!IRQCount && IRQa) {
		X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void StateRestore(int version) {
	MMC6_FixPRG();
	MMC6_FixCHR();
	MMC6_FixMIR();
}

static void MMC6_Power(void) {
	if (UNIFchrrama) {
		setchr8(0);
	}

	SetWriteHandler(0x8000, 0xBFFF, MMC6_CMDWrite);
	SetWriteHandler(0xC000, 0xFFFF, MMC6_IRQWrite);
	SetReadHandler(0x8000, 0xFFFF, CartBR);

	setmirror(1);

	FCEU_CheatAddRAM(1, 0x7000, WRAM);
	SetReadHandler(0x7000, 0x7FFF, MAWRAMMMC6);
	SetWriteHandler(0x7000, 0x7FFF, MBWRAMMMC6);
	if (!(mmc6.opts & 2)) {
		FCEU_MemoryRand(WRAM, WRAMSIZE);
	}

	MMC6_Reset();
}

static void MMC6_Close(void) {
}

void MMC6_Init(CartInfo *info) {
	WRAMSIZE = 1024;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	if (info->battery) {
		mmc6.opts |= 2;
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}

	AddExState(MMC6_StateRegs, ~0, 0, 0);

	info->Power = MMC6_Power;
	info->Reset = MMC6_Reset;
	info->Close = MMC6_Close;

	GameHBIRQHook = MMC6_IRQHBHook;
	GameStateRestore = StateRestore;
}
