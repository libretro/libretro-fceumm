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

static uint8 prg[4], chr[8], nt[4];
static uint8 IRQCount, IRQa;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
    { chr, 8, "CREG" },
    { nt, 4, "NTAM" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 1, "IRQC" },
	{ 0 }
};

static void Sync(void) {
    prg[3] |= 0x0C;
	setprg8(0x8000, prg[0] & 0x0F);
    setprg8(0xA000, prg[1] & 0x0F);
    setprg8(0xC000, prg[2] & 0x0F);
    setprg8(0xE000, prg[3] & 0x0F);

    setchr1(0x0000, chr[0] & 0x7F);
    setchr1(0x0400, chr[1] & 0x7F);
    setchr1(0x0800, chr[2] & 0x7F);
    setchr1(0x0C00, chr[3] & 0x7F);
    setchr1(0x1000, chr[4] & 0x7F);
    setchr1(0x1400, chr[5] & 0x7F);
    setchr1(0x1800, chr[6] & 0x7F);
    setchr1(0x1C00, chr[7] & 0x7F);

    setmirrorw(nt[0] & 0x01, nt[1] & 0x01, nt[2] & 0x01, nt[3] & 0x01);
}

static DECLFW(M127Write) {
	switch (A & 0x73) {
    case 0x00: case 0x01: case 0x02: case 0x03:
        prg[A & 3] = V;
        Sync();
        break;
    
    case 0x10: case 0x11: case 0x12: case 0x13:
    case 0x20: case 0x21: case 0x22: case 0x23:
        chr[((A >> 3) & 4) | (A & 3)] = V;
        Sync();
        break;
    
    case 0x30: case 0x31: case 0x32: case 0x33:
        IRQa = 1;
        break;
    
    case 0x40: case 0x41: case 0x42: case 0x43:
        IRQa = 0;
        IRQCount = 0;
        X6502_IRQEnd(FCEU_IQEXT);
        break;
    
    case 0x50: case 0x51: case 0x52: case 0x53:
        nt[A & 3] = V;
        Sync();
        break;
	}
}

static void M127Power(void) {
    prg[0] = prg[1] = prg[2] = prg[3] = ~0;
    chr[0] = chr[1] = chr[2] = chr[3] = 0;
    chr[4] = chr[5] = chr[6] = chr[7] = 0;
    nt[0] = nt[1] = nt[2] = nt[3] = 0;
    IRQa = IRQCount = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M127Write);
}

static void M127IRQHook(int a) {
    int count = a;
	while (count--) {
        if (IRQa) {
			IRQCount--;
            if (!IRQCount) {
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper127_Init(CartInfo *info) {
	info->Power = M127Power;
	MapIRQHook = M127IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
