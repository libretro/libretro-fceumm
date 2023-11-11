/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 * Copyright (C) 2023
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 533 is used for the Sachen 3014 board, used for the game
 * 動動腦 II: 國中英文(一) (Dòngdòngnǎo II: Guózhōng Yīngwén (I),
 * also known as Middle School English II, SA-003).
 * It's a CNROM-like board with the added ability to read back
 * the latch content for protection purposes.
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	setprg32(0x8000, 0);
	setchr8((latch.data >> 4) & 0x01);
}

static DECLFR(M533Read) {
	switch (A & 0xF000) {
	case 0xE000:
		return ((PRGptr[0][0x6000 | (A & 0xFFFF)] & 0xF0) | (latch.data >> 4));
	default:
		break;
	}
	return CartBROB(A);
}

void Mapper533_Init(CartInfo *info) {
	Latch_Init(info, Sync, M533Read, FALSE, TRUE);
}
