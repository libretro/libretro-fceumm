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

/* Map 78 */
/* Should be two separate emulation functions for this "mapper".  Sigh.  URGE TO KILL RISING. */
/* Submapper 1 - Uchuusen - Cosmo Carrier ( one-screen mirroring ) */
/* Submapper 3 - Holy Diver ( horizontal/vertical mirroring ) */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	setprg16(0x8000, latch.data & 0x07);
	setprg16(0xc000, ~0);
	setchr8(latch.data >> 4);
	if (iNESCart.submapper == 3) {
		setmirror((latch.data >> 3) & 0x01);
	} else {
		setmirror(MI_0 + ((latch.data >> 3) & 0x01));
	}
}

void Mapper078_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, TRUE);
}
