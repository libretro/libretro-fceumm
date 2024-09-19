/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

/* NES 2.0 Mapper 374
 * 1995 Super HiK 4-in-1 - 新系列機器戰警组合卡 (JY-022)
 * 1996 Super HiK 4-in-1 - 新系列超級飛狼組合卡 (JY-051)
 */

#include "mapinc.h"
#include "mmc1.h"

static uint8 game = 0;

static SFORMAT StateRegs[] = {
	{ &game, 1, "GAME" },
	{ 0 }
};

static void M374PRG(uint16 A, uint16 V) {
	setprg16(A, (game << 3) | (V & 0x07));
}

static void M374CHR(uint16 A, uint16 V) {
	setchr4(A, (game << 5) | (V & 0x1F));
}

static void M374Reset(void) {
	game = (game + 1) & 0x03;
	MMC1_Reset();
}

void Mapper374_Init(CartInfo *info) {
	MMC1_Init(info, 0, 0);
	MMC1_cwrap = M374CHR;
	MMC1_pwrap = M374PRG;
	info->Reset = M374Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
