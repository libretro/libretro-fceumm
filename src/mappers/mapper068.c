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
 */

#include "mapinc.h"

/* FIXME: needs updating, submapper 1 support etc */

static uint8 chr[4];
static uint8 nt[2];
static uint8 kogame, prg, mirr;
static uint32 count;

static SFORMAT StateRegs[] = {
	{ &mirr, 1, "MIRR" },
	{ &prg, 1, "PRG" },
	{ &kogame, 1, "KGME" },
	{ &count, 4, "CNT" },
	{ chr, 4, "CHR" },
	{ nt, 2, "NTAR" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg16r((PRGptr[1]) ? kogame : 0, 0x8000, prg);
	setprg16(0xC000, 0x07);

	setchr2(0x0000, chr[0]);
	setchr2(0x0800, chr[1]);
	setchr2(0x1000, chr[2]);
	setchr2(0x1800, chr[3]);

	if ((!UNIFchrrama) && (mirr & 0x10)) {
		int i;
		PPUNTARAM = 0;
		for (i = 0; i < 4; i++) {
			switch (mirr & 0x03) {
			case 0: vnapage[i] = CHRptr[0] + (((nt[i & 0x01] | 0x80) & CHRmask1[0]) << 10); break;
			case 1: vnapage[i] = CHRptr[0] + (((nt[(i >> 0x01) & 0x01] | 0x80) & CHRmask1[0]) << 10); break;
			case 2: vnapage[i] = CHRptr[0] + (((nt[0] | 0x80) & CHRmask1[0]) << 10); break;
			case 3: vnapage[i] = CHRptr[0] + (((nt[1] | 0x80) & CHRmask1[0]) << 10); break;
			}
		}
	} else {
		switch (mirr & 0x03) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
	}
}

static DECLFR(M68Read) {
	if (!(kogame & 0x08)) {
		count++;
		if (count == 1784) {
			setprg16r(0, 0x8000, prg);
		}
	}
	return CartBR(A);
}

static DECLFW(M68WriteLo) {
	if (!V) {
		count = 0;
		setprg16r((PRGptr[1]) ? kogame : 0, 0x8000, prg);
	}
	CartBW(A, V);
}

static DECLFW(M068Write) {
	switch (A & 0xF000) {
	case 0x8000:
	case 0x9000:
	case 0xA000:
	case 0xB000:
		chr[(A >> 12) & 0x03] = V;
		Sync();
		break;
	case 0xC000:
	case 0xD000:
		nt[(A >> 12) & 0x01] = V;
		Sync();
		break;
	case 0xE000:
		mirr = V;
		Sync();
		break;
	case 0xF000:
		prg = V & 0x07;
		kogame = ((V >> 3) & 0x01) ^ 0x01;
		Sync();
		break;
	}
}

static void M68Power(void) {
	prg = 0;
	kogame = 0;
	Sync();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xBFFF, M68Read);
	SetReadHandler(0xC000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M068Write);
	SetWriteHandler(0x6000, 0x6000, M68WriteLo);
	SetWriteHandler(0x6001, 0x7FFF, CartBW);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M68Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper068_Init(CartInfo *info) {
	info->Power = M68Power;
	info->Close = M68Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
}
