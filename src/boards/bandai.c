/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
 *  Copyright (C) 2011 FCEUX team
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
 * Bandai mappers
 *
 */

#include "mapinc.h"
#include "eeprom_24C0x.h"

static uint8_t reg[16], is153, x24c02;
static uint8_t IRQa;
static int16_t IRQCount, IRQLatch;

static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;

/* TODO: Clean this up. State variables are expanded for
 * big-endian compatibility when saving and loading states */
static SFORMAT StateRegs[] =
{
	{ &reg[0], 1, "REG0" },
	{ &reg[1], 1, "REG1" },
	{ &reg[2], 1, "REG2" },
	{ &reg[3], 1, "REG3" },
	{ &reg[4], 1, "REG4" },
	{ &reg[5], 1, "REG5" },
	{ &reg[6], 1, "REG6" },
	{ &reg[7], 1, "REG7" },
	{ &reg[8], 1, "REG8" },
	{ &reg[9], 1, "REG9" },
	{ &reg[10], 1, "REGA" },
	{ &reg[11], 1, "REGB" },
	{ &reg[12], 1, "REGC" },
	{ &reg[13], 1, "REGD" },
	{ &reg[14], 1, "REGE" },
	{ &reg[15], 1, "REGF" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 2 | FCEUSTATE_RLSB, "IRQC" },
	{ &IRQLatch, 2 | FCEUSTATE_RLSB, "IRQL" },	/* need for Famicom Jump II - Saikyou no 7 Nin (J) [!] */
	{ 0 }
};

static void SyncMirror(void) {
	switch (reg[9] & 3) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static void Sync(void) {
	if (is153) {
		int base = (reg[0] & 1) << 4;
		setchr8(0);
		setprg16(0x8000, (reg[8] & 0x0F) | base);
		setprg16(0xC000, 0x0F | base);
	} else {
		int i;
		for (i = 0; i < 8; i++) setchr1(i << 10, reg[i]);
		setprg16(0x8000, reg[8]);
		setprg16(0xC000, ~0);
	}
	SyncMirror();
}

static DECLFW(BandaiWrite) {
	A &= 0x0F;
	if (A < 0x0A) {
		reg[A & 0x0F] = V;
		Sync();
	} else
		switch (A) {
		case 0x0A: X6502_IRQEnd(FCEU_IQEXT); IRQa = V & 1; IRQCount = IRQLatch; break;
		case 0x0B: IRQLatch &= 0xFF00; IRQLatch |= V; break;
		case 0x0C: IRQLatch &= 0xFF; IRQLatch |= V << 8; break;
		case 0x0D:
			if (x24c02)
				x24c02_write(V >>5 &1, V >>6 &1);
			else
				x24c01_write(V >>5 &1, V >>6 &1);
			break;
		}
}

static DECLFR(BandaiRead) {
	if (x24c02)
		return (X.DB & 0xEF) | (x24c02_out << 4);
	else
		return (X.DB & 0xEF) | (x24c01_out << 4);
}

static void FP_FASTAPASS(1) BandaiIRQHook(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount < 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
			IRQCount = -1;
		}
	}
}

static void BandaiPower(void) {
	IRQa = 0;
	if (x24c02)
		x24c02_init();
	else
		x24c01_init();
	Sync();
	SetReadHandler(0x6000, 0x7FFF, BandaiRead);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0xFFFF, BandaiWrite);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper16_Init(CartInfo *info) {
	x24c02 = 1;
	is153 = 0;
	info->Power = BandaiPower;
	MapIRQHook = BandaiIRQHook;

	info->battery = 1;
	info->SaveGame[0] = x24c0x_data + 256;
	info->SaveGameLen[0] = 256;
	AddExState(x24c0x_data + 256, 256, 0, "DAT2"); /* 24C02 occupies the upper half; distinct key so older states that carried the unused lower half do not overwrite it */

	GameStateRestore = StateRestore;
	AddExState(&x24c02StateRegs, ~0, 0, 0);
	AddExState(&StateRegs, ~0, 0, 0);
}

void Mapper159_Init(CartInfo *info) {
	x24c02 = 0;
	is153 = 0;
	info->Power = BandaiPower;
	MapIRQHook = BandaiIRQHook;

	info->battery = 1;
	info->SaveGame[0] = x24c0x_data;
	info->SaveGameLen[0] = 128;
	AddExState(x24c0x_data, 128, 0, "DATA");

	GameStateRestore = StateRestore;
	AddExState(&x24c01StateRegs, ~0, 0, 0);
	AddExState(&StateRegs, ~0, 0, 0);
}

