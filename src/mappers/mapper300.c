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
 *
 */

/* NES 2.0 Mapper 300 - BMC-190in1 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	setprg16(0x8000, latch.addr >> 2);
	setprg16(0xC000, latch.addr >> 2);
	setchr8(latch.addr >> 2);
	setmirror((latch.addr & 0x01) ^ 0x01);
}

void Mapper300_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
}