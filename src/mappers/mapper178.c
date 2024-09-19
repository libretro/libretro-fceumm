/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2013 CaH4e3
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
 * DSOUNDV1/FL-TR8MA boards (32K WRAM, 8/16M), 178 mapper boards (8K WRAM, 4/8M)
 * Various Education Cartridges
 * 
 * mapper 551
 * Compared to INES Mapper 178, mirroring is hard-wired, and the chipset's internal CHR-RAM is not used in favor of CHR-ROM.
 *
 */

#include "mapinc.h"

static uint8 reg[4];

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint16 base = (reg[1] & 0x07) | (reg[2] << 3);

	if ((reg[0] & 0x02)) {
		setprg16(0x8000, base);
		setprg16(0xC000, base | ((reg[0] & 0x04) ? 0x06 : 0x07));
	} else {
		if (reg[0] & 0x04) {
			setprg16(0x8000, base);
			setprg16(0xC000, base);
		} else {
			setprg32(0x8000, base >> 1);
		}
	}

	if (iNESCart.mapper == 551) {
		setprg8r(0x10, 0x6000, 0);
		setchr8(reg[3]);
	} else {
		setchr8(0);
		setprg8r(0x10, 0x6000, reg[3] & 3);
		setmirror((reg[0] & 1) ^ 1);
	}
}

static DECLFW(M178Write) {
	reg[A & 3] = V;
/*	FCEU_printf("cmd %04x:%02x\n", A, V); */
	Sync();
}

static void M178Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	Sync();
	SetWriteHandler(0x4800, 0x4FFF, M178Write);
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M178Reset(void) {
	/* Always reset to menu */
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	Sync();
}

static void M178Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper178_Init(CartInfo *info) {
	info->Power = M178Power;
	info->Reset = M178Reset;
	info->Close = M178Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
}
