#ifndef FDS_APU_H
#define FDS_APU_H

#include "fceu-types.h"

void FDSSoundReset(void);
void FDSSoundStateAdd(void);

/* Used for FDS conversion-based mappers to allow access to FDS APU registers */
void FDSSoundPower(void); 
void FDSSoundWrite(uint32_t A, uint8_t V); /* $4040-$407F, $4080-$408A */

#endif /* FDS_APU_H */
