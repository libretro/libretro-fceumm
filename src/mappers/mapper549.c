/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

/* NES 2.0 Mapper 549
 * Used for the Kaiser port of Meikyuu Jiin Dababa from the Famicom Disk System.
 */

#include "mapinc.h"
#include "latch.h"
#include "fdssound.h"

static void Sync(void) {
	setprg8(0x6000, ((latch.addr >> 3) & 0x04) | (latch.addr >> 2));
	setprg32(0x8000, 0x02);
	setchr8(0);
}

static void M549Power() {
	Latch_Power();
	FDSSound_Power();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
}

static void M549Reset() {
	FDSSound_Reset();
	Sync();
}

void Mapper549_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 1);
	info->Power = M549Power;
	info->Reset = M549Reset;
}
