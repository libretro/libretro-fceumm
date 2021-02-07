/* FCE Ultra - NES/Famicom Emulator
*
* Copyright notice for this file:
*  Copyright (C) 2016 Cluster
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

const uint32 save_flash_size = 512 * 1024 * 8;

static uint8 *WRAM = NULL;
extern uint8 *UNIFchrrama;
static uint8 *SAVE_FLASH = NULL;

static uint8 REG_WRAM_enabled;
static uint8 REG_WRAM_page;
static uint8 REG_can_write_CHR_RAM;
static uint8 REG_map_ROM_on_6000;
static uint8 REG_flags;
static uint8 REG_mapper;
static uint8 REG_can_write_PRG;
static uint8 REG_mirroring;
static uint8 REG_four_screen;
static uint8 REG_lockout;

static uint16 REG_PRG_base;
static uint8 REG_PRG_mask;
static uint8 REG_PRG_mode;
static uint8 REG_PRG_bank_6000;
static uint8 REG_PRG_bank_A;
static uint8 REG_PRG_bank_B;
static uint8 REG_PRG_bank_C;
static uint8 REG_PRG_bank_D;

static uint8 REG_CHR_mask;
static uint8 REG_CHR_mode;
static uint8 REG_CHR_bank_A;
static uint8 REG_CHR_bank_B;
static uint8 REG_CHR_bank_C;
static uint8 REG_CHR_bank_D;
static uint8 REG_CHR_bank_E;
static uint8 REG_CHR_bank_F;
static uint8 REG_CHR_bank_G;
static uint8 REG_CHR_bank_H;
static uint8 REG_CHR_bank_A_alt;
static uint8 REG_CHR_bank_B_alt;
static uint8 REG_CHR_bank_C_alt;
static uint8 REG_CHR_bank_D_alt;

static uint8 REG_scanline_IRQ_enabled;
static uint8 REG_scanline_IRQ_counter;
static uint8 REG_scanline_IRQ_latch;
static uint8 REG_scanline_IRQ_reload;

static uint8 REG_scanline2_IRQ_enabled;
static uint8 REG_scanline2_IRQ_line;
static uint8 REG_scanline2_IRQ_pending;

static uint16 REG_CPU_IRQ_value;
static uint8 REG_CPU_IRQ_control;
static uint16 REG_CPU_IRQ_latch;
static uint8 REG_VRC4_IRQ_prescaler;
static uint8 REG_VRC4_IRQ_prescaler_counter;

static uint8 REG_R0;
static uint8 REG_R1;
static uint8 REG_R2;
static uint8 REG_R3;
static uint8 REG_R4;
static uint8 REG_R5;

static uint8 REG_mul1;
static uint8 REG_mul2;

static uint8 flash_state = 0;
static uint16 flash_buffer_a[10];
static uint8 flash_buffer_v[10];

static uint8 ppu_latch0;
static uint8 ppu_latch1;
static uint8 ppu_mapper163_latch;
static uint8 TKSMIR[8];

static uint32 REG_PRG_bank_6000_mapped;
static uint32 REG_PRG_bank_A_mapped;
static uint32 REG_PRG_bank_B_mapped;
static uint32 REG_PRG_bank_C_mapped;
static uint32 REG_PRG_bank_D_mapped;

static void COOLGIRL_Sync_PRG(void) {
	REG_PRG_bank_6000_mapped = (REG_PRG_base << 1) | (REG_PRG_bank_6000 & (((~REG_PRG_mask & 0x7F) << 1) | 1));
	REG_PRG_bank_A_mapped = (REG_PRG_base << 1) | (REG_PRG_bank_A & (((~REG_PRG_mask & 0x7F) << 1) | 1));
	REG_PRG_bank_B_mapped = (REG_PRG_base << 1) | (REG_PRG_bank_B & (((~REG_PRG_mask & 0x7F) << 1) | 1));
	REG_PRG_bank_C_mapped = (REG_PRG_base << 1) | (REG_PRG_bank_C & (((~REG_PRG_mask & 0x7F) << 1) | 1));
	REG_PRG_bank_D_mapped = (REG_PRG_base << 1) | (REG_PRG_bank_D & (((~REG_PRG_mask & 0x7F) << 1) | 1));
	/*FCEU_printf("PRG BASE: %08x\n", REG_PRG_base);
	FCEU_printf("PRG MASK: %02x\n", REG_PRG_mask);
	FCEU_printf("PRG MODE: %02x\n", REG_PRG_mode);
	FCEU_printf("PRG REG A: %04x\n", REG_PRG_bank_A_mapped);
	FCEU_printf("PRG REG B: %04x\n", REG_PRG_bank_B_mapped);
	FCEU_printf("PRG REG C: %04x\n", REG_PRG_bank_C_mapped);
	FCEU_printf("PRG REG D: %04x\n", REG_PRG_bank_D_mapped);*/
	uint8 REG_A_CHIP = (/*REG_PRG_bank_A_mapped < 0x10 * 3 ||*/ REG_PRG_bank_A_mapped >= 0x20000 - 0x10 * 32) ? 0x11 : 0;
	uint8 REG_B_CHIP = (/*REG_PRG_bank_B_mapped < 0x10 * 3 ||*/ REG_PRG_bank_B_mapped >= 0x20000 - 0x10 * 32) ? 0x11 : 0;
	uint8 REG_C_CHIP = (/*REG_PRG_bank_C_mapped < 0x10 * 3 ||*/ REG_PRG_bank_C_mapped >= 0x20000 - 0x10 * 32) ? 0x11 : 0;
	uint8 REG_D_CHIP = (/*REG_PRG_bank_D_mapped < 0x10 * 3 ||*/ REG_PRG_bank_D_mapped >= 0x20000 - 0x10 * 32) ? 0x11 : 0;

	switch (REG_PRG_mode & 7)
	{
	default:
	case 0:
		setprg16r(REG_A_CHIP, 0x8000, REG_PRG_bank_A_mapped >> 1);
		//FCEU_printf("0x8000: %08x\n", (REG_PRG_bank_A_mapped >> 1) * 0x4000);
		setprg16r(REG_C_CHIP, 0xC000, REG_PRG_bank_C_mapped >> 1);
		//FCEU_printf("0xC000: %08x\n", (REG_PRG_bank_C_mapped >> 1) * 0x4000);
		break;
	case 1:
		setprg16r(REG_C_CHIP, 0x8000, REG_PRG_bank_C_mapped >> 1);
		setprg16r(REG_A_CHIP, 0xC000, REG_PRG_bank_A_mapped >> 1);
		break;
	case 4:
		setprg8r(REG_A_CHIP, 0x8000, REG_PRG_bank_A_mapped);
		setprg8r(REG_B_CHIP, 0xA000, REG_PRG_bank_B_mapped);
		setprg8r(REG_C_CHIP, 0xC000, REG_PRG_bank_C_mapped);
		setprg8r(REG_D_CHIP, 0xE000, REG_PRG_bank_D_mapped);
		break;
	case 5:
		setprg8r(REG_C_CHIP, 0x8000, REG_PRG_bank_C_mapped);
		setprg8r(REG_B_CHIP, 0xA000, REG_PRG_bank_B_mapped);
		setprg8r(REG_A_CHIP, 0xC000, REG_PRG_bank_A_mapped);
		setprg8r(REG_D_CHIP, 0xE000, REG_PRG_bank_D_mapped);
		break;
	case 6:
		setprg32r(REG_A_CHIP, 0x8000, REG_PRG_bank_B_mapped >> 2);
		break;
	case 7:
		setprg32r(REG_A_CHIP, 0x8000, REG_PRG_bank_A_mapped >> 2);
		break;
	}

	if (!((REG_mapper == 20) && (REG_flags & 1))) // Mapper #189?
		setmirror((REG_mirroring < 2) ? (REG_mirroring ^ 1) : REG_mirroring);

	if (!REG_map_ROM_on_6000)
		setprg8r(0x10, 0x6000, REG_WRAM_page); // Select WRAM page
	else
		setprg8(0x6000, REG_PRG_bank_6000_mapped); // Map ROM on $6000-$7FFF
}

static void COOLGIRL_Sync_CHR(void) {
	SetupCartCHRMapping(0x12, UNIFchrrama, ((~REG_CHR_mask & 0x1F) + 1) * 0x2000 /*256 * 1024*/, REG_can_write_CHR_RAM); // enable or disable writes to CHR RAM, setup CHR mask

	switch (REG_CHR_mode & 7)
	{
	default:
	case 0:
		setchr8r(0x12, REG_CHR_bank_A >> 3);
		//FCEU_printf("CHR 0x0000: %08x\n", (REG_CHR_bank_A >> 3) * 0x2000);
		break;
	case 1:
		setchr4r(0x12, 0x0000, ppu_mapper163_latch);
		setchr4r(0x12, 0x1000, ppu_mapper163_latch);
		break;
	case 2:
		setchr2r(0x12, 0x0000, REG_CHR_bank_A >> 1);
		TKSMIR[0] = TKSMIR[1] = REG_CHR_bank_A;
		setchr2r(0x12, 0x0800, REG_CHR_bank_C >> 1);
		TKSMIR[2] = TKSMIR[3] = REG_CHR_bank_C;
		setchr1r(0x12, 0x1000, REG_CHR_bank_E);
		TKSMIR[4] = REG_CHR_bank_E;
		setchr1r(0x12, 0x1400, REG_CHR_bank_F);
		TKSMIR[5] = REG_CHR_bank_F;
		setchr1r(0x12, 0x1800, REG_CHR_bank_G);
		TKSMIR[6] = REG_CHR_bank_G;
		setchr1r(0x12, 0x1C00, REG_CHR_bank_H);
		TKSMIR[7] = REG_CHR_bank_H;
		break;
	case 3:
		setchr1r(0x12, 0x0000, REG_CHR_bank_E);
		TKSMIR[0] = REG_CHR_bank_E;
		setchr1r(0x12, 0x0400, REG_CHR_bank_F);
		TKSMIR[1] = REG_CHR_bank_F;
		setchr1r(0x12, 0x0800, REG_CHR_bank_G);
		TKSMIR[2] = REG_CHR_bank_G;
		setchr1r(0x12, 0x0C00, REG_CHR_bank_H);
		TKSMIR[3] = REG_CHR_bank_H;
		setchr2r(0x12, 0x1000, REG_CHR_bank_A >> 1);
		TKSMIR[4] = TKSMIR[5] = REG_CHR_bank_A;
		setchr2r(0x12, 0x1800, REG_CHR_bank_C >> 1);
		TKSMIR[6] = TKSMIR[7] = REG_CHR_bank_C;
		break;
	case 4:
		setchr4r(0x12, 0x0000, REG_CHR_bank_A >> 2);
		setchr4r(0x12, 0x1000, REG_CHR_bank_E >> 2);
		break;
	case 5:
		if (!ppu_latch0)
			setchr4r(0x12, 0x0000, REG_CHR_bank_A >> 2);
		else
			setchr4r(0x12, 0x0000, REG_CHR_bank_B >> 2);
		if (!ppu_latch1)
			setchr4r(0x12, 0x1000, REG_CHR_bank_E >> 2);
		else
			setchr4r(0x12, 0x1000, REG_CHR_bank_F >> 2);
		break;
	case 6:
		setchr2r(0x12, 0x0000, REG_CHR_bank_A >> 1);
		setchr2r(0x12, 0x0800, REG_CHR_bank_C >> 1);
		setchr2r(0x12, 0x1000, REG_CHR_bank_E >> 1);
		setchr2r(0x12, 0x1800, REG_CHR_bank_G >> 1);
		break;
	case 7:
		setchr1r(0x12, 0x0000, REG_CHR_bank_A);
		setchr1r(0x12, 0x0400, REG_CHR_bank_B);
		setchr1r(0x12, 0x0800, REG_CHR_bank_C);
		setchr1r(0x12, 0x0C00, REG_CHR_bank_D);
		setchr1r(0x12, 0x1000, REG_CHR_bank_E);
		setchr1r(0x12, 0x1400, REG_CHR_bank_F);
		setchr1r(0x12, 0x1800, REG_CHR_bank_G);
		setchr1r(0x12, 0x1C00, REG_CHR_bank_H);
		break;
	}
}

