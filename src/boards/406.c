/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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

#include "mapinc.h"
#include "mmc3.h"
#include "flashrom.h"

static uint8 submapper;

static DECLFW(Write) {
	flashrom_write(A &0x1FFF | (Page[A >>11] +A -PRGptr[0]) &~0x1FFF, V);

	if (submapper ==1)
		A =A <0xA000 || A >=0xE000? (A ^0x6000): A;
	else
		A =A &0xE000 | A >>1 &0x0001;
	
	if (A &0x4000)
		MMC3_IRQWrite(A, V);
	else
		MMC3_CMDWrite(A, V);
}

static void Power(void) {
	GenMMC3Power();
	SetReadHandler(0x8000, 0xFFFF, flashrom_read);
	SetWriteHandler(0x8000, 0xFFFF, Write);
}

void Mapper406_Init(CartInfo *info) {
	submapper =info->submapper;
	GenMMC3_Init(info, 512, 256, 0, 0);
	flashrom_init (submapper ==1? 0x01: 0xC2, 0xA4, 65536, 0x5555, 0x2AAA, 0x7FFF); /* Macronix MX29F040 (0)/AMD AM29F040 (1) */
	info->Power =Power;
	info->SaveGame[0] =PRGptr[0];
	info->SaveGameLen[0] =PRGsize[0];
	MapIRQHook =flashrom_cpuCycle;
}
