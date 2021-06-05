#ifndef _FCEU_PPU_H
#define _FCEU_PPU_H

typedef struct {
    /* PPU Overclocking */
	int overclock_enabled;		/* 1 : enable, 0 : disable duh! */
	int skip_7bit_overclocking;	/* 7-bit samples have priority over overclocking */
	int extrascanlines;
	int vblankscanlines;
	int normal_scanlines;		/* 240 : normal, 290 : dendy */
	int totalscanlines;
} FCEU_PPU;

extern FCEU_PPU ppu;

void FCEUPPU_Init(void);
void FCEUPPU_Reset(void);
void FCEUPPU_Power(void);
int FCEUPPU_Loop(int skip);

void FCEUPPU_LineUpdate();
void FCEUPPU_SetVideoSystem(int w);

extern void (*GameHBIRQHook)(void), (*GameHBIRQHook2)(void);
extern void FP_FASTAPASS(1) (*PPU_hook)(uint32 A);

/* For cart.c and banksw.h, mostly */
extern uint8 NTARAM[0x800], *vnapage[4];
extern uint8 PPUNTARAM;
extern uint8 PPUCHRRAM;
extern uint8 PALRAM[0x20];
extern uint8 SPRAM[0x100];

void FCEUPPU_SaveState(void);
void FCEUPPU_LoadState(int version);

extern int scanline;
extern uint8 PPU[4];

#endif
