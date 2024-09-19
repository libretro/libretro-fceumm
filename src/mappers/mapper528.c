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

/* NES 2.0 Mapper 528
 * UNIF BMC-831128C
 */

#include "mapinc.h"
#include "fme7.h"
#include "vrcirq.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M528CW(uint16 A, uint16 V) {
	uint16 mask = 0xFF;
	uint16 base = reg << 4;

	setchr1(A, (base | (V & mask)));
}

static void M528PW(uint16 A, uint16 V) {
	uint16 base = reg;
	uint16 mask = base | 0x0F;

	setprg8(A, base + (V & mask));
}

static void M528SyncWRAM(void) {
	uint16 base = reg;
	uint16 mask = base | 0x0F;
	
	if (fme7.prg[0] == 1) {
		setprg8r(0x10, 0x6000, 0);
	} else {
		setprg8(0x6000, (base + (fme7.prg[0] & mask)));
	}
}

static DECLFW(M528Write) {
	switch (A & 0x0F) {
	case 0x0B: break;
	case 0x0D: VRCIRQ_Control(V); break;
	case 0x0E: VRCIRQ_Acknowledge(); break;
	case 0x0F: VRCIRQ_Latch(V); break;
	default:
		FME7_WriteIndex(0x8000, A & 0x0F);
		FME7_WriteReg(0xA000, V);
		break;
	}
	reg = (A & 0x4000) >> 10;
	FME7_FixPRG();
	FME7_FixCHR();
	FME7_FixWRAM();
}

static void M528Power(void) {
	reg = 0;
	FME7_Power();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0xA000, 0xAFFF, M528Write);
	SetWriteHandler(0xC000, 0xCFFF, M528Write);
}

void Mapper528_Init(CartInfo *info) {
	FME7_Init(info, TRUE, info->battery);
	FME7_FixWRAM = M528SyncWRAM;
	FME7_pwrap = M528PW;
	FME7_cwrap = M528CW;
	info->Power = M528Power;
	AddExState(StateRegs, ~0, 0, NULL);

	VRCIRQ_Init(TRUE);
	MapIRQHook = VRCIRQ_CPUHook;
	AddExState(&VRCIRQ_StateRegs, ~0, 0, 0);
}
