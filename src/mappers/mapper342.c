/* FCEUmm - NES/Famicom Emulator
*
* Copyright notice for this file:
*  Copyright (C) 2022 Cluster
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
*
* Very complicated homebrew multicart mapper with.
* The code is so obscured and weird because it's ported from Verilog CPLD source code:
* https://github.com/ClusterM/coolgirl-famicom-multicart/blob/master/CoolGirl_mappers.vh
*
* Range: $5000-$5FFF
*
* Mask: $5007
*
* All registers are $00 on power-on and reset.
*
* $5xx0
* 7  bit  0
* ---- ----
* PPPP PPPP
* |||| ||||
* ++++-++++-- PRG base offset (A29-A22)
*
* $5xx1
* 7  bit  0
* ---- ----
* PPPP PPPP
* |||| ||||
* ++++-++++-- PRG base offset (A21-A14)
*
* $5xx2
* 7  bit  0
* ---- ----
* AMMM MMMM
* |||| ||||
* |+++-++++-- PRG mask (A20-A14, inverted+anded with PRG address)
* +---------- CHR mask (A18, inverted+anded with CHR address)
*
* $5xx3
* 7  bit  0
* ---- ----
* BBBC CCCC
* |||| ||||
* |||+-++++-- CHR bank A (bits 7-3)
* +++-------- PRG banking mode (see below)
*
* $5xx4
* 7  bit  0
* ---- ----
* DDDE EEEE
* |||| ||||
* |||+-++++-- CHR mask (A17-A13, inverted+anded with CHR address)
* +++-------- CHR banking mode (see below)
*
* $5xx5
* 7  bit  0
* ---- ----
* CDDE EEWW
* |||| ||||
* |||| ||++-- 8KiB WRAM page at $6000-$7FFF
* |+++-++---- PRG bank A (bits 5-1)
* +---------- CHR bank A (bit 8)
*
* $5xx6
* 7  bit  0
* ---- ----
* FFFM MMMM
* |||| ||||
* |||+ ++++-- Mapper code (bits 4-0, see below)
* +++-------- Flags 2-0, functionality depends on selected mapper
*
* $5xx7
* 7  bit  0
* ---- ----
* LMTR RSNO
* |||| |||+-- Enable WRAM (read and write) at $6000-$7FFF
* |||| ||+--- Allow writes to CHR RAM
* |||| |+---- Allow writes to flash chip
* |||+-+----- Mirroring (00=vertical, 01=horizontal, 10=1Sa, 11=1Sb)
* ||+-------- Enable four-screen mode
* |+-- ------ Mapper code (bit 5, see below)
* +---------- Lockout bit (prevent further writes to all registers)
*
*/

#include "mapinc.h"

#define SAVE_FLASH_SIZE (1024 * 1024 * 8)
#define FLASH_SECTOR_SIZE (128 * 1024)
#define ROM_CHIP 0x00
#define WRAM_CHIP 0x10
#define FLASH_CHIP 0x11
#define CFI_CHIP 0x13

static uint32 CHR_SIZE = 0;
static uint8 *SAVE_FLASH = NULL;
static uint8* CFI = NULL;

static uint8 sram_enabled = 0;
static uint8 sram_page = 0;		/* [1:0] */
static uint8 can_write_chr = 0;
static uint8 map_rom_on_6000 = 0;
static uint8 flags = 0;			/* [2:0] */
static uint8 mapper = 0;		/* [5:0] */
static uint8 can_write_flash = 0;
static uint8 mirroring = 0;		/* [1:0] */
static uint8 four_screen = 0;
static uint8 lockout = 0;

static uint32 prg_base = 0;		/* [26:14] */
static uint32 prg_mask = 0xF8;  /* 11111000, 128KB	 */	/* [20:14] */
static uint8 prg_mode = 0;		/* [2:0] */
static uint8 prg_bank_6000 = 0;	/* [7:0] */
static uint8 prg_bank_a = 0;	/* [7:0] */
static uint8 prg_bank_b = 1;	/* [7:0] */
static uint8 prg_bank_c = ~1;	/* [7:0] */
static uint8 prg_bank_d = ~0;	/* [7:0] */

static uint32 chr_mask = 0;		/* [18:13] */
static uint8 chr_mode = 0;		/* [2:0] */
static uint16 chr_bank_a = 0;	/* [8:0] */
static uint16 chr_bank_b = 1;	/* [8:0] */
static uint16 chr_bank_c = 2;	/* [8:0] */
static uint16 chr_bank_d = 3;	/* [8:0] */
static uint16 chr_bank_e = 4;	/* [8:0] */
static uint16 chr_bank_f = 5;	/* [8:0] */
static uint16 chr_bank_g = 6;	/* [8:0] */
static uint16 chr_bank_h = 7;	/* [8:0] */

static uint8 TKSMIR[8];

static uint32 prg_bank_6000_mapped = 0;
static uint32 prg_bank_a_mapped = 0;
static uint32 prg_bank_b_mapped = 0;
static uint32 prg_bank_c_mapped = 0;
static uint32 prg_bank_d_mapped = 0;

/* for MMC2/MMC4 */
static uint8 ppu_latch0 = 0;
static uint8 ppu_latch1 = 0;
/* for MMC1 */
static uint64 lreset = 0;
static uint8 mmc1_load_register = 0;	/* [5:0] */
/* for MMC3 */
static uint8 mmc3_internal = 0;			/* [2:0] */
/* for mapper #69 */
static uint8 mapper69_internal = 0;		/* [3:0] */
/* for mapper #112 */
static uint8 mapper112_internal = 0;	/* [2:0] */
/* for mapper #163 */
static uint8 mapper_163_latch = 0;
static uint8 mapper163_r0 = 0;			/* [7:0] */
static uint8 mapper163_r1 = 0;			/* [7:0] */
static uint8 mapper163_r2 = 0;			/* [7:0] */
static uint8 mapper163_r3 = 0;			/* [7:0] */
static uint8 mapper163_r4 = 0;			/* [7:0] */
static uint8 mapper163_r5 = 0;			/* [7:0] */

/* For mapper #90 */
static uint8 mul1 = 0;
static uint8 mul2 = 0;

/* for MMC3 scanline-based interrupts, counts A12 rises after long A12 falls */
static uint8 mmc3_irq_enabled = 0;				/* register to enable/disable counter */
static uint8 mmc3_irq_latch = 0;				/* [7:0], stores counter reload latch value */
static uint8 mmc3_irq_counter = 0;				/* [7:0], counter itself (downcounting) */
static uint8 mmc3_irq_reload = 0;				/* flag to reload counter from latch */
/* for MMC5 scanline-based interrupts, counts dummy PPU reads */
static uint8 mmc5_irq_enabled = 0;				/* register to enable/disable counter */
static uint8 mmc5_irq_line = 0;					/* [7:0], scanline on which IRQ will be triggered */
static uint8 mmc5_irq_out = 0;					/* stores 1 when IRQ is triggered */
/* for mapper #18 */
static uint16 mapper18_irq_value = 0;			/* [15:0], counter itself (downcounting) */
static uint8 mapper18_irq_control = 0;			/* [3:0], IRQ settings */
static uint16 mapper18_irq_latch = 0;			/* [15:0], stores counter reload latch value */
/* for mapper #65 */
static uint8 mapper65_irq_enabled = 0;			/* register to enable/disable IRQ */
static uint16 mapper65_irq_value = 0;			/* [15:0], counter itself (downcounting) */
static uint16 mapper65_irq_latch = 0;			/* [15:0], stores counter reload latch value  */
/* reg mapper65_irq_out = 0; */
/* for Sunsoft FME-7 */
static uint8 mapper69_irq_enabled = 0;			/* register to enable/disable IRQ */
static uint8 mapper69_counter_enabled = 0;		/* register to enable/disable counter */
static uint16 mapper69_irq_value = 0;			/* counter itself (downcounting) */
/* for VRC4 CPU-based interrupts */
static uint8 vrc4_irq_value = 0;				/* [7:0], counter itself (upcounting) */
static uint8 vrc4_irq_control = 0;				/* [2:0]� IRQ settings */
static uint8 vrc4_irq_latch = 0;				/* [7:0], stores counter reload latch value */
static uint8 vrc4_irq_prescaler = 0;			/* [6:0], prescaler counter for VRC4 */
static uint8 vrc4_irq_prescaler_counter = 0;	/* prescaler cicles counter for VRC4 */
/* for VRC3 CPU-based interrupts */
static uint16 vrc3_irq_value = 0;				/* [15:0], counter itself (upcounting) */
static uint8 vrc3_irq_control = 0;				/* [3:0], IRQ settings */
static uint16 vrc3_irq_latch = 0;				/* [15:0], stores counter reload latch value */
/* for mapper #42 (only Baby Mario) */
static uint8 mapper42_irq_enabled = 0;			/* register to enable/disable counter */
static uint16 mapper42_irq_value = 0;			/* [14:0], counter itself (upcounting) */
/* for mapper #83 */
static uint8 mapper83_irq_enabled_latch = 0;
static uint8 mapper83_irq_enabled = 0;
static uint16 mapper83_irq_counter = 0;
/* for mapper #90 */
static uint8 mapper90_xor = 0;
/* for mapper #67 */
static uint8 mapper67_irq_enabled = 0;
static uint8 mapper67_irq_latch = 0;
static uint16 mapper67_irq_counter = 0;

static uint8 flash_state = 0;
static uint16 flash_buffer_a[10];
static uint8 flash_buffer_v[10];
static uint8 cfi_mode = 0;

static uint8 show_error_log = 0;
static uint8 vrc24_compatibility = 0;

