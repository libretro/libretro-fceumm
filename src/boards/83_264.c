/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2026 NewRisingSun
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
 *
 */

#include "mapinc.h"
#include "cartram.h"

static uint8_t reg[16];
static uint8_t scratch[4];
static uint8_t flags;
static uint8_t pad;
static int mapper;
static uint8_t submapper;

static SFORMAT stateRegs[] = {
	{ reg,    16, "REGS" },
	{ scratch, 4, "SCRA" },
	{&flags,   1, "FLAG" },
	{&pad,     1, "DIPS" },
	{ 0 }
};

static void sync () {
	int prgAND = mapper == 264 || submapper == 3? 0x0F: 0x1F;
	switch(reg[1] &0x18) {
		case 0x00:
			setprg16(0x8000, reg[0]);
			setprg16(0xC000, reg[0] | prgAND >>1);
			break;
		case 0x08:
			setprg16(0x8000, reg[0]);
			setprg16(0xC000, reg[0]);
			break;
		case 0x10: case 0x18:
			setprg8(0x8000, reg[0] <<1 &~prgAND | reg[4] &prgAND);
			setprg8(0xA000, reg[0] <<1 &~prgAND | reg[5] &prgAND);
			setprg8(0xC000, reg[0] <<1 &~prgAND | reg[6] &prgAND);
			setprg8(0xE000, reg[0] <<1 &~prgAND | 0xFF &prgAND);
			break;
	}

	if (submapper == 2 && PRGsize[0x10])
		setprg8r(0x10, 0x6000, reg[0] >>6);
	else
		setprg8(0x6000, reg[7]);

	if (submapper == 2 || reg[1] &0x20) {
		SetReadHandler (0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
	} else {
		SetReadHandler (0x6000, 0xFFFF, CartBROB);
		SetWriteHandler(0x6000, 0x7FFF, NULL);
	}

	switch(mapper == 264? 1: submapper) {
		case 0:
			setchr1(0x0000, reg[8 | 0]);
			setchr1(0x0400, reg[8 | 1]);
			setchr1(0x0800, reg[8 | 2]);
			setchr1(0x0C00, reg[8 | 3]);
			setchr1(0x1000, reg[8 | 4]);
			setchr1(0x1400, reg[8 | 5]);
			setchr1(0x1800, reg[8 | 6]);
			setchr1(0x1C00, reg[8 | 7]);
			break;
		case 1:
			setchr2(0x0000, reg[8 | 0]);
			setchr2(0x0800, reg[8 | 1]);
			setchr2(0x1000, reg[8 | 6]);
			setchr2(0x1800, reg[8 | 7]);
			break;
		case 2:
			setchr1(0x0000, reg[0] <<4 &~0xFF | reg[8 | 0]);
			setchr1(0x0400, reg[0] <<4 &~0xFF | reg[8 | 1]);
			setchr1(0x0800, reg[0] <<4 &~0xFF | reg[8 | 2]);
			setchr1(0x0C00, reg[0] <<4 &~0xFF | reg[8 | 3]);
			setchr1(0x1000, reg[0] <<4 &~0xFF | reg[8 | 4]);
			setchr1(0x1400, reg[0] <<4 &~0xFF | reg[8 | 5]);
			setchr1(0x1800, reg[0] <<4 &~0xFF | reg[8 | 6]);
			setchr1(0x1C00, reg[0] <<4 &~0xFF | reg[8 | 7]);
			break;
		case 3:
			setchr1(0x0000, reg[0] <<2 &~0xFF | reg[8 | 0]);
			setchr1(0x0400, reg[0] <<2 &~0xFF | reg[8 | 1]);
			setchr1(0x0800, reg[0] <<2 &~0xFF | reg[8 | 2]);
			setchr1(0x0C00, reg[0] <<2 &~0xFF | reg[8 | 3]);
			setchr1(0x1000, reg[0] <<2 &~0xFF | reg[8 | 4]);
			setchr1(0x1400, reg[0] <<2 &~0xFF | reg[8 | 5]);
			setchr1(0x1800, reg[0] <<2 &~0xFF | reg[8 | 6]);
			setchr1(0x1C00, reg[0] <<2 &~0xFF | reg[8 | 7]);
			break;
	}
	switch (reg[1] &0x03) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
	}
}

static DECLFR (readPadScratch) {
	if (A &(mapper == 264? 0x400: 0x100))
		return scratch[A &3];
	else
		return pad;
}

static DECLFW (writeScratch) {
	scratch[A &3] = V;
}

static DECLFW (writeReg) {
	if (mapper == 264) A = A >>2 &~0x3F | A &0x3F;
	switch (A &0x318) {
		case 0x000: case 0x008: case 0x010: case 0x018: case 0x100: case 0x108: case 0x110: case 0x118:
			reg[A >>8 &1] = V;
			sync();
			break;
		case 0x200: case 0x208: case 0x210: case 0x218:
			reg[2 | A &1] = V;
			if (A &1)
				flags = flags &~0x80 | reg[1] &0x80; /* Copy "counting" flag from Misc. register to internal flag register */
			else
				X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 0x300: case 0x308:
			reg[4 | A &3] = V;
			sync();
			break;
		case 0x310:
			reg[8 | A &7] = V;
			sync();
			break;
		case 0x318:
			flags = flags &~0x40 | V &0x40;
			break;

	}
}

void clockCounter (void) {
	uint16_t counter = reg[2] | reg[3] <<8;
	if (flags &0x80 && counter) {
		if (reg[1] &0x40)
			counter--;
		else
			counter++;
		if (!counter) {
			X6502_IRQBegin(FCEU_IQEXT);
			flags &= ~0x80;
		}
	}
	reg[2] = counter &0xFF;
	reg[3] = counter >>8 &0xFF;
}

void FP_FASTAPASS(1) cycleCounter (int a) {
	while (a--) if (~flags &0x40) clockCounter();
}

void scanlineCounter() {
	if (flags &0x40) {
		clockCounter();
		clockCounter();
		clockCounter();
		clockCounter();
		clockCounter();
		clockCounter();
		clockCounter();
		clockCounter();
	}
}

static void reset () {
	int i;
	for (i = 0; i < 16; i++) reg[i] = 0;
	pad++;
	X6502_IRQEnd(FCEU_IQEXT);
	sync();
}

static void power () {
	int i;
	SetReadHandler(0x5000, 0x5FFF, readPadScratch);
	SetWriteHandler(0x5000, 0x5FFF, writeScratch);
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeReg);
	MapIRQHook = cycleCounter;
	GameHBIRQHook = scanlineCounter;
	for (i = 0; i < 16; i++) reg[i] = 0;
	for (i = 4; i < 4; i++) scratch[i] = 0;
	pad = 0;
	sync();
}

void restore (int version) {
	sync();
}

void Mapper83_Init (CartInfo *info) {
	mapper = 83;
	submapper = info->submapper;
	if (!info->iNES2) {
		if (info->CHRRomSize ==512*1024) submapper = 1; else
		if (info->PRGRomSize ==1024*1024) submapper = 2; else
		if (info->PRGRomSize ==512*1024) submapper = 3;
	}
	if (submapper == 2) WRAM_init(info, 32);
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = restore;
	AddExState(stateRegs, ~0, 0, 0);
}

void Mapper264_Init (CartInfo *info) {
	mapper = 264;
	submapper = info->submapper;
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = restore;
	AddExState(stateRegs, ~0, 0, 0);
}