static void COOLGIRL_Sync_Mirroring(void) {	
	if (!REG_four_screen)
	{
		if (!((REG_mapper == 20) && (REG_flags & 1))) // Mapper #189?
			setmirror((REG_mirroring < 2) ? (REG_mirroring ^ 1) : REG_mirroring);
	} else { // four screen mode
		vnapage[0] = UNIFchrrama + 0x3F000;
		vnapage[1] = UNIFchrrama + 0x3F400;
		vnapage[2] = UNIFchrrama + 0x3F800;
		vnapage[3] = UNIFchrrama + 0x3FC00;
	}
}

static void COOLGIRL_Sync(void) {
	COOLGIRL_Sync_PRG();
	COOLGIRL_Sync_CHR();
	COOLGIRL_Sync_Mirroring();
}

static DECLFW(COOLGIRL_Flash_Write) {
	if (flash_state < 10)
	{
		flash_buffer_a[flash_state] = A & 0xFFF;
		flash_buffer_v[flash_state] = V;
		flash_state++;

		// sector erase
		if ((flash_state == 6) &&
			(flash_buffer_a[0] == 0x0AAA) && (flash_buffer_v[0] == 0xAA) &&
			(flash_buffer_a[1] == 0x0555) && (flash_buffer_v[1] == 0x55) &&
			(flash_buffer_a[2] == 0x0AAA) && (flash_buffer_v[2] == 0x80) &&
			(flash_buffer_a[3] == 0x0AAA) && (flash_buffer_v[3] == 0xAA) &&
			(flash_buffer_a[4] == 0x0555) && (flash_buffer_v[4] == 0x55) &&
			(flash_buffer_v[5] == 0x30))
		{
			uint32 sector_address = REG_PRG_bank_A_mapped * 0x2000;
			for (uint32 i = sector_address; i < sector_address + 128 * 1024; i++)
				SAVE_FLASH[i % save_flash_size] = 0xFF;
			FCEU_printf("Flash sector erased: %08x - %08x\n", sector_address, sector_address + 128*1024-1);
			flash_state = 0;
		}

		// writing byte
		if ((flash_state == 4) &&
			(flash_buffer_a[0] == 0x0AAA) && (flash_buffer_v[0] == 0xAA) &&
			(flash_buffer_a[1] == 0x0555) && (flash_buffer_v[1] == 0x55) &&
			(flash_buffer_a[2] == 0x0AAA) && (flash_buffer_v[2] == 0xA0))
		{
			uint32 sector_address = REG_PRG_bank_A_mapped * 0x2000;
			uint32 flash_addr = sector_address+(A%0x8000);
			if (SAVE_FLASH[flash_addr % save_flash_size] != 0xFF) FCEU_PrintError("Error flash sector is not erased: %08x\n", flash_addr);
			SAVE_FLASH[flash_addr % save_flash_size] =  V;
			if (A % 0x2000 == 0)
				FCEU_printf("Flash sector writed: %08x\n", flash_addr);
			flash_state = 0;
		}
	}
	if (V == 0xF0)
		flash_state = 0;
}

static uint64 lreset = 0;

