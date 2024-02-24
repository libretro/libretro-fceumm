/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
 *  Copyright (C) 2011 FCEUX team
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

/* x24C0x interface */

#include "mapinc.h"
#include "eeprom_x24c0x.h"

#define X24C0X_STANDBY 0
#define X24C0X_ADDRESS 1
#define X24C0X_WORD    2
#define X24C0X_READ    3
#define X24C0X_WRITE   4

struct X24C01 {
	uint8 *data;
	uint8 state;
	uint8 addr;
	uint8 word;
	uint8 latch;
	uint8 bitcount;
	uint8 sda;
	uint8 scl;
	uint8 out;
} X24C01;

struct X24C02 {
	uint8 *data;
	uint8 state;
	uint8 addr;
	uint8 word;
	uint8 latch;
	uint8 bitcount;
	uint8 sda;
	uint8 scl;
	uint8 out;
} X24C02;

static struct X24C01 x24c01 = { 0 };
static struct X24C02 x24c02 = { 0 };

SFORMAT x24c01_StateRegs[] = {
	{ &x24c01.addr, 1, "ADDR" },
	{ &x24c01.word, 1, "WORD" },
	{ &x24c01.latch, 1, "LATC" },
	{ &x24c01.bitcount, 1, "BITC" },
	{ &x24c01.sda, 1, "SDA" },
	{ &x24c01.scl, 1, "SCL" },
	{ &x24c01.out, 1, "OUT" },
	{ &x24c01.state, 1, "STAT" },
	{ 0 }
};

SFORMAT x24c02_StateRegs[] = {
	{ &x24c02.addr, 1, "ADDR" },
	{ &x24c02.word, 1, "WORD" },
	{ &x24c02.latch, 1, "LATC" },
	{ &x24c02.bitcount, 1, "BITC" },
	{ &x24c02.sda, 1, "SDA" },
	{ &x24c02.scl, 1, "SCL" },
	{ &x24c02.out, 1, "OUT" },
	{ &x24c02.state, 1, "STAT" },
	{ 0 },
};

void x24c01_init(uint8 *data) {
	x24c01.data = data;
	x24c01.addr = 0;
	x24c01.word = 0;
	x24c01.latch = 0;
	x24c01.bitcount = 0;
	x24c01.sda = 0;
	x24c01.scl = 0;
	x24c01.state = X24C0X_STANDBY;
}

void x24c02_init(uint8 *data) {
	x24c02.data = data;
	x24c02.addr = 0;
	x24c02.word = 0;
	x24c02.latch = 0;
	x24c02.bitcount = 0;
	x24c02.sda = 0;
	x24c02.scl = 0;
	x24c02.state = X24C0X_STANDBY;
}

uint8 x24c01_read(void) {
	return x24c01.out;
}

void x24c01_write(uint8 V) {
	uint8 scl = (V >> 5) & 1;
	uint8 sda = (V >> 6) & 1;

	if (x24c01.scl && scl) {
		if (x24c01.sda && !sda) { /* START */
			x24c01.state = X24C0X_ADDRESS;
			x24c01.bitcount = 0;
			x24c01.addr = 0;
		} else if (!x24c01.sda && sda) { /* STOP */
			x24c01.state = X24C0X_STANDBY;
		}
	} else if (!x24c01.scl && scl) { /* RISING EDGE */
		switch (x24c01.state) {
		case X24C0X_ADDRESS:
			if (x24c01.bitcount < 7) {
				x24c01.addr <<= 1;
				x24c01.addr |= sda;
			} else {
				x24c01.word = x24c01.addr;
				if (sda) { /* READ COMMAND */
					x24c01.state = X24C0X_READ;
				} else { /* WRITE COMMAND */
					x24c01.state = X24C0X_WRITE;
				}
			}
			x24c01.bitcount++;
			break;
		case X24C0X_READ:
			if (x24c01.bitcount == 8) { /*  ACK */
				x24c01.out = 0;
				x24c01.latch = x24c01.data[x24c01.word];
				x24c01.bitcount = 0;
			} else { /* REAL OUTPUT */
				x24c01.out = x24c01.latch >> 7;
				x24c01.latch <<= 1;
				x24c01.bitcount++;
				if (x24c01.bitcount == 8) {
					x24c01.word++;
					x24c01.word &= 0xff;
				}
			}
			break;
		case X24C0X_WRITE:
			if (x24c01.bitcount == 8) { /* ACK */
				x24c01.out = 0;
				x24c01.latch = 0;
				x24c01.bitcount = 0;
			} else { /* REAL INPUT */
				x24c01.latch <<= 1;
				x24c01.latch |= sda;
				x24c01.bitcount++;
				if (x24c01.bitcount == 8) {
					x24c01.data[x24c01.word] = x24c01.latch;
					x24c01.word++;
					x24c01.word &= 0xff;
				}
			}
			break;
		}
	}

	x24c01.sda = sda;
	x24c01.scl = scl;
}

uint8 x24c02_read(void) {
	return x24c02.out;
}

void x24c02_write(uint8 V) {
	uint8 scl = (V >> 5) & 1;
	uint8 sda = (V >> 6) & 1;

	if (x24c02.scl && scl) {
		if (x24c02.sda && !sda) { /* START */
			x24c02.state = X24C0X_ADDRESS;
			x24c02.bitcount = 0;
			x24c02.addr = 0;
		} else if (!x24c02.sda && sda) { /* STOP */
			x24c02.state = X24C0X_STANDBY;
		}
	} else if (!x24c02.scl && scl) { /* RISING EDGE */
		switch (x24c02.state) {
		case X24C0X_ADDRESS:
			if (x24c02.bitcount < 7) {
				x24c02.addr <<= 1;
				x24c02.addr |= sda;
			} else {
				if (sda) { /* READ COMMAND */
					x24c02.state = X24C0X_READ;
				} else { /* WRITE COMMAND */
					x24c02.state = X24C0X_WORD;
				}
			}
			x24c02.bitcount++;
			break;
		case X24C0X_WORD:
			if (x24c02.bitcount == 8) { /* ACK */
				x24c02.word = 0;
				x24c02.out = 0;
			} else { /* WORD ADDRESS INPUT */
				x24c02.word <<= 1;
				x24c02.word |= sda;
				if (x24c02.bitcount == 16) { /* END OF ADDRESS INPUT */
					x24c02.bitcount = 7;
					x24c02.state = X24C0X_WRITE;
				}
			}
			x24c02.bitcount++;
			break;
		case X24C0X_READ:
			if (x24c02.bitcount == 8) { /* ACK */
				x24c02.out = 0;
				x24c02.latch = x24c02.data[x24c02.word];
				x24c02.bitcount = 0;
			} else { /* REAL OUTPUT */
				x24c02.out = x24c02.latch >> 7;
				x24c02.latch <<= 1;
				x24c02.bitcount++;
				if (x24c02.bitcount == 8) {
					x24c02.word++;
					x24c02.word &= 0xff;
				}
			}
			break;
		case X24C0X_WRITE:
			if (x24c02.bitcount == 8) { /* ACK */
				x24c02.out = 0;
				x24c02.latch = 0;
				x24c02.bitcount = 0;
			} else { /* REAL INPUT */
				x24c02.latch <<= 1;
				x24c02.latch |= sda;
				x24c02.bitcount++;
				if (x24c02.bitcount == 8) {
					x24c02.data[x24c02.word] = x24c02.latch;
					x24c02.word++;
					x24c02.word &= 0xff;
				}
			}
			break;
		}
	}

	x24c02.sda = sda;
	x24c02.scl = scl;
}
