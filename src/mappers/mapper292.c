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
 * NES 2.0 Mapper 292 - PCB BMW8544
 * UNIF UNL-DRAGONFIGHTER
 * "Dragon Fighter" protected MMC3 based custom mapper board
 * mostly hacky implementation, I can't verify if this mapper can read a RAM of the
 * console or watches the bus writes somehow.
 *
 * TODO: needs updating
 *
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[3];

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
	{ 0 }
};

static void M292PW(uint16 A, uint16 V) {
	setprg8(A, V);
	setprg8(0x8000, reg[0] & 0x1F);
}

static void M292CW(uint16 A, uint16 V) {
	setchr2(0x0000, (mmc3.reg[0] >> 1) ^ reg[1]);
	setchr2(0x0800, (mmc3.reg[1] >> 1) ^ ((reg[2] & 0x40) << 1));
	setchr4(0x1000, reg[2] & 0x3F);
}

static DECLFW(M292ProtWrite) {
	reg[0] = V;
	MMC3_FixPRG();
}

static DECLFR(M292ProtRead) {
	if ((reg[0] & 0xE0) == 0xC0) {
		reg[1] = ARead[0x6A](0x6A);
	} else {
		reg[2] = ARead[0xFF](0xFF);
	}
	MMC3_FixCHR();
	return cpu.openbus;
}

static void M292Power(void) {
	reg[0] = reg[1] = reg[2] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M292ProtWrite);
	SetReadHandler(0x6000, 0x7FFF, M292ProtRead);
}

void Mapper292_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap  = M292PW;
	MMC3_cwrap  = M292CW;
	info->Power = M292Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
