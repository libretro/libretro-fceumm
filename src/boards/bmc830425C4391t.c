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
 *
 */

/* NES 2.0 Mapper 320 is used for the Super HiK 6-in-1 A-030 multicart.
 * Basically UxROM with an address-latch-based outer bank register.
 * UNIF board name is BMC-830425C-4391T. Mirroring is hard-wired.
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_320
 */

#include "mapinc.h"

static uint8 regs[2];
static uint8 isMermaid = 0;

static SFORMAT StateRegs[] =
{
	{ regs, 2, "REGS" },
	{ &isMermaid, 1, "MERM" },
	{ 0 }
};

static void Sync(void) {
	uint8 bank_size = (regs[0] & 0x80) ? 0x07: 0x0F;
	uint8 outer_bank = !isMermaid ? (regs[0] & 0x78) : 0x04;
	uint8 inner_bank = regs[1];
	uint8 game_block = regs[0] & 0x78;

	/* TODO: Hacky and forced mapping of outer banks(game block)... */
	if (!isMermaid) {
		switch(game_block) {
		case 0x00: /* botb outerbank=0 */
		case 0x10: /* rocketeer outerbank=1  */
		case 0x20: /* contra outerbank=2  */
		case 0x28: /* ducktales outerbank=3 */
		case 0x38: /* school fight outerbank=5 */
			outer_bank = (game_block >> 4) + ((game_block & 0x08) ? 0x01 : 0x00);
			outer_bank |= (game_block >> 3) & 1;
			break;
		case 0x30:
			/* Mermaid game block is tricky (to me at least). Aim here is
			 * when this register is selected, ignore any other outer bank reg changes */
			isMermaid = 1;
			outer_bank = 0x04;
			break;
		}
	}

	setprg16r(outer_bank, 0x8000, (inner_bank & bank_size));
	setprg16r(outer_bank, 0xC000, bank_size);
	setchr8(0);
}

static DECLFW(M320Write) {
	if ((A & 0xF0E0) == 0xF0E0) {
		regs[0] = (A & 0x1F) << 3;
	}
	regs[1] = V & 0x0F;
	Sync();
}

static void M320Power(void) {
	regs[0] = regs[1] = 0;
	isMermaid = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M320Write);
}

static void M320Reset(void) {
	regs[0] = regs[1] = 0;
	isMermaid = 0;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void BMC830425C4391T_Init(CartInfo *info) {
	info->Power = M320Power;
	info->Reset = M320Reset;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
