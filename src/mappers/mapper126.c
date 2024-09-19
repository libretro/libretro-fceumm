/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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

/* Mapper 422: "Normal" version of the mapper. Represents UNIF boards BS-400R and BS-4040R.
 * Mapper 126: Power Joy version of the mapper, connecting CHR A18 and A19 in reverse order.
 * Mapper 534: Waixing version of the mapper, inverting the reload V of the MMC3 scanline counter.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 dipsw;
static uint8 oldump = FALSE;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M126PW(uint16 A, uint16 V) {
	uint8 reg0 = reg[0] ^ (oldump ? 0 : 0x20);
	uint16 mask = (reg0 & 0x40) ? 0x0F : 0x1F;
	uint16 base = (((reg0 << 4) & 0x70) | ((reg0 << 3) & 0x180)) & ~mask;

	switch (iNESCart.submapper) {
	case 1: base |= ((base & 0x100) >> 1); break;
	case 2: base |= ((reg[1] & 0x02) << 5); break;
	}

	if (reg[3] & 0x08) {
		uint8 b = (A >> 13) & 0x03;
		V = MMC3_GetPRGBank(b & (((reg[3] & 0x0D) == 0x0D) ? 0x02 : ((reg[3] & 0x01) ? 0 : 0x03)));
		switch (reg[3] & 0x03) {
		case 0: V = ((V << 1) & ~0x03) | (V & 0x03); break;
		case 1: V = ((V << 1) & ~0x01) | (b & 0x03); break;
		case 2: V = ((V << 2) & ~0x03) | (V & 0x03); break;
		case 3: V = ((V << 2) & ~0x03) | (b & 0x03); break;
		}
	} else {
		switch (reg[3] & 0x03) {
		case 1:
		case 2:
			base = base | (mmc3.reg[6] & mask);
			V = (A >> 13) & 0x01;
			mask = 0x01;
			break;
		case 3:
			base = base | (mmc3.reg[6] & mask);
			V = (A >> 13) & 0x03;
			mask = 0x03;
			break;
		}
	}
	setprg8(A, (base & ~mask) | (V & mask));
}

static void M126CW(uint16 A, uint16 V) {
	uint8 reg0 = reg[0] ^ (oldump ? 0 : 0x20);
	uint16 mask = (reg0 & 0x80) ? 0x7F : 0xFF;
	uint16 base = (reg0 << 4) & 0x380;

	if (iNESCart.mapper == 126) {
		base = ((reg0 << 4) & 0x080) | ((reg0 << 3) & 0x100) | ((reg0 << 5) & 0x200);
	}
	if (reg[3] & 0x10) {
		base = (((base & ~mask) >> 3) | (reg[2] & (mask >> 3))) << 3;
		V = (A >> 10) & 0x07;
		mask = 0x07;
	}
	setchr1(A, (base & ~mask) | (V & mask));
}

static void M126MIR(void) {
	if (reg[3] & 0x20) {
		setmirror(MI_0 + ((mmc3.reg[6] & 0x10) >> 4));
	} else if (reg[1] & 0x02) {
		switch (mmc3.mirr & 0x03) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static DECLFR(M126ReadDIP) {
	if (reg[1] & 0x01) {
		return CartBR((A & ~0x01) | (dipsw & 0x01));
	}
	return CartBR(A);
}

static DECLFW(M126WriteWRAM) {
	CartBW(A, V);
	if (!(reg[3] & 0x80)) {
		reg[A & 0x03] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	} else if ((A & 0x03) == 0x02) {
		const uint8 mask = 0xFF & ~(reg[2] & 0x80 ? 0xF0 : 0x00) & ~((reg[2]) >> 3 & 0x0E);
		reg[2] = (reg[2] & ~mask) | (V & mask);
		MMC3_FixCHR();
	}
}

static DECLFW(M534IRQWrite) {
	V ^= 0xFF;
	MMC3_IRQWrite(A, V);
}

static DECLFW(M126MMC3Write) {
	if ((reg[3] & 0x09) == 0x09) {
		MMC3_Write(0x8000 | (((reg[3] & 0x08) ? 0x01 : A) & 0x01), V);
	} else {
		MMC3_Write(A, V);
	}
}

static void M126Reset(void) {
	dipsw++;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Reset();
}

static void M126Power(void) {
	dipsw = 0;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M126WriteWRAM);
	SetWriteHandler(0x8000, 0xFFFF, M126MMC3Write);
	SetReadHandler(0x8000, 0xFFFF, M126ReadDIP);
	if (iNESCart.mapper == 534) {
		SetWriteHandler(0xC000, 0xDFFF, M534IRQWrite);
	}
}

static void InitCommon(CartInfo *info) {
	uint8 ws = 8;
	if (info->iNES2) {
		ws = (info->PRGRamSize + info->PRGRamSaveSize) / 1024;
	}
	MMC3_Init(info, ws, info->battery);
	MMC3_FixMIR = M126MIR;
	MMC3_cwrap = M126CW;
	MMC3_pwrap = M126PW;
	info->Power = M126Power;
	info->Reset = M126Reset;
	AddExState(StateRegs, ~0, 0, NULL);

	switch (iNESCart.CRC32) {
	case 0xEAD80031: /* Gamezone 118-in-1 (AT-207) */
	case 0x6FCBC309: /* Power Joy Classic TV Game 84-in-1 (PJ-008) */
	case 0x6D61FE21: /* 1998 4000000-in-1 (BS-400 PCB) */
	case 0x3FF46175:
	case 0xA3FF9D9B:
	case 0x2BDD0FC2:
	case 0x5789017D: /* (GD-106) 18-in-1 */
	case 0x46A01871: /* 3000000-in-1 (BS-300 PCB) */
	case 0x2466B80A: /* 700000-in-1 (BS-400 PCB) */
	case 0x871CFD16:
	case 0xB2724618:
	case 0x42A9219D:
		oldump = TRUE;
		break;
	}
}

void Mapper126_Init(CartInfo *info) {
	InitCommon(info);
}

void Mapper422_Init(CartInfo *info) {
	InitCommon(info);
}

void Mapper534_Init(CartInfo *info) {
	InitCommon(info);
}
