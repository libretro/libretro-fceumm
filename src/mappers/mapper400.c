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
 *
 * NES 2.0 Mapper 400 is used for retroUSB's 8-BIT XMAS 2017 homebrew cartridge.
 *
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg;
static uint8 led;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, (reg & ~0x07) | (latch.data & 0x07));
	setprg16(0xC000, (reg & ~0x07) | 0x07);
	setchr8(0);
    if (reg != 0x80) {
        setmirror(((reg >> 5) & 0x01) ^ 0x01);
    }
}

static DECLFW(M400WriteReg) {
    reg = V;
    Sync();
}

static DECLFW(M400WriteLED) {
    led = V;
}

static void M400Reset(void) {
	reg = 0x80;
	Latch_RegReset();
}

static void M400Power(void) {
    reg = 0x80;
    Latch_Power();
    SetWriteHandler(0x7800, 0x7FFF, M400WriteReg);
    SetWriteHandler(0x8000, 0xBFFF, M400WriteLED);
}

void Mapper400_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, TRUE);
	info->Power = M400Power;
    info->Reset = M400Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
