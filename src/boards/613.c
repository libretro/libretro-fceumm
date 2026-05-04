/* FCE Ultra - NES/Famicom Emulator
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
 */

#include "mapinc.h"
#include "asic_mmc1.h"
#include "asic_mmc3.h"

static uint8_t reg;

static void sync (void) {
	if (reg &0x01) {
		MMC3_syncPRG(0x0F, reg <<4);
		MMC3_syncCHR(0x7F, reg <<7);
		MMC3_syncMirror();
	} else {
		MMC1_syncPRG(0x07, reg <<3);
		MMC1_syncCHR(0x1F, reg <<5);
		MMC1_syncMirror();
	}
}

static void applyMode (uint8_t clear) {
	PPU_hook = NULL;
	MapIRQHook = NULL;
	GameHBIRQHook = NULL;
	if (reg &0x01)
		MMC3_activate(clear, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, NULL);
	else
		MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
}

static void reset (void) {
	reg++;
	applyMode(1);
}

static void power (void) {
	reg = 0;
	applyMode(1);
}

static void restore (int version) {
	applyMode(0);
}

void Mapper613_Init (CartInfo *info) {
	MMC1_addExState();
	MMC3_addExState();
	info->Reset = reset;
	info->Power = power;
	GameStateRestore = restore;
	AddExState(&reg, 1, 0, "EXPR");
}
