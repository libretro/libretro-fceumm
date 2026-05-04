#ifndef _FCEU_FDS_H
#define _FCEU_FDS_H

#include "file.h"

uint8_t *FDSROM_ptr(void);
uint32_t FDSROM_size(void);

void FCEU_FDSInsert(int oride);
void FCEU_FDSEject(void);
void FCEU_FDSSelect(void);

int FDSLoad(const char *name, FCEUFILE *fp);

#endif
