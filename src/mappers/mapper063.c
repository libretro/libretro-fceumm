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

/* Mapper 63 NTDEC-Multicart
 * http://wiki.nesdev.com/w/index.php/INES_Mapper_063
 * - Powerful 250-in-1
 * - Hello Kitty 255-in-1 */

#include "mapinc.h"
#include "latch.h"

static uint16 openBus = 0;

static void Sync(void) {
	uint8 prg_mask = (iNESCart.submapper == 0) ? 0xFF : 0x7F;
	uint8 prg_bank = (latch.addr >> 2) & prg_mask;
	uint8 chr_protect = (latch.addr & ((iNESCart.submapper == 0) ? 0x400 : 0x200)) == 0;

	/* return openbus for unpopulated rom banks */
	openBus = prg_bank >= ROM.prg.size;

	if (latch.addr & 2) {
		setprg32(0x8000, prg_bank >> 1);
	} else {
		setprg16(0x8000, prg_bank);
		setprg16(0xC000, prg_bank);
	}

	setchr8(0);
	setmirror((latch.addr & 1) ^ 1);

	/* chr-ram protect */
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, chr_protect);
}

static DECLFR(M063Read) {
	if (openBus) {
		return cpu.openbus;
	}
	return CartBR(A);
}

void Mapper063_Init(CartInfo *info) {
	Latch_Init(info, Sync, M063Read, FALSE, FALSE);
	info->Reset = Latch_RegReset;
}
