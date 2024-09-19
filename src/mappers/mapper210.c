/* FCEUmm - NES/Famicom Emulator
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

/* NES 2.0 Mapper 210 - simplified version of Mapper 19 
 * Namco 175 - submapper 1 - optional wram, hard-wired mirroring
 * Namco 340 - submapper 2 - selectable H/V/0 mirroring
 */

#include "mapinc.h"

static uint8 prg[4];
static uint8 chr[8];
static uint8 wram_enable;

static SFORMAT StateRegs[] = {
	{ prg, 3, "PRG" },
	{ chr, 8, "CHR" },
    { &wram_enable, 1, "WREN" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0] & 0x3F);
	setprg8(0xA000, prg[1] & 0x3F);
	setprg8(0xC000, prg[2] & 0x3F);
	setprg8(0xE000, prg[3] & 0x3F);

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	if (iNESCart.submapper != 1) {
		switch((prg[0] >> 6) & 0x03) {
		case 0: setmirror(MI_0); break;
		case 1: setmirror(MI_V); break;
		case 2: setmirror(MI_H); break;
		case 3: setmirror(MI_0); break;
		}
	}
}

static DECLFR(AWRAM) {
    A = ((A - 0x6000) & (WRAMSIZE -1));
	return WRAM[A];
}

static DECLFW(BWRAM) {
    if (wram_enable) {
        A = ((A - 0x6000) & (WRAMSIZE -1));
	    WRAM[A] = V;
    }
}

static DECLFW(M210Write) {
	switch (A & 0xF800) {
	case 0x8000:
	case 0x8800:
	case 0x9000:
	case 0x9800:
	case 0xA000:
	case 0xA800:
	case 0xB000:
	case 0xB800:
		chr[(A - 0x8000) >> 11] = V;
		Sync();
		break;
	case 0xC000:
		wram_enable = V & 0x01;
		break;
	case 0xE000:
	case 0xE800:
	case 0xF000:
		prg[(A - 0xE000) >> 11] = V;
		Sync();
		break;
	}
}

static void M210Power(void) {
	int i;
	for (i = 0; i < 4; i++) prg[i] = 0xFC | i;
	for (i = 0; i < 4; i++) chr[0 | i] = 0 | i;
	for (i = 0; i < 4; i++) chr[4 | i] = 4 | i;
	wram_enable = 0;
	Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xffff, M210Write);

	if (WRAM) {
        SetReadHandler(0x6000, 0x7FFF, AWRAM);
	    SetWriteHandler(0x6000, 0x7FFF, BWRAM);
	    FCEU_CheatAddRAM(8, 0x6000, WRAM);
    }

	if (WRAM && !iNESCart.battery) {
		FCEU_MemoryRand(WRAM, sizeof(WRAM));
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper210_Init(CartInfo *info) {
	GameStateRestore = StateRestore;
	info->Power = M210Power;
    AddExState(StateRegs, ~0, 0, NULL);

    WRAMSIZE = 8192;
    if (info->iNES2) {
        WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
    }

    if (WRAMSIZE) {
        WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	    AddExState(WRAM, WRAMSIZE, 0, "WRAM");
        if (info->battery) {
            info->SaveGame[0] = WRAM;
            info->SaveGameLen[0] = WRAMSIZE;
        }
    }
}
