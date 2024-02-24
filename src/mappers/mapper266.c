/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
 * City Fighter IV sith Sound VRC4 hacked
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 reg;
static writefunc pcm;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M266PW(uint16 A, uint16 V) {
	setprg32(0x8000, reg >> 2);
}

static DECLFW(M266Write) {
	/* FCEU_printf("%04x %02x",A,V); */
	A = (A & 0x9FFF) | ((A << 1) & 0x4000) | ((A >> 1) & 0x2000);
	VRC24_Write(A, V);
}

static DECLFW(M266WriteMisc) {
	if (A & 0x800) {
		pcm(0x4011, (V & 0x0F) << 3);
	} else {
		reg = V & 0x0C;
		VRC24_FixPRG();
	}
}

static void M266Power(void) {
	reg = 0;
	VRC24_Power();
	pcm = GetWriteHandler(0x4011);
	SetWriteHandler(0x8000, 0xFFFF, M266Write);
}

void Mapper266_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x04, 0x08, FALSE, TRUE);
	info->Power = M266Power;
	VRC24_pwrap = M266PW;
	VRC24_miscWrite = M266WriteMisc;
	AddExState(StateRegs, ~0, 0, NULL);
}
