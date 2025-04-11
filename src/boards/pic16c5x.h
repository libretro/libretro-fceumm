/* license:BSD-3-Clause */
/* copyright-holders:Tony La Porta */
	/**************************************************************************\
	*                      Microchip PIC16C5x_ Emulator                         *
	*                                                                          *
	*                    Copyright Tony La Porta                               *
	*                 Originally written for the MAME project.                 *
	*                                                                          *
	*                                                                          *
	*      Addressing architecture is based on the Harvard addressing scheme.  *
	*                                                                          *
	\**************************************************************************/

#ifndef _PIC16C5x_H
#define _PIC16C5x_H

#include "../../fceu-types.h"

typedef uint8 (*pic16c5x_readfunc)(int);
typedef void (*pic16c5x_writefunc)(int, int);

enum {
	PIC16C5x_RTCC = 0
};


/* in the mid-90s RTCC was renamed to T0CKI */
#define PIC16C5x_T0CKI PIC16C5x_RTCC

/* i/o ports */
enum {
	PIC16C5x_PORTA = 0,
	PIC16C5x_PORTB,
	PIC16C5x_PORTC,
	PIC16C5x_PORTD
};

/****************************************************************************
 *  Function to configure the CONFIG register. This is actually hard-wired
 *  during ROM programming, so should be called in the driver INIT, with
 *  the value if known (available in HEX dumps of the ROM).
 */
void pic16c5x_set_input(int line, int state);
void pic16c5x_set_config(uint16 data);
void pic16c5x_reset(uint8 hard);
void pic16c5x_run(void);
void pic16c5x_add_statesinfo(void);

void pic16c54_init(uint8* rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr);
void pic16c55_init(uint8* rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr);
void pic16c56_init(uint8* rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr);
void pic16c57_init(uint8* rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr);
void pic16c58_init(uint8* rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr);
void pic1650_init(uint8* rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr);
void pic1655_init(uint8* rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr);

#endif /* _PIC16C5x_H */
