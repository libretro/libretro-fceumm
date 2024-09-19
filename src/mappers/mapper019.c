/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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
#include "n163sound.h"

static uint8 prg[4];
static uint8 chr[8];
static uint8 nt[4];
static uint8 wram_protect;

static uint16 IRQCount;
static uint8 IRQa;

static SFORMAT N163_StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ nt, 4, "NMTA" },
	{ &IRQCount, 2, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ &wram_protect, 1, "WPRT" },

	{ 0 }
};

static void SyncPRG(void) {
	setprg8(0x8000, prg[0] & 0x3F);
	setprg8(0xa000, prg[1] & 0x3F);
	setprg8(0xc000, prg[2] & 0x3F);
	setprg8(0xe000, prg[3] & 0x3F);
}

static void DoCHRRAMROM(int x, uint8 V) {
	chr[x] = V;
	if (((prg[1] >> ((x >> 2) + 6)) & 1) || (V < 0xE0)) {
		setchr1(x << 10, V);
	}
}

static void FixCRR(void) {
	int x;
	for (x = 0; x < 8; x++) {
		DoCHRRAMROM(x, chr[x]);
	}
}

static void DoNTARAMROM(int w, uint8 V) {
	nt[w] = V;
	if (V < 0xE0) {
		V &= CHRmask1[0];
		setntamem(CHRptr[0] + (V << 10), 0, w);
	} else {
		setntamem(NTARAM + ((V & 1) << 10), 1, w);
	}
}

static void FixNTAR(void) {
	int x;
	for (x = 0; x < 4; x++) {
		DoNTARAMROM(x, nt[x]);
	}
}

static void NamcoIRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount >= 0x7FFF) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
			IRQCount = 0x7FFF;
		}
	}
}

static DECLFR(AWRAM) {
	return WRAM[A - 0x6000];
}

static DECLFW(BWRAM) {
	if (((A >= 0x6000) && (A <= 0x67FF) && ((wram_protect & 0xF1) == 0x40)) ||
	    ((A >= 0x6800) && (A <= 0x6FFF) && ((wram_protect & 0xF2) == 0x40)) ||
	    ((A >= 0x7000) && (A <= 0x77FF) && ((wram_protect & 0xF4) == 0x40)) ||
	    ((A >= 0x7800) && (A <= 0x7FFF) && ((wram_protect & 0xF8) == 0x40))) {
		WRAM[A - 0x6000] = V;
	}
}

static DECLFR(M019Read) {
	switch (A & 0xF800) {
	case 0x4800:
		return N163Sound_Read(A);
	case 0x5000:
		return IRQCount & 0xFF;
	case 0x5800:
		return IRQCount >> 8;
	}

	return 0;
}

static DECLFW(M019Write) {
	switch (A & 0xF800) {
	case 0x4800:
		N163Sound_Write(A, V);
		break;
	case 0x5000:
		IRQCount &= 0xFF00;
		IRQCount |= V;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x5800:
		IRQCount &= 0x00ff;
		IRQCount |= (V & 0x7F) << 8;
		IRQa = V & 0x80;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x8000:
	case 0x8800:
	case 0x9000:
	case 0x9800:
	case 0xA000:
	case 0xA800:
	case 0xB000:
	case 0xB800:
		DoCHRRAMROM((A - 0x8000) >> 11, V);
		break;
	case 0xC000:
	case 0xC800:
	case 0xD000:
	case 0xD800:
		DoNTARAMROM((A - 0xC000) >> 11, V);
		break;
	case 0xE000:
		prg[0] = V;
		SyncPRG();
		break;
	case 0xE800:
		prg[1] = V;
		SyncPRG();
		FixCRR();
		break;
	case 0xF000:
		prg[2] = V;
		SyncPRG();
		break;
	case 0xF800:
		wram_protect = V;
		N163Sound_Write(A, V);
		break;
	}
}

static void StateRestore(int version) {
	SyncPRG();
	FixNTAR();
	FixCRR();
}

static int battery = 0;

static void N163_Power(void) {
	prg[0] = ~3;
	prg[1] = ~2;
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

	nt[0] = 0xE0;
	nt[1] = 0xE1;
	nt[2] = 0xE0;
	nt[3] = 0xE1;

	wram_protect = 0xFF;

	SyncPRG();
	FixCRR();
	FixNTAR();

	SetReadHandler(0x4800, 0x5FFF, M019Read);
	SetWriteHandler(0x4800, 0x5FFF, M019Write);

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M019Write);

	if (WRAMSIZE) {
		SetReadHandler(0x6000, 0x7FFF, AWRAM);
		SetWriteHandler(0x6000, 0x7FFF, BWRAM);
		FCEU_CheatAddRAM(8, 0x6000, WRAM);
	}

	if (!battery) {
		FCEU_MemoryRand(WRAM, sizeof(WRAM));
		FCEU_MemoryRand(GetIRAM_ptr(), GetIRAM_size());
	}
}

void Mapper019_Init(CartInfo *info) {
	battery = info->battery;
	info->Power = N163_Power;

	MapIRQHook = NamcoIRQHook;
	GameStateRestore = StateRestore;
	AddExState(N163_StateRegs, ~0, 0, 0);

	if (info->iNES2) {
		WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	} else if (info->battery) {
		WRAMSIZE = 8192;
	}

	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}

	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = 8192;
		info->SaveGame[1] = GetIRAM_ptr();
		info->SaveGameLen[1] = GetIRAM_size();
	}

	N163Sound_ESI();
	N163Sound_AddStateInfo();
}
