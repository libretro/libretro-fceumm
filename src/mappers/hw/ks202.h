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

#ifndef _KS202_H
#define _KS202_H

typedef struct __KS202 {
	uint8 reg[8];
	uint8 cmd;
} KS202;

extern KS202 ks202;

DECLFW(KS202_Write);

void KS202_Power(void);
void KS202_Close(void);
void KS202_Reset(void);
void KS202_Restore(int version);

void KS202_Init(CartInfo *info, void (*proc)(void), int wram, int battery);

#endif /* _KS202_H */
