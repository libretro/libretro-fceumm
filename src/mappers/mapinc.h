#ifndef _MAPINC_H
#define _MAPINC_H

#include "../fceu-types.h"
#include "../x6502.h"
#include "../fceu.h"
#include "../ppu.h"
#include "../fceu-memory.h"
#include "../sound.h"
#include "../state.h"
#include "../cart.h"
#include "../cheat.h"
#include "../unif.h"
#include "../ines.h"
#include "../nsf.h"
#include "../x6502.h"
#include <string.h>
#include <stdlib.h>

extern uint8  *WRAM;
extern uint32 WRAMSIZE;

extern uint8  *CHRRAM;
extern uint32 CHRRAMSIZE;

extern uint8  *miscROM;
extern uint32 miscROMSIZE;

#define CPU_OPENBUS X.DB

#define PRG_PAGE_SIZE     (16 * 1024)
#define CHR_PAGE_SIZE     ( 8 * 1024)

#define PRG_ROM_PTR       ROM
#define PRG_ROM_SIZE      (ROM_size * PRG_PAGE_SIZE)

#define CHR_ROM_PTR       VROM
#define CHR_ROM_SIZE      (VROM_size * CHR_PAGE_SIZE)

#define PRG_BANK_COUNT(x) (PRG_ROM_SIZE / ((x) * 1024))
#define CHR_BANK_COUNT(x) (CHR_ROM_SIZE / ((x) * 1024))

#define MISC_ROM_PTR      miscROM
#define MISC_ROM_SIZE     miscROMSIZE

#endif /* _MAPINC_H */
