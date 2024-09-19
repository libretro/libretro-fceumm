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

#ifndef _N163_SOUND_H
#define _N163_SOUND_H

void N163Sound_ESI(void);
DECLFR(N163Sound_Read);
DECLFW(N163Sound_Write);
void N163Sound_AddStateInfo(void);

uint8 *GetIRAM_ptr(void); /* pointer to internal RAM */
uint32 GetIRAM_size(void);

#endif /*  _N163_SOUND_H */
