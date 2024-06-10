/* FCE Ultra - NES/Famicom Emulator
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

/* NES 2.0 Mapper 362 - PCB 830506C
 * 1995 Super HiK 4-in-1 (JY-005)
 */

#include "mapinc.h"

static uint8 game;
static uint8 PPUCHRBus;
static uint8 vrc4Prg[2];
static uint8 vrc4Mirr;
static uint8 vrc4Misc;
static uint16 vrc4Chr[8];
static uint8 vrc4IRQLatch;
static uint8 vrc4IRQa;
static uint8 vrc4IRQCount;
static int16 vrc4IRQCycles;

static SFORMAT StateRegs[] = {
    { &game,          1, "GAME" },
    { &PPUCHRBus,     1, "CHRB" },
	{ vrc4Prg,        2, "V4PR" },
	{ &vrc4Mirr,      1, "V4MI" },
	{ &vrc4Misc,      1, "V4MS" },
	{ vrc4Chr,       16, "V4CH" },
	{ &vrc4IRQLatch,  1, "VILA" },
	{ &vrc4IRQa,      1, "VIMO" },
	{ &vrc4IRQCount,  1, "VICO" },
	{ &vrc4IRQCycles, 2, "VICY" },
	{ 0 },
};

static void Sync(void) {
    uint32 cbase = (vrc4Misc << 13) & 0x4000;
	uint32 prgBase = (game == 0) ? ((vrc4Chr[PPUCHRBus] & 0x180) >> 3) : 0x40;
    uint32 chrBase = (game == 0) ? (vrc4Chr[PPUCHRBus] & 0x180) : 0x200;
	uint32 chrMask = (game == 0) ? 0x7F : 0x1FF;

	setprg8(0x8000 ^ cbase, prgBase | (vrc4Prg[0] & 0x0F));
	setprg8(0xA000,         prgBase | (vrc4Prg[1] & 0x0F));
	setprg8(0xC000 ^ cbase, prgBase | ((~1) & 0x0F));
	setprg8(0xE000,         prgBase | ((~0) & 0x0F));

	setchr1(0x0000, chrBase | (vrc4Chr[0] & chrMask));
	setchr1(0x0400, chrBase | (vrc4Chr[1] & chrMask));
	setchr1(0x0800, chrBase | (vrc4Chr[2] & chrMask));
	setchr1(0x0C00, chrBase | (vrc4Chr[3] & chrMask));
	setchr1(0x1000, chrBase | (vrc4Chr[4] & chrMask));
	setchr1(0x1400, chrBase | (vrc4Chr[5] & chrMask));
	setchr1(0x1800, chrBase | (vrc4Chr[6] & chrMask));
	setchr1(0x1C00, chrBase | (vrc4Chr[7] & chrMask));

	switch (vrc4Mirr & 0x03) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFW(writeVRC4) {
	uint8 index;
	A &= 0xF003;
	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		vrc4Prg[(A >> 13) & 1] = V;
		Sync();
		break;
	case 0x9000:
		if (~A & 2) {
			vrc4Mirr = V;
		} else if (~A & 1) {
			vrc4Misc = V;
		}
		Sync();
		break;
	case 0xF000:
		switch (A & 3) {
		case 0:
			vrc4IRQLatch = (vrc4IRQLatch & 0xF0) | (V & 0x0F);
			break;
		case 1:
			vrc4IRQLatch = (vrc4IRQLatch & 0x0F) | (V << 4);
			break;
		case 2:
			vrc4IRQa = V;
			if (vrc4IRQa & 0x02) {
				vrc4IRQCount = vrc4IRQLatch;
				vrc4IRQCycles = 341;
			}
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 3:
			/* vrc4IRQa = (vrc4IRQa & ~0x02) | ((vrc4IRQa << 1) & 0x02); */
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		}
		break;
	default:
		index = ((A - 0xB000) >> 11) | ((A >> 1) & 1);
		if (A & 1) {
			vrc4Chr[index] = (vrc4Chr[index] & 0x0F) | (V << 4);
		} else {
			vrc4Chr[index] = (vrc4Chr[index] & ~0x0F) | (V & 0x0F);
		}
		Sync();
		break;
	}
}

static void FP_FASTAPASS(1) M362CPUHook(int a) {
	int count = a;
	while (count--) {
		if ((vrc4IRQa & 0x02) && ((vrc4IRQa & 0x04) || ((vrc4IRQCycles -= 3) <= 0))) {
			if (~vrc4IRQa & 0x04) {
				vrc4IRQCycles += 341;
			}
			if (!++vrc4IRQCount) {
				vrc4IRQCount = vrc4IRQLatch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void FP_FASTAPASS(1) M362PPUHook(uint32 A) {
    uint8 bank = (A & 0x1FFF) >> 10;
	if ((game == 0) && (PPUCHRBus != bank) && ((A & 0x3000) != 0x2000)) {
		PPUCHRBus = bank;
		Sync();
	}
}

static void M362Reset(void) {
	if (PRGsize[0] <= (512 * 1024)) {
        game = 0;
    } else {
        game = (game + 1) & 0x01;
    }
	Sync();
}

static void M362Power(void) {
	int i;

	for (i = 0; i < 2; i++) {
		vrc4Prg[i] = 0;
	}
	for (i = 0; i < 8; i++) {
		vrc4Chr[i] = 0;
	}
	vrc4Mirr = vrc4Misc = vrc4IRQLatch = vrc4IRQa = vrc4IRQCount = vrc4IRQCycles = 0;
	PPUCHRBus = game = 0;
    Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeVRC4);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper362_Init(CartInfo *info) {
	info->Reset = M362Reset;
	info->Power = M362Power;
	MapIRQHook = M362CPUHook;
    PPU_hook = M362PPUHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, 0);
}
