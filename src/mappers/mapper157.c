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

/* FIXME: Bar code input interface not attached yet */

#include "mapinc.h"
#include "eeprom_x24c0x.h"
#include "bandai.h"

/* Datach Barcode Battler */

static uint8 BarcodeData[256];
static int BarcodeReadPos;
static int BarcodeCycleCount;
static uint32 BarcodeOut;

/* #define INTERL2OF5 */

int FCEUI_DatachSet(uint8 *rcode) {
	int prefix_parity_type[10][6] = {
		{ 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 1, 1 }, { 0, 0, 1, 1, 0, 1 }, { 0, 0, 1, 1, 1, 0 },
		{ 0, 1, 0, 0, 1, 1 }, { 0, 1, 1, 0, 0, 1 }, { 0, 1, 1, 1, 0, 0 }, { 0, 1, 0, 1, 0, 1 },
		{ 0, 1, 0, 1, 1, 0 }, { 0, 1, 1, 0, 1, 0 }
	};
	int data_left_odd[10][7] = {
		{ 0, 0, 0, 1, 1, 0, 1 }, { 0, 0, 1, 1, 0, 0, 1 }, { 0, 0, 1, 0, 0, 1, 1 }, { 0, 1, 1, 1, 1, 0, 1 },
		{ 0, 1, 0, 0, 0, 1, 1 }, { 0, 1, 1, 0, 0, 0, 1 }, { 0, 1, 0, 1, 1, 1, 1 }, { 0, 1, 1, 1, 0, 1, 1 },
		{ 0, 1, 1, 0, 1, 1, 1 }, { 0, 0, 0, 1, 0, 1, 1 }
	};
	int data_left_even[10][7] = {
		{ 0, 1, 0, 0, 1, 1, 1 }, { 0, 1, 1, 0, 0, 1, 1 }, { 0, 0, 1, 1, 0, 1, 1 }, { 0, 1, 0, 0, 0, 0, 1 },
		{ 0, 0, 1, 1, 1, 0, 1 }, { 0, 1, 1, 1, 0, 0, 1 }, { 0, 0, 0, 0, 1, 0, 1 }, { 0, 0, 1, 0, 0, 0, 1 },
		{ 0, 0, 0, 1, 0, 0, 1 }, { 0, 0, 1, 0, 1, 1, 1 }
	};
	int data_right[10][7] = {
		{ 1, 1, 1, 0, 0, 1, 0 }, { 1, 1, 0, 0, 1, 1, 0 }, { 1, 1, 0, 1, 1, 0, 0 }, { 1, 0, 0, 0, 0, 1, 0 },
		{ 1, 0, 1, 1, 1, 0, 0 }, { 1, 0, 0, 1, 1, 1, 0 }, { 1, 0, 1, 0, 0, 0, 0 }, { 1, 0, 0, 0, 1, 0, 0 },
		{ 1, 0, 0, 1, 0, 0, 0 }, { 1, 1, 1, 0, 1, 0, 0 }
	};
	uint8 code[13 + 1];
	uint32 tmp_p = 0;
	uint32 csum = 0;
	int i, j;
	int len;

	for (i = len = 0; i < 13; i++) {
		if (!rcode[i]) break;
		if ((code[i] = rcode[i] - '0') > 9)
			return(0);
		len++;
	}
	if (len != 13 && len != 12 && len != 8 && len != 7) return(0);

	#define BS(x) BarcodeData[tmp_p] = x; tmp_p++

	for (j = 0; j < 32; j++) { /* delay before sending a code */
		BS(0x00);
	}

#ifdef INTERL2OF5

	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 cs */
	BS(1); BS(1); BS(0); BS(0); /* 1 */

#else
	/* Left guard bars */
	BS(1); BS(0); BS(1);

	if (len == 13 || len == 12) {

		for (i = 0; i < 6; i++) {
			if (prefix_parity_type[code[0]][i]) {
				for (j = 0; j < 7; j++) {
					BS(data_left_even[code[i + 1]][j]);
				}
			} else {
				for (j = 0; j < 7; j++) {
					BS(data_left_odd[code[i + 1]][j]);
				}
			}
		}

		/* Center guard bars */
		BS(0); BS(1); BS(0); BS(1); BS(0);

		for (i = 7; i < 12; i++) {
			for (j = 0; j < 7; j++) {
				BS(data_right[code[i]][j]);
			}
		}
		/* Calc and write down the control code if not assigned, instead, send code as is
		   Battle Rush uses modified type of codes with different control code calculation */
		if (len == 12) {
			for (i = 0; i < 12; i++) {
				csum += code[i] * ((i & 1) ? 3 : 1);
			}
			csum = (10 - (csum % 10)) % 10;
			rcode[12] = csum + 0x30;	/* update check code to the input string as well */
			rcode[13] = 0;
			code[12] = csum;
		}
		for (j = 0; j < 7; j++) {
			BS(data_right[code[12]][j]);
		}
	} else if (len == 8 || len == 7) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 7; j++) {
				BS(data_left_odd[code[i]][j]);
			}
		}

		/* Center guard bars */
		BS(0); BS(1); BS(0); BS(1); BS(0);

		for (i = 4; i < 7; i++) {
			for (j = 0; j < 7; j++) {
				BS(data_right[code[i]][j]);
			}
		}
		csum = 0;
		for (i = 0; i < 7; i++) {
			csum += (i & 1) ? code[i] : (code[i] * 3);
		}
		csum = (10 - (csum % 10)) % 10;
		rcode[7] = csum + 0x30;	/* update check code to the input string as well */
		rcode[8] = 0;
		for (j = 0; j < 7; j++) {
			BS(data_right[csum][j]);
		}
	}

	/* Right guard bars */
	BS(1); BS(0); BS(1);
