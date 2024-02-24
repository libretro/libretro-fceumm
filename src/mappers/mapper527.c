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

/* NES 2.0 Mapper 527 is used for a bootleg version of
 * Taito's 不動明王伝 (Fudō Myōō Den).
 * Its UNIF board name is UNL-AX-40G. The original INES Mapper 207 is
 * replaced with a VRC2 clone (A0/A1, i.e. VRC2b) while retaining
 * Mapper 207's extended mirroring.
 */

#include "mapinc.h"
#include "vrc2and4.h"

static void M527CW(uint16 A, uint16 V) {
	setchr1(A, V);
	setmirrorw((vrc24.chr[0] >> 7) & 1, (vrc24.chr[0] >> 7) & 1, (vrc24.chr[1] >> 7) & 1, (vrc24.chr[1] >> 7) & 1);
}

void Mapper527_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, 0, 1);
	VRC24_cwrap = M527CW;
}
