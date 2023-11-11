#include "mapinc.h"
#include "eeprom_93Cx6.h"

static uint8* eeprom_93Cx6_storage;
static uint8  eeprom_93Cx6_opcode;
static uint16 eeprom_93Cx6_data;
static uint16 eeprom_93Cx6_address;
static uint8  eeprom_93Cx6_state;
static uint8  eeprom_93Cx6_lastCLK;
static uint8  eeprom_93Cx6_writeEnabled;
static uint8  eeprom_93Cx6_output;
static uint8  eeprom_93Cx6_capacity;
static uint8  eeprom_93Cx6_wordsize;
static uint8  eeprom_93Cx6_state_address;
static uint8  eeprom_93Cx6_state_data;

#define OPCODE_MISC         0
#define OPCODE_WRITE        1
#define OPCODE_READ         2
#define OPCODE_ERASE        3
#define OPCODE_WRITEDISABLE 10
#define OPCODE_WRITEALL     11
#define OPCODE_ERASEALL     12
#define OPCODE_WRITEENABLE  13

#define STATE_STANDBY       0
#define STATE_STARTBIT      1
#define STATE_OPCODE        3
#define STATE_ADDRESS8      12
#define STATE_DATA8         20
#define STATE_ADDRESS16     11
#define STATE_DATA16        27
#define STATE_FINISHED      99

void eeprom_93Cx6_init(uint8 *data, uint32 capacity, uint8 wordsize) {
	eeprom_93Cx6_storage = data;
	eeprom_93Cx6_address = 0;
	eeprom_93Cx6_state = STATE_STANDBY;
	eeprom_93Cx6_lastCLK = FALSE;
	eeprom_93Cx6_writeEnabled = FALSE;
	eeprom_93Cx6_capacity = capacity;
	eeprom_93Cx6_wordsize = wordsize;

	eeprom_93Cx6_state_address = (wordsize == 16) ? STATE_ADDRESS16 : STATE_ADDRESS8;
	eeprom_93Cx6_state_data = (wordsize == 16) ? STATE_DATA16 : STATE_DATA8;
}

uint8 eeprom_93Cx6_read(void) {
	return eeprom_93Cx6_output;
}

