/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <string.h>

#include "fceu-types.h"
#include "x6502.h"

#include "fceu.h"
#include "netplay.h"
#include "movie.h"

#include "input.h"
#include "vsuni.h"
#include "fds.h"

extern INPUTC *FCEU_InitJoyPad(int w);
extern INPUTC *FCEU_InitZapper(int w);
extern INPUTC *FCEU_InitMouse(int w);
extern INPUTC *FCEU_InitPowerpadA(int w);
extern INPUTC *FCEU_InitPowerpadB(int w);
extern INPUTC *FCEU_InitArkanoid(int w);

extern INPUTCFC *FCEU_InitFami4(void);
extern INPUTCFC *FCEU_InitArkanoidFC(void);
extern INPUTCFC *FCEU_InitSpaceShadow(void);
extern INPUTCFC *FCEU_InitFKB(void);
extern INPUTCFC *FCEU_InitSuborKB(void);
extern INPUTCFC *FCEU_InitPEC586KB(void);
extern INPUTCFC *FCEU_InitHS(void);
extern INPUTCFC *FCEU_InitMahjong(void);
extern INPUTCFC *FCEU_InitQuizKing(void);
extern INPUTCFC *FCEU_InitFamilyTrainerA(void);
extern INPUTCFC *FCEU_InitFamilyTrainerB(void);
extern INPUTCFC *FCEU_InitOekaKids(void);
extern INPUTCFC *FCEU_InitTopRider(void);
extern INPUTCFC *FCEU_InitBarcodeWorld(void);

extern uint8 coinon;

static void *InputDataPtr[2];
uint8 LastStrobe;

static int JPAttrib[2] = { 0, 0 };
static int JPType[2] = { 0, 0 };
static INPUTC DummyJPort = { 0, 0, 0, 0, 0 };
static INPUTC *JPorts[2] = { &DummyJPort, &DummyJPort };

static int JPAttribFC = 0;
static int JPTypeFC = 0;
static void *InputDataPtrFC;
static INPUTCFC *FCExp = 0;

void (*InputScanlineHook)(uint8 *bg, uint8 *spr, uint32 linets, int final);

/*
#define DI      01
#define CLK     02
#define CS      04
#define OUT0    01
#define D3      01
#define D4      02

typedef struct {
	uint8 state;
	uint8 cmd;
	uint8 addr;
	uint8 iswritable;
	uint16 acc;
	uint16 data[128];
} EEPROM;

EEPROM serialROM[2];
uint8 oldCLK, bankFlip, DIFlip, OUT0state;

uint8 serialROMautomat(uint8 chip, uint16 data) {
	uint8 resp = 1;
	chip &= 1;
	if (!(data & CS)) {
		if (!(data & CLK)) {
			uint8 state = serialROM[chip].state;
			uint8 mask, i;
			FCEU_printf("> state = %02x\n", serialROM[chip].state);
			switch (serialROM[chip].cmd) {
			case 0x00:
				mask = ~(1 << (state & 7));
				if (state < 8) {
					serialROM[chip].addr &= mask;
					serialROM[chip].addr |= ((data & 1) << (state & 7));
					if (state == 7)
						FCEU_printf("> addr = %02x\n", serialROM[chip].addr);
				} else if (state < 15) {
					serialROM[chip].acc &= mask;
					serialROM[chip].acc |= ((data & 1) << (state & 7));
				} else {
					serialROM[chip].acc &= mask;
					serialROM[chip].acc |= ((data & 1) << (state & 7));
					serialROM[chip].cmd = serialROM[chip].acc;
					FCEU_printf("> cmd = %02x\n", serialROM[chip].cmd);
				}
				break;
			case 0x01:
				if (state < 30)
					resp = (serialROM[chip].data[serialROM[chip].addr] >> (state & 15)) & 1;
				else {
					resp = (serialROM[chip].data[serialROM[chip].addr] >> (state & 15)) & 1;
					serialROM[chip].cmd = 0;
				}
				break;
			case 0x06:
				mask = ~(1 << (state & 15));
				if (state < 30) {
					serialROM[chip].acc &= mask;
					serialROM[chip].acc |= ((data & 1) << (state & 15));
				} else {
					serialROM[chip].acc &= mask;
					serialROM[chip].acc |= ((data & 1) << (state & 15));
					if (serialROM[chip].iswritable)
						serialROM[chip].data[serialROM[chip].addr] = serialROM[chip].acc;
					serialROM[chip].cmd = 0;
				}
				break;
			case 0x0C:
				for (i = 0; i < 128; i++)
					serialROM[chip].data[i] = 0xFFFF;
				serialROM[chip].cmd = 0;
				resp = 1;
				break;
			case 0x0D:
				serialROM[chip].cmd = 0;
				resp = 1;
				break;
			case 0x09:
				serialROM[chip].cmd = 0;
				serialROM[chip].iswritable = 1;
				break;
			case 0x0B:
				serialROM[chip].cmd = 0;
				serialROM[chip].iswritable = 0;
				break;
			default:
				serialROM[chip].cmd = 0;
				serialROM[chip].state = 0;
				break;
			}
		} else {
			if (serialROM[chip].cmd == 0) {
				if (serialROM[chip].state > 15)
					serialROM[chip].state = 0;
			} else
				serialROM[chip].state++;
		}
	} else {
		serialROM[chip].cmd = 0;
		serialROM[chip].state = 0;
	}
	return resp;
}

uint8 serialROMstate(uint8 linestate) {
	uint8 answ = 0, newCLK = linestate & CLK;
	if ((!oldCLK) && newCLK) {
		DIFlip ^= 1;
		if (linestate && OUT0) {
			serialROMautomat(bankFlip, 04 + DIFlip);
			bankFlip ^= 1;
			serialROMautomat(bankFlip, 02 + DIFlip);
		}
	}
	answ = DIFlip ^ 1;
	answ |= (serialROMautomat(bankFlip, newCLK + DIFlip) << 1);
	oldCLK = newCLK;
	return answ << 3;
}*/

