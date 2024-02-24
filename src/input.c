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

/* merged pad.c 2017-9-30 */

#include <string.h>

#include "fceu-types.h"
#include "x6502.h"

#include "fceu.h"
#include "state.h"

#include "cart.h"
#include "input.h"
#include "vsuni.h"
#include "fds.h"

#include "input/zapper.h"

extern INPUTC *FCEU_InitZapper(int w);
extern INPUTC *FCEU_InitMouse(int w);
extern INPUTC *FCEU_InitPowerpadA(int w);
extern INPUTC *FCEU_InitPowerpadB(int w);
extern INPUTC *FCEU_InitArkanoid(int w);
extern INPUTC *FCEU_InitLCDCompZapper(int w);
extern INPUTC *FCEU_InitSNESMouse(int w);
extern INPUTC *FCEU_InitSNESGamepad(int w);
extern INPUTC *FCEU_InitVirtualBoy(int w);

extern INPUTCFC *FCEU_InitArkanoidFC(void);
extern INPUTCFC *FCEU_InitSpaceShadow(void);
extern INPUTCFC *FCEU_InitFKB(void);
extern INPUTCFC *FCEU_InitSuborKB(void);
extern INPUTCFC *FCEU_InitPEC586KB(void);
extern INPUTCFC *FCEU_InitHS(void);
extern INPUTCFC *FCEU_InitMahjong(void);
extern INPUTCFC *FCEU_InitPartyTap(void);
extern INPUTCFC *FCEU_InitFamilyTrainerA(void);
extern INPUTCFC *FCEU_InitFamilyTrainerB(void);
extern INPUTCFC *FCEU_InitOekaKids(void);
extern INPUTCFC *FCEU_InitTopRider(void);
extern INPUTCFC *FCEU_InitFamiNetSys(void);
extern INPUTCFC *FCEU_InitBarcodeWorld(void);
extern INPUTCFC *FCEU_InitExcitingBoxing(void);

typedef struct JOYPORT {
	uint8 type;
	uint8 attrib;
	void *dataptr;
	INPUTC *driver;
} JOYPORT;

typedef struct PORTFC {
	uint8 type;
	uint8 attrib;
	void *dataptr;
	INPUTCFC *driver;
} PORTFC;

static uint8 joy_readbit[2];
static uint8 joy[4] = { 0, 0, 0, 0 };
static uint8 LastStrobe;

static int FourScoreAttached = FALSE; /* Set to 1 if NES-style four-player adapter is disabled. */

static JOYPORT joyport[2];
static PORTFC portFC;

static INPUTC DummyJPort = { 0, 0, 0, 0, 0, 0 };

static DECLFR(JPRead) {
	uint8 ret               = 0;
	static uint8 microphone = 0;

	if (joyport[A & 0x01].driver->Read) {
		ret |= joyport[A & 0x01].driver->Read(A & 0x01);
	}

	/* Test if the port 2 start button is being pressed.
	 * On a famicom, port 2 start shouldn't exist, so this removes it.
	 * Games can't automatically be checked for NES/Famicom status,
	 * so it's an all-encompassing change in the input config menu.
	 */
	if ((FSettings.ReplaceP2StartWithMicrophone) && (A & 0x01) && (joy_readbit[1] == 0x04)) {
		/* Nullify Port 2 Start Button */
		ret &= 0xFE;
	}

	if (portFC.driver && portFC.driver->Read) {
		ret = portFC.driver->Read(A & 0x01, ret);
	}

	/* Not verified against hardware. */
	if (FSettings.ReplaceP2StartWithMicrophone) {
		if (joy[1] & 0x08) {
			microphone = !microphone;
			if (microphone) {
				ret |= 0x04;
			}
		} else {
			microphone = 0;
		}
	}

	ret |= (cpu.openbus & 0xC0);

	return (ret);
}

static DECLFW(B4016) {
	int x;

	if (portFC.driver && portFC.driver->Write) {
		portFC.driver->Write(V & 0x07);
	}

	for (x = 0; x < 2; x++) {
		if (joyport[x].driver->Write) {
			joyport[x].driver->Write(V & 0x01);	
		}
	}

	if ((LastStrobe & 0x01) && (!(V & 0x01))) {
		for (x = 0; x < 2; x++) {
			if (joyport[x].driver->Strobe) {
				joyport[x].driver->Strobe(x);
			}
		}
		if (portFC.driver && portFC.driver->Strobe) {
			portFC.driver->Strobe();
		}
	}

	LastStrobe = V;
}

/**********************************************************************/
/* This function is a quick hack to get the NSF player to use emulated gamepad
	input.
*/
uint8 FCEU_GetJoyJoy(void) {
	return (joy[0] | joy[1] | joy[2] | joy[3]);
}

