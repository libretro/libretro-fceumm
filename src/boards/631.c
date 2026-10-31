/* FCEUmm - NES/Famicom Emulator: YS4IN1
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

static uint8_t reg;

static void sync () {
	int prgAND = 0x0F;
	int chrAND = reg &0x10? 0xFF: reg &0x20? 0x3F: 0x7F;
	int prgOR = reg >>2 &0x30;
	int chrOR = reg <<7 &0x180 | reg <<1 &0x40;
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	reg = V;
	sync();
}

static void reset () {
	reg = 0;
	MMC3_clear();
}

static void power () {
	reg = 0;
	MMC3_power();
}

void Mapper631_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