static DECLFW(COOLGIRL_WRITE) {
	if (REG_WRAM_enabled && A >= 0x6000 && A < 0x8000 && !REG_map_ROM_on_6000)
		CartBW(A, V); // WRAM is enabled and writable
	if (REG_can_write_PRG && A >= 0x8000) // writing flash
		COOLGIRL_Flash_Write(A, V);

	// block two writes in a row
	if ((timestampbase + timestamp) < (lreset + 2))	return;
	lreset = timestampbase + timestamp;

	if (A >= 0x5000 && A < 0x6000 && !REG_lockout)
	{
		//FCEU_printf("Write: %02x => %04x\n", V, A);
		switch (A & 7)
		{
		case 0:
			REG_PRG_base = (REG_PRG_base & 0xFF) | (V << 8); // PRG base hi
			//FCEU_printf("REG_PRG_base: %02x\n", REG_PRG_base);
			break;
		case 1:
			REG_PRG_base = (REG_PRG_base & 0xFF00) | V; // PRG base low
			//FCEU_printf("REG_PRG_base: %02x\n", REG_PRG_base);
			break;
		case 2:
			REG_PRG_mask = V & 0xFF; // PRG mask
			//FCEU_printf("REG_PRG_mask: %02x\n", REG_PRG_mask);
			break;
		case 3:
			REG_PRG_mode = (V & 0xE0) >> 5; // PRG mode
			//FCEU_printf("REG_PRG_mode: %02x\n", REG_PRG_mode);
			REG_CHR_bank_A = (REG_CHR_bank_A & 7) | (V << 3); // direct access to REG_CHR_bank_A
			break;
		case 4:
			REG_CHR_mode = (V & 0xE0) >> 5; // CHR mode
			//FCEU_printf("REG_CHR_mode: %02x\n", REG_CHR_mode);
			REG_CHR_mask = V & 0x1F; // CHR mask
			//FCEU_printf("REG_CHR_mask: %02x\n", REG_CHR_mask);
			break;
		case 5:
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xC1) | ((V & 0x7C) >> 1); // direct access to REG_PRG_bank_A
			REG_WRAM_page = V & 3; // WRAM page
			//FCEU_printf("REG_WRAM_page: %02x\n", REG_WRAM_page);
			break;
		case 6:
			REG_flags = (V & 0xE0) >> 5;
			//FCEU_printf("REG_flags: %02x\n", REG_flags);
			REG_mapper = V & 0x1F;
			//FCEU_printf("REG_mapper: %02x\n", REG_mapper);
			break;
		case 7:
			REG_lockout = (V & 0x80) >> 7;
			if (REG_lockout)
			{
				FCEU_printf("Lockout enabled, mapper id: 0x%02x\n", REG_mapper);
				FCEU_printf("REG_PRG_mode: 0x%02x\n", REG_PRG_mode);
				FCEU_printf("REG_CHR_mode: 0x%02x\n", REG_CHR_mode);
			}
			REG_four_screen = (V & 0x20) >> 5;
			//FCEU_printf("REG_lockout: %02x\n", REG_lockout);
			REG_mirroring = (V & 0x18) >> 3;
			//FCEU_printf("REG_mirroring: %02x\n", REG_mirroring);
			REG_can_write_PRG = (V & 4) >> 2;
			//FCEU_printf("REG_can_write_PRG: %02x\n", REG_can_write_PRG);
			REG_can_write_CHR_RAM = (V & 2) >> 1;
			//FCEU_printf("REG_can_write_CHR_RAM: %02x\n", REG_can_write_CHR_RAM);
			REG_WRAM_enabled = V & 1;
			//FCEU_printf("REG_WRAM_enabled: %02x\n", REG_WRAM_enabled);
			break;
		}

		if (REG_mapper == 17)  REG_PRG_bank_B = ~2; // for MMC2
		if (REG_mapper == 14) REG_PRG_bank_B = 1; // for mapper #65f
	}

	if (A < 0x8000) // $0000-$7FFF
	{		
		// Mapper #163
		if (REG_mapper == 6)
		{
			if (A == 0x5101) // if (cpu_addr_in[14:0] == 15'h5101)
			{
				if (REG_R4 && !V) // if ((r4 != 0) && (cpu_data_in == 0))
					REG_R5 ^= 1; // r5[0] = ~r5[0];
				REG_R4 = V;
			} else if (A == 0x5100 && V == 6) // if ((cpu_addr_in[14:0] == 15'h5100) && (cpu_data_in == 6))
			{
				REG_PRG_mode = REG_PRG_mode & 0xFE; // prg_mode[0] = 0;
				REG_PRG_bank_B = 12; // prb_bank_b = 4'b1100;
			} else {
				if (((A >> 12) & 7) == 5) // if (cpu_addr_in[14:12] == 3'b101) begin
				{
					switch ((A >> 8) & 3) // case (cpu_addr_in[9:8])
					{
					case 2:
						REG_PRG_mode |= 1; // prg_mode[0] = 1;
						REG_PRG_bank_A = (REG_PRG_bank_A & 0x3F) | ((V & 3) << 6); // prg_bank_a[7:6] = cpu_data_in[1:0];
						REG_R0 = V;
						break;
					case 0:
						REG_PRG_mode |= 1; // prg_mode[0] = 1;
						REG_PRG_bank_A = (REG_PRG_bank_A & 0xC3) | ((V & 0x0F) << 2); // prg_bank_a[5:2] = cpu_data_in[3:0];
						REG_CHR_mode = (REG_CHR_mode&0xFE) | (V>>7); // chr_mode[0] = cpu_data_in[7];
						REG_R1 = V;
						break;
					case 3:
						REG_R2 = V; // r2 = cpu_data_in;
						break;
					case 1:
						REG_R3 = V; // r3 = cpu_data_in;
						break;
					}
				}
			}
		}

		// Mapper #87
		if (REG_mapper == 12)
		{
			if (((A >> 13) & 3) == 3) // if (cpu_addr_in[14] & cpu_addr_in[13]) // $6000-$7FFF
				// chr_bank_a[4:3] = {cpu_data_in[0], cpu_data_in[1]};
				REG_CHR_bank_A = (REG_CHR_bank_A & 0xE7) | ((V & 1) << 4) | ((V & 2) << 2);
		}

		// Mapper #90 - JY
		/*
		if (REG_mapper == 13)
		{
			switch (A)
			{
			case 0x5800: REG_mul1 = V; break;
			case 0x5801: REG_mul2 = V; break;
			}
		}
		*/

		// MMC5
		if (REG_mapper == 15)
		{
			switch (A)
			{
			case 0x5105:
				if (V == 0xFF)
					REG_four_screen = 1;
				else {
					REG_four_screen = 0;
					switch (((V >> 2) & 1) | ((V >> 3) & 2))
					{
						case 0: 
							REG_mirroring = 2; break;
						case 1:
							REG_mirroring = 0; break;
						case 2:
							REG_mirroring = 1; break;
						case 3:
							REG_mirroring = 3; break;
						default:
							FCEU_PrintError("Mirroring mode is not supported, please report to Cluster");
							break;
					}
				}
				break;
			case 0x5115:
				REG_PRG_bank_A = V & ~1;
				REG_PRG_bank_B = V | 1;
				break;
			case 0x5116:
				REG_PRG_bank_C = V;
				break;
			case 0x5117:
				REG_PRG_bank_D = V;
				break;
			case 0x5120:
				REG_CHR_bank_A = V;
				break;
			case 0x5121:
				REG_CHR_bank_B = V;
				break;
			case 0x5122:
				REG_CHR_bank_C = V;
				break;
			case 0x5123:
				REG_CHR_bank_D = V;
				break;
			case 0x5128:
				REG_CHR_bank_E = V;
				break;
			case 0x5129:
				REG_CHR_bank_F = V;
				break;
			case 0x512A:
				REG_CHR_bank_G = V;
				break;
			case 0x512B:
				REG_CHR_bank_H = V;
				break;
			case 0x5203:
				X6502_IRQEnd(FCEU_IQEXT);
				REG_scanline2_IRQ_pending = 0;
				REG_scanline2_IRQ_line = V;
				break;
			case 0x5204:
				X6502_IRQEnd(FCEU_IQEXT);
				REG_scanline2_IRQ_pending = 0;
				REG_scanline2_IRQ_enabled = V & 0x80;
				break;
			}
		}

		// Mapper #189
		if ((REG_mapper == 20) && (A >= 0x4120) && (REG_flags & 2))
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xC3) | ((V & 0x0F) << 2) | ((V & 0xF0) >> 2);
		}

		// temp/test
		if (REG_mapper == 31)
		{
			if (A == (0x4025))
			{
				REG_mirroring = ((V & 8) >> 3) & 1;
			}
		}
	}
	else // $8000-$FFFF
	{
		// Mapper #2 - UxROM
		// flag0 - mapper #71 - for Fire Hawk only.
		// other mapper-#71 games are UxROM
		if (REG_mapper == 1)
		{
			if (!(REG_flags & 1) || ((A & 0xF000) != 0x9000)) // (!USE_FIRE_HAWK | ~flags[0] | (cpu_addr_in[14:12] != 3'b001))
				REG_PRG_bank_A = (REG_PRG_bank_A & 0xC1) | ((V & 0x1F) << 1); // prg_bank_a[5:1] = cpu_data_in[4:0];
			else
				REG_mirroring = 2 + ((V >> 4) & 1); // mirroring[1:0] = {1'b1, cpu_data_in[4]};
		}

		// Mapper #3 - CNROM
		if (REG_mapper == 2)
		{
			REG_CHR_bank_A = (REG_CHR_bank_A & 7) | (V << 3); // chr_bank_a[7:3] = cpu_data_in[4:0];
		}

		// Mapper #78 - Holy Diver
		if (REG_mapper == 3)
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xF1) | ((V & 7) << 1); // prg_bank_a[3:1] = cpu_data_in[2:0];
			REG_CHR_bank_A = (REG_CHR_bank_A & 0x87) | ((V & 0xF0) >> 1); // chr_bank_a[6:3] = cpu_data_in[7:4];
			REG_mirroring = ((V >> 3) & 1) ^ 1; // mirroring = {1'b0, ~cpu_data_in[3]};
		}

		// Mapper #97 - Irem's TAM-S1
		if (REG_mapper == 4)
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xE1) | ((V & 0x0F) << 1); // prg_bank_a[4:1] = cpu_data_in[3:0];
			REG_mirroring = (V >> 6) ^ ((V >> 6) & 2); // mirroring = cpu_data_in[7:6] ^ {~cpu_data_in[6], 1'b0};
		}

		// Mapper #93 - Sunsoft-2
		if (REG_mapper == 5)
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xF1) | ((V & 0x70) >> 3); // prg_bank_a[3:1] = {cpu_data_in[6:4]};
			REG_can_write_CHR_RAM = V & 1;
		}
		
		// Mapper #18
		if (REG_mapper == 7)
		{
			switch (((A>>10)&0x1C) | (A&3)) // case ({cpu_addr_in[14:12], cpu_addr_in[1:0]})
			{
			case 0: // 5'b00000: prg_bank_a[3:0] = cpu_data_in[3:0]; // $8000
				REG_PRG_bank_A = (REG_PRG_bank_A & 0xF0) | (V & 0x0F); break;
			case 1: // 5'b00001: prg_bank_a[7:4] = cpu_data_in[3:0]; // $8001
				REG_PRG_bank_A = (REG_PRG_bank_A & 0x0F) | ((V & 0x0F) << 4); break;
			case 2: // 5'b00010: prg_bank_b[3:0] = cpu_data_in[3:0]; // $8002
				REG_PRG_bank_B = (REG_PRG_bank_B & 0xF0) | (V & 0x0F); break;
			case 3: // 5'b00011: prg_bank_b[7:4] = cpu_data_in[3:0]; // $8003
				REG_PRG_bank_B = (REG_PRG_bank_B & 0x0F) | ((V & 0x0F) << 4); break;
			case 4: // 5'b00100: prg_bank_c[3:0] = cpu_data_in[3:0]; // $9000
				REG_PRG_bank_C = (REG_PRG_bank_C & 0xF0) | (V & 0x0F); break; 
			case 5: // 5'b00101: prg_bank_c[7:4] = cpu_data_in[3:0]; // $9001
				REG_PRG_bank_C = (REG_PRG_bank_C & 0x0F) | ((V & 0x0F) << 4); break;
			case 6: 
				break;
			case 7:
				break;
			case 8: // 5'b01000: chr_bank_a[3:0] = cpu_data_in[3:0]; // $A000
				REG_CHR_bank_A = (REG_CHR_bank_A & 0xF0) | (V & 0x0F); break;
			case 9: // 5'b01001: chr_bank_a[7:4] = cpu_data_in[3:0]; // $A001
				REG_CHR_bank_A = (REG_CHR_bank_A & 0x0F) | ((V & 0x0F) << 4); break;
			case 10: // 5'b01010: chr_bank_b[3:0] = cpu_data_in[3:0]; // $A002
				REG_CHR_bank_B = (REG_CHR_bank_B & 0xF0) | (V & 0x0F); break;
			case 11: // 5'b01011: chr_bank_b[7:4] = cpu_data_in[3:0]; // $A003
				REG_CHR_bank_B = (REG_CHR_bank_B & 0x0F) | ((V & 0x0F) << 4); break;
			case 12: // 5'b01100: chr_bank_c[3:0] = cpu_data_in[3:0]; // $B000
				REG_CHR_bank_C = (REG_CHR_bank_C & 0xF0) | (V & 0x0F); break;
			case 13: // 5'b01101: chr_bank_c[7:4] = cpu_data_in[3:0]; // $B001
				REG_CHR_bank_C = (REG_CHR_bank_C & 0x0F) | ((V & 0x0F) << 4); break;
			case 14: // 5'b01110: chr_bank_d[3:0] = cpu_data_in[3:0]; // $B002
				REG_CHR_bank_D = (REG_CHR_bank_D & 0xF0) | (V & 0x0F); break;
			case 15: // 5'b01111: chr_bank_d[7:4] = cpu_data_in[3:0]; // $B003
				REG_CHR_bank_D = (REG_CHR_bank_D & 0x0F) | ((V & 0x0F) << 4); break;
			case 16: // 5'b10000: chr_bank_e[3:0] = cpu_data_in[3:0]; // $C000
				REG_CHR_bank_E = (REG_CHR_bank_E & 0xF0) | (V & 0x0F); break;
			case 17: // 5'b10001: chr_bank_e[7:4] = cpu_data_in[3:0]; // $C001
				REG_CHR_bank_E = (REG_CHR_bank_E & 0x0F) | ((V & 0x0F) << 4); break;
			case 18: // 5'b10010: chr_bank_f[3:0] = cpu_data_in[3:0]; // $C002
				REG_CHR_bank_F = (REG_CHR_bank_F & 0xF0) | (V & 0x0F); break;
			case 19: // 5'b10011: chr_bank_f[7:4] = cpu_data_in[3:0]; // $C003
				REG_CHR_bank_F = (REG_CHR_bank_F & 0x0F) | ((V & 0x0F) << 4); break;
			case 20: // 5'b10100: chr_bank_g[3:0] = cpu_data_in[3:0]; // $D000
				REG_CHR_bank_G = (REG_CHR_bank_G & 0xF0) | (V & 0x0F); break;
			case 21: // 5'b10101: chr_bank_g[7:4] = cpu_data_in[3:0]; // $D001
				REG_CHR_bank_G = (REG_CHR_bank_G & 0x0F) | ((V & 0x0F) << 4); break;
			case 22: // 5'b10110: chr_bank_h[3:0] = cpu_data_in[3:0]; // $D002
				REG_CHR_bank_H = (REG_CHR_bank_H & 0xF0) | (V & 0x0F); break;
			case 23: // 5'b10111: chr_bank_h[7:4] = cpu_data_in[3:0]; // $D003
				REG_CHR_bank_H = (REG_CHR_bank_H & 0x0F) | ((V & 0x0F) << 4); break;
			case 24: // 5'b11000: irq_cpu_latch[3:0] = cpu_data_in[3:0]; // $E000
				REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0xFFF0) | (V & 0x0F); break;
			case 25: // 5'b11001: irq_cpu_latch[7:4] = cpu_data_in[3:0]; // $E001
				REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0xFF0F) | ((V & 0x0F) << 4); break;
			case 26: // 5'b11010: irq_cpu_latch[11:8] = cpu_data_in[3:0]; // $E002
				REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0xF0FF) | ((V & 0x0F) << 8); break;
			case 27: //5'b11011: irq_cpu_latch[15:12] = cpu_data_in[3:0]; // $E003
				REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0x0FFF) | ((V & 0x0F) << 12); break;
			case 28: // 5'b11100: begin // $F000
				X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_value[15:0] = irq_cpu_latch[15:0];
				REG_CPU_IRQ_value = REG_CPU_IRQ_latch; break; // irq_cpu_out = 0;
			case 29: // 5'b11101: begin // $F001
				X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_control[3:0] = cpu_data_in[3:0];
				REG_CPU_IRQ_control = V & 0x0F; break; // irq_cpu_out = 0;
			case 30: // 5'b11110: mirroring = cpu_data_in[1:0] ^ {1'b0, ~cpu_data_in[1]}; // $F002
				REG_mirroring = V ^ (((V >> 1) & 1) ^ 1); break;
			case 31:
				break; // sound
			}
		}

		// Mapper #7 - AxROM, mapper #241 - BNROM
		if (REG_mapper == 8)
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xC3) | ((V & 0xF) << 2); // prg_bank_a[5:2] = cpu_data_in[3:0];
			if (!REG_flags&1) // if (!USE_MAPPER_241 || !flags[0]) // BNROM?
				REG_mirroring = 2 + ((V >> 4) & 1); // mirroring = {1'b1, cpu_data_in[4]};
		}

		// Mapper #228 - Cheetahmen II				
		if (REG_mapper == 9)
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xC3) | ((A & 0x780) >> 5); // prg_bank_a[5:2] = cpu_addr_in[10:7];
			REG_CHR_bank_A = (REG_CHR_bank_A & 7) | ((A & 7) << 5) | ((V & 3) << 3); // chr_bank_a[7:3] = {/*cpu_addr_in[3]*/cpu_addr_in[2:0], cpu_data_in[1:0] }; // only 256k, sorry
			REG_mirroring = (A >> 13) & 1;  // mirroring = { 1'b0, cpu_addr_in[13]};
		}

		// Mapper #11 - ColorDreams
		if (REG_mapper == 10)
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xF3) | ((V & 3) << 2); // prg_bank_a[3:2] = cpu_data_in[1:0];
			REG_CHR_bank_A = (REG_CHR_bank_A & 0x87) | ((V & 0xF0) >> 1); // chr_bank_a[6:3] = cpu_data_in[7:4];
		}

		// Mapper #66 - GxROM
		if (REG_mapper == 11)
		{
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xF3) | ((V & 0x30) >> 2); // prg_bank_a[3:2] = cpu_data_in[5:4];
			REG_CHR_bank_A = (REG_CHR_bank_A & 0xE7) | ((V & 3) << 3); // chr_bank_a[4:3] = cpu_data_in[1:0];
		}

		// Mapper #90 - JY
		if (REG_mapper == 13)
		{
			if (((A >> 12) & 7) == 0)
			{
				switch (A & 3)
				{
				case 0: REG_PRG_bank_A = (REG_PRG_bank_A & 0xC0) | (V & 0x3F); break; // $8000
				case 1: REG_PRG_bank_B = (REG_PRG_bank_B & 0xC0) | (V & 0x3F); break; // $8001
				case 2: REG_PRG_bank_C = (REG_PRG_bank_C & 0xC0) | (V & 0x3F); break; // $8002
				case 3: REG_PRG_bank_D = (REG_PRG_bank_D & 0xC0) | (V & 0x3F); break; // $8003
				}
			}

			if (((A >> 12) & 7) == 1)
			{
				switch (A & 7)
				{
				case 0: REG_CHR_bank_A = V; break; // $9000
				case 1: REG_CHR_bank_B = V; break; // $9001
				case 2: REG_CHR_bank_C = V; break; // $9002
				case 3: REG_CHR_bank_D = V; break; // $9003
				case 4: REG_CHR_bank_E = V; break; // $9004
				case 5: REG_CHR_bank_F = V; break; // $9005
				case 6: REG_CHR_bank_G = V; break; // $9006
				case 7: REG_CHR_bank_H = V; break; // $9007
				}
			}

			if (((A >> 12) & 7) == 5) // D00x
			{
				switch (A & 3)// $D001
				{
					case 1: REG_mirroring = V & 3; break;
				}
			}

			if (((A >> 12) & 7) == 4) // $C00x
			{
				switch (A & 7)
				{
				case 0:
					if (V&1)
					{
						REG_scanline_IRQ_enabled = 1;
					} else {
						REG_scanline_IRQ_enabled = 0;
						X6502_IRQEnd(FCEU_IQEXT);
					}
					break;
				case 1:
					break; // who cares about this shit?
				case 2:
					REG_scanline_IRQ_enabled = 0;
					X6502_IRQEnd(FCEU_IQEXT);
					break;
				case 3:
					REG_scanline_IRQ_enabled = 1;
					break; 
				case 4:
					break; // prescaler? who cares?
				case 5:
					REG_scanline_IRQ_latch = V ^ REG_R0;
					REG_scanline_IRQ_reload = 1;
					break;
				case 6:
					REG_R0 = V;
					break;
				case 7:
					break; // meh
				}
			}			
		}

		// Mapper #65 - Irem's H3001
		if (REG_mapper == 14)
		{
			switch (((A>>9)&0x38) | (A & 7)) // case ({cpu_addr_in[14:12], cpu_addr_in[2:0]})
			{
			case 0: // 6'b000000: prg_bank_a[5:0] = cpu_data_in[5:0]; // $8000
				REG_PRG_bank_A = (REG_PRG_bank_A & 0xC0) | (V & 0x3F); break;
			case 9: // 6'b001001: mirroring = {1'b0, cpu_data_in[7]}; // $9001, mirroring
				REG_mirroring = (V>>7)&1; break;
			case 11: // 6'b001011: irq_cpu_control[0] = cpu_data_in[7]; // $9003, enable IRQ
				REG_CPU_IRQ_control = (REG_CPU_IRQ_control & 0xFE) | ((V>>7)&1);
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 12: // 6'b001100: irq_cpu_value[15:0] = {r0, r1};
				REG_CPU_IRQ_value = (REG_R0 << 8) | REG_R1;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 13: // 6'b001101: r0 = cpu_data_in; // $9005, IRQ high value
				REG_R0 = V; break;
			case 14: // 6'b001110: r1 = cpu_data_in; // $9006, IRQ low value
				REG_R1 = V; break;
			case 16: // 6'b010000: prg_bank_b[5:0] = cpu_data_in[5:0]; // $A000
				REG_PRG_bank_B = (REG_PRG_bank_B & 0xC0) | (V & 0x3F); break;
			case 24: // 6'b011000: chr_bank_a = cpu_data_in; // $B000
				REG_CHR_bank_A = V; break;
			case 25:
				REG_CHR_bank_B = V; break;
			case 26:
				REG_CHR_bank_C = V; break;
			case 27:
				REG_CHR_bank_D = V; break;
			case 28:
				REG_CHR_bank_E = V; break;
			case 29:
				REG_CHR_bank_F = V; break;
			case 30:
				REG_CHR_bank_G = V; break;
			case 31:
				REG_CHR_bank_H = V; break;
			case 32: // 3'b100000: prg_bank_c[5:0] = cpu_data_in[5:0]; // $C000
				REG_PRG_bank_C = (REG_PRG_bank_C & 0xC0) | (V & 0x3F); break;
			}
		}

		// Mapper #1 - MMC1
		/*
		r0 - load register
		flag0 - 16KB of WRAM (SOROM)
		*/
		if (REG_mapper == 16)
		{
			if (V & 0x80) // reset
			{
				REG_R0 = (REG_R0 & 0xC0) | 0x20; // r0[5:0] = 6'b100000;
				REG_PRG_mode = 0; // 0x4000 (A) + fixed last (C)
				REG_PRG_bank_C = (REG_PRG_bank_C & 0xE0) | 0x1E; // prg_bank_c[4:0] = 5'b11110;
			}
			else {
				REG_R0 = (REG_R0 & 0xC0) | ((V & 1) << 5) | ((REG_R0 & 0x3E) >> 1); //  r0[5:0] = { cpu_data_in[0], r0[5:1] };
				if (REG_R0 & 1)
				{
					switch ((A >> 13) & 3)
					{
					case 0: // $8000-$9FFF
						if ((REG_R0 & 0x18) == 0x18)
						{
							REG_PRG_mode = 0;
							REG_PRG_bank_C = (REG_PRG_bank_C & 0xE0) | 0x1E; // prg_bank_c[4:0] = 5'b11110;
						}
						else if ((REG_R0 & 0x18) == 0x10)
						{
							REG_PRG_mode = 1;
							REG_PRG_bank_C = (REG_PRG_bank_C & 0xE0); // prg_bank_c[4:0] = 5'b00000;
						}
						else 
							REG_PRG_mode = 7;
						if ((REG_R0 >> 5) & 1)
							REG_CHR_mode = 4;
						else
							REG_CHR_mode = 0;
						REG_mirroring = ((REG_R0 >> 1) & 3) ^ 2;
						break;
					case 1: // $A000-$BFFF
						REG_CHR_bank_A = (REG_CHR_bank_A & 0x83) | ((REG_R0 & 0x3E) << 1); // chr_bank_a[6:2] = r0[5:1];
						REG_PRG_bank_A = (REG_PRG_bank_A & 0xDF) | (REG_R0 & 0x20); // prg_bank_a[5] = r0[5]; // for SUROM, 512k PRG support
						REG_PRG_bank_C = (REG_PRG_bank_C & 0xDF) | (REG_R0 & 0x20); // prg_bank_c[5] = r0[5]; // for SUROM, 512k PRG support
						break;
					case 2: // $C000-$DFFF
						REG_CHR_bank_E = (REG_CHR_bank_E & 0x83) | ((REG_R0 & 0x3E) << 1); // 2'b10: chr_bank_e[6:2] = r0[5:1];
						break;
					case 3: // $E000-$FFFF
						REG_PRG_bank_A = (REG_PRG_bank_A & 0xE1) | (REG_R0 & 0x1E); // 2'b11: prg_bank_a[4:1] = r0[4:1];
						REG_WRAM_enabled = ((REG_R0 >> 5) & 1) ^ 1; // sram_enabled = ~r0[5];
						break;
					}
					REG_R0 = (REG_R0 & 0xC0) | 0x20; // r0[5:0] = 6'b100000;
					if (REG_flags & 1) // (flags[0]) - 16KB of WRAM
					{
						if (REG_CHR_mode & 4)
							REG_WRAM_page = 2 | (REG_CHR_bank_A >> 6) ^ 1; // sram_page = {1'b1, ~chr_bank_a[6] }, page #2 is battery backed
						else
							REG_WRAM_page = 2 | (REG_CHR_bank_A >> 5) ^ 1; // sram_page = {1'b1, ~chr_bank_a[5]}
					}
				}
			}
		}

		// Mapper #9 and #10 - MMC2 and MMC4
		// flag0 - 0=MMC2, 1=MMC4
		if (REG_mapper == 17)
		{
			switch ((A >> 12) & 7)
			{
			case 2:  // $A000-$AFFF
				if (!(REG_flags & 1)) // MMC2
					REG_PRG_bank_A = (REG_PRG_bank_A & 0xF0) | (V & 0x0F); // prg_bank_a[3:0] = cpu_data_in[3:0];
				else // MMC4
					REG_PRG_bank_A = (REG_PRG_bank_A & 0xE1) | ((V & 0x0F) << 1); // prg_bank_a[4:0] = { cpu_data_in[3:0], 1'b0};
				break;
			case 3: // $B000-$BFFF
				REG_CHR_bank_A = (REG_CHR_bank_A & 0x83) | ((V & 0x1F) << 2); // chr_bank_a[6:2] = cpu_data_in[4:0];
				break;
			case 4: // $C000-$CFFF
				REG_CHR_bank_B = (REG_CHR_bank_B & 0x83) | ((V & 0x1F) << 2); // chr_bank_b[6:2] = cpu_data_in[4:0];
				break;
			case 5: // $D000-$DFFF
				REG_CHR_bank_E = (REG_CHR_bank_E & 0x83) | ((V & 0x1F) << 2); // chr_bank_b[6:2] = cpu_data_in[4:0];
				break;
			case 6: // $E000-$EFFF
				REG_CHR_bank_F = (REG_CHR_bank_F & 0x83) | ((V & 0x1F) << 2); // chr_bank_b[6:2] = cpu_data_in[4:0];
				break;
			case 7: // $F000-$FFFF
				REG_mirroring = V & 1;
				break;
			}
		}

		// Mapper #152
		if (REG_mapper == 18)
		{
			REG_CHR_bank_A = (REG_CHR_bank_A & 0x87) | ((V & 0x0F) << 3); // chr_bank_a[6:3] = cpu_data_in[3:0];
			REG_PRG_bank_A = (REG_PRG_bank_A & 0xF1) | ((V & 0x70) >> 3); // prg_bank_a[3:1] = cpu_data_in[6:4];
			REG_mirroring = 2 | (V>>7); // mirroring = {1'b1, cpu_data_in[7]};
		}

		// Mapper #73 - VRC3
		if (REG_mapper == 19)
		{
			switch ((A>>12)&7)
			{
				case 0: // $8000-$8FFF
					REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0xFFF0) | (V & 0x0F); // irq_cpu_latch[3:0] = cpu_data_in[3:0];
					break;
				case 1: // $9000-$9FFF
					REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0xFF0F) | ((V & 0x0F) << 4); // irq_cpu_latch[7:4] = cpu_data_in[3:0];
					break;
				case 2: // $A000-$AFFF
					REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0xF0FF) | ((V & 0x0F) << 8); // irq_cpu_latch[11:8] = cpu_data_in[3:0];
					break;
				case 3: // $B000-$BFFF
					REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0x0FFF) | ((V & 0x0F) << 12); // irq_cpu_latch[15:12] = cpu_data_in[3:0];
					break; 
				case 4: // // $C000-$CFFF
					X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_out = 0; // ack
					REG_CPU_IRQ_control = (REG_CPU_IRQ_control & 0xF8) | (V & 7); // irq_cpu_control[2:0] = cpu_data_in[2:0]; // mode, enabled, enabled after ack
					if (REG_CPU_IRQ_control & 2) // if (irq_cpu_control[1]) begin // if E is set
						REG_CPU_IRQ_value = REG_CPU_IRQ_latch; // irq_cpu_value[7:0] = vrc4_irq_latch;	// reload with latch
					break;
				case 5: // // $D000-$DFFF
					X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_out = 0; // ack
					REG_CPU_IRQ_control = (REG_CPU_IRQ_control & 0xFD) | (REG_CPU_IRQ_control & 1) << 1; // irq_cpu_control[1] = irq_cpu_control[0];
					break;
				case 6: // $E000-$EFFF
					break;
				case 7: // $F000-$FFFF
					REG_PRG_bank_A = (REG_PRG_bank_A & 0xF1) | ((V & 7) << 1); // 3'b111: prg_bank_a[3:1] = cpu_data_in[2:0];
					break;
			}
		}

		// Mapper #4 - MMC3/MMC6
		/*
		r0[2:0] - internal register
		flag0 - TxSROM
		flag1 - mapper #189
		*/
		if (REG_mapper == 20)
		{
			switch (((A >> 12) & 6) | (A & 1))
			{
			case 0: // $8000-$9FFE, even
				REG_R0 = (REG_R0 & 0xF8) | (V & 7); // r0[2:0] = cpu_data_in[2:0];
				if (!(REG_flags & 2)) // if (!USE_MAPPER_189 | ~flags[1])
				{
					if (V & 0x40) // if (cpu_data_in[6])
						REG_PRG_mode = 5;
					else
						REG_PRG_mode = 4;
				}
				if (V & 0x80) // if (cpu_data_in[7])
					REG_CHR_mode = 3;
				else
					REG_CHR_mode = 2;
				break;
			case 1: // $8001-$9FFF, odd
				switch (REG_R0 & 7)
				{
				case 0:
					REG_CHR_bank_A = V; // chr_bank_a = cpu_data_in;
					break;
				case 1:
					REG_CHR_bank_C = V; // chr_bank_c = cpu_data_in;
					break;
				case 2:
					REG_CHR_bank_E = V; // chr_bank_e = cpu_data_in;
					break;
				case 3:
					REG_CHR_bank_F = V; // chr_bank_f = cpu_data_in;
					break;
				case 4:
					REG_CHR_bank_G = V; // chr_bank_g = cpu_data_in;
					break;
				case 5:
					REG_CHR_bank_H = V; // chr_bank_h = cpu_data_in;
					break;
				case 6:
					if (!(REG_flags & 2)) // if (!USE_MAPPER_189 | ~flags[1])
						REG_PRG_bank_A = (REG_PRG_bank_A & 0xC0) | (V & 0x3F); // prg_bank_a[5:0] = cpu_data_in[5:0];
					break;
				case 7:
					if (!(REG_flags & 2)) // if (!USE_MAPPER_189 | ~flags[1])
						REG_PRG_bank_B = (REG_PRG_bank_B & 0xC0) | (V & 0x3F); // prg_bank_b[5:0] = cpu_data_in[5:0];
					break;
				}
				break;
			case 2: // $A000-$BFFE, even (mirroring)
				REG_mirroring = V & 1;
				break;
			case 3: // RAM protect... no
				break;
			case 4: // $C000-$DFFE, even (IRQ latch)
				REG_scanline_IRQ_latch = V;
				break;
			case 5: // $C001-$DFFF, odd
				REG_scanline_IRQ_reload = 1;
				break;
			case 6: // $E000-$FFFE, even
				REG_scanline_IRQ_enabled = 0;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 7: // $E001-$FFFF, odd	
				REG_scanline_IRQ_enabled = 1;
				break;
			}
		}

		// Mapper #112
		/*
		r0[2:0] - internal register
		*/
		if (REG_mapper == 21)
		{
			switch ((A >> 13) & 3)
			{
			case 0: // $8000-$9FFF
				REG_R0 = (REG_R0 & 0xF8) | (V & 7); // r0[2:0] = cpu_data_in[2:0];
				break;
			case 1: // $A000-BFFF
				switch (REG_R0 & 7)
				{
				case 0:
					REG_PRG_bank_A = (REG_PRG_bank_A & 0xC0) | (V & 0x3F); // prg_bank_a[5:0] = cpu_data_in[5:0];
					break;
				case 1:
					REG_PRG_bank_B = (REG_PRG_bank_B & 0xC0) | (V & 0x3F); // prg_bank_b[5:0] = cpu_data_in[5:0];
					break;
				case 2:
					REG_CHR_bank_A = V; // chr_bank_a = cpu_data_in;
					break;
				case 3:
					REG_CHR_bank_C = V; // chr_bank_c = cpu_data_in;
					break;
				case 4:
					REG_CHR_bank_E = V; // chr_bank_e = cpu_data_in;
					break;
				case 5:
					REG_CHR_bank_F = V; // chr_bank_f = cpu_data_in;
					break;
				case 6:
					REG_CHR_bank_G = V; // chr_bank_g = cpu_data_in;
					break;
				case 7:
					REG_CHR_bank_H = V; // chr_bank_h = cpu_data_in;
					break;
				}
				break;
			case 3: // $E000-$FFFF
				REG_mirroring = V & 1;
				break;
			}
		}

		// Mappers #33 + #48 - Taito
		// flag0=0 - #33, flag0=1 - #48
		if (REG_mapper == 22)
		{
			// case ({cpu_addr_in[14:13], cpu_addr_in[1:0]})
			switch (((A >> 11) & (3 << 2)) | (A & 3))
			{
			case 0:
				REG_PRG_bank_A = (REG_PRG_bank_A & 0xC0) | (V & 0x3F); // prg_bank_a[5:0] = cpu_data_in[5:0]; // $8000, PRG Reg 0 (8k @ $8000)
				if (!(REG_flags & 1)) // if (~flags[0]) // 33
					REG_mirroring = (V >> 6) & 1; // mirroring = cpu_data_in[6];
				break;
			case 1: // 4'b0001: prg_bank_b[5:0] = cpu_data_in[5:0]; // $8001, PRG Reg 1 (8k @ $A000)
				REG_PRG_bank_B = (REG_PRG_bank_B & 0xC0) | (V & 0x3F); break;
			case 2: // 4'b0010: chr_bank_a = {cpu_data_in[6:0], 1'b0};  // $8002, CHR Reg 0 (2k @ $0000)
				REG_CHR_bank_A = V << 1; break;
			case 3: // 4'b0011: chr_bank_c = {cpu_data_in[6:0], 1'b0};  // $8003, CHR Reg 1 (2k @ $0800)
				REG_CHR_bank_C = V << 1; break;
			case 4: // 4'b0100: chr_bank_e = cpu_data_in;	// $A000, CHR Reg 2 (1k @ $1000)
				REG_CHR_bank_E = V; break;
			case 5: // 4'b0101: chr_bank_f = cpu_data_in; // $A001, CHR Reg 2 (1k @ $1400)
				REG_CHR_bank_F = V; break;
			case 6: // 4'b0110: chr_bank_g = cpu_data_in; // $A002, CHR Reg 2 (1k @ $1800)
				REG_CHR_bank_G = V; break;
			case 7: // 4'b0111: chr_bank_h = cpu_data_in; // $A003, CHR Reg 2 (1k @ $1C00)
				REG_CHR_bank_H = V; break;
			case 12: // 4'b1100: if (flags[0]) mirroring = cpu_data_in[6];	// $E000, mirroring, for mapper #48
				if (REG_flags & 1) // 48
					REG_mirroring = (V >> 6) & 1; // mirroring = cpu_data_in[6];
			case 8: // 4'b1000: irq_scanline_latch = ~cpu_data_in; // $C000, IRQ latch
				REG_scanline_IRQ_latch = V; break;
			case 9: // 4'b1001: irq_scanline_reload = 1; // $C001, IRQ reload
				REG_scanline_IRQ_reload = 1; break;
			case 10: // 4'b1010: irq_scanline_enabled = 1; // $C002, IRQ enable
				REG_scanline_IRQ_enabled = 1; break;
			case 11: // 4'b1011: irq_scanline_enabled = 0; // $C003, IRQ disable & ack
				REG_scanline_IRQ_enabled = 0;
				X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_out = 0; // ack
				break;
			}
		}
		
		// Mapper #23 - VRC2/4
		/*
		flag0 - switches A0 and A1 lines. 0=A0,A1 like VRC2b (mapper #23), 1=A1,A0 like VRC2a(#22), VRC2c(#25)
		flag1 - divides CHR bank select by two (mapper #22, VRC2a)
		*/
		if (REG_mapper == 24)
		{
			uint8 vrc_2b_hi = ((A >> 1) & 1) | ((A >> 3) & 1) | ((A >> 5) & 1) | ((A >> 7) & 1);
			uint8 vrc_2b_low = (A & 1) | ((A >> 2) & 1) | ((A >> 4) & 1) | ((A >> 6) & 1);

			// case ({ cpu_addr_in[14:12], flags[0] ? vrc_2b_low : vrc_2b_hi, flags[0] ? vrc_2b_hi : vrc_2b_low })
			switch (((A >> 10) & 0x1C) | (((REG_flags & 1) ? vrc_2b_low : vrc_2b_hi) << 1) | ((REG_flags & 1) ? vrc_2b_hi : vrc_2b_low))
			{
			case 0:	// $8000-$8003, PRG0
			case 1:
			case 2:
			case 3:
				REG_PRG_bank_A = (REG_PRG_bank_A & 0xE0) | (V & 0x1F); // prg_bank_a[4:0] = cpu_data_in[4:0];
				break;
			case 4: // $9000-$9001, mirroring
			case 5:
				if (V != 0xFF) // WTF?
					REG_mirroring = V & 3; // 3'b010: mirroring = cpu_data_in[1:0];
				break;
			case 6: // $9002-$9004, PRG swap
			case 7:
				REG_PRG_mode = (REG_PRG_mode & 0xFE) | ((V >> 1) & 1); // prg_mode[0] = cpu_data_in[1];
				break;
			case 8:	// $A000-$A003, PRG1
			case 9:
			case 10:
			case 11:
				REG_PRG_bank_B = (REG_PRG_bank_B & 0xE0) | (V & 0x1F); // prg_bank_b[4:0] = cpu_data_in[4:0];
				break;
			}
			if (!(REG_flags & 2))
			{
				// case ({ cpu_addr_in[14:12], flags[0] ? vrc_2b_low : vrc_2b_hi, flags[0] ? vrc_2b_hi : vrc_2b_low })
				switch (((A >> 10) & 0x1C) | (((REG_flags & 1) ? vrc_2b_low : vrc_2b_hi) << 1) | ((REG_flags & 1) ? vrc_2b_hi : vrc_2b_low))
				{
				case 12: // 5'b01100: chr_bank_a[3:0] = cpu_data_in[3:0];  // $B000, CHR0 low
					REG_CHR_bank_A = (REG_CHR_bank_A & 0xF0) | (V & 0x0F); break;
				case 13: // 5'b01101: chr_bank_a[7:4] = cpu_data_in[3:0];  // $B001, CHR0 hi
					REG_CHR_bank_A = (REG_CHR_bank_A & 0x0F) | ((V & 0x0F) << 4); break;
				case 14: // 5'b01110: chr_bank_b[3:0] = cpu_data_in[3:0];  // $B002, CHR1 low
					REG_CHR_bank_B = (REG_CHR_bank_B & 0xF0) | (V & 0x0F); break;
				case 15: // 5'b01111: chr_bank_b[7:4] = cpu_data_in[3:0];  // $B003, CHR1 hi
					REG_CHR_bank_B = (REG_CHR_bank_B & 0x0F) | ((V & 0x0F) << 4); break;
				case 16: // 5'b10000: chr_bank_c[3:0] = cpu_data_in[3:0];  // $C000, CHR2 low
					REG_CHR_bank_C = (REG_CHR_bank_C & 0xF0) | (V & 0x0F); break;
				case 17: // 5'b10001: chr_bank_c[7:4] = cpu_data_in[3:0];  // $C001, CHR2 hi
					REG_CHR_bank_C = (REG_CHR_bank_C & 0x0F) | ((V & 0x0F) << 4); break;
				case 18: // 5'b10010: chr_bank_d[3:0] = cpu_data_in[3:0];  // $C002, CHR3 low
					REG_CHR_bank_D = (REG_CHR_bank_D & 0xF0) | (V & 0x0F); break;
				case 19: // 5'b10011: chr_bank_d[7:4] = cpu_data_in[3:0];  // $C003, CHR3 hi
					REG_CHR_bank_D = (REG_CHR_bank_D & 0x0F) | ((V & 0x0F) << 4); break;
				case 20: // 5'b10100: chr_bank_e[3:0] = cpu_data_in[3:0];  // $D000, CHR4 low
					REG_CHR_bank_E = (REG_CHR_bank_E & 0xF0) | (V & 0x0F); break;
				case 21: // 5'b10101: chr_bank_e[7:4] = cpu_data_in[3:0];  // $D001, CHR4 hi
					REG_CHR_bank_E = (REG_CHR_bank_E & 0x0F) | ((V & 0x0F) << 4); break;
				case 22: // 5'b10110: chr_bank_f[3:0] = cpu_data_in[3:0];  // $D002, CHR5 low
					REG_CHR_bank_F = (REG_CHR_bank_F & 0xF0) | (V & 0x0F); break;
				case 23: // 5'b10111: chr_bank_f[7:4] = cpu_data_in[3:0];  // $D003, CHR5 hi
					REG_CHR_bank_F = (REG_CHR_bank_F & 0x0F) | ((V & 0x0F) << 4); break;
				case 24: // 5'b11000: chr_bank_g[3:0] = cpu_data_in[3:0];  // $E000, CHR6 low
					REG_CHR_bank_G = (REG_CHR_bank_G & 0xF0) | (V & 0x0F); break;
				case 25: // 5'b11001: chr_bank_g[7:4] = cpu_data_in[3:0];  // $E001, CHR6 hi
					REG_CHR_bank_G = (REG_CHR_bank_G & 0x0F) | ((V & 0x0F) << 4); break;
				case 26: // 5'b11010: chr_bank_h[3:0] = cpu_data_in[3:0];  // $E002, CHR7 low
					REG_CHR_bank_H = (REG_CHR_bank_H & 0xF0) | (V & 0x0F); break;
				case 27: // 5'b11011: chr_bank_h[7:4] = cpu_data_in[3:0];  // $E003, CHR7 hi
					REG_CHR_bank_H = (REG_CHR_bank_H & 0x0F) | ((V & 0x0F) << 4); break;
				}
			}
			else {
				// VRC2a
				// case ({ cpu_addr_in[14:12], flags[0] ? vrc_2b_low : vrc_2b_hi, flags[0] ? vrc_2b_hi : vrc_2b_low })
				switch (((A >> 10) & 0x1C) | (((REG_flags & 1) ? vrc_2b_low : vrc_2b_hi) << 1) | ((REG_flags & 1) ? vrc_2b_hi : vrc_2b_low))
				{
				case 12: // 5'b01100: chr_bank_a[2:0] = cpu_data_in[3:1];  // $B000, CHR0 low
					REG_CHR_bank_A = (REG_CHR_bank_A & 0x78) | ((V & 0x0E) >> 1); break;
				case 13: // 5'b01101: chr_bank_a[7:3] = {1'b0, cpu_data_in[3:0]};  // $B001, CHR0 hi
					REG_CHR_bank_A = (REG_CHR_bank_A & 0x07) | ((V & 0x0F) << 3); break;
				case 14: // 5'b01110: chr_bank_b[2:0] = cpu_data_in[3:1];  // $B002, CHR1 low
					REG_CHR_bank_B = (REG_CHR_bank_B & 0x78) | ((V & 0x0E) >> 1); break;
				case 15: // 5'b01111: chr_bank_b[7:3] = {1'b0, cpu_data_in[3:0]};  // $B003, CHR1 hi
					REG_CHR_bank_B = (REG_CHR_bank_B & 0x07) | ((V & 0x0F) << 3); break;
				case 16: // 5'b10000: chr_bank_c[2:0] = cpu_data_in[3:1];  // $C000, CHR2 low
					REG_CHR_bank_C = (REG_CHR_bank_C & 0x78) | ((V & 0x0E) >> 1); break;
				case 17: // 5'b10001: chr_bank_c[7:3] = {1'b0, cpu_data_in[3:0]};  // $C001, CHR2 hi
					REG_CHR_bank_C = (REG_CHR_bank_C & 0x07) | ((V & 0x0F) << 3); break;
				case 18: // 5'b10010: chr_bank_d[2:0] = cpu_data_in[3:1];  // $C002, CHR3 low
					REG_CHR_bank_D = (REG_CHR_bank_D & 0x78) | ((V & 0x0E) >> 1); break;
				case 19: // 5'b10011: chr_bank_d[7:3] = {1'b0, cpu_data_in[3:0]};  // $C003, CHR3 hi
					REG_CHR_bank_D = (REG_CHR_bank_D & 0x07) | ((V & 0x0F) << 3); break;
				case 20: // 5'b10100: chr_bank_e[2:0] = cpu_data_in[3:1];  // $D000, CHR4 low
					REG_CHR_bank_E = (REG_CHR_bank_E & 0x78) | ((V & 0x0E) >> 1); break;
				case 21: // 5'b10101: chr_bank_e[7:3] = {1'b0, cpu_data_in[3:0]};  // $D001, CHR4 hi
					REG_CHR_bank_E = (REG_CHR_bank_E & 0x07) | ((V & 0x0F) << 3); break;
				case 22: // 5'b10110: chr_bank_f[2:0] = cpu_data_in[3:1];  // $D002, CHR5 low
					REG_CHR_bank_F = (REG_CHR_bank_F & 0x78) | ((V & 0x0E) >> 1); break;
				case 23: // 5'b10111: chr_bank_f[7:3] = {1'b0, cpu_data_in[3:0]};  // $D003, CHR5 hi
					REG_CHR_bank_F = (REG_CHR_bank_F & 0x07) | ((V & 0x0F) << 3); break;
				case 24: // 5'b11000: chr_bank_g[2:0] = cpu_data_in[3:1];  // $E000, CHR6 low
					REG_CHR_bank_G = (REG_CHR_bank_G & 0x78) | ((V & 0x0E) >> 1); break;
				case 25: // 5'b11001: chr_bank_g[7:3] = {1'b0, cpu_data_in[3:0]};  // $E001, CHR6 hi
					REG_CHR_bank_G = (REG_CHR_bank_G & 0x07) | ((V & 0x0F) << 3); break;
				case 26: // 5'b11010: chr_bank_h[2:0] = cpu_data_in[3:1];  // $E002, CHR7 low
					REG_CHR_bank_H = (REG_CHR_bank_H & 0x78) | ((V & 0x0E) >> 1); break;
				case 27: // 5'b11011: chr_bank_h[7:3] = {1'b0, cpu_data_in[3:0]};  // $E003, CHR7 hi
					REG_CHR_bank_H = (REG_CHR_bank_H & 0x07) | ((V & 0x0F) << 3); break;
				}
			}

			// if (cpu_addr_in[14:12] == 3'b111)
			if (((A >> 12) & 7) == 7)
			{
				// case ({flags[0] ? vrc_2b_low : vrc_2b_hi, flags[0] ? vrc_2b_hi : vrc_2b_low}) 
				switch ((((REG_flags & 1) ? vrc_2b_low : vrc_2b_hi) << 1) | ((REG_flags & 1) ? vrc_2b_hi : vrc_2b_low))
				{
				case 0: // 2'b00: vrc4_irq_latch[3:0] = cpu_data_in[3:0];  // IRQ latch low
					REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0xF0) | (V & 0x0F); break;
				case 1: // 2'b01: vrc4_irq_latch[7:4] = cpu_data_in[3:0];  // IRQ latch hi
					REG_CPU_IRQ_latch = (REG_CPU_IRQ_latch & 0x0F) | ((V & 0x0F) << 4); break;
				case 2: // 2'b10 // IRQ control
					X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_out = 0; // ack
					REG_CPU_IRQ_control = (REG_CPU_IRQ_control & 0xF8) | (V & 7); // irq_cpu_control[2:0] = cpu_data_in[2:0]; // mode, enabled, enabled after ack
					if (REG_CPU_IRQ_control & 2) // if (irq_cpu_control[1]) begin // if E is set
					{
						REG_VRC4_IRQ_prescaler_counter = 0; // vrc4_irq_prescaler_counter = 2'b00; // reset prescaler
						REG_VRC4_IRQ_prescaler = 0;
						REG_CPU_IRQ_value = REG_CPU_IRQ_latch; // irq_cpu_value[7:0] = vrc4_irq_latch;	// reload with latch
					}
					break;
				case 3: // 2'b11 // IRQ ack
					X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_out = 0; // ack
					REG_CPU_IRQ_control = (REG_CPU_IRQ_control & 0xFD) | (REG_CPU_IRQ_control & 1) << 1; // irq_cpu_control[1] = irq_cpu_control[0];
					break;
				}
			}
		}
		
		// Mapper #69 - Sunsoft FME-7
		/*
		r0 - command register
		*/
		if (REG_mapper == 25)
		{
			// if (cpu_addr_in[14:13] == 2'b00) r0[3:0] = cpu_data_in[3:0];
			if (((A >> 13) & 3) == 0) REG_R0 = (REG_R0 & 0xF0) | (V & 0x0F);
			// if (cpu_addr_in[14:13] == 2'b01)
			if (((A >> 13) & 3) == 1)
			{
				switch (REG_R0 & 0x0F) // case (r0[3:0])
				{
				case 0: // 4'b0000: chr_bank_a = cpu_data_in; // CHR0
					REG_CHR_bank_A = V; break;
				case 1: // 4'b0001: chr_bank_b = cpu_data_in; // CHR1
					REG_CHR_bank_B = V; break;
				case 2: // 4'b0010: chr_bank_c = cpu_data_in; // CHR2
					REG_CHR_bank_C = V; break;
				case 3: // 4'b0011: chr_bank_d = cpu_data_in; // CHR3
					REG_CHR_bank_D = V; break;
				case 4: // 4'b0100: chr_bank_e = cpu_data_in; // CHR4
					REG_CHR_bank_E = V; break;
				case 5: // 4'b0101: chr_bank_f = cpu_data_in; // CHR5
					REG_CHR_bank_F = V; break;
				case 6: // 4'b0110: chr_bank_g = cpu_data_in; // CHR6
					REG_CHR_bank_G = V; break;
				case 7: // 4'b0111: chr_bank_h = cpu_data_in; // CHR7
					REG_CHR_bank_H = V; break;
				case 8: // 4'b1000: {sram_enabled, map_rom_on_6000, prg_bank_6000} = {cpu_data_in[7], ~cpu_data_in[6], cpu_data_in[5:0]}; // PRG0
					REG_WRAM_enabled = (V >> 7) & 1;
					REG_map_ROM_on_6000 = ((V >> 6) & 1) ^ 1;
					REG_PRG_bank_6000 = V & 0x3F;
					break;
				case 9: // 4'b1001: prg_bank_a[5:0] = cpu_data_in[5:0]; // PRG1
					REG_PRG_bank_A = (REG_PRG_bank_A & 0xC0) | (V & 0x3F); break;
				case 10: // 4'b1010: prg_bank_b[5:0] = cpu_data_in[5:0]; // PRG2
					REG_PRG_bank_B = (REG_PRG_bank_B & 0xC0) | (V & 0x3F); break;
				case 11: // 4'b1011: prg_bank_c[5:0] = cpu_data_in[5:0]; // PRG3
					REG_PRG_bank_C = (REG_PRG_bank_C & 0xC0) | (V & 0x3F); break;
				case 12: // 4'b1100: mirroring[1:0] = cpu_data_in[1:0]; // mirroring
					REG_mirroring = V & 3; break;
				case 13: // 4'b1101
					REG_CPU_IRQ_control = ((V >> 6) & 1) | (V & 1); // irq_cpu_control[1:0] = {cpu_data_in[7], cpu_data_in[0]}; // IRQ control
					X6502_IRQEnd(FCEU_IQEXT); // irq_cpu_out = 0; // ack
					break;
				case 14: // 4'b1110: irq_cpu_value[7:0] = cpu_data_in; // IRQ low
					REG_CPU_IRQ_value = (REG_CPU_IRQ_value & 0xFF00) | V; break;
				case 15: // 4'b1111: irq_cpu_value[15:8] = cpu_data_in; // IRQ high
					REG_CPU_IRQ_value = (REG_CPU_IRQ_value & 0x00FF) | (V << 8); break;
				}
			}
		}

		// Mapper #32 - IREM G-101
		if (REG_mapper == 26)
		{
			switch ((A >> 12) & 3)
			{
			case 0: // PRG0
				REG_PRG_bank_A = (REG_PRG_bank_A & 0xC0) | (V & 0x3F); // prg_bank_a[5:0] = cpu_data_in[5:0];
				break;
			case 1: // PRG mode, mirroring
				// {prg_mode[0], mirroring} = {cpu_data_in[1], 1'b0, cpu_data_in[0]};
				REG_PRG_mode = (REG_PRG_mode & 6) | ((V >> 1) & 1);
				REG_mirroring = V & 1;
				break;
			case 2: // PRG1
				REG_PRG_bank_B = (REG_PRG_bank_B & 0xC0) | (V & 0x3F); // prg_bank_b[5:0] = cpu_data_in[5:0];
				break;
			case 3: // CHR regs
				switch (A & 7)
				{
				case 0:
					REG_CHR_bank_A = V;
					break;
				case 1:
					REG_CHR_bank_B = V;
					break;
				case 2:
					REG_CHR_bank_C = V;
					break;
				case 3:
					REG_CHR_bank_D = V;
					break;
				case 4:
					REG_CHR_bank_E = V;
					break;
				case 5:
					REG_CHR_bank_F = V;
					break;
				case 6:
					REG_CHR_bank_G = V;
					break;
				case 7:
					REG_CHR_bank_H = V;
					break;
				}
				break;
			}
		}

		// temp/test
		if (REG_mapper == 31)
		{
			REG_PRG_bank_6000 = ((V>>1)&0xF) + 4;
			REG_map_ROM_on_6000 = 1;
		}
	}

	COOLGIRL_Sync();
}

