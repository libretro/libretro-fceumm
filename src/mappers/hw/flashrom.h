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

#ifndef FLASHROM_H
#define FLASHROM_H

DECLFW(FlashROM_Write);
DECLFR(FlashROM_Read);

void FlashROM_Init(uint8 *data, uint32 size, uint8 manufacter_id, uint8 model_id, uint32 sector_size, uint32 adr1, uint32 adr2);
void FlashROM_CPUCyle(int a);

#endif /* FLASHROM_H */
