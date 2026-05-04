#ifndef _FCEU_INPUT_H
#define _FCEU_INPUT_H

typedef struct {
	uint8_t FP_FASTAPASS(1) (*Read)(int w);
	void FP_FASTAPASS(1) (*Write)(uint8_t v);
	void FP_FASTAPASS(1) (*Strobe)(int w);
	void FP_FASTAPASS(3) (*Update)(int w, void *data, int arg);
	void FP_FASTAPASS(3) (*SLHook)(int w, uint8_t *bg, uint8_t *spr, uint32_t linets, int final);
	void FP_FASTAPASS(3) (*Draw)(int w, uint8_t *buf, int arg);
} INPUTC;

typedef struct {
	uint8_t FP_FASTAPASS(2) (*Read)(int w, uint8_t ret);
	void FP_FASTAPASS(1) (*Write)(uint8_t v);
	void (*Strobe)(void);
	void FP_FASTAPASS(2) (*Update)(void *data, int arg);
	void FP_FASTAPASS(3) (*SLHook)(uint8_t *bg, uint8_t *spr, uint32_t linets, int final);
	void FP_FASTAPASS(2) (*Draw)(uint8_t *buf, int arg);
} INPUTCFC;

void FCEU_DrawInput(uint8_t *buf);
void FCEU_UpdateInput(void);
void InitializeInput(void);
extern void (*PStrobe[2])(void);
extern void (*InputScanlineHook)(uint8_t *bg, uint8_t *spr, uint32_t linets, int final);

void FCEU_DoSimpleCommand(int cmd);

#endif