static DECLFR(MAWRAM) {
	if (REG_mapper == 0 && A >= 0x5000 && A < 0x6000)
		return 0;

	// Mapper #163
	if (REG_mapper == 6 && ((A & 0x7700) == 0x5100)) //	(USE_MAPPER_163 && (mapper == 5'b00110) && m2 && romsel && cpu_rw_in && ((cpu_addr_in[14:8] & 7'h77) == 7'h51)) ?  
		return REG_R2 | REG_R0 | REG_R1 | ~REG_R3;	// {1'b1, r2 | r0 | r1 | ~r3}				
	if (REG_mapper == 6 && ((A & 0x7700) == 0x5500)) // (USE_MAPPER_163 && (mapper == 5'b00110) && m2 && romsel && cpu_rw_in && ((cpu_addr_in[14:8] & 7'h77) == 7'h55)) ? 
		return (REG_R5&1) ? REG_R2 : REG_R1;	// {1'b1, r5[0] ? r2 : r1}
	
	// MMC5
	if (REG_mapper == 15 && A == 0x5204)
	{
		uint8 p = REG_scanline2_IRQ_pending;
		int ppuon = (PPU[1] & 0x18);
		uint8 r = (!ppuon || scanline+1 >= 241) ? 0 : 1;
		X6502_IRQEnd(FCEU_IQEXT);
		REG_scanline2_IRQ_pending = 0;
		return (p<<7) | (r<<6);
	}

	// Mapper #90 - JY
	/*
	if ((REG_mapper == 13) && (A == 0x5800)) return (REG_mul1 * REG_mul2) & 0xFF;
	if ((REG_mapper == 13) && (A == 0x5801)) return ((REG_mul1 * REG_mul2) >> 8) & 0xFF;
	*/
	
	if (REG_WRAM_enabled && !REG_map_ROM_on_6000 && A >= 0x6000 && A < 0x8000)
		return CartBR(A);          // WRAM
	if (REG_map_ROM_on_6000 && A >= 0x6000 && A < 0x8000)
		return CartBR(A);          // PRG
	return X.DB;
}

