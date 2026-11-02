/* FCE Ultra - NES/Famicom Emulator: Top_Star
 *
 * Copyright notice for this file:
 *  Copyright (C) 2026 NewRisingSun
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

#include "mapinc.h"
#include "asic_latch.h"
#include "eeprom_24C0x.h"

/* There is also a speech chip, but it's unclear how it works. The only cartridge we have only says "Hi" at power-on and then nothing else. */ 

static void sync () {
	setprg32(0x8000, Latch_data >>1 &0x0F | Latch_data >>3 &0x10);
	setchr8(0);
}

static DECLFR (readEEPROM) {
	return x24c02_out;
}

static DECLFW (writeEEPROM) {
	x24c02_write(V >>2 &1, V &1);
}

static void power () {
	Latch_power();
	x24c02_init();
	SetReadHandler(0x6000, 0x7FFF, readEEPROM);
	SetWriteHandler(0x6000, 0x7FFF, writeEEPROM);
}

void Mapper636_Init (CartInfo *info) {
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = Latch_clear;
	info->battery = 1;
	info->SaveGame[0] = x24c0x_data + 256;
	info->SaveGameLen[0] = 256;
	AddExState(x24c0x_data + 256, 256, 0, "DAT2");
	AddExState(&x24c02StateRegs, ~0, 0, 0);
}
