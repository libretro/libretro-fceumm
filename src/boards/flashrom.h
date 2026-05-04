#ifndef _FLASHROM_H
#define _FLASHROM_H
#include "mapinc.h"

extern uint8_t		flashrom_manufacturerID;
extern uint8_t		flashrom_modelID;
extern int		flashrom_state;
extern uint32_t		flashrom_sectorSize;
extern int             flashrom_timeOut;
extern uint32_t		flashrom_magicAddr1;
extern uint32_t		flashrom_magicAddr2;
extern uint32_t		flashrom_magicMask;

void            flashrom_init (uint8_t, uint8_t, uint32_t, uint32_t, uint32_t, uint32_t);
uint8_t		flashrom_read (uint32_t);
void		flashrom_write (uint32_t, uint8_t);
void FP_FASTAPASS(1) flashrom_cpuCycle(int);

#endif
