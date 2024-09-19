/* license:BSD-3-Clause
   copyright-holders:Tony La Porta */
	/**************************************************************************\
	*                      Microchip PIC16C5x Emulator                         *
	*                                                                          *
	*                    Copyright Tony La Porta                               *
	*                 Originally written for the MAME project.                 *
	*                                                                          *
	*                                                                          *
	*      Addressing architecture is based on the Harvard addressing scheme.  *
	*                                                                          *
	*                                                                          *
	*  **** Change Log ****                                                    *
	*  TLP (06-Apr-2003)                                                       *
	*   - First Public release.                                                *
	*  BO  (07-Apr-2003) Ver 1.01                                              *
	*   - Renamed 'sleep' function to 'sleepic' to avoid C conflicts.          *
	*  TLP (09-Apr-2003) Ver 1.10                                              *
	*   - Fixed modification of file register $03 (Status).                    *
	*   - Corrected support for 7FFh (12-bit) size ROMs.                       *
	*   - The 'call' and 'goto' instructions weren't correctly handling the    *
	*     STATUS page info correctly.                                          *
	*   - The FSR register was incorrectly oring the data with 0xe0 when read. *
	*   - Prescaler masking information was set to 3 instead of 7.             *
	*   - Prescaler assign bit was set to 4 instead of 8.                      *
	*   - Timer source and edge select flags/masks were wrong.                 *
	*   - Corrected the memory bank selection in GET/SET_REGFILE and also the  *
	*     indirect register addressing.                                        *
	*  BMP (18-May-2003) Ver 1.11                                              *
	*   - PIC16C5x_get_reg functions were missing 'returns'.                   *
	*  TLP (27-May-2003) Ver 1.12                                              *
	*   - Fixed the WatchDog timer count.                                      *
	*   - The Prescaler rate was incorrectly being zeroed, instead of the      *
	*     actual Prescaler counter in the CLRWDT and SLEEP instructions.       *
	*   - Added masking to the FSR register. Upper unused bits are always 1.   *
	*  TLP (27-Aug-2009) Ver 1.13                                              *
	*   - Indirect addressing was not taking into account special purpose      *
	*     memory mapped locations.                                             *
	*   - 'iorlw' instruction was saving the result to memory instead of       *
	*     the W register.                                                      *
	*   - 'tris' instruction no longer modifies Port-C on PIC models that      *
	*     do not have Port-C implemented.                                      *
	*  TLP (07-Sep-2009) Ver 1.14                                              *
	*   - Edge sense control for the T0 count input was incorrectly reversed   *
	*  LE (05-Feb-2017) Ver 1.15                                               *
	*   - Allow writing all bits of the status register except TO and PD.      *
	*     This enables e.g. bcf, bsf or clrf to change the flags when the      *
	*     status register is the destination.                                  *
	*   - Changed rlf and rrf to update the carry flag in the last step.       *
	*     Fixes the case where the status register is the destination.         *
	*  hap (12-Feb-2017) Ver 1.16                                              *
	*   - Added basic support for the old GI PIC1650 and PIC1655.              *
	*   - Made RTCC(aka T0CKI) pin an inputline handler.                       *
	*                                                                          *
	*                                                                          *
	*  **** Notes: ****                                                        *
	*  PIC WatchDog Timer has a separate internal clock. For the moment, we're *
	*     basing the count on a 4MHz input clock, since 4MHz is the typical    *
	*     input frequency (but by no means always).                            *
	*  A single scaler is available for the Counter/Timer or WatchDog Timer.   *
	*     When connected to the Counter/Timer, it functions as a Prescaler,    *
	*     hence prescale overflows, tick the Counter/Timer.                    *
	*     When connected to the WatchDog Timer, it functions as a Postscaler   *
	*     hence WatchDog Timer overflows, tick the Postscaler. This scenario   *
	*     means that the WatchDog timeout occurs when the Postscaler has       *
	*     reached the scaler rate value, not when the WatchDog reaches zero.   *
	*  CLRWDT should prevent the WatchDog Timer from timing out and generating *
	*     a device reset, but how is not known. The manual also mentions that  *
	*     the WatchDog Timer can only be disabled during ROM programming, and  *
	*     no other means seem to exist???                                      *
	*                                                                          *
	\**************************************************************************/

#include <string.h>
#include "pic16c5x.h"

/******************** CPU Internal Registers *******************/
static uint16 m_PC;
static uint16 m_PREVPC; /* previous program counter */
static uint8 m_W;
static uint8 m_OPTION;
static uint16 m_CONFIG;
static uint8 m_ALU;
static uint16 m_WDT;
static uint8 m_TRISA;
static uint8 m_TRISB;
static uint8 m_TRISC;
static uint16 m_STACK[2];
static uint16 m_prescaler; /* Note: this is really an 8-bit register */
static uint16 m_opcode;
static uint8 m_internalram[128];
static uint8 *m_rom;

