#ifndef _FCEU_PPU_H
#define _FCEU_PPU_H

typedef struct {
	/* overclock the console by adding dummy scanlines to PPU loop
	 * disables DMC DMA and WaveHi filling for these dummies
	 * doesn't work with new PPU */
	int overclock_enabled;
	int overclocked;
	int skip_7bit_overclocking;
	int totalscanlines;
	int normal_scanlines;
	int extrascanlines;
	int vblankscanlines;
} PPU_T;

extern PPU_T ppu;

void FCEUPPU_Init(void);
void FCEUPPU_Reset(void);
void FCEUPPU_Power(void);
int FCEUPPU_Loop(int skip);

void FCEUPPU_LineUpdate(void);
void FCEUPPU_SetVideoSystem(int w);

extern void (*GameHBIRQHook)(void), (*GameHBIRQHook2)(void);
extern void (*PPU_hook)(uint32 A);

/* For cart.c and banksw.h, mostly */
extern uint8 NTARAM[0x800], *vnapage[4];
extern uint8 PPUNTARAM;
extern uint8 PPUCHRRAM;

void FCEUPPU_SaveState(void);
void FCEUPPU_LoadState(int version);

extern int scanline;
extern uint8 PPU[4];

#endif
