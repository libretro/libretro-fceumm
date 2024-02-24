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
#include "latch.h"
#include "mmc1.h"
#include "mmc3.h"
#include "vrc2and4.h"
#include "vrc6.h"
#include "flashrom.h"

#define MAPPER_UNROM     0x00
#define MAPPER_MMC3      0x01
#define MAPPER_NROM      0x02
#define MAPPER_CNROM     0x03
#define MAPPER_ANROM     0x04
#define MAPPER_SLROM     0x05
#define MAPPER_SNROM     0x06
#define MAPPER_SUROM     0x07
#define MAPPER_GNROM     0x08
#define MAPPER_PNROM     0x09
#define MAPPER_HKROM     0x0A
#define MAPPER_BANDAI152 0x0B
#define MAPPER_TLSROM    0x0E
#define MAPPER_189       0x0F
#define MAPPER_VRC6      0x10
#define MAPPER_VRC2_22   0x12
#define MAPPER_VRC4_25   0x15
#define MAPPER_VRC4_23   0x18
#define MAPPER_VRC1      0x1A

#define WRAM_CHIP  0x10
#define FLASH_CHIP 0x11

static uint8 *flash = NULL;
static uint8_t reg[8], latch189, mapper;

static SFORMAT StateRegs[] = {
    { reg, 8, "REGS" },
	{ &mapper, 1, "MAPR" },
    { &latch189, 1, "L189" },
    { 0 }
};

static uint32 GetPRGBase(void) {
	return ((reg[2] << 8) | reg[1]);
}

static uint32 GetPRGMask(void) {
	return (~reg[3]);
}

static uint32 GetCHRBase(void) {
	return reg[6];
}

static void M446MMC1PW(uint16 A, uint16 V) {
    setprg16r(FLASH_CHIP, A, (GetPRGBase() >> 1) | (V & (GetPRGMask() >> 1)));
}
static void M446MMC1CW(uint16 A, uint16 V) {
    setchr4(A, (GetCHRBase() >> 2) | (V & 0x1F));
}

static void M446MMC3PW(uint16 A, uint16 V) {
    switch (mapper) {
    case MAPPER_MMC3:
    case MAPPER_TLSROM:
        setprg8r(FLASH_CHIP, A, GetPRGBase() | (V & GetPRGMask()));
        break;
    case MAPPER_189:
        setprg32r(FLASH_CHIP, 0x8000, (GetPRGBase() >> 2) | (latch189 & 0x03));
        break;
    }
}
static void M446MMC3CW(uint16 A, uint16 V) {
    switch (mapper) {
    case MAPPER_TLSROM:
        setchr1(A, GetCHRBase() | (V & 0x7F));
        break;
	case MAPPER_MMC3:
    case MAPPER_189:
        setchr1(A, GetCHRBase() | (V & 0xFF));
        break;
    }
}

static void M446VRC24PW(uint16 A, uint16 V) {
    switch (mapper) {
    case MAPPER_VRC2_22:
    case MAPPER_VRC4_23:
	case MAPPER_VRC4_25:
        setprg8r(FLASH_CHIP, A, GetPRGBase() | (V & GetPRGMask()));
        break;
    }
}
static void M446VRC24CW(uint16 A, uint16 V) {
    int i;
    switch (mapper) {
    case MAPPER_VRC2_22:
        for (i = 0; i < 8; i++) setchr1(i << 10, vrc24.chr[i] >> 1);
        break;
    case MAPPER_VRC4_23:
	case MAPPER_VRC4_25:
        setchr1(A, V & 0xFF);
        break;
    }
}

static void M446VRC6PW(uint16 A, uint16 V) {
    setprg8r(FLASH_CHIP, A, GetPRGBase() | (V & GetPRGMask()));
}
static void M446VRC6CW(uint16 A, uint16 V) {
    setchr1(A, V & 0xFF);
}