void eeprom_93Cx6_write(uint8 CS, uint8 CLK, uint8 DAT) {
	if (!CS && (eeprom_93Cx6_state <= eeprom_93Cx6_state_address)) {
		eeprom_93Cx6_state = STATE_STANDBY;
	} else if ((eeprom_93Cx6_state == STATE_STANDBY) && CS && CLK && !eeprom_93Cx6_lastCLK) {
		eeprom_93Cx6_state = STATE_STARTBIT;
		eeprom_93Cx6_opcode = 0;
		eeprom_93Cx6_address = 0;
		eeprom_93Cx6_output = TRUE;
	} else if (CLK && !eeprom_93Cx6_lastCLK) {
		if ((eeprom_93Cx6_state >= STATE_STARTBIT) && (eeprom_93Cx6_state < STATE_OPCODE)) {
			eeprom_93Cx6_opcode = (eeprom_93Cx6_opcode << 1) | (DAT ? 1 : 0);
		} else if ((eeprom_93Cx6_state >= STATE_OPCODE) && (eeprom_93Cx6_state < eeprom_93Cx6_state_address)) {
			eeprom_93Cx6_address = (eeprom_93Cx6_address << 1) | (DAT ? 1 : 0);
		} else if ((eeprom_93Cx6_state >= eeprom_93Cx6_state_address) && (eeprom_93Cx6_state < eeprom_93Cx6_state_data)) {
			if ((eeprom_93Cx6_opcode == OPCODE_WRITE) || (eeprom_93Cx6_opcode == OPCODE_WRITEALL)) {
				eeprom_93Cx6_data = (eeprom_93Cx6_data << 1) | (DAT ? 1 : 0);
			} else if (eeprom_93Cx6_opcode == OPCODE_READ) {
				if (eeprom_93Cx6_wordsize == 16) {
					eeprom_93Cx6_output = (eeprom_93Cx6_data & 0x8000) != 0;
				} else {
					eeprom_93Cx6_output = (eeprom_93Cx6_data & 0x80) != 0;
				}
				eeprom_93Cx6_data = eeprom_93Cx6_data << 1;
			}
		}

		eeprom_93Cx6_state++;
		if (eeprom_93Cx6_state == eeprom_93Cx6_state_address) {
			switch (eeprom_93Cx6_opcode) {
			case OPCODE_MISC:
				if (eeprom_93Cx6_wordsize == 16) {
					eeprom_93Cx6_opcode = (eeprom_93Cx6_address >> 6) + 10;
				} else {
					eeprom_93Cx6_opcode = (eeprom_93Cx6_address >> 7) + 10;
				}
				switch (eeprom_93Cx6_opcode) {
				case OPCODE_WRITEDISABLE:
					eeprom_93Cx6_writeEnabled = FALSE;
					eeprom_93Cx6_state = STATE_FINISHED;
					break;
				case OPCODE_WRITEENABLE:
					eeprom_93Cx6_writeEnabled = TRUE;
					eeprom_93Cx6_state = STATE_FINISHED;
					break;
				case OPCODE_ERASEALL:
					if (eeprom_93Cx6_writeEnabled) {
						int i;

						for (i = 0; i < eeprom_93Cx6_capacity; i++) {
							eeprom_93Cx6_storage[i] = 0xFF;
						}
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
					if (eeprom_93Cx6_wordsize == 16) {
						eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 0] = 0xFF;
						eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 1] = 0xFF;
					} else {
						eeprom_93Cx6_storage[eeprom_93Cx6_address] = 0xFF;
					}
				}
				eeprom_93Cx6_state = STATE_FINISHED;
				break;
			case OPCODE_READ:
				if (eeprom_93Cx6_wordsize == 16) {
					eeprom_93Cx6_data = eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 0];
					eeprom_93Cx6_data |= (eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 1] << 8);
					eeprom_93Cx6_address++;
				} else {
					eeprom_93Cx6_data = eeprom_93Cx6_storage[eeprom_93Cx6_address++];
				}
				break;
			}
		} else if (eeprom_93Cx6_state == eeprom_93Cx6_state_data) {
			if (eeprom_93Cx6_opcode == OPCODE_WRITE) {
				if (eeprom_93Cx6_wordsize == 16) {
					eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 0] = eeprom_93Cx6_data & 0xFF;
					eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 1] = eeprom_93Cx6_data >> 8;
					eeprom_93Cx6_address++;
				} else {
					eeprom_93Cx6_storage[eeprom_93Cx6_address++] = eeprom_93Cx6_data;
				}
				eeprom_93Cx6_state = STATE_FINISHED;
			} else if (eeprom_93Cx6_opcode == OPCODE_WRITEALL) {
				if (eeprom_93Cx6_wordsize == 16) {
					eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 0] = eeprom_93Cx6_data & 0xFF;
					eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 1] = eeprom_93Cx6_data >> 8;
					eeprom_93Cx6_address++;
				} else {
					eeprom_93Cx6_storage[eeprom_93Cx6_address++] = eeprom_93Cx6_data;
				}
				eeprom_93Cx6_state = (CS && (eeprom_93Cx6_address < eeprom_93Cx6_capacity)) ? eeprom_93Cx6_state_address : STATE_FINISHED;
			} else if (eeprom_93Cx6_opcode == OPCODE_READ) {
				if (eeprom_93Cx6_address < eeprom_93Cx6_capacity) {
					if (eeprom_93Cx6_wordsize == 16) {
						eeprom_93Cx6_data = eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 0];
						eeprom_93Cx6_data |= (eeprom_93Cx6_storage[(eeprom_93Cx6_address << 1) | 1] << 8);
					} else {
						eeprom_93Cx6_data = eeprom_93Cx6_storage[eeprom_93Cx6_address];
					}
				}
				eeprom_93Cx6_state = (CS && (++eeprom_93Cx6_address <= eeprom_93Cx6_capacity)) ? eeprom_93Cx6_state_address : STATE_FINISHED;
			}
		}
		if (eeprom_93Cx6_state == STATE_FINISHED) {
			eeprom_93Cx6_output = FALSE;
			eeprom_93Cx6_state = STATE_STANDBY;
		}
	}
	eeprom_93Cx6_lastCLK = CLK;
}
