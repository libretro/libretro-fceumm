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

/* NES 2.0 Mapper 308 - UNL-TH2131-1 */
/* https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_308
 * NES 2.0 Mapper 308 is used for a bootleg version of the Sunsoft game Batman
 * similar to Mapper 23 Submapper 3) with custom IRQ functionality.
 * UNIF board name is UNL-TH2131-1.
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 IRQLatch, IRQa;
static uint16 IRQCount;

static SFORMAT IRQStateRegs[] = {
	{ &IRQCount, 2, "IRQC" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQa, 1, "IRQA" },

	{ 0 }
};

static DECLFW(M308Write) {
	switch (A & 0xF003) {
	case 0xF000: X6502_IRQEnd(FCEU_IQEXT); IRQa = 0; IRQCount = 0; break;
	case 0xF001: IRQa = 1; break;
	case 0xF003: IRQLatch = (V & 0xF0) >> 4; break;
	}
}

static void M308IRQHook(int a) {
	if (IRQa) {
		while (a--) {
			IRQCount++;
			if ((IRQCount & 0x0FFF) == 2048) {
				IRQLatch--;
			}
			if (!IRQLatch && (IRQCount & 0x0FFF) < 2048) {
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void M308Power(void) {
	IRQa = IRQCount = IRQLatch = 0;
	VRC24_Power();
	SetWriteHandler(0xF000, 0xFFFF, M308Write);
}

void Mapper308_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, 0, 1);
	info->Power = M308Power;
	MapIRQHook = M308IRQHook;
	AddExState(IRQStateRegs, ~0, 0, NULL);
}
