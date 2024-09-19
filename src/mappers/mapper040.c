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
 *
 */

#include "mapinc.h"

static uint8 reg[2];
static uint32 IRQCount, IRQa;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &IRQCount, 4, "IRQC" },
	{ &IRQa, 4, "IRQA" },
	{ 0 }
};

static void Sync(void) {
	if (reg[1] & 0x08) {
		if (reg[1] & 0x10)
			setprg32(0x8000, 2 | (reg[1] >> 6));
		else {
			setprg16(0x8000, 4 | (reg[1] >> 5));
			setprg16(0xC000, 4 | (reg[1] >> 5));
		}
	} else {
		setprg8(0x6000, 6);
		setprg8(0x8000, 4);
		setprg8(0xA000, 5);
		setprg8(0xC000, reg[0] & 0x07);
		setprg8(0xE000, 7);
	}
	setchr8((reg[1] >> 1) & 0x03);
	setmirror((reg[1] & 0x01) ^ 0x01);
}

static DECLFW(M040Write) {
	switch (A & 0xE000) {
	case 0x8000:
		IRQa = FALSE;
		IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xA000:
		IRQa = TRUE;
		break;
	case 0xC000:
		if (iNESCart.submapper == 1) {
			reg[1] = A & 0xFF;
			Sync();
		}
		break;
	case 0xE000:
		reg[0] = V;
		Sync();
		break;
	}
}

static void M040Power(void) {
	reg[0] =  reg[1] = 0;
	IRQCount = IRQa = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M040Write);
}

static void M040Reset(void) {
	reg[0] = 0;
	reg[1] = 0;
	IRQCount = IRQa = 0;
	Sync();
}

static void M040IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount & 0x1000) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper040_Init(CartInfo *info) {
	info->Reset = M040Reset;
	info->Power = M040Power;
	MapIRQHook = M040IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