static int m_icount;
static int m_picmodel;
static int m_delay_timer;
static uint16 m_temp_config;
static int m_rtcc;
static uint8 m_count_pending; /* boolean type */
static int8 m_old_data;
static uint8 m_picRAMmask;
static uint16 m_picROMmask;
static int m_inst_cycles;
static int m_clock2cycle;

/*address_space *m_program;
memory_access_cache<1, -1, ENDIANNESS_LITTLE> *m_cache;
address_space *m_data;*/

/* i/o handlers */
static pic16c5x_readfunc m_read;
static pic16c5x_writefunc m_write;

static void PIC16C5x(int program_width, int data_width, int picmodel, uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr)
{
	m_rom = rom;
	m_picmodel = picmodel;
	m_temp_config = 0;
	m_picRAMmask = ((1 << data_width) - 1);
	m_picROMmask = ((1 << program_width) - 1);
	m_read = _rd;
	m_write = _wr;
	m_PC = 0;
	m_PREVPC = 0;
	m_W = 0;
	m_OPTION = 0;
	m_CONFIG = 0;
	m_ALU = 0;
	m_WDT = 0;
	m_TRISA = 0;
	m_TRISB = 0;
	m_TRISC = 0;
	m_prescaler = 0;
	m_icount = 0;
	m_delay_timer = 0;
	m_rtcc = 0;
	m_count_pending = 0;
	m_old_data = 0;
	m_inst_cycles = 0;
	m_clock2cycle = 0;
}

void pic16c54_init(uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr) { PIC16C5x( 9, 5, 0x16C54, rom, _rd, _wr); }
void pic16c55_init(uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr) { PIC16C5x( 9, 5, 0x16C55, rom, _rd, _wr); }
void pic16c56_init(uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr) { PIC16C5x(10, 5, 0x16C56, rom, _rd, _wr); }
void pic16c57_init(uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr) { PIC16C5x(11, 7, 0x16C57, rom, _rd, _wr); }
void pic16c58_init(uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr) { PIC16C5x(11, 7, 0x16C58, rom, _rd, _wr); }
void pic1650_init (uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr) { PIC16C5x( 9, 5, 0x1650,  rom, _rd, _wr); }
void pic1655_init (uint8 *rom, pic16c5x_readfunc _rd, pic16c5x_writefunc _wr) { PIC16C5x( 9, 5, 0x1655,  rom, _rd, _wr); }

#define M_OPCODE_B0 (m_opcode & 0xFF)
#define M_OPCODE_B1 ((m_opcode >> 8) & 0xFF)
#define M_OPCODE_S0 m_opcode

#define M_RDRAM(A)      m_internalram[A &m_picRAMmask]
#define M_WRTRAM(A,V)   m_internalram[A &m_picRAMmask] =(V)
#define M_RDOP(A)       m_rom[(A &m_picROMmask) <<1 |0] | m_rom[(A &m_picROMmask) <<1 |1] <<8
#define ADDR_MASK       0x7ff

#define TMR0    m_internalram[1]
#define PCL     m_internalram[2]
#define STATUS  m_internalram[3]
#define FSR     m_internalram[4]
#define PORTA   m_internalram[5]
#define PORTB   m_internalram[6]
#define PORTC   m_internalram[7]
#define PORTD   m_internalram[8]
#define INDF    M_RDRAM(FSR)

#define ADDR    (M_OPCODE_B0 & 0x1f)


/********  The following is the Status Flag register definition.  *********/
			/* | 7 | 6 | 5 |  4 |  3 | 2 |  1 | 0 | */
			/* |    PA     | TO | PD | Z | DC | C | */
#define PA_REG      0xe0    /* PA   Program Page Preselect - bit 8 is unused here */
#define TO_FLAG     0x10    /* TO   Time Out flag (WatchDog) */
#define PD_FLAG     0x08    /* PD   Power Down flag */
#define Z_FLAG      0x04    /* Z    Zero Flag */
#define DC_FLAG     0x02    /* DC   Digit Carry/Borrow flag (Nibble) */
#define C_FLAG      0x01    /* C    Carry/Borrow Flag (Byte) */

#define PA      (STATUS & PA_REG)
#define TO      (STATUS & TO_FLAG)
#define PD      (STATUS & PD_FLAG)
#define ZERO    (STATUS & Z_FLAG)
#define DC      (STATUS & DC_FLAG)
#define CARRY   (STATUS & C_FLAG)