/* Micron 4-gbit memory CFI data */
static const uint8 cfi_data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x51, 0x52, 0x59, 0x02, 0x00, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x27, 0x36, 0x00, 0x00, 0x06,
    0x06, 0x09, 0x13, 0x03, 0x05, 0x03, 0x02, 0x1E,
    0x02, 0x00, 0x06, 0x00, 0x01, 0xFF, 0x03, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
    0x50, 0x52, 0x49, 0x31, 0x33, 0x14, 0x02, 0x01,
    0x00, 0x08, 0x00, 0x00, 0x02, 0xB5, 0xC5, 0x05,
    0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static void COOLGIRL_Sync_PRG(void) {
	uint8 REG_A_CHIP, REG_B_CHIP, REG_C_CHIP, REG_D_CHIP;

	prg_bank_6000_mapped = (prg_base << 1) | (prg_bank_6000 & ((~(prg_mask << 1) & 0xFE) | 1));
	prg_bank_a_mapped = (prg_base << 1) | (prg_bank_a & ((~(prg_mask << 1) & 0xFE) | 1));
	prg_bank_b_mapped = (prg_base << 1) | (prg_bank_b & ((~(prg_mask << 1) & 0xFE) | 1));
	prg_bank_c_mapped = (prg_base << 1) | (prg_bank_c & ((~(prg_mask << 1) & 0xFE) | 1));
	prg_bank_d_mapped = (prg_base << 1) | (prg_bank_d & ((~(prg_mask << 1) & 0xFE) | 1));
	REG_A_CHIP = (SAVE_FLASH != NULL && prg_bank_a_mapped >= 0x20000 - SAVE_FLASH_SIZE / 1024 / 8) ? FLASH_CHIP : ROM_CHIP;
	REG_B_CHIP = (SAVE_FLASH != NULL && prg_bank_b_mapped >= 0x20000 - SAVE_FLASH_SIZE / 1024 / 8) ? FLASH_CHIP : ROM_CHIP;
	REG_C_CHIP = (SAVE_FLASH != NULL && prg_bank_c_mapped >= 0x20000 - SAVE_FLASH_SIZE / 1024 / 8) ? FLASH_CHIP : ROM_CHIP;
	REG_D_CHIP = (SAVE_FLASH != NULL && prg_bank_d_mapped >= 0x20000 - SAVE_FLASH_SIZE / 1024 / 8) ? FLASH_CHIP : ROM_CHIP;

	if (!cfi_mode || !SAVE_FLASH) {
		switch (prg_mode & 7) {
		default:
		case 0:
			setprg16r(REG_A_CHIP, 0x8000, prg_bank_a_mapped >> 1);
			setprg16r(REG_C_CHIP, 0xC000, prg_bank_c_mapped >> 1);
			break;
		case 1:
			setprg16r(REG_C_CHIP, 0x8000, prg_bank_c_mapped >> 1);
			setprg16r(REG_A_CHIP, 0xC000, prg_bank_a_mapped >> 1);
			break;
		case 4:
			setprg8r(REG_A_CHIP, 0x8000, prg_bank_a_mapped);
			setprg8r(REG_B_CHIP, 0xA000, prg_bank_b_mapped);
			setprg8r(REG_C_CHIP, 0xC000, prg_bank_c_mapped);
			setprg8r(REG_D_CHIP, 0xE000, prg_bank_d_mapped);
			break;
		case 5:
			setprg8r(REG_C_CHIP, 0x8000, prg_bank_c_mapped);
			setprg8r(REG_B_CHIP, 0xA000, prg_bank_b_mapped);
			setprg8r(REG_A_CHIP, 0xC000, prg_bank_a_mapped);
			setprg8r(REG_D_CHIP, 0xE000, prg_bank_d_mapped);
			break;
		case 6:
			setprg32r(REG_A_CHIP, 0x8000, prg_bank_b_mapped >> 2);
			break;
		case 7:
			setprg32r(REG_A_CHIP, 0x8000, prg_bank_a_mapped >> 2);
			break;
		}
	} else {
		setprg32r(CFI_CHIP, 0x8000, 0);
	}

	if (!map_rom_on_6000 && WRAM) {
		setprg8r(WRAM_CHIP, 0x6000, sram_page); /* Select SRAM page */
    } else if (map_rom_on_6000) {
		setprg8(0x6000, prg_bank_6000_mapped); /* Map ROM on $6000-$7FFF */
    }
}

static void COOLGIRL_Sync_CHR(void) {
	/* calculate CHR shift */
	/* wire shift_chr_right = ENABLE_MAPPER_021_022_023_025 && ENABLE_MAPPER_022 && (mapper == 6'b011000) && flags[1]; */
	int chr_shift_right = ((mapper == 24) && (flags & 2)) ? 1 : 0;
	int chr_shift_left = 0;

	/* enable or disable writes to CHR RAM, setup CHR mask */
	SetupCartCHRMapping(0, UNIFchrrama, ((((~chr_mask & 0x3F) + 1) * 0x2000 - 1) & (CHR_SIZE - 1)) + 1, can_write_chr);

	switch (chr_mode & 7) {
	default:
	case 0:
		setchr8(chr_bank_a >> 3 >> chr_shift_right << chr_shift_left);
		break;
	case 1:
		setchr4(0x0000, mapper_163_latch >> chr_shift_right << chr_shift_left);
		setchr4(0x1000, mapper_163_latch >> chr_shift_right << chr_shift_left);
		break;
	case 2:
		setchr2(0x0000, chr_bank_a >> 1 >> chr_shift_right << chr_shift_left);
		TKSMIR[0] = TKSMIR[1] = chr_bank_a;
		setchr2(0x0800, chr_bank_c >> 1 >> chr_shift_right << chr_shift_left);
		TKSMIR[2] = TKSMIR[3] = chr_bank_c;
		setchr1(0x1000, chr_bank_e >> chr_shift_right << chr_shift_left);
		TKSMIR[4] = chr_bank_e;
		setchr1(0x1400, chr_bank_f >> chr_shift_right << chr_shift_left);
		TKSMIR[5] = chr_bank_f;
		setchr1(0x1800, chr_bank_g >> chr_shift_right << chr_shift_left);
		TKSMIR[6] = chr_bank_g;
		setchr1(0x1C00, chr_bank_h >> chr_shift_right << chr_shift_left);
		TKSMIR[7] = chr_bank_h;
		break;
	case 3:
		setchr1(0x0000, chr_bank_e >> chr_shift_right << chr_shift_left);
		TKSMIR[0] = chr_bank_e;
		setchr1(0x0400, chr_bank_f >> chr_shift_right << chr_shift_left);
		TKSMIR[1] = chr_bank_f;
		setchr1(0x0800, chr_bank_g >> chr_shift_right << chr_shift_left);
		TKSMIR[2] = chr_bank_g;
		setchr1(0x0C00, chr_bank_h >> chr_shift_right << chr_shift_left);
		TKSMIR[3] = chr_bank_h;
		setchr2(0x1000, chr_bank_a >> 1 >> chr_shift_right << chr_shift_left);
		TKSMIR[4] = TKSMIR[5] = chr_bank_a;
		setchr2(0x1800, chr_bank_c >> 1 >> chr_shift_right << chr_shift_left);
		TKSMIR[6] = TKSMIR[7] = chr_bank_c;
		break;
	case 4:
		setchr4(0x0000, chr_bank_a >> 2 >> chr_shift_right << chr_shift_left);
		setchr4(0x1000, chr_bank_e >> 2 >> chr_shift_right << chr_shift_left);
		break;
	case 5:
		if (!ppu_latch0) {
			setchr4(0x0000, chr_bank_a >> 2 >> chr_shift_right << chr_shift_left);
        } else {
			setchr4(0x0000, chr_bank_b >> 2 >> chr_shift_right << chr_shift_left);
        }
		if (!ppu_latch1) {
			setchr4(0x1000, chr_bank_e >> 2 >> chr_shift_right << chr_shift_left);
        } else {
			setchr4(0x1000, chr_bank_f >> 2 >> chr_shift_right << chr_shift_left);
        }
		break;
	case 6:
		setchr2(0x0000, chr_bank_a >> 1 >> chr_shift_right << chr_shift_left);
		setchr2(0x0800, chr_bank_c >> 1 >> chr_shift_right << chr_shift_left);
		setchr2(0x1000, chr_bank_e >> 1 >> chr_shift_right << chr_shift_left);
		setchr2(0x1800, chr_bank_g >> 1 >> chr_shift_right << chr_shift_left);
		break;
	case 7:
		setchr1(0x0000, chr_bank_a >> chr_shift_right << chr_shift_left);
		setchr1(0x0400, chr_bank_b >> chr_shift_right << chr_shift_left);
		setchr1(0x0800, chr_bank_c >> chr_shift_right << chr_shift_left);
		setchr1(0x0C00, chr_bank_d >> chr_shift_right << chr_shift_left);
		setchr1(0x1000, chr_bank_e >> chr_shift_right << chr_shift_left);
		setchr1(0x1400, chr_bank_f >> chr_shift_right << chr_shift_left);
		setchr1(0x1800, chr_bank_g >> chr_shift_right << chr_shift_left);
		setchr1(0x1C00, chr_bank_h >> chr_shift_right << chr_shift_left);
		break;
	}
}

