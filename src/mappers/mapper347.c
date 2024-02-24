/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
 * NES 2.0 Mapper 347 - Kaiser 7030
 * UNIF UNL-KS7030
 * FDS Conversion - Yume Koujou: Doki Doki Panic
 *
 * Logical bank layot 32 K BANK 0, 64K BANK 1, 32K ~0 hardwired, 8K is missing
 * need redump from MASKROM!
 * probably need refix mapper after hard dump
 *
 */

/* 2020-3-6 - update mirroring
 * PRG-ROM Bank Select #1/Mirroring Select ($8000-$8FFF, write)
 * A~FEDC BA98 7654 3210
 * -------------------
 *  1000 .... .... MBBB
 *                 |+++- Select 4 KiB PRG-ROM bank at CPU $7000-$7FFF
 *                 +---- Select nametable mirroring type
 *                        0: Vertical
 *                        1: Horizontal
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg[2];
static void (*WSync)(void);

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

/* Legacy bank order */

/* 6000 - 6BFF - RAM
 * 6C00 - 6FFF - BANK 1K REG1
 * 7000 - 7FFF - BANK 4K REG0
 * 8000 - B7FF - PRG
 * B800 - BFFF - RAM
 * C000 - CBFF - BANK 3K
 * CC00 - D7FF - RAM
 * D800 - FFFF - PRG
 */

static void M347Sync2(void) {
	setchr8(0);
	setprg32(0x8000, ~0);
	setprg4(0xb800, reg[0] & 0x07);
	setprg4(0xc800, reg[1] + 8);
	setmirror(((reg[0] >> 3) & 0x01) ^ 0x01);
}

static DECLFR(M347Read2) {
	if ((A >= 0x6000) && (A <= 0x6BFF)) {
		return WRAM[A - 0x6000];
	} else if ((A >= 0x6C00) && (A <= 0x6FFF)) {
		return CartBR(0xC800 + (A - 0x6C00));
	} else if ((A >= 0x7000) && (A <= 0x7FFF)) {
		return CartBR(0xB800 + (A - 0x7000));
	} else if ((A >= 0xB800) && (A <= 0xBFFF)) {
		return WRAM[0x0C00 + (A - 0xB800)];
	} else if ((A >= 0xC000) && (A <= 0xCBFF)) {
		return CartBR(0xCC00 + (A - 0xC000));
	} else if ((A >= 0xCC00) && (A <= 0xD7FF)) {
		return WRAM[0x1400 + (A - 0xCC00)];
	} else if ((A >= 0x8000) && (A <= 0xB7FF)) {
		return CartBR(A);
	} else if (A >= 0xD800) {
		return CartBR(A);
	}
	return cpu.openbus;
}

static DECLFW(M347Write2) {
	if ((A >= 0x6000) && (A <= 0x6BFF)) {
		WRAM[A - 0x6000] = V;
	} else if ((A >= 0x6C00) && (A <= 0x6FFF)) {
		CartBW(0xC800 + (A - 0x6C00), V);
	} else if ((A >= 0x7000) && (A <= 0x7FFF)) {
		CartBW(0xB800 + (A - 0x7000), V);
	} else if ((A >= 0xB800) && (A <= 0xBFFF)) {
		WRAM[0x0C00 + (A - 0xB800)] = V;
	} else if ((A >= 0xC000) && (A <= 0xCBFF)) {
		CartBW(0xCC00 + (A - 0xC000), V);
	} else if ((A >= 0xCC00) && (A <= 0xD7FF)) {
		WRAM[0x1400 + (A - 0xCC00)] = V;
	} else if ((A >= 0x8000) && (A <= 0x8FFF)) {
		reg[0] = A & 0x0F;
		WSync();
	} else if ((A >= 0x9000) && (A <= 0x9FFF)) {
		reg[1] = A & 0x0F;
		WSync();
	}
}
/* end of legacy bank order handling */

static void Sync(void) {
	setchr8(0);
	setprg32(0x8000, ~0);
	if (reg[0] & 8)
		setmirror(MI_H);
	else
		setmirror(MI_V);
}

static DECLFR(M347Read) {
	if (A >= 0x6000 && A <= 0x6BFF) {
		return WRAM[A - 0x6000 + 0x00000];
	} else if (A >= 0x6C00 && A <= 0x6FFF) {
		return PRGptr[0][A - 0x6000 + 0x01000 * reg[1] + 0x00000];
	} else if (A >= 0x7000 && A <= 0x7FFF) {
		return PRGptr[0][A - 0x7000 + 0x01000 * (reg[0] & 0x07) + 0x10000];
	} else if (A >= 0xB800 && A <= 0xBFFF) {
		return WRAM[A - 0xB800 + 0x00C00];
	} else if (A >= 0xC000 && A <= 0xCBFF) {
		return PRGptr[0][A - 0xC000 + 0x01000 * reg[1] + 0x00000];
	} else if (A >= 0xCC00 && A <= 0xD7FF) {
		return WRAM[A - 0xCC00 + 0x01400];
	} else if (A >= 0x8000) {
		return PRGptr[0][A - 0x8000 + 0x18000];
	}
	return cpu.openbus;
}

static DECLFW(M347Write) {
	if (A >= 0x6000 && A <= 0x6BFF) {
		WRAM[A - 0x6000 + 0x0000] = V;
	} else if (A >= 0xB800 && A <= 0xBFFF) {
		WRAM[A - 0xB800 + 0x0C00] = V;
	} else if (A >= 0xCC00 && A <= 0xD7FF) {
		WRAM[A - 0xCC00 + 0x1400] = V;
	} else if (A >= 0x8000 && A <= 0x8FFF) {
		reg[0] = A & 15;
		WSync();
	} else if (A >= 0x9000 && A <= 0x9FFF) {
		reg[1] = A & 15;
		WSync();
	}
}

static void M347Power(void) {
	FDSSound_Power();
	reg[0] = reg[1] = ~0;

	if (iNESCart.PRGCRC32 == 0xFA4DAC91) {
		SetReadHandler(0x6000, 0xFFFF, M347Read2);
		SetWriteHandler(0x6000, 0xFFFF, M347Write2);
		WSync = M347Sync2;
	} else {
		SetReadHandler(0x6000, 0xFFFF, M347Read);
		SetWriteHandler(0x6000, 0xFFFF, M347Write);
		WSync = Sync;
	}
	WSync();
}

static void M347Close(void) {
}

static void StateRestore(int version) {
	WSync();
}

void Mapper347_Init(CartInfo *info) {
	info->Power = M347Power;
	info->Close = M347Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
