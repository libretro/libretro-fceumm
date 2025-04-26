#ifndef _FLASHROM_H
#define _FLASHROM_H
#include "mapinc.h"

extern uint8		flashrom_manufacturerID;
extern uint8		flashrom_modelID;
extern int		flashrom_state;
extern uint32		flashrom_sectorSize;
extern int             flashrom_timeOut;
extern uint32		flashrom_magicAddr1;
extern uint32		flashrom_magicAddr2;
extern uint32		flashrom_magicMask;

void            flashrom_init (uint8, uint8, uint32, uint32, uint32, uint32);
uint8		flashrom_read (uint32);
void		flashrom_write (uint32, uint8);
void FP_FASTAPASS(1) flashrom_cpuCycle(int);

#endif
