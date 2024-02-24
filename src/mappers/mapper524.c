/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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

/* NES 2.0 Mapper 524 - BTL-900218 */
/* http://wiki.nesdev.com/w/index.php/UNIF/900218
 * NES 2.0 Mapper 524 describes the PCB used for the pirate port Lord of King or Axe of Fight.
 * UNIF board name is BTL-900218.
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint16 IRQCount;
static uint8 IRQa;

static SFORMAT IRQStateRegs[] = {
	{ &IRQCount, 2, "IRQC" },
	{ &IRQa, 1, "IRQA" },

	{ 0 }
};

static DECLFW(M524Write) {
	switch (A & 0xF00C) {
	case 0xF008:
		IRQa = 1;
		break;
	case 0xF00C:
		IRQa = 0;
		IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	}
}

static void M524IRQHook(int a) {
	if (!IRQa)
		return;

	IRQCount += a;
	if (IRQCount & 1024)
		X6502_IRQBegin(FCEU_IQEXT);
}

static void M524Power(void) {
	IRQa = IRQCount = 0;
	VRC24_Power();
	SetWriteHandler(0xF000, 0xFFFF, M524Write);
}

void Mapper524_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, 0, 1);
	info->Power = M524Power;
	MapIRQHook = M524IRQHook;
	AddExState(IRQStateRegs, ~0, 0, NULL);
}
