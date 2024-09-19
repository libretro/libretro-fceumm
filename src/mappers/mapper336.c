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

/* BMC-K-3046 */
/* NES 2.0 mapper 336 is used for an 11-in-1 multicart
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_336 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	setprg16(0x8000, latch.data);
	setprg16(0xC000, latch.data | 0x07);
	setchr8(0);
}

static DECLFW(M336Write) {
    latch.addr = A;
    latch.data = V | CartBR(A);
    Sync();
}

static void M336Power(void) {
    Latch_Power();
    SetWriteHandler(0x8000, 0xFFFF, M336Write);
}

void Mapper336_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
    info->Power = M336Power;
	info->Reset = Latch_RegReset;
}
