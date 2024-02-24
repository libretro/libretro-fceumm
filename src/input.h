#ifndef _FCEU_INPUT_H
#define _FCEU_INPUT_H

typedef struct {
	uint8 (*Read)(int w);
	void (*Write)(uint8 v);
	void (*Strobe)(int w);
	void (*Update)(int w, void *data, int arg);
	void (*SLHook)(int w, uint8 *bg, uint8 *spr, uint32 linets, int final);
	void (*Draw)(int w, uint8 *buf, int arg);
} INPUTC;

typedef struct {
	uint8 (*Read)(int w, uint8 ret);
	void (*Write)(uint8 v);
	void (*Strobe)(void);
	void (*Update)(void *data, int arg);
	void (*SLHook)(uint8 *bg, uint8 *spr, uint32 linets, int final);
	void (*Draw)(uint8 *buf, int arg);
} INPUTCFC;

uint8 FCEU_GetJoyJoy(void);

void FCEU_DrawInput(uint8 *buf);
void FCEU_UpdateInput(void);
void FCEUINPUT_Power(void);

extern void (*PStrobe[2])(void);
void InputScanlineHook(uint8 *bg, uint8 *spr, uint32 linets, int final);

void FCEU_DoSimpleCommand(int cmd);

void FCEU_ZapperSetTolerance(int t);
void FCEU_ZapperSetSTMode(int mode);
void FCEU_ZapperInvertTrigger(int invert);
void FCEU_ZapperInvertSensor(int invert);

#endif