static DECLFR(JPRead) {
	uint8 ret = 0;

	if (JPorts[A & 1]->Read)
		ret |= JPorts[A & 1]->Read(A & 1);

	if (FCExp)
		if (FCExp->Read)
			ret = FCExp->Read(A & 1, ret);

	ret |= X.DB & 0xC0;

//  if(A==0x4017)
//{
//    ret |= serialROMstate(OUT0);
//    serialROMstate(OUT0|CLK);
//    FCEU_printf("> 4017 read %02x\n",ret);
//  }

	return(ret);
}

static DECLFW(B4016) {
//  OUT0state = V;
//  serialROMstate(OUT0state|CLK);
//  FCEU_printf("> 4016 write %02x\n",V);

	if (FCExp)
		if (FCExp->Write)
			FCExp->Write(V & 7);

	if (JPorts[0]->Write)
		JPorts[0]->Write(V & 1);
	if (JPorts[1]->Write)
		JPorts[1]->Write(V & 1);

	if ((LastStrobe & 1) && (!(V & 1))) {
//    FCEUD_UpdateInput();
		if (JPorts[0]->Strobe)
			JPorts[0]->Strobe(0);
		if (JPorts[1]->Strobe)
			JPorts[1]->Strobe(1);
		if (FCExp)
			if (FCExp->Strobe)
				FCExp->Strobe();
	}
	LastStrobe = V & 0x1;
}

void FCEU_DrawInput(uint8 *buf) {
	int x;
	for (x = 0; x < 2; x++)
		if (JPorts[x]->Draw)
			JPorts[x]->Draw(x, buf, JPAttrib[x]);
	if (FCExp)
		if (FCExp->Draw)
			FCExp->Draw(buf, JPAttribFC);
}

void FCEU_UpdateInput(void) {
	int x;

	for (x = 0; x < 2; x++) {
		if (JPorts[x]->Update)
			JPorts[x]->Update(x, InputDataPtr[x], JPAttrib[x]);
	}
	if (FCExp)
		if (FCExp->Update)
			FCExp->Update(InputDataPtrFC, JPAttribFC);

	if (GameInfo->type == GIT_VSUNI)
		if (coinon) coinon--;
}

static DECLFR(VSUNIRead0) {
	uint8 ret = 0;

	if (JPorts[0]->Read)
		ret |= (JPorts[0]->Read(0)) & 1;

	ret |= (vsdip & 3) << 3;
	if (coinon)
		ret |= 0x4;
	return ret;
}

static DECLFR(VSUNIRead1) {
	uint8 ret = 0;

	if (JPorts[1]->Read)
		ret |= (JPorts[1]->Read(1)) & 1;
	ret |= vsdip & 0xFC;
	return ret;
}

static void SLHLHook(uint8 *bg, uint8 *spr, uint32 linets, int final) {
	int x;
	for (x = 0; x < 2; x++)
		if (JPorts[x]->SLHook)
			JPorts[x]->SLHook(x, bg, spr, linets, final);
	if (FCExp)
		if (FCExp->SLHook)
			FCExp->SLHook(bg, spr, linets, final);
}

static void CheckSLHook(void) {
	InputScanlineHook = 0;
	if (JPorts[0]->SLHook || JPorts[1]->SLHook)
		InputScanlineHook = SLHLHook;
	if (FCExp)
		if (FCExp->SLHook)
			InputScanlineHook = SLHLHook;
}

