/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* added 2020-1-28 - */
/* NES 2.0 Mapper 372 is used for a revision of the Rockman I-VI multicart (PCB ID SFC-12).
 * It is INES Mapper 045 but with one bit of outer bank register #2 working as a CHR-ROM/RAM switch.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 cmd;

static uint32 CHRRAMSIZE;
static uint8 *CHRRAM;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &cmd, 1, "CMD0" },
	{ 0 }
};

static void M372CW(uint16 A, uint16 V) {
	if (reg[2] & 0x20) {
		setchr8r(0x10, 0);
	} else {
		uint32 mask = 0xFF >> (~reg[2] & 0x0F);
		uint32 base = ((reg[2] << 4) & 0xF00) | reg[0];

		setchr1(A, (base & ~mask) | (V & mask));
	}
}

static void M372PW(uint16 A, uint16 V) {
	uint32 mask = ~reg[3] & 0x3F;
	uint32 base = ((reg[2] << 2) & 0x300) | reg[1];

	setprg8(A, (base & ~mask) | (V & mask));
}

static DECLFW(M372Write) {
	if (!(reg[3] & 0x40)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 0x03;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M372Reset(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Reset();
}

static void M372Power(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M372Write);
}

static void M372Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

void Mapper372_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M372CW;
	MMC3_pwrap = M372PW;
	info->Reset = M372Reset;
	info->Power = M372Power;
	info->Close = M372Close;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAMSIZE = 8192;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}
