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
 * Many-in-one hacked mapper crap.
 *
 * Original BNROM is actually AxROM variations without mirroring control,
 * and haven't SRAM on-board, so it must be removed from here
 *
 * Difficult banking is what NINA board doing, most hacks for 34 mapper are
 * NINA hacks, so this is actually 34 mapper
 *
 */

#include "mapinc.h"
#include "latch.h"

#define M034_NINA001  1
#define M034_BNROM    2
#define M034_NESTICLE 3

static uint8 reg[3];

static uint8 type;
static void (*WSync)(void);

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
	{ 0 }
};

/* submapper 1 - AVE NINA-001 */

static void Sync_NINA001(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg32(0x8000, reg[0]);
	setchr4(0x0000, reg[1]);
	setchr4(0x1000, reg[2]);
}

static DECLFW(M034Write_NINA001) {
	CartBW(A, V);
	if (A >= 0x7FFD) {
		reg[A - 0x7FFD] = V;
		WSync();
	}
}

static void M034Power_NINA001(void) {
	reg[0] = reg[1] = 0;
	reg[2] = 1;
	WSync();

	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, M034Write_NINA001);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

/* submapper 2 - BNROM */

static void Sync_BNROM(void) {
	setprg32(0x8000, latch.data);
	setchr8(0);
}

/* nesticle */

static void Sync_Nesticle(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg32(0x8000, reg[0]);
	setchr4(0x0000, reg[1]);
	setchr4(0x1000, reg[2]);
}

static DECLFW(M034Write_Nesticle) {
	if (A >= 0x8000) {
		reg[0] = V;
		WSync();
	} else {
		CartBW(A, V);
		if (A >= 0x7FFD) {
			reg[A - 0x7FFD] = V;
			WSync();
		}
	}
}

static void M034Power_Nesticle(void) {
	reg[0] = reg[1] = 0;
	reg[2] = 1;
	WSync();

	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0xFFFF, M034Write_Nesticle);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M034Close(void) {
}

static void StateRestore(int version) {
	WSync();
}

void Mapper034_Init(CartInfo *info) {
	if (ROM.trainer.data && (ROM.trainer.size > 0)) {
		type = M034_NESTICLE;
		WSync = Sync_Nesticle;
		info->Power = M034Power_Nesticle;
	} else if ((info->submapper == 1) || ((info->submapper != 2) && ROM.chr.size > 0)) {
		type = M034_NINA001;
		WSync = Sync_NINA001;
		info->Power = M034Power_NINA001;
	} else if ((info->submapper == 2) || ((info->submapper != 1) && ROM.chr.size == 0)) {
		type = M034_BNROM;
		Latch_Init(info, Sync_BNROM, NULL, FALSE, TRUE);
		info->Reset = Latch_RegReset;
	}

	switch (type) {
	case M034_NESTICLE:
	case M034_NINA001:
		info->Close = M034Close;
		GameStateRestore = StateRestore;
		AddExState(StateRegs, ~0, 0, NULL);

		WRAMSIZE = 8192;
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		break;
	}
}
