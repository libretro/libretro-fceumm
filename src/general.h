#ifndef _FCEU_GENERAL_H
#define _FCEU_GENERAL_H

extern uint32_t uppow2(uint32_t n);

char *FCEU_MakeFName(int type, int id1, const char *cd1);

enum FILE_TYPES
{
   FCEUMKF_FDSROM = 1,
   FCEUMKF_PALETTE,
   FCEUMKF_GGROM
};

#endif
