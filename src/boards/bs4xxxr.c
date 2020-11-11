/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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

/* originally submitted by dragon2snow
 * Known carts:
 * UNIF BMC-BS-400R  : 700000-in-1 (BS-400R)(Unl), PRG 1024K CHR 1024K CRC 0x5DBDFAA0
 * UNIF BMC-BS-4040R : Double Dragon 310000-in-1 (4040R) PRG 512 CHR 512 CRC 0x3A642F31
 *
 * FIXME: Future Kids notes
 * Either something is wrong or missing because chr banks for the game is $E8 - EB
 * but game uses $EC-EF here. These are chr banks for the next game and does not masked properly
 * by mapper code.
 *                                                                                      ~ c@2020
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 pointer;
static uint8 offset;

static int getPRGBankBS4XXXR(int bank)
{
	if (((~bank) & 1) && (pointer & 0x40)) bank ^= 2;
	return ((bank & 2) ? 0xFE | (bank & 1) : DRegBuf[6 | (bank & 1)]);
}

static void BS4XXXRPW(uint32 A, uint8 V)
{
	if ((EXPREGS[3] >> 4) & 1) {
		uint8 prgAND = ((EXPREGS[0] >> 1) & 1) ? 0x0F : 0x0F;
		uint8 prgOR = (EXPREGS[0] & 7) << 4;
		uint8 bank0 = getPRGBankBS4XXXR(0);
		uint8 bank1 = getPRGBankBS4XXXR(1);
		/* 16K Mode */
		if (!((EXPREGS[3] >>1 )& 0x1)) {
			setprg8(0x8000, (bank0 & prgAND) | prgOR);
			setprg8(0xA000, (bank1 & prgAND) | prgOR);
			setprg8(0xC000, (bank0 & prgAND) | prgOR);
			setprg8(0xE000, (bank1 & prgAND) | prgOR);
		}
		/* 32K Mode */
		else {
			setprg8(0x8000, (bank0 & prgAND) | prgOR);
			setprg8(0xA000, (bank1 & prgAND) | prgOR);
			setprg8(0xC000, ((bank0 | 2) & prgAND) | prgOR);
			setprg8(0xE000, ((bank1 | 2) & prgAND) | prgOR);
		}
	}
	/* Mmc3 Mode */
	else {
		uint8 prgAND = ((EXPREGS[0] >> offset) & 1) ? 0x0F : 0x1F; /* 4040R 6,400R 1 */
		uint8 prgOR = (EXPREGS[0] & 7) << 4;
		setprg8(A, (V & prgAND) | prgOR);
	}
}

static void BS4XXXRCW(uint32 A, uint8 V) {
	if ((EXPREGS[3] >> 4) & 1) {
		uint8 chrAND = ((EXPREGS[0] >> 1) & 1) ? 0x0F : 0x0F;
		uint8 chrOR = ((EXPREGS[0] >> 3) & 7) << 4;
		/* FIXME: Ugly hack for Future Kids */
		if (EXPREGS[4]) {
			chrAND = 0x03;
			chrOR |= 8;
		}
		setchr8((EXPREGS[2] & chrAND) | chrOR);
	} else {
		uint8 chrAND = ((EXPREGS[0] >> 1) & 1) ? 0xFF : 0xFF;
		uint16 chrOR = ((EXPREGS[0] >> 3) & 7) << 7;
		setchr1(A, (V & chrAND) | chrOR);
	}
}

static DECLFW(BS4XXXRHiWrite) {
	/* FCEU_printf("Wr8B: A:%04x V:%02x\n", A, V); */
	if (A == 0x8000)
		pointer = MMC3_cmd ^ V;
	MMC3_CMDWrite(A, V);
}

static DECLFW(BS4XXXRLoWrite) {
	/* FCEU_printf("WrLo: A:%04x V:%02x\n", A, V); */
	if (A & 0x800) {
		/* Future Kids hack */
		if (!EXPREGS[4]) {
			if ((((A & 3) == 2) && (V == 0xE8)))
				EXPREGS[4] = 1;
		}
		if (!(EXPREGS[3] & 0x80)) {
			EXPREGS[A & 0x03] = V;
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
		}
		else if (EXPREGS[3] & 0x10) {
			EXPREGS[A & 0x03] = V;
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
		}
	}
	CartBW(A, V);
}

static void BS4XXXRReset(void) {
	EXPREGS[4] = 0;
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	MMC3RegReset();
}

static void BS4XXXRPower(void) {
	EXPREGS[4] = 0;
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, BS4XXXRLoWrite);
	SetWriteHandler(0x8000, 0xBFFF, BS4XXXRHiWrite);
}

void BS4XXXR_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, 0);
	cwrap = BS4XXXRCW;
	pwrap = BS4XXXRPW;

	info->Power = BS4XXXRPower;
	info->Reset = BS4XXXRReset;

	AddExState(EXPREGS, 4, 0, "EXPR");
	AddExState(&pointer, 1, 0, "PNTR");
	AddExState(&offset, 1, 0, "OFFS");
}

void BS400R_Init(CartInfo *info) {
	offset = 1;
	BS4XXXR_Init(info);
}

void BS4040R_Init(CartInfo *info) {
	offset = 6;
	BS4XXXR_Init(info);
}
