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

#include "mapinc.h"
#include "mmc3.h"

/* MMC3 board with optional command address line connection, allows to
 * make three-four different wirings to IRQ address lines and separately to
 * CMD address line, Mali Boss additionally check if wiring are correct for
 * game
 */

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void M196PW(uint16 A, uint16 V) {
	if (reg[0]) {
		setprg32(0x8000, reg[1]);
    } else {
		setprg8(A, V);
    }
}

static DECLFW(M196Write) {
	A = (A & 0xF000) | (!!(A & 0x0E) ^ (A & 0x01));
	MMC3_Write(A, V);
}

static DECLFW(M196WriteNROM) {
	reg[0] = 1;
	reg[1] = V | (V >> 4);
	MMC3_FixPRG();
}

static void M196Power(void) {
	reg[0] = reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M196WriteNROM);
	SetWriteHandler(0x8000, 0xFFFF, M196Write);
}

void Mapper196_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M196PW;
	info->Power = M196Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
