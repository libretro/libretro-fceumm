/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 * Copyright (C) 2023
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

/*	晶太 YY840708C PCB
    Solely used for the "1995 Soccer 6-in-1 足球小将專輯 (JY-014)" multicart.
    MMC3+PAL16L8 combination, resulting in a bizarre mapper that switches banks in part upon *reads*.
*/

#include "mapinc.h"
#include "mmc3.h"

static uint8 pal_A15, pal_A16, pal_A1718;

static SFORMAT StateRegs[] = {
	{ &pal_A15, 1, "A15_"},
	{ &pal_A16, 1, "A16_"},
	{ &pal_A1718, 1, "A178"},
	{ 0 }
};

static void M383PW(uint16 A, uint16 V) {
	uint16 base;
	uint16 mask;

	switch (pal_A1718) {
	case 0x00:
		/* "Setting 0 provides a round-about means of dividing the first 128 KiB bank into two 32 KiB and one 64 KiB bank." */
		base = pal_A1718 | pal_A16 | (pal_A16 ? 0x00 : pal_A15);
		mask = pal_A16 ? 0x07 : 0x03;
		break;
	case 0x30:
		/* "Setting 3 provides 128 KiB MMC3 banking with the CPU A14 line fed to the MMC3 clone reversed.
		   This is used for the game Tecmo Cup: Soccer Game (renamed "Tecmo Cup Soccer"),
		   originally an MMC1 game with the fixed bank at $8000-$BFFF and the switchable bank at $C000-$FFFF,
		   a configuration that could not be reproduced with an MMC3 alone." */
		base = pal_A1718;
		mask = 0x0F;
		A ^= 0x4000;

		/* "It is also used for the menu,
		   which in part executes from PRG-ROM mapped to the CPU $6000-$7FFF address range on the MMC3 clone's fixed
		   banks alone, as no MMC3 PRG bank register is written to before JMPing to this address range." */
		if (A == 0xA000) {
			setprg8(0x6000, base | (V & 0x0B));
		}
		break;
	default:
		/* "Settings 1 and 2 provide normal 128 KiB MMC3 banking." */
		base = pal_A1718;
		mask = 0x0F;
		break;
	}

	setprg8(A, base | (V & mask));
}

static void M383CW(uint16 A, uint16 V) {
	setchr1(A, (pal_A1718 << 3) | (V & 0x7F));
}

static DECLFR(M383Read) {
	if (pal_A1718 == 0x00) { /* "PAL PRG pal_A16 is updated with the content of the corresponding MMC3 PRG bank bit by reading from the
		 respective address range, which in turn will then be applied across the entire ROM address range." */
		pal_A16 = mmc3.reg[0x06 | ((A >> 13) & 0x01)] & 0x08;
		MMC3_FixPRG();
	}
	return CartBR(A);
}

static DECLFW(M383Write) {
	if (A & 0x0100) {
		pal_A15 = (A >> 11) & 0x04;
		pal_A1718 = A & 0x30;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
	MMC3_Write(A, V);
}

static void M383Reset(void) {
	pal_A15 = 0;
	pal_A16 = 0;
	pal_A1718 = 0;
	MMC3_Reset();
}

static void M383Power(void) {
	pal_A15 = 0;
	pal_A16 = 0;
	pal_A1718 = 0;
	MMC3_Power();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xBFFF, M383Read);
	SetWriteHandler(0x8000, 0xFFFF, M383Write);
}

void Mapper383_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M383PW;
	MMC3_cwrap = M383CW;
	info->Power = M383Power;
	info->Reset = M383Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
