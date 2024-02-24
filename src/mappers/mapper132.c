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
 */

#include "mapinc.h"
#include "txc.h"

/**** LEGACY MAPPER IMPLEMENTATION ****/

static uint8 reg[4];

static SFORMAT UNL22211StateRegs[] = {
	{ reg, 4, "REGS" },
	{ 0 }
};

static void UNL22211Sync(void) {
	setprg32(0x8000, (reg[2] >> 2) & 1);
	setchr8(reg[2] & 3);
}

static DECLFW(UNL22211WriteLo) {
	if (A & 0x100) {
		reg[A & 3] = V;
		UNL22211Sync();
	}
}

static DECLFR(UNL22211ReadLo) {
	return ((reg[1] ^ reg[2]) | 0x40);
}

static void UNL22211Power(void) {
	UNL22211Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x4100, UNL22211ReadLo);
	SetWriteHandler(0x4100, 0x4FFF, UNL22211WriteLo);
}

static void UNL22211StateRestore(int version) {
	UNL22211Sync();
}

static void UNL22211_Init(CartInfo *info) {
	info->Power      = UNL22211Power;
	GameStateRestore = UNL22211StateRestore;
	AddExState(&UNL22211StateRegs, ~0, 0, 0);
}

/* Mapper 132 */

static void M132Sync(void) {
	setprg32(0x8000, (txc.output >> 2) & 0x01);
	setchr8(txc.output & 0x03);
}

static DECLFW(M132Write) {
	TXC_Write(A, V & 0x0F);
}

static DECLFR(M132Read) {
	return ((cpu.openbus & 0xF0) | (TXC_Read(A) & 0x0F));
}

static void M132Power(void) {
	TXC_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, M132Read);
	SetWriteHandler(0x4100, 0xFFFF, M132Write);
}

void Mapper132_Init(CartInfo *info) {
	if ((info->CRC32) == 0x2A5F4C5A) {
		/* Jin Gwok Sei Chuen Saang (Ch) [U][!] */
		FCEU_printf(" WARNING: Using alternate mapper implementation.\n");
		UNL22211_Init(info);
        return;
	}
	TXC_Init(info, M132Sync);
	info->Power = M132Power;
}
