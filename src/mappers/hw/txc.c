/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2019 Libretro Team
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
 * TXC/Micro Genius simplified mapper
 * updated 06-2019 http://wiki.nesdev.com/w/index.php/INES_Mapper_036
 *
 * Known games:
 * - Strike Wolf (Asia) (Unl)
 * - Policeman (Gluk Video) (unl)
 * - F-15 City War (Spain) (Gluk Video) (Unl)
 *
 * TXC mappers, originally much complex banksitching
 *
 * 01-22111-000 (05-00002-010) (132, 22211) - MGC-001 Qi Wang
 * 01-22110-000 (52S         )              - MGC-002 2-in-1 Gun
 * 01-22111-100 (02-00002-010) (173       ) - MGC-008 Mahjong Block
 *                             (079       ) - MGC-012 Poke Block
 * 01-22110-200 (05-00002-010) (036       ) - MGC-014 Strike Wolf
 * 01-22000-400 (05-00002-010) (036       ) - MGC-015 Policeman
 * 01-22017-000 (05-PT017-080) (189       ) - MGC-017 Thunder Warrior
 * 01-11160-000 (04-02310-000) (   , 11160) - MGC-023 6-in-1
 * 01-22270-000 (05-00002-010) (132, 22211) - MGC-xxx Creatom
 * 01-22200-400 (------------) (079       ) - ET.03   F-15 City War
 *                             (172       ) -         1991 Du Ma Racing
 *
 */

/* added 2020-2-16
 * Updated based on latest source
 * Mappers 36, 132, 173
 * Mappers 136, 147, 172
 */

#include "mapinc.h"
#include "txc.h"

TXC txc;

static void Dummyfunc(void) { }
static void (*WSync)(void) = Dummyfunc;

static SFORMAT StateRegs[] =
{
	{ &txc.accumulator, 1, "ACC0" },
	{ &txc.inverter,    1, "INVR" },
	{ &txc.staging,     1, "STG0" },
	{ &txc.output,      1, "OUT0" },
	{ &txc.increase,    1, "INC0" },
	{ &txc.X,           1, "XFLG" },
	{ &txc.Y,           1, "YFLG" },
	{ &txc.invert,      1, "INVT" },
	{ 0 }
};

DECLFR(TXC_Read) {
	uint8 ret = cpu.openbus;
	if ((A & 0x103) == 0x100) {
		ret = ((txc.accumulator & 0x07) | ((txc.inverter ^ txc.invert) & ~0x07));
		txc.Y = txc.X || ((ret & 0x10) != 0);
		WSync();
	}
	return ret;
}

DECLFW(TXC_Write) {
	if (A & 0x8000) {
		txc.output = (txc.accumulator & 0x0F) | ((txc.inverter << 1) & 0x10);
	} else {
		switch (A & 0x103) {
		case 0x100:
			if (txc.increase) {
				txc.accumulator++;
			} else {
				txc.accumulator = ((txc.accumulator & ~0x07) | ((txc.staging ^ txc.invert) & 0x07));
			}
			break;
		case 0x101:
			txc.invert = (V & 0x01) ? 0xFF : 0x00;
			break;
		case 0x102:
			txc.staging  = V & 0x07;
			txc.inverter = V & ~0x07;
			break;
		case 0x103:
			txc.increase = ((V & 0x01) != 0);
			break;
		}
	}
    txc.X = txc.invert ? txc.A : txc.B;
	txc.Y = txc.X || ((V & 0x10) != 0);
	WSync();
}

static void TXCRegReset(void) {
	WSync();
}

void TXC_Power(void) {
	txc.output      = 0;
	txc.accumulator = 0;
	txc.inverter    = 0;
	txc.staging     = 0;
	txc.increase    = 0;
    txc.invert      = 0;
	txc.X           = 0;
	txc.Y           = 0;
	txc.A           = 0;
	txc.B           = 1;
	TXCRegReset();
}

static void StateRestore(int version) {
	WSync();
}

void TXC_Init(CartInfo *info, void (*proc)(void)) {
	WSync = proc;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
