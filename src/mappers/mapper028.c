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

/* added 2019-5-23
 * Mapper 28 - Action 53
 * http://wiki.nesdev.com/w/index.php/INES_Mapper_028 */

#include "mapinc.h"

static uint8 cmd;
static uint8 reg[4];

static SFORMAT StateRegs[] = {
	{&cmd, 1, "REG"},
	{reg, 4, "REGS"},
	{0}
};

static uint8 bank_size_masks[4] = { 0x01, 0x03, 0x07, 0x0F };
static uint16 GetPRGBank(int V) {
	uint16 cpu_a14 = V & 0x01;
    uint16 outer_bank = reg[3] << 1;
    uint16 bank_mode = reg[2] >> 2;  /* discard mirroring bits */
	uint16 current_bank = reg[1];
	uint16 bank_size_mask = 0;

    if (((bank_mode ^ cpu_a14) & 0x03) == 0x02) { /* in UNROM fixed bank? */
        bank_mode = 0;  /* if so, treat as NROM */
	}
    if ((bank_mode & 0x02) == 0) { /* in 32K bank mode? */
        current_bank = (current_bank << 1) | cpu_a14;
	}
    bank_size_mask = bank_size_masks[(bank_mode >> 2) & 3];
    return ((current_bank & bank_size_mask) | (outer_bank & ~bank_size_mask));
}

static void Sync(void) {
	setprg16(0x8000, GetPRGBank(0));
	setprg16(0xC000, GetPRGBank(1));
	setchr8(reg[0] & 0x03);
	switch (reg[2] & 0x03) {
	case 0: setmirror(MI_0); break;
	case 1: setmirror(MI_1); break;
	case 2: setmirror(MI_V); break;
	case 3: setmirror(MI_H); break;
	}
}

static DECLFW(WriteCMD) {
	cmd = ((V >> 6) & 0x02) | (V & 0x01);
}

static DECLFW(WriteREG) {
	reg[cmd] = V;
	if (!(cmd & 0x02) && !(reg[2] & 0x02)) {
		reg[2] &= ~0x01;
		reg[2] |= (V >> 4) & 0x01;
	}
	Sync();
}

static void M028Power(void) {
	reg[0] = ~0;
	reg[1] = ~0;
	reg[2] = ~0;
	reg[3] = ~0;
	Sync();

	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	
	SetWriteHandler(0x5000, 0x5FFF, WriteCMD);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, WriteREG);
}

static void M028Reset(void) {
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper028_Init(CartInfo *info) {
	info->Power = M028Power;
	info->Reset = M028Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
