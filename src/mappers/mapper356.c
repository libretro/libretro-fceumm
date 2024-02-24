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

/* NES 2.0 Mapper 356 -  J.Y. Company's 7-in-1 Rockman (JY-208)
 * All registers work as INES Mapper 045, except $6000 sequential register 2 (third write):
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 cmd;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE = 0;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &cmd, 1, "CMD0" },
	{ 0 }
};

static void M356CW(uint16 A, uint16 V) {
	if (reg[2] & 0x20) {
		uint16 mask = 0xFF >> (~reg[2] & 0xF);
		uint16 base = ((reg[2] << 4) & 0xF00) | reg[0];

		setchr1(A, (base & ~mask) | (V & mask));
	} else {
		setchr8r(0x10, 0);
	}
}

static void M356PW(uint16 A, uint16 V) {
	uint16 mask = ~reg[3] & 0x3F;
	uint16 base = ((reg[2] << 2) & 0x300) | reg[1];

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M356MIR(void) {
	if (reg[2] & 0x40) {
		setmirror(MI_4);
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static DECLFW(M356Write) {
	if (!(reg[3] & 0x40)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 3;
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	}
}

static void M356Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_free(CHRRAM);
	}
	CHRRAM = NULL;
}

static void M356Reset(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Reset();
}

static void M356Power(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M356Write);
}

void Mapper356_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M356CW;
	MMC3_pwrap = M356PW;
	MMC3_FixMIR = M356MIR;
	info->Reset = M356Reset;
	info->Power = M356Power;
	info->Close = M356Close;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAMSIZE = 8192;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}
