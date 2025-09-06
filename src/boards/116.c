/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

/* NES 2.0 Mapper 556
 * Used for the for the 餓狼傳說 激鬥篇 HiK 7-in-1 (JY-215) multicart.
 */

#include "mapinc.h"
#include "asic_mmc1.h"
#include "asic_mmc3.h"
#include "asic_vrc2and4.h"

static uint8 submapper;
static uint8 reg;
static uint8 init;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "MODE" },
	{ &init, 1, "INIT" },
	{ 0 },
};

static void sync (void) {
	int prgAND = 0x3F;
	int chrAND = 0xFF;
	int prgOR  = 0x00 &~prgAND;
	int chrOR  = reg <<6 &0x100 &~chrAND;
	if (reg &0x02) {
		prgAND >>= 1;
		prgOR >>= 1;
		chrAND >>= 2;
		chrOR >>= 2;
		MMC1_syncPRG(prgAND, prgOR);
		MMC1_syncCHR(chrAND, chrOR);
		MMC1_syncMirror();
	} else
	if (reg &0x01) {
		MMC3_syncPRG(prgAND, prgOR);
		MMC3_syncCHR(chrAND, chrOR);
		MMC3_syncMirror();
	} else {
		VRC24_syncPRG(prgAND, prgOR);
		VRC24_syncCHR(chrAND, chrOR);
		VRC24_syncMirror();
	}
}

int Huang2_getPRGBank (uint8 bank) {
	return MMC1_getPRGBank(bank) >>1;
}

static void applyMode (uint8 clear) {
	if (reg &0x02) {
		MMC1_activate(clear && init &1, sync, MMC1_TYPE_MMC1B, submapper == 2? Huang2_getPRGBank: NULL, NULL, NULL, NULL);
		MMC1_writeReg(0x8000, 0x80);
		init &= ~1;
	} else
	if (reg &0x01) {
		MMC3_activate(clear && init &2, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, NULL);
		init &= ~2;
	} else {
		VRC2_activate(clear && init &4, sync, 0x01, 0x02, NULL, NULL, NULL, NULL);
		if (init &4) {
			VRC24_writeReg(0xB000, 0xFF); VRC24_writeReg(0xB001, 0xFF); VRC24_writeReg(0xB002, 0xFF); VRC24_writeReg(0xB003, 0xFF);
			VRC24_writeReg(0xC000, 0xFF); VRC24_writeReg(0xC001, 0xFF); VRC24_writeReg(0xC002, 0xFF); VRC24_writeReg(0xC003, 0xFF);
			VRC24_writeReg(0xD000, 0xFF); VRC24_writeReg(0xD001, 0xFF); VRC24_writeReg(0xD002, 0xFF); VRC24_writeReg(0xD003, 0xFF);
			VRC24_writeReg(0xE000, 0xFF); VRC24_writeReg(0xE001, 0xFF); VRC24_writeReg(0xE002, 0xFF); VRC24_writeReg(0xE003, 0xFF);
			init &= ~4;
		}
	}
}

static DECLFW(writeReg) {
	if (A &0x100) {
		uint8 previousReg = reg;
		reg = V;
		if ((previousReg ^V) &3)
			applyMode(1);
		else
			sync();
	}
}

static void reset (void) {
	reg = 0;
	init = 7;
	applyMode(1);
}

static void power (void) {
	reg = 0;
	init = 7;
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0x5FFF, writeReg);
	applyMode(1);
}

static void restore (int version) {
	applyMode(0);
}

void UNLSL12_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC1_addExState();
	MMC3_addExState();
	VRC24_addExState();
	info->Reset = reset;
	info->Power = power;
	GameStateRestore = restore;
	AddExState(StateRegs, ~0, 0, 0);
}
