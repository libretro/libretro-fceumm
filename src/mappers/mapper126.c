/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
 *  Copyright (C) 2023
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
 * Mapper 534: Waixing version of the mapper, inverting the reload value of the MMC3 scanline counter.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M126PW(uint32 A, uint8 V) {
	uint16 mask = (reg[0] & 0x40) ? 0x0F : 0x1F; /* 128 KiB or 256 KiB inner PRG bank selection */
	uint16 base = (((reg[0] << 4) & 0x70) | ((reg[0] << 3) & 0x180)) & ~mask; /* outer PRG bank */

	switch (iNESCart.submapper) {
	case 1: /* uses PRG A21 as a chip select between two 1 MiB chips. */
		base |= ((base & 0x100) >> 1);
		break;
	case 2: /* uses $6001 bit 2 as a chip select between two 1 MiB chips. */
		base |= ((reg[1] & 0x02) << 5);
		break;
	}

	if (reg[3] & 0x03) {
		if ((reg[3] & 0x03) == 0x03) {
			setprg32(0x8000, (base | (mmc3.reg[6] & mask)) >> 2);
		} else {
			setprg16(0x8000, (base | (mmc3.reg[6] & mask)) >> 1);
			setprg16(0xC000, (base | (mmc3.reg[6] & mask)) >> 1);
		}
	} else {
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static void M126CW(uint32 A, uint8 V) {
	uint16 mask = (reg[0] & 0x80) ? 0x7F : 0xFF; /* 128 KiB or 256 KiB innter CHR bank selection */
	uint16 base; /* outer CHR bank */

	if (iNESCart.mapper == 126) {
		/* Mapper 126 swaps CHR A18 and A19 */
		base = ((reg[0] << 4) & 0x080) | ((reg[0] << 3) & 0x100) | ((reg[0] << 5) & 0x200);
	} else {
		base = (reg[0] << 4) & 0x380;
	}

	if (reg[3] & 0x10) {
		/* CNROM mode: 8 KiB inner CHR bank comes from outer bank register #2 */
		setchr8((reg[2] & (mask >> 3)) | ((base & ~mask) >> 3));
	} else {
		/* MMC3 CHR mode */
		setchr1(A, (base & ~mask) | (V & mask));
	}
}

static DECLFR(M126ReadDIP) {
	if (reg[1] & 0x01) {
		/* Replace bottom two bits with solder pad or DIP switch setting if so selected */
		return CartBR((A & ~0x03) | (dipsw & 0x03));
	}
	return CartBR(A);
}

static DECLFW(M126WriteWRAM) {
	CartBW(A, V);
	if (!(reg[3] & 0x80)) { /* Lock bit clear: Update any outer bank register */
		reg[A & 0x03] = V;
		MMC3_FixPRG();
	} else if ((A & 0x03) == 2) { /* Lock bit set: Only update the bottom one or two bits of the CNROM bank */
		uint8 mask = ((reg[2] & 0x10) ? 0x01 : 0x03);

		reg[2] = (reg[2] & ~mask) | (V & mask);
	}
	MMC3_FixCHR();
}

static DECLFW(M534IRQWrite) {
	/* Mapper 534 takes the inverted $C000 value for the scanline counter */
	V ^= 0xFF;
	MMC3_IRQWrite(A, V);
}

static void M126Reset(void) {
	dipsw++; /* Soft-resetting cycles through solder pad or DIP switch settings */
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Reset();
}

static void M126Power(void) {
	dipsw = 0;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M126WriteWRAM);
	SetReadHandler(0x8000, 0xFFFF, M126ReadDIP);
	if (iNESCart.mapper == 534) {
		SetWriteHandler(0xC000, 0xDFFF, M534IRQWrite);
	}
}

void Mapper126_Init(CartInfo *info) {
	uint8 ws = 8;
	if (info->iNES2) {
		ws = info->PRGRamSize + info->PRGRamSaveSize;
	}
	MMC3_Init(info, ws, info->battery);
	MMC3_cwrap = M126CW;
	MMC3_pwrap = M126PW;
	info->Power = M126Power;
	info->Reset = M126Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}

void Mapper422_Init(CartInfo *info) {
	Mapper126_Init(info);
}

void Mapper534_Init(CartInfo *info) {
	Mapper126_Init(info);
}
