/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright (C) 2019 Libretro Team
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
 * UNIF: BMC-60311C:
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_289
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg[2];
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
	uint32 bank = reg[1] & 0x7F;

	if (reg[0] & 0x02) {
		setprg16(0x8000, (bank & ~0x07) | ((reg[0] & 0x01) ? 0x07 : (latch.data & 0x07)));
		setprg16(0xC000, bank | 0x07);
	} else {
		setprg16(0x8000, bank & ~(reg[0] & 0x01));
		setprg16(0xC000, bank |  (reg[0] & 0x01));
	}
	/* CHR-RAM write-protect */
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, ((reg[0] >> 2) & 0x01) ^ 0x01);
	setchr8(0);
	setmirror(((reg[0] >> 3) & 0x01) ^ 0x01);
}

static DECLFR(M289Read) {
	return (cpu.openbus & ~0x03) | (dipsw & 0x03);
}

static DECLFW(M289Write) {
	reg[A & 0x01] = V;
	Sync();
}

static void M289Power(void) {
	dipsw = 0;
	reg[0] = reg[1] = 0;
	Latch_Power();
	SetReadHandler(0x6000, 0x7FFF, M289Read);
	SetWriteHandler(0x6000, 0x7FFF, M289Write);
}

static void M289Reset(void) {
	dipsw++;
	reg[0] = reg[1] = 0;
	Latch_RegReset();
}

void Mapper289_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M289Power;
	info->Reset = M289Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
