#ifndef _EEPROM_93CX6_H
#define _EEPROM_93CX6_H
#include "mapinc.h"

extern uint8* eeprom_93Cx6_storage;

void  eeprom_93Cx6_init  (size_t capacity, uint8 wordSize);
uint8 eeprom_93Cx6_read  ();
void  eeprom_93Cx6_write (uint8 CS, uint8 CLK, uint8 DAT);
#endif
