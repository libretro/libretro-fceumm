#ifndef _FCEU_GENERAL_H
#define _FCEU_GENERAL_H

void GetFileBase(const char *f);
extern uint32 uppow2(uint32 n);

char *FCEU_MakeFName(int type, int id1, char *cd1);

enum FILE_TYPES
{
    FCEUMKF_FDSROM = 1,
    FCEUMKF_PALETTE,
    FCEUMKF_GGROM,
    FCEUMKF_FDS
};

#endif
