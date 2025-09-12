/* FCEUmm - NES/Famicom Emulator
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
 *
 *
 */

#include "mapinc.h"
#include "asic_mmc3.h"

static uint8 submapper;
static uint8 reg;
static uint8 pad;

static DECLFR (readPad) {
	return CartBR(A &~0xF | pad &0xF);
}

static void sync () {
	if (reg &(submapper == 1? 0x08: 0x10))
		setprg32(0x8000, reg >>1);
	else {
		setprg16(0x8000, reg);
		setprg16(0xC000, reg);
	}
	MMC3_syncCHR(0x7F, reg <<4 &~0x7F);
	MMC3_syncMirror();
	SetReadHandler(0x8000, 0xFFFF, reg &(submapper == 1? 0x80: 0x20)? readPad: CartBR);
}

static DECLFW (writeReg) {
	reg = A &0xFF;
	sync();
}

static void reset () {
	reg = 0;
	pad++;
	MMC3_clear();
}

static void power () {
	reg = 0;
	pad = 0;
	MMC3_power();
}

void Mapper458_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&pad, 1, 0, "DIPS");
}