#endif

	for (j = 0; j < 32; j++) {
		BS(0x00);
	}

	BS(0xFF);

	#undef BS

	BarcodeReadPos = 0;
	BarcodeOut = 0x8;
	BarcodeCycleCount = 0;
	return(1);
}

static uint8 latch;
static uint8 hasExternalEEPROM = FALSE;
/* first 256K is internal eeprom data, 2nd 256 is for external eeprom if used.
 * Combined here for simplicity and frontend save compatibility */
static uint8 eeprom[512];

static void M157PW(uint16 A, uint16 V) {
	setprg16(A, V & 0x0F);
}

static void M157CW(uint16 A, uint16 V) {
	setchr8(0);
}

static DECLFW(M157Write) {
	switch (A & 0x0F) {
	case 0x00:
		if (hasExternalEEPROM) {
			latch = (latch & ~0x20) | ((V << 2) & 0x20);
			x24c01_write(latch);
		}
		break;
	case 0x0D:
		if (hasExternalEEPROM) {
			latch = (V & ~0x20) | (latch & 0x20);
			x24c01_write(latch);
		}
		x24c02_write(V);
		break;
	default:
		BANDAI_Write(A, V);
		break;
	}
}

static void M157IRQHook(int a) {
	BANDAI_IRQHook(a);

	BarcodeCycleCount += a;
	if (BarcodeCycleCount >= 1000) {
		BarcodeCycleCount -= 1000;
		if (BarcodeData[BarcodeReadPos] == 0xFF) {
			BarcodeOut = 0;
		} else {
			BarcodeOut = (BarcodeData[BarcodeReadPos] ^ 1) << 3;
			BarcodeReadPos++;
		}
	}
}

static DECLFR(M157Read) {
	return ((cpu.openbus & 0xE7) | ((x24c02_read() | x24c01_read()) << 4) | BarcodeOut);
}

static void M157Power(void) {
	BANDAI_Reset();

	BarcodeData[0] = 0xFF;
	BarcodeReadPos = 0;
	BarcodeOut = 0;
	BarcodeCycleCount = 0;

	SetReadHandler(0x6000, 0x7FFF, M157Read);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M157Write);
}

void Mapper157_Init(CartInfo *info) {
	BANDAI_Init(info, EEPROM_NONE, FALSE);
	BANDAI_pwrap = M157PW;
	BANDAI_cwrap = M157CW;

	info->Power = M157Power;
	MapIRQHook = M157IRQHook;

	GameInfo->cspecial = SIS_DATACH;

	/* internal eeprom shared among all games
	and always enabled regardless of battery flag */
	info->battery = 1;
	x24c02_init(eeprom);
	AddExState(&x24c02_StateRegs, ~0, 0, 0);

	if (info->PRGRamSaveSize && info->PRGRamSaveSize <= 128) {
		/* additional 128 external eeprom */
		x24c01_init(&eeprom[256]);
		AddExState(&x24c01_StateRegs, ~0, 0, 0);
		AddExState(&latch, 1, 0, "LATC");
		hasExternalEEPROM = TRUE;
	}

	if (hasExternalEEPROM) {
		info->SaveGame[0] = eeprom;
		info->SaveGameLen[0] = 512;
	} else {
		info->SaveGame[0] = eeprom;
		info->SaveGameLen[0] = 256;
	}
}
