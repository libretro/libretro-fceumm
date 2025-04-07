#include "eeprom_93Cx6.h"

uint8* eeprom_93Cx6_storage;
uint8  eeprom_93Cx6_opcode;
uint16 eeprom_93Cx6_data;
uint16 eeprom_93Cx6_address;
uint8  eeprom_93Cx6_state;
uint8  eeprom_93Cx6_lastCLK;
uint8  eeprom_93Cx6_writeEnabled;
uint8  eeprom_93Cx6_output;
size_t eeprom_93Cx6_capacity;
uint8  eeprom_93Cx6_wordSize;

static uint8 STATE_ADDRESS;
static uint8 STATE_DATA;

#define OPCODE_MISC         0
#define OPCODE_WRITE        1
#define OPCODE_READ         2
#define OPCODE_ERASE        3
#define OPCODE_WRITEDISABLE 10
#define OPCODE_WRITEALL     11
#define OPCODE_ERASEALL     12
#define OPCODE_WRITEENABLE  13

#define STATE_STANDBY   0
#define STATE_STARTBIT  1
#define STATE_OPCODE    3
#define STATE_ADDRESS8  12
#define STATE_DATA8     20
#define STATE_ADDRESS16 11
#define STATE_DATA16    27
#define STATE_FINISHED  99

void eeprom_93Cx6_init (size_t capacity, uint8 wordSize) {
	eeprom_93Cx6_capacity     =capacity;
	eeprom_93Cx6_wordSize     =wordSize;
	eeprom_93Cx6_address      =0;
	eeprom_93Cx6_state        =STATE_STANDBY;
	eeprom_93Cx6_lastCLK      =0;
	eeprom_93Cx6_writeEnabled =0;
	STATE_ADDRESS =wordSize ==16? STATE_ADDRESS16: STATE_ADDRESS8;
	STATE_DATA    =wordSize ==16? STATE_DATA16:    STATE_DATA8;   
	if (capacity ==128) {
		STATE_ADDRESS -=2;
		STATE_DATA -=2;
	}
}

uint8 eeprom_93Cx6_read () {
   return eeprom_93Cx6_output;
}

