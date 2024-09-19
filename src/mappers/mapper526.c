/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

/* NES 2.0 Mapper 526 is used for a bootleg version of Namco's 三国志: 中原の覇者 (Sangokushi: Chūgen no Hasha).
 * Its UNIF board name is UNL-BJ-56.
 * Mirroring seems to be hard-wired (to vertical).
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_526
 */

#include "mapinc.h"

static uint8 prg[4], chr[8];
static uint32 IRQCount;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ &IRQCount, 4, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);

	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, prg[3]);

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	setmirror(MI_V);
}

static DECLFW(M526Write) {
	/*	FCEU_printf("Wr: A:%04x V:%02x\n", A, V); */
	switch (A & 0x0F) {
	case 0x00: case 0x01: case 0x02: case 0x03:
	case 0x04: case 0x05: case 0x06: case 0x07:
		chr[A & 0x07] = V;
		Sync();
		break;
	case 0x08: case 0x09: case 0x0A: case 0x0B:
		prg[A & 0x03] = V;
		Sync();
		break;
	case 0x0D:
	case 0x0F:
		/* One of these two acknowledges a pending IRQ, and the other
		 * resets to IRQ counter to zero. Because they are always written
		 * to one after the other, it's not clear which one does which. */
		X6502_IRQEnd(FCEU_IQEXT);
		IRQCount = 0;
		break;
	}
}

static void M526IRQHook(int a) {
	IRQCount += a;
	if (IRQCount & 0x1000) {
		X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void M526Close(void) {
}

static void M526Power(void) {
	prg[0] = ~3;
	prg[1] = ~2;
	prg[2] = ~1;
	prg[3] = ~0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0x800F, M526Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper526_Init(CartInfo *info) {
	info->Power = M526Power;
	info->Close = M526Close;
	MapIRQHook = M526IRQHook;
	GameStateRestore = StateRestore;
	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	AddExState(StateRegs, ~0, 0, NULL);
}
