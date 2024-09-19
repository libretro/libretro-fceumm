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

static uint8 reg[8];

static void M268CW(uint16 A, uint16 V) {
	uint16 base = ((reg[0] << 4) & 0x380) | ((reg[2] << 3) & 0x078);
	uint16 mask = (reg[0] & 0x80) ? 0x7F : 0xFF;
	uint8  vram = CHRRAM && (reg[4] & 0x01) && ((V & 0xFE) == (reg[4] & 0xFE));

	/* CHR-RAM write protect on submapper 8/9) */
	if ((iNESCart.submapper & ~1) == 8) {
		if (reg[0] & 0x10) {
			SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 1);
		} else {
			SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 0);
		}
	}

	if (reg[3] & 0x10) {
		/* GNROM */
		mask = 0x07;
		V = A >> 10;
	}
	setchr1r(vram ? 0x10 : 0x00, A, (base & ~mask) | (V & mask));
}

static void M268PW(uint16 A, uint16 V) {
	uint16 base;
	uint16 mask = 0x0F				/* PRG A13-A16 */
	    | ((~reg[0] >> 2) & 0x10)	/* PRG A17     */
	    | ((~reg[1] >> 2) & 0x20)	/* PRG A18     */
	    | ((reg[1] >> 0) & 0x40)	/* PRG A19     */
	    | ((reg[1] << 2) & 0x80)	/* PRG A20     */
	    ;

	switch (iNESCart.submapper & ~1) {
	default: /* Original implementation */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[1] << 3) & 0x80) |
		    ((reg[1] << 6) & 0x300) | ((reg[0] << 6) & 0xC00);
		break;
	case 2: /* Later revision with different arrangement of register 1 */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[1] << 4) & 0x80) |
		    ((reg[1] << 6) & 0x100) | ((reg[1] << 8) & 0x200) | ((reg[0] << 6) & 0xC00);
		break;
	case 4: /* LD622D: PRG A20-21 moved to register 0 */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[0] << 3) & 0x180);
		break;
	case 6: /* J-852C: CHR A17 selects between two PRG chips */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[1] << 3) & 0x80) |
		    ((reg[1] << 6) & 0x300) | ((reg[0] << 6) & 0xC00);
		base &= PRG_BANK_COUNT(16) - 1;
		if ((reg[0] & 0x80) ? !!(reg[0] & 0x08) : !!(mmc3.reg[0] & 0x80)) {
			base |= PRG_BANK_COUNT(16);
		}
		break;
	}
	if (reg[3] & 0x10) {
		/* GNROM */
		switch (iNESCart.submapper & ~1) {
		default: mask = (reg[1] & 0x02) ? 0x03 : 0x01; break;
		case 2: mask = (reg[1] & 0x10) ? 0x01 : 0x03; break;
		}
		V = A >> 13;
	}
	setprg8(A, (base & ~mask) | (V & mask));

	if (mmc3.wram & 0x20) {
		/* Hack for FS005 games on Mindkids board that only work with emulation. */
		mmc3.wram &= ~0x40;
	}
}

static void M268MIR(void) {
	if (!(reg[0] & 0x20)) {
		switch (iNESCart.submapper) {
		case 10:
		case 11:
			setmirror(MI_0 + ((reg[0] >> 4) & 0x01));
			break;
		default:
			setmirror((mmc3.mirr & 0x01) ^ 0x01);
			break;
		}
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static DECLFR(M268WramRead) {
	if (mmc3.wram & 0x80) {
		return CartBR(A);
	}
	return CPU_OPENBUS;
}

static DECLFW(M268WramWrite) {
	if (MMC3_WramIsWritable()) {
		CartBW(A, V);
	}
}

static DECLFW(M268Write) {
	uint8 index = A & 0x07;

	if (!(iNESCart.submapper & 0x01) && (A >= 0x6000)) {
		M268WramWrite(A, V);
	}

	if (!(reg[3] & 0x80) || (index == 2)) {
		if (index == 2) {
			if (reg[2] & 0x80) {
				V = (V & 0x0F) | (reg[2] & ~0x0F);
			}
			V &= 0xF1 | ((~reg[2] >> 3) & 0x0E);
		}
		reg[index] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	}
}

static void M268Reset(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0x00;
	reg[4] = reg[5] = reg[6] = reg[7] = 0x00;
	MMC3_Reset();
}

static void M268Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0x00;
	reg[4] = reg[5] = reg[6] = reg[7] = 0x00;
	MMC3_Power();
	SetReadHandler(0x6000, 0x7FFF, M268WramRead);
	SetWriteHandler(0x6000, 0x7FFF, M268WramWrite);
	if (iNESCart.mapper == 224 || (iNESCart.submapper & 0x01)) {
		SetWriteHandler(0x5000, 0x5FFF, M268Write);
	} else {
		SetWriteHandler(0x6000, 0x7FFF, M268Write);
	}
}

static void M268Close(void) {
	MMC3_Close();
}

static void Common_Init(CartInfo *info) {
	int ws = info->PRGRamSize + info->PRGRamSaveSize;

	MMC3_Init(info, ws / 1024, info->battery);
	MMC3_FixMIR = M268MIR;
	MMC3_pwrap = M268PW;
	MMC3_cwrap = M268CW;

	info->Power = M268Power;
	info->Reset = M268Reset;
	info->Close = M268Close;

	AddExState(reg, 8, 0, "EXPR");

	if (CHR_ROM_SIZE_8K) {
		CHRRAMSIZE = info->CHRRamSize + info->CHRRamSaveSize;
		if (CHRRAMSIZE) {
			CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
			SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
			AddExState(CHRRAM, CHRRAMSIZE, 0x00, "CRAM");
		}
	}
}

void Mapper268_Init(CartInfo *info) {
	Common_Init(info);
}

/* UNIF loader */

void COOLBOY_Init(CartInfo *info) {
	info->mapper = 268;
	info->submapper = 0;
	info->PRGRamSize = 8192;
	Common_Init(info);
}

void MINDKIDS_Init(CartInfo *info) { /* M224 */
	info->mapper = 224;
	info->submapper = 1;
	info->PRGRamSize = 8192;
	Common_Init(info);
}

