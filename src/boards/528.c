/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

/* NES 2.0 Mapper 528 
 * UNIF BMC-831128C
 */

#include "mapinc.h"
#include "vrcirq.h"

static uint8 preg[4], creg[8], mirr;
static uint8 gamesel;
static uint8 *WRAM;
static uint32 WRAMSIZE;

static SFORMAT StateRegs[] =
{
	{ preg, 4, "PREG" },
	{ creg, 8, "CREG" },
	{ &mirr, 1, "MIRR" },
    { &gamesel, 1, "GSEL" },
	{ 0 }
};

static void Sync(void) {
    uint8 mask = (gamesel << 4) | 0x0F;
	if (preg[3] == 1) {
		setprg8r(0x10, 0x6000, 0);
    } else {
		setprg8(0x6000, (preg[3] & mask) + gamesel);
    }
	setprg8(0x8000, (preg[0] & mask) + (gamesel << 4));
	setprg8(0xA000, (preg[1] & mask) + (gamesel << 4));
	setprg8(0xC000, (     ~1 & mask) + (gamesel << 4));
	setprg8(0xE000, (     ~0 & mask) + (gamesel << 4));
	setchr1(0x0000, creg[0] + (gamesel << 8));
    setchr1(0x0400, creg[1] + (gamesel << 8));
    setchr1(0x0800, creg[2] + (gamesel << 8));
    setchr1(0x0C00, creg[3] + (gamesel << 8));
    setchr1(0x1000, creg[4] + (gamesel << 8));
    setchr1(0x1400, creg[5] + (gamesel << 8));
    setchr1(0x1800, creg[6] + (gamesel << 8));
    setchr1(0x1C00, creg[7] + (gamesel << 8));
	switch (mirr & 3) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
	}
}

static DECLFW(M528Write1) {
	switch (A & 0x0F) {
	case 0x0: creg[0] = V; break;
	case 0x1: creg[1] = V; break;
	case 0x2: creg[2] = V; break;
	case 0x3: creg[3] = V; break;
	case 0x4: creg[4] = V; break;
	case 0x5: creg[5] = V; break;
	case 0x6: creg[6] = V; break;
	case 0x7: creg[7] = V; break;
	case 0x8: preg[3] = V; break;
	case 0x9: preg[0] = V; break;
	case 0xA: preg[1] = V; break;
	case 0xB: break;
	case 0xC: mirr = V & 3; break;
	case 0xD: VRCIRQ_Control(V); break;
	case 0xE: VRCIRQ_Acknowledge(); break;
	case 0xF: VRCIRQ_Latch(V); break;
	}
	gamesel = (A & 0x4000) >> 14;
    Sync();
}

static void M528Power(void) {
    int i;
    gamesel = 0;
    preg[0] = 0;
    preg[1] = 1;
    preg[2] = 0;
    preg[3] = 0;
    for (i = 0; i < 8; i++) {
        creg[i] = i;
    }
	Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0xA000, 0xAFFF, M528Write1);
    SetWriteHandler(0xC000, 0xCFFF, M528Write1);

    SetReadHandler(0x6000, 0x7FFF, CartBR);
    SetWriteHandler(0x6000, 0x7FFF, CartBW);
    FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper528_Init(CartInfo *info) {
	info->Power = M528Power;
	GameStateRestore = StateRestore;
    VRCIRQ_Init();
    AddExState(&StateRegs, ~0, 0, 0);

    WRAMSIZE = 8192;
    WRAM = (uint8*) FCEU_gmalloc(WRAMSIZE);
    SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
    AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
