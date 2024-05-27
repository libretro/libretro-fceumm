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

/* NES 2.0 Mapper 448
 * VRC4-based 830768C multicart circuit board used by a Super 6-in-1 multicart.
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg;
static uint8 vrc4Prg;
static uint8 vrc4Mirr;
static uint8 vrc4Misc;
static uint8 vrc4IRQLatch;
static uint8 vrc4IRQa;
static uint8 vrc4IRQCount;
static int16 vrc4IRQCycles;

static SFORMAT StateRegs[] = {
	{ &reg,           1, "REGS" },
	{ &vrc4Prg,       1, "PREG" },
	{ &vrc4Mirr,      1, "V4MI" },
	{ &vrc4Misc,      1, "V4MS" },
	{ &vrc4IRQLatch,  1, "VILA" },
	{ &vrc4IRQa,      1, "VIMO" },
	{ &vrc4IRQCount,  1, "VICO" },
	{ &vrc4IRQCycles, 2, "VICY" },
	{ 0 },
};

static void Sync(void) {
	setchr8(0);
	if (reg & 0x08) { /* AOROM */
		setprg32(0x8000, ((reg << 2) & ~0x07) | (latch.data & 0x07));
		setmirror(MI_0 + ((latch.data >> 4) & 0x01));
	} else {
		if (reg & 0x04) { /* UOROM */
			setprg16(0x8000, ((reg << 3) & ~0x0F) | (vrc4Prg & 0x0F));
			setprg16(0xC000, ((reg << 3) & ~0x0F) | 0x0F);
		} else { /* UNROM */
			setprg16(0x8000, (reg << 3) | (vrc4Prg & 0x07));
			setprg16(0xC000, (reg << 3) | 0x07);
		}
		switch (vrc4Mirr & 0x03) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
	}
}

static DECLFW(writeVRC4) {
	if (A < 0x9000) {
		vrc4Prg = V;
		Sync();
	} else {
		switch (A & 0xF000 | ((A >> 2) & 3)) {		
		case 0x9000:
			vrc4Mirr = V;
			Sync();
			break;
		case 0x9002:
			vrc4Misc = V;
			break;
		case 0xF000:
			vrc4IRQLatch = (vrc4IRQLatch & 0xF0) | (V & 0x0F);
			break;
		case 0xF001:
			vrc4IRQLatch = (vrc4IRQLatch & 0x0F) | (V << 4);
			break;
		case 0xF002:
			vrc4IRQa = V;
			if (vrc4IRQa & 0x02) {
				vrc4IRQCount = vrc4IRQLatch;
				vrc4IRQCycles = 341;
			}
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 0xF003:
			vrc4IRQa = (vrc4IRQa & ~0x02) | ((vrc4IRQa << 1) & 0x02);
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		}
	}
}

static DECLFW(M448WriteReg) {
	if (vrc4Misc & 1) {
		reg = A & 0xFF;
		Sync();
	}
}

static DECLFW(M448Write) {
	LatchWrite(A, V);
    writeVRC4(A, V);
}

static void FP_FASTAPASS(1) M448CPUHook(int a) {
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

static void M448Reset(void) {
	reg = 0;
	Sync();
}

static void M448Power(void) {
	reg = 0;
	vrc4Prg = 0;
	vrc4Mirr = vrc4Misc = 0;
	vrc4IRQLatch = vrc4IRQa = vrc4IRQCount = vrc4IRQCycles = 0;
	LatchPower();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, M448WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M448Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper448_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
	info->Reset = M448Reset;
	info->Power = M448Power;
	MapIRQHook = M448CPUHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, 0);
}
