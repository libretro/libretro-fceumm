/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

/* 2019-05-29 - Mapper 224, KT-008
 * iNES Mapper 224 is used for the 晶科泰 (Jncota) KT-008 PCB.
 * It's an MMC3 clone that supports 1024 KiB of PRG-ROM through an additional
 * outer bank register at $5000.
 * http://wiki.nesdev.com/w/index.php/INES_Mapper_224 */

#include "mapinc.h"
#include "mmc3.h"

static void M224PW(uint32 A, uint8 V) {
	setprg8(A, (V & 0x3f) | ((EXPREGS[0] << 4) & 0x40));
}

static DECLFW(Mapper224Write) {
	EXPREGS[0] = V;
	FixMMC3PRG(MMC3_cmd);
}

static void M224Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x5000, 0x5000, Mapper224Write);
}

void Mapper224_Init(CartInfo *info) {
	GenMMC3_Init(info, 1024, 0, 8, 0);
	pwrap = M224PW;
	info->Power = M224Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
