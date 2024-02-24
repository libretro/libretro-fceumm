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
 *
 * NES 2.0 - Mapper 401 (reference from NewRisingSun)
 * Super 19-in-1 (VIP19) (crc 0x2F497313)
 *
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 cmd;
static uint8 dipsw = 0;

static void M401CW(uint16 A, uint16 V) {
	uint32 mask = (0xFF >> (~reg[2] & 0xF));
	uint32 bank = (reg[0] | ((reg[2] << 4) & 0xF00));

	setchr1(A, bank | (V & mask));
}

static void M401PW(uint16 A, uint16 V) {
	uint32 mask = (~reg[3] & 0x1F);
	uint32 bank = (reg[1] & 0x1F) | (reg[2] & 0x80) |
	    ((dipsw & 2) ? (reg[2] & 0x20) : ((reg[1] >> 1) & 0x20)) |
	    ((dipsw & 4) ? (reg[2] & 0x40) : ((reg[1] << 1) & 0x40));
	setprg8(A, bank | (V & mask));
}

static DECLFR(M401Read) {
	if ((dipsw & 0x01) && (reg[1] & 0x80)) {
		return cpu.openbus;
	}
	return CartBR(A);
}

static DECLFW(M401Write) {
	/* FCEU_printf("Wr A:%04x V:%02x index:%d\n", A, V, cmd); */
	if (!(reg[3] & 0x40)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 0x03;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
	CartBW(A, V);
}

static void M401Reset(void) {
	dipsw = (dipsw + 1) & 7;
	FCEU_printf("dipsw = %d\n", dipsw);
	reg[0] = 0x00;
	reg[1] = 0x00;
	reg[2] = 0x0F;
	reg[3] = 0x00;
	cmd = 0x00;
	MMC3_Reset();
}

static void M401Power(void) {
	dipsw = 7;
	reg[0] = 0x00;
	reg[1] = 0x00;
	reg[2] = 0x0F;
	reg[3] = 0x00;
	cmd = 0x00;
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, M401Read);
	SetWriteHandler(0x6000, 0x7FFF, M401Write);
}

void Mapper401_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_cwrap = M401CW;
	MMC3_pwrap = M401PW;
	info->Power = M401Power;
	info->Reset = M401Reset;
	AddExState(reg, 4, 0, "EXPR");
	AddExState(&cmd, 1, 0, "CMD0");
	AddExState(&dipsw, 1, 0, "DPSW");
}
