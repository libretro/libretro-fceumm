#include "eeprom_24C0x.h"

/* x24C0x interface */

#define X24C0X_STANDBY		0
#define X24C0X_ADDRESS		1
#define X24C0X_WORD		2
#define X24C0X_READ		3
#define X24C0X_WRITE		4

uint8_t x24c0x_data[512];

static uint8_t x24c01_state;
static uint8_t x24c01_addr, x24c01_word, x24c01_latch, x24c01_bitcount;
static uint8_t x24c01_sda, x24c01_scl;
uint8_t x24c01_out;

static uint8_t x24c02_state;
static uint8_t x24c02_addr, x24c02_word, x24c02_latch, x24c02_bitcount;
static uint8_t x24c02_sda, x24c02_scl;
uint8_t x24c02_out;

SFORMAT x24c01StateRegs[] =
{
	{ &x24c01_addr, 1, "ADDR" },
	{ &x24c01_word, 1, "WORD" },
	{ &x24c01_latch, 1, "LATC" },
	{ &x24c01_bitcount, 1, "BITC" },
	{ &x24c01_sda, 1, "SDA" },
	{ &x24c01_scl, 1, "SCL" },
	{ &x24c01_out, 1, "OUT" },
	{ &x24c01_state, 1, "STAT" },
	{ 0 }
};

SFORMAT x24c02StateRegs[] =
{
	{ &x24c02_addr, 1, "ADDR" },
	{ &x24c02_word, 1, "WORD" },
	{ &x24c02_latch, 1, "LATC" },
	{ &x24c02_bitcount, 1, "BITC" },
	{ &x24c02_sda, 1, "SDA" },
	{ &x24c02_scl, 1, "SCL" },
	{ &x24c02_out, 1, "OUT" },
	{ &x24c02_state, 1, "STAT" },
	{ 0 }
};

void x24c01_init(void) {
	x24c01_addr = x24c01_word = x24c01_latch = x24c01_bitcount = x24c01_sda = x24c01_scl = 0;
	x24c01_state = X24C0X_STANDBY;
}

void x24c02_init(void) {
	x24c02_addr = x24c02_word = x24c02_latch = x24c02_bitcount = x24c02_sda = x24c02_scl = 0;
	x24c02_state = X24C0X_STANDBY;
}

void x24c01_write(uint8_t scl, uint8_t sda) {
	if(x24c01_scl && scl) {
		if(x24c01_sda && !sda) {			/* START */
			x24c01_state = X24C0X_ADDRESS;
			x24c01_bitcount = 0;
			x24c01_addr = 0;
		} else if(!x24c01_sda && sda) {		/* STOP */
			x24c01_state = X24C0X_STANDBY;
		}
	} else if(!x24c01_scl && scl) {			/* RISING EDGE */
		switch(x24c01_state) {
		case X24C0X_ADDRESS:
			if(x24c01_bitcount < 7) {
				x24c01_addr <<= 1;
				x24c01_addr |= sda;
			} else {
				x24c01_word = x24c01_addr;
				if(sda) 					/* READ COMMAND */
					x24c01_state = X24C0X_READ;
				 else 						/* WRITE COMMAND */
					x24c01_state = X24C0X_WRITE;
			}
			x24c01_bitcount++;
			break;
		case X24C0X_READ:
			if (x24c01_bitcount == 8) {		/*  ACK */
				x24c01_out = 0;
				x24c01_latch = x24c0x_data[x24c01_word];
				x24c01_bitcount = 0;
			} else {						/* REAL OUTPUT */
				x24c01_out = x24c01_latch >> 7;
				x24c01_latch <<= 1;
				x24c01_bitcount++;
				if(x24c01_bitcount == 8) {
					x24c01_word++;
					x24c01_word &= 0xff;
				}
			}
			break;
		case X24C0X_WRITE:
			if (x24c01_bitcount == 8) {		/* ACK */
				x24c01_out = 0;
				x24c01_latch = 0;
				x24c01_bitcount = 0;
			} else {						/* REAL INPUT */
				x24c01_latch <<= 1;
				x24c01_latch |= sda;
				x24c01_bitcount++;
				if(x24c01_bitcount == 8) {
					x24c0x_data[x24c01_word] = x24c01_latch;
					x24c01_word++;
					x24c01_word &= 0xff;
				}
			}
			break;
		}
	}

	x24c01_sda = sda;
	x24c01_scl = scl;
}

void x24c02_write(uint8_t scl, uint8_t sda) {
	if (x24c02_scl && scl) {
		if (x24c02_sda && !sda) {			/* START */
			x24c02_state = X24C0X_ADDRESS;
			x24c02_bitcount = 0;
			x24c02_addr = 0;
		} else if (!x24c02_sda && sda) {	/* STOP */
			x24c02_state = X24C0X_STANDBY;
		}
	} else if (!x24c02_scl && scl) {		/* RISING EDGE */
		switch (x24c02_state) {
		case X24C0X_ADDRESS:
			if (x24c02_bitcount < 7) {
				x24c02_addr <<= 1;
				x24c02_addr |= sda;
			} else {
				if (sda)					/* READ COMMAND */
					x24c02_state = X24C0X_READ;
				else						/* WRITE COMMAND */
					x24c02_state = X24C0X_WORD;
			}
			x24c02_bitcount++;
			break;
		case X24C0X_WORD:
			if (x24c02_bitcount == 8) {		/* ACK */
				x24c02_word = 0;
				x24c02_out = 0;
			} else {						/* WORD ADDRESS INPUT */
				x24c02_word <<= 1;
				x24c02_word |= sda;
				if (x24c02_bitcount == 16) { /* END OF ADDRESS INPUT */
					x24c02_bitcount = 7;
					x24c02_state = X24C0X_WRITE;
				}
			}
			x24c02_bitcount++;
			break;
		case X24C0X_READ:
			if (x24c02_bitcount == 8) {		/* ACK */
				x24c02_out = 0;
				x24c02_latch = x24c0x_data[x24c02_word|0x100];
				x24c02_bitcount = 0;
			} else {						/* REAL OUTPUT */
				x24c02_out = x24c02_latch >> 7;
				x24c02_latch <<= 1;
				x24c02_bitcount++;
				if (x24c02_bitcount == 8) {
					x24c02_word++;
					x24c02_word &= 0xff;
				}
			}
			break;
		case X24C0X_WRITE:
			if (x24c02_bitcount == 8) {		/* ACK */
				x24c02_out = 0;
				x24c02_latch = 0;
				x24c02_bitcount = 0;
			} else {						/* REAL INPUT */
				x24c02_latch <<= 1;
				x24c02_latch |= sda;
				x24c02_bitcount++;
				if (x24c02_bitcount == 8) {
					x24c0x_data[x24c02_word|0x100] = x24c02_latch;
					x24c02_word++;
					x24c02_word &= 0xff;
				}
			}
			break;
		}
	}

	x24c02_sda = sda;
	x24c02_scl = scl;
}
