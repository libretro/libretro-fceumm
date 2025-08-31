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
#include "asic_vrc1.h"

static void (*VRC1_cbSync)();
static uint8 VRC1_reg[8];

static SFORMAT VRC1_state[] = {
	{ VRC1_reg, 8, "VC1R" },
	{ 0 }
};

void VRC1_syncPRG (int AND, int OR) {
	setprg8(0x8000, VRC1_reg[0] &AND |OR);
	setprg8(0xA000, VRC1_reg[2] &AND |OR);
	setprg8(0xC000, VRC1_reg[4] &AND |OR);
	setprg8(0xE000,        0xFF &AND |OR);
}

void VRC1_syncCHR (int AND, int OR) {
	setchr4(0x0000, (VRC1_reg[6] &0x0F | VRC1_reg[1] <<3 &0x10) &AND |OR);
	setchr4(0x1000, (VRC1_reg[7] &0x0F | VRC1_reg[1] <<2 &0x10) &AND |OR);
}

void VRC1_syncMirror () {
	setmirror(VRC1_reg[1] &0x01? MI_H: MI_V);
}

DECLFW(VRC1_write) {
	VRC1_reg[A >>12 &7] = V;
	VRC1_cbSync();
}

static void VRC1_clear () {
	VRC1_reg[0] = 0x00; VRC1_reg[1] = 0; VRC1_reg[2] = 2; VRC1_reg[3] = 0;VRC1_reg[4] = 0x00; VRC1_reg[5] = 0; VRC1_reg[6] = 2; VRC1_reg[7] = 0;
	VRC1_cbSync();
}

static void VRC1_setHandlers () {
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC1_write);
}

static void VRC1_configure (void (*sync)()) {
	VRC1_cbSync = sync;
}

void VRC1_activate (uint8 clear, void (*sync)()) {
	VRC1_configure(sync);
	VRC1_setHandlers();
	if (clear)
		VRC1_clear();
	else
		VRC1_cbSync();
}

void VRC1_addExState () {
	AddExState(VRC1_state, ~0, 0, 0);
}

void VRC1_restore (int version) {
	VRC1_cbSync();
}

void VRC1_power () {
	VRC1_setHandlers();
	VRC1_clear();
}

void VRC1_init (CartInfo *info, void (*sync)()) {
	VRC1_addExState();
	VRC1_configure(sync);
	info->Power = VRC1_power;
	info->Reset = VRC1_cbSync;
	GameStateRestore = VRC1_restore;
}
