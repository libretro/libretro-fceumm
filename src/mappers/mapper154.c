/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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

/* NES 2.0 Mapper 154 It is identical to Mapper 88, but with the addition of a single bit allowing for mapper-controlled one-screen regoring: */

#include "mapinc.h"
#include "n118.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
    { &reg, 1, "REGS" },
    { 0 }
};

static void M154Mirroring(void) {
    setmirror(MI_0 + ((reg >> 6) & 0x01));
}

static void M154FixCHR(void) {
	setchr2(0x0000, (n118.reg[0] & 0x3F) >> 1);
	setchr2(0x0800, (n118.reg[1] & 0x3F) >> 1);
	setchr1(0x1000, 0x40 | (n118.reg[2] & 0x3F));
	setchr1(0x1400, 0x40 | (n118.reg[3] & 0x3F));
	setchr1(0x1800, 0x40 | (n118.reg[4] & 0x3F));
	setchr1(0x1C00, 0x40 | (n118.reg[5] & 0x3F));
}

static DECLFW(M154Write) {
    if (A < 0xA000) {
        N118_Write(A, V);
    }
    reg = V; /* mirroring latch */
    M154Mirroring();
}

static void M154Power(void) {
    reg = 0;
    N118_Power();
    SetWriteHandler(0x8000, 0xFFFF, M154Write);
}

static void StateRestore(int version) {
    N118_FixPRG();
    N118_FixCHR();
    M154Mirroring();
}

void Mapper154_Init(CartInfo *info) {
	N118_Init(info, 0, 0);
    info->Power = M154Power;
	N118_FixCHR = M154FixCHR;
    GameStateRestore = StateRestore;
    AddExState(StateRegs, ~0, 0, NULL);
}
