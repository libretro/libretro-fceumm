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

/* NES 2.0 Mapper 398 - PCB YY840820C
 * 1995 Super HiK 5-in-1 - 新系列米奇老鼠組合卡 (JY-048)
 */

#include "mapinc.h"

static uint8 latch;
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
	{ &latch, 1, "GAME" },
	{ &PPUCHRBus, 1, "CHRB" },
	{ vrc4Prg, 2, "V4PR" },
	{ &vrc4Mirr, 1, "V4MI" },
	{ &vrc4Misc, 1, "V4MS" },
	{ vrc4Chr, 16, "V4CH" },
	{ &vrc4IRQLatch, 1, "VILA" },
	{ &vrc4IRQa, 1, "VIMO" },
	{ &vrc4IRQCount, 1, "VICO" },
	{ &vrc4IRQCycles, 2, "VICY" },
	{ 0 },
};

static void Sync(void) {
	if (latch & 0x80) {
        /* GNROM-like */
		setprg32(0x8000, ((latch >> 5) & 0x06) | ((vrc4Chr[PPUCHRBus] >> 2) & 0x01));
		setchr8(0x40 | ((latch >> 3) & 0x08) | (vrc4Chr[PPUCHRBus] & 0x07));
	} else {
		uint32 cbase = (vrc4Misc << 13) & 0x4000;

		setprg8(0x8000 ^ cbase, (vrc4Prg[0] & 0x0F));
		setprg8(0xA000,         (vrc4Prg[1] & 0x0F));
		setprg8(0xC000 ^ cbase, ((~1) & 0x0F));
		setprg8(0xE000,         ((~0) & 0x0F));

		setchr1(0x0000, (vrc4Chr[0] & 0x1FF));
		setchr1(0x0400, (vrc4Chr[1] & 0x1FF));
		setchr1(0x0800, (vrc4Chr[2] & 0x1FF));
		setchr1(0x0C00, (vrc4Chr[3] & 0x1FF));
		setchr1(0x1000, (vrc4Chr[4] & 0x1FF));
		setchr1(0x1400, (vrc4Chr[5] & 0x1FF));
		setchr1(0x1800, (vrc4Chr[6] & 0x1FF));
		setchr1(0x1C00, (vrc4Chr[7] & 0x1FF));
	}

	switch (vrc4Mirr & 0x03) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFW(writeVRC4) {
	uint8 index;
    latch = A & 0xFF;
	A &= 0xF003;
	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		vrc4Prg[(A >> 13) & 1] = V;
		break;
	case 0x9000:
		if (~A & 2) {
			vrc4Mirr = V;
		} else if (~A & 1) {
			vrc4Misc = V;
		}
		break;
	case 0xF000:
		switch (A & 3) {
		case 0: vrc4IRQLatch = (vrc4IRQLatch & 0xF0) | (V & 0x0F); break;
		case 1: vrc4IRQLatch = (vrc4IRQLatch & 0x0F) | (V << 4); break;
		case 2:
			vrc4IRQa = V;
			if (vrc4IRQa & 0x02) {
				vrc4IRQCount = vrc4IRQLatch;
				vrc4IRQCycles = 341;
			}
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 3:
			vrc4IRQa = (vrc4IRQa & ~0x02) | ((vrc4IRQa << 1) & 0x02);
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
		break;
	}

    Sync();
}

static void FP_FASTAPASS(1) M398CPUHook(int a) {
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

static void FP_FASTAPASS(1) M398PPUHook(uint32 A) {
	uint8 bank = (A & 0x1FFF) >> 10;
	if ((PPUCHRBus != bank) && ((A & 0x3000) != 0x2000)) {
		PPUCHRBus = bank;
		Sync();
	}
}

static void M398Reset(void) {
	latch = 0xC0;
	Sync();
}

static void M398Power(void) {
	int i;

	for (i = 0; i < 2; i++)
		vrc4Prg[i] = 0;
	for (i = 0; i < 8; i++)
		vrc4Chr[i] = 0;
	vrc4Mirr = vrc4Misc = vrc4IRQLatch = vrc4IRQa = vrc4IRQCount = vrc4IRQCycles = 0;
	PPUCHRBus = 0;
    latch = 0xC0;
	Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeVRC4);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper398_Init(CartInfo *info) {
	info->Reset = M398Reset;
	info->Power = M398Power;
	MapIRQHook = M398CPUHook;
	PPU_hook = M398PPUHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, 0);
}
