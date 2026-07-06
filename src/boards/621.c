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
#include "asic_mmc1.h"

static void sync () {
	MMC1_syncPRG(0x0F, 0x00);
	MMC1_syncCHR(0x1F, 0x00);
	MMC1_syncMirror();
}

static DECLFW (writeReg) {
	if (V &0x80)
		MMC1_writeReg(0x8000, V);
	else {
		MMC1_writeReg(0x8000 | A <<13 &0x6000, V >>0 &1); MMC1_cpuCycle(2);
		MMC1_writeReg(0x8000 | A <<13 &0x6000, V >>1 &1); MMC1_cpuCycle(2);
		MMC1_writeReg(0x8000 | A <<13 &0x6000, V >>2 &1); MMC1_cpuCycle(2);
		MMC1_writeReg(0x8000 | A <<13 &0x6000, V >>3 &1); MMC1_cpuCycle(2);
		MMC1_writeReg(0x8000 | A <<13 &0x6000, V >>4 &1);
	}
}

static void power () {
	MMC1_power();
	SetWriteHandler(0x6000, 0x7FFF, writeReg);
}

void Mapper621_Init (CartInfo *info) {
	MMC1_init(info, sync, MMC1_TYPE_MMC1A, NULL, NULL, NULL, NULL);
	info->Power = power;
}
