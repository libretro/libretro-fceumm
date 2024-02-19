/* FCEUmm - NES/Famicom Emulator
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

#include "mapinc.h"
#include "mmc3.h"

static void M430CW(uint32 A, uint8 V) {
	uint8 mask = (mmc3.expregs[0] & 4) ? 0x7F : 0xFF;
	setchr1(A, ((mmc3.expregs[0] << 6) & ~mask) | (V & mask));
}

static void M430PW(uint32 A, uint8 V) {
	if (mmc3.expregs[0] & 8) {
		setprg8(0x8000, ((mmc3.expregs[0] << 4) & 0x30) | ((mmc3.regs[6] & ~2) & 0x0F));
		setprg8(0xA000, ((mmc3.expregs[0] << 4) & 0x30) | ((mmc3.regs[7] & ~2) & 0x0F));
		setprg8(0xC000, ((mmc3.expregs[0] << 4) & 0x30) | ((mmc3.regs[6] |  2) & 0x0F));
		setprg8(0xE000, ((mmc3.expregs[0] << 4) & 0x30) | ((mmc3.regs[7] |  2) & 0x0F));
	} else {
		setprg8(A, ((mmc3.expregs[0] << 4) & 0x30) | (V & 0x0F));
	}
}

static DECLFW(M430Write) {
	if (MMC3CanWriteToWRAM()) {
	    mmc3.expregs[0] = A & 0xFF;
	    FixMMC3PRG(mmc3.cmd);        
	    FixMMC3CHR(mmc3.cmd);
    }
}

static void M430Reset(void) {
	mmc3.expregs[0] = 0;
	MMC3RegReset();
}

static void M430Power(void) {
	mmc3.expregs[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M430Write);
}

void Mapper430_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 256, 0, 0);
	mmc3.cwrap = M430CW;
	mmc3.pwrap = M430PW;
	info->Reset = M430Reset;
	info->Power = M430Power;
	AddExState(mmc3.expregs, 1, 0, "EXPR");
}
