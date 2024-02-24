/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 * NewStar 12-in-1 and 76-in-1
 */

#include "mapinc.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint8 mode = ((reg[0] >> 2) & 0x02) | ((reg[1] >> 6) & 0x01);
	uint8 base = ((reg[1] << 5) & 0x20) | ((reg[1] >> 1) & 0x18);
	uint8 bank = (reg[0] & 0x07);

	switch (mode) {
	case 0: /* UNROM */
		setprg16(0x8000, base | bank);
		setprg16(0xC000, base | 0x07);
		break;
	case 1:
		setprg16(0x8000, base | (bank & 0xFE));
		setprg16(0xC000, base | 0x07);
		break;
	case 2: /* NROM-128 */
		setprg16(0x8000, base | bank);
		setprg16(0xC000, base | bank);
		break;
	case 3: /* NROM-256 */
		setprg16(0x8000, (base | bank) & 0xFE);
		setprg16(0xC000, (base | bank) | 1);
		break;
	}
	setchr8(0);
	setmirror(((reg[1] >> 7) & 0x01) ^ 0x01);
}

static DECLFW(M293Write) {
	if (!(A & 0x2000)) {
		/* First Banking Register ($8000-$9FFF, $C000-$DFFF) */
		reg[0] = V;
	}
	if (!(A & 0x4000)) {
		/* Second Banking Register ($8000-$BFFF) */
		reg[1] = V;
	}
}

static void M293Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xDFFF, M293Write);
}

static void StateRestore(int version) {
	Sync();
}

/* BMC 12-in-1/76-in-1 (NewStar) (Unl) */
void Mapper293_Init(CartInfo *info) {
	info->Power = M293Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
