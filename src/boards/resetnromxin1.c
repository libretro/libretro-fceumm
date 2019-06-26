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

/* BMC-RESETNROM-XIN1
 * - Sheng Tian 2-in-1(Unl,ResetBase)[p1] - Kung Fu (Spartan X), Super Mario Bros (alt)
 * - Sheng Tian 2-in-1(Unl,ResetBase)[p2] - B-Wings, Twin-bee
 *
 * BMC-KS106C
 * - Kaiser 4-in-1(Unl,KS106C)[p1] - B-Wings, Kung Fu, 1942, SMB1 (wrong mirroring)
 */

#include "mapinc.h"

static uint8 gameblock, limit;

static SFORMAT StateRegs[] =
{
	{ &gameblock, 1, "GAME" },
	{ 0 }
};

static void Sync(void) {
	setchr8r(gameblock, 0);
	setprg32r(gameblock, 0x8000, 0);
}

static void BMCRESETNROMXIN1Power(void) {
	gameblock = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void BMCRESETNROMXIN1Reset(void) {
	gameblock++;
	gameblock &= limit;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void BMCRESETNROMXIN1_Init(CartInfo *info) {
	uint8 x;
	uint64 partialmd5 = 0;

	for (x = 0; x < 8; x++)
		partialmd5 |= (uint64)info->MD5[15 - x] << (x * 8);

	/* Mirroring override - these boards have incorrect mirroring for some reasons in their headers */
	if (partialmd5 == 0x616851e56946893bLL) /* Sheng Tian 2-in-1(Unl,ResetBase)[p1].unf */
		SetupCartMirroring(1, 1, NULL);
	else if (partialmd5 == 0x4cd729b5ae23a3cfLL) /* Sheng Tian 2-in-1(Unl,ResetBase)[p2].unf */
		SetupCartMirroring(0, 1, NULL);

	limit = 0x01;
	info->Power = BMCRESETNROMXIN1Power;
	info->Reset = BMCRESETNROMXIN1Reset;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

void BMCKS106C_Init(CartInfo *info) {
	limit = 0x03;
	info->Power = BMCRESETNROMXIN1Power;
	info->Reset = BMCRESETNROMXIN1Reset;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
