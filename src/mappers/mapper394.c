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

/* Mapper 394: HSK007 circuit board that can simulate J.Y. ASIC, MMC3, and NROM. */

#include "mapinc.h"
#include "jyasic.h"
#include "mmc3.h"

static uint8 reg[4];

static uint32 M394_PRGBank_JY(uint32 V) {
    uint8 base = ((reg[1] << 5) & 0x020) | ((reg[3] << 1) & 0x010);

	return (base | (V & 0x1F));
}

static uint32 M394_CHRBank_JY(uint32 V) {
    uint32 base = ((reg[1] << 8) & 0x100) | ((reg[3] << 1) & 0x080);

	return (base | (V & 0x0FF));
}

static void M394MMC3PW(uint16 A, uint16 V) {
	uint8 mask = (reg[3] & 0x10) ? 0x1F : 0x0F;
	uint8 base = ((reg[1] << 5) & 0x020) | ((reg[3] << 1) & 0x010);

	if (reg[1] & 0x08) {
		setprg8(A, base | (V & mask));
	} else {
		setprg32(0x8000, (base | ((reg[3] << 1) & 0x0F)) >> 2);
	}
}

static void M394MMC3CW(uint16 A, uint16 V) {
	uint16 mask  = (reg[3] & 0x80) ? 0xFF : 0x7F;
	uint16 base = ((reg[3] << 1) & 0x080) | ((reg[1] << 8) & 0x100);

	setchr1(A, (base & ~mask) | (V & mask));
}

static DECLFW(M394WriteReg) {
	uint8 oldMode = reg[1];

	A &= 3;
	reg[A] = V;
	if (A == 1) {
		if (!(oldMode & 0x10) && (V & 0x10)) {
			JYASIC_Power();
        } else if ((oldMode & 0x10) && !(V & 0x10)) {
			JYASIC_restoreWriteHandlers();
			MMC3_Power();
		}
	} else {
		if (reg[1] & 0x10) {
			JYASIC_FixPRG();
            JYASIC_FixCHR();
            JYASIC_FixMIR();
		} else {
			MMC3_FixPRG();
			MMC3_FixCHR();
			MMC3_FixMIR();
		}
	}
}

static void M394StateRestore(int version) {
	int i;

	JYASIC_restoreWriteHandlers();
	if (reg[1] & 0x10) {
		SetWriteHandler(0x5000, 0x5FFF, JYASIC_WriteALU);
		SetWriteHandler(0x6000, 0x7fff, CartBW);
		SetWriteHandler(0x8000, 0x87FF, JYASIC_WritePRG); /* 8800-8FFF ignored */
		SetWriteHandler(0x9000, 0x97FF, JYASIC_WriteCHRLow); /* 9800-9FFF ignored */
		SetWriteHandler(0xA000, 0xA7FF, JYASIC_WriteCHRHigh); /* A800-AFFF ignored */
		SetWriteHandler(0xB000, 0xB7FF, JYASIC_WriteNT); /* B800-BFFF ignored */
		SetWriteHandler(0xC000, 0xCFFF, JYASIC_WriteIRQ);
		SetWriteHandler(0xD000, 0xD7FF, JYASIC_WriteMode); /* D800-DFFF ignored */
		for (i = 0; i < 0x10000; i++) {
			JYASIC_cpuWrite[i] = GetWriteHandler(i);
        }
		SetWriteHandler(0x0000, 0xFFFF, JYASIC_trapCPUWrite); /* Trap all CPU writes for IRQ clocking purposes */
		JYASIC_CPUWriteHandlersSet = 1;
		SetReadHandler(0x5000, 0x5FFF, JYASIC_ReadALU_DIP);
		SetReadHandler(0x6000, 0xFFFF, CartBR);
		JYASIC_FixPRG();
		JYASIC_FixCHR();
		JYASIC_FixMIR();
	} else {
		SetWriteHandler(0x8000, 0xBFFF, MMC3_CMDWrite);
		SetWriteHandler(0xC000, 0xFFFF, MMC3_IRQWrite);
		SetReadHandler(0x8000, 0xFFFF, CartBR);
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M394Power(void) {
	reg[0] = 0x00;
	reg[1] = 0x0F; /* start in MMC3 mode */
	reg[2] = 0x00;
	reg[3] = 0x10;
	MMC3_Power();
	SetWriteHandler(0x5000, 0x5FFF, M394WriteReg);
}

void Mapper394_Init(CartInfo *info) {
	/* Multicart */
	JYASIC_Init(info, TRUE);
	JYASIC_GetPRGBank = M394_PRGBank_JY;
	JYASIC_GetCHRBank = M394_CHRBank_JY;

    MMC3_Init(info, 0, 0);
    MMC3_pwrap = M394MMC3PW;
	MMC3_cwrap = M394MMC3CW;

	info->Reset = M394Power;
	info->Power = M394Power;

	AddExState(reg, 4, 0, "HSK");
	GameStateRestore = M394StateRestore;
}
