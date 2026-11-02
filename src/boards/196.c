/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2026 NewRisingSun
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

static uint8_t submapper;
static uint8_t reg;
static uint8_t pad;
static uint8_t allowPad;

static void sync () {
	if (submapper == 3)
		setprg32(0x8000, reg >>4);
	else
		MMC3_syncPRG(0xFF, 0x00);
	MMC3_syncCHR(0xFF, 0x00);
	MMC3_syncMirror();
}

static DECLFR (readPad) {
	return pad;
}

static DECLFW (writeReg) {
	reg = V;
	sync();
}

static DECLFW (interceptASICWrite) {
	switch(submapper) {
		default:
			A = A &0xF000 | (!!(A &0x000E) ^ (A &0x0001)) &0x0001;
			break;
		case 1: case 3:
			A = A &0xF000 | A >>1 &0x0001;
			break;
		case 2:
			A = A &0xF000 | A >>2 &0x0001;
			break;
	}
	MMC3_writeReg(A, V);
}

static void reset () {
	reg = 0;
	if (allowPad) pad++;
	MMC3_clear();
}

static void power () {
	reg = 0;
	pad = 0;
	MMC3_power();
	SetReadHandler(0x5000, 0x5FFF, readPad);
	SetWriteHandler(0x8000, 0xFFFF, interceptASICWrite);
}

void Mapper196_Init (CartInfo *info) {
	submapper = info->submapper;
	allowPad = info->CRC32 != 0xE0C0B5A3; /* 古今英雄伝 - The Ancient & Modern Heroes {Street Fighter II Pro} */
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&pad, 1, 0, "DIPS");
}
