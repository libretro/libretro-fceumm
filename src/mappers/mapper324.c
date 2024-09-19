/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

/* NES 2.0 Mapper 324
 * FARID_UNROM_8-IN-1
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_324
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	setchr8(0);
	setprg16(0x8000, ((latch.data & 0x70) >> 1) | (latch.data & 0x07));
	setprg16(0xC000, ((latch.data & 0x70) >> 1) | 0x07 );
}

static DECLFW(M324Write) {
	if ((V & 0x80) && !(latch.data & 0x80) && !(latch.data & 0x08)) {
		Latch_Write(A, V);
	} else {
		latch.data = (latch.data & ~0x07) | (V & 0x07);
		Sync();
	}
}

static void M324Power(void) {
	Latch_Power();
	SetWriteHandler(0x8000, 0xFFFF, M324Write);
}

void Mapper324_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, TRUE);
	info->Power = M324Power;
	info->Reset = Latch_RegReset;
}
