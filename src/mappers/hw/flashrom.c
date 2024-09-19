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

#include "mapinc.h"
#include "flashrom.h"

static uint8 *flash_data;
static uint32 flash_size;

static uint8  flash_id[2];
static uint8  flash_state;
static uint32 flash_addr1;
static uint32 flash_addr2;
static uint32 flash_sect_size;
static int32  time_out;

static SFORMAT FlashStateRegs[] = {
    { &flash_state, 1, "STAT" },
    { &time_out,    4, "TIME" },
    { 0 }
};

#define PRG_OFFSET(A) (&Page[(A) >> 11][(A)] - flash_data)

DECLFR(FlashROM_Read) {
	if (flash_state == 0x90) {
        /* 0: manufacturer id */
        /* 1: model id */
        return flash_id[A & 1];
	} else if (time_out > 0) {
		return ((flash_data[PRG_OFFSET(A)] ^ ((time_out & 1) << 6)) & 0x77);
	}
	return flash_data[PRG_OFFSET(A)];
}

DECLFW(FlashROM_Write) {
	uint32 chip_address = PRG_OFFSET(A);
	uint32 cmd = chip_address & 0x7FFF;

	switch (flash_state) {
	default:
	case 0x80:
		if ((cmd == flash_addr1) && (V == 0xAA)) {
			flash_state++;
		}
		break;
	case 0x01:
	case 0x81:
		if ((cmd == flash_addr2) && (V == 0x55)) {
			flash_state++;
		}
		break;
	case 0x02:
		if (cmd == flash_addr1) {
			flash_state = V;
		}
		break;
	case 0x82:
		/* sector or chip erase */
		if (V == 0x30) {
			/* sector erase */
			if (chip_address < flash_size) {
				uint32 i;
				chip_address &= ~(flash_sect_size - 1);
				for (i = 0; i < flash_sect_size; i++) {
					flash_data[chip_address + i] = 0xFF;
				}
				FCEU_printf("Flash sector #%d is erased (0x%08x - 0x%08x).\n", chip_address / flash_sect_size, chip_address, chip_address + flash_sect_size);
				time_out = flash_sect_size;
			}
		} else if ((cmd == flash_addr1) && (V == 0x10)) {
			int i;
			/* chip erase */
			for (i = 0; i <= (int)flash_size; i++) {
				flash_data[i] = 0xFF;
			}
			FCEU_printf("Flash chip erased.\n");
			time_out = flash_size;
		} else if (V == 0xF0) {
			flash_state = 0;
		}
		break;
	case 0x90:
		/* software id */
		if (V == 0xF0) {
			flash_state = 0;
		}
		break;
	case 0xA0:
		/* byte program */
		flash_data[chip_address] = V;
		flash_state = 0;
		break;
	}

	/* FCEU_printf("%04x:%02x cmd:%04x state:%02x addr1:%04x addr2:%04x\n", A, V, cmd, flash_state, flash_addr1, flash_addr2); */
}

void FlashROM_CPUCyle(int a) {
	if (time_out > 0) {
		time_out -= a;
		if (time_out <= 0) {
			flash_state = 0;
		}
	}
}

void FlashROM_Init(uint8 *data, uint32 size, uint8 manufacter_id, uint8 model_id, uint32 sector_size, uint32 adr1, uint32 adr2) {
	flash_data      = data;
	flash_size      = size;
	flash_id[0]     = manufacter_id;
	flash_id[1]     = model_id;
	flash_addr1     = adr1;
	flash_addr2     = adr2;
	flash_sect_size = sector_size;

	AddExState(FlashStateRegs, ~0, 0, NULL);
}