/********  The following is the Option Flag register definition.  *********/
			/* | 7 | 6 |   5  |   4  |  3  | 2 | 1 | 0 | */
			/* | 0 | 0 | TOCS | TOSE | PSA |    PS     | */
#define T0CS_FLAG   0x20    /* TOCS     Timer 0 clock source select */
#define T0SE_FLAG   0x10    /* TOSE     Timer 0 clock source edge select */
#define PSA_FLAG    0x08    /* PSA      Prescaler Assignment bit */
#define PS_REG      0x07    /* PS       Prescaler Rate select */

#define T0CS    (m_OPTION & T0CS_FLAG)
#define T0SE    (m_OPTION & T0SE_FLAG)
#define PSA     (m_OPTION & PSA_FLAG)
#define PS      (m_OPTION & PS_REG)


/********  The following is the Config Flag register definition.  *********/
	/* | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 |   2  | 1 | 0 | */
	/* |              CP                     | WDTE |  FOSC | */
							/* CP       Code Protect (ROM read protect) */
#define WDTE_FLAG   0x04    /* WDTE     WatchDog Timer enable */
#define FOSC_FLAG   0x03    /* FOSC     Oscillator source select */

#define WDTE    (m_CONFIG & WDTE_FLAG)
#define FOSC    (m_CONFIG & FOSC_FLAG)


/************************************************************************
 *  Shortcuts
 ************************************************************************/

#define CLR(flagreg, flag) ( flagreg &= (uint8)(~flag) )
#define SET(flagreg, flag) ( flagreg |=  flag )


