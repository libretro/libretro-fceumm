/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/* NES 2.0 Mapper 353 is used for the 92 Super Mario Family multicart,
 * consisting of an MMC3 clone ASIC together with a PAL.
 * The PCB code is 81-03-05-C.
 */

#include "mapinc.h"
#include "mmc3.h"
#include "fdssound.h"

static uint8 reg;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M353PW(uint16 A, uint16 V) {
	uint16 base = reg << 5;
	uint16 mask = 0x1F;

	if (reg == 2) {
		base |= (mmc3.reg[0] & 0x80) ? 0x10 : 0x00;
		mask >>= 1;
	} else if ((reg == 3) && !(mmc3.reg[0] & 0x80) && (A >= 0xC000)) {
		base = 0x70;
		mask = 0x0F;
		V = mmc3.reg[6 + ((A >> 13) & 0x01)];
	}

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M353CW(uint16 A, uint16 V) {
	uint16 mask = 0x7F;
	uint16 base = reg << 7;

	if ((reg == 2) && (mmc3.reg[0] & 0x80)) {
		setchr8r(0x10, 0);
	} else {
		setchr1(A, (base & ~mask) | (V & mask));
	}
}

static void M353MIR(void) {
	if (reg == 0) {
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
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static DECLFW(M353Write) {
	if (A & 0x80) {
		reg = (A >> 13) & 0x03;
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	} else {
		uint8 oldcmd = mmc3.cmd;

		switch (A & 0xE001) {
		case 0x8000:
			mmc3.cmd = V;
			if ((oldcmd & 0x40) != (mmc3.cmd & 0x40)) {
				MMC3_FixPRG();
			}
			if ((oldcmd & 0x80) != (mmc3.cmd & 80)) {
				MMC3_FixCHR();
				MMC3_FixMIR();
			}
			break;
		case 0x8001:
			mmc3.reg[mmc3.cmd & 0x07] = V;
			switch (mmc3.cmd & 0x07) {
			case 0:
				MMC3_FixPRG();
				MMC3_FixCHR();
				MMC3_FixMIR();
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				MMC3_FixCHR();
				MMC3_FixMIR();
				break;
			case 6:
			case 7:
				MMC3_FixPRG();
				break;
			}
			break;
		default:
			MMC3_Write(A, V);
			break;
		}
	}
}

static void M353Power(void) {
	FDSSound_Power();
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x8000, 0xFFFF, M353Write);
}

static void M353Reset(void) {
	reg = 0;
	MMC3_Reset();
	FDSSoundRegReset();
	FDSSound_SC();
}

static void M353Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

void Mapper353_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	MMC3_cwrap = M353CW;
	MMC3_pwrap = M353PW;
	MMC3_FixMIR = M353MIR;
	info->Power = M353Power;
	info->Close = M353Close;
	info->Reset = M353Reset;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAMSIZE = 8192;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}
