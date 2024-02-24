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

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static uint32 scrambleBankOrder(uint32 V, const uint8 *source, const uint8 *target, uint32 length) {
	uint32 bank = 0;
	uint32 bit = 0;

	for (bit = 0; bit < 8; bit++) {
		if (V & (0x01 << bit)) {
			uint32 index = 0;

			for (index = 0; index < length; index++) {
				if (source[index] == bit) {
					break;
				}
			}
			bank |= (0x01 << (index == length ? bit : target[index]));
		}
	}
	return bank;
}

static void M249PW(uint16 A, uint16 V) {
	static const uint8 prg_pattern[4][4] = {
		{ 3, 4, 2, 1 },
		{ 4, 3, 1, 2 },
		{ 1, 2, 3, 4 },
		{ 2, 1, 4, 3 },
	};
	uint32 bank = scrambleBankOrder(V, prg_pattern[reg & 0x03], prg_pattern[(iNESCart.mapper == 249) ? 0 : 2], 4);
	setprg8(A, bank);
}

static void M249CW(uint16 A, uint16 V) {
	static const uint8 chr_pattern[8][6] = {
		{ 5, 2, 6, 7, 4, 3 },
		{ 4, 5, 3, 2, 7, 6 },
		{ 2, 3, 4, 5, 6, 7 },
		{ 6, 4, 2, 3, 7, 5 },
		{ 5, 3, 7, 6, 2, 4 },
		{ 4, 2, 5, 6, 7, 3 },
		{ 3, 6, 4, 5, 2, 7 },
		{ 2, 5, 6, 7, 3, 4 },
	};
	uint32 bank = scrambleBankOrder(V, chr_pattern[reg & 0x07], chr_pattern[(iNESCart.mapper == 249) ? 0 : 2], 6);
	setchr1(A, bank);
}

static DECLFW(M249Write) {
	reg = V;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M249Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x5000, 0x5FFF, M249Write);
}

void Mapper249_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	MMC3_cwrap = M249CW;
	MMC3_pwrap = M249PW;
	info->Power = M249Power;
	AddExState(&reg, 1, 0, "EXPR");
}