/* Easy bit position selectors */
#define POS  ((M_OPCODE_B0 >> 5) & 7)
static const unsigned int bit_clr[8] = { 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f };
static const unsigned int bit_set[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

static INLINE void CALCULATE_Z_FLAG(void)
{
	if (m_ALU == 0) SET(STATUS, Z_FLAG);
	else CLR(STATUS, Z_FLAG);
}

static INLINE void CALCULATE_ADD_CARRY(void)
{
	if ((uint8)(m_old_data) > (uint8)(m_ALU)) {
		SET(STATUS, C_FLAG);
	}
	else {
		CLR(STATUS, C_FLAG);
	}
}

static INLINE void CALCULATE_ADD_DIGITCARRY(void)
{
	if (((uint8)(m_old_data) & 0x0f) > ((uint8)(m_ALU) & 0x0f)) {
		SET(STATUS, DC_FLAG);
	}
	else {
		CLR(STATUS, DC_FLAG);
	}
}

static INLINE void CALCULATE_SUB_CARRY(void)
{
	if ((uint8)(m_old_data) < (uint8)(m_ALU)) {
		CLR(STATUS, C_FLAG);
	}
	else {
		SET(STATUS, C_FLAG);
	}
}

static INLINE void CALCULATE_SUB_DIGITCARRY(void)
{
	if (((uint8)(m_old_data) & 0x0f) < ((uint8)(m_ALU) & 0x0f)) {
		CLR(STATUS, DC_FLAG);
	}
	else {
		SET(STATUS, DC_FLAG);
	}
}



static INLINE uint16 POP_STACK(void)
{
	uint16 data = m_STACK[1];
	m_STACK[1] = m_STACK[0];
	return (data & ADDR_MASK);
}
static INLINE void PUSH_STACK(uint16 data)
{
	m_STACK[0] = m_STACK[1];
	m_STACK[1] = (data & ADDR_MASK);
}



static INLINE uint8 GET_REGFILE(uint32_t addr) /* Read from internal memory */
{
	uint8 data = 0;

	if (addr == 0) {                        /* Indirect addressing  */
		addr = (FSR & m_picRAMmask);
	}

	if ((m_picmodel == 0x16C57) || (m_picmodel == 0x16C58)) {
		addr |= (FSR & 0x60);     /* FSR bits 6-5 are used for banking in direct mode */
	}

	if ((addr & 0x10) == 0) addr &= 0x0f;

	switch(addr)
	{
		case 0:     /* Not an actual register, so return 0 */
					data = 0;
					break;
		case 4:     data = (FSR | (uint8)(~m_picRAMmask));
					break;
		case 5:     /* read port A */
					if (m_picmodel == 0x1650) {
						data = m_read(PIC16C5x_PORTA) & PORTA;
					}
					else if (m_picmodel == 0x1655) {
						data = m_read(PIC16C5x_PORTA) & 0x0f;
					}
					else {
						data = m_read(PIC16C5x_PORTA);
						data &= m_TRISA;
						data |= ((uint8)(~m_TRISA) & PORTA);
						data &= 0x0f; /* 4-bit port (only lower 4 bits used) */
					}
					break;
		case 6:     /* read port B */
					if (m_picmodel == 0x1650) {
						data = m_read(PIC16C5x_PORTB) & PORTB;
					}
					else if (m_picmodel != 0x1655) { /* B is output-only on 1655 */
						data = m_read(PIC16C5x_PORTB);
						data &= m_TRISB;
						data |= ((uint8)(~m_TRISB) & PORTB);
					}
					break;
		case 7:     /* read port C */
					if (m_picmodel == 0x1650 || m_picmodel == 0x1655) {
						data = m_read(PIC16C5x_PORTC) & PORTC;
					}
					else if ((m_picmodel == 0x16C55) || (m_picmodel == 0x16C57)) {
						data = m_read(PIC16C5x_PORTC);
						data &= m_TRISC;
						data |= ((uint8)(~m_TRISC) & PORTC);
					}
					else { /* PIC16C54, PIC16C56, PIC16C58 */
						data = M_RDRAM(addr);
					}
					break;
		case 8:     /* read port D */
					if (m_picmodel == 0x1650) {
						data = m_read(PIC16C5x_PORTD) & PORTD;
					}
					else {
						data = M_RDRAM(addr);
					}
					break;
		default:    data = M_RDRAM(addr);
					break;
	}
	return data;
}

static INLINE void STORE_REGFILE(uint32_t addr, uint8 data)    /* Write to internal memory */
{
	if (addr == 0) {                        /* Indirect addressing  */
		addr = (FSR & m_picRAMmask);
	}

	if ((m_picmodel == 0x16C57) || (m_picmodel == 0x16C58)) {
		addr |= (FSR & 0x60);     /* FSR bits 6-5 are used for banking in direct mode */
	}

	if ((addr & 0x10) == 0) addr &= 0x0f;

	switch(addr)
	{
		case 0:     /* Not an actual register, nothing to save */
					break;
		case 1:     m_delay_timer = 2; /* Timer starts after next two instructions */
					if (PSA == 0) m_prescaler = 0; /* Must clear the Prescaler */
					TMR0 = data;
					break;
		case 2:     PCL = data;
					m_PC = ((STATUS & PA_REG) << 4) | data;
					break;
		case 3:     STATUS = (STATUS & (TO_FLAG | PD_FLAG)) | (data & (uint8)(~(TO_FLAG | PD_FLAG)));
					break;
		case 4:     FSR = (data | (uint8)(~m_picRAMmask));
					break;
		case 5:     /* write port A */
					if (m_picmodel == 0x1650) {
						m_write(PIC16C5x_PORTA, data);
					}
					else if (m_picmodel != 0x1655) { /* A is input-only on 1655 */
						data &= 0x0f; /* 4-bit port (only lower 4 bits used) */
						m_write(PIC16C5x_PORTA, data & (uint8)(~m_TRISA));
					}
					PORTA = data;
					break;
		case 6:     /* write port B */
					if (m_picmodel == 0x1650 || m_picmodel == 0x1655) {
						m_write(PIC16C5x_PORTB, data);
					}
					else {
						m_write(PIC16C5x_PORTB, data & (uint8)(~m_TRISB));
					}
					PORTB = data;
					break;
		case 7:     /* write port C */
					if (m_picmodel == 0x1650 || m_picmodel == 0x1655) {
						m_write(PIC16C5x_PORTC, data);
					}
					else if ((m_picmodel == 0x16C55) || (m_picmodel == 0x16C57)) {
						m_write(PIC16C5x_PORTC, data & (uint8)(~m_TRISC));
					}
					PORTC = data; /* also writes to RAM */
					break;
		case 8:     /* write port D */
					if (m_picmodel == 0x1650) {
						m_write(PIC16C5x_PORTD, data);
					}
					PORTD = data; /* also writes to RAM */
					break;
		default:    M_WRTRAM(addr, data);
					break;
	}
}


static INLINE void STORE_RESULT(uint32_t addr, uint8 data)
{
	if (M_OPCODE_B0 & 0x20)
	{
		STORE_REGFILE(addr, data);
	}
	else
	{
		m_W = data;
	}
}


/************************************************************************
 *  Emulate the Instructions
 ************************************************************************/

/* This following function is here to fill in the void for */
/* the opcode call function. This function is never called. */


static INLINE void illegal(void)
{
	/* printf("PIC16C5x:  PC=%03x,  Illegal opcode = %04x\n", (m_PC-1), M_OPCODE_S0); */
}

/*
  Note:
  According to the manual, if the STATUS register is the destination for an instruction that affects the Z, DC or C bits
  then the write to these three bits is disabled. These bits are set or cleared according to the device logic.
  To ensure this is correctly emulated, in instructions that write to the file registers, always change the status flags
  *after* storing the result of the instruction.
  e.g. CALCULATE_*, SET(STATUS,*_FLAG) and CLR(STATUS,*_FLAG) should appear as the last steps of the instruction emulation.
*/

static INLINE void addwf(void)
{
	m_old_data = GET_REGFILE(ADDR);
	m_ALU = m_old_data + m_W;
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
	CALCULATE_ADD_CARRY();
	CALCULATE_ADD_DIGITCARRY();
}

static INLINE void andwf(void)
{
	m_ALU = GET_REGFILE(ADDR) & m_W;
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
}

static INLINE void andlw(void)
{
	m_ALU = M_OPCODE_B0 & m_W;
	m_W = m_ALU;
	CALCULATE_Z_FLAG();
}

static INLINE void bcf(void)
{
	m_ALU = GET_REGFILE(ADDR);
	m_ALU &= bit_clr[POS];
	STORE_REGFILE(ADDR, m_ALU);
}

static INLINE void bsf(void)
{
	m_ALU = GET_REGFILE(ADDR);
	m_ALU |= bit_set[POS];
	STORE_REGFILE(ADDR, m_ALU);
}

static INLINE void btfss(void)
{
	if ((GET_REGFILE(ADDR) & bit_set[POS]) == bit_set[POS])
	{
		m_PC++;
		PCL = m_PC & 0xff;
		m_inst_cycles += 1;     /* Add NOP cycles */
	}
}

static INLINE void btfsc(void)
{
	if ((GET_REGFILE(ADDR) & bit_set[POS]) == 0)
	{
		m_PC++;
		PCL = m_PC & 0xff;
		m_inst_cycles += 1;     /* Add NOP cycles */
	}
}

static INLINE void call(void)
{
	PUSH_STACK(m_PC);
	m_PC = ((STATUS & PA_REG) << 4) | M_OPCODE_B0;
	m_PC &= 0x6ff;
	PCL = m_PC & 0xff;
}

static INLINE void clrw(void)
{
	m_W = 0;
	SET(STATUS, Z_FLAG);
}

static INLINE void clrf(void)
{
	STORE_REGFILE(ADDR, 0);
	SET(STATUS, Z_FLAG);
}

static INLINE void clrwdt(void)
{
	m_WDT = 0;
	if (PSA) m_prescaler = 0;
	SET(STATUS, TO_FLAG);
	SET(STATUS, PD_FLAG);
}

static INLINE void comf(void)
{
	m_ALU = (uint8)(~(GET_REGFILE(ADDR)));
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
}

static INLINE void decf(void)
{
	m_ALU = GET_REGFILE(ADDR) - 1;
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
}

static INLINE void decfsz(void)
{
	m_ALU = GET_REGFILE(ADDR) - 1;
	STORE_RESULT(ADDR, m_ALU);
	if (m_ALU == 0)
	{
		m_PC++;
		PCL = m_PC & 0xff;
		m_inst_cycles += 1;     /* Add NOP cycles */
	}
}

static INLINE void goto_op(void)
{
	m_PC = ((STATUS & PA_REG) << 4) | (M_OPCODE_S0 & 0x1ff);
	m_PC &= ADDR_MASK;
	PCL = m_PC & 0xff;
}

static INLINE void incf(void)
{
	m_ALU = GET_REGFILE(ADDR) + 1;
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
}

static INLINE void incfsz(void)
{
	m_ALU = GET_REGFILE(ADDR) + 1;
	STORE_RESULT(ADDR, m_ALU);
	if (m_ALU == 0)
	{
		m_PC++;
		PCL = m_PC & 0xff;
		m_inst_cycles += 1;     /* Add NOP cycles */
	}
}

static INLINE void iorlw(void)
{
	m_ALU = M_OPCODE_B0 | m_W;
	m_W = m_ALU;
	CALCULATE_Z_FLAG();
}

static INLINE void iorwf(void)
{
	m_ALU = GET_REGFILE(ADDR) | m_W;
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
}

static INLINE void movf(void)
{
	m_ALU = GET_REGFILE(ADDR);
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
}

static INLINE void movlw(void)
{
	m_W = M_OPCODE_B0;
}

static INLINE void movwf(void)
{
	STORE_REGFILE(ADDR, m_W);
}

static INLINE void nop(void)
{
	/* Do nothing */
}

static INLINE void option(void)
{
	m_OPTION = m_W & (T0CS_FLAG | T0SE_FLAG | PSA_FLAG | PS_REG);
}

static INLINE void retlw(void)
{
	m_W = M_OPCODE_B0;
	m_PC = POP_STACK();
	PCL = m_PC & 0xff;
}

static INLINE void rlf(void)
{
	uint8 bit7;
	m_ALU = GET_REGFILE(ADDR);
	bit7 = m_ALU & 0x80;
	m_ALU <<= 1;
	if (STATUS & C_FLAG) m_ALU |= 1;
	STORE_RESULT(ADDR, m_ALU);
	if (bit7) SET(STATUS, C_FLAG);
	else CLR(STATUS, C_FLAG);
}

static INLINE void rrf(void)
{
	uint8 bit0;
	m_ALU = GET_REGFILE(ADDR);
	bit0 = m_ALU & 1;
	m_ALU >>= 1;
	if (STATUS & C_FLAG) m_ALU |= 0x80;
	STORE_RESULT(ADDR, m_ALU);
	if (bit0) SET(STATUS, C_FLAG);
	else CLR(STATUS, C_FLAG);
}

static INLINE void sleepic(void)
{
	if (WDTE) m_WDT = 0;
	if (PSA) m_prescaler = 0;
	SET(STATUS, TO_FLAG);
	CLR(STATUS, PD_FLAG);
}

static INLINE void subwf(void)
{
	m_old_data = GET_REGFILE(ADDR);
	m_ALU = m_old_data - m_W;
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
	CALCULATE_SUB_CARRY();
	CALCULATE_SUB_DIGITCARRY();
}

static INLINE void swapf(void)
{
	m_ALU  = ((GET_REGFILE(ADDR) << 4) & 0xf0);
	m_ALU |= ((GET_REGFILE(ADDR) >> 4) & 0x0f);
	STORE_RESULT(ADDR, m_ALU);
}

static INLINE void tris(void)
{
	switch(M_OPCODE_B0 & 0x7)
	{
		case 5:     if   (m_TRISA == m_W) break;
					else { m_TRISA = m_W | 0xf0; m_write(PIC16C5x_PORTA, 0x1000 | (PORTA & (uint8)(~m_TRISA) & 0x0f)); break; }
		case 6:     if   (m_TRISB == m_W) break;
					else { m_TRISB = m_W; m_write(PIC16C5x_PORTB, 0x1000 | (PORTB & (uint8)(~m_TRISB))); break; }
		case 7:     if ((m_picmodel == 0x16C55) || (m_picmodel == 0x16C57)) {
						if   (m_TRISC == m_W) break;
						else { m_TRISC = m_W; m_write(PIC16C5x_PORTC, 0x1000 | (PORTC & (uint8)(~m_TRISC))); break; }
					}
					else {
						illegal(); break;
					}
		default:    illegal(); break;
	}
}

static INLINE void xorlw(void)
{
	m_ALU = m_W ^ M_OPCODE_B0;
	m_W = m_ALU;
	CALCULATE_Z_FLAG();
}

static INLINE void xorwf(void)
{
	m_ALU = GET_REGFILE(ADDR) ^ m_W;
	STORE_RESULT(ADDR, m_ALU);
	CALCULATE_Z_FLAG();
}




/***********************************************************************
 *  Opcode Table (Cycles, Instruction)
 ***********************************************************************/

static const uint8 s_opcode_main_cycles[256] = {
/*00*/  1, 1, 1, 1, 1, 1, 1, 1,
/*08*/  1, 1, 1, 1, 1, 1, 1, 1,
/*10*/  1, 1, 1, 1, 1, 1, 1, 1,
/*18*/  1, 1, 1, 1, 1, 1, 1, 1,
/*20*/  1, 1, 1, 1, 1, 1, 1, 1,
/*28*/  1, 1, 1, 1, 1, 1, 1, 1,
/*30*/  1, 1, 1, 1, 1, 1, 1, 1,
/*38*/  1, 1, 1, 1, 1, 1, 1, 1,
/*40*/  1, 1, 1, 1, 1, 1, 1, 1,
/*48*/  1, 1, 1, 1, 1, 1, 1, 1,
/*50*/  1, 1, 1, 1, 1, 1, 1, 1,
/*58*/  1, 1, 1, 1, 1, 1, 1, 1,
/*60*/  1, 1, 1, 1, 1, 1, 1, 1,
/*68*/  1, 1, 1, 1, 1, 1, 1, 1,
/*70*/  1, 1, 1, 1, 1, 1, 1, 1,
/*78*/  1, 1, 1, 1, 1, 1, 1, 1,

/*80*/  2, 2, 2, 2, 2, 2, 2, 2,
/*88*/  2, 2, 2, 2, 2, 2, 2, 2,
/*90*/  2, 2, 2, 2, 2, 2, 2, 2,
/*98*/  2, 2, 2, 2, 2, 2, 2, 2,
/*A0*/  2, 2, 2, 2, 2, 2, 2, 2,
/*A8*/  2, 2, 2, 2, 2, 2, 2, 2,
/*B0*/  2, 2, 2, 2, 2, 2, 2, 2,
/*B8*/  2, 2, 2, 2, 2, 2, 2, 2,

/*C0*/  1, 1, 1, 1, 1, 1, 1, 1,
/*C8*/  1, 1, 1, 1, 1, 1, 1, 1,
/*D0*/  1, 1, 1, 1, 1, 1, 1, 1,
/*D8*/  1, 1, 1, 1, 1, 1, 1, 1,
/*E0*/  1, 1, 1, 1, 1, 1, 1, 1,
/*E8*/  1, 1, 1, 1, 1, 1, 1, 1,
/*F0*/  1, 1, 1, 1, 1, 1, 1, 1,
/*F8*/  1, 1, 1, 1, 1, 1, 1, 1
};

static const uint8 s_opcode_00x_cycles[16] = {
/*00*/  1, 1, 1, 1, 1, 1, 1, 1,
/*08*/  1, 1, 1, 1, 1, 1, 1, 1
};



/****************************************************************************
 *  Inits CPU emulation
 ****************************************************************************/

enum
{
	PIC16C5x_PC=1, PIC16C5x_STK0, PIC16C5x_STK1, PIC16C5x_FSR,
	PIC16C5x_W,    PIC16C5x_ALU,  PIC16C5x_STR,  PIC16C5x_OPT,
	PIC16C5x_TMR0, PIC16C5x_PRTA, PIC16C5x_PRTB, PIC16C5x_PRTC, PIC16C5x_PRTD,
	PIC16C5x_WDT,  PIC16C5x_TRSA, PIC16C5x_TRSB, PIC16C5x_TRSC,
	PIC16C5x_PSCL
};

/****************************************************************************
 *  Reset registers to their initial values
 ****************************************************************************/

static INLINE void PIC16C5x_reset_regs(void)
{
	m_PC     = m_picROMmask;
	m_CONFIG = m_temp_config;
	m_TRISA  = 0xff;
	m_TRISB  = 0xff;
	m_TRISC  = 0xff;
	m_OPTION = (T0CS_FLAG | T0SE_FLAG | PSA_FLAG | PS_REG);
	PCL    = 0xff;
	FSR   |= (uint8)(~m_picRAMmask);
	m_prescaler = 0;
	m_delay_timer = 0;
	m_inst_cycles = 0;
	m_count_pending = FALSE;
}

void pic16c5x_reset(uint8 hard)
{
	if (hard) {
		memset(m_internalram, 0, sizeof(m_internalram));
		PIC16C5x_reset_regs();
		CLR(STATUS, PA_REG);
		SET(STATUS, (TO_FLAG | PD_FLAG));
		m_icount = 0;
		m_clock2cycle = 0;
	} else {
		SET(STATUS, (TO_FLAG | PD_FLAG | Z_FLAG | DC_FLAG | C_FLAG));
		PIC16C5x_reset_regs();
	}
}

void pic16c5x_set_config(uint16 data)
{
	m_temp_config = data;
}



/****************************************************************************
 *  WatchDog
 ****************************************************************************/

static INLINE void PIC16C5x_update_watchdog(int counts)
{
	/* WatchDog is set up to count 18,000 (0x464f hex) ticks to provide */
	/* the timeout period of 0.018ms based on a 4MHz input clock. */
	/* Note: the 4MHz clock should be divided by the PIC16C5x_CLOCK_DIVIDER */
	/* which effectively makes the PIC run at 1MHz internally. */

	/* If the current instruction is CLRWDT or SLEEP, don't update the WDT */

	if ((M_OPCODE_S0 != 3) && (M_OPCODE_S0 != 4))
	{
		uint16 old_WDT = m_WDT;

		m_WDT -= counts;

		if (m_WDT > 0x464f) {
			m_WDT = 0x464f - (0xffff - m_WDT);
		}

		if (((old_WDT != 0) && (old_WDT < m_WDT)) || (m_WDT == 0))
		{
			if (PSA) {
				m_prescaler++;
				if (m_prescaler >= (1 << PS)) { /* Prescale values from 1 to 128 */
					m_prescaler = 0;
					CLR(STATUS, TO_FLAG);
					/* PIC16C5x_soft_reset(); */
				}
			}
			else {
				CLR(STATUS, TO_FLAG);
				/* PIC16C5x_soft_reset(); */
			}
		}
	}
}


/****************************************************************************
 *  Update Timer
 ****************************************************************************/

static INLINE void PIC16C5x_update_timer(int counts)
{
	if (PSA == 0) {
		m_prescaler += counts;
		if (m_prescaler >= (2 << PS)) { /* Prescale values from 2 to 256 */
			TMR0 += (m_prescaler / (2 << PS));
			m_prescaler %= (2 << PS);   /* Overflow prescaler */
		}
	}
	else {
		TMR0 += counts;
	}
}

void pic16c5x_set_input(int line, int state)
{
	switch (line)
	{
		/* RTCC/T0CKI pin */
		case PIC16C5x_RTCC:
			if (T0CS && state != m_rtcc) /* Count mode, edge triggered */
				if ((T0SE && !state) || (!T0SE && state))
					m_count_pending = TRUE;

			m_rtcc = state;
			break;

		default:
			break;
	}
}


void pic16c5x_run(void)
{
	if ((++m_clock2cycle &3) ==0) m_icount++;

	while (m_icount > 0) {
		if (PD == 0) /* Sleep Mode */
		{
			m_count_pending = FALSE;
			m_inst_cycles = 1;
			if (WDTE) {
				PIC16C5x_update_watchdog(1);
			}
		}
		else
		{
			if (m_count_pending) { /* RTCC/T0CKI clocked while in Count mode */
				m_count_pending = FALSE;
				PIC16C5x_update_timer(1);
			}

			m_PREVPC = m_PC;

			M_OPCODE_S0 = M_RDOP(m_PC);
			/* printf("%04X: %03X %03X\n", m_PC, M_OPCODE_S0, M_OPCODE_B0); */
			m_PC++;
			PCL++;

			if (m_picmodel == 0x1650 || m_picmodel == 0x1655 || (M_OPCODE_S0 & 0xff0) != 0x000) { /* Do all opcodes except the 00? ones */
				uint8 b1 = (M_OPCODE_S0 >> 4) & 0xff;
				m_inst_cycles = s_opcode_main_cycles[b1];
				switch (b1) {
					#include "pic16c5x_ops_main.inc"
				}
			}
			else {  /* Opcode 0x00? has many opcodes in its minor nibble */
				uint8 b1 = M_OPCODE_B0 & 0x1f;
				m_inst_cycles = s_opcode_00x_cycles[b1];
				switch (b1) {
					#include "pic16c5x_ops_00x.inc"
				}
			}

			if (!T0CS) { /* Timer mode */
				if (m_delay_timer) {
					m_delay_timer--;
				}
				else {
					PIC16C5x_update_timer(m_inst_cycles);
				}
			}
			if (WDTE) {
				PIC16C5x_update_watchdog(m_inst_cycles);
			}
		}

		m_icount -= m_inst_cycles;
	} 
}

void AddExState(void *v, uint32 s, int type, char *desc);

void pic16c5x_add_statesinfo(void) {
	AddExState(&m_PC, sizeof(m_PC), 0, "PC00");
	AddExState(&m_PREVPC, sizeof(m_PREVPC), 0, "PRPC");
	AddExState(&m_W, sizeof(m_W), 0, "W000");
	AddExState(&m_OPTION, sizeof(m_OPTION), 0, "OPT0");
	AddExState(&m_CONFIG, sizeof(m_CONFIG), 0, "CONF");
	AddExState(&m_ALU, sizeof(m_ALU), 0, "ALU");
	AddExState(&m_WDT, sizeof(m_WDT), 0, "WDT");
	AddExState(&m_TRISA, sizeof(m_TRISA), 0, "TRSA");
	AddExState(&m_TRISB, sizeof(m_TRISB), 0, "TRSB");
	AddExState(&m_TRISC, sizeof(m_TRISC), 0, "TRSC");
	AddExState(&m_STACK[0], sizeof(m_STACK[0]), 0, "STC0");
	AddExState(&m_STACK[1], sizeof(m_STACK[1]), 0, "STC1");
	AddExState(&m_prescaler, sizeof(m_prescaler), 0, "PSCL");
	AddExState(&m_opcode, sizeof(m_opcode), 0, "OPS0");
	AddExState(m_internalram, sizeof(m_internalram), 0, "IRAM");
	AddExState(&m_icount, sizeof(m_icount), 0, "ICNT");
	AddExState(&m_delay_timer, sizeof(m_delay_timer), 0, "DTIM");
	AddExState(&m_rtcc, sizeof(m_rtcc), 0, "RTCC");
	AddExState(&m_count_pending, sizeof(m_count_pending), 0, "CNTP");
	AddExState(&m_inst_cycles, sizeof(m_inst_cycles), 0, "ICYC");
	AddExState(&m_clock2cycle, sizeof(m_clock2cycle), 0, "CL2C");
}