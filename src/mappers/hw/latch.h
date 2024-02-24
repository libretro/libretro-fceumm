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

#ifndef _FCEU_LATCH_H
#define _FCEU_LATCH_H

typedef struct __LATCH {
	uint16 addr;
    uint8 data;
} LATCH;

extern LATCH latch;

DECLFW(Latch_Write);

void Latch_Power(void);
void Latch_Close(void);
void Latch_RegReset(void);

void Latch_Init(CartInfo *info, void (*proc)(void), readfunc func, uint8 wram, uint8 busc);

#endif /* _FCEU_LATCH_H */