void eeprom_93Cx6_write (uint8 CS, uint8 CLK, uint8 DAT) {
	if (!CS && eeprom_93Cx6_state <= STATE_ADDRESS)
		eeprom_93Cx6_state =STATE_STANDBY;
	else
	if (eeprom_93Cx6_state ==STATE_STANDBY && CS && CLK && !eeprom_93Cx6_lastCLK) {
		if (DAT) eeprom_93Cx6_state =STATE_STARTBIT;
		eeprom_93Cx6_opcode  =0;
		eeprom_93Cx6_address =0;
		eeprom_93Cx6_output  =1;
	} else
	if (CLK && !eeprom_93Cx6_lastCLK && eeprom_93Cx6_state >=STATE_STARTBIT) {
		if (eeprom_93Cx6_state >=STATE_STARTBIT && eeprom_93Cx6_state <STATE_OPCODE) 
			eeprom_93Cx6_opcode  =(eeprom_93Cx6_opcode  <<1) | (DAT? 1: 0);
		else
		if (eeprom_93Cx6_state >=STATE_OPCODE   && eeprom_93Cx6_state <STATE_ADDRESS)
			eeprom_93Cx6_address =(eeprom_93Cx6_address <<1) | (DAT? 1: 0);
		else
		if (eeprom_93Cx6_state >=STATE_ADDRESS  && eeprom_93Cx6_state <STATE_DATA) {
			if (eeprom_93Cx6_opcode ==OPCODE_WRITE || eeprom_93Cx6_opcode ==OPCODE_WRITEALL)
				eeprom_93Cx6_data =(eeprom_93Cx6_data    <<1) | (DAT? 1: 0);
			else
			if (eeprom_93Cx6_opcode ==OPCODE_READ) {
				eeprom_93Cx6_output =eeprom_93Cx6_wordSize ==16? !!(eeprom_93Cx6_data &0x8000): !!(eeprom_93Cx6_data &0x80);
				eeprom_93Cx6_data   =   eeprom_93Cx6_data <<1;
			}
		}
		eeprom_93Cx6_state++;
		if (eeprom_93Cx6_state ==STATE_ADDRESS) {
			switch (eeprom_93Cx6_opcode) {
				case OPCODE_MISC:
					eeprom_93Cx6_opcode =(eeprom_93Cx6_address >>(eeprom_93Cx6_wordSize ==16? 6: 7)) + 10;
					switch (eeprom_93Cx6_opcode) {
						case OPCODE_WRITEDISABLE:
							eeprom_93Cx6_writeEnabled = 0;
							eeprom_93Cx6_state = STATE_FINISHED;
							break;
						case OPCODE_WRITEENABLE: 
							eeprom_93Cx6_writeEnabled = 1;
							eeprom_93Cx6_state = STATE_FINISHED;
							break;
						case OPCODE_ERASEALL:
							if (eeprom_93Cx6_writeEnabled) {
								int i;
								for (i =0; i <eeprom_93Cx6_capacity; i++)
									eeprom_93Cx6_storage[i] = 0xFF;
								}
							eeprom_93Cx6_state = STATE_FINISHED;
							break;
						case OPCODE_WRITEALL:
							eeprom_93Cx6_address = 0;
							break;
					}
					break;
				case OPCODE_ERASE:
					if (eeprom_93Cx6_writeEnabled) {
						if (eeprom_93Cx6_wordSize ==16) {
							eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |0] = 0xFF;
							eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |1] = 0xFF;
						} else
							eeprom_93Cx6_storage[eeprom_93Cx6_address] = 0xFF;
					}
					eeprom_93Cx6_state = STATE_FINISHED;
					break;
				case OPCODE_READ:
					if (eeprom_93Cx6_wordSize ==16) {
						eeprom_93Cx6_data = eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |0] | eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |1] <<8;
						eeprom_93Cx6_address++;
					} else
						eeprom_93Cx6_data = eeprom_93Cx6_storage[eeprom_93Cx6_address++];
					break;
			}
		} else
		if (eeprom_93Cx6_state ==STATE_DATA) {
			if (eeprom_93Cx6_opcode ==OPCODE_WRITE) {
				if (eeprom_93Cx6_wordSize ==16) {
					eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |0] = eeprom_93Cx6_data &0xFF;
					eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |1] = eeprom_93Cx6_data >>8;
					eeprom_93Cx6_address++;
				} else
					eeprom_93Cx6_storage[eeprom_93Cx6_address++] = eeprom_93Cx6_data;
				eeprom_93Cx6_state = STATE_FINISHED;
			} else
			if (eeprom_93Cx6_opcode ==OPCODE_WRITEALL) {
				if (eeprom_93Cx6_wordSize ==16) {
					eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |0] = eeprom_93Cx6_data &0xFF;
					eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |1] = eeprom_93Cx6_data >>8;
					eeprom_93Cx6_address++;
				} else
					eeprom_93Cx6_storage[eeprom_93Cx6_address++] = eeprom_93Cx6_data;
				eeprom_93Cx6_state = CS && eeprom_93Cx6_address <eeprom_93Cx6_capacity? STATE_ADDRESS: STATE_FINISHED;
			} else
			if (eeprom_93Cx6_opcode ==OPCODE_READ) {
				if (eeprom_93Cx6_address <eeprom_93Cx6_capacity) {
					if (eeprom_93Cx6_wordSize ==16)
						eeprom_93Cx6_data = eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |0] | eeprom_93Cx6_storage[eeprom_93Cx6_address <<1 |1] <<8;
					else
						eeprom_93Cx6_data = eeprom_93Cx6_storage[eeprom_93Cx6_address];
				}
				eeprom_93Cx6_state = CS && ++eeprom_93Cx6_address <=eeprom_93Cx6_capacity? STATE_ADDRESS: STATE_FINISHED;
			}
		}
		if (eeprom_93Cx6_state == STATE_FINISHED) {
			eeprom_93Cx6_output = 0;
			eeprom_93Cx6_state = STATE_STANDBY;
		}
	}
	if (eeprom_93Cx6_opcode ==OPCODE_READ && eeprom_93Cx6_state ==(STATE_ADDRESS -2)) eeprom_93Cx6_output =0; /* Dummy Zero */
	eeprom_93Cx6_lastCLK = CLK;
}