static void COOLGIRL_ScanlineCounter(void) {
	// for MMC3 and MMC3-based
	if (REG_scanline_IRQ_reload || !REG_scanline_IRQ_counter)
	{
		REG_scanline_IRQ_counter = REG_scanline_IRQ_latch;
		REG_scanline_IRQ_reload = 0;
	}
	else
		REG_scanline_IRQ_counter--;
	if (!REG_scanline_IRQ_counter && REG_scanline_IRQ_enabled)
		X6502_IRQBegin(FCEU_IQEXT);

	// for MMC5
	if (REG_scanline2_IRQ_line == scanline+1)
	{
		if (REG_scanline2_IRQ_enabled)
		{
			X6502_IRQBegin(FCEU_IQEXT);
			REG_scanline2_IRQ_pending = 1;
		}
	}

	// for mapper #163
	if (REG_mapper == 6)
	{
		if (scanline == 239)
		{
			ppu_mapper163_latch = 0;
			COOLGIRL_Sync_CHR();
		} else if (scanline == 127)
		{
			ppu_mapper163_latch = 1;
			COOLGIRL_Sync_CHR();
		}
	}
}

static void COOLGIRL_CpuCounter(int a) {
	while (a--)
	{
		// Mapper #23 - VRC4
		if (REG_mapper == 24 && (REG_CPU_IRQ_control & 2))
		{
			// Cycle mode without prescaler is not used by any games? It's missed in fceux source code.
			if (REG_CPU_IRQ_control & 4) // if (irq_cpu_control[2]) // cycle mode
			{
				FCEU_PrintError("IRQ mode is not supported, please report to Cluster");
				REG_CPU_IRQ_value++; // irq_cpu_value[7:0] = irq_cpu_value[7:0] + 1'b1;
				if ((REG_CPU_IRQ_value & 0xFF) == 0) // if (irq_cpu_value[7:0] == 0)
				{
					X6502_IRQBegin(FCEU_IQEXT);
					REG_CPU_IRQ_value = REG_CPU_IRQ_latch; // irq_cpu_value[7:0] = vrc4_irq_latch;
				}
			}
			else {
				REG_VRC4_IRQ_prescaler++; // vrc4_irq_prescaler = vrc4_irq_prescaler + 1'b1; // count prescaler
				// if ((vrc4_irq_prescaler_counter[1] == 0 && vrc4_irq_prescaler == 114) || (vrc4_irq_prescaler_counter[1] == 1 && vrc4_irq_prescaler == 113)) // 114, 114, 113
				if ((!(REG_VRC4_IRQ_prescaler_counter&2) && REG_VRC4_IRQ_prescaler == 114) || ((REG_VRC4_IRQ_prescaler_counter&2) && REG_VRC4_IRQ_prescaler == 113))
				{
					REG_CPU_IRQ_value++; // irq_cpu_value[7:0] = irq_cpu_value[7:0] + 1'b1;
					REG_VRC4_IRQ_prescaler = 0; // vrc4_irq_prescaler = 0;
					REG_VRC4_IRQ_prescaler_counter++; // vrc4_irq_prescaler_counter = vrc4_irq_prescaler_counter + 1'b1;
					if (REG_VRC4_IRQ_prescaler_counter == 3) REG_VRC4_IRQ_prescaler_counter = 0; // if (vrc4_irq_prescaler_counter == 2'b11) vrc4_irq_prescaler_counter =  2'b00;
					if ((REG_CPU_IRQ_value & 0xFF) == 0) // if (irq_cpu_value[7:0] == 0)
					{
						X6502_IRQBegin(FCEU_IQEXT);
						REG_CPU_IRQ_value = REG_CPU_IRQ_latch; // irq_cpu_value[7:0] = vrc4_irq_latch;
					}
				}
			}
		}

		// Mapper #73 - VRC3
		if (REG_mapper == 19 && (REG_CPU_IRQ_control & 2))
		{
			if (REG_CPU_IRQ_control & 4) // if (irq_cpu_control[2])
			{  // 8-bit mode
				REG_CPU_IRQ_value = (REG_CPU_IRQ_value & 0xFF00) | ((REG_CPU_IRQ_value+1) & 0xFF); // irq_cpu_value[7:0] = irq_cpu_value[7:0] + 1'b1;
				if ((REG_CPU_IRQ_value & 0xFF) == 0) // if (irq_cpu_value[7:0] == 0)
				{
					X6502_IRQBegin(FCEU_IQEXT); // irq_cpu_out = 1;
					REG_CPU_IRQ_value = (REG_CPU_IRQ_value & 0xFF00) | (REG_CPU_IRQ_latch & 0xFF); // irq_cpu_value[7:0] = irq_cpu_latch[7:0];
				}
			} else { // 16-bit
				REG_CPU_IRQ_value++; // irq_cpu_value[15:0] = irq_cpu_value[15:0] + 1'b1;
				if ((REG_CPU_IRQ_value & 0xFFFF) == 0)
				{
					X6502_IRQBegin(FCEU_IQEXT); // irq_cpu_out = 1;
					REG_CPU_IRQ_value = REG_CPU_IRQ_latch; //irq_cpu_value[15:0] = irq_cpu_latch[15:0];
				}
			}
		}

		// Mapper #69 - Sunsoft FME-7
		if (REG_mapper == 25)
		{
			// if ((irq_cpu_value[15:0] == 0) & irq_cpu_control[0]) irq_cpu_out = 1;
			if (((REG_CPU_IRQ_value & 0xFFFFF) == 0) && (REG_CPU_IRQ_control&1))
				X6502_IRQBegin(FCEU_IQEXT);
			// irq_cpu_value[15:0] = irq_cpu_value[15:0] - 1'b1;
			REG_CPU_IRQ_value--;
		}

		// Mapper #18
		if (REG_mapper == 7)
		{
			if (REG_CPU_IRQ_control & 1) //	if (irq_cpu_control[0])
			{
				if (REG_CPU_IRQ_control & 8) // if (irq_cpu_control[3])				
				{
					if ((REG_CPU_IRQ_value & 0x000F) == 0) X6502_IRQBegin(FCEU_IQEXT); // if (irq_cpu_value[3:0] == 0) irq_cpu_out = 1;
					REG_CPU_IRQ_value = (REG_CPU_IRQ_value & 0xFFF0) | ((REG_CPU_IRQ_value - 1) & 0x000F); // irq_cpu_value[3:0] = irq_cpu_value[3:0] - 1'b1;
				} else if (REG_CPU_IRQ_control & 4) // if (irq_cpu_control[2]) begin
				{
					if ((REG_CPU_IRQ_value & 0x00FF) == 0) X6502_IRQBegin(FCEU_IQEXT); // if (irq_cpu_value[7:0] == 0) irq_cpu_out = 1;
					REG_CPU_IRQ_value = (REG_CPU_IRQ_value & 0xFF00) | ((REG_CPU_IRQ_value - 1) & 0x00FF); // irq_cpu_value[7:0] = irq_cpu_value[7:0] - 1'b1;				
				} else if (REG_CPU_IRQ_control & 2) // if (irq_cpu_control[1]) begin
				{
					if ((REG_CPU_IRQ_value & 0x0FFF) == 0) X6502_IRQBegin(FCEU_IQEXT); // if (irq_cpu_value[11:0] == 0) irq_cpu_out = 1;
					REG_CPU_IRQ_value = (REG_CPU_IRQ_value & 0xF000) | ((REG_CPU_IRQ_value - 1) & 0x0FFF); // irq_cpu_value[11:0] = irq_cpu_value[11:0] - 1'b1;
				} else {
					if ((REG_CPU_IRQ_value & 0xFFFF) == 0) X6502_IRQBegin(FCEU_IQEXT); // if (irq_cpu_value[15:0] == 0) irq_cpu_out = 1;
					REG_CPU_IRQ_value = ((REG_CPU_IRQ_value - 1) & 0xFFFF); // irq_cpu_value[15:0] = irq_cpu_value[15:0] - 1'b1;
				}
			}
		}

		// Mapper #65 - Irem's H3001
		if (REG_mapper == 14)
		{
			if (REG_CPU_IRQ_control & 1) //	if (irq_cpu_control[0])
			{
				if ((REG_CPU_IRQ_value & 0xFFFF) > 0) // if (irq_cpu_value[15:0] > 0)
				{
					REG_CPU_IRQ_value--; // irq_cpu_value[15:0] = irq_cpu_value[15:0] - 1;
					if (!REG_CPU_IRQ_value) 

						X6502_IRQBegin(FCEU_IQEXT); // if (irq_cpu_value[15:0] == 0) irq_cpu_out = 1;
				}
			}
		}
	}
}

