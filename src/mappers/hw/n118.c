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
#include "n118.h"

void (*N118_FixPRG)(void);
void (*N118_FixCHR)(void);

void (*N118_pwrap)(uint16 A, uint16 V);
void (*N118_cwrap)(uint16 A, uint16 V);

N118 n118;

static SFORMAT StateRegs[] = {
	{ n118.reg, 8, "REGS" },
    { &n118.cmd, 1, "CMD0" },
	{ 0 }
};

static void GENCWRAP(uint16 A, uint16 V) {
    setchr1(A, V & 0x3F);
}

static void GENPWRAP(uint16 A, uint16 V) {
    setprg8(A, V & 0x0F);
}

static void GENFIXCHR(void) {
    N118_cwrap(0x0000, n118.reg[0] & (~1));
    N118_cwrap(0x0400, n118.reg[0] |  1);
    N118_cwrap(0x0800, n118.reg[1] & (~1));
    N118_cwrap(0x0C00, n118.reg[1] |  1);
    N118_cwrap(0x1000, n118.reg[2]);
    N118_cwrap(0x1400, n118.reg[3]);
    N118_cwrap(0x1800, n118.reg[4]);
    N118_cwrap(0x1C00, n118.reg[5]);
}

static void GENFIXPRG(void) {
    N118_pwrap(0x8000, n118.reg[6]);
    N118_pwrap(0xA000, n118.reg[7]);
    N118_pwrap(0xC000, ~1);
    N118_pwrap(0xE000, ~0);
}

DECLFW(N118_Write) {
	if (A & 0x01) {
        n118.reg[n118.cmd & 0x07] = V;
    } else {
		n118.cmd = V;
	}
    N118_FixPRG();
    N118_FixCHR();
}

void N118_Power(void) {
	n118.reg[0] = 0;
	n118.reg[1] = 2;
	n118.reg[2] = 4;
	n118.reg[3] = 5;
	n118.reg[4] = 6;
	n118.reg[5] = 7;
	n118.reg[6] = 0;
	n118.reg[7] = 1;
	n118.cmd = 0;

    N118_FixPRG();
    N118_FixCHR();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0x9FFF, N118_Write);

    if (WRAM) {
        SetReadHandler(0x6000, 0x7FFF, CartBR);
	    SetWriteHandler(0x6000, 0x7FFF, CartBW);
        FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
    }
}

static void StateRestore(int version) {
	N118_FixPRG();
    N118_FixCHR();
}

void N118_Init(CartInfo *info, int wsize, int battery) {
    N118_FixPRG = GENFIXPRG;
    N118_FixCHR = GENFIXCHR;

    N118_pwrap = GENPWRAP;
    N118_cwrap = GENCWRAP;

    WRAMSIZE = wsize * 1024;

    if (WRAMSIZE) {
        WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
        SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
        AddExState(WRAM, WRAMSIZE, 0, "WRAM");

        if (battery) {
            info->SaveGame[0] = WRAM;
            info->SaveGameLen[0] = WRAMSIZE;
        }
    }

	info->Power = N118_Power;

	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