static void Sync(void) {
	if (reg[0] & 0x80) {
		switch (mapper) {
		case MAPPER_NROM:
			setprg8r(FLASH_CHIP, 0x8000, GetPRGBase() | (0x00 & GetPRGMask()));
			setprg8r(FLASH_CHIP, 0xA000, GetPRGBase() | (0x01 & GetPRGMask()));
			setprg8r(FLASH_CHIP, 0xC000, GetPRGBase() | (0x02 & GetPRGMask()));
			setprg8r(FLASH_CHIP, 0xE000, GetPRGBase() | (0x03 & GetPRGMask()));
			setchr8(GetCHRBase());
            setmirror(reg[4] & 0x01);
			break;
		case MAPPER_CNROM:
			setprg8r(FLASH_CHIP, 0x8000, GetPRGBase() | (0x00 & GetPRGMask()));
			setprg8r(FLASH_CHIP, 0xA000, GetPRGBase() | (0x01 & GetPRGMask()));
			setprg8r(FLASH_CHIP, 0xC000, GetPRGBase() | (0x02 & GetPRGMask()));
			setprg8r(FLASH_CHIP, 0xE000, GetPRGBase() | (0x03 & GetPRGMask()));
			setchr8(latch.data & 3);
			setmirror(reg[4] & 0x01);
			break;
		case MAPPER_UNROM:
			setprg16r(FLASH_CHIP, 0x8000, (GetPRGBase() >> 1) | (latch.data & (GetPRGMask() >> 1)));
			setprg16r(FLASH_CHIP, 0xC000, (GetPRGBase() >> 1) | ((GetPRGMask() >> 1) & 0x1F));
			setchr8(GetCHRBase());
			setmirror(reg[4] & 0x01);
			break;
		case MAPPER_BANDAI152:
			setprg16r(FLASH_CHIP, 0x8000, (GetPRGBase() >> 1) | ((latch.data >> 4) & (GetPRGMask() >> 1)));
			setprg16r(FLASH_CHIP, 0xC000, (GetPRGBase() >> 1) | (0xFF & (GetPRGMask() >> 1)));
			setchr8(latch.data & 0xF);
			setmirror(MI_0 + ((latch.data & 0x80) >> 7));
			break;
		case MAPPER_ANROM:
			setprg32r(FLASH_CHIP, 0x8000, (GetPRGBase() >> 2) | (latch.data & (GetPRGMask() >> 2)));
			setchr8(GetCHRBase());
            setmirror(MI_0 + ((latch.data & 0x10) >> 4));
			break;
		case MAPPER_GNROM:
			setprg32r(FLASH_CHIP, 0x8000, (GetPRGBase() >> 2) | ((latch.data >> 4) & (GetPRGMask() >> 2)));
			setchr8(latch.data & 0x03);
			setmirror(reg[4] & 0x01);
			break;

		case MAPPER_SLROM:
		case MAPPER_SNROM:
			setprg8r(WRAM_CHIP, 0x6000, 0);
            MMC1_FixPRG();
            MMC1_FixCHR();
            MMC1_FixMIR();
			break;

		case MAPPER_MMC3:
			setprg8r(WRAM_CHIP, 0x6000, 0);
            MMC3_FixPRG();
            MMC3_FixCHR();
            MMC3_FixMIR();
			break;
		case MAPPER_TLSROM:
			setprg8r(WRAM_CHIP, 0x6000, 0);
            MMC3_FixPRG();
            MMC3_FixCHR();
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
			break;
		case MAPPER_189:
			setprg8r(WRAM_CHIP, 0x6000, 0);
            MMC3_FixPRG();
            MMC3_FixCHR();
            MMC3_FixMIR();
			break;

		case MAPPER_VRC2_22:
			setprg8r(WRAM_CHIP, 0x6000, 0);
            VRC24_FixPRG();
            VRC24_FixCHR();
            VRC24_FixMIR();
			break;
		case MAPPER_VRC4_23:
		case MAPPER_VRC4_25:
			setprg8r(WRAM_CHIP, 0x6000, 0);
            VRC24_FixPRG();
            VRC24_FixCHR();
            VRC24_FixMIR();
			break;

		case MAPPER_VRC6:
			setprg8r(WRAM_CHIP, 0x6000, 0);
            VRC6_FixPRG();
            VRC6_FixCHR();
			break;
		}
	} else {
		setprg8r(FLASH_CHIP, 0x8000, GetPRGBase());
		setprg8r(FLASH_CHIP, 0xA000, 0x3D);
		setprg8r(FLASH_CHIP, 0xC000, 0x3E);
		setprg8r(FLASH_CHIP, 0xE000, 0x3F);
		setchr8(GetCHRBase());
        setmirror(reg[4] & 0x01);
	}

    /* CHR-RAM Protect */
    SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], (reg[5] & 0x04) ? 0 : 1);
}

static void applyMode(void) {
	if (reg[0] & 0x80) {
		switch (mapper) {
		case MAPPER_ANROM:
		case MAPPER_CNROM:
		case MAPPER_UNROM:
		case MAPPER_GNROM:
		case MAPPER_BANDAI152:
			Latch_RegReset();
			break;
		case MAPPER_SLROM:
		case MAPPER_SNROM:
            MMC1_Reset();
			break;
		case MAPPER_189:
		case MAPPER_MMC3:
		case MAPPER_TLSROM:
            MMC3_Reset();
			break;
		case MAPPER_VRC2_22:
			vrc2and4_VRC4 = FALSE;
			vrc2and4_A0 = 0x02;
			vrc2and4_A1 = 0x01;
            VRC24_Reset();
			break;
		case MAPPER_VRC4_23:
			vrc2and4_VRC4 = TRUE;
			vrc2and4_A0 = 0x05;
			vrc2and4_A1 = 0x0A;
			VRC24_Reset();
			break;
		case MAPPER_VRC4_25:
			vrc2and4_VRC4 = TRUE;
			vrc2and4_A0 = 0x0A;
			vrc2and4_A1 = 0x05;
			VRC24_Reset();
			break;
		case MAPPER_VRC6:
			VRC6_Reset();
			break;
		}
	}
}

