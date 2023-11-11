/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2020
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

/* added 2020-2-15
 * Street Fighter 3, Mortal Kombat II, Dragon Ball Z 2, Mario & Sonic 2 (JY-016)
 * 1995 Super HIK 4-in-1 (JY-016), 1995 Super HiK 4-in-1 (JY-017)
 * submapper 1 - Super Fighter III
 * NOTE: nesdev's notes for IRQ is different that whats implemented here
 */

#include "mapinc.h"

static uint8 chr[4], prg[2];
static uint8 outerbank;
static uint8 mirr;

static uint8 IRQa;
static uint8 IRQPrescaler;
static uint8 IRQCount;
static int16 IRQCount16;

static SFORMAT StateRegs[] = {
	{ chr, 4, "CREG" },
	{ prg, 2, "PREG" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQPrescaler, 1, "IRQP" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQCount16, 4, "IRQ2" },
	{ &outerbank, 1, "OUTB" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	/*	FCEU_printf("P0:%02x P1:%02x outerbank:%02x\n", prg[0], prg[1], outerbank);*/
	setprg8(0x8000, ((outerbank << 3) & ~0x0F) | prg[0]);
	setprg8(0xa000, ((outerbank << 3) & ~0x0F) | prg[1]);
	setprg8(0xc000, ((outerbank << 3) & ~0x0F) | 0x0E);
	setprg8(0xe000, ((outerbank << 3) & ~0x0F) | 0x0F);

	setchr2(0x0000, ((outerbank << 8) & 0x100) | chr[0]);
	setchr2(0x0800, ((outerbank << 8) & 0x100) | chr[1]);
	setchr2(0x1000, ((outerbank << 8) & 0x100) | chr[2]);
	setchr2(0x1800, ((outerbank << 8) & 0x100) | chr[3]);

	if (iNESCart.submapper != 0) {
		setmirror((mirr & 0x01) ^ 0x01);
	}
}

static DECLFW(M091CHRWrite) {
	if (iNESCart.submapper == 1) {
		switch (A & 0x07) {
		case 0:
		case 1:
		case 2:
		case 3:
			chr[A & 0x03] = V;
			Sync();
			break;
		case 4:
		case 5:
			mirr = V;
			Sync();
			break;
		case 6:
			IRQCount16 = (IRQCount16 & 0xFF00) | V;
			break;
		case 7:
			IRQCount16 = (IRQCount16 & 0x00FF) | (V << 8);
			break;
		}
	} else {
		chr[A & 0x03] = V;
		Sync();
	}
}

static DECLFW(M091IRQWrite) {
	switch (A & 0x03) {
	case 0:
	case 1:
		prg[A & 0x01] = V;
		Sync();
		break;
	case 2:
		IRQa = IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 3:
		IRQa = 1;
		IRQPrescaler = 3;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	}
}

static DECLFW(M091OuterBankWrite) {
	outerbank = A & 0xFF;
	Sync();
}

static void M091Power(void) {
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x6FFF, M091CHRWrite);
	SetWriteHandler(0x7000, 0x7FFF, M091IRQWrite);
	SetWriteHandler(0x8000, 0x9FFF, M091OuterBankWrite);
}

static void M091HBHook(void) {
	if ((IRQCount < 8) && IRQa) {
		IRQCount++;
		if (IRQCount >= 8) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void M091IRQHook(int a) {
	IRQPrescaler += a;
	if (IRQPrescaler >= 4) {
		IRQPrescaler -= 4;
		IRQCount16 -= 5;
		if ((IRQCount16 <= 0) && IRQa) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper091_Init(CartInfo *info) {
	info->Power = M091Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
	if (info->submapper == 1) {
		MapIRQHook = M091IRQHook;
	} else {
		GameHBIRQHook = M091HBHook;
	}
}