static void COOLGIRL_PPUHook(uint32 A) {
	// For TxROM
	if ((REG_mapper == 20) && (REG_flags & 1))
	{
		setmirror(MI_0 + (TKSMIR[(A & 0x1FFF) >> 10] >> 7));
	}

	// Mapper #9 and #10 - MMC2 and MMC4
	if (REG_mapper == 17)
	{
		if ((A >> 4) == 0xFD) // if (ppu_addr_in[13:3] == 11'b00111111011) ppu_latch0 = 0;
		{
			ppu_latch0 = 0;
			COOLGIRL_Sync_CHR();
		}
		if ((A >> 4) == 0xFE) // if (ppu_addr_in[13:3] == 11'b00111111101) ppu_latch0 = 1;
		{
			ppu_latch0 = 1;
			COOLGIRL_Sync_CHR();
		}
		if ((A >> 4) == 0x1FD) // if (ppu_addr_in[13:3] == 11'b01111111011) ppu_latch1 = 0;
		{
			ppu_latch1 = 0;
			COOLGIRL_Sync_CHR();
		}
		if ((A >> 4) == 0x1FE) // if (ppu_addr_in[13:3] == 11'b01111111101) ppu_latch1 = 1;
		{
			ppu_latch1 = 1;
			COOLGIRL_Sync_CHR();
		}
	}
}

