#include "mapinc.h"
#include "eeprom_93Cx6.h"

struct EEPROM_93Cx6 {
	uint8 *storage;
	uint8 opcode;
	uint16 data;
	uint16 address;
	uint8 state;
	uint8 lastCLK;
	uint8 writeEnabled;
	uint8 output;
	uint8 capacity;
	uint8 wordsize;
	uint8 state_address;
	uint8 state_data;
} EEPROM_93Cx6;

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

static struct EEPROM_93Cx6 eeprom_93Cx6 = { 0 };

void eeprom_93Cx6_init(uint8 *data, uint32 capacity, uint8 wordsize) {
	eeprom_93Cx6.storage = data;
	eeprom_93Cx6.address = 0;
	eeprom_93Cx6.state = STATE_STANDBY;
	eeprom_93Cx6.lastCLK = FALSE;
	eeprom_93Cx6.writeEnabled = FALSE;
	eeprom_93Cx6.capacity = capacity;
	eeprom_93Cx6.wordsize = wordsize;

	eeprom_93Cx6.state_address = (wordsize == 16) ? STATE_ADDRESS16 : STATE_ADDRESS8;
	eeprom_93Cx6.state_data = (wordsize == 16) ? STATE_DATA16 : STATE_DATA8;
}

uint8 eeprom_93Cx6_read(void) {
	return eeprom_93Cx6.output;
}

