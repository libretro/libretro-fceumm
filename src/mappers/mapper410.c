/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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

/* NES 2.0 Mapper 410 is a variant of mapper 45 where the
 * ASIC's PRG A21/CHR A20 output (set by bit 6 of the third write to $6000)
 * selects between regularly-banked CHR-ROM (=0) and 8 KiB of unbanked CHR-RAM (=1).
 * It is used solely for the Super 8-in-1 - 98格鬥天王＋熱血 (JY-302) multicart.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 cmd;

static uint8 *CHRRAM;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &cmd, 1, "CMD0" },
	{ 0 }
};

static void M410PW(uint16 A, uint16 V) {
	uint32 mask = ~reg[3] & 0x3F;
	uint32 base = ((reg[2] << 2) & 0x300) | reg[1];

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M410CW(uint16 A, uint16 V) {
	if (!(reg[2] & 0x40)) {
		uint32 mask = 0xFF >> (~reg[2] & 0x0F);
		uint32 base = ((reg[2] << 4) & 0xF00) | reg[0];

		setchr1(A, (base & ~mask) | (V & mask));
	} else {
		setchr8r(0x10, 0);
	}
}

static DECLFW(M410Write) {
	if (!(reg[3] & 0x40)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 0x03;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M410Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_free(CHRRAM);
	}
	CHRRAM = NULL;
}

static void M410Reset(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Reset();
}

static void M410Power(void) {
	MMC3_Power();
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	SetWriteHandler(0x6000, 0x7FFF, M410Write);
}

void Mapper410_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M410CW;
	MMC3_pwrap = M410PW;
	info->Reset = M410Reset;
	info->Power = M410Power;
	info->Close = M410Close;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAM = (uint8 *)FCEU_gmalloc(8192);
	SetupCartCHRMapping(0x10, CHRRAM, 8192, 1);
	AddExState(CHRRAM, 8192, 0, "CRAM");
}