static void COOLGIRL_Reset(void) {
	lreset = 0;
	REG_WRAM_enabled = 0;
	REG_WRAM_page = 0;
	REG_can_write_CHR_RAM = 0;
	REG_map_ROM_on_6000 = 0;
	REG_flags = 0;
	REG_mapper = 0;
	REG_can_write_PRG = 0;
	REG_mirroring = 0;
	REG_four_screen = 0;
	REG_lockout = 0;
	REG_PRG_base = 0;
	REG_PRG_mask = 0xF8; // 11111000, 128KB
	REG_PRG_mode = 0;
	REG_PRG_bank_6000 = 0;
	REG_PRG_bank_A = 0;
	REG_PRG_bank_B = 1; 
	REG_PRG_bank_C = ~1;
	REG_PRG_bank_D = ~0;
	REG_CHR_mask = 0;
	REG_CHR_mode = 0;
	REG_CHR_bank_A = 0;
	REG_CHR_bank_B = 1;
	REG_CHR_bank_C = 2;
	REG_CHR_bank_D = 3;
	REG_CHR_bank_E = 4;
	REG_CHR_bank_F = 5;
	REG_CHR_bank_G = 6;
	REG_CHR_bank_H = 7;
	REG_scanline_IRQ_enabled = 0;
	REG_scanline_IRQ_counter = 0;
	REG_scanline_IRQ_latch = 0;
	REG_scanline_IRQ_reload = 0;
	REG_scanline2_IRQ_enabled = 0;
	REG_scanline2_IRQ_line = 0;
	REG_CPU_IRQ_value = 0;
	REG_CPU_IRQ_control = 0;
	REG_CPU_IRQ_latch = 0;
	REG_VRC4_IRQ_prescaler = 0;
	REG_VRC4_IRQ_prescaler_counter = 0;
	REG_R0 = 0;
	REG_R1 = 0;
	REG_R2 = 0;
	REG_R3 = 0;
	REG_R4 = 0;
	REG_R5 = 0;

	COOLGIRL_Sync();
}

