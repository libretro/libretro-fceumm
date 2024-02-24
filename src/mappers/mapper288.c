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

/* NES 2.0 Mapper 288 is used for two GKCX1 21-in-1 multicarts
 * - 21-in-1 (GA-003)
 * - 64-in-1 (CF-015)
 */

#include "mapinc.h"
#include "latch.h"

static uint8 dipsw;

static void Sync(void) {
	setprg32(0x8000, latch.addr >> 3);
	setchr8(latch.addr);
	setmirror(((latch.addr >> 5) & 0x01) ^ 0x01);
}

static DECLFR(M288Read) {
	uint8 ret = CartBR(A);
	if (latch.addr & 0x20) {
		return CartBR(A | (dipsw & 0x0F));
	}
	return ret;
}

static void M288Reset(void) {
	dipsw++;
	Sync();
}

void Mapper288_Init(CartInfo *info) {
	Latch_Init(info, Sync, M288Read, FALSE, FALSE);
	info->Reset = M288Reset;
	AddExState(&dipsw, 1, 0, "DIPSW");
}
