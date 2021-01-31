/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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

/* Rewrite by NewRisingSun.

   Mapper 422: "Normal" version of the mapper. Represents UNIF boards BS-400R and BS-4040R.
   Mapper 126: Power Joy version of the mapper, connecting CHR A18 and A19 in reverse order.
   Mapper 534: Waixing version of the mapper, inverting the reload value of the MMC3 scanline counter.
*/

#include "mapinc.h"
#include "mmc3.h"

static bool reverseCHR_A18_A19;
static bool invertC000;
static uint8 dipSwitch;

static void syncPRG(uint32 A, uint8 V) {
	int prgAND = EXPREGS[0] &0x40? 0x0F: 0x1F;
	int prgOR  =(EXPREGS[0] <<4 &0x70 | EXPREGS[0] <<3 &0x180) &~prgAND;
	switch(EXPREGS[3] &3) {
		case 0: break;
		case 1:
		case 2: V =DRegBuf[6] &~1 | A >>13 &1;
			break;
		case 3: V =DRegBuf[6] &~3 | A >>13 &3;
			break;
	}
	setprg8(A, V &prgAND | prgOR);	
}

static void syncCHR(uint32 A, uint8 V) {
	int chrAND = EXPREGS[0] &0x80? 0x7F: 0xFF;
	int chrOR;
	if (reverseCHR_A18_A19)
		chrOR =(EXPREGS[0] <<4 &0x080 | EXPREGS[0] <<3 &0x100 | EXPREGS[0] <<5 &0x200) &~chrAND;
	else
		chrOR =EXPREGS[0] <<4 &0x380 &~chrAND;
	
	if (EXPREGS[3] &0x10)
		setchr8(EXPREGS[2] &0x0F | chrOR >>3);
	else
		setchr1(A, (V & chrAND) | chrOR);
}

static DECLFW(writeWRAM) {
	if (~EXPREGS[3] &0x80) {
		// Lock bit clear: Update any outer bank register
		EXPREGS[A &3] =V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else
	if ((A &3) ==2) {
		// Lock bit set: Only update the bottom two bits of the CNROM bank
		EXPREGS[2] &=~3;
		EXPREGS[2] |= V &3;
		FixMMC3CHR(MMC3_cmd);
	}
	CartBW(A, V);
}

static DECLFR(readDIP) {
	uint8 result =CartBR(A);
	if (EXPREGS[1] &1) result =result &~3 | dipSwitch &3;
	return result;
}

static DECLFW(writeIRQ) {
	MMC3_IRQWrite(A, V ^0xFF);
}

static void reset(void) {
	dipSwitch++;
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	MMC3RegReset();
}

static void power(void) {
	dipSwitch =0;
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, writeWRAM);
	SetReadHandler(0x8000, 0xFFFF, readDIP);
	if (invertC000) SetWriteHandler(0xC000, 0xDFFF, writeIRQ);
}

static void init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = syncCHR;
	pwrap = syncPRG;

	info->Power = power;
	info->Reset = reset;

	AddExState(EXPREGS, 4, 0, "EXPR");
	AddExState(&dipSwitch, 1, 0, "DPSW");
}

void Mapper126_Init(CartInfo *info) {
	reverseCHR_A18_A19 = true;
	invertC000 = false;
	init(info);
}

void Mapper422_Init(CartInfo *info) {
	reverseCHR_A18_A19 = false;
	invertC000 = false;
	init(info);
}

void Mapper534_Init(CartInfo *info) {
	reverseCHR_A18_A19 = false;
	invertC000 = true;
	init(info);
}
