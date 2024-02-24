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
 * FDS Conversion - Doki Doki Panic
 *
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg[3];

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, 0x0C);
	setprg8(0xE000, 0x0F);
	if (reg[2] & 0x10) {
		setprg8(0x6000, reg[0] & 0x0F);
		setprg8(0xA000, 0x0D);
		setprg8(0xC000, 0x0E);
	} else {
		setprg8r(0x10, 0x6000, 0);
		setprg4(0xA000, (0x0D << 1));
		setprg2(0xB000, (0x0D << 2) + 2);
		setprg2r(0x10, 0xB800, 4);
		setprg2r(0x10, 0xC000, 5);
		setprg2r(0x10, 0xC800, 6);
		setprg2r(0x10, 0xD000, 7);
		setprg2(0xD800, (0x0E << 2) + 3);
	}
	setchr8(0);
	setmirror(((reg[1] >> 3) & 0x01) ^ 0x01);
}

static DECLFW(M103RamWrite0) {
	WRAM[A & 0x1FFF] = V;
}

static DECLFW(M103RamWrite1) {
	WRAM[0x2000 + ((A - 0xB800) & 0x1FFF)] = V;
}

static DECLFW(M103WritePRG) {
	reg[0] = V;
	Sync();
}

static DECLFW(M103WriteMirror) {
	reg[1] = V;
	Sync();
}

static DECLFW(M103WriteRAMEnable) {
	reg[2] = V;
	Sync();
}

static void M103Power(void) {
	FDSSound_Power();
	reg[0] = reg[1] = 0;
	reg[2] = 0;
	Sync();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, M103RamWrite0);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0xB800, 0xD7FF, M103RamWrite1);
	SetWriteHandler(0x8000, 0x8FFF, M103WritePRG);
	SetWriteHandler(0xE000, 0xEFFF, M103WriteMirror);
	SetWriteHandler(0xF000, 0xFFFF, M103WriteRAMEnable);
}

static void M103Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper103_Init(CartInfo *info) {
	info->Power = M103Power;
	info->Close = M103Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 16384;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
