/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020 negativeExponent
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

/* NES 2.0 Mapper 534 - 2-in-1 数独/五子棋 (Sudoku/Gomoku, NJ064) */

#include "mapinc.h"
#include "mmc3.h"

static uint32 GetPRGBank(uint32 bank)
{
	if (~bank & 1 && (MMC3_cmd & 0x40)) bank ^= 2;
	return (bank & 2) ? (0xFE | (bank & 1)) : DRegBuf[6 | (bank & 1)];
}

void SyncPRG_GNROM(int A14, int AND, int OR) {
	setprg8(0x8000, ((GetPRGBank(0) & ~A14) & AND) | OR);
	setprg8(0xA000, ((GetPRGBank(1) & ~A14) & AND) | OR);
	setprg8(0xC000, ((GetPRGBank(0) |  A14) & AND) | OR);
	setprg8(0xE000, ((GetPRGBank(1) |  A14) & AND) | OR);
}

static void M534PW(uint32 A, uint8 V) {
	if (EXPREGS[0] & 0x40)
		SyncPRG_GNROM(EXPREGS[3] & 0x02, 0x0F, ((EXPREGS[0] & 3) << 4));
	else
		setprg8(A, (V & 0x1F) | ((EXPREGS[0] & 0x2) << 4));
}

static void M534CW(uint32 A, uint8 V) {
	setchr1(A, (V & 0xFF) | ((EXPREGS[2] & 0x0F) << 3) | ((EXPREGS[0] & 0x18) << 4));
}

static DECLFW(M534IRQWrite) {
	MMC3_IRQWrite(0xC000 | (A & 1), V ^ 0xFF);
}

static DECLFW(M534WriteLo) {
	if ((A & 0x800) && (!(EXPREGS[3] & 0x80) || (A & 3) == 2)) {
		EXPREGS[A & 3] = V;
		FixMMC3CHR(MMC3_cmd);
		FixMMC3PRG(MMC3_cmd);
	}
}

static void M534Power(void) {
	EXPREGS[0] = 0x00;
	EXPREGS[1] = 0x00;
	EXPREGS[2] = 0x00;
	EXPREGS[3] = 0x00;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x6FFF, M534WriteLo);
	SetWriteHandler(0xC000, 0xDFFF, M534IRQWrite);
}

static void M534Reset(void) {
	EXPREGS[0] = 0x00;
	EXPREGS[1] = 0x00;
	EXPREGS[2] = 0x00;
	EXPREGS[3] = 0x00;
	EXPREGS[4] = (EXPREGS[4] + 1) & 7;
	FCEU_printf("dipswitch = %d\n", EXPREGS[4]);
	MMC3RegReset();
}

void Mapper534_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 512, 0, 0);
	pwrap = M534PW;
	cwrap = M534CW;
	info->Power = M534Power;
	info->Reset = M534Reset;
	AddExState(EXPREGS, 5, 0, "EXPR");
}
