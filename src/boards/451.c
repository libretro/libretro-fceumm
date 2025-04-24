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

/* Missing feature: MP3 playback, used by Haratyler MP */

#include "mapinc.h"
#include "mmc3.h"
#include "flashrom.h"

static void PRGWrap(uint32 A, uint8 V) {
	if (A ==0x8000) V =0x00; else
	if (A ==0xE000) V =0x30;
	setprg8(A, V);
}

static DECLFW(Write) {
	flashrom_write(A &0x1FFF | (Page[A >>11] +A -PRGptr[0]) &~0x1FFF, V);
	switch (A &0xE000) {
		case 0xA000: MMC3_CMDWrite(0xA000, A &1);
		             break;
		case 0xC000: MMC3_IRQWrite(0xC000, (A &0xFF) -1),
		             MMC3_IRQWrite(0xC001, 0);
			     MMC3_IRQWrite((A &0xFF) ==0xFF? 0xE000: 0xE001, 0);
			     break;
		case 0xE000: V =A <<2 &8 | A &1;
		             MMC3_CMDWrite(0x8000, 0x40); MMC3_CMDWrite(0x8001, V <<3 |0); 
		             MMC3_CMDWrite(0x8000, 0x41); MMC3_CMDWrite(0x8001, V <<3 |2); 
		             MMC3_CMDWrite(0x8000, 0x42); MMC3_CMDWrite(0x8001, V <<3 |4); 
		             MMC3_CMDWrite(0x8000, 0x43); MMC3_CMDWrite(0x8001, V <<3 |5); 
		             MMC3_CMDWrite(0x8000, 0x44); MMC3_CMDWrite(0x8001, V <<3 |6); 
		             MMC3_CMDWrite(0x8000, 0x45); MMC3_CMDWrite(0x8001, V <<3 |7); 
		             MMC3_CMDWrite(0x8000, 0x46); MMC3_CMDWrite(0x8001, V |0x20); 
     		             MMC3_CMDWrite(0x8000, 0x47); MMC3_CMDWrite(0x8001, V |0x10);
			     break;
	}
}

static void Power(void) {
	GenMMC3Power();
	SetReadHandler(0x8000, 0xFFFF, flashrom_read);
	SetWriteHandler(0x8000, 0xFFFF, Write);
}

void Mapper451_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 32, 0, 0);
	pwrap = PRGWrap;
	flashrom_init (0x37, 0x86, 65536, 0x555, 0x2AA, 0x0FFF); /* AMIC A29040B */
	info->Power =Power;
	info->SaveGame[0] =PRGptr[0];
	info->SaveGameLen[0] =PRGsize[0];
	MapIRQHook =flashrom_cpuCycle;
}