static DECLFR(M446Read) {
	return FlashROM_Read(A);
}

static DECLFW(M446WriteLatch) {
	if (mapper == MAPPER_189) {
		latch189 = A & 0xFF;
		Sync();
	} else {
		CartBW(A, V);
	}
}

static DECLFW(M446WriteReg) {
	A &= 0x07;
	if (!A && !iNESCart.submapper && ((V & 0x1F) == 0x01)) {
		V = (V & ~0x1F) | MAPPER_SNROM;
	}
	reg[A] = V;
	if (reg[0] & 0x80) {
		mapper = reg[0] & 0x1F;
		applyMode();
	}
	Sync();
}

static DECLFW(M446Write) {
	if (reg[0] & 0x80) {
		switch (mapper) {
		case MAPPER_ANROM:
		case MAPPER_CNROM:
		case MAPPER_UNROM:
		case MAPPER_GNROM:
		case MAPPER_BANDAI152:
			Latch_Write(A, V);
			break;
		case MAPPER_SLROM:
		case MAPPER_SNROM:
            MMC1_Write(A, V);
			break;
		case MAPPER_189:
		case MAPPER_MMC3:
		case MAPPER_TLSROM:
            MMC3_Write(A, V);
			break;
		case MAPPER_VRC2_22:
            VRC24_Write(A, V);
			break;
		case MAPPER_VRC4_23:
			VRC24_Write(A, V);
			break;
		case MAPPER_VRC4_25:
			VRC24_Write(A, V);
			break;
		case MAPPER_VRC6:
			VRC6_Write(A, V);
			break;
		}
	} else {
		FlashROM_Write(A, V);
	}
}

static void M446HBIRQHook(void) {
	if (reg[0] & 0x80) {
		switch (mapper) {
		case MAPPER_MMC3:
		case MAPPER_TLSROM:
		case MAPPER_189:
			MMC3_IRQHBHook();
			break;
		}
	}
}

static void M446CPUIRQHook(int a) {
	FlashROM_CPUCyle(a);
	if (reg[0] & 0x80) {
		switch (mapper) {
		case MAPPER_VRC2_22:
		case MAPPER_VRC4_23:
		case MAPPER_VRC4_25:
			VRC24_IRQCPUHook(a);
			break;
		case MAPPER_VRC6:
			VRC6_IRQCPUHook(a);
			break;
		}
	}
}

static void M446Close(void) {
	if (WRAM) {
		FCEU_free(WRAM);
		WRAM = NULL;
	}
	if (flash) {
		FCEU_free(flash);
		flash = NULL;
	}
}

static void M446Reset(void) {
	mapper = 0;
	memset(reg, 0, sizeof(reg));
	applyMode();
	Sync();
}

static void M446Power(void) {
	mapper = 0;
	memset(reg, 0, sizeof(reg));

	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xFFFF, M446Read);

	SetWriteHandler(0x5000, 0x5FFF, M446WriteReg);
	SetWriteHandler(0x6000, 0x6FFF, M446WriteLatch);
	SetWriteHandler(0x7000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M446Write);

	applyMode();
	Sync();
}

static void StateRestore(int version) {
	applyMode();
	Sync();
}

void Mapper446_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);

	MMC1_Init(info, FALSE, FALSE);
	MMC1_pwrap = M446MMC1PW;
	MMC1_cwrap = M446MMC1CW;

	MMC3_Init(info, FALSE, FALSE);
	MMC3_pwrap = M446MMC3PW;
	MMC3_cwrap = M446MMC3CW;

	VRC24_Init(info, true, 0x01, 0x02, FALSE, TRUE);
	VRC24_pwrap = M446VRC24PW;
	VRC24_cwrap = M446VRC24CW;

	VRC6_Init(info, 0x01, 0x02, FALSE);
	VRC6_pwrap = M446VRC6PW;
	VRC6_cwrap = M446VRC6CW;

	info->Power = M446Power;
	info->Reset = M446Reset;
	info->Close = M446Close;

	MapIRQHook = M446CPUIRQHook;
	GameHBIRQHook = M446HBIRQHook;

	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(WRAM_CHIP, WRAM, WRAMSIZE, TRUE);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");

		if (info->battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
	}

	/* Allocate memory for flash */
	flash = (uint8 *)FCEU_gmalloc(PRGsize[0]);
	SetupCartPRGMapping(FLASH_CHIP, flash, PRGsize[0], 0);
	AddExState(flash, PRGsize[0], 0, "FLSH");
	memcpy(flash, PRGptr[0], PRGsize[0]);
	FlashROM_Init(flash, PRGsize[0], 0x01, 0x7E, 131072, 0xAAA, 0x555);
}
