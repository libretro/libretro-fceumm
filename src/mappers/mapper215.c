/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2011 CaH4e3
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
 *
 * Submapper 0, UNIF board name UNL-8237:
 * Earthworm Jim 2
 * Mortal Kombat 3 (SuperGame, not Extra 60, not to be confused by similarly-named games from other developers)
 * Mortal Kombat 3 Extra 60 (both existing ROM images are just extracts of the 2-in-1 multicart containing this game)
 * Pocahontas Part 2
 * 2-in-1: Aladdin, EarthWorm Jim 2 (Super 808)
 * 2-in-1: The Lion King, Bomber Boy (GD-103)
 * 2-in-1: Super Golden Card: EarthWorm Jim 2, Boogerman (king002)
 * 2-in-1: Mortal Kombat 3 Extra 60, The Super Shinobi (king005)
 * 3-in-1: Boogerman, Adventure Island 3, Double Dragon 3 (Super 308)
 * 5-in-1: Golden Card: Aladdin, EarthWorm Jim 2, Garo Densetsu Special, Silkworm, Contra Force (SPC005)
 * 6-in-1: Golden Card: EarthWorm Jim 2, Mortal Kombat 3, Double Dragon 3, Contra 3, The Jungle Book, Turtles Tournament Fighters (SPC009)
 *
 * Submapper 1, UNIF board name UNL-8237A:
 * 9-in-1 High Standard Card: The Lion King, EarthWorm Jim 2, Aladdin, Boogerman, Somari, Turtles Tournament Fighters, Mortal Kombat 3, Captain Tsubasa 2, Taito Basketball (king001)
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];

static const uint8 regperm[8][8] =
{
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 0, 2, 6, 1, 7, 3, 4, 5 },
	{ 0, 5, 4, 1, 7, 2, 6, 3 },		/* unused */
	{ 0, 6, 3, 7, 5, 2, 4, 1 },
	{ 0, 2, 5, 3, 6, 1, 7, 4 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },		/* empty */
	{ 0, 1, 2, 3, 4, 5, 6, 7 },		/* empty */
	{ 0, 1, 2, 3, 4, 5, 6, 7 },		/* empty */
};

static const uint16 adrperm[8][8] = {
	{ 0x8000, 0x8001, 0xA000, 0xA001, 0xC000, 0xC001, 0xE000, 0xE001 },
	{ 0xA001, 0xA000, 0x8000, 0xC000, 0x8001, 0xC001, 0xE000, 0xE001 },
	{ 0x8000, 0x8001, 0xA000, 0xA001, 0xC000, 0xC001, 0xE000, 0xE001 },	/* unused */
	{ 0xC001, 0x8000, 0x8001, 0xA000, 0xA001, 0xE001, 0xE000, 0xC000 },
	{ 0xA001, 0x8001, 0x8000, 0xC000, 0xA000, 0xC001, 0xE000, 0xE001 },
	{ 0x8000, 0x8001, 0xA000, 0xA001, 0xC000, 0xC001, 0xE000, 0xE001 },	/* empty */
	{ 0x8000, 0x8001, 0xA000, 0xA001, 0xC000, 0xC001, 0xE000, 0xE001 },	/* empty */
	{ 0x8000, 0x8001, 0xA000, 0xA001, 0xC000, 0xC001, 0xE000, 0xE001 },	/* empty */
};

static const uint8 protarray[8][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0 Super Hang-On               */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 }, /* 1 Monkey King                 */
	{ 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00 }, /* 2 Super Hang-On/Monkey King   */
	{ 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x05, 0x00 }, /* 3 Super Hang-On/Monkey King   */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 4                             */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 5                             */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 6                             */
	{ 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0F, 0x00 }  /* 7 (default) Blood of Jurassic */
};

static void M215CW(uint16 A, uint16 V) {
	uint16 mask = (reg[0] & 0x40) ? 0x7F : 0xFF;
	uint16 base;

	if (iNESCart.submapper == 1) {
		base = (reg[1] << 7) & 0x700;
	} else {
		base = (reg[1] << 6) & 0x300;
	}

	base = (base & ~mask) | (((reg[1] << 2) & 0x80) & ~mask);
	setchr1(A, base | (V & mask));
}

static void M215PW(uint16 A, uint16 V) {
	uint16 mask = (reg[0] & 0x40) ? 0x0F : 0x1F;
	uint16 base;

	if (iNESCart.submapper == 1) {
		base = ((reg[1] << 5) & 0x60) | ((reg[1] << 4) & 0x80);
	} else {
		base = (reg[1] << 5) & 0x60;
	}

	if (reg[0] & 0x80) { /* NROM */
		uint16 bank = (((reg[1] & 0x10) & ~mask) >> 1) | ((base & ~mask) >> 1) | (reg[0] & (mask >> 1));

		if (reg[0] & 0x20) { /* NROM-256 */
			setprg32(0x8000, bank >> 1);
		} else { /* NROM-128 */
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	} else {
		setprg8(A, ((reg[1] & 0x10) & ~mask) | (base & ~mask) | (V & mask));
	}
}

static DECLFR(M215ProtRead) {
	return (cpu.openbus & ~0x0F) | (protarray[reg[2]][A & 0x07] & 0x0F);
}

static DECLFW(M215Write) {
	A = adrperm[reg[3]][((A >> 12) & 0x06) | (A & 0x01)];
	switch (A & 0xE001) {
	case 0x8000:
		MMC3_Write(A, (V & 0xC0) | regperm[reg[3]][V & 0x07]);
		break;
	default:
		MMC3_Write(A, V);
		break;
	}
}

static DECLFW(M215Write5) {
	switch (A & 0x07) {
	case 0:
		reg[0] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		break;
	case 1:
		reg[1] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		break;
	case 2:
		reg[2] = V & 0x07;
		break;
	case 7:
		reg[3] = V & 0x07;
		break;
	}
}

static void M215Power(void) {
	reg[0] = 0x00;
	reg[1] = 0x0F;
	reg[3] = 0x04;
	reg[2] = 0x07;
	MMC3_Power();
	SetReadHandler(0x5000, 0x5FFF, M215ProtRead);
	SetWriteHandler(0x5000, 0x5FFF, M215Write5);
	SetWriteHandler(0x8000, 0xFFFF, M215Write);
}

static void M215Reset(void) {
	reg[0] = 0x00;
	reg[1] = 0x0F;
	reg[3] = 0x04;
	reg[2] = 0x07;
	MMC3_Reset();
}

void Mapper215_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M215CW;
	MMC3_pwrap = M215PW;

	info->Power = M215Power;
	info->Reset = M215Reset;

	AddExState(reg, 4, 0, "EXPR");

	if ((!info->iNES2) && ((ROM.prg.size * 16) >= 2048)) { /* UNL-8237A */
		info->submapper = 1;
	}
}
