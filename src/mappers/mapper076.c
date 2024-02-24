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

/* iNES Mapper 076
 * iNES Mapper 076 represents NAMCOT-3446, a board used onlyfor the game
 * Megami Tensei: Digital Devil Story.
 * It rewires the Namcot 108 mapper IC to be able to address 128 KiB of CHR,
 * in exchange for coarser CHR banking.
 * The PCB also has a 7432 which allows the use of a 28-pin CHR-ROM.
 * https://www.nesdev.org/wiki/INES_Mapper_076
 */

#include "mapinc.h"
#include "n118.h"

static void M076PW(uint16 A, uint16 V) {
    setprg8(A, V & 0x1F); /* support for PRG bank for fan translations */
}

static void M076FixCHR(void) {
    setchr2(0x0000, n118.reg[2] & 0x3F);
    setchr2(0x0800, n118.reg[3] & 0x3F);
    setchr2(0x1000, n118.reg[4] & 0x3F);
    setchr2(0x1800, n118.reg[5] & 0x3F);
}

void Mapper076_Init(CartInfo *info) {
	N118_Init(info, 0, 0);
    N118_pwrap = M076PW;
    N118_FixCHR = M076FixCHR;
}
