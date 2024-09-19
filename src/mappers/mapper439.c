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

/* NES 2.0 Mapper 439 denotes the YS2309 multicart PCB.
 * Its UNIF MAPRs are BMC-DS-07 and BMC-K86B.
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg[2];

static void Sync(void) {
    uint8 mask = ((~reg[1] >> 1) & 0x38) | 0x07;
    uint8 base = reg[0] >> 1;
	setprg16(0x8000, (base & ~mask) | (latch.data & mask));
	setprg16(0xC000, (base & ~mask) | (0x3F & mask));
	setchr8(0);
	setmirror(((latch.data >> 7) & 0x01) ^ 0x01);
}

static DECLFW(M439WriteReg) {
    reg[A & 0x01] = V;
    Sync();
}

static DECLFW(M439WriteLatch) {
    uint8 mask = (reg[1] & 0x80) | ((reg[1] >> 1) & 0x38);
    Latch_Write(A, (V & ~mask) | (latch.data & mask));
}

static void M439Reset(void) {
    reg[0] = reg[1] = 0;
    Latch_RegReset();
}

static void M439Power(void) {
    reg[0] = reg[1] = 0;
    Latch_Power();
    SetWriteHandler(0x6000, 0x7FFF, M439WriteReg);
    SetWriteHandler(0x8000, 0xFFFF, M439WriteLatch);
}

void Mapper439_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
    info->Power = M439Power;
	info->Reset = M439Reset;
    AddExState(reg, 2, 0, "REGS");
}
