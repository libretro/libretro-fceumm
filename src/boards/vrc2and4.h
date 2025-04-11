#ifndef _VRC2AND4_H
#define _VRC2AND4_H

extern int    VRC24_A0, VRC24_A1;
extern uint8  VRC24_isVRC4;
extern uint8  VRC24_useRepeatBit;
extern uint8  VRC24_prg[2];
extern uint16 VRC24_chr[8];
extern uint8  VRC24_mirroring;
extern uint8  VRC24_misc;
extern uint8  VRC2_pins;
extern uint8  VRC4_latch;
extern uint8  VRC4_mode;
extern uint8  VRC4_count;
extern signed short int VRC4_cycles;

extern int    (*VRC24_GetPRGBank)(int);
extern int    (*VRC24_GetCHRBank)(int);
extern void   (*VRC24_Sync)();
extern DECLFW((*VRC24_ExternalSelect));
extern DECLFR((*VRC24_WRAMRead));
extern DECLFW((*VRC24_WRAMWrite));

void VRC24_syncPRG    (int AND, int OR);
void VRC24_syncCHR    (int AND, int OR);
void VRC24_syncWRAM   (int OR);
int  VRC24_getPRGBank (int bank);
int  VRC24_getCHRBank (int bank);
void VRC24_syncMirror ();
DECLFW(VRC24_writeReg);

void VRC24_power(void);
void VRC24_close(void);
void VRC24_init(CartInfo *info, void (*_sync)(), int _A0, int _A1, uint8 _isVRC4, uint8 _useRepeatBit, uint8 defaultWRAMSizeKiB);

#endif