static void COOLGIRL_Power(void) {
	FCEU_CheatAddRAM(32, 0x6000, WRAM);
	SetReadHandler(0x4020, 0x7FFF, MAWRAM);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0xFFFF, COOLGIRL_WRITE);
	GameHBIRQHook = COOLGIRL_ScanlineCounter;
	MapIRQHook = COOLGIRL_CpuCounter;
	PPU_hook = COOLGIRL_PPUHook;
	COOLGIRL_Reset();
}

static void COOLGIRL_Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	if (SAVE_FLASH)
		FCEU_gfree(SAVE_FLASH);
	WRAM = SAVE_FLASH = NULL;
}

static void COOLGIRL_Restore(int version) {
	COOLGIRL_Sync();
	lreset = 0;
}

void COOLGIRL_Init(CartInfo *info) {
	WRAM = (uint8*)FCEU_gmalloc(32 * 1024);
	memset(WRAM, 0, 32 * 1024);
	SetupCartPRGMapping(0x10, WRAM, 32 * 1024, 1);
	AddExState(WRAM, 32 * 1024, 0, "WRAM");
	info->SaveGame[0] = WRAM;
	info->SaveGameLen[0] = 32 * 1024;

	SAVE_FLASH = (uint8*)FCEU_gmalloc(save_flash_size);
	SetupCartPRGMapping(0x11, SAVE_FLASH, save_flash_size, 1);
	AddExState(SAVE_FLASH, save_flash_size, 0, "SAVF");
	info->SaveGame[1] = SAVE_FLASH;
	info->SaveGameLen[1] = save_flash_size;

	AddExState(&REG_WRAM_enabled, 1, 0, "WREN");
	AddExState(&REG_WRAM_page, 1, 0, "WRPG");
	AddExState(&REG_can_write_CHR_RAM, 1, 0, "CHWR");
	AddExState(&REG_map_ROM_on_6000, 1, 0, "MA60");
	AddExState(&REG_flags, 1, 0, "FLAG");
	AddExState(&REG_mapper, 1, 0, "MAPR");
	AddExState(&REG_can_write_PRG, 1, 0, "PRWR");
	AddExState(&REG_mirroring, 1, 0, "MIRR");
	AddExState(&REG_four_screen, 1, 0, "4SCR");
	AddExState(&REG_lockout, 1, 0, "LOCK");
	AddExState(&REG_PRG_base, 2, 0, "PRBS");
	AddExState(&REG_PRG_mask, 1, 0, "PRMS");
	AddExState(&REG_PRG_mode, 1, 0, "PRMD");
	AddExState(&REG_PRG_bank_6000, 1, 0, "PRG6");
	AddExState(&REG_PRG_bank_A, 1, 0, "PRGA");
	AddExState(&REG_PRG_bank_B, 1, 0, "PRGB");
	AddExState(&REG_PRG_bank_C, 1, 0, "PRGC");
	AddExState(&REG_PRG_bank_D, 1, 0, "PRGD");
	AddExState(&REG_CHR_mask, 1, 0, "CHMS");
	AddExState(&REG_CHR_mode, 1, 0, "CHMD");
	AddExState(&REG_CHR_bank_A, 1, 0, "CHRA");
	AddExState(&REG_CHR_bank_B, 1, 0, "CHRB");
	AddExState(&REG_CHR_bank_C, 1, 0, "CHRC");
	AddExState(&REG_CHR_bank_D, 1, 0, "CHRD");
	AddExState(&REG_CHR_bank_E, 1, 0, "CHRE");
	AddExState(&REG_CHR_bank_F, 1, 0, "CHRF");
	AddExState(&REG_CHR_bank_G, 1, 0, "CHRG");
	AddExState(&REG_CHR_bank_H, 1, 0, "CHRH");
	AddExState(&REG_scanline_IRQ_enabled, 1, 0, "IRQ1");
	AddExState(&REG_scanline_IRQ_counter, 1, 0, "IRQ2");
	AddExState(&REG_scanline_IRQ_latch, 1, 0, "IRQ3");
	AddExState(&REG_scanline_IRQ_reload, 1, 0, "IRQ4");
	AddExState(&REG_CPU_IRQ_value, 2, 0, "IRQ5");
	AddExState(&REG_CPU_IRQ_control, 1, 0, "IRQ6");
	AddExState(&REG_CPU_IRQ_latch, 2, 0, "IRQ7");
	AddExState(&REG_VRC4_IRQ_prescaler, 1, 0, "IRQ8");
	AddExState(&REG_VRC4_IRQ_prescaler_counter, 1, 0, "IRQ9");
	AddExState(&REG_scanline2_IRQ_enabled, 1, 0, "IRQA");
	AddExState(&REG_scanline2_IRQ_line, 1, 0, "IRQB");
	AddExState(&REG_R0, 1, 0, "REG0");
	AddExState(&REG_R1, 1, 0, "REG1");
	AddExState(&REG_R2, 1, 0, "REG2");
	AddExState(&REG_R3, 1, 0, "REG3");
	AddExState(&REG_R4, 1, 0, "REG4");
	AddExState(&REG_R5, 1, 0, "REG5");

	info->Power = COOLGIRL_Power;
	info->Reset = COOLGIRL_Reset;
	info->Close = COOLGIRL_Close;
	GameStateRestore = COOLGIRL_Restore;
}

