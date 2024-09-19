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

/*
 * NES 2.0 Mapper 352
 * BMC-KS106C
 * - Kaiser 4-in-1(Unl,M352)[p1] - B-Wings, Kung Fu, 1942, SMB1
 */

#include "mapinc.h"

static uint8 gameblock;

static SFORMAT StateRegs[] = {
	{ &gameblock, 1, "GAME" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, gameblock);
	setchr8(gameblock);
	setmirror(gameblock & 0x01);
}

static void M352Power(void) {
	gameblock = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void M352Reset(void) {
	gameblock++;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper352_Init(CartInfo *info) {
	info->Power = M352Power;
	info->Reset = M352Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
