/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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

#include "mapinc.h"
#include "latch.h"

static uint8 smb2j_reg;
static uint8 IRQa;
static uint16 IRQCount;

static SFORMAT StateRegs[] = {
	{ &smb2j_reg, 1, "SMB2" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 2, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	uint8 prg = ((latch.data >> 1) & 0x04) | ((latch.data >> 6) & 0x02) | ((latch.data >> 5) & 0x01);

	if (latch.addr >= 0xA000) {
		/* latch should not respond to $A000-$FFFF */
		return;
	}

	setprg8(0x6000, 0x07);
	if (latch.data & 0x08) {
		switch (latch.data & 0xC0) {
		case 0x00:
			setprg8(0x8000, prg << 1);
			setprg8(0xA000, prg << 1);
			setprg8(0xC000, prg << 1);
			setprg8(0xE000, prg << 1);
			break;
		case 0x40:
			setprg16(0x8000, prg);
			setprg16(0xC000, prg);
			break;
		case 0x80:
		case 0xC0:
			setprg32(0x8000, prg >> 1);
			break;
		}
	} else {
		setprg8(0x8000, 0x00);
		setprg8(0xA000, 0x01);
		setprg8(0xC000, smb2j_reg);
		setprg8(0xE000, 0x03);
	}
	setchr8((latch.data >> 1) & 0x03);
	setmirror(((latch.data >> 2) & 0x01) ^ 0x01);
}

static DECLFW(M416WriteReg) {
	if ((A & 0x20) && !(A & 0x40)) {
		if (A & 0x100) {
			IRQa = V & 0x01;
			if (!IRQa) {
				IRQCount = 0;
				X6502_IRQEnd(FCEU_IQEXT);
			}
		} else {
			smb2j_reg = (V & ~0x07) | ((V << 2) & 0x04) | ((V >> 1) & 0x03);
			Sync();
		}
	}
}

static void M416Power(void) {
	smb2j_reg = IRQa = IRQCount = 0;
	Latch_Power();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x4020, 0x5FFF, M416WriteReg);
}

static void M416IRQHook(int a) {
	if (IRQa & 0x01) {
		IRQCount += a;
		if (IRQCount & 0x1000) {
			X6502_IRQBegin(FCEU_IQEXT);
		} else {
			X6502_IRQEnd(FCEU_IQEXT);
		}
	}
}

void Mapper416_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M416Power;
	MapIRQHook = M416IRQHook;
	AddExState(StateRegs, ~0, 0, NULL);
}
