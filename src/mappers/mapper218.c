/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 * Copyright (C) 2023
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "mapinc.h"

extern uint8 **VPageR;
static const uint8 mirrorings[]  = { MI_V, MI_H, MI_0, MI_1 };
static const uint8 mapping[4][8] = {
	{ 0, 1, 0, 1, 0, 1, 0, 1 }, /* MI_V */
	{ 0, 0, 1, 1, 0, 0, 1, 1 }, /* MI_H */
	{ 0, 0, 0, 0, 1, 1, 1, 1 }, /* MI_0 */
	{ 0, 0, 0, 0, 0, 0, 0, 0 }  /* MI_1 */
};

static void M218Power(void) {
	setchr8(0);
	setprg32(0x8000, 0);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

void Mapper218_Init(CartInfo *info) {
	info->Power = M218Power;

	/* similar to mapper 30, this mapper interprets the two bits in headers mirroring in idiosyncratic ways */
	SetupCartMirroring(mirrorings[info->mirror2bits], 1, NULL);

	/* cryptic logic to effect the CHR RAM mappings by mapping 1k blocks to NTARAM according to how the pins are wired
	 this could be done by bit logic, but this is self-documenting */
	VPageR[0] = &NTARAM[mapping[info->mirror2bits][0]];
	VPageR[1] = &NTARAM[mapping[info->mirror2bits][1]];
	VPageR[2] = &NTARAM[mapping[info->mirror2bits][2]];
	VPageR[3] = &NTARAM[mapping[info->mirror2bits][3]];
	VPageR[4] = &NTARAM[mapping[info->mirror2bits][4]];
	VPageR[5] = &NTARAM[mapping[info->mirror2bits][5]];
	VPageR[6] = &NTARAM[mapping[info->mirror2bits][6]];
	VPageR[7] = &NTARAM[mapping[info->mirror2bits][7]];
	PPUCHRRAM = 0xFF;
}
