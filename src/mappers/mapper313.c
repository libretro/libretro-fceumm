/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

/* NES 2.0 Mapper 313 is used for MMC3-based multicarts that switch
 * between 128 KiB PRG-ROM/128 KiB CHR-ROM-sized games on each reset and
 * thus require no additional registers.
 * Its UNIF board name is BMC-RESET-TXROM.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M313CW(uint16 A, uint16 V) {
	switch (iNESCart.submapper) {
	default: setchr1(A, (reg << 7) | (V & 0x7F)); break;
	case 1:  setchr1(A, (reg << 7) | (V & 0x7F)); break;
	case 2:  setchr1(A, (reg << 8) | (V & 0xFF)); break;
	case 3:  setchr1(A, (reg << 8) | (V & 0xFF)); break;
	case 4:  setchr1(A, (reg << 7) | (V & 0x7F)); break;
	}
}

static void M313PW(uint16 A, uint16 V) {
	switch (iNESCart.submapper) {
	default: setprg8(A, (reg << 4) | (V & 0x0F)); break;
	case 1:  setprg8(A, (reg << 5) | (V & 0x1F)); break;
	case 2:  setprg8(A, (reg << 4) | (V & 0x0F)); break;
	case 3:  setprg8(A, (reg << 5) | (V & 0x1F)); break;
	case 4:
		if (reg == 0) {
			setprg8(A, (reg << 5) | (V & 0x1F));
		} else {
			setprg8(A, (reg << 4) | (V & 0x0F));
		}
		break;
	}
}

static void M313Reset(void) {
	reg++;
	reg &= 0x03;
	MMC3_Reset();
}

static void M313Power(void) {
	reg = 0;
	MMC3_Power();
}

/* NES 2.0 313, UNIF BMC-RESET-TXROM */
void Mapper313_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_cwrap = M313CW;
	MMC3_pwrap = M313PW;
	info->Power = M313Power;
	info->Reset = M313Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
