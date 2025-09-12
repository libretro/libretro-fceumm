/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022 NewRisingSun
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
#include "asic_fme7.h"
#include "asic_latch.h"
#include "asic_mmc1.h"
#include "asic_mmc2and4.h"
#include "asic_mmc3.h"
#include "asic_vrc1.h"
#include "asic_vrc2and4.h"
#include "asic_vrc3.h"
#include "asic_vrc6.h"
#include "asic_vrc7.h"
#include "cartram.h"

static uint8 submapper;
static uint8 reg[4]; /* Supervisor registers */
static uint8 Custom_reg[4]; /* Registers for custom mappers */
static uint8 eeprom[16], eep_clock, state, command, output; /* Serial EEPROM */

static void (*mapperSync)(int) = NULL;
static void applyMode (uint8);

static SFORMAT stateRegs[] = {
	{ &reg,         4, "REGS" },
	{ &Custom_reg,  4, "CURG" },
	{ eeprom,      16, "EEPR" },
	{ &eep_clock,   1, "EEP0" },
	{ &state,       1, "EEP1" },
	{ &command,     1, "EEP2" },
	{ &output,      1, "EEP3" },
	{ 0 }
};

/* Serial EEPROM */
static const uint16 lut509[512] = { /* Look-up table, used only by Legendary Games of NES 509-in-1 */
   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,
  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,   0,   1,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,
  85,  86,  87,  88,  89,  90,   4,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
 124, 125, 126, 127, 128, 129,   2,   3, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,   5, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 238, 239, 240, 241,
 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 256,   6, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281,
 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321,
 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362,
 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402,
 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442,
 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482,
 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 512, 513, 514, 515, 516, 517
};

void setPins(uint8 select, uint8 newClock, uint8 newData) { /* Serial EEPROM */
	if (select)
		state = 0;
	else
	if (!eep_clock && !!newClock) {
		if (state <8) {
			command = command <<1 | !!(newData)*1;
			if (++state == 8 && (command &0xF0) != 0x50 && (command &0xF0) != 0xA0) state = 0;
		} else {
			int mask = 1 <<(15 -state);
			int address = command &0x0F;
			if ((command &0xF0) == 0xA0) {
				eeprom[address] = eeprom[address] &~mask | !!(newData)*mask;
				/* The "write" command also silently returns the content of a lookup table */
				output = !!(lut509[eeprom[0] | eeprom[1] | eeprom[2] <<8 &0x1FF] >>(address &1? 0: 8) &mask);
			} else
			if ((command &0xF0) == 0x50)
				output = !!(eeprom[address] &mask);

			if (++state == 16) state = 0;
		}
	}
	eep_clock = newClock;
}

/* Mapper syncs */
static void sync () {
	if (mapperSync) mapperSync(reg[submapper == 1? 2: 3] <<9 &0x2000 | reg[1] <<5 &0x1FE0 | reg[0] <<4 &0x0010);
}

static void sync_AxROM (int prgOR) {
	int prgAND = reg[0] &0x20? 0x0F: reg[0] &0x02? 0x03: 0x07;
	setprg32(0x8000, Latch_data &prgAND | prgOR >>2 &~prgAND);
	setchr8(0);
	setmirror(Latch_data &0x10? MI_1: MI_0);
}

static void sync_BxROM (int prgOR) {
	int prgAND = reg[0] &0x20? 0x0F: reg[0] &0x02? 0x03: 0x07;
	setprg32(0x8000, Latch_data &prgAND | prgOR >>2 &~prgAND);
	setchr8(0);
	setmirror(reg[0] &0x04? MI_H: MI_V);
}

static void sync_FME7 (int prgOR) {
	int prgAND = reg[0] &0x02? 0x0F: 0x1F;
	FME7_syncWRAM(0);
	FME7_syncPRG(prgAND, prgOR &~prgAND);
	FME7_syncCHR(0xFF, 0x00);
	FME7_syncMirror();
}

