/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
#include "asic_vrc2and4.h"

static uint8 reg;

static void sync () {
	int prgAND = reg &0x04? 0x1F: 0x0F;
	int chrAND = reg &0x02? 0x7F: 0xFF;
	int prgOR = reg <<4;
	int chrOR = reg <<7;
	VRC24_syncPRG(prgAND, prgOR &~prgAND);
	VRC24_syncCHR(chrAND, chrOR &~chrAND);
	VRC24_syncMirror();
}

static DECLFW (writeReg) {
	if (~reg &0x08) {
		reg = A &0xFF;
		sync();
	}
}

static void reset () {
	reg = 0;
	sync();
}

static void power () {
	reg = 0;
	VRC24_power();
}

void Mapper617_Init (CartInfo *info) {
	VRC4_init(info, sync, 0x05, 0x0A, 1, NULL, NULL, NULL, writeReg, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
