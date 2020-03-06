/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020 negativeExponent
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

/* Mapper 369 (BMC-N49C-300) - Super Mario Bros. Party multicart */

#include "mapinc.h"
#include "mmc3.h"

static uint8 M40IRQa;
static uint16 M40IRQCount;

static void M369PW(uint32 A, uint8 V) {
	switch (EXPREGS[0]) {
	case 0x00:
		setprg32(0x8000, 0);
		break;
	case 0x01:
		setprg32(0x8000, 1);
		break;
	case 0x13:
		setprg8(0x6000, 0x0E);
		setprg8(0x8000, 0x0C);
		setprg8(0xa000, 0x0D);
		setprg8(0xc000, EXPREGS[1] | 0x08);
		setprg8(0xe000, 0x0F);
		break;
	case 0x37:
		setprg8r(0x10, 0x6000, 0);
		setprg8(A, (V & 0x0F) | 0x10);
		break;
	case 0xFF:
		setprg8r(0x10, 0x6000, 0);
		setprg8(A, (V & 0x1F) | 0x20);
		break;
	}
}

static void M369CW(uint32 A, uint8 V) {
	switch (EXPREGS[0]) {
	case 0x00:
		setchr8(0);
		break;
	case 0x01:
		setchr8(1);
		break;
	case 0x13:
		setprg8(0xE000, 0xF);
		setchr8(3);
		break;
	case 0x37:
		setchr1(A, (V & 0x7F) | 0x80);
		break;
	case 0xFF:
		setchr1(A, V | 0x100);
		break;
	}
}

static DECLFW(M369WriteCMD) {
	EXPREGS[0] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static DECLFW(M369Write) {
	if (EXPREGS[0] == 0x13) {
		switch (A & 0xE000) {
		case 0x8000:
			M40IRQa = 0;
			M40IRQCount = 0;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 0xA000:
			M40IRQa = 1;
			break;
		case 0xE000:
			EXPREGS[1] = V & 7;
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
			break;
		}
	} else {
		if (A < 0xC000) {
			MMC3_CMDWrite(A, V);
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
		} else
			MMC3_IRQWrite(A, V);
	}
}

static void M369Power(void) {
	EXPREGS[0]  = 0x00;
	EXPREGS[1]  = 0x00;
	M40IRQa     = 0x00;
	M40IRQCount = 0x00;
	GenMMC3Power();
	SetWriteHandler(0x4120, 0x4120, M369WriteCMD);
	SetWriteHandler(0x8000, 0xFFFF, M369Write);
}

static void M369Reset(void) {
	EXPREGS[0]  = 0x00;
	EXPREGS[1]  = 0x00;
	M40IRQa     = 0x00;
	M40IRQCount = 0x00;
	MMC3RegReset();
}

static void FP_FASTAPASS(1) M40IRQHook(int a) {
   if (EXPREGS[0] == 0x13)
   {
	  if (M40IRQa)
	  {
		 if (M40IRQCount < 4096)
			M40IRQCount += a;
		 else
		 {
			M40IRQa = 0;
			X6502_IRQBegin(FCEU_IQEXT);
		 }
	  }
   }
}

void Mapper369_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 384, 8, 0);
	pwrap = M369PW;
	cwrap = M369CW;
	MapIRQHook = M40IRQHook;
	info->Power = M369Power;
	info->Reset = M369Reset;
	AddExState(EXPREGS, 4, 0, "EXPR");
	AddExState(&M40IRQa, 1, 0, "M2a");
	AddExState(&M40IRQCount, 2 | FCEUSTATE_RLSB, 0, "M2CN");
}
