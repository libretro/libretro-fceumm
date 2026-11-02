/* FCEUmm - NES/Famicom Emulator: Unmarked VRC4-based 19-in-1 PCB
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

static void sync () {
	int prgAND = (reg &0x0E) == 0x04? 0x07F: 0x00F; /* 128 KiB PRG-ROM at game slots 0-6, game slot 7 is the second half of the sole 256 KiB game (#2) */
	int chrAND = (reg &0x0E) == 0x00? 0x3FF: 0x07F; /* 128 KiB CHR-ROM at game slots 0-6, game slot 7 is the second half of the sole 256 KiB game (#0) */
	int prgOR = reg <<3;
	int chrOR = reg <<6;
	VRC24_syncWRAM(0);
	VRC24_syncPRG(prgAND, prgOR &~prgAND);
	VRC24_syncCHR(chrAND, chrOR &~chrAND);
	VRC24_syncMirror();
	if (reg &0x20 && ~reg &0x10)
		VRC24_reconfigure(0x0A, 0x05);
	else
		VRC24_reconfigure(0x05, 0x0A);
}

static int getPRGBank (uint8_t bank) { /* For the 256 KiB PRG-ROM game #2, the second 128 KiB is at the very end in game slot 7 */
	int result = VRC24_getPRGBank(bank);
	return result &0x0F | (result &0x10? 0x70: 0x20);
}

static int getCHRBank (uint8_t bank) { /* For the 256 KiB CHR-ROM game #0, the second 128 KiB is at the very end in game slot 7 */
	int result = VRC24_getCHRBank(bank);
	return result &0x07F | (result &0x080? 0x380: 0x000);
}

static DECLFW (writeReg) {
	if (~reg &0x01) {
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

void Mapper635_Init (CartInfo *info) {
	VRC4_init(info, sync, 0x05, 0x0A, 1, getPRGBank, getCHRBank, NULL, writeReg, NULL);
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
