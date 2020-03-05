#ifndef _FCEU_FDS_H
#define _FCEU_FDS_H

uint8 *FDSROM_ptr(void);
uint32 FDSROM_size(void);

void FCEU_FDSInsert(int oride);
void FCEU_FDSEject(void);
void FCEU_FDSSelect(void);

#endif
