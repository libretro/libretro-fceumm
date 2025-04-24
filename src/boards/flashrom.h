#ifndef _FLASHROM_H
#define _FLASHROM_H
#include "mapinc.h"

uint8		flashrom_manufacturerID;
uint8		flashrom_modelID;
int		flashrom_state;
uint32		flashrom_sectorSize;
int             flashrom_timeOut;
uint32		flashrom_magicAddr1;
uint32		flashrom_magicAddr2;
uint32		flashrom_magicMask;

void            flashrom_init (uint8, uint8, uint32, uint32, uint32, uint32);
uint8		flashrom_read (uint32);
void		flashrom_write (uint32, uint8);
void FP_FASTAPASS(1) flashrom_cpuCycle(int);

#endif
