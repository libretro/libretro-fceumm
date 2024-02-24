/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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
 * YOKO mapper, almost the same as 83, TODO: figure out difference
 * Mapper 83 - 30-in-1 mapper, two modes for single game carts, one mode for
 * multigame Dragon Ball Z Party
 *
 * Mortal Kombat 2 YOKO
 * N-CXX(M), XX -  PRG+CHR, 12 - 128+256, 22 - 256+256, 14 - 128+512
 *
 */

/*
 * iNES Mapper 083 also knows as Yoko
 * 256 KiB CHR-ROM => Submapper 0 (1 KiB CHR-ROM banking, no WRAM)
 * - Street Fighter II Pro/Street Blaster II Pro
 * - Street Fighter IV Pro 10/Street Blaster IV Pro 10
 * - Street Blaster V Turbo 20
 * - Street Fighter X Turbo 40
 * - Fatal Fury 2/餓狼伝説 2
 * - Fatal Fury 2'/餓狼伝説 2'
 * 512 KiB CHR-ROM => Submapper 1 (2 KiB CHR-ROM banking, no WRAM)
 * - Super Blaster VII Turbo 28
 * - World Heroes 2/快打英雄榜 2
 * - World Heroes 2 Pro//快打英雄榜 2 Pro
 * 1024 KiB CHR-ROM => Submapper 2 (1 KiB CHR-ROM banking with outer bank, 32 KiB banked WRAM)
 * - Dragon Ball Party
 *
 * NES 2.0 264 - UNL-Yoko
 * - Mortal Kombat II/V Pro
 * - Master Fighter VI'
*/

#include "mapinc.h"

static uint8 prg[4], chr[8], low[4];
static uint8 mode, bank, dip;
static uint8 IRQa;
static int32 IRQCount;

static uint8 prgMask;
static uint8 chrMode;
static uint16 dipMask;

static SFORMAT StateRegs[] =
{
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ &mode, 1, "MODE" },
	{ &bank, 1, "BANK" },
	{ &IRQCount, 4, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ low, 4, "LOWR" },
	{ 0 }
};

static void M083Sync(void) {
	uint8 prgMode = (mode >> 3) & 0x03;
	uint8 mirr = mode & 0x03;

	if (WRAMSIZE) {
		setprg8r(0x10, 0x6000, (bank >> 6));
	} else if (mode & 0x20) {
		setprg8(0x6000, prg[3]);
	}
	switch (prgMode) {
	case 0:
		setprg16(0x8000, bank);
		setprg16(0xC000, bank | (prgMask >> 1));
		break;
	case 1:
		setprg32(0x8000, bank >> 1);
		break;
	case 2:
	case 3:
		setprg8(0x8000, ((bank << 1) & ~prgMask) | (prg[0] & prgMask));
		setprg8(0xA000, ((bank << 1) & ~prgMask) | (prg[1] & prgMask));
		setprg8(0xC000, ((bank << 1) & ~prgMask) | (prg[2] & prgMask));
		setprg8(0xE000, ((bank << 1) & ~prgMask) | (~0 & prgMask));
		break;
	}
	switch (chrMode) {
	case 0:
		setchr1(0x0000, chr[0]);
		setchr1(0x0400, chr[1]);
		setchr1(0x0800, chr[2]);
		setchr1(0x0C00, chr[3]);
		setchr1(0x1000, chr[4]);
		setchr1(0x1400, chr[5]);
		setchr1(0x1800, chr[6]);
		setchr1(0x1C00, chr[7]);
		break;
	case 1:
		setchr2(0x0000, chr[0]);
		setchr2(0x0800, chr[1]);
		setchr2(0x1000, chr[6]);
		setchr2(0x1800, chr[7]);
		break;
	case 2:
		setchr1(0x0000, ((bank << 4) & 0x300) | chr[0]);
		setchr1(0x0400, ((bank << 4) & 0x300) | chr[1]);
		setchr1(0x0800, ((bank << 4) & 0x300) | chr[2]);
		setchr1(0x0C00, ((bank << 4) & 0x300) | chr[3]);
		setchr1(0x1000, ((bank << 4) & 0x300) | chr[4]);
		setchr1(0x1400, ((bank << 4) & 0x300) | chr[5]);
		setchr1(0x1800, ((bank << 4) & 0x300) | chr[6]);
		setchr1(0x1C00, ((bank << 4) & 0x300) | chr[7]);
		break;
	}
	switch (mirr) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFW(M083Write) {
	if (iNESCart.mapper == 264) {
		A = ((A >> 2) & 0x3C0) | (A & 0x3F);
	}
	switch (A & 0x300) {
	case 0x000:
		bank = V;
		break;
	case 0x100:
		mode = V;
		break;
	case 0x200:
		if (A & 0x01) {
			IRQa = mode & 0x80;
			IRQCount &= 0xFF;
			IRQCount |= V << 8;
		} else {
			IRQCount &= 0xFF00;
			IRQCount |= V;
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;
	case 0x300:
		A &= 0x1F;
		if (A < 0x10) {
			prg[A & 0x03] = V;
		} else if (A < 0x18) {
			chr[A & 0x07] = V;
		}
		break;
	}
	M083Sync();
}

static DECLFR(M083ReadLow) {
	if (A & dipMask) {
		return low[A & 3];
	}
	return dip;
}

static DECLFW(M083WriteLow) {
	low[A & 3] = V;
}

static void M083Power(void) {
	mode = 0x10;
	bank = 0;
	dip = (iNESCart.mapper == 264) ? 0x01 : 0x00;
	M083Sync();
	SetReadHandler(0x5000, 0x5FFF, M083ReadLow);
	SetWriteHandler(0x5000, 0x5FFF, M083WriteLow);
	SetReadHandler(0x6000, 0x7fff, CartBR);
	SetReadHandler(0x8000, 0xffff, CartBR);
	SetWriteHandler(0x8000, 0xffff, M083Write);
	if (WRAMSIZE) {
		SetWriteHandler(0x6000, 0x7fff, CartBW);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);

	}
}

static void M083Reset(void) {
	dip++;
	if (iNESCart.mapper == 264) {
		dip &= 3;
	} else {
		dip &= 1;
	}
	mode = bank = 0;
	M083Sync();
}

static void M083Close(void) {
}

static void M083IRQHook(int a) {
	if (IRQa && (IRQCount > 0)) {
		if (mode & 0x40) {
			IRQCount -= a;
		} else {
			IRQCount += a;
		}
		if (IRQCount <= 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
		}
	}
}

static void M083StateRestore(int version) {
	M083Sync();
}

void Mapper083_Init(CartInfo *info) {
	info->Power = M083Power;
	info->Reset = M083Reset;
	info->Close = M083Close;
	MapIRQHook = M083IRQHook;
	GameStateRestore = M083StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	if (!info->iNES2) {
		if (info->CHRRomSize >= (1024 * 1024)) {
			info->submapper = 2;
		} else if (info->CHRRomSize >= (512 * 1024)) {
			info->submapper = 1;
		}
	}

	chrMode = info->submapper;
	prgMask = 0x1F;
	dipMask = 0x100;

	WRAMSIZE = (info->submapper == 2) ? 32768 : 0;
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}
}

void Mapper264_Init(CartInfo *info) {
	info->Power = M083Power;
	info->Reset = M083Reset;
	info->Close = M083Close;
	MapIRQHook = M083IRQHook;
	GameStateRestore = M083StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	chrMode = 1;
	prgMask = 0x0F;
	dipMask = 0x400;
}
