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

#ifndef _JV001_H
#define _JV001_H

typedef struct __JV001 {
	uint8 accumulator;
	uint8 inverter;
	uint8 staging;
	uint8 output;
	uint8 increase;
	uint8 invert;
	uint8 A;
	uint8 B;
	uint8 X;
} JV001;

extern JV001 jv001;

void JV001_Power(void);
void JV001_Reset(void);

void JV001_Init(CartInfo *info, void (*proc)(void));

DECLFR(JV001_Read);
DECLFW(JV001_Write);

#endif /* _JV001_H */
