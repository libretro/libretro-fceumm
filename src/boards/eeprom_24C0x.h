#ifndef _EEPROM_24C0X_H
#define _EEPROM_24C0X_H
#include "mapinc.h"

extern uint8_t x24c0x_data[512]; /* 24C01 accesses +0x00, 24C02 accesses +0x100 */
extern uint8_t x24c01_out;
extern uint8_t x24c02_out;
extern SFORMAT x24c01StateRegs[];
extern SFORMAT x24c02StateRegs[];
void x24c01_init(void);
void x24c02_init(void);
void x24c01_write(uint8_t scl, uint8_t sda);
void x24c02_write(uint8_t scl, uint8_t sda);
#endif