static void FASTAPASS(1) SetInputStuff(int x) {
	switch (JPType[x]) {
	case SI_NONE:      JPorts[x] = &DummyJPort; break;
	case SI_GAMEPAD:   JPorts[x] = FCEU_InitJoyPad(x); break;
	case SI_ARKANOID:  JPorts[x] = FCEU_InitArkanoid(x); break;
	case SI_MOUSE:     JPorts[x] = FCEU_InitMouse(x); break;
	case SI_ZAPPER:    JPorts[x] = FCEU_InitZapper(x); break;
	case SI_POWERPADA: JPorts[x] = FCEU_InitPowerpadA(x); break;
	case SI_POWERPADB: JPorts[x] = FCEU_InitPowerpadB(x); break;
	}
	CheckSLHook();
}

static void SetInputStuffFC(void) {
	switch (JPTypeFC) {
	case SIFC_NONE:      FCExp = 0; break;
	case SIFC_ARKANOID:  FCExp = FCEU_InitArkanoidFC(); break;
	case SIFC_SHADOW:    FCExp = FCEU_InitSpaceShadow(); break;
	case SIFC_OEKAKIDS:  FCExp = FCEU_InitOekaKids(); break;
	case SIFC_4PLAYER:   FCExp = FCEU_InitFami4(); break;
	case SIFC_FKB:       FCExp = FCEU_InitFKB(); break;
	case SIFC_SUBORKB:   FCExp = FCEU_InitSuborKB(); break;
	case SIFC_PEC586KB:  FCExp = FCEU_InitPEC586KB(); break;
	case SIFC_HYPERSHOT: FCExp = FCEU_InitHS(); break;
	case SIFC_MAHJONG:   FCExp = FCEU_InitMahjong(); break;
	case SIFC_QUIZKING:  FCExp = FCEU_InitQuizKing(); break;
	case SIFC_FTRAINERA: FCExp = FCEU_InitFamilyTrainerA(); break;
	case SIFC_FTRAINERB: FCExp = FCEU_InitFamilyTrainerB(); break;
	case SIFC_BWORLD:    FCExp = FCEU_InitBarcodeWorld(); break;
	case SIFC_TOPRIDER:  FCExp = FCEU_InitTopRider(); break;
	}
	CheckSLHook();
}

void InitializeInput(void) {
	LastStrobe = 0;
	if (GameInfo->type == GIT_VSUNI) {
		SetReadHandler(0x4016, 0x4016, VSUNIRead0);
		SetReadHandler(0x4017, 0x4017, VSUNIRead1);
	} else
		SetReadHandler(0x4016, 0x4017, JPRead);

	SetWriteHandler(0x4016, 0x4016, B4016);

	SetInputStuff(0);
	SetInputStuff(1);
	SetInputStuffFC();
}

void FCEUI_SetInput(int port, int type, void *ptr, int attrib) {
	JPAttrib[port] = attrib;
	JPType[port] = type;
	InputDataPtr[port] = ptr;
	SetInputStuff(port);
}

void FCEUI_SetInputFC(int type, void *ptr, int attrib) {
	JPAttribFC = attrib;
	JPTypeFC = type;
	InputDataPtrFC = ptr;
	SetInputStuffFC();
}

void FCEU_DoSimpleCommand(int cmd) {
	switch (cmd) {
	case FCEUNPCMD_FDSINSERT: FCEU_FDSInsert(-1); break;
	case FCEUNPCMD_FDSSELECT: FCEU_FDSSelect(); break;
	case FCEUNPCMD_FDSEJECT: FCEU_FDSEject(); break;
	case FCEUNPCMD_VSUNICOIN: FCEU_VSUniCoin(); break;
	case FCEUNPCMD_VSUNIDIP0 ... (FCEUNPCMD_VSUNIDIP0 + 7): FCEU_VSUniToggleDIP(cmd - FCEUNPCMD_VSUNIDIP0); break;
	case FCEUNPCMD_POWER: PowerNES(); break;
	case FCEUNPCMD_RESET: ResetNES(); break;
	}
}

void FCEU_QSimpleCommand(int cmd) {
#ifdef NETWORK
	if (FCEUnetplay)
		FCEUNET_SendCommand(cmd, 0);
	else
#endif
	{
		if (!FCEUMOV_IsPlaying())
			FCEU_DoSimpleCommand(cmd);
		else
			FCEUMOV_AddCommand(cmd);
	}
}

void FCEUI_FDSSelect(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_FDSSELECT);
}

int FCEUI_FDSInsert(int oride) {
	FCEU_QSimpleCommand(FCEUNPCMD_FDSINSERT);
	return(1);
}

int FCEUI_FDSEject(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_FDSEJECT);
	return(1);
}

void FCEUI_VSUniToggleDIP(int w) {
	FCEU_QSimpleCommand(FCEUNPCMD_VSUNIDIP0 + w);
}

void FCEUI_VSUniCoin(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_VSUNICOIN);
}

void FCEUI_ResetNES(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_RESET);
}

void FCEUI_PowerNES(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_POWER);
}

