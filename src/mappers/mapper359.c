/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 359 - BMC-SB-5013
 * NES 2.0 Mapper 540 - UNL-82112C
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 prg[4];
static uint8 chr[8];
static uint8 reg[4];

static uint8 IRQReload;
static uint8 IRQa;
static uint8 irqPA12;
static uint8 IRQAutoEnable;
static uint8 IRQLatch;
static uint8 IRQCount;
static int16 IRQCount16;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ reg, 4, "EXPR" },
	{ &IRQReload, 1, "IRQL" },
	{ &IRQa, 1, "IRQa" },
	{ &irqPA12, 1, "IRQp" },
	{ &IRQAutoEnable, 1, "IRQe" },
	{ &IRQLatch, 1, "IRQl" },
	{ &IRQCount, 1, "IRQ8" },
	{ &IRQCount16, 2, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	uint16 prgMask = 0x3F;
	uint16 prgBase = (reg[0] & 0x38) << 1;

	switch (reg[1] & 0x03) {
	case 0: prgMask = 0x3F; break;
	case 1: prgMask = 0x1F; break;
	case 2: prgMask = 0x2F; break;
	case 3: prgMask = 0x0F; break;
	}

	setprg8(0x6000, prgBase | (prg[3] & prgMask));
	setprg8(0x8000, prgBase | (prg[0] & prgMask));
	setprg8(0xA000, prgBase | (prg[1] & prgMask));
	setprg8(0xC000, prgBase | (prg[2] & prgMask));
	setprg8(0xE000, prgBase | (    ~0 & prgMask));

	if (UNIFchrrama) {
		setchr8(0);
	} else {
		if (iNESCart.mapper == 540) {
			setchr2(0x0000, chr[0]);
			setchr2(0x0800, chr[1]);
			setchr2(0x1000, chr[6]);
			setchr2(0x1800, chr[7]);
		} else {
			uint16 chrMask = (reg[1] & 0x40) ? 0xFF : 0x7F;
			uint16 chrBase = (reg[3] << 7);

			setchr1(0x0000, chrBase | (chr[0] & chrMask));
			setchr1(0x0400, chrBase | (chr[1] & chrMask));
			setchr1(0x0800, chrBase | (chr[2] & chrMask));
			setchr1(0x0C00, chrBase | (chr[3] & chrMask));
			setchr1(0x1000, chrBase | (chr[4] & chrMask));
			setchr1(0x1400, chrBase | (chr[5] & chrMask));
			setchr1(0x1800, chrBase | (chr[6] & chrMask));
			setchr1(0x1C00, chrBase | (chr[7] & chrMask));
		}
	}

	switch (reg[2] & 0x03) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFW(M359Write) {
	switch (A & 0xF000) {
	case 0x8000:
		prg[A & 0x03] = V;
		Sync();
		break;
	case 0x9000:
		reg[A & 0x03] = V;
		Sync();
		break;
	case 0xA000:
	case 0xB000:
		chr[((A >> 10) & 0x04) | (A & 0x03)] = V;
		Sync();
		break;
	case 0xC000:
		switch (A & 0x03) {
		case 0:
			if (IRQAutoEnable) {
				IRQa = FALSE;
			}
			IRQCount16 &= 0xFF00;
			IRQCount16 |= V;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 1:
			if (IRQAutoEnable) {
				IRQa = TRUE;
			}
			IRQCount16 &= 0x00FF;
			IRQCount16 |= (V << 8);
			IRQReload = TRUE;
			IRQLatch = V;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 2:
			IRQa = (V & 0x01);
			irqPA12 = (V & 0x02) >> 1;
			IRQAutoEnable = (V & 0x04) >> 2;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 3:
			IRQa = (V & 0x01);
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		}
	}
}

static void M359Power(void) {
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
	reg[0] = 0;
	reg[1] = 0x40;
	reg[2] = 0;
	reg[3] = 0;
	IRQReload = IRQa = irqPA12 = IRQAutoEnable = 0;
	IRQLatch = IRQCount = IRQCount16 = 0;
	Sync();
	FDSSound_Power();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xCFFF, M359Write);
}

static void M359Reset(void) {
	Sync();
	FDSSoundRegReset();
	FDSSound_SC();
}

static void M359CPUHook(int a) {
	if (!irqPA12) {
		if (IRQa && IRQCount16) {
			IRQCount16 -= a;
			if (IRQCount16 <= 0)
				X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void M359IRQHook(void) {
	if (irqPA12) {
		if (!IRQCount || IRQReload) {
			IRQCount = IRQLatch;
		} else {
			IRQCount--;
		}
		if (!IRQCount && IRQa) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
		IRQReload = FALSE;
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper359_Init(CartInfo *info) {
	info->Power = M359Power;
	info->Reset = M359Reset;
	MapIRQHook = M359CPUHook;
	GameHBIRQHook = M359IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}

void Mapper540_Init(CartInfo *info) {
	info->Power = M359Power;
	MapIRQHook = M359CPUHook;
	GameHBIRQHook = M359IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
