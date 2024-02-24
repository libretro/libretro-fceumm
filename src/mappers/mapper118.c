/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

#include "mapinc.h"
#include "mmc3.h"

static void M118MIR(void) {
	if (mmc3.cmd & 0x80) {
		setntamem(NTARAM + 0x400 * ((mmc3.reg[2] >> 7) & 0x01), 1, 0);
		setntamem(NTARAM + 0x400 * ((mmc3.reg[3] >> 7) & 0x01), 1, 1);
		setntamem(NTARAM + 0x400 * ((mmc3.reg[4] >> 7) & 0x01), 1, 2);
		setntamem(NTARAM + 0x400 * ((mmc3.reg[5] >> 7) & 0x01), 1, 3);
	} else {
		setntamem(NTARAM + 0x400 * ((mmc3.reg[0] >> 7) & 0x01), 1, 0);
		setntamem(NTARAM + 0x400 * ((mmc3.reg[0] >> 7) & 0x01), 1, 1);
		setntamem(NTARAM + 0x400 * ((mmc3.reg[1] >> 7) & 0x01), 1, 2);
		setntamem(NTARAM + 0x400 * ((mmc3.reg[1] >> 7) & 0x01), 1, 3);
	}
}

static DECLFW(M118Write) {
	switch (A & 0xE001) {
	case 0x8001:
		switch (mmc3.cmd & 0x07) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			mmc3.reg[mmc3.cmd & 0x07] = V;
			MMC3_FixCHR();
			MMC3_FixMIR();
			break;
		default:
			MMC3_CMDWrite(A, V);
			break;
		}
	case 0xA000:
		/* MMC3 mirroring not used in favor of nametable mirroring */
		break;
	default:
		MMC3_CMDWrite(A, V);
		break;
	}
}

static void M118Power(void) {
	MMC3_Power();
	SetWriteHandler(0x8000, 0xBFFF, M118Write);
}

void Mapper118_Init(CartInfo *info) {
	uint8 ws = info->iNES2 ? (info->PRGRamSize + info->PRGRamSaveSize) / 1024 : 8;
	MMC3_Init(info, ws, info->battery);
	info->Power = M118Power;
	MMC3_FixMIR = M118MIR;
}
