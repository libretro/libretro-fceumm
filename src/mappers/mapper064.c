/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

/* Mapper 64 - 	Tengen 800032 Rambo-1
 * Mapper 158 -	Tengen 800037 (Alien Syndrome Unl)
*/

#include "mapinc.h"

static uint8 cmd, mirr, reg[16];
static uint8 IRQReload, IRQmode, IRQCount, IRQa, IRQLatch;
static uint8 IRQPrescaler;

static SFORMAT StateRegs[] = {
	{ reg, 16, "REGS" },
	{ &cmd, 1, "CMDR" },
	{ &mirr, 1, "MIRR" },
	{ &IRQReload, 1, "IRQR" },
	{ &IRQmode, 1, "IRQM" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQPrescaler, 1, "IRQP" },
	{ 0 }
};

static void Sync(void) {
	/*
	0000: R0: Select 2 (K=0) or 1 (K=1) KiB CHR bank at PPU $0000 (or $1000)
	0001: R1: Select 2 (K=0) or 1 (K=1) KiB CHR bank at PPU $0800 (or $1800)
	0010: R2: Select 1 KiB CHR bank at PPU $1000-$13FF (or $0000-$03FF)
	0011: R3: Select 1 KiB CHR bank at PPU $1400-$17FF (or $0400-$07FF)
	0100: R4: Select 1 KiB CHR bank at PPU $1800-$1BFF (or $0800-$0BFF)
	0101: R5: Select 1 KiB CHR bank at PPU $1C00-$1FFF (or $0C00-$0FFF)
	0110: R6: Select 8 KiB PRG ROM bank at $8000-$9FFF (or $C000-$DFFF)
	0111: R7: Select 8 KiB PRG ROM bank at $A000-$BFFF
	1000: R8: If K=1, Select 1 KiB CHR bank at PPU $0400 (or $1400)
	1001: R9: If K=1, Select 1 KiB CHR bank at PPU $0C00 (or $1C00)
	1111: RF: Select 8 KiB PRG ROM bank at $C000-$DFFF (or $8000-$9FFF)
	*/
	setprg8(0x8000, reg[6]);
	setprg8(0xA000, reg[7]);
	setprg8(0xC000, reg[15]);
	setprg8(0xE000, ~0);

	if (cmd & 0x20) {
		setchr1(0x0000, reg[0]);
		setchr1(0x0400, reg[8]);
		setchr1(0x0800, reg[1]);
		setchr1(0x0C00, reg[9]);
	} else {
		setchr1(0x0000, (reg[0] & 0xFE));
		setchr1(0x0400, (reg[0] & 0xFE) | 1);
		setchr1(0x0800, (reg[1] & 0xFE));
		setchr1(0x0C00, (reg[1] & 0xFE) | 1);
	}

	setchr1(0x1000, reg[2]);
	setchr1(0x1400, reg[3]);
	setchr1(0x1800, reg[4]);
	setchr1(0x1C00, reg[5]);

	if (iNESCart.mapper == 158) {
		if (cmd & 0x20) {
			setntamem(NTARAM + ((reg[0] >> 7) << 10), 1, 0);
			setntamem(NTARAM + ((reg[8] >> 7) << 10), 1, 1);
			setntamem(NTARAM + ((reg[1] >> 7) << 10), 1, 2);
			setntamem(NTARAM + ((reg[9] >> 7) << 10), 1, 3);
		} else {
			setntamem(NTARAM + ((reg[0] >> 7) << 10), 1, 0);
			setntamem(NTARAM + ((reg[0] >> 7) << 10), 1, 1);
			setntamem(NTARAM + ((reg[1] >> 7) << 10), 1, 2);
			setntamem(NTARAM + ((reg[1] >> 7) << 10), 1, 3);
		}
	} else {
		setmirror((mirr & 1) ^ 1);
	}
}

static DECLFW(M064Write) {
	switch (A & 0xF001) {
	case 0xA000:
		mirr = V;
		Sync();
		break;
	case 0x8000:
		cmd = V;
		break;
	case 0x8001:
		reg[cmd & 0x0F] = V;
		Sync();
		break;
	case 0xC000:
		IRQLatch = V;
		if (IRQReload) {
			IRQCount = IRQLatch;
		}
		break;
	case 0xC001:
		IRQReload = TRUE;
		IRQCount = IRQLatch;
		IRQmode = V & 1;
		break;
	case 0xE000:
		IRQa = FALSE;
		X6502_IRQEnd(FCEU_IQEXT);
		if (IRQReload) {
			IRQCount = IRQLatch;
		}
		break;
	case 0xE001:
		IRQa = TRUE;
		if (IRQReload) {
			IRQCount = IRQLatch;
		}
		break;
	}
}

static void M064Power(void) {
	cmd = mirr = 0;
	reg[0] = reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = ~0;
	reg[6] = reg[7] = reg[8] = reg[9] = reg[10] = ~0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M064Write);
}

static void StateRestore(int version) {
	Sync();
}

static void M064CPUHook(int a) {
	static int32 smallcount;
	if (IRQmode) {
		smallcount += a;
		while (smallcount >= 4) {
			smallcount -= 4;
			IRQCount--;
			if (IRQCount == 0xFF)
				if (IRQa) X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void M064HBHook(void) {
	if ((!IRQmode) && (scanline != 240)) {
		IRQReload = 0;
		IRQCount--;
		if (IRQCount == 0xFF) {
			if (IRQa) {
				IRQReload = 1;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

void Mapper064_Init(CartInfo *info) {
	info->Power = M064Power;
	GameHBIRQHook = M064HBHook;
	MapIRQHook = M064CPUHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