/* 4-player support for famicom expansion */
static uint8 F4ReadBit[2];
static void StrobeFami4(void) {
	F4ReadBit[0] = F4ReadBit[1] = 0;
}

static uint8 ReadFami4(int w, uint8 ret) {
	ret &= 0x01;
	ret |= ((joy[2 + w] >> (F4ReadBit[w])) & 0x01) << 1;

	if (F4ReadBit[w] >= 8) {
		ret |= 0x02;
	} else {
		F4ReadBit[w]++;
	}

	return (ret);
}

/* Hori 4 player driver for expansion port */
static uint8 Hori4ReadBit[2];
static void StrobeHori4(void) {
	Hori4ReadBit[0] = Hori4ReadBit[1] = 0;
}

static uint8 ReadHori4(int w, uint8 ret) {
	ret &= 0x01;
	if (Hori4ReadBit[w] < 8) {
		ret |= ((joy[w] >> (Hori4ReadBit[w])) & 0x01) << 1;
	} else if (Hori4ReadBit[w] < 16) {
		ret |= ((joy[2 + w] >> (Hori4ReadBit[w] - 8)) & 0x01) << 1;
	} else if (Hori4ReadBit[w] < 24) {
		ret |= (((w ? 0x10 : 0x20) >> (7 - (Hori4ReadBit[w] - 16))) & 0x01) << 1;
	}

	if (Hori4ReadBit[w] >= 24) {
		ret |= 0x02;
	} else {
		Hori4ReadBit[w]++;
	}

	return (ret);
}

/* VS. Unisystem inputs */
static uint8 ReadGPVS(int w) {
	uint8 ret = 0;

	if (joy_readbit[w] >= 8) {
		ret = 1;
	} else {
		ret = ((joy[w] >> (joy_readbit[w])) & 1);
#ifdef FCEUDEF_DEBUGGER
		if (!fceuindbg)
#endif
			joy_readbit[w]++;
	}
	return ret;
}

/* standard gamepad inputs */
static uint8 ReadGP(int w) {
	uint8 ret = 0;

	if (joy_readbit[w] >= 8) {
		ret = ((joy[2 + w] >> (joy_readbit[w] & 0x07)) & 0x01);
	} else {
		ret = ((joy[w] >> (joy_readbit[w])) & 0x01);
	}
	if (joy_readbit[w] >= 16) {
		ret = 0;
	}
	if (!FourScoreAttached) {
		if (joy_readbit[w] >= 8) {
			ret |= 0x01;
		}
	} else {
		if (joy_readbit[w] == 19 - w) {
			ret |= 0x01;
		}
	}
#ifdef FCEUDEF_DEBUGGER
	if (!fceuindbg)
#endif
		joy_readbit[w]++;
	return ret;
}

static void UpdateGP(int w, void *data, int arg) {
	uint32 *ptr = (uint32 *)data;
	if (!w) {
		joy[0] = *(uint32 *)ptr;
		joy[2] = *(uint32 *)ptr >> 16;
	} else {
		joy[1] = *(uint32 *)ptr >> 8;
		joy[3] = *(uint32 *)ptr >> 24;
	}
}

static void StrobeGP(int w) {
	joy_readbit[w] = 0;
}

static INPUTC GPC      = { ReadGP, 0, StrobeGP, UpdateGP, 0, 0 };
static INPUTC GPCVS    = { ReadGPVS, 0, StrobeGP, UpdateGP, 0, 0 };
static INPUTCFC FAMI4C = { ReadFami4, 0, StrobeFami4, 0, 0, 0 };
static INPUTCFC HORI4C = { ReadHori4, 0, StrobeHori4, 0, 0, 0 };

/**********************************************************************/
void FCEU_DrawInput(uint8 *buf) {
	int x;

	for (x = 0; x < 2; x++) {
		if (joyport[x].driver->Draw) {
			joyport[x].driver->Draw(x, buf, joyport[x].attrib);
		}
	}

	if (portFC.driver && portFC.driver->Draw) {
		portFC.driver->Draw(buf, portFC.attrib);
	}
}

void FCEU_UpdateInput(void) {
	int x;

	for (x = 0; x < 2; x++) {
		if (joyport[x].driver && joyport[x].driver->Update) {
			joyport[x].driver->Update(x, joyport[x].dataptr, joyport[x].attrib);
		}
	}

	if (portFC.driver && portFC.driver->Update) {
		portFC.driver->Update(portFC.dataptr, portFC.attrib);
	}

	if (GameInfo && ((GameInfo->type == GIT_VSUNI) || (iNESCart.mapper == 124))) {
		for (x = 0; x < 2; x++) {
			if (vsuni_system.coinon[x]) {
				vsuni_system.coinon[x]--;
			}	
		}
		if (vsuni_system.service) {
			vsuni_system.service--;
		}
	}

	if (GameInfo->type == GIT_VSUNI) {
		FCEU_VSUniSwap(&joy[0], &joy[1]);
	}
}