static void sync_FxROM (int prgOR) {
	int prgAND = reg[0] &0x02? 0x07: 0x0F;
	MMC24_syncWRAM(0);
	MMC4_syncPRG(prgAND, prgOR >>1 &~prgAND);
	MMC24_syncCHR(0xFF, 0x00);
	MMC24_syncMirror();
}

static void sync_GNROM (int prgOR) {
	int prgAND = reg[0] &0x08? 0x01: 0x03;
	int value = Latch_data;
	if (submapper == 1 && ~reg[0] &0x08 || submapper != 1 && prgOR &0x2000) value = Latch_data >>4 &0x0F | Latch_data <<4 &0xF0;
	prgOR = prgOR >>2 | reg[0] >>1 &0x02;
	setprg32(0x8000, value &prgAND | prgOR &~prgAND);
	setchr8(value >>4);
	setmirror(reg[0] &0x10? MI_H: MI_V);
}

static void sync_IF12 (int prgOR) {
	int prgAND = reg[0] &0x02? 0x07: 0x0F;
	setprg16(0x8000, Custom_reg[1] &prgAND | prgOR >>1 &~prgAND);
	setprg16(0xC000,                         prgOR >>1 | prgAND);
	setchr8(Custom_reg[0] >>1 &0x0F);
	setmirror(Custom_reg[0] &0x01? MI_H: MI_V);
}

static DECLFW(IF12_writeReg) {
	Custom_reg[A >>14 &1] = V;
	sync();
}

static void sync_LF36 (int prgOR) {
	prgOR |= reg[0] &0x08;
	setprg8(0x8000,                0x04 | prgOR);
	setprg8(0xA000,                0x05 | prgOR);
	setprg8(0xC000, Custom_reg[0] &0x07 | prgOR);
	setprg8(0xE000,                0x07 | prgOR);
	setchr8(0);
	setmirror(reg[0] &0x04? MI_H: MI_V);
}

void FP_FASTAPASS(1) LF36_cpuCycle (int a) {
	while (a--) {
		if (Custom_reg[1] &1) {
			if (!++Custom_reg[2]) ++Custom_reg[3];
			if (Custom_reg[3] &0x10)
				X6502_IRQBegin(FCEU_IQEXT);
			else
				X6502_IRQEnd(FCEU_IQEXT);
		} else {
			Custom_reg[2] = Custom_reg[3] = 0;
			X6502_IRQEnd(FCEU_IQEXT);
		}
	}
}

static DECLFW(LF36_writeReg) {
	switch(A >>13 &3) {
		case 0: case 1:
			Custom_reg[1] = A >>13 &1;
			break;
		case 3:
			Custom_reg[0] = V;
			sync();
	}
}

static void sync_Misc (int prgOR) {
	if (reg[0] &0x02) {
		setprg16(0x8000, Custom_reg[2] <<1 &0x0E | reg[0] &0x01 | prgOR >>1 &~0x0F);
		setprg16(0xC000, Custom_reg[2] <<1 &0x0E | reg[0] &0x01 | prgOR >>1 &~0x0F);
		setchr8(Custom_reg[0] &0x03);
	} else {
		setprg32(0x8000, Custom_reg[2] &0x07 | prgOR >>2 &~0x07);
		setchr8(Custom_reg[0] &0x0F);
	}
	if (reg[0] &0x08)
		setmirror(reg[0] &0x04? MI_H: MI_V);
	else
		setmirror(Custom_reg[1] &0x10? MI_1: MI_0);
}

static DECLFW(Misc_writeReg) {
	switch(A >>12 &7) {
		case 0: case 2: case 3:
			Custom_reg[0] = V;
			sync();
			break;
		case 1:
			Custom_reg[reg[0] &0x08? 0: 1] = V;
			sync();
			break;
		case 6: case 7:
			Custom_reg[2] = V;
			sync();
			break;
	}
}

