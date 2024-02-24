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

/* Mapper 285 - A65AS  */
/* actually, there is two cart in one... First have extra mirroring
 * mode (one screen) and 32K bankswitching, second one have only
 * 16 bankswitching mode and normal mirroring... But there is no any
 * correlations between modes and they can be used in one mapper code.
 *
 * Submapper 0 - 3-in-1 (N068)
 * Submapper 0 - 3-in-1 (N080)
 * Submapper 1 - 4-in-1 (JY-066)
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	if (latch.data & 0x40) {
		setprg32(0x8000, (latch.data >> 1) & 0x0F);
	} else {
		if (iNESCart.submapper == 1) {
			setprg16(0x8000, ((latch.data & 0x30) >> 1) | (latch.data & 0x07));
			setprg16(0xC000, ((latch.data & 0x30) >> 1) | 0x07);
		} else {
			setprg16(0x8000, latch.data & 0x0F);
			setprg16(0xC000, (latch.data & 0x0F) | 0x07);
		}
	}
	setchr8(0);
	if (latch.data & 0x80) {
		setmirror(MI_0 + (((latch.data >> 5) & 0x01)));
    } else {
		setmirror(((latch.data >> 3) & 0x01) ^ 0x01);
    }
}

void Mapper285_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
}
