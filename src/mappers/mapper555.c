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

/* NES 2.0 Mapper 516 - Brilliant Com Cocoma Pack */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[2];
static uint8 count_expired;
static uint32 count;
static uint32 count_target = 0x20000000;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &count, 2, "CNTR" },
	{ &count_expired, 2, "CNTE" },
	{ 0 }
};

static void M555CW(uint16 A, uint16 V) {
	uint16 base = (reg[0] << 5) & 0x80;

	if ((reg[0] & 0x06) == 0x02) {
		if (V & 0x40) {
			setchr1r(0x10, A, base | (V & 0x07));
		} else {
			setchr1(A, base | (V & 0xFF));
		}
	} else {
		setchr1(A, base | (V & 0x7F));
	}
}

static void M555PW(uint16 A, uint16 V) {
	uint16 mask = ((reg[0] << 3) & 0x18) | 0x07;
	uint16 base = ((reg[0] << 3) & 0x20);

	setprg8(A, base | (V & mask));
}

static DECLFR(M555Read5) {
	if (A & 0x800) {
		return (0x5C | (count_expired ? 0x80 : 0));
	}
	return WRAM[0x2000 | (A & 0xFFF)];
}

static DECLFW(M555Write5) {
	if (A & 0x800) {
		reg[(A >> 10) & 0x01] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	} else {
		WRAM[0x2000 | (A & 0xFFF)] = V;
	}
}

static void M555Reset(void) {
	count_target = 0x20000000 | ((uint32)GameInfo->cspecial << 25);
	count = 0;
	memset(reg, 0, sizeof(reg));
	MMC3_Reset();
}

static void M555Power(void) {
	count_target = 0x20000000 | ((uint32)GameInfo->cspecial << 25);
	count = 0;
	memset(reg, 0, sizeof(reg));
	MMC3_Power();

	SetReadHandler(0x5000, 0x5FFF, M555Read5);
	SetWriteHandler(0x5000, 0x5FFF, M555Write5);

	setprg8r(0x10, 0x6000, 0);
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
}

static void M555CPUIRQHook(int a) {
	while (a--) {
		if (!(reg[0] & 0x08)) {
			count = 0;
			count_expired = false;
		} else {
			if (++count == count_target) {
				count_expired = TRUE;
			}
			if ((count % 1789773) == 0) {
				uint32 seconds = (count_target - count) / 1789773;
				FCEU_DispMessage(RETRO_LOG_INFO, 1000, "Time left: %02i:%02i\n", seconds / 60, seconds % 60);
			}
		}
	}
}

void Mapper555_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M555CW;
	MMC3_pwrap = M555PW;
	info->Power = M555Power;
	info->Reset = M555Reset;
	MapIRQHook = M555CPUIRQHook;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 16 * 1024;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, TRUE);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	CHRRAMSIZE = 8 * 1024;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, TRUE);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}
