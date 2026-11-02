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
#include "asic_vrc2and4.h"
#include "cartram.h"

static uint8_t reg;

static void sync0 () {
	int prgAND = reg &0x04? 0x1F: 0x0F;
	int chrAND = reg &0x02? 0x7F: 0xFF;
	int prgOR = reg <<4;
	int chrOR = reg <<7;
	VRC24_syncWRAM(0);
	VRC24_syncPRG(prgAND, prgOR &~prgAND);
	VRC24_syncCHR(chrAND, chrOR &~chrAND);
	VRC24_syncMirror();
}

static void sync1 () {
	int prgAND = reg &0x02? 0x1F: 0x0F;
	int chrAND = reg &0x01? 0x7F: 0xFF;
	int prgOR = reg <<4;
	int chrOR = reg <<8 &0x100 | reg <<6 &0x080;
	VRC24_syncWRAM(0);
	VRC24_syncPRG(prgAND, prgOR &~prgAND);
	VRC24_syncCHR(chrAND, chrOR &~chrAND);
	VRC24_syncMirror();
}

static DECLFW (writeReg) {
	if (~reg &0x08) {
		reg = A &0xFF;
		VRC24_restore(FCEU_VERSION_NUMERIC); /* Calls VRC24_cbSync() */
	}
}

static void reset () {
	reg = 0;
	VRC24_restore(FCEU_VERSION_NUMERIC); /* Calls VRC24_cbSync() */
}

static void power () {
	reg = 0;
	VRC24_power();
}

void Mapper617_Init (CartInfo *info) {
	VRC4_init(info, info->submapper == 1? sync1: sync0, 0x05, 0x0A, 1, NULL, NULL, NULL, writeReg, NULL);
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
