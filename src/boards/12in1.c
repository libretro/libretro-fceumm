/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
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
 * 7-in-1  Darkwing Duck, Snake, MagicBlock (PCB marked as "12 in 1")
 * 12-in-1 1991 New Star Co. Ltd.
 *
 */

#include "mapinc.h"

static uint8 prgchr[2], ctrl;
static SFORMAT StateRegs[] =
{
	{ prgchr, 2, "REGS" },
	{ &ctrl, 1, "CTRL" },
	{ 0 }
};

static void BMC12IN1Sync(void) {
	uint8 bank = (ctrl & 3) << 3;
	setchr4(0x0000, (prgchr[0] >> 3) | (bank << 2));
	setchr4(0x1000, (prgchr[1] >> 3) | (bank << 2));
	if (ctrl & 8) {
		setprg16(0x8000, bank | (prgchr[0] & 6) | 0);   /* actually, both 0 and 1 registers used, but they will switch each PA12 transition */
		setprg16(0xc000, bank | (prgchr[0] & 6) | 1);   /* if bits are different for both registers, so they must be programmed strongly the same! */
	} else {
		setprg16(0x8000, bank | (prgchr[0] & 7));
		setprg16(0xc000, bank | 7 );
	}
	setmirror(((ctrl & 4) >> 2) ^ 1);
}

static void BMC12IN1Write(uint32 A, uint8 V) {
	switch (A & 0xE000) {
	case 0xA000: prgchr[0] = V; BMC12IN1Sync(); break;
	case 0xC000: prgchr[1] = V; BMC12IN1Sync(); break;
	case 0xE000: ctrl = V & 0x0F; BMC12IN1Sync(); break;
	}
}

static void BMC12IN1Power(void) {
	prgchr[0] = prgchr[1] = ctrl = 0;
	BMC12IN1Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, BMC12IN1Write);
}

static void BMC12IN1StateRestore(int version) {
	BMC12IN1Sync();
}

void BMC12IN1_Init(CartInfo *info) {
	info->Power = BMC12IN1Power;
	GameStateRestore = BMC12IN1StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

