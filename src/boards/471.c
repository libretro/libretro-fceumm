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

static uint8 latch;

static SFORMAT stateRegs[] ={
	{ &latch, 1, "LATC" },
	{ 0 }
};

static void sync () {
	setprg32(0x8000, latch);
	setchr8(latch);
}

static DECLFW(writeLatch) {
	X6502_IRQEnd(FCEU_IQEXT);
	latch =A &0xFF;
	sync();
}

static void power(void) {
	latch =0;
	sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeLatch);
}

static void pseudoScanline () {
	X6502_IRQBegin(FCEU_IQEXT);
}

void Mapper471_Init(CartInfo *info) {
	AddExState(stateRegs, ~0, 0, 0);
	info->Power =power;
	GameHBIRQHook =pseudoScanline;
}
