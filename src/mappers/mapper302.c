/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2011 CaH4e3
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
 *
 * NES 2.0 Mapper 302 - UNL-KS7057
 * FDS Conversion
 */

#include "mapinc.h"
#include "vrc2and4.h"

static void M302PW(uint16 A, uint16 V) {
	setprg8( 0x8000, 0x00);
	setprg8( 0xA000, 0x0D);
	setprg16(0xC000, 0x07);
}

static void M302CW(uint16 A, uint16 V) {
	setchr8(0);
}

static DECLFR(M302Read) {
	uint8 bank = (((A - 0x6000) >> 11) & 0x06) | ((A >> 11) & 0x01);

	return PRGptr[0][(vrc24.chr[bank ^ 4] << 11) | (A & 0x07FF)];
}

static void M302Power(void) {
	VRC24_Power();
	SetReadHandler(0x6000, 0x9FFF, M302Read);
}

void Mapper302_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, FALSE, TRUE);
	info->Power = M302Power;
	VRC24_pwrap = M302PW;
	VRC24_cwrap = M302CW;;
}
