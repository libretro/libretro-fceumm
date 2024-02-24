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
#include "jv001.h"

JV001 jv001;

static void Dummyfunc(void) { }
static void (*WSync)(void) = Dummyfunc;

static SFORMAT StateRegs[] =
{
	{ &jv001.accumulator, 1, "ACC0" },
	{ &jv001.inverter,    1, "INVR" },
	{ &jv001.staging,     1, "STG0" },
	{ &jv001.output,      1, "OUT0" },
	{ &jv001.increase,    1, "INC0" },
	{ &jv001.X,           1, "XFLG" },
	{ &jv001.invert,      1, "INVT" },
	{ 0 }
};

DECLFR(JV001_Read) {
	uint8 ret = cpu.openbus;
	if ((A & 0x103) == 0x100) {
		ret = ((jv001.accumulator & 0x0F) | ((jv001.inverter ^ jv001.invert) & ~0x0F));
		WSync();
	}
	return ret;
}

DECLFW(JV001_Write) {
	if (A & 0x8000) {
		jv001.output = (jv001.accumulator & 0x0F) | (jv001.inverter & 0xF0);
	} else {
		switch (A & 0x103) {
		case 0x100:
			if (jv001.increase) {
				jv001.accumulator++;
			} else {
				jv001.accumulator = ((jv001.accumulator & ~0x0F) | ((jv001.staging ^ jv001.invert) & 0x0F));
			}
			break;
		case 0x101:
			jv001.invert = (V & 0x01) ? 0xFF : 0x00;
			break;
		case 0x102:
			jv001.staging  = V & 0x0F;
			jv001.inverter = V & ~0x0F;
			break;
		case 0x103:
			jv001.increase = ((V & 0x01) != 0);
			break;
		}
	}
    jv001.X = jv001.invert ? jv001.A : jv001.B;
	WSync();
}

void JV001_Reset(void) {
	WSync();
}

void JV001_Power(void) {
	jv001.output      = 0;
	jv001.accumulator = 0;
	jv001.inverter    = 0;
	jv001.staging     = 0;
	jv001.increase    = 0;
    jv001.invert      = 0xFF;
	jv001.X           = 0;
	jv001.A           = 0;
	jv001.B           = 1;
	JV001_Reset();
}

static void StateRestore(int version) {
	WSync();
}

void JV001_Init(CartInfo *info, void (*proc)(void)) {
	WSync            = proc;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
