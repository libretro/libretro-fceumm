/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2015 CaH4e3
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
 * NES 2.0 Mapper 259 - BMC-F-15
 * BMC F-15 PCB (256+266 MMC3 based, with 16/32Kb banking discrete logic)
 * 150-in-1 Unchaied Melody FIGHT version with system test (START+SELECT)
 *
 * CHR - MMC3 stock regs
 * PRG - MMC3 regs disabled, area 6000-7FFF used instead
 *		 011xxxxxxxxxxxxx addr mask,
 *       ----APPp reg bits mask
 *       A - higher 128K PRG bank select/32K bank mode override
 *       PP - bank number in 32K mode
 *       PPp - bank number in 16K mode
 * initial state of extra regs is undefined, A001 enables/disables the 6000 area
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M259PW(uint16 A, uint16 V) {
	uint8 mode = (reg & 0x08) >> 3;

	setprg16(0x8000, ((reg & 0x0F) & ~mode));
	setprg16(0xC000, ((reg & 0x0F) |  mode));
}

static DECLFW(M259Write) {
	if (MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
	}
}

static void M259Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M259Write);
}

void Mapper259_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M259PW;
	info->Power = M259Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
