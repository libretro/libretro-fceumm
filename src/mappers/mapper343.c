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

/* NES 2.0 Mapper 343
 * BMC-RESETNROM-XIN1
 * - Sheng Tian 2-in-1(Unl,ResetBase)[p1] - Kung Fu (Spartan X), Super Mario Bros (alt)
 * - Sheng Tian 2-in-1(Unl,ResetBase)[p2] - B-Wings, Twin-bee
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	if (iNESCart.submapper == 1) {
		setprg32(0x8000, latch.data);
	} else {
		setprg16(0x8000, latch.data);
		setprg16(0xC000, latch.data);
	}
	setchr8(latch.data);
	setmirror((latch.data >> 7) & 0x01);
}

static DECLFW(M343Write) {
	Latch_Write(A, V ^ 0xFF);
}

static void M343Power(void) {
	Latch_Power();
	SetWriteHandler(0x8000, 0xFFFF, M343Write);
}

void Mapper343_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M343Power;
}
