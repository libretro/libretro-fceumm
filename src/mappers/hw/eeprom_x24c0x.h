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

#ifndef _EEPROM_X24C0X_H
#define _EEPROM_X24C0X_H

typedef enum {
	EEPROM_NONE = 0,
	EEPROM_X24C01,
	EEPROM_X24C02
} EEPROM_TYPE;

void x24c01_init(uint8 *data);
void x24c01_write(uint8 V);
uint8 x24c01_read(void);
extern SFORMAT x24c01_StateRegs[9];

void x24c02_init(uint8 *data);
void x24c02_write(uint8 V);
uint8 x24c02_read(void);
extern SFORMAT x24c02_StateRegs[9];

#endif /* _EEPROM_X24C0X_H */