/* Famicom jump 2:
 * 0-7: Lower bit of data selects which 256KB PRG block is in use.
 * This seems to be a hack on the developers' part, so I'll make emulation
 * of it a hack(I think the current PRG block would depend on whatever the
 * lowest bit of the CHR bank switching register that corresponds to the
 * last CHR address read).
 */

static void M153Power(void) {
	Sync();
	setprg8r(0x10, 0x6000, 0);
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, BandaiWrite);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}


static void M153Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void Mapper153_Init(CartInfo *info) {
	is153 = 1;
	info->Power = M153Power;
	info->Close = M153Close;
	MapIRQHook = BandaiIRQHook;

	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

/* Datach Barcode Battler */

static uint8_t BarcodeData[256];
static int BarcodeReadPos;
static int BarcodeCycleCount;
static uint32_t BarcodeOut;

/* #define INTERL2OF5 */

int FCEUI_DatachSet(uint8_t *rcode) {
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
	uint8_t code[13 + 1];
	uint32_t tmp_p = 0;
	uint32_t csum = 0;
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

		for (i = 0; i < 6; i++)
			if (prefix_parity_type[code[0]][i]) {
				for (j = 0; j < 7; j++) {
					BS(data_left_even[code[i + 1]][j]);
				}
			} else
				for (j = 0; j < 7; j++) {
					BS(data_left_odd[code[i + 1]][j]);
				}

		/* Center guard bars */
		BS(0); BS(1); BS(0); BS(1); BS(0);

		for (i = 7; i < 12; i++)
			for (j = 0; j < 7; j++) {
				BS(data_right[code[i]][j]);
			}
		/* Calc and write down the control code if not assigned, instead, send code as is
		   Battle Rush uses modified type of codes with different control code calculation */
		if (len == 12) {
			for (i = 0; i < 12; i++)
				csum += code[i] * ((i & 1) ? 3 : 1);
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

		for (i = 4; i < 7; i++)
			for (j = 0; j < 7; j++) {
				BS(data_right[code[i]][j]);
			}
		csum = 0;
		for (i = 0; i < 7; i++)
			csum += (i & 1) ? code[i] : (code[i] * 3);
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

static void BarcodeSync(void) {
	setchr8(0);
	setprg16(0x8000, (reg[8] & 0x0F));
	setprg16(0xC000, 0x0F);
	SyncMirror();
}

static DECLFW(BarcodeWrite) {
	A &= 0x0F;
	switch (A) {
	case 0x00:
		reg[0] = (V & 8) << 2;
		V = reg[0xD] | reg[0];
		x24c01_write(V >>5 &1, V >>6 &1);
		break;		/* extra EEPROM x24C01 used in Battle Rush mini-cart */
	case 0x08: 
	case 0x09: reg[A] = V; BarcodeSync(); break;
	case 0x0A: X6502_IRQEnd(FCEU_IQEXT); IRQa = V & 1; IRQCount = IRQLatch; break;
	case 0x0B: IRQLatch &= 0xFF00; IRQLatch |= V; break;
	case 0x0C: IRQLatch &= 0xFF; IRQLatch |= V << 8; break;
	case 0x0D:
		x24c02_write(V >>5 &1, V >>6 &1);
		reg[0xD] = V & (~0x20);
		V = reg[0xD] | reg[0];
		x24c01_write(V >>5 &1, V >>6 &1);
		break;
	}
}

static void FP_FASTAPASS(1) BarcodeIRQHook(int a) {
	BandaiIRQHook(a);

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

static DECLFR(BarcodeRead) {
	return (X.DB & 0xE7) | ((x24c02_out | x24c01_out) << 4) | BarcodeOut;
}

static void M157Power(void) {
	IRQa = 0;
	BarcodeData[0] = 0xFF;
	BarcodeReadPos = 0;
	BarcodeOut = 0;
	BarcodeCycleCount = 0;

	x24c01_init();
	x24c02_init();
	BarcodeSync();

	SetReadHandler(0x6000, 0x7FFF, BarcodeRead);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, BarcodeWrite);
}

void Mapper157_Init(CartInfo *info) {
	x24c02 = 1;
	info->Power = M157Power;
	MapIRQHook = BarcodeIRQHook;

	GameInfo->cspecial = SIS_DATACH;
	info->battery = 1;
	info->SaveGame[0] = x24c0x_data;
	info->SaveGameLen[0] = 512;
	GameStateRestore = StateRestore;
	AddExState(x24c0x_data, 512, 0, "DATA");
	AddExState(&x24c01StateRegs, ~0, 0, 0);
	AddExState(&x24c02StateRegs, ~0, 0, 0);
	AddExState(&StateRegs, ~0, 0, 0);
}
