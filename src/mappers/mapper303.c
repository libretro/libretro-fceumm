/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
 * NES 2.0 Mapper 303 - Kaiser 7017
 * UNIF UNL-KS7017
 * FDS Conversion - Almana No Kiseki
 *
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 latch;
static uint8 prg, mirr;
static int32 IRQa, IRQCount, IRQLatch;

static SFORMAT StateRegs[] = {
	{ &mirr, 1, "MIRR" },
	{ &prg, 1, "REGS" },
	{ &latch, 1, "LATC" },
	{ &IRQa, 4, "IRQA" },
	{ &IRQCount, 4, "IRQC" },
	{ &IRQLatch, 4, "IRQL" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, prg);
	setprg16(0xC000, 2);
	setchr8(0);
	setprg8r(0x10, 0x6000, 0);
	setmirror(mirr);
}

static DECLFW(M303Write) {
	/* FCEU_printf("bs %04x %02x\n",A,V); */
	if ((A & 0xFF00) == 0x4A00) {
		latch = ((A >> 2) & 0x03) | ((A >> 4) & 0x04);
	} else if ((A & 0xFF00) == 0x5100) {
		prg = latch;
		Sync();
	} else {
		if (A == 0x4020) {
			X6502_IRQEnd(FCEU_IQEXT);
			IRQCount &= 0xFF00;
			IRQCount |= V;
		} else if (A == 0x4021) {
			X6502_IRQEnd(FCEU_IQEXT);
			IRQCount &= 0x00FF;
			IRQCount |= V << 8;
			IRQa = 1;
		} else if (A == 0x4025) {
			mirr = ((V & 8) >> 3) ^ 0x01;
		}
	}
	if (A >= 0x4040 && A <= 0x407F) {
		FDSWaveWrite(A, V);
	}
	switch (A) {
	case 0x4080: FDSSReg0Write(A, V); break;
	case 0x4082: FDSSReg1Write(A, V); break;
	case 0x4083: FDSSReg2Write(A, V); break;
	case 0x4084: FDSSReg3Write(A, V); break;
	case 0x4085: FDSSReg4Write(A, V); break;
	case 0x4086: FDSSReg5Write(A, V); break;
	case 0x4087: FDSSReg6Write(A, V); break;
	case 0x4088: FDSSReg7Write(A, V); break;
	case 0x4089: FDSSReg8Write(A, V); break;
	case 0x408A: FDSSReg9Write(A, V); break;
	}
}

static DECLFR(FDSRead4030) {
	uint8 ret = (cpu.IRQlow & FCEU_IQEXT) ? 1 : 0;

	X6502_IRQEnd(FCEU_IQEXT);
	return ret;
}

static void UNL7017IRQ(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount <= 0) {
			IRQa = 0;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void M303Power(void) {
	latch = prg = mirr = IRQa = IRQCount = IRQLatch = 0;
	FDSSound_Power();
	Sync();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4030, 0x4030, FDSRead4030);
	SetWriteHandler(0x4020, 0x5FFF, M303Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M303Reset(void) {
	Sync();
	FDSSoundRegReset();
	FDSSound_SC();
}

static void M303Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper303_Init(CartInfo *info) {
	info->Power = M303Power;
	info->Close = M303Close;
	info->Reset = M303Reset;
	MapIRQHook = UNL7017IRQ;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
