#ifndef _EEPROM_93CX6_H
#define _EEPROM_93CX6_H
#include "mapinc.h"

extern uint8_t* eeprom_93Cx6_storage;

void  eeprom_93Cx6_init  (size_t capacity, uint8_t wordSize);
uint8_t eeprom_93Cx6_read  ();
void  eeprom_93Cx6_write (uint8_t CS, uint8_t CLK, uint8_t DAT);
#endif
