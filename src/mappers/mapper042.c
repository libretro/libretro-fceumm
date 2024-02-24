/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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
 * FDS Conversion
 * - Submapper 1 - Ai Senshi Nicol (256K PRG, 128K CHR, fixed Mirroring)
 * - Submapper 3 - Bio Miracle Bokutte Upa (J) (128K PRG, 0K CHR, IRQ)
 *
 * - Submapper 2
 * - KS-018/AC-08/LH09
 * - UNIF UNL-AC08
 * - [UNIF] Green Beret (FDS Conversion, LH09) (Unl) [U][!][t1] (160K PRG)
 * - Green Beret (FDS Conversion) (Unl) (256K PRG)
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg[2];
static uint8 IRQa;
static uint16 IRQCount;

static void (*WSync)(void);

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 2, "IRQC" },
	{ 0 }
};

/* Submapper 1 - Ai Senshi Nicol */

static void M042_Sub1_Sync(void) {
	setprg8(0x6000, reg[1] & 0x0F);
	setprg32(0x8000, ~0);
	setchr8(reg[0] & 0x0F);
}

static DECLFW(M042_Sub1_Write) {
	switch (A & 0xE000) {
	case 0x8000:
		reg[0] = V;
		WSync();
		break;
	case 0xE000:
		reg[1] = V;
		WSync();
		break;
	}
}

static void M042_Sub1_Power(void) {
	reg[1] = 0;
	reg[0] = 0;
	FDSSound_Power();
	WSync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M042_Sub1_Write);
}

/* Submapper 2 - Green Beret */

static void M042_Sub2_Sync(void) {
	setprg8(0x6000, (reg[0] >> 1) & 0x0F);
	setprg32(0x8000, (ROM.prg.size & 0x0F) ? 4 : 7);
	setchr8(0);
	setmirror(((reg[1] >> 3) & 1) ^ 1);
}

static DECLFW(M042_Sub2_Write) {
	switch (A & 0xF001) {
	case 0x4001:
	case 0x4000:
		if ((A & 0xFF) != 0x25) {
			break;
		}
		reg[1] = V;
		WSync();
		break;
	case 0x8001:
		reg[0] = V;
		WSync();
		break;
	}
}

static void M042_Sub2_Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	WSync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0xFFFF, M042_Sub2_Write);
}

/* Submapper 3 - Mario Baby */

static void M042_Sub3_Sync(void) {
	setprg8(0x6000, reg[0] & 0x0F);
	setprg32(0x8000, ~0);
	setchr8(0);
	setmirror(((reg[1] >> 3) & 1) ^ 1);
}

static DECLFW(M042_Sub3_Write) {
	switch (A & 0xE003) {
	case 0xE000:
		reg[0] = V;
		WSync();
		break;
	case 0xE001:
		reg[1] = V;
		WSync();
		break;
	case 0xE002:
		IRQa = (V & 0x02) != 0;
		if (!IRQa) {
			IRQCount = 0;
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;
	}
}

static void M042_Sub3_Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	IRQa = IRQCount = 0;
	WSync();
	FDSSound_Power();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0xE000, 0xFFFF, M042_Sub3_Write);
}

static void M042_Sub3_IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount >= 24576) {
			X6502_IRQBegin(FCEU_IQEXT);
		} else {
			X6502_IRQEnd(FCEU_IQEXT);
		}
	}
}

/* Mapper 42 Loader */
static void StateRestore(int version) {
	WSync();
}

void Mapper042_Init(CartInfo *info) {
	if (info->submapper == 0 || info-> submapper > 3) {
		if (!UNIFchrrama) {
			/* Ai Senshi Nicole, only cart with CHR-ROM, all others use CHR-RAM */
			info->submapper = 1;
		} else {
			if ((ROM.prg.size * 16) > 128) {
				/* Green Beret LH09 FDS Conversion can be 160K or 256K */
				info->submapper = 2;
			} else {
				/* Mario Baby has only 128K PRG */
				info->submapper = 3;
			}
		}
	}

	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	switch (info->submapper) {
	case 1:
		info->Power = M042_Sub1_Power;
		WSync = M042_Sub1_Sync;
		break;
	case 2:
		info->Power = M042_Sub2_Power;
		WSync = M042_Sub2_Sync;
		break;
	default:
		info->Power = M042_Sub3_Power;
		WSync = M042_Sub3_Sync;
		MapIRQHook = M042_Sub3_IRQHook;
		break;
	}
}