static void sync_Nanjing (int prgOR) {
	setprg32(0x8000, Custom_reg[2] <<4 &0x30 | Custom_reg[0] &0x0F | (Custom_reg[3] &0x04? 0x00: 0x03) | prgOR >>2);
	setchr8(0);
	setmirror(reg[0] &0x04? MI_H: MI_V);
}

static void Nanjing_scanline (void) {
	if (Custom_reg[0] &0x80 && scanline <239) {
		setchr4(0x0000, scanline >= 127? 1: 0);
		setchr4(0x1000, scanline >= 127? 1: 0);
	} else
		setchr8(0);
}

static DECLFW(Nanjing_writeReg) {
	Custom_reg[A >>8 &3] = V;
	sync();
}

static void sync_PNROM (int prgOR) {
	MMC24_syncWRAM(0);
	MMC2_syncPRG(0x0F, prgOR &~0x0F);
	MMC24_syncCHR(0xFF, 0x00);
	MMC24_syncMirror();
}

static void sync_SxROM (int prgOR) {
	int prgAND = reg[0] &0x02? (reg[0] &0x08? 0x03: 0x07): 0x0F;
	MMC1_syncWRAM(0);
	MMC1_syncPRG(prgAND, (prgOR >>1 | reg[0] &0x06) &~prgAND);
	MMC1_syncCHR(0x1F, 0x00);
	MMC1_syncMirror();
}

static void sync_SUROM (int prgOR) {
	MMC1_syncWRAM(0);
	MMC1_syncPRG(0x1F, prgOR >>1 &~0x1F);
	MMC1_syncCHR(0x0F, 0x00);
	MMC1_syncMirror();
}

static int SUROM_getPRGBank(uint8 bank) {
	return MMC1_getPRGBank(bank) | MMC1_getCHRBank(0) &0x10;
}

static void sync_TxROM (int prgOR) {
	int prgAND = reg[0] &0x08? (reg[0] &0x04? (reg[0] &0x02? (reg[2] &0x02? 0x07: 0x0F): 0x1F): 0x3F): 0x7F;
	prgOR |= reg[2] &0x01? 0x0C: 0x00;
	MMC3_syncWRAM(0);
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(reg[0] &0x10? 0xFF: 0x7F, 0x00);
	MMC3_syncMirror();
}

static void sync_TxSROM (int prgOR) {
	int prgAND = reg[0] &0x08? (reg[0] &0x04? (reg[0] &0x02? (reg[2] &0x02? 0x07: 0x0F): 0x1F): 0x3F): 0x7F;
	prgOR |= reg[2] &0x01? 0x0C: 0x00;
	MMC3_syncWRAM(0);
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(0x7F, 0x00);
	switch(MMC3_getMirroring() &3) { /* Only A000=02 is TxSROM. H/V mirroring is necessary for Ys 1, modified for MMC3. */
		case 0:
			setmirror(MI_V);
			break;
		case 1:
			setmirror(MI_H);
			break;
		case 2:
			setmirror(MMC3_getCHRBank(0) &0x80? MI_1: MI_0);
			break;
		case 3:
			setmirror(MI_1);
			break;
	}
}

static void sync_UxROM (int prgOR) {
	int prgAND = reg[0] &0x02? 0x07: submapper == 1 && ~reg[0] &0x04? 0x1F: 0x0F;
	setprg16(0x8000, Latch_data &prgAND | prgOR >>1 &~prgAND);
	setprg16(0xC000,                      prgOR >>1 | prgAND);
	setchr8(0);
	setmirror(reg[0] &0x04? MI_H: MI_V);
}

static void sync_UNROM512 (int prgOR) {
	setprg16(0x8000, Latch_data &0x1F | prgOR >>1 &~0x1F);
	setprg16(0xC000,                    prgOR >>1 | 0x1F);
	setchr8(Latch_data >>5);
	setmirror(reg[0] &0x04? MI_H: MI_V);
}

