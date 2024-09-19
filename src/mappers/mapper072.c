/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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
 * Mapper 72:
 * Moero!! Pro Tennis have ADPCM codec on-board, PROM isn't dumped, emulation isn't
 * possible just now.
 *
 * Mapper 092
 * Another two-in-one mapper, two Jaleco carts uses similar
 * hardware, but with different wiring.
 * Original code provided by LULU
 * Additionally, PCB contains DSP extra sound chip, used for voice samples (unemulated)
 * This mapper is identical to mapper 072 except for the different PRG Setup.
 */

#include "mapinc.h"

static uint8 prg, chr, reg;

static SFORMAT StateRegs[] = {
	{ &prg, 1, "PREG" },
	{ &chr, 1, "CREG" },
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	if (iNESCart.mapper == 92) {
		setprg16(0x8000, 0);
		setprg16(0xC000, prg);
		setchr8(chr);
	} else {
		setprg16(0x8000, prg);
		setprg16(0xC000, ~0);
		setchr8(chr);
	}
}

static DECLFW(M072Write) {
	V &= CartBR(A); /* bus conflict */

	reg = (reg ^ V) & V;
	if (reg & 0x80) prg = V;
	if (reg & 0x40) chr = V;

	Sync();
}

static void M072Power(void) {
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M072Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper072_Init(CartInfo *info) {
	info->Power = M072Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