static void COOLGIRL_Sync_Mirroring(void) {
	if (!four_screen) {
		if (!((mapper == 20) && (flags & 1))) { /* Mapper #189? */
			switch (mirroring) {
			case 0: setmirror(MI_V); break;
			case 1: setmirror(MI_H); break;
			case 2: setmirror(MI_0); break;
			case 3: setmirror(MI_1); break;
			}
		}
	} else { /* four screen mode */
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
	if (flash_state < sizeof(flash_buffer_a) / sizeof(flash_buffer_a[0])) {
		flash_buffer_a[flash_state] = A & 0xFFF;
		flash_buffer_v[flash_state] = V;
		flash_state++;

		/* enter CFI mode */
		if ((flash_state == 1) &&
			(flash_buffer_a[0] == 0x0AAA) && (flash_buffer_v[0] == 0x98)) {
			cfi_mode = 1;
			flash_state = 0;
		}

		/* sector erase */
		if ((flash_state == 6) &&
			(flash_buffer_a[0] == 0x0AAA) && (flash_buffer_v[0] == 0xAA) &&
			(flash_buffer_a[1] == 0x0555) && (flash_buffer_v[1] == 0x55) &&
			(flash_buffer_a[2] == 0x0AAA) && (flash_buffer_v[2] == 0x80) &&
			(flash_buffer_a[3] == 0x0AAA) && (flash_buffer_v[3] == 0xAA) &&
			(flash_buffer_a[4] == 0x0555) && (flash_buffer_v[4] == 0x55) &&
			(flash_buffer_v[5] == 0x30)) {
			int sector = prg_bank_a_mapped * 0x2000 / FLASH_SECTOR_SIZE;
			uint32 sector_address = sector * FLASH_SECTOR_SIZE;
			uint32 i;
			for (i = sector_address; i < sector_address + FLASH_SECTOR_SIZE; i++)
				SAVE_FLASH[i % SAVE_FLASH_SIZE] = 0xFF;
			FCEU_printf("Flash sector #%d is erased: 0x%08x - 0x%08x.\n", sector, sector_address, sector_address + FLASH_SECTOR_SIZE - 1);
			flash_state = 0;
		}

		/* write byte */
		if ((flash_state == 4) &&
			(flash_buffer_a[0] == 0x0AAA) && (flash_buffer_v[0] == 0xAA) &&
			(flash_buffer_a[1] == 0x0555) && (flash_buffer_v[1] == 0x55) &&
			(flash_buffer_a[2] == 0x0AAA) && (flash_buffer_v[2] == 0xA0)) {
			/*int sector = prg_bank_a_mapped * 0x2000 / FLASH_SECTOR_SIZE; */
			uint32 flash_addr = prg_bank_a_mapped * 0x2000 + (A % 0x8000);
			if (SAVE_FLASH[flash_addr % SAVE_FLASH_SIZE] != 0xFF) {
				if (!(show_error_log & 2)) {
					FCEU_PrintError("Error: can't write to 0x%08x, flash sector is not erased.\n", flash_addr);
					show_error_log |= 2; /* show error log only on reset or power-on */
				}
			} else {
				SAVE_FLASH[flash_addr % SAVE_FLASH_SIZE] = V;
			}
			flash_state = 0;
		}
	}

	/* not a command */
	if (((A & 0xFFF) != 0x0AAA) && ((A & 0xFFF) != 0x0555)) {
		flash_state = 0;
	}

	/* reset */
	if (V == 0xF0) {
		flash_state = 0;
		cfi_mode = 0;
	}

	COOLGIRL_Sync_PRG();
}

static DECLFW(COOLGIRL_WRITE) {
	if (sram_enabled && A >= 0x6000 && A < 0x8000 && !map_rom_on_6000) {
		CartBW(A, V); /* SRAM is enabled and writable */
    }
	if (SAVE_FLASH && can_write_flash && A >= 0x8000) {
        /* writing flash */
		COOLGIRL_Flash_Write(A, V);
    }

	/* block two writes in a row */
	if ((timestampbase + timestamp) < (lreset + 2)) {
        return;
    }
	lreset = timestampbase + timestamp;

	if (A >= 0x5000 && A < 0x6000 && !lockout) {
		switch (A & 7) {
		case 0:
			/* use bits 29-27 to simulate flash memory */
			prg_base = (prg_base & 0xFF) | (V << 8);
			break;
		case 1:
			prg_base = (prg_base & 0xFF00) | V;
			break;
		case 2:
			chr_mask = (chr_mask & 0x1F) | ((V & 0x80) >> 2);
			prg_mask = V & 0x7F;
			break;
		case 3:
			prg_mode = V >> 5;
			chr_bank_a = (chr_bank_a & 7) | (V << 3);
			break;
		case 4:
			chr_mode = V >> 5;
			chr_mask = (chr_mask & 0x20) | (V & 0x1F);
			break;
		case 5:
			chr_bank_a = (chr_bank_a & 0xFF) | ((V & 0x80) << 1);
			prg_bank_a = (prg_bank_a & 0xC1) | ((V & 0x7C) >> 1);
			sram_page = V & 3;
			break;
		case 6:
			flags = V >> 5;
			mapper = (mapper & 0x20) | (V & 0x1F);
			if (lockout) {
				FCEU_printf("Mapper: %3d/%02X\n", mapper, flags);
			}
			break;
		case 7:
			lockout = V >> 7;
			mapper = (mapper & 0x1F) | ((V & 0x40) >> 1);
			four_screen = (V & 0x20) >> 5;
			mirroring = (V & 0x18) >> 3;
			can_write_flash = (V & 4) >> 2;
			can_write_chr = (V & 2) >> 1;
			sram_enabled = V & 1;
			switch (mapper) {
			case 14:
				prg_bank_b = 1;
				break;
			case 17:
				prg_bank_b = ~2;
				break;
			case 23:
				map_rom_on_6000 = 1;
				break;
			}
			if (lockout) {
				FCEU_printf("Mapper: %3d/%02X\n", mapper, flags);
			}
			break;
		}
	}

	/* $0000-$7FFF */
    if (A < 0x8000) {
		/* Mapper #163 */
		if (mapper == 6) {
			if (A == 0x5101) {
				if (mapper163_r4 && !V) {
					mapper163_r5 ^= 1;
                }
				mapper163_r4 = V;
			} else if (A == 0x5100 && V == 6) {
				prg_mode = prg_mode & 0xFE;
				prg_bank_b = 12;
			} else {
				if ((A & 0x7000) == 0x5000) {
					switch ((A & 0x300) >> 8) {
					case 2:
						prg_mode |= 1;
						prg_bank_a = (prg_bank_a & 0x3F) | ((V & 3) << 6);
						mapper163_r0 = V;
						break;
					case 0:
						prg_mode |= 1;
						prg_bank_a = (prg_bank_a & 0xC3) | ((V & 0x0F) << 2);
						chr_mode = (chr_mode & 0xFE) | (V >> 7);
						mapper163_r1 = V;
						break;
					case 3:
						mapper163_r2 = V;
						break;
					case 1:
						mapper163_r3 = V;
						break;
					}
				}
			}
		}

		/* Mapper #87 */
		if (mapper == 12) {
			/* $6000-$7FFF */
			if ((A & 0x6000) == 0x6000) {
				chr_bank_a = (chr_bank_a & 0xE7) | ((V & 1) << 4) | ((V & 2) << 2);
			}
		}

		/* Mapper #90 - JY */
		/*
		if (mapper == 13)
		{
			switch (A)
			{
			case 0x5800: mul1 = V; break;
			case 0x5801: mul2 = V; break;
			}
		}
		*/

		/* MMC5 (not really) */
		if (mapper == 15) {
			switch (A) {
			case 0x5105:
				if (V == 0xFF) {
					four_screen = 1;
				} else {
					four_screen = 0;
					switch (((V >> 2) & 1) | ((V >> 3) & 2)) {
					case 0:
						mirroring = 2;
                        break;
					case 1:
						mirroring = 0;
                        break;
					case 2:
						mirroring = 1;
                        break;
					case 3:
						mirroring = 3;
                        break;
					}
				}
				break;
			case 0x5115:
				prg_bank_a = V & 0x1E;
				prg_bank_b = (V & 0x1E) | 1;
				break;
			case 0x5116:
				prg_bank_c = V & 0x1F;
				break;
			case 0x5117:
				prg_bank_d = V & 0x1F;
				break;
			case 0x5120:
				chr_bank_a = V;
				break;
			case 0x5121:
				chr_bank_b = V;
				break;
			case 0x5122:
				chr_bank_c = V;
				break;
			case 0x5123:
				chr_bank_d = V;
				break;
			case 0x5128:
				chr_bank_e = V;
				break;
			case 0x5129:
				chr_bank_f = V;
				break;
			case 0x512A:
				chr_bank_g = V;
				break;
			case 0x512B:
				chr_bank_h = V;
				break;
			case 0x5203:
				X6502_IRQEnd(FCEU_IQEXT);
				mmc5_irq_out = 0;
				mmc5_irq_line = V;
				break;
			case 0x5204:
				X6502_IRQEnd(FCEU_IQEXT);
				mmc5_irq_out = 0;
				mmc5_irq_enabled = V >> 7;
				break;
			}
		}

		/* Mapper #189 */
		if ((mapper == 20) && (flags & 2)) {
			/* $4120-$7FFF */
            if (A >= 0x4120) {
				prg_bank_a = (prg_bank_a & 0xC3) |
				    ((V & 0x0F) << 2) | ((V & 0xF0) >> 2);
			}
		}

		/* Mappers #79 and #146 - NINA-03/06 and Sachen 3015: (flag0 = 1) */
		if (mapper == 27) {
			if ((A & 0x6100) == 0x4100) {
				chr_bank_a = (chr_bank_a & 0xC7) | ((V & 7) << 3);
				prg_bank_a = (chr_bank_a & 0xF8) | ((V & 8) >> 1);
			}
		}

		/* Mapper #133 */
		if (mapper == 28) {
			if ((A & 0x6100) == 0x4100) {
				chr_bank_a = (chr_bank_a & 0xE7) | ((V & 3) << 3);
				prg_bank_a = (chr_bank_a & 0xF8) | (V & 4);
			}
		}

		/* Mapper #184 */
		if (mapper == 31)
		{
			if ((A & 0x6000) == 0x6000) {
				chr_bank_a = (chr_bank_a & 0xE3) | ((V & 7) << 2);
				chr_bank_e = (chr_bank_e & 0xE3) | ((V & 0x30) >> 2) | 0x10;
			}
		}

		/* Mapper #38 */
		if (mapper == 32) {
			if ((A & 0x7000) == 0x7000) {
				prg_bank_a = (prg_bank_a & 0xF7) | ((V & 3) << 2);
				chr_bank_a = (chr_bank_a & 0xE7) | ((V & 0x0C) << 1);
			}
		}
	} else { /* $8000-$FFFF */
		/* Mapper #2 - UxROM */
		/* flag0 - mapper #71 - for Fire Hawk only. */
		/* other mapper-#71 games are UxROM */
		if (mapper == 1) {
			if (!(flags & 1) || ((A & 0x7000) != 0x1000)) {
				/* UxROM_BITSIZE = 4 */
				prg_bank_a = (prg_bank_a & 0xC1) | ((V & 0x1F) << 1);
				if (flags & 2) {
					/* One screen mirroring select, CHR RAM bank, PRG ROM bank */
					mirroring = 2 | (V >> 7);
					chr_bank_a = (chr_bank_a & 0xFC) | ((V & 0x60) >> 5);
				}
			} else {
				/* CodeMasters, blah. Mirroring control used only by Fire Hawk */
				mirroring = 2 | ((V >> 4) & 1);
			}
		}

		/* Mapper #3 - CNROM */
		if (mapper == 2) {
			chr_bank_a = (chr_bank_a & 7) | ((V & 0x1F) << 3);
		}

		/* Mapper #78 - Holy Diver */
		if (mapper == 3) {
			prg_bank_a = (prg_bank_a & 0x1F) | ((V & 7) << 1);
			chr_bank_a = (chr_bank_a & 0x87) | ((V & 0xF0) >> 1);
			mirroring = ((V >> 3) & 1) ^ 1;
		}

		/* Mapper #97 - Irem's TAM-S1 */
		if (mapper == 4) {
			prg_bank_a = (prg_bank_a & 0xC1) | ((V & 0x1F) << 1);
			mirroring = ((V >> 7) & 1) ^ 1;
		}

		/* Mapper #93 - Sunsoft-2 */
		if (mapper == 5) {
			prg_bank_a = (prg_bank_a & 0xF1) | ((V & 0x70) >> 3);
			can_write_chr = V & 1;
		}

		/* Mapper #18 */
		if (mapper == 7) {
			switch (((A & 0x7000) >> 10) | (A & 3)) {
			case 0: /* $8000 */
				prg_bank_a = (prg_bank_a & 0xF0) | (V & 0x0F);
                break;
			case 1: /* $8001 */
				prg_bank_a = (prg_bank_a & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 2: /* $8002 */
				prg_bank_b = (prg_bank_b & 0xF0) | (V & 0x0F);
                break;
			case 3: /* $8003 */
				prg_bank_b = (prg_bank_b & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 4: /* $9000 */
				prg_bank_c = (prg_bank_c & 0xF0) | (V & 0x0F);
                break;
			case 5: /* $9001 */
				prg_bank_c = (prg_bank_c & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 6:
				break;
			case 7:
				break;
			case 8: /* $A000 */
				chr_bank_a = (chr_bank_a & 0xF0) | (V & 0x0F);
                break;
			case 9: /* $A001 */
				chr_bank_a = (chr_bank_a & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 10: /* $A002 */
				chr_bank_b = (chr_bank_b & 0xF0) | (V & 0x0F);
                break;
			case 11: /* $A003 */
				chr_bank_b = (chr_bank_b & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 12: /* $B000 */
				chr_bank_c = (chr_bank_c & 0xF0) | (V & 0x0F);
                break;
			case 13: /* $B001 */
				chr_bank_c = (chr_bank_c & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 14: /* $B002 */
				chr_bank_d = (chr_bank_d & 0xF0) | (V & 0x0F);
                break;
			case 15: /* $B003 */
				chr_bank_d = (chr_bank_d & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 16: /* $C000 */
				chr_bank_e = (chr_bank_e & 0xF0) | (V & 0x0F);
                break;
			case 17: /* $C001 */
				chr_bank_e = (chr_bank_e & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 18: /* $C002 */
				chr_bank_f = (chr_bank_f & 0xF0) | (V & 0x0F);
                break;
			case 19: /* $C003 */
				chr_bank_f = (chr_bank_f & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 20: /* $D000 */
				chr_bank_g = (chr_bank_g & 0xF0) | (V & 0x0F);
                break;
			case 21: /* $D001 */
				chr_bank_g = (chr_bank_g & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 22: /* $D002 */
				chr_bank_h = (chr_bank_h & 0xF0) | (V & 0x0F);
                break;
			case 23: /* $D003 */
				chr_bank_h = (chr_bank_h & 0x0F) | ((V & 0x0F) << 4);
                break;
			case 24: /* $E000 */
				mapper18_irq_latch = (mapper18_irq_latch & 0xFFF0) | (V & 0x0F);
                break;
			case 25: /* $E001 */
				mapper18_irq_latch = (mapper18_irq_latch & 0xFF0F) | ((V & 0x0F) << 4);
                break;
			case 26: /* $E002 */
				mapper18_irq_latch = (mapper18_irq_latch & 0xF0FF) | ((V & 0x0F) << 8);
                break;
			case 27: /* $E003 */
				mapper18_irq_latch = (mapper18_irq_latch & 0x0FFF) | ((V & 0x0F) << 12);
                break;
			case 28: /* $F000 */
				X6502_IRQEnd(FCEU_IQEXT);
				mapper18_irq_value = mapper18_irq_latch;
                break;
			case 29: /* $F001 */
				X6502_IRQEnd(FCEU_IQEXT);
				mapper18_irq_control = V & 0x0F;
                break;
			case 30:
				switch (A & 3) {
				case 0: mirroring = 1; break; /* Horz */
				case 1: mirroring = 0; break; /* Vert */
				case 2: mirroring = 2; break; /* 1SsA */
				case 3: mirroring = 3; break; /* 1SsB */
				}
			case 31:
				break; /* sound */
			}
		}

		/* Mapper #7 - AxROM, mapper #241 - BNROM */
		if (mapper == 8) {
			prg_bank_a = (prg_bank_a & 0xC3) | ((V & 0x0F) << 2);
			if (!(flags & 1)) {
				mirroring = (1 << 1) | ((V >> 4) & 1);
            }
		}

		/* Mapper #228 - Cheetahmen II */
		if (mapper == 9) {
			prg_bank_a = (prg_bank_a & 0xC3) | ((A & 0x0780) >> 5);
			chr_bank_a = (chr_bank_a & 0x07) | ((A & 0x0007) << 5) | ((V & 0x03) << 3);
			mirroring = (A >> 13) & 0x01;
		}

		/* Mapper #11 - ColorDreams */
		if (mapper == 10) {
			prg_bank_a = (prg_bank_a & 0xF3) | ((V & 0x03) << 2);
			chr_bank_a = (chr_bank_a & 0x87) | ((V & 0xF0) >> 1);
		}

		/* Mapper #66 - GxROM */
		if (mapper == 11) {
			prg_bank_a = (prg_bank_a & 0xF3) | ((V & 0x30) >> 2);
			chr_bank_a = (chr_bank_a & 0xE7) | ((V & 0x03) << 3);
		}

		/* Mapper #90 - JY */
		if (mapper == 13) {
            switch (A & 0xF000) {
            case 0x8000:
                switch (A & 3) {
				case 0: prg_bank_a = (prg_bank_a & 0xC0) | (V & 0x3F); break;
				case 1: prg_bank_b = (prg_bank_b & 0xC0) | (V & 0x3F); break;
				case 2: prg_bank_c = (prg_bank_c & 0xC0) | (V & 0x3F); break;
				case 3: prg_bank_d = (prg_bank_d & 0xC0) | (V & 0x3F); break;
				}
                break;
            case 0x9000:
                switch (A & 7) {
				case 0: chr_bank_a = V; break; /* $9000 */
				case 1: chr_bank_b = V; break; /* $9001 */
				case 2: chr_bank_c = V; break; /* $9002 */
				case 3: chr_bank_d = V; break; /* $9003 */
				case 4: chr_bank_e = V; break; /* $9004 */
				case 5: chr_bank_f = V; break; /* $9005 */
				case 6: chr_bank_g = V; break; /* $9006 */
				case 7: chr_bank_h = V; break; /* $9007 */
				}
                break;
            case 0xC000:
                /* use MMC3's IRQs */
                switch (A & 7) {
				case 0:
					if (V & 1) {
						mmc3_irq_enabled = 1;
					} else {
						X6502_IRQEnd(FCEU_IQEXT);
						mmc3_irq_enabled = 0;
					}
					break;
				case 1:
					break; /* who cares about this shit? */
				case 2:
					mmc3_irq_enabled = 0;
					X6502_IRQEnd(FCEU_IQEXT);
					break;
				case 3:
					mmc3_irq_enabled = 1;
					break;
				case 4:
					break; /* prescaler? who cares? */
				case 5:
					mmc3_irq_latch = V ^ mapper90_xor;
					mmc3_irq_reload = 1;
					break;
				case 6:
					mapper90_xor = V;
					break;
				case 7:
					break; /* meh */
				}
                break;
            case 0xD000:
                if ((A & 3) == 1) {
				    mirroring = V & 3;
				}
                break;
            }
		}

		/* Mapper #65 - Irem's H3001 */
		if (mapper == 14) {
			switch (((A & 0x7000) >> 9) | (A & 7)) {
			case 0: /* $8000 */
				prg_bank_a = (prg_bank_a & 0xC0) | (V & 0x3F);
				break;
			case 9: /* $9001, mirroring */
				mirroring = (V >> 7) & 0x01;
				break;
			case 11: /* $9003, enable IRQ */
				X6502_IRQEnd(FCEU_IQEXT);
				mapper65_irq_enabled = V >> 7;
				break;
			case 12: /* $9004 */
				X6502_IRQEnd(FCEU_IQEXT); /* mapper65_irq_out = 0; */ /* ack */
				mapper65_irq_value = mapper65_irq_latch; /* $9004, IRQ reload */
				break;
			case 13: /* $9005, IRQ high V */
				mapper65_irq_latch = (mapper65_irq_latch & 0x00FF) | (V << 8);
                break;
			case 14: /* $9006, IRQ low V */
				mapper65_irq_latch = (mapper65_irq_latch & 0xFF00) | V;
				break;
			case 16: /* $A000 */
				prg_bank_b = (prg_bank_b & 0xC0) | (V & 0x3F);
				break;
			case 24: /* $B000 */
				chr_bank_a = V;
				break;
			case 25: /* $B001 */
				chr_bank_b = V;
				break;
			case 26: /* $B002 */
				chr_bank_c = V;
				break;
			case 27: /* $B003 */
				chr_bank_d = V;
				break;
			case 28: /* $B004 */
				chr_bank_e = V;
				break;
			case 29: /* $B005 */
				chr_bank_f = V;
				break;
			case 30:/* $B006 */
				chr_bank_g = V;
				break;
			case 31: /* $B007 */
				chr_bank_h = V;
				break;
			case 32: /* $C000 */
				prg_bank_c = (prg_bank_c & 0xC0) | (V & 0x3F);
				break;
			}
		}

		/* Mapper #1 - MMC1 */
		/*
		r0 - load register
		flag0 - 16KB of SRAM (SOROM)
		*/
		if (mapper == 16) {
			if (V & 0x80) {
                /* reset */
				mmc1_load_register = (mmc1_load_register & 0xC0) | 0x20;
				prg_mode = 0;
				prg_bank_c = (prg_bank_c & 0xE0) | 0x1E;
			} else {
				mmc1_load_register = (mmc1_load_register & 0xC0) | ((V & 0x01) << 5) |
				    ((mmc1_load_register & 0x3E) >> 1);
				if (mmc1_load_register & 1) {
					switch ((A >> 13) & 3) {
					case 0: /* $8000-$9FFF */
						if ((mmc1_load_register & 0x18) == 0x18) {
							prg_mode = 0;
							prg_bank_c = (prg_bank_c & 0xE1) | 0x1E;
						} else if ((mmc1_load_register & 0x18) == 0x10)	{
							prg_mode = 1;
							prg_bank_c = (prg_bank_c & 0xE1);
						} else {
							prg_mode = 7;
                        }
						if (mmc1_load_register & 0x20) {
							chr_mode = 4;
						} else {
							chr_mode = 0;
						}
						mirroring = ((mmc1_load_register >> 1) & 0x03) ^ 2;
						break;
					case 1:
						chr_bank_a = (chr_bank_a & 0x83) | ((mmc1_load_register & 0x3E) << 1);
						if (flags & 1) {
                            /* (flags[0]) - 16KB of SRAM */
							/* PRG RAM page #2 is battery backed */
							sram_page = 2 | (((mmc1_load_register >> 4) & 1) ^ 1);
						}
						prg_bank_a = (prg_bank_a & 0xDF) | (mmc1_load_register & 0x20); /* for SUROM, 512k PRG support */
						prg_bank_c = (prg_bank_c & 0xDF) | (mmc1_load_register & 0x20); /* for SUROM, 512k PRG support */
						break;
					case 2: /* $C000-$DFFF */
						chr_bank_e = (chr_bank_e & 0x83) | ((mmc1_load_register & 0x3E) << 1);
						break;
					case 3:
						prg_bank_a = (prg_bank_a & 0xE1) | (mmc1_load_register & 0x1E);
						sram_enabled = ((mmc1_load_register >> 5) & 1) ^ 1;
						break;
					}
					mmc1_load_register = 0x20;
				}
			}
		}

		/* Mapper #9 and #10 - MMC2 and MMC4 */
		/* flag0 - 0=MMC2, 1=MMC4 */
		if (mapper == 17) {
			switch ((A >> 12) & 7) {
			case 2:  /* $A000-$AFFF */
				if (!(flags & 1)) {
					/* MMC2 */
					prg_bank_a = (prg_bank_a & 0xF0) | (V & 0x0F);
				} else {
					/* MMC4 */
					prg_bank_a = (prg_bank_a & 0xE1) | ((V & 0x0F) << 1);
				}
				break;
			case 3: /* $B000-$BFFF */
				chr_bank_a = (chr_bank_a & 0x83) | ((V & 0x1F) << 2);
				break;
			case 4: /* $C000-$CFFF */
				chr_bank_b = (chr_bank_b & 0x83) | ((V & 0x1F) << 2);
				break;
			case 5: /* $D000-$DFFF */
				chr_bank_e = (chr_bank_e & 0x83) | ((V & 0x1F) << 2);
				break;
			case 6: /* $E000-$EFFF */
				chr_bank_f = (chr_bank_f & 0x83) | ((V & 0x1F) << 2);
				break;
			case 7: /* $F000-$FFFF */
				mirroring = V & 1;
				break;
			}
		}

		/* Mapper #152 */
		if (mapper == 18) {
			chr_bank_a = (chr_bank_a & 0x87) | ((V & 0x0F) << 3);
			prg_bank_a = (prg_bank_a & 0xF1) | ((V & 0x70) >> 3);
			mirroring = 2 | (V >> 7);
		}

		/* Mapper #73 - VRC3 */
		if (mapper == 19) {
			switch ((A >> 12) & 7) {
			case 0: /* $8000-$8FFF */
				vrc3_irq_latch = (vrc3_irq_latch & 0xFFF0) | (V & 0x0F);
				break;
			case 1: /* $9000-$9FFF */
				vrc3_irq_latch = (vrc3_irq_latch & 0xFF0F) | ((V & 0x0F) << 4);
				break;
			case 2: /* $A000-$AFFF */
				vrc3_irq_latch = (vrc3_irq_latch & 0xF0FF) | ((V & 0x0F) << 8);
				break;
			case 3: /* $B000-$BFFF */
				vrc3_irq_latch = (vrc3_irq_latch & 0x0FFF) | ((V & 0x0F) << 12);
				break;
			case 4: /* $C000-$CFFF */
				X6502_IRQEnd(FCEU_IQEXT); /* ack */
				vrc3_irq_control = (vrc3_irq_control & 0xF8) | (V & 0x07);
				if (vrc3_irq_control & 0x02) {
					vrc3_irq_value = vrc3_irq_latch;
				}
				break;
			case 5: /* $D000-$DFFF */
				X6502_IRQEnd(FCEU_IQEXT); /* ack */
				vrc3_irq_control = (vrc3_irq_control & 0xFD) | (vrc3_irq_control & 0x01) << 1;
				break;
			case 6: /* $E000-$EFFF */
				break;
			case 7: /* $F000-$FFFF */
				prg_bank_a = (prg_bank_a & 0xF1) | ((V & 0x07) << 1);
				break;
			}
		}

		/* Mapper #4 - MMC3/MMC6 */
		/*
		flag0 - TxSROM
		flag1 - mapper #189
		*/
		if (mapper == 20) {
			switch (((A & 0x6000) >> 12) | (A & 1)) {
			case 0: /* $8000-$9FFE, even */
				mmc3_internal = (mmc3_internal & 0xF8) | (V & 0x07);
				if (!(flags & 2) && !(flags & 4)) {
					if (V & 0x40) {
						prg_mode = 5;
					} else {
						prg_mode = 4;
					}
				}
				if (!(flags & 4)) {
					if (V & 0x80) {
						chr_mode = 3;
					} else {
						chr_mode = 2;
					}
				}
				break;
			case 1: /* $8001-$9FFF, odd */
				switch (mmc3_internal & 7) {
				case 0:
					chr_bank_a = V;
					break;
				case 1:
					chr_bank_c = V;
					break;
				case 2:
					chr_bank_e = V;
					break;
				case 3:
					chr_bank_f = V;
					break;
				case 4:
					chr_bank_g = V;
					break;
				case 5:
					chr_bank_h = V;
					break;
				case 6:
					if (!(flags & 2)) {
						prg_bank_a = V;
					}
					break;
				case 7:
					if (!(flags & 2)) {
						prg_bank_b = V;
					}
					break;
				}
				break;
			case 2: /* $A000-$BFFE, even (mirroring) */
				if (!(flags & 4)) {
					mirroring = V & 1;
				}
				break;
			case 3: /* RAM protect... no */
				break;
			case 4: /* $C000-$DFFE, even (IRQ latch) */
				mmc3_irq_latch = V;
				break;
			case 5: /* $C001-$DFFF, odd */
				mmc3_irq_reload = 1;
				break;
			case 6: /* $E000-$FFFE, even */
				X6502_IRQEnd(FCEU_IQEXT);
				mmc3_irq_enabled = 0;
				break;
			case 7: /* $E001-$FFFF, odd */
				if (!(flags & 4)) {
					mmc3_irq_enabled = 1;
				}
				break;
			}
		}

		/* Mapper #112 */
		if (mapper == 21) {
			switch ((A >> 13) & 3) {
			case 0: /* $8000-$9FFF */
				mapper112_internal = (mapper112_internal & 0xF8) | (V & 0x07);
				break;
			case 1: /* $A000-BFFF */
				switch (mapper112_internal & 7)	{
				case 0:
					prg_bank_a = (prg_bank_a & 0xC0) | (V & 0x3F);
					break;
				case 1:
					prg_bank_b = (prg_bank_b & 0xC0) | (V & 0x3F);
					break;
				case 2:
					chr_bank_a = V;
					break;
				case 3:
					chr_bank_c = V;
					break;
				case 4:
					chr_bank_e = V;
					break;
				case 5:
					chr_bank_f = V;
					break;
				case 6:
					chr_bank_g = V;
					break;
				case 7:
					chr_bank_h = V;
					break;
				}
				break;
			case 2: /* $C000-$DFFF  */
				break;
			case 3: /* $E000-$FFFF */
				mirroring = V & 1;
				break;
			}
		}

		/* Mappers #33 + #48 - Taito */
		/* flag0=0 - #33, flag0=1 - #48 */
		if (mapper == 22) {
			switch (((A & 0x6000) >> 11) | (A & 3)) {
			case 0: /* $8000, PRG Reg 0 (8k @ $8000) */
				prg_bank_a = (prg_bank_a & 0xC0) | (V & 0x3F);
				if (!(flags & 1)) { /* 33 */
					mirroring = (V >> 6) & 0x01;
				}
				break;
			case 1: /* $8001, PRG Reg 1 (8k @ $A000) */
				prg_bank_b = (prg_bank_b & 0xC0) | (V & 0x3F);
				break;
			case 2: /* $8002, CHR Reg 0 (2k @ $0000) */
				chr_bank_a = V << 1;
				break;
			case 3: /* $8003, CHR Reg 1 (2k @ $0800) */
				chr_bank_c = V << 1;
				break;
			case 4: /* $A000, CHR Reg 2 (1k @ $1000) */
				chr_bank_e = V;
				break;
			case 5: /* $A001, CHR Reg 2 (1k @ $1400) */
				chr_bank_f = V;
				break;
			case 6: /* $A002, CHR Reg 2 (1k @ $1800) */
				chr_bank_g = V;
				break;
			case 7: /* $A003, CHR Reg 2 (1k @ $1C00) */
				chr_bank_h = V;
				break;
			case 12: /* $E000, mirroring, for mapper #48 */
				if (flags & 1) { /* 48 */
					mirroring = (V >> 6) & 1;
				}
				break;
			case 8: /* $C000, IRQ latch */
				mmc3_irq_latch = V ^ 0xFF;
				break;
			case 9: /* $C001, IRQ reload */
				mmc3_irq_reload = 1;
				break;
			case 10: /* $C002, IRQ enable */
				mmc3_irq_enabled = 1;
				break;
			case 11: /* $C003, IRQ disable & ack */
				mmc3_irq_enabled = 0;
				X6502_IRQEnd(FCEU_IQEXT); /* ack */
				break;
			}
		}

		/* Mapper #42 */
		if (mapper == 23) {
			switch (((A & 0x4000) >> 12) | (A & 3))	{
			case 0: /* $8000, CHR Reg (8k @ $8000) */
				chr_bank_a = (chr_bank_a & 0xE0) | ((V & 0x1F) << 3);
				break;
			case 4: /* $E000, PRG Reg (8k @ $6000) */
				prg_bank_6000 = (prg_bank_6000 & 0xF0) | (V & 0x0F);
				break;
			case 5: /* Mirroring */
				mirroring = (V >> 3) & 1;
				break;
			case 6: /* IRQ */
				mapper42_irq_enabled = (V & 0x02) >> 1;
				if (!mapper42_irq_enabled) {
					X6502_IRQEnd(FCEU_IQEXT);
					mapper42_irq_value = 0;
				}
				break;
			}
		}

		/* Mapper #23 - VRC2/4 */
		/*
		flag0 - switches A0 and A1 lines. 0=A0,A1 like VRC2b (mapper #23), 1=A1,A0 like VRC2a(#22), VRC2c(#25)
		flag1 - divides CHR bank select by two (mapper #22, VRC2a)
		*/
		if (mapper == 24) {
			uint8 vrc_2b_hi = 0;
			uint8 vrc_2b_low = 0;
			uint8 vrc_2b_addr = 0;

			if (vrc24_compatibility) {
				/* Compatibility code - for rom variants using the older firmware */
				vrc_2b_hi = ((A >> 1) & 1) | ((A >> 3) & 1) | ((A >> 5) & 1) | ((A >> 7) & 1);
				vrc_2b_low = (A & 1) | ((A >> 2) & 1) | ((A >> 4) & 1) | ((A >> 6) & 1);
				vrc_2b_addr =
			        (((flags & 1) ? vrc_2b_low : vrc_2b_hi) << 1) |
			        ((flags & 1) ? vrc_2b_hi : vrc_2b_low);
			} else {
				/* Updated code, not compatible with earlier VRC24 cart variants */
				vrc_2b_hi =
				    (flags & 5) == 0 ?
				    (((A >> 7) & 1) | ((A >> 2) & 1)) /* mapper #21 */
				    : (flags & 5) == 1 ?
				    (A & 1) /* mapper #22 */
				    : (flags & 5) == 4 ?
				    (((A >> 5) & 1) | ((A >> 3) & 1) | ((A >> 1) & 1)) /* mapper #23 */
				    : (((A >> 2) & 1) | (A & 1)); /* mapper #25 */
				vrc_2b_low =
				    (flags & 5) == 0 ?
				    (((A >> 6) & 1) | ((A >> 1) & 1)) /* mapper #21 */
				    : (flags & 5) == 1 ?
				    ((A >> 1) & 1) /* mapper #22 */
				    : (flags & 5) == 4 ?
				    (((A >> 4) & 1) | ((A >> 2) & 1) | (A & 1)) /* mapper #23 */
				    : (((A >> 3) & 1) | ((A >> 1) & 1)); /* mapper #25 */
				vrc_2b_addr = (vrc_2b_hi << 1) | vrc_2b_low;
			}

			switch (((A >> 10) & 0x1C) | vrc_2b_addr) {
			case 0: /* $8000-$8003, PRG0 */
			case 1:
			case 2:
			case 3:
				prg_bank_a = (prg_bank_a & 0xE0) | (V & 0x1F);
				break;
			case 4: /* $9000-$9001, mirroring */
			case 5:
				/* VRC2 - using games are usually well - behaved and only write 0 or 1 to this register, */
				/* but Wai Wai World in one instance writes $FF instead */
				if (V != 0xFF) {
					mirroring = V & 3;
				}
				break;
			case 6: /* $9002-$9004, PRG swap */
			case 7:
				prg_mode = (prg_mode & 0xFE) | ((V >> 1) & 1);
				break;
			case 8:	/* $A000-$A003, PRG1 */
			case 9:
			case 10:
			case 11:
				prg_bank_b = (prg_bank_b & 0xE0) | (V & 0x1F);
				break;
			case 12: /* $B000, CHR0 low */
				chr_bank_a = (chr_bank_a & 0xF0) | (V & 0x0F);
				break;
			case 13: /* $B001, CHR0 hi */
				chr_bank_a = (chr_bank_a & 0x0F) | ((V & 0x0F) << 4);
				break;
			case 14: /* $B002, CHR1 low */
				chr_bank_b = (chr_bank_b & 0xF0) | (V & 0x0F);
				break;
			case 15: /* $B003, CHR1 hi */
				chr_bank_b = (chr_bank_b & 0x0F) | ((V & 0x0F) << 4);
				break;
			case 16: /* $C000, CHR2 low */
				chr_bank_c = (chr_bank_c & 0xF0) | (V & 0x0F);
				break;
			case 17: /* $C001, CHR2 hi */
				chr_bank_c = (chr_bank_c & 0x0F) | ((V & 0x0F) << 4);
				break;
			case 18: /* $C002, CHR3 low */
				chr_bank_d = (chr_bank_d & 0xF0) | (V & 0x0F);
				break;
			case 19: /* $C003, CHR3 hi */
				chr_bank_d = (chr_bank_d & 0x0F) | ((V & 0x0F) << 4);
				break;
			case 20: /* $D000, CHR4 low */
				chr_bank_e = (chr_bank_e & 0xF0) | (V & 0x0F);
				break;
			case 21: /* $D001, CHR4 hi */
				chr_bank_e = (chr_bank_e & 0x0F) | ((V & 0x0F) << 4);
				break;
			case 22: /* $D002, CHR5 low */
				chr_bank_f = (chr_bank_f & 0xF0) | (V & 0x0F);
				break;
			case 23: /* $D003, CHR5 hi */
				chr_bank_f = (chr_bank_f & 0x0F) | ((V & 0x0F) << 4);
				break;
			case 24: /* $E000, CHR6 low */
				chr_bank_g = (chr_bank_g & 0xF0) | (V & 0x0F);
				break;
			case 25: /* $E001, CHR6 hi */
				chr_bank_g = (chr_bank_g & 0x0F) | ((V & 0x0F) << 4);
				break;
			case 26: /* $E002, CHR7 low */
				chr_bank_h = (chr_bank_h & 0xF0) | (V & 0x0F);
				break;
			case 27: /* $E003, CHR7 hi */
				chr_bank_h = (chr_bank_h & 0x0F) | ((V & 0x0F) << 4);
				break;
			}
			if ((A & 0x7000) == 0x7000) {
				switch (vrc_2b_addr)	{
				case 0: /* IRQ latch low */
					vrc4_irq_latch = (vrc4_irq_latch & 0xF0) | (V & 0x0F);
					break;
				case 1: /* IRQ latch hi */
					vrc4_irq_latch = (vrc4_irq_latch & 0x0F) | ((V & 0x0F) << 4);
					break;
				case 2: /* IRQ control */
					X6502_IRQEnd(FCEU_IQEXT); /* ack */
					vrc4_irq_control = (vrc4_irq_control & 0xF8) | (V & 0x07); /* mode, enabled, enabled after ack */
					if (vrc4_irq_control & 2) { /* if E is set */
						vrc4_irq_prescaler_counter = 0; /* reset prescaler */
						vrc4_irq_prescaler = 0;
						vrc4_irq_value = vrc4_irq_latch; /* reload with latch */
					}
					break;
				case 3: /* IRQ ack */
					X6502_IRQEnd(FCEU_IQEXT);
					vrc4_irq_control = (vrc4_irq_control & 0xFD) | (vrc4_irq_control & 0x01) << 1;
					break;
				}
			}
		}

		/* Mapper #69 - Sunsoft FME-7 */
		if (mapper == 25) {
			if (((A >> 13) & 3) == 0) {
				mapper69_internal = (mapper69_internal & 0xF0) | (V & 0x0F);
			}
			if (((A >> 13) & 3) == 1) {
				switch (mapper69_internal & 0x0F)
				{
				case 0: /* CHR0 */
					chr_bank_a = V;
					break;
				case 1: /* CHR1 */
					chr_bank_b = V;
					break;
				case 2: /* CHR2 */
					chr_bank_c = V;
					break;
				case 3: /* CHR3 */
					chr_bank_d = V;
					break;
				case 4: /* CHR4 */
					chr_bank_e = V;
					break;
				case 5: /* CHR5 */
					chr_bank_f = V;
					break;
				case 6: /* CHR6 */
					chr_bank_g = V;
					break;
				case 7: /* CHR7 */
					chr_bank_h = V;
					break;
				case 8: /* PRG0 */
					sram_enabled = (V >> 7) & 1;
					map_rom_on_6000 = ((V >> 6) & 1) ^ 1;
					prg_bank_6000 = V & 0x3F;
					break;
				case 9: /* PRG1 */
					prg_bank_a = (prg_bank_a & 0xC0) | (V & 0x3F);
					break;
				case 10: /* PRG2 */
					prg_bank_b = (prg_bank_b & 0xC0) | (V & 0x3F);
					break;
				case 11: /* PRG3 */
					prg_bank_c = (prg_bank_c & 0xC0) | (V & 0x3F);
					break;
				case 12: /* mirroring */
					mirroring = V & 3;
					break;
				case 13:
					X6502_IRQEnd(FCEU_IQEXT); /* ack */
					mapper69_counter_enabled = V >> 7;
					mapper69_irq_enabled = V & 1;
					break;
				case 14: /* IRQ low */
					mapper69_irq_value = (mapper69_irq_value & 0xFF00) | V;
					break;
				case 15:  /* IRQ high */
					mapper69_irq_value = (mapper69_irq_value & 0x00FF) | (V << 8);
					break;
				}
			}
		}

		/* Mapper #32 - Irem's G-101 */
		if (mapper == 26) {
			switch ((A & 0x7000) >> 12) {
			case 0: /* $8000-$8FFF, PRG0 */
				prg_bank_a = (prg_bank_a & 0xC0) | (V & 0x3F);
				break;
			case 1: /* $9000-$9FFF, PRG mode, mirroring */
				prg_mode = (prg_mode & 0x06) | ((V >> 1) & 0x01);
				mirroring = V & 1;
				break;
			case 2: /* $A000-$AFFF, PRG1 */
				prg_bank_b = (prg_bank_b & 0xC0) | (V & 0x3F);
				break;
			case 3: /* $B000-$BFFF, CHR regs */
				switch (A & 7) {
				case 0:
					chr_bank_a = V;
					break;
				case 1:
					chr_bank_b = V;
					break;
				case 2:
					chr_bank_c = V;
					break;
				case 3:
					chr_bank_d = V;
					break;
				case 4:
					chr_bank_e = V;
					break;
				case 5:
					chr_bank_f = V;
					break;
				case 6:
					chr_bank_g = V;
					break;
				case 7:
					chr_bank_h = V;
					break;
				}
				break;
			}
		}

		/* Mapper #36 is assigned to TXC's PCB 01-22000-400 */
		if (mapper == 29) {
			if ((A & 0x7FFE) == 0x7FFE) {
				prg_bank_a = (prg_bank_a & 0xC3) | ((V & 0xF0) >> 2);
				chr_bank_a = (chr_bank_a & 0x87) | ((V & 0x0F) << 3);
			}
		}

		/* Mapper #70 */
		if (mapper == 30) {
			prg_bank_a = (prg_bank_a & 0xE1) | ((V & 0xF0) >> 3);
			chr_bank_a = (chr_bank_a & 0x87) | ((V & 0x0F) << 3);
		}

		/* Mapper #75 - VRC1 */
		if (mapper == 34) {
			switch ((A >> 12) & 7) {
			case 0: /* $8000-$8FFF */
				prg_bank_a = (prg_bank_a & 0xF0) | (V & 0x0F);
				break;
			case 1: /* $9000-$9FFF */
				mirroring = V & 1;
				chr_bank_a = (chr_bank_a & 0xBF) | ((V & 2) << 5);
				chr_bank_e = (chr_bank_a & 0xBF) | ((V & 4) << 4);
				break;
			case 2: /* $A000-$AFFF */
				prg_bank_b = (prg_bank_b & 0xF0) | (V & 0x0F);
				break;
			case 4: /* $C000-$CFFF */
				prg_bank_c = (prg_bank_c & 0xF0) | (V & 0x0F);
				break;
			case 6: /* $E000-$EFFF */
				chr_bank_a = (chr_bank_a & 0xC3) | ((V & 0x0F) << 2);
				break;
			case 7: /* $F000-$FFFF */
				chr_bank_e = (chr_bank_e & 0xC3) | ((V & 0x0F) << 2);
				break;
			}
		}

		/* Mapper #83 - Cony/Yoko */
		if (mapper == 35) {
			switch ((A >> 8) & 3) {
			case 0: /* $80xx */
				prg_bank_a = (prg_bank_a & 0xE1) | ((V & 0x0F) << 1);
				break;
			case 1: /* $81xx */
				mirroring = V & 3;
				prg_mode = (prg_mode & 3) | ((V >> 2) & 4);
				map_rom_on_6000 = (V & 0x20) >> 5;
				mapper83_irq_enabled_latch = (V & 0x80) >> 7;
				break;
			case 2: /* 82xx */
				if (!(A & 1)) {
					X6502_IRQEnd(FCEU_IQEXT);
					mapper83_irq_counter = (mapper83_irq_counter & 0xFF00) | V;
				} else {
					mapper83_irq_enabled = mapper83_irq_enabled_latch;
					mapper83_irq_counter = (mapper83_irq_counter & 0x00FF) | (V << 8);
				}
				break;
			case 3:
				if (!(A & 0x10)) {
					switch (A & 3) {
					case 0:
						prg_bank_a = V;
						break;
					case 1:
						prg_bank_b = V;
						break;
					case 2:
						prg_bank_b = V;
						break;
					case 3:
						prg_bank_6000 = V;
						break;
					}
				} else {
					if (!(flags & 4)) {
						switch (A & 7) {
						case 0:
							chr_bank_a = V;
							break;
						case 1:
							chr_bank_b = V;
							break;
						case 2:
							chr_bank_c = V;
							break;
						case 3:
							chr_bank_d = V;
							break;
						case 4:
							chr_bank_e = V;
							break;
						case 5:
							chr_bank_f = V;
							break;
						case 6:
							chr_bank_g = V;
							break;
						case 7:
							chr_bank_h = V;
							break;
						}
					} else {
						switch (A & 7) {
						/* TODO: verify CHR mask */
						case 0:
							chr_bank_a = (chr_bank_a & 1) | (V << 1);
							break;
						case 1:
							chr_bank_c = (chr_bank_c & 1) | (V << 1);
							break;
						case 6:
							chr_bank_e = (chr_bank_e & 1) | (V << 1);
							break;
						case 7:
							chr_bank_g = (chr_bank_g & 1) | (V << 1);
							break;
						}
					}
				}
				break;
			}
		}

		/* Mapper #67 - Sunsoft-3 */
		if (mapper == 36) {
			if (A & 0x800) {
				switch ((A >> 12) & 7) {
				case 0: /* $8800 */
					chr_bank_a = (chr_bank_a & 0x81) | ((V & 0x3F) << 1);
					break;
				case 1: /* $9800 */
					chr_bank_c = (chr_bank_c & 0x81) | ((V & 0x3F) << 1);
					break;
				case 2: /* $A800 */
					chr_bank_e = (chr_bank_e & 0x81) | ((V & 0x3F) << 1);
					break;
				case 3: /* $B800 */
					chr_bank_g = (chr_bank_g & 0x81) | ((V & 0x3F) << 1);
					break;
				case 4: /* $C800, IRQ load */
					mapper67_irq_latch = ~mapper67_irq_latch;
					if (mapper67_irq_latch) {
						mapper67_irq_counter = (mapper67_irq_counter & 0x00FF) | (V << 8);
					} else {
						mapper67_irq_counter = (mapper67_irq_counter & 0xFF00) | V;
					}
					break;
				case 5: /* $D800, IRQ enable */
					mapper67_irq_latch = 0;
					mapper67_irq_enabled = (V & 0x10) >> 4;
					break;
				case 6: /* $E800 */
					mirroring = V & 3;
					break;
				case 7: /* $F800 */
					prg_bank_a = (prg_bank_a & 0xE1) | ((V & 0x0F) << 1);
					break;
				}
			} else {
				/* Interrupt Acknowledge ($8000) */
				X6502_IRQEnd(FCEU_IQEXT);
			}
		}

		/* Mapper #89 - Sunsoft-2 chip on the Sunsoft-3 board */
		if (mapper == 37) {
			prg_bank_a = (prg_bank_a & 0xF1) | ((V & 0x70) >> 3);
			chr_bank_a = (chr_bank_a & 0x87) | ((V & 0x80) >> 1) | ((V & 7) << 3);
			mirroring = 2 | ((V & 8) >> 3);
		}
	}

	COOLGIRL_Sync();
}

static DECLFR(MAFRAM) {
	if ((mapper == 0) && (A >= 0x5000) && (A < 0x6000))
		return 0;

	/* Mapper #163 */
	if (mapper == 6) {
		if ((A & 0x7700) == 0x5100) {
			return mapper163_r2 | mapper163_r0 | mapper163_r1 | ~mapper163_r3;
		}
		if ((A & 0x7700) == 0x5500) {
			return (mapper163_r5 & 1) ? mapper163_r2 : mapper163_r1;
		}
	}

	/* Mapper #90 - JY */
	if (mapper == 13) {
		if ((A == 0x5800)) {
			return (mul1 * mul2) & 0xFF;
		}
		if ((A == 0x5801)) {
			return ((mul1 * mul2) >> 8) & 0xFF;
		}
	}

	/* MMC5 */
	if (mapper == 15) {
		if (A == 0x5204) {
			uint8 ppuon = (PPU[1] & 0x18);
			uint8 ret = (mmc5_irq_out << 7) | (!ppuon || ((scanline + 1) >= 241) ? 0 : 0x40);
			X6502_IRQEnd(FCEU_IQEXT);
			mmc5_irq_out = 0;
			return ret;
		}
	}

	/* Mapper #36 is assigned to TXC's PCB 01-22000-400 */
	if ((mapper == 29) && ((A & 0xE100) == 0x4100))	{
		return (prg_bank_a & 0x0C) << 2;
	}

	/* Mapper #83 - Cony/Yoko */
	if ((mapper == 35) && ((A & 0x7000) == 0x5000)) {
		return flags & 3;
	}

	if (sram_enabled && !map_rom_on_6000 && (A >= 0x6000) && (A < 0x8000)) {
		return CartBR(A); /* SRAM */
	}
	if (map_rom_on_6000 && (A >= 0x6000) && (A < 0x8000)) {
		return CartBR(A); /* PRG */
	}

	return cpu.openbus; /* Open bus */
}

static void COOLGIRL_ScanlineCounter(void) {
	/* for MMC3 and MMC3-based */
	if (mmc3_irq_reload || !mmc3_irq_counter) {
		mmc3_irq_counter = mmc3_irq_latch;
		mmc3_irq_reload = 0;
	} else {
		mmc3_irq_counter--;
	}
	if (!mmc3_irq_counter && mmc3_irq_enabled) {
		X6502_IRQBegin(FCEU_IQEXT);
	}

	/* for MMC5 */
	if (mmc5_irq_line == scanline + 1) {
		if (mmc5_irq_enabled) {
			X6502_IRQBegin(FCEU_IQEXT);
			mmc5_irq_out = 1;
		}
	}

	/* for mapper #163 */
	if (scanline == 239) {
		mapper_163_latch = 0;
		COOLGIRL_Sync_CHR();
	} else if (scanline == 127) {
		mapper_163_latch = 1;
		COOLGIRL_Sync_CHR();
	}
}

static void COOLGIRL_CpuCounter(int a) {
	while (a--)
	{
		/* Mapper #23 - VRC4 */
		if (vrc4_irq_control & 2) {
			/* Cycle mode without prescaler is not used by any games? It's missed in fceux source code. */
			if (vrc4_irq_control & 4) { /* cycle mode */
				if (!(show_error_log & 1)) {
					show_error_log |= 1; /* show error log only on reset or power-on */
					FCEU_printf("Cycle IRQ mode is not supported, please report to Cluster\n");
				}

				vrc4_irq_value++; /* just count IRQ value */
				if (vrc4_irq_value == 0) { /* if (carry) */
					X6502_IRQBegin(FCEU_IQEXT);
					vrc4_irq_value = vrc4_irq_latch;
				}
			} else {
				vrc4_irq_prescaler++; /* count prescaler */
				if ((!(vrc4_irq_prescaler_counter & 2) && vrc4_irq_prescaler == 114) ||
				    ((vrc4_irq_prescaler_counter & 2) && vrc4_irq_prescaler == 113)) {
					vrc4_irq_prescaler = 0;
					vrc4_irq_prescaler_counter++;
					if (vrc4_irq_prescaler_counter == 3) {
						vrc4_irq_prescaler_counter = 0;
					}
					vrc4_irq_value++;
					if (vrc4_irq_value == 0) { /* if (carry) */
						X6502_IRQBegin(FCEU_IQEXT);
						vrc4_irq_value = vrc4_irq_latch;
					}
				}
			}
		}

		/* Mapper #73 - VRC3 */
		if (vrc3_irq_control & 2) {
			if (vrc3_irq_control & 4) { /* 8-bit mode */
				vrc3_irq_value = (vrc3_irq_value & 0xFF00) | ((vrc3_irq_value + 1) & 0xFF);
				if ((vrc3_irq_value & 0xFF) == 0) {
					X6502_IRQBegin(FCEU_IQEXT);
					vrc3_irq_value = (vrc3_irq_value & 0xFF00) | (vrc3_irq_latch & 0xFF);
				}
			} else { /* 16-bit */
				vrc3_irq_value += 1;
				if (vrc3_irq_value == 0) {
					X6502_IRQBegin(FCEU_IQEXT);
					vrc3_irq_value = vrc3_irq_latch;
				}
			}
		}

		/* Mapper #69 - Sunsoft FME-7 */
		if (mapper69_counter_enabled) {
			mapper69_irq_value--;
			if (mapper69_irq_value == 0xFFFF) {
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}

		/* Mapper #18 */
		if (mapper18_irq_control & 1) {
			uint8 carry;
			carry = (mapper18_irq_value & 0x0F) - 1;
			mapper18_irq_value = (mapper18_irq_value & 0xFFF0) | (carry & 0x0F);
			carry = (carry >> 4) & 1;
			if (!(mapper18_irq_control & 8)) {
				carry = ((mapper18_irq_value >> 4) & 0x0F) - carry;
				mapper18_irq_value = (mapper18_irq_value & 0xFF0F) | ((carry & 0x0F) << 4);
				carry = (carry >> 4) & 1;
			}
			if (!(mapper18_irq_control & 0x0C)) {
				carry = ((mapper18_irq_value >> 8) & 0x0F) - carry;
				mapper18_irq_value = (mapper18_irq_value & 0xF0FF) | ((carry & 0x0F) << 8);
				carry = (carry >> 4) & 1;
			}
			if (!(mapper18_irq_control & 0x0E)) {
				carry = ((mapper18_irq_value >> 12) & 0x0F) - carry;
				mapper18_irq_value = (mapper18_irq_value & 0x0FFF) | ((carry & 0x0F) << 12);
				carry = (carry >> 4) & 1;
			}
			if (carry) {
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}

		/* Mapper #65 - Irem's H3001 */
		if (mapper65_irq_enabled) {
			if (mapper65_irq_value != 0) {
				mapper65_irq_value--;
				if (!mapper65_irq_value) {
					X6502_IRQBegin(FCEU_IQEXT);
				}
			}
		}

		/* Mapper #42 */
		if (mapper42_irq_enabled) {
			mapper42_irq_value++;
			if (mapper42_irq_value >> 15) {
				mapper42_irq_value = 0;
			}
			if (((mapper42_irq_value >> 13) & 3) == 3) {
				X6502_IRQBegin(FCEU_IQEXT);
			} else {
				X6502_IRQEnd(FCEU_IQEXT);
			}
		}

		/* Mapper #83 - Cony/Yoko */
		if (mapper83_irq_enabled) {
			if (mapper83_irq_counter == 0) {
				X6502_IRQBegin(FCEU_IQEXT);
			}
			mapper83_irq_counter--;
		}

		/* Mapper #67 - Sunsoft-3 */
		if (mapper67_irq_enabled) {
			mapper67_irq_counter--;
			if (mapper67_irq_counter == 0xFFFF) {
				X6502_IRQBegin(FCEU_IQEXT); /* fire IRQ */
				mapper67_irq_enabled = 0; /* disable IRQ */
			}
		}
	}
}

static void COOLGIRL_PPUHook(uint32 A) {
	/* For TxROM */
	if ((mapper == 20) && (flags & 1)) {
		setmirror(MI_0 + (TKSMIR[(A & 0x1FFF) >> 10] >> 7));
	}

	/* Mapper #9 and #10 - MMC2 and MMC4 */
	if (mapper == 17) {
		if ((A >> 4) == 0xFD) {
			ppu_latch0 = 0;
			COOLGIRL_Sync_CHR();
		}
		if ((A >> 4) == 0xFE) {
			ppu_latch0 = 1;
			COOLGIRL_Sync_CHR();
		}
		if ((A >> 4) == 0x1FD) {
			ppu_latch1 = 0;
			COOLGIRL_Sync_CHR();
		}
		if ((A >> 4) == 0x1FE) {
			ppu_latch1 = 1;
			COOLGIRL_Sync_CHR();
		}
	}
}

static void COOLGIRL_Reset(void) {
	sram_enabled = 0;
	sram_page = 0;
	can_write_chr = 0;
	map_rom_on_6000 = 0;
	flags = 0;
	mapper = 0;
	can_write_flash = 0;
	mirroring = 0;
	four_screen = 0;
	lockout = 0;
	prg_base = 0;
	prg_mask = 0xF8;
	prg_mode = 0;
	prg_bank_6000 = 0;
	prg_bank_a = 0;
	prg_bank_b = 1;
	prg_bank_c = ~1;
	prg_bank_d = ~0;
	chr_mask = 0;
	chr_mode = 0;
	chr_bank_a = 0;
	chr_bank_b = 1;
	chr_bank_c = 2;
	chr_bank_d = 3;
	chr_bank_e = 4;
	chr_bank_f = 5;
	chr_bank_g = 6;
	chr_bank_h = 7;
	ppu_latch0 = 0;
	ppu_latch1 = 0;
	lreset = 0;
	mmc1_load_register = 0;
	mmc3_internal = 0;
	mapper69_internal = 0;
	mapper112_internal = 0;
	mapper_163_latch = 0;
	mapper163_r0 = 0;
	mapper163_r1 = 0;
	mapper163_r2 = 0;
	mapper163_r3 = 0;
	mapper163_r4 = 0;
	mapper163_r5 = 0;
	mul1 = 0;
	mul2 = 0;
	mmc3_irq_enabled = 0;
	mmc3_irq_latch = 0;
	mmc3_irq_counter = 0;
	mmc3_irq_reload = 0;
	mmc5_irq_enabled = 0;
	mmc5_irq_line = 0;
	mmc5_irq_out = 0;
	mapper18_irq_value = 0;
	mapper18_irq_control = 0;
	mapper18_irq_latch = 0;
	mapper65_irq_enabled = 0;
	mapper65_irq_value = 0;
	mapper65_irq_latch = 0;
	mapper69_irq_enabled = 0;
	mapper69_counter_enabled = 0;
	mapper69_irq_value = 0;
	vrc4_irq_value = 0;
	vrc4_irq_control = 0;
	vrc4_irq_latch = 0;
	vrc4_irq_prescaler = 0;
	vrc4_irq_prescaler_counter = 0;
	vrc3_irq_value = 0;
	vrc3_irq_control = 0;
	vrc3_irq_latch = 0;
	mapper42_irq_enabled = 0;
	mapper42_irq_value = 0;
	mapper90_xor = 0;
	flash_state = 0;
	cfi_mode = 0;
	show_error_log = 0;
	COOLGIRL_Sync();
}

static void COOLGIRL_Power(void) {
	FCEU_CheatAddRAM(32, 0x6000, WRAM);
	SetReadHandler(0x4020, 0x7FFF, MAFRAM);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0xFFFF, COOLGIRL_WRITE);
	COOLGIRL_Reset();
}

static void COOLGIRL_Close(void) {
	if (SAVE_FLASH)
		FCEU_gfree(SAVE_FLASH);
	if (CFI)
		FCEU_gfree(CFI);
	SAVE_FLASH = CFI = NULL;
}

static void COOLGIRL_Restore(int version) {
	COOLGIRL_Sync();
	lreset = 0;
}

#define ExState(var, varname) AddExState(&var, sizeof(var), 0, varname)

void COOLGIRL_Init(CartInfo *info) {
	int i;

	info->Power = COOLGIRL_Power;
	info->Reset = COOLGIRL_Reset;
	info->Close = COOLGIRL_Close;
	GameStateRestore = COOLGIRL_Restore;
	GameHBIRQHook = COOLGIRL_ScanlineCounter;
	MapIRQHook = COOLGIRL_CpuCounter;
	PPU_hook = COOLGIRL_PPUHook;

	CHR_SIZE = info->CHRRamSize;

	WRAMSIZE = (info->PRGRamSize + info->PRGRamSaveSize) ? (info->PRGRamSize + info->PRGRamSaveSize) : (32 * 1024);
	if (WRAMSIZE > 0) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		memset(WRAM, 0, WRAMSIZE);
		SetupCartPRGMapping(WRAM_CHIP, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		if (info->battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
	}

	if (info->battery) {
		SAVE_FLASH = (uint8 *)FCEU_gmalloc(SAVE_FLASH_SIZE);
		SetupCartPRGMapping(FLASH_CHIP, SAVE_FLASH, SAVE_FLASH_SIZE, 1);
		info->SaveGame[1] = SAVE_FLASH;
		info->SaveGameLen[1] = SAVE_FLASH_SIZE;
	}

	CFI = (uint8 *)FCEU_gmalloc(sizeof(cfi_data) * 2);
	for (i = 0; i < (int)sizeof(cfi_data); i++) {
		CFI[i * 2] = CFI[i * 2 + 1] = cfi_data[i];
	}
	SetupCartPRGMapping(CFI_CHIP, CFI, sizeof(cfi_data) * 2, 0);

	switch (info->PRGCRC32) {
	/* Earlier version of roms using VRC24 were using incorrect flags
	 * which are now incompatible with later updates of Coolgirl firmware causing
	 * graphics to be broken. This enables compatibily code to handle early roms while
	 * keeping the latest and updated VRC24 handling intact for newer roms when they become available.
	 */
	case 0xCF0FE3F3: /* 0b89251f6d63d49586ee3bbe41914ab7.unif */
	case 0x62BEFE75: /* 320 ¿úÓ äÑ¡ñ¿.unf */
	case 0xBF617289: /* d61e011f86b13bed9a22d3c56326e689.unif */
	case 0x3A70CB07: /* (MMK-02A-01) Gradius 10-in-1.nes */
	case 0x5352A128: /* (MMK-02D-00) Coolgirl 11-in-1.nes */
	case 0x861F89A0: /* (MMK-02E-00) Coolgirl 7-in-1.nes */
	case 0xA25CD951: /* (MMK-02F-00) Shooting Game 16-in-1.nes */
	case 0xE368F1F6: /* (MMK-033-00) Game 150-in-1.nes */
	case 0x49EE3B04: /* (YG-6014) Super Captain Tsubasa 2 Hack 6-in-1.nes (not using mapper code 24 but whatever) */
	case 0x7B85868B: /* (Yhc-4006-00) Super Konami 80-in-1.nes */
	case 0x2F0D22CD: /* (Yhc-BS-8165-01) Super Plane Game 11-in-1.nes */
	case 0x242B9218: /* (Yhc-CK-124-07) Super Konami 3-in-1.nes */
		vrc24_compatibility = 1;
		break;
	default:
		vrc24_compatibility = 0;
		break;
	}

	ExState(sram_enabled, "SREN");
	ExState(sram_page, "SRPG");
	ExState(can_write_chr, "SRWR");
	ExState(map_rom_on_6000, "MR6K");
	ExState(flags, "FLGS");
	ExState(mapper, "MPPR");
	ExState(can_write_flash, "FLWR");
	ExState(mirroring, "MIRR");
	ExState(four_screen, "4SCR");
	ExState(lockout, "LOCK");
	ExState(prg_base, "PBAS");
	ExState(prg_mask, "PMSK");
	ExState(prg_mode, "PMOD");
	ExState(prg_bank_6000, "P6BN");
	ExState(prg_bank_a, "PABN");
	ExState(prg_bank_b, "PBBN");
	ExState(prg_bank_c, "PCBN");
	ExState(prg_bank_d, "PDBN");
	ExState(chr_mask, "CMSK");
	ExState(chr_mode, "CMOD");
	ExState(chr_bank_a, "CABN");
	ExState(chr_bank_b, "CBBN");
	ExState(chr_bank_c, "CCBN");
	ExState(chr_bank_d, "CDBN");
	ExState(chr_bank_e, "CEBN");
	ExState(chr_bank_f, "CFBN");
	ExState(chr_bank_g, "CGBN");
	ExState(chr_bank_h, "CHBN");
	ExState(ppu_latch0, "PPU0");
	ExState(ppu_latch1, "PPU1");
	ExState(lreset, "LRST");
	ExState(mmc1_load_register, "M01R");
	ExState(mmc3_internal, "M01I");
	ExState(mapper69_internal, "M69I");
	ExState(mapper112_internal, "112I");
	ExState(mapper_163_latch, "163L");
	ExState(mapper163_r0, "1630");
	ExState(mapper163_r1, "1631");
	ExState(mapper163_r2, "1632");
	ExState(mapper163_r3, "1633");
	ExState(mapper163_r4, "1634");
	ExState(mapper163_r5, "1635");
	ExState(mul1, "MUL1");
	ExState(mul2, "MUL2");
	ExState(mmc3_irq_enabled, "M4IE");
	ExState(mmc3_irq_latch, "M4IL");
	ExState(mmc3_irq_counter, "M4IC");
	ExState(mmc3_irq_reload, "M4IR");
	ExState(mmc5_irq_enabled, "M5IE");
	ExState(mmc5_irq_line, "M5IL");
	ExState(mmc5_irq_out, "M5IO");
	ExState(mapper18_irq_value, "18IV");
	ExState(mapper18_irq_control, "18IC");
	ExState(mapper18_irq_latch, "18IL");
	ExState(mapper65_irq_enabled, "65IE");
	ExState(mapper65_irq_value, "65IV");
	ExState(mapper65_irq_latch, "65IL");
	ExState(mapper69_irq_enabled, "69IE");
	ExState(mapper69_counter_enabled, "69CE");
	ExState(mapper69_irq_value, "69IV");
	ExState(vrc4_irq_value, "V4IV");
	ExState(vrc4_irq_control, "V4IC");
	ExState(vrc4_irq_latch, "V4IL");
	ExState(vrc4_irq_prescaler, "V4PP");
	ExState(vrc4_irq_prescaler_counter, "V4PC");
	ExState(vrc3_irq_value, "V3IV");
	ExState(vrc3_irq_control, "V3IC");
	ExState(vrc3_irq_latch, "V3IL");
	ExState(mapper42_irq_enabled, "42IE");
	ExState(mapper42_irq_value, "42IV");
	ExState(mapper83_irq_enabled_latch, "M83L");
	ExState(mapper83_irq_enabled, "M83I");
	ExState(mapper83_irq_counter, "M83C");
	ExState(mapper90_xor, "90XR");
	ExState(mapper67_irq_enabled, "67IE");
	ExState(mapper67_irq_latch, "67IL");
	ExState(mapper67_irq_counter, "67IC");
	ExState(flash_state, "FLST");
	ExState(flash_buffer_a, "FLBA");
	ExState(flash_buffer_v, "FLBV");
	ExState(cfi_mode, "CFIM");
}