static void sync_VRC1 (int prgOR) {
	int prgAND = reg[0] &0x08? (reg[0] &0x04? (reg[0] &0x02? 0x0F: 0x1F): 0x3F): 0x7F;
	VRC1_syncPRG(prgAND, prgOR &~prgAND);
	VRC1_syncCHR(0x1F, 0x00);
	VRC1_syncMirror();
}

static void sync_VRC24 (int prgOR) {
	int prgAND = reg[0] &0x02? 0x0F: 0x1F;
	VRC24_syncWRAM(0);
	VRC24_syncPRG(prgAND, prgOR &~prgAND);
	VRC24_syncCHR(0xFF, 0x00);
	VRC24_syncMirror();
}

static void sync_VRC3 (int prgOR) {
	VRC3_syncWRAM(0);
	VRC3_syncPRG(0x07, prgOR >>1 &~0x07);
	VRC3_syncCHR(0x01, 0x00);
	setmirror(reg[0] &0x04? MI_H: MI_V);
}

static void sync_VRC6 (int prgOR) {
	int prgAND = reg[0] &0x02? 0x0F: 0x1F;
	VRC6_syncWRAM(0);
	VRC6_syncPRG(prgAND, prgOR &~prgAND);
	VRC6_syncCHR(0xFF, 0x00);
	VRC6_syncMirror();
}

static void sync_VRC7 (int prgOR) {
	int prgAND = reg[0] &0x08? (reg[0] &0x04? (reg[0] &0x02? 0x0F: 0x1F): 0x3F): 0x7F;
	VRC7_syncWRAM(0);
	VRC7_syncPRG(prgAND, prgOR &~prgAND);
	VRC7_syncCHR(reg[0] &0x10? 0xFF: 0x7F, 0x00);
	VRC7_syncMirror();
}

/* Supervisor */
static DECLFR(readReg) {
	switch(A) {
		case 0x5301: case 0x5601:
			return output? 0x80: 0x00;
		default:
			return 0xFF;
	}
}

static DECLFW(writeReg) {
	switch(A) {
		case 0x5301:
			if (submapper == 0) setPins(!!(V &0x04), !!(V &0x02), !!(V &0x01));
			break;
		case 0x5601:
			if (submapper == 1) setPins(!!(V &0x10), !!(V &0x02), !!(V &0x01));
			if (~reg[3] &0x80) {
				reg[3] = V;
				sync();
			}
			break;
		case 0x5700:
			reg[A &3] = V;
			applyMode(1);
			break;
		case 0x5701: case 0x5702:
			reg[A &3] = V;
			sync();
			break;
	}
}

