/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

static uint8 preg[4], creg[8];
static uint32 IRQCount;
static uint8 *WRAM = NULL;

#define UNLBJ56_WRAMSIZE 8192

static SFORMAT StateRegs[] =
{
	{ preg, 4, "PREG" },
	{ creg, 8, "CREG" },
	{ &IRQCount, 4, "IRQC" },
	{ 0 }
};

static void UNLBJ56Sync(void) {
	uint8 i;
	setprg8r(0x10, 0x6000, 0);
	setprg8(0x8000, preg[0]);
	setprg8(0xA000, preg[1]);
	setprg8(0xC000, preg[2]);
	setprg8(0xE000, preg[3]);
	for (i = 0; i < 8; i++)
		setchr1((i << 10), creg[i]);
	setmirror(MI_V);
}

static void UNLBJ56Write(uint32 A, uint8 V) {
	A &= 0xF00F;
	if (A <= 0x8007) {
		creg[A & 0x07] = V;
		UNLBJ56Sync();
	} else if (A <= 0x800B) {
		preg[A & 0x03] = V;
		UNLBJ56Sync();
	} else {
		switch (A & 0x0F) {
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
}

static void UNLBJ56IRQHook(int a) {
	IRQCount += a;
	if (IRQCount & 4096)
		X6502_IRQBegin(FCEU_IQEXT);
}

static void UNLBJ56Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void UNLBJ56Power(void) {
	preg[0] = ~3;
	preg[1] = ~2;
	preg[2] = ~1;
	preg[3] = ~0;
	UNLBJ56Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0x800F, UNLBJ56Write);
	FCEU_CheatAddRAM(UNLBJ56_WRAMSIZE >> 10, 0x6000, WRAM);
}

static void UNLBJ56StateRestore(int version) {
	UNLBJ56Sync();
}

void UNLBJ56_Init(CartInfo *info) {
	info->Power = UNLBJ56Power;
	info->Close = UNLBJ56Close;
	MapIRQHook = UNLBJ56IRQHook;
	GameStateRestore = UNLBJ56StateRestore;
	WRAM = (uint8*)FCEU_gmalloc(UNLBJ56_WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, UNLBJ56_WRAMSIZE, 1);
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = UNLBJ56_WRAMSIZE;
	}
	AddExState(WRAM, UNLBJ56_WRAMSIZE, 0, "WRAM");
	AddExState(&StateRegs, ~0, 0, 0);
}
