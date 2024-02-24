/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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
 * iNES Mapper 222 - (CTC-31?)
 * (VRC2 mapper)
 *
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 IRQCount;
static uint8 IRQa;

static SFORMAT StateRegs[] = {
	{ &IRQCount, 1, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ 0 }
};

static void M222IRQ(void) {
	if (IRQa) {
		IRQCount++;
		if (IRQCount >= 238) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static DECLFW(M222WriteCHR) {
	if (!(A & 0x01)) {
		VRC24_Write(A, V);
		VRC24_Write(A | 0x01, V >> 4);
	}
}

static DECLFW(M222WriteIRQ) {
	/* TODO: Update this */
	IRQa = IRQCount = V;
	if (scanline < 240)
		IRQCount -= 8;
	else
		IRQCount += 4;
	X6502_IRQEnd(FCEU_IQEXT);
}

static void M222Power(void) {
	VRC24_Power();
	SetWriteHandler(0xB000, 0xEFFF, M222WriteCHR);
	SetWriteHandler(0xF000, 0xFFFF, M222WriteIRQ);
}

void Mapper222_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, 0, 1);
	info->Power = M222Power;
	MapIRQHook = NULL;
	GameHBIRQHook = M222IRQ;
	AddExState(StateRegs, ~0, 0, NULL);
}
