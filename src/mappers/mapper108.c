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
 * FDS Conversion
 *
 * Bubble Bobble (LH31)
 * - submapper 1: CHR-RAM 8K
 * - submapper 2: CHR-ROM 32K - UNIF UNL-BB
 * Submapper 3:
 * - Falsion (LH54)
 * - Meikyuu Jiin Dababa (LH28)
 * Submapper 4: UNIF UNL-BB
 * - Pro Wrestling (LE05)
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	if (iNESCart.submapper == 4) {
		setprg8(0x6000, ~0);	
	} else {
		setprg8(0x6000, reg);
	}
	setprg32(0x8000, ~0);
	if (UNIFchrrama) {
		setchr8(0);
	} else {
		setchr8(reg);
	}
}

static DECLFW(M108Write) {
	reg = V;
	Sync();
}

static void M108Power(void) {
	reg = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	switch (iNESCart.submapper) {
	case 1:  SetWriteHandler(0xF000, 0xFFFF, M108Write); break;
	case 2:  SetWriteHandler(0xE000, 0xFFFF, M108Write); break;
	default: SetWriteHandler(0x8000, 0xFFFF, M108Write); break;
	}
	FDSSound_Power();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper108_Init(CartInfo *info) {
	info->Power = M108Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	if (!info->iNES2 || !info->submapper) {
		if (UNIFchrrama) {
			if (info->mirror == MI_H) {
				info->submapper = 1;
			} else {
				info->submapper = 3;
			}
		} else {
			if (CHRsize[0] > (16 * 1024)) {
				info->submapper = 2;
			} else {
				info->submapper = 4;
			}
		}
	}
}