void eeprom_93Cx6_write(uint8 CS, uint8 CLK, uint8 DAT) {
	if (!CS && (eeprom_93Cx6.state <= eeprom_93Cx6.state_address)) {
		eeprom_93Cx6.state = STATE_STANDBY;
	} else if ((eeprom_93Cx6.state == STATE_STANDBY) && CS && CLK && !eeprom_93Cx6.lastCLK) {
		eeprom_93Cx6.state = STATE_STARTBIT;
		eeprom_93Cx6.opcode = 0;
		eeprom_93Cx6.address = 0;
		eeprom_93Cx6.output = TRUE;
	} else if (CLK && !eeprom_93Cx6.lastCLK) {
		if ((eeprom_93Cx6.state >= STATE_STARTBIT) && (eeprom_93Cx6.state < STATE_OPCODE)) {
			eeprom_93Cx6.opcode = (eeprom_93Cx6.opcode << 1) | (DAT ? 1 : 0);
		} else if ((eeprom_93Cx6.state >= STATE_OPCODE) && (eeprom_93Cx6.state < eeprom_93Cx6.state_address)) {
			eeprom_93Cx6.address = (eeprom_93Cx6.address << 1) | (DAT ? 1 : 0);
		} else if ((eeprom_93Cx6.state >= eeprom_93Cx6.state_address) && (eeprom_93Cx6.state < eeprom_93Cx6.state_data)) {
			if ((eeprom_93Cx6.opcode == OPCODE_WRITE) || (eeprom_93Cx6.opcode == OPCODE_WRITEALL)) {
				eeprom_93Cx6.data = (eeprom_93Cx6.data << 1) | (DAT ? 1 : 0);
			} else if (eeprom_93Cx6.opcode == OPCODE_READ) {
				if (eeprom_93Cx6.wordsize == 16) {
					eeprom_93Cx6.output = (eeprom_93Cx6.data & 0x8000) != 0;
				} else {
					eeprom_93Cx6.output = (eeprom_93Cx6.data & 0x80) != 0;
				}
				eeprom_93Cx6.data = eeprom_93Cx6.data << 1;
			}
		}

		eeprom_93Cx6.state++;
		if (eeprom_93Cx6.state == eeprom_93Cx6.state_address) {
			switch (eeprom_93Cx6.opcode) {
			case OPCODE_MISC:
				if (eeprom_93Cx6.wordsize == 16) {
					eeprom_93Cx6.opcode = (eeprom_93Cx6.address >> 6) + 10;
				} else {
					eeprom_93Cx6.opcode = (eeprom_93Cx6.address >> 7) + 10;
				}
				switch (eeprom_93Cx6.opcode) {
				case OPCODE_WRITEDISABLE:
					eeprom_93Cx6.writeEnabled = FALSE;
					eeprom_93Cx6.state = STATE_FINISHED;
					break;
				case OPCODE_WRITEENABLE:
					eeprom_93Cx6.writeEnabled = TRUE;
					eeprom_93Cx6.state = STATE_FINISHED;
					break;
				case OPCODE_ERASEALL:
					if (eeprom_93Cx6.writeEnabled) {
						int i;

						for (i = 0; i < eeprom_93Cx6.capacity; i++) {
							eeprom_93Cx6.storage[i] = 0xFF;
						}
					}
					eeprom_93Cx6.state = STATE_FINISHED;
					break;
				case OPCODE_WRITEALL:
					eeprom_93Cx6.address = 0;
					break;
				}
				break;
			case OPCODE_ERASE:
				if (eeprom_93Cx6.writeEnabled) {
					if (eeprom_93Cx6.wordsize == 16) {
						eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 0] = 0xFF;
						eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 1] = 0xFF;
					} else {
						eeprom_93Cx6.storage[eeprom_93Cx6.address] = 0xFF;
					}
				}
				eeprom_93Cx6.state = STATE_FINISHED;
				break;
			case OPCODE_READ:
				if (eeprom_93Cx6.wordsize == 16) {
					eeprom_93Cx6.data = eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 0];
					eeprom_93Cx6.data |= (eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 1] << 8);
					eeprom_93Cx6.address++;
				} else {
					eeprom_93Cx6.data = eeprom_93Cx6.storage[eeprom_93Cx6.address++];
				}
				break;
			}
		} else if (eeprom_93Cx6.state == eeprom_93Cx6.state_data) {
			if (eeprom_93Cx6.opcode == OPCODE_WRITE) {
				if (eeprom_93Cx6.wordsize == 16) {
					eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 0] = eeprom_93Cx6.data & 0xFF;
					eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 1] = eeprom_93Cx6.data >> 8;
					eeprom_93Cx6.address++;
				} else {
					eeprom_93Cx6.storage[eeprom_93Cx6.address++] = eeprom_93Cx6.data;
				}
				eeprom_93Cx6.state = STATE_FINISHED;
			} else if (eeprom_93Cx6.opcode == OPCODE_WRITEALL) {
				if (eeprom_93Cx6.wordsize == 16) {
					eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 0] = eeprom_93Cx6.data & 0xFF;
					eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 1] = eeprom_93Cx6.data >> 8;
					eeprom_93Cx6.address++;
				} else {
					eeprom_93Cx6.storage[eeprom_93Cx6.address++] = eeprom_93Cx6.data;
				}
				eeprom_93Cx6.state = (CS && (eeprom_93Cx6.address < eeprom_93Cx6.capacity)) ? eeprom_93Cx6.state_address : STATE_FINISHED;
			} else if (eeprom_93Cx6.opcode == OPCODE_READ) {
				if (eeprom_93Cx6.address < eeprom_93Cx6.capacity) {
					if (eeprom_93Cx6.wordsize == 16) {
						eeprom_93Cx6.data = eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 0];
						eeprom_93Cx6.data |= (eeprom_93Cx6.storage[(eeprom_93Cx6.address << 1) | 1] << 8);
					} else {
						eeprom_93Cx6.data = eeprom_93Cx6.storage[eeprom_93Cx6.address];
					}
				}
				eeprom_93Cx6.state = (CS && (++eeprom_93Cx6.address <= eeprom_93Cx6.capacity)) ? eeprom_93Cx6.state_address : STATE_FINISHED;
			}
		}
		if (eeprom_93Cx6.state == STATE_FINISHED) {
			eeprom_93Cx6.output = FALSE;
			eeprom_93Cx6.state = STATE_STANDBY;
		}
	}
	eeprom_93Cx6.lastCLK = CLK;
}
