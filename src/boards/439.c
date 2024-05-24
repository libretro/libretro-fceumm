/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

/* NES 2.0 Mapper 439 denotes the YS2309 multicart PCB.
 * Its UNIF MAPRs are BMC-DS-07 and BMC-K86B.
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg[2];

static void Sync(void)
{
	setprg16(0x8000, ((reg[0] >> 1) & ~7) | latch.data & 7);
	setprg16(0xC000, ((reg[0] >> 1) & ~7) | 7);
	setchr8(0);
	setmirror(((latch.data >> 7) & 1) ^ 1);
}

static DECLFW(M439WriteReg)
{
    reg[A & 1] = V;
    Sync();
}

static DECLFW(M439WriteLatch)
{
    if (!(reg[0] & 0x80))
        latch.data = (latch.data & ~7) | (V & 7);
    else
        latch.data = V;
    Sync();
}

static void M439Reset(void)
{
    reg[0] = reg[1] = ~0;
    LatchHardReset();
}

static void M439Power(void)
{
    reg[0] = reg[1] = ~0;
    LatchPower();
    SetWriteHandler(0x6000, 0x7FFF, M439WriteReg);
    SetWriteHandler(0x8000, 0xFFFF, M439WriteLatch);
}

void Mapper439_Init(CartInfo *info)
{
	Latch_Init(info, Sync, NULL, 0, 1);
   info->Power = M439Power;
   info->Reset = M439Reset;
   AddExState(reg, 2, 0, "REGS");
}
