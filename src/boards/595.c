/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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
#include "cartram.h"
#include "flashrom.h"

static uint8 reg;

static void sync () {
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, reg &0x1F);
	setprg16(0xC000, 0xFF);
	setchr8(0);
}

static DECLFW (writeFlash) {
	flashrom_write(A &0x3FFF | (Page[A >>11] +A -PRGptr[0]) &~0x3FFF, V);
}

static DECLFW (writeReg) {
	reg = reg >>1 | V <<4 &0x10;
	sync();
}

static void power () {
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xFFFF, flashrom_read);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xBFFF, writeFlash);
	SetWriteHandler(0xC000, 0xFFFF, writeReg);
	reg = 0;
	sync();
}

static void stateRestore (int version) {
	sync();
}

void Mapper595_Init (CartInfo *info) {
	WRAM_init(info, 8);
	flashrom_init (0xBF, 0xB4, 4096, 0x5555, 0x2AAA, 0x7FFF);
	info->Reset = power;
	info->Power = power;
	MapIRQHook =flashrom_cpuCycle;
	GameStateRestore = stateRestore;
	AddExState(&reg, 1, 0, "REGS");
	if (info->battery) {
		info->SaveGame[0] = ROM;
		info->SaveGameLen[0] = PRGsize[0];
		AddExState(ROM, PRGsize[0], 0, "FLAS");
	}
}
