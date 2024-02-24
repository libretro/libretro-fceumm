/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
 *  Copyright (C) 2011 FCEUX team
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
 * INES Mapper 016
 * iNES Mapper 016 is used for some of the Bandai FCG boards, namely, boards with
 * the FCG-1 ASIC that supports no EEPROM, and the LZ93D50 ASIC with no or 256 bytes of EEPROM.
 *
 * INES Mapper 016 submapper table
 * Submapper #	Meaning											Note
 * 0			Unspecified										Emulate both FCG-1/2 and LZ93D50 chips in their respective CPU address ranges.
 * 1			LZ93D50 with 128 byte serial EEPROM (24C01)		Deprecated, use INES Mapper 159 instead.
 * 2			Datach 	Joint ROM System						Deprecated, use INES Mapper 157 instead.
 * 3			8 KiB of WRAM instead of serial EEPROM			Deprecated, use INES Mapper 153 instead.
 * 4			FCG-1/2											Responds only in the CPU $6000-$7FFF address range; IRQ counter is not latched.
 * 5			LZ93D50 with no or 256-byte serial EEPROM (24C02)		Responds only in the CPU $8000-$FFFF address range; IRQ counter is latched.
 *
 */

#include "mapinc.h"
#include "eeprom_x24c0x.h"
#include "bandai.h"

EEPROM_TYPE eeprom_type = EEPROM_NONE;

static uint8 prg;
static uint8 chr[8];
static uint8 mirr;
static uint8 IRQa;
static int16 IRQCount, IRQLatch;

static uint8 isFCG12 = 0;

static uint8 eeprom[256];

void (*BANDAI_pwrap)(uint16 A, uint16 V);
void (*BANDAI_cwrap)(uint16 A, uint16 V);

static SFORMAT StateRegs[] =
{
	{ chr, 8, "CRGS" },
	{ &prg, 1, "PREG" },
	{ &mirr, 1, "MIRR" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 2, "IRQC" },
	{ &IRQLatch, 2, "IRQL" },	/* need for Famicom Jump II - Saikyou no 7 Nin (J) [!] */
	{ 0 }
};

static void GENPWRAP(uint16 A, uint16 V) {
	setprg16(A, V & 0x0F);
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V);
}

void BANDAI_FixPRG(void) {
	BANDAI_pwrap(0x8000, prg);
	BANDAI_pwrap(0xC000, ~0);
}

void BANDAI_FixCHR(void) {
	BANDAI_cwrap(0x0000, chr[0]);
	BANDAI_cwrap(0x0400, chr[1]);
	BANDAI_cwrap(0x0800, chr[2]);
	BANDAI_cwrap(0x0C00, chr[3]);
	BANDAI_cwrap(0x1000, chr[4]);
	BANDAI_cwrap(0x1400, chr[5]);
	BANDAI_cwrap(0x1800, chr[6]);
	BANDAI_cwrap(0x1C00, chr[7]);
}

void BANDAI_FixMIR(void) {
	switch (mirr & 0x03) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFR(BANDAI_Read) {
	if (eeprom_type == EEPROM_X24C01) {
		return (cpu.openbus & ~0x010) | (x24c01_read() << 4);
	}
	return (cpu.openbus & ~0x010) | (x24c02_read() << 4);
}

DECLFW(BANDAI_Write) {
	switch (A & 0x0F) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
		chr[A & 0x07] = V;
		BANDAI_FixCHR();
		break;
	case 0x08:
		prg = V;
		BANDAI_FixPRG();
		break;
	case 0x09:
		mirr = V;
		BANDAI_FixMIR();
		break;
	case 0x0A:
		IRQa = V & 1;
		IRQCount = IRQLatch;
		if (IRQa && !IRQLatch) {
			X6502_IRQBegin(FCEU_IQEXT);
		} else {
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;
	case 0x0B:
		IRQLatch &= 0xFF00;
		IRQLatch |= V;
		break;
	case 0x0C:
		IRQLatch &= 0x00FF;
		IRQLatch |= V << 8;
		break;
	case 0x0D:
		if (eeprom_type == EEPROM_X24C02) {
			x24c02_write(V);
		} else if (eeprom_type == EEPROM_X24C01) {
			x24c01_write(V);
		}
		break;
	}
}

void BANDAI_IRQHook(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount < 0) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

void BANDAI_Reset(void) {
	int x;

	prg = 0x00;
	for (x = 0; x < 8; x++) {
		chr[x] = x;
	}
	IRQCount = 0;
	IRQLatch = 0;
	IRQa = FALSE;

	BANDAI_FixPRG();
	BANDAI_FixCHR();
	BANDAI_FixMIR();
}

static void StateRestore(int version) {
	BANDAI_FixPRG();
	BANDAI_FixCHR();
	BANDAI_FixMIR();
}

void BANDAI_Power(void) {
	if (UNIFchrrama) {
		setchr8(0);
	}

	BANDAI_Reset();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, BANDAI_Write);
	if (isFCG12) {
		/* submapper 4 */
		SetWriteHandler(0x6000, 0x7FFF, BANDAI_Write);
	}

	if (eeprom_type != EEPROM_NONE) {
		SetReadHandler(0x6000, 0x7FFF, BANDAI_Read);
		if (EEPROM_X24C02) {
			x24c02_init(eeprom);
		} else {
			x24c01_init(eeprom);
		}
	}
}

void BANDAI_Init(CartInfo *info, EEPROM_TYPE _eeprom_type, int _isFCG) {
	BANDAI_pwrap = GENPWRAP;
	BANDAI_cwrap = GENCWRAP;

	eeprom_type = _eeprom_type;
	isFCG12 = _isFCG;

	info->Power = BANDAI_Power;
	MapIRQHook = BANDAI_IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	if (eeprom_type != EEPROM_NONE) {
		info->battery = 1;
		memset(eeprom, 0, sizeof(eeprom));
		if (eeprom_type == EEPROM_X24C02) {
			info->SaveGame[0] = eeprom;
			info->SaveGameLen[0] = 256;
			AddExState(&x24c02_StateRegs, ~0, 0, 0);
		} else if (eeprom_type == EEPROM_X24C01) {
			info->SaveGame[0] = eeprom;
			info->SaveGameLen[0] = 128;
			AddExState(&x24c01_StateRegs, ~0, 0, 0);
		}
	}
}
