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
 */

#include "mapinc.h"

static uint8 prg[4];
static uint8 chr[8];
static uint8 nt[4];
static uint8 IRQa;
static uint16 IRQCount;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
    { chr, 8, "CREG" },
    { nt, 4, "NREG" },
    { &IRQa, 1, "IRQA" },
    { &IRQCount, 2, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, ~0);

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	setmirrorw(nt[0] & 0x01, nt[1] & 0x01, nt[2] & 0x01, nt[3] & 0x01);
}

static DECLFW(M417Write) {
	switch ((A >> 4) & 0x07) {
	case 0:
		prg[A & 0x03] = V;
		Sync();
		break;
	case 1:
		chr[0 | (A & 0x03)] = V;
		if (iNESCart.submapper == 1) nt[A & 0x03] = V >> 7;
		Sync();
		break;
	case 2:
		chr[4 | (A & 0x03)] = V;
		Sync();
		break;
		break;
	case 3:
		IRQCount = 0;
		IRQa = TRUE;
		break;
	case 4:
		IRQa = FALSE;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 5:
		if (iNESCart.submapper == 0) nt[A & 0x03] = V & 0x01;
		Sync();
		break;
	}
}

static void M417Power(void) {
	memset(prg, 0, sizeof(prg));
	memset(chr, 0, sizeof(chr));
	memset(nt, 0, sizeof(nt));
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M417Write);
}

static void M417IRQHook(int a) {
	uint16 mask = (iNESCart.submapper == 1) ? 0x1000 : 0x400;

	IRQCount += a;
	if (IRQa && (IRQCount & mask)) {
		X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper417_Init(CartInfo *info) {
	info->Power = M417Power;
	MapIRQHook = M417IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
