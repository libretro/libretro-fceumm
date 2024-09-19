/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/* NES 2.0 Mapper 297 - 2-in-1 Uzi Lightgun (MGC-002) */

#include "mapinc.h"
#include "mmc1.h"

static uint8 mode;
static uint8 latch;

static SFORMAT StateRegs[] = {
	{ &mode, 1, "MODE" },
	{ &latch, 1, "LATC" },
	{ 0 }
};

static void M297PRG(uint16 A, uint16 V) {
	setprg16(A, 0x08 | (V & 0x07));
}

static void M297CHR(uint16 A, uint16 V) {
	setchr4(A, 0x20 | (V & 0x1F));
}

static void Sync(void) {
	if (mode & 0x01) {
		/* MMC1 */
		MMC1_FixPRG();
		MMC1_FixCHR();
		MMC1_FixMIR();
	} else {
		/* Mapper 70 */
		setprg16(0x8000, ((mode & 0x02) << 1) | ((latch >> 4) & 0x03));
		setprg16(0xC000, ((mode & 0x02) << 1) | 0x03);
		setchr8(latch & 0x0F);
		setmirror(MI_V);
	}
}

static DECLFW(M297Mode) {
	if (A & 0x100) {
		mode = V;
		Sync();
	}
}

static DECLFW(M297Latch) {
	if (mode & 0x01) {
		MMC1_Write(A, V);
	} else {
		latch = V;
		Sync();
	}
}

static void M297Power(void) {
	latch = 0;
	mode = 0;
	Sync();
	MMC1_Power();
	SetWriteHandler(0x4100, 0x5FFF, M297Mode);
	SetWriteHandler(0x8000, 0xFFFF, M297Latch);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper297_Init(CartInfo *info) {
	MMC1_Init(info, 0, 0);
	info->Power = M297Power;
	MMC1_cwrap = M297CHR;
	MMC1_pwrap = M297PRG;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
