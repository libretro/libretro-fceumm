/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
 *  Copyright (C) 2023-2024 negativeExponent
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

/* NES 2.0 Mapper 357 is used for a 4-in-1 multicart (cartridge ID 4602) from Bit Corp.
 * The first game is Bit Corp's hack of the YUNG-08 conversion of Super Mario Brothers 2 (J) named Mr. Mary 2,
 * the other three games are UNROM games.
 *
 * Implementation is modified so reset actually sets the correct dipswitch (or outer banks) for each of the 4 games
 */

#include "mapinc.h"

static uint8 reg[4];
static uint8 dipsw;
static uint8 IRQa;
static uint16 IRQCount;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REG" },
	{ &dipsw, 1, "DPSW" },
	{ &IRQCount, 2, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ 0 }
};

static void Sync(void) {
	if (dipsw == 0) {
		static const uint8 banks[2][8] = {
			{ 4, 3, 5, 3, 6, 3, 7, 3 },
			{ 1, 1, 5, 1, 4, 1, 5, 1 }
		};
		/* SMB2J Mode */
		setprg8(0x6000, reg[1] ? 0 : 2);
		setprg8(0x8000, reg[1] ? 0 : 1);
		setprg8(0xA000, 0);
		setprg8(0xC000, banks[reg[1]][reg[0]]);
		setprg8(0xE000, reg[1] ? 8 : 10);
	} else {
		/* UNROM Mode */
		setprg16(0x8000, (dipsw << 3) | reg[2]);
		setprg16(0xc000, (dipsw << 3) | 0x07);
	}
}

static DECLFW(M357Write) {
	if (A & 0x8000) {
		reg[2] = V & 0x07;
		Sync();
	}
	if ((A & 0x71FF) == 0x4022) {
		reg[0] = V & 0x07;
		Sync();
	}
	if ((A & 0x71FF) == 0x4120) {
		reg[1] = V & 0x01;
		Sync();
	}
	if ((A & 0xF1FF) == 0x4122) {
		IRQa = V & 0x01;
		IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
	}
}

static void M357Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	reg[2] = 0;
	IRQa = IRQCount = 0;
	setchr8(0);
	setmirror(MI_V);
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0xFFFF, M357Write);
}

static void M357Reset(void) {
	reg[0] = 0;
	reg[1] = 0;
	reg[2] = 0;
	IRQa = IRQCount = 0;
	dipsw++;
	dipsw &= 3;
	setmirror((dipsw == 3) ? MI_H : MI_V);
	Sync();
}

static void M357IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount & 0x1000) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper357_Init(CartInfo *info) {
	info->Reset = M357Reset;
	info->Power = M357Power;
	MapIRQHook = M357IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