static DECLFR(VSUNIRead0) {
	uint8 ret = 0;

	if (joyport[0].driver->Read) {
		ret |= (joyport[0].driver->Read(0)) & 0x01;
	}

	ret |= (vsuni_system.vsdip & 0x03) << 3;

	if (vsuni_system.coinon[0]) {
		ret |= 0x20;
	}
	if (vsuni_system.coinon[1]) {
		ret |= 0x40;
	}
	if (vsuni_system.service) {
		ret |= 0x04;
	}

	return (ret);
}

static DECLFR(VSUNIRead1) {
	uint8 ret = 0;

	if (joyport[1].driver && joyport[1].driver->Read) {
		ret |= (joyport[1].driver->Read(1)) & 0x01;
	}

	ret |= vsuni_system.vsdip & 0xFC;

	return (ret);
}

void InputScanlineHook(uint8 *bg, uint8 *spr, uint32 linets, int final) {
	int x;

	for (x = 0; x < 2; x++) {
		if (joyport[x].driver && joyport[x].driver->SLHook) {
			joyport[x].driver->SLHook(x, bg, spr, linets, final);
		}
	}

	if (portFC.driver && portFC.driver->SLHook) {
		portFC.driver->SLHook(bg, spr, linets, final);
	}
}

static void SetInputStuff(int x) {
	switch (joyport[x].type) {
	case SI_NONE:
		joyport[x].driver = &DummyJPort;
		break;
	case SI_GAMEPAD:
		if (GameInfo->type == GIT_VSUNI)
			joyport[x].driver = &GPCVS;
		else
			joyport[x].driver = &GPC;
		break;
	case SI_ARKANOID:
		joyport[x].driver = FCEU_InitArkanoid(x);
		break;
	case SI_MOUSE:
		joyport[x].driver = FCEU_InitMouse(x);
		break;
	case SI_ZAPPER:
		joyport[x].driver = FCEU_InitZapper(x);
		break;
	case SI_POWERPADA:
		joyport[x].driver = FCEU_InitPowerpadA(x);
		break;
	case SI_POWERPADB:
		joyport[x].driver = FCEU_InitPowerpadB(x);
		break;
	case SI_LCDCOMP_ZAPPER:
		joyport[x].driver = FCEU_InitLCDCompZapper(x);
		break;
	case SI_SNES_GAMEPAD:
		joyport[x].driver = FCEU_InitSNESGamepad(x);
		break;
	case SI_SNES_MOUSE:
		joyport[x].driver = FCEU_InitSNESMouse(x);
		break;
	case SI_VIRTUALBOY:
		joyport[x].driver = FCEU_InitVirtualBoy(x);
		break;
	}
}

static void SetInputStuffFC(void) {
	switch (portFC.type) {
	case SIFC_NONE:
		portFC.driver = 0;
		break;
	case SIFC_ARKANOID:
		portFC.driver = FCEU_InitArkanoidFC();
		break;
	case SIFC_SHADOW:
		portFC.driver = FCEU_InitSpaceShadow();
		break;
	case SIFC_OEKAKIDS:
		portFC.driver = FCEU_InitOekaKids();
		break;
	case SIFC_4PLAYER:
		portFC.driver = &FAMI4C;
		memset(&F4ReadBit, 0, sizeof(F4ReadBit));
		break;
	case SIFC_FKB:
		portFC.driver = FCEU_InitFKB();
		break;
	case SIFC_SUBORKB:
		portFC.driver = FCEU_InitSuborKB();
		break;
	case SIFC_PEC586KB:
		portFC.driver = FCEU_InitPEC586KB();
		break;
	case SIFC_HYPERSHOT:
		portFC.driver = FCEU_InitHS();
		break;
	case SIFC_MAHJONG:
		portFC.driver = FCEU_InitMahjong();
		break;
	case SIFC_PARTYTAP:
		portFC.driver = FCEU_InitPartyTap();
		break;
	case SIFC_FTRAINERA:
		portFC.driver = FCEU_InitFamilyTrainerA();
		break;
	case SIFC_FTRAINERB:
		portFC.driver = FCEU_InitFamilyTrainerB();
		break;
	case SIFC_BWORLD:
		portFC.driver = FCEU_InitBarcodeWorld();
		break;
	case SIFC_TOPRIDER:
		portFC.driver = FCEU_InitTopRider();
		break;
	case SIFC_FAMINETSYS:
		portFC.driver = FCEU_InitFamiNetSys();
		break;
	case SIFC_EXCITINGBOXING:
		portFC.driver = FCEU_InitExcitingBoxing();
		break;
	case SIFC_HORI4PLAYER:
		portFC.driver = &HORI4C;
		memset(&Hori4ReadBit, 0, sizeof(Hori4ReadBit));
		break;
	}
}

