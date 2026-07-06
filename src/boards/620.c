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

static uint8_t reg;

static void sync () {
	if (reg &0x40) {
		setprg16(0x8000, reg >>3 &0x0E | reg >>7 &0x01);
		setprg16(0xC000, reg >>3 &0x0E | reg >>7 &0x01);
	} else
		setprg32(0x8000, reg >>4 &0x07);
	MMC3_syncCHR(0x7F, reg <<1 &0x80);
	MMC3_syncMirror();
}

static DECLFW (writeReg) {
	if (A &0x100) {
		reg = V;
		sync();
	}
}

static void reset () {
	reg = 0x40;
	MMC3_clear();
}

static void power () {
	reg = 0x40;
	MMC3_power();
	SetWriteHandler(0x4020, 0x5FFF, writeReg);
}

void Mapper620_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
}