static void applyMode (uint8 clear) {
	uint8 previousMirroring;
	MapIRQHook = NULL;
	PPU_hook = NULL;
	GameHBIRQHook = NULL;
	SetReadHandler(0x5000, 0x5FFF, readReg);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	SetWriteHandler(0x6000, 0xFFFF, CartBW);
	switch(submapper <<8 | reg[0] >>4) {
		case 0x000: case 0x302:
			mapperSync = sync_SxROM;
			MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
			break;
		case 0x001:
			mapperSync = sync_SUROM;
			MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, SUROM_getPRGBank, NULL, NULL, NULL);
			break;
		case 0x004: case 0x006: case 0x104: case 0x106:
			mapperSync = reg[0] &0x08? sync_BxROM: sync_AxROM;
			Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
			break;
		case 0x005: case 0x105:
			mapperSync = sync_Misc; /* NROM, CNROM, Fire Hawk */
			SetWriteHandler(0x8000, 0xFFFF, Misc_writeReg);
			if (clear) Custom_reg[0] = Custom_reg[1] = Custom_reg[2] = Custom_reg[3] = 0;
			sync();
			break;
		case 0x007:
			mapperSync = sync_LF36; /* SMB2J */
			MapIRQHook = LF36_cpuCycle;
			SetWriteHandler(0x8000, 0xFFFF, LF36_writeReg);
			if (clear) Custom_reg[0] = Custom_reg[1] = Custom_reg[2] = Custom_reg[3] = 0;
			sync();
			break;
		case 0x008:
			mapperSync = sync_FxROM;
			MMC24_activate(clear, sync);
			break;
		case 0x009: case 0x107: case 0x307:
			if (reg[0] &0x08) {
				mapperSync = sync_UxROM;
				Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
			} else {
				mapperSync = sync_IF12; /* Not Irem's actual IF-12 mapper, but something custom by BlazePro */
				SetWriteHandler(0x8000, 0xFFFF, IF12_writeReg);
				if (clear) Custom_reg[0] = Custom_reg[1] = Custom_reg[2] = Custom_reg[3] = 0;
				sync();
			}
			break;
		case 0x00A:
			mapperSync = sync_PNROM;
			MMC24_activate(clear, sync);
			break;
		case 0x00B:
			mapperSync = sync_UNROM512;
			Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
			break;
		case 0x00C: case 0x00D: case 0x10C: case 0x10D:
			mapperSync = sync_GNROM;
			Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
			break;
		case 0x00E: case 0x10E:
			mapperSync = sync_Nanjing;
			GameHBIRQHook = Nanjing_scanline;
			SetWriteHandler(0x5000, 0x53FF, Nanjing_writeReg);
			if (clear) Custom_reg[0] = Custom_reg[1] = Custom_reg[2] = Custom_reg[3] = 0;
			sync();
			break;
		case 0x100: case 0x101:
			mapperSync = sync_TxROM;
			previousMirroring = MMC3_getMirroring();
			MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, NULL);
			MMC3_writeReg(0xA000, previousMirroring);
			break;
		case 0x102:
			mapperSync = sync_TxSROM;
			MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, NULL);
			break;
		case 0x200:
			mapperSync = sync_VRC24;
			VRC2_activate(clear, sync, 0x05, 0x0A, NULL, NULL, NULL, NULL);
			break;
		case 0x201:
			mapperSync = sync_VRC24;
			VRC4_activate(clear, sync, 0x05, 0x0A, 1, NULL, NULL, NULL, NULL, NULL);
			break;
		case 0x202:
			mapperSync = sync_VRC24;
			VRC2_activate(clear, sync, 0x0A, 0x05, NULL, NULL, NULL, NULL);
			break;
		case 0x203:
			mapperSync = sync_VRC24;
			VRC4_activate(clear, sync, 0x0A, 0x05, 1, NULL, NULL, NULL, NULL, NULL);
			break;
		case 0x300: case 0x301:
			mapperSync = sync_VRC6;
			VRC6_activate(clear, sync, 0x01, 0x02, NULL, NULL, NULL, NULL);
			break;
		case 0x400:
			mapperSync = sync_VRC1;
			VRC1_activate(clear, sync);
			break;
		case 0x401:
			mapperSync = sync_VRC7;
			VRC7_activate(clear, sync, 0x18);
			break;
		case 0x404:
			mapperSync = sync_VRC3;
			VRC3_activate(clear, sync);
			break;
		case 0x500:
			mapperSync = sync_FME7;
			FME7_activate(clear, sync);
			break;
		default:
			break;
	}
}

static void power() {
	reg[0] = 0x0F;
	reg[1] = 0xFF;
	reg[2] = submapper == 1? 0x10: 0x00;
	reg[3] = 0x00;
	eep_clock = command = output = 1;
	command = state = 0;
	applyMode(1);
}


static void stateRestore(int version) {
	applyMode(0);
}

void Mapper468_Init(CartInfo *info) {
	submapper = info->submapper;
	FME7_addExState();
	Latch_addExState();
	MMC1_addExState();
	MMC24_addExState();
	MMC3_addExState();
	VRC1_addExState();
	VRC24_addExState();
	VRC3_addExState();
	VRC6_addExState();
	VRC7_addExState();
	WRAM_init(info, 8);
	info->Reset = power;
	info->Power = power;
	GameStateRestore = stateRestore;
	AddExState(stateRegs, ~0, 0, 0);
}
