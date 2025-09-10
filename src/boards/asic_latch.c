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
#include "asic_latch.h"

static void (*Latch_cbSync)();
static uint16 Latch_from, Latch_to;
static void (*Latch_cbWrite)(uint16*, uint8*, uint8);
uint16 Latch_address;
uint8 Latch_data;

static SFORMAT Latch_state[] = {
	{&Latch_address, 2, "LATA" },
	{&Latch_data,    1, "LATD" },
	{ 0 }
};

DECLFW(Latch_write) {
	uint16 newAddress = A &0xFFFF;
	if (Latch_cbWrite) Latch_cbWrite(&newAddress, &V, CartBR(A));
	Latch_address = newAddress;
	Latch_data = V;
	Latch_cbSync();
}

void Latch_clear () {
	Latch_address = 0;
	Latch_data = 0;
	Latch_cbSync();
}

static void Latch_setHandlers() {
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(Latch_from, Latch_to, Latch_write);
}

static void Latch_configure (void (*sync)(), uint16 from, uint16 to, void (*write)(uint16*, uint8*, uint8)) {
	Latch_cbSync = sync;
	Latch_from = from;
	Latch_to = to;
	Latch_cbWrite = write;
}

void Latch_activate (uint8 clear, void (*sync)(), uint16 from, uint16 to, void (*write)(uint16*, uint8*, uint8)) {
	Latch_configure(sync, from, to, write);
	Latch_setHandlers();
	if (clear)
		Latch_clear();
	else
		Latch_cbSync();
}

void Latch_addExState () {
	AddExState(Latch_state, ~0, 0, 0);
}

void Latch_restore (int version) {
	Latch_cbSync();
}

void Latch_power () {
	Latch_setHandlers();
	Latch_clear();
}

void Latch_init (CartInfo *info, void (*sync)(), uint16 from, uint16 to, void (*write)(uint16*, uint8*, uint8)) {
	Latch_addExState();
	Latch_configure(sync, from, to, write);
	info->Power = Latch_power;
	info->Reset = Latch_cbSync;
	GameStateRestore = Latch_restore;
}
