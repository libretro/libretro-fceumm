/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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
 * NES 2.0 Mapper 272 is used for a bootleg implementation of
 * 悪魔城 Special: ぼくDracula君 (Akumajō Special: Boku Dracula-kun).
 *
 * as implemented from
 * https://forums.nesdev.org/viewtopic.php?f=9&t=15302&start=60#p205862
 *
 */

#include "mapinc.h"

static uint8 prg[2];
static uint8 chr[8];
static uint8 mirr;
static uint8 pal_mirr;
static uint8 IRQCount;
static uint8 IRQa;

static uint16 lastAddr;

static SFORMAT StateRegs[] = {
	{ prg, 2, "PREG" },
	{ chr, 8, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ &pal_mirr, 1, "PALM" },
	{ &IRQCount, 1, "CNTR" },
	{ &IRQa, 1, "CCLK" },
	{ &lastAddr, 1, "LADR" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg16(0xC000, ~0);

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	if (pal_mirr & 0x02) {
		setmirror(MI_0 + (pal_mirr & 0x01));
	} else {
		setmirror((mirr & 0x01) ^ 0x01);
	}
}

static DECLFW(M272Write) {
	/* writes to VRC chip */
	switch (A & 0xF000) {
	case 0x8000:
		prg[0] = V;
		break;
	case 0x9000:
		mirr = V;
		break;
	case 0xA000:
		prg[1] = V;
		break;
	case 0xB000:
	case 0xC000:
	case 0xD000:
	case 0xE000: {
		int bank = (((A - 0xB000) >> 11) & 0x06) | ((A >> 1) & 0x01);
		if (A & 0x01) {
			chr[bank] = (chr[bank] & ~0xF0) | (V << 4);
		} else {
			chr[bank] = (chr[bank] & ~0x0F) | (V & 0x0F);
		}
		break;
	}
	}

	/* writes to PAL chip */
	switch (A & 0xC00C) {
	case 0x8004:
		pal_mirr = V;
		break;
	case 0x800c:
		X6502_IRQBegin(FCEU_IQEXT);
		break;
	case 0xc004:
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xc008:
		IRQa = 1;
		break;
	case 0xc00c:
		IRQa = 0;
		IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	}

	Sync();
}

static void M272Reset(void) {
	prg[0] = prg[1] = 0;
	chr[0] = chr[1] = chr[2] = chr[3] = 0;
	chr[4] = chr[5] = chr[6] = chr[7] = 0;
	mirr = pal_mirr = 0;
	lastAddr = 0;
	IRQCount = 0;
	IRQa = 0;
	Sync();
}

static void M272Power(void) {
	M272Reset();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M272Write);
}

static void M272Hook(uint32 A) {
	if ((lastAddr & 0x2000) && !(A & 0x2000)) {
		if (IRQa) {
			IRQCount++;
			if (IRQCount == 84) {
				IRQCount = 0;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
	lastAddr = A;
}

static void StateRestore(int version) {
	Sync();
}

void Mapper272_Init(CartInfo *info) {
	info->Power = M272Power;
	info->Reset = M272Reset;
	PPU_hook = M272Hook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