void FCEUINPUT_Power(void) {
	memset(joy_readbit, 0, sizeof(joy_readbit));
	memset(joy, 0, sizeof(joy));
	LastStrobe = 0;

	if (GameInfo && GameInfo->type == GIT_VSUNI) {
		SetReadHandler(0x4016, 0x4016, VSUNIRead0);
		SetReadHandler(0x4017, 0x4017, VSUNIRead1);
	} else {
		SetReadHandler(0x4016, 0x4017, JPRead);
	}

	SetWriteHandler(0x4016, 0x4016, B4016);

	SetInputStuff(0);
	SetInputStuff(1);
	SetInputStuffFC();
}

void FCEUI_SetInput(int port, int type, void *ptr, int attrib) {
	joyport[port].attrib  = attrib;
	joyport[port].type    = type;
	joyport[port].dataptr = ptr;
	SetInputStuff(port);
}

void FCEUI_SetInputFC(int type, void *ptr, int attrib) {
	portFC.attrib  = attrib;
	portFC.type    = type;
	portFC.dataptr = ptr;
	SetInputStuffFC();
}

void FCEUI_SetInputFourScore(int useFourScore) {
	FourScoreAttached = useFourScore;
}

SFORMAT FCEUCTRL_STATEINFO[] = {
	{ joy_readbit, 2, "JYRB" },
	{ joy, 4, "JOYS" },
	{ &LastStrobe, 1, "LSTS" },
	{ &ZD[0].bogo, 1, "ZBG0" },
	{ &ZD[1].bogo, 1, "ZBG1" },
	{ &ZD[0].zaphit, sizeof(ZD[0].zaphit), "ZHT0" },
	{ &ZD[1].zaphit, sizeof(ZD[1].zaphit), "ZHT1" },
	{ &ZD[0].zappo, sizeof(ZD[0].zappo), "ZAP0" },
	{ &ZD[1].zappo, sizeof(ZD[1].zappo), "ZAP1" },
	{ &ZD[0].zap_readbit, sizeof(ZD[0].zap_readbit), "ZRB0" },
	{ &ZD[1].zap_readbit, sizeof(ZD[1].zap_readbit), "ZRB1" },
	{ 0 },
};

void FCEU_DoSimpleCommand(int cmd) {
	switch (cmd) {
	case FCEUNPCMD_FDSINSERT:
		FCEU_FDSInsert(-1);
		break;
	case FCEUNPCMD_FDSSELECT:
		FCEU_FDSSelect();
		break;
	case FCEUNPCMD_FDSEJECT:
		FCEU_FDSEject();
		break;
	case FCEUNPCMD_VSUNICOIN:
		FCEU_VSUniCoin(0);
		break;
	case FCEUNPCMD_VSUNICOIN2:
		FCEU_VSUniCoin(1);
		break;
	case FCEUNPCMD_VSUNISERVICE:
		FCEU_VSUniService();
		break;
	case FCEUNPCMD_VSUNIDIP0:
	case (FCEUNPCMD_VSUNIDIP0 + 1):
	case (FCEUNPCMD_VSUNIDIP0 + 2):
	case (FCEUNPCMD_VSUNIDIP0 + 3):
	case (FCEUNPCMD_VSUNIDIP0 + 4):
	case (FCEUNPCMD_VSUNIDIP0 + 5):
	case (FCEUNPCMD_VSUNIDIP0 + 6):
	case (FCEUNPCMD_VSUNIDIP0 + 7):
		FCEU_VSUniToggleDIP(cmd - FCEUNPCMD_VSUNIDIP0);
		break;
	case FCEUNPCMD_POWER:
		PowerNES();
		break;
	case FCEUNPCMD_RESET:
		ResetNES();
		break;
	}
}

static void FCEU_QSimpleCommand(int cmd) {
	FCEU_DoSimpleCommand(cmd);
}

void FCEUI_FDSSelect(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_FDSSELECT);
}

void FCEUI_FDSInsert(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_FDSINSERT);
}

void FCEUI_FDSEject(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_FDSEJECT);
}

void FCEUI_VSUniToggleDIP(int w) {
	FCEU_QSimpleCommand(FCEUNPCMD_VSUNIDIP0 + w);
}

void FCEUI_VSUniCoin(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_VSUNICOIN);
}

void FCEUI_VSUniCoin2(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_VSUNICOIN2);
}

void FCEUI_VSUniService(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_VSUNISERVICE);
}

void FCEUI_ResetNES(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_RESET);
}

void FCEUI_PowerNES(void) {
	FCEU_QSimpleCommand(FCEUNPCMD_POWER);
}
