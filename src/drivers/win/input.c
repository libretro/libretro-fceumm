/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

#include "common.h"
#include <dinput.h>

#include "input.h"
#include "keyboard.h"
#include "joystick.h"

#define MAX_EXTENSIONS 7
#define MAX_EXPANSIONS 15

LPDIRECTINPUT7 lpDI;

/* UsrInputType[] is user-specified.  InputType[] is current
		(game loading can override user settings)
*/

int UsrInputType[3] = { SI_GAMEPAD, SI_GAMEPAD, SIFC_NONE };
int InputType[3];
static int cspec;

int gametype;

int InitDInput(void) {
	HRESULT ddrval;

	ddrval = DirectInputCreateEx(fceu_hInstance, DIRECTINPUT_VERSION, &IID_IDirectInput7, (LPVOID*)&lpDI, 0);
	if (ddrval != DI_OK) {
		FCEUD_PrintError("DirectInput: Error creating DirectInput object.");
		return 0;
	}
	return 1;
}

static uint32 MouseData[3];
static int screenmode = 0;
void InputScreenChanged(int fs) {
	int x;
	if (GI) {
		for (x = 0; x < 2; x++) {
			if (InputType[x] == SI_ZAPPER)
				FCEUI_SetInput(x, SI_ZAPPER, MouseData, fs);
			if (InputType[x] == SI_MOUSE)
				FCEUI_SetInput(x, SI_MOUSE, MouseData, fs);
		}
		if (InputType[2] == SIFC_SHADOW)
			FCEUI_SetInputFC(SIFC_SHADOW, MouseData, fs);
	}
	screenmode = fs;
}

/* Necessary for proper GUI functioning(configuring when a game isn't loaded). */
void InputUserActiveFix(void) {
	int x;
	for (x = 0; x < 3; x++) InputType[x] = UsrInputType[x];
}

void ParseGIInput(FCEUGI *gi) {
	InputType[0] = UsrInputType[0];
	InputType[1] = UsrInputType[1];
	InputType[2] = UsrInputType[2];

	if (gi) {
		if (gi->input[0] >= 0)
			InputType[0] = gi->input[0];
		if (gi->input[1] >= 0)
			InputType[1] = gi->input[1];
		if (gi->inputfc >= 0)
			InputType[2] = gi->inputfc;
		cspec = gi->cspecial;
		gametype = gi->type;

		InitOtherInput();
	} else cspec = gametype = 0;
}


static uint8 QuizKingData;
static uint8 HyperShotData;
static uint32 MahjongData;
static uint32 FTrainerData;
static uint8 TopRiderData;

static uint8 BWorldData[1 + 13 + 1];

static void UpdateFKB(void);
static void UpdateSuborKB(void);
static void UpdateGamepad(void);
static void UpdateQuizKing(void);
static void UpdateHyperShot(void);
static void UpdateMahjong(void);
static void UpdateFTrainer(void);
static void UpdateTopRider(void);

static uint32 JSreturn;
int NoWaiting = 1;

#include "keyscan.h"
static char *keys;
static int DIPS = 0;

static uint8 keyonce[MKK_COUNT];
#define KEY(__a) keys[MKK(__a)]

static int _keyonly(int a) {
	if (keys[a]) {
		if (!keyonce[a]) {
			keyonce[a] = 1;
			return(1);
		}
	} else
		keyonce[a] = 0;
	return(0);
}

#define keyonly(__a) _keyonly(MKK(__a))

int cidisabled = 0;

static void KeyboardCommands(void) {
	int is_shift, is_alt;

	keys = GetKeyboard();

	if ((InputType[2] == SIFC_FKB) || (InputType[2] == SIFC_SUBORKB) || (InputType[2] == SIFC_PEC586KB)) {
		if (cidisabled) return;
	}

	is_shift = KEY(LEFTSHIFT) | KEY(RIGHTSHIFT);
	is_alt = KEY(LEFTALT) | KEY(RIGHTALT);

	if (keyonly(F4)) {
		if (is_shift) FCEUI_SetRenderDisable(-1, 2);
		else FCEUI_SetRenderDisable(2, -1);
	}

	{
		static int nocount = 0;

		NoWaiting &= ~1;
		if (KEY(GRAVE))	//&& nocount)
			NoWaiting |= 1;
		nocount = (nocount + 1) & 3;
	}

	if (gametype == GIT_FDS) {
		if (keyonly(F6)) FCEUI_FDSSelect();
		if (keyonly(F8)) FCEUI_FDSInsert(0);
	}

	if (keyonly(F9)) FCEUI_SaveSnapshot();
	if (gametype != GIT_NSF) {
		if (keyonly(F5)) {
			if (is_shift)
				FCEUI_SaveMovie(NULL);
			else
				FCEUI_SaveState(NULL);
		}
		if (keyonly(F7)) {
			if (is_shift)
				FCEUI_LoadMovie(NULL);
			else
				FCEUI_LoadState(NULL);
		}
	}

//  if(keyonly(F1)) FCEUI_ToggleTileView();

	if (gametype == GIT_VSUNI) {
		if (keyonly(F8)) FCEUI_VSUniCoin();
		if (keyonly(F6)) {
			DIPS ^= 1;
			FCEUI_VSUniToggleDIPView();
		}
		if (!(DIPS & 1)) goto DIPSless;
		if (keyonly(1)) FCEUI_VSUniToggleDIP(0);
		if (keyonly(2)) FCEUI_VSUniToggleDIP(1);
		if (keyonly(3)) FCEUI_VSUniToggleDIP(2);
		if (keyonly(4)) FCEUI_VSUniToggleDIP(3);
		if (keyonly(5)) FCEUI_VSUniToggleDIP(4);
		if (keyonly(6)) FCEUI_VSUniToggleDIP(5);
		if (keyonly(7)) FCEUI_VSUniToggleDIP(6);
		if (keyonly(8)) FCEUI_VSUniToggleDIP(7);
	} else {
		static uint8 bbuf[32];
		static int bbuft;
		static int barcoder = 0;

		if (keyonly(H)) FCEUI_NTSCSELHUE();
		if (keyonly(T)) FCEUI_NTSCSELTINT();
		if (KEY(KP_MINUS) || KEY(MINUS)) FCEUI_NTSCDEC();
		if (KEY(KP_PLUS) || KEY(EQUAL)) FCEUI_NTSCINC();

		if ((InputType[2] == SIFC_BWORLD) || (cspec == SIS_DATACH)) {
			if (keyonly(F8)) {
				barcoder ^= 1;
				if (!barcoder) {
					if (InputType[2] == SIFC_BWORLD) {
						strcpy(&BWorldData[1], bbuf);
						BWorldData[0] = 1;
					} else
						FCEUI_DatachSet(bbuf);
					FCEUI_DispMessage("Barcode Entered");
				} else { bbuft = 0; FCEUI_DispMessage("Enter Barcode"); }
			}
		} else barcoder = 0;

		#define SSM(x)				  \
	{ if (barcoder) { if (bbuft < 13) { bbuf[bbuft++] = '0' + x; bbuf[bbuft] = 0; } FCEUI_DispMessage("Barcode: %s", bbuf); }		 \
	  else {						 \
		  if (is_shift) FCEUI_SelectMovie(x);		  \
		  else FCEUI_SelectState(x);		 \
	  }	\
	}

 DIPSless:
		if (keyonly(0)) SSM(0);
		if (keyonly(1)) SSM(1);
		if (keyonly(2)) SSM(2);
		if (keyonly(3)) SSM(3);
		if (keyonly(4)) SSM(4);
		if (keyonly(5)) SSM(5);
		if (keyonly(6)) SSM(6);
		if (keyonly(7)) SSM(7);
		if (keyonly(8)) SSM(8);
		if (keyonly(9)) SSM(9);
   #undef SSM
	}
}

#define MK(x)   { { BUTTC_KEYBOARD }, { 0 }, { MKK(x) }, 1 }
#define MC(x)   { { BUTTC_KEYBOARD }, { 0 }, { x }, 1 }
#define MK2(x1, x2)        { { BUTTC_KEYBOARD }, { 0 }, { MKK(x1), MKK(x2) }, 2 }

#define MKZ()   { { 0 }, { 0 }, { 0 }, 0 }

#define GPZ()   { MKZ(), MKZ(), MKZ(), MKZ() }

ButtConfig GamePadConfig[4][10] = {
	/* Gamepad 1 */
	{
		MK(LEFTALT), MK(LEFTCONTROL), MK(TAB), MK(ENTER), MK(BL_CURSORUP),
		MK(BL_CURSORDOWN), MK(BL_CURSORLEFT), MK(BL_CURSORRIGHT)
	},

	/* Gamepad 2 */
	GPZ(),

	/* Gamepad 3 */
	GPZ(),

	/* Gamepad 4 */
	GPZ()
};


static void UpdateGamepad(void) {
	static int rapid = 0;
	uint32 JS = 0;
	int x;
	int wg;

	rapid = (rapid + 1) & 3;

	for (wg = 0; wg < 4; wg++) {
		for (x = 0; x < 8; x++)
			if (DTestButton(&GamePadConfig[wg][x]))
				JS |= (1 << x) << (wg << 3);

		if (rapid & 2)
			for (x = 0; x < 2; x++)
				if (DTestButton(&GamePadConfig[wg][8 + x]))
					JS |= (1 << x) << (wg << 3);
	}

	for (x = 0; x < 32; x += 8) {	/* Now, test to see if anything weird(up+down at same time)
									is happening, and correct */
		if ((JS & (0xC0 << x)) == (0xC0 << x)) JS &= ~(0xC0 << x);
		if ((JS & (0x30 << x)) == (0x30 << x)) JS &= ~(0x30 << x);
	}

	JSreturn = JS;
}

ButtConfig powerpadsc[2][12] = {
	{
		MK(O), MK(P), MK(BRACKET_LEFT),
		MK(BRACKET_RIGHT),

		MK(K), MK(L), MK(SEMICOLON),
		MK(APOSTROPHE),
		MK(M), MK(COMMA), MK(PERIOD), MK(SLASH)
	},
	{
		MK(O), MK(P), MK(BRACKET_LEFT),
		MK(BRACKET_RIGHT), MK(K), MK(L), MK(SEMICOLON),
		MK(APOSTROPHE),
		MK(M), MK(COMMA), MK(PERIOD), MK(SLASH)
	}
};

static uint32 powerpadbuf[2];

static uint32 UpdatePPadData(int w) {
	uint32 r = 0;
	ButtConfig *ppadtsc = powerpadsc[w];
	int x;

	for (x = 0; x < 12; x++)
		if (DTestButton(&ppadtsc[x])) r |= 1 << x;

	return r;
}


static uint8 fkbkeys[0x48];
static uint8 suborkbkeys[0x65];

void GetMouseData(uint32 *);

void FCEUD_UpdateInput(void) {
	int x;
	int t = 0;

	KeyboardUpdateState();
	UpdateJoysticks();

	//UpdatePhysicalInput();
	KeyboardCommands();

	for (x = 0; x < 2; x++)
		switch (InputType[x]) {
		case SI_GAMEPAD: t |= 1; break;
		case SI_ARKANOID: t |= 2; break;
		case SI_MOUSE: t |= 2; break;
		case SI_ZAPPER: t |= 2; break;
		case SI_POWERPADA:
		case SI_POWERPADB: powerpadbuf[x] = UpdatePPadData(x); break;
		}

	switch (InputType[2]) {
	case SIFC_ARKANOID: t |= 2; break;
	case SIFC_SHADOW: t |= 2; break;
	case SIFC_FKB: if (cidisabled) UpdateFKB(); break;
	case SIFC_PEC586KB:
	case SIFC_SUBORKB: if (cidisabled) UpdateSuborKB(); break;
	case SIFC_HYPERSHOT: UpdateHyperShot(); break;
	case SIFC_MAHJONG: UpdateMahjong(); break;
	case SIFC_QUIZKING: UpdateQuizKing(); break;
	case SIFC_FTRAINERB:
	case SIFC_FTRAINERA: UpdateFTrainer(); break;
	case SIFC_TOPRIDER: UpdateTopRider(); break;
	case SIFC_OEKAKIDS: t |= 2; break;
	}

	if (t & 1)
		UpdateGamepad();

	if (t & 2)
		GetMouseData(MouseData);
}

void InitOtherInput(void) {
	void *InputDPtr;

	int t;
	int x;
	int attrib;

	for (t = 0, x = 0; x < 2; x++) {
		attrib = 0;
		InputDPtr = 0;
		switch (InputType[x]) {
		case SI_POWERPADA:
		case SI_POWERPADB: InputDPtr = &powerpadbuf[x]; break;
		case SI_GAMEPAD: InputDPtr = &JSreturn; break;
		case SI_ARKANOID: InputDPtr = MouseData; t |= 1; break;
		case SI_MOUSE: InputDPtr = MouseData; t |= 1; break;
		case SI_ZAPPER: InputDPtr = MouseData;
			t |= 1;
			attrib = screenmode;
			break;
		}
		FCEUI_SetInput(x, InputType[x], InputDPtr, attrib);
	}

	attrib = 0;
	InputDPtr = 0;
	switch (InputType[2]) {
	case SIFC_SHADOW: InputDPtr = MouseData; t |= 1; attrib = screenmode; break;
	case SIFC_OEKAKIDS: InputDPtr = MouseData; t |= 1; attrib = screenmode; break;
	case SIFC_ARKANOID: InputDPtr = MouseData; t |= 1; break;
	case SIFC_FKB: InputDPtr = fkbkeys; break;
	case SIFC_PEC586KB:
	case SIFC_SUBORKB: InputDPtr = suborkbkeys; break;
	case SIFC_HYPERSHOT: InputDPtr = &HyperShotData; break;
	case SIFC_MAHJONG: InputDPtr = &MahjongData; break;
	case SIFC_QUIZKING: InputDPtr = &QuizKingData; break;
	case SIFC_TOPRIDER: InputDPtr = &TopRiderData; break;
	case SIFC_BWORLD: InputDPtr = BWorldData; break;
	case SIFC_FTRAINERA:
	case SIFC_FTRAINERB: InputDPtr = &FTrainerData; break;
	}

	FCEUI_SetInputFC(InputType[2], InputDPtr, attrib);
	FCEUI_DisableFourScore(eoptions & EO_NOFOURSCORE);
}

ButtConfig fkbmap[0x48] =
{
	MK(F1), MK(F2), MK(F3), MK(F4), MK(F5), MK(F6), MK(F7), MK(F8), MK(1), MK(2), MK(3), MK(4), MK(5), MK(6), MK(7), MK(8), MK(9), MK(0),
	MK(MINUS), MK(EQUAL), MK(BACKSLASH), MK(BACKSPACE), MK(ESCAPE), MK(Q), MK(W), MK(E), MK(R), MK(T), MK(Y), MK(U), MK(I), MK(O),
	MK(P), MK(GRAVE), MK(BRACKET_LEFT), MK(ENTER), MK(LEFTCONTROL), MK(A), MK(S), MK(D), MK(F), MK(G), MK(H), MK(J), MK(K),
	MK(L), MK(SEMICOLON), MK(APOSTROPHE), MK(BRACKET_RIGHT), MK(INSERT), MK(LEFTSHIFT), MK(Z), MK(X), MK(C), MK(V), MK(B), MK(N), MK(M), MK(COMMA),
	MK(PERIOD), MK(SLASH), MK(RIGHTALT), MK(RIGHTSHIFT), MK(LEFTALT), MK(SPACE), MK(BL_DELETE), MK(BL_END), MK(BL_PAGEDOWN),
	MK(BL_CURSORUP), MK(BL_CURSORLEFT), MK(BL_CURSORRIGHT), MK(BL_CURSORDOWN)
};

ButtConfig suborkbmap[0x65] =
{
	MC(0x01), MC(0x3b), MC(0x3c), MC(0x3d), MC(0x3e), MC(0x3f), MC(0x40), MC(0x41), MC(0x42), MC(0x43),
	MC(0x44), MC(0x57), MC(0x58), MC(0x45), MC(0x29), MC(0x02), MC(0x03), MC(0x04), MC(0x05), MC(0x06),
	MC(0x07), MC(0x08), MC(0x09), MC(0x0a), MC(0x0b), MC(0x0c), MC(0x0d), MC(0x0e), MC(0xd2), MC(0xc7),
	MC(0xc9), MC(0xc5), MC(0xb5), MC(0x37), MC(0x4a), MC(0x0f), MC(0x10), MC(0x11), MC(0x12), MC(0x13),
	MC(0x14), MC(0x15), MC(0x16), MC(0x17), MC(0x18), MC(0x19), MC(0x1a), MC(0x1b), MC(0x1c), MC(0xd3),
	MC(0xca), MC(0xd1), MC(0x47), MC(0x48), MC(0x49), MC(0x4e), MC(0x3a), MC(0x1e), MC(0x1f), MC(0x20),
	MC(0x21), MC(0x22), MC(0x23), MC(0x24), MC(0x25), MC(0x26), MC(0x27), MC(0x28), MC(0x4b), MC(0x4c),
	MC(0x4d), MC(0x2a), MC(0x2c), MC(0x2d), MC(0x2e), MC(0x2f), MC(0x30), MC(0x31), MC(0x32), MC(0x33),
	MC(0x34), MC(0x35), MC(0x2b), MC(0xc8), MC(0x4f), MC(0x50), MC(0x51), MC(0x1d), MC(0x38), MC(0x39),
	MC(0xcb), MC(0xd0), MC(0xcd), MC(0x52), MC(0x53), MC(0x00), MC(0x00), MC(0x00), MC(0x00), MC(0x00),
	MC(0x00),
};

static void UpdateFKB(void) {
	int x;

	for (x = 0; x < sizeof(fkbkeys); x++) {
		fkbkeys[x] = 0;

		if (DTestButton(&fkbmap[x]))
			fkbkeys[x] = 1;
	}
}

static void UpdateSuborKB(void) {
	int x;

	for (x = 0; x < sizeof(suborkbkeys); x++) {
		suborkbkeys[x] = 0;

		if (DTestButton(&suborkbmap[x]))
   			suborkbkeys[x] = 1;
	}
}

static ButtConfig HyperShotButtons[4] =
{
	MK(Q), MK(W), MK(E), MK(R)
};

static void UpdateHyperShot(void) {
	int x;

	HyperShotData = 0;
	for (x = 0; x < 0x4; x++) {
		if (DTestButton(&HyperShotButtons[x]))
			HyperShotData |= 1 << x;
	}
}

static ButtConfig MahjongButtons[21] =
{
	MK(Q), MK(W), MK(E), MK(R), MK(T),
	MK(A), MK(S), MK(D), MK(F), MK(G), MK(H), MK(J), MK(K), MK(L),
	MK(Z), MK(X), MK(C), MK(V), MK(B), MK(N), MK(M)
};

static void UpdateMahjong(void) {
	int x;

	MahjongData = 0;
	for (x = 0; x < 21; x++) {
		if (DTestButton(&MahjongButtons[x]))
			MahjongData |= 1 << x;
	}
}

ButtConfig QuizKingButtons[6] =
{
	MK(Q), MK(W), MK(E), MK(R), MK(T), MK(Y)
};

static void UpdateQuizKing(void) {
	int x;

	QuizKingData = 0;

	for (x = 0; x < 6; x++) {
		if (DTestButton(&QuizKingButtons[x]))
			QuizKingData |= 1 << x;
	}
}

ButtConfig TopRiderButtons[8] =
{
	MK(Q), MK(W), MK(E), MK(R), MK(T), MK(Y), MK(U), MK(I)
};

static void UpdateTopRider(void) {
	int x;
	TopRiderData = 0;
	for (x = 0; x < 8; x++)
		if (DTestButton(&TopRiderButtons[x]))
			TopRiderData |= 1 << x;
}

ButtConfig FTrainerButtons[12] =
{
	MK(O), MK(P), MK(BRACKET_LEFT),
	MK(BRACKET_RIGHT), MK(K), MK(L), MK(SEMICOLON),
	MK(APOSTROPHE),
	MK(M), MK(COMMA), MK(PERIOD), MK(SLASH)
};

static void UpdateFTrainer(void) {
	int x;

	FTrainerData = 0;

	for (x = 0; x < 12; x++) {
		if (DTestButton(&FTrainerButtons[x]))
			FTrainerData |= 1 << x;
	}
}

int DWaitButton(HWND hParent, const uint8 *text, ButtConfig *bc);

CFGSTRUCT InputConfig[] = {
	ACA(UsrInputType),
	AC(powerpadsc),
	AC(QuizKingButtons),
	AC(FTrainerButtons),
	AC(HyperShotButtons),
	AC(MahjongButtons),
	AC(GamePadConfig),
	AC(fkbmap),
	AC(suborkbmap),
	ENDCFGSTRUCT
};

void InitInputStuff(void) {
	int x, y;

	KeyboardInitialize();
	InitJoysticks(hAppWnd);

	for (x = 0; x < 4; x++)
		for (y = 0; y < 10; y++)
			JoyClearBC(&GamePadConfig[x][y]);

	for (x = 0; x < 2; x++)
		for (y = 0; y < 12; y++)
			JoyClearBC(&powerpadsc[x][y]);

	for (x = 0; x < sizeof(fkbkeys); x++)
		JoyClearBC(&fkbmap[x]);

	for (x = 0; x < sizeof(suborkbkeys); x++)
		JoyClearBC(&suborkbmap[x]);

	for (x = 0; x < 6; x++)
		JoyClearBC(&QuizKingButtons[x]);
	for (x = 0; x < 12; x++)
		JoyClearBC(&FTrainerButtons[x]);
	for (x = 0; x < 21; x++)
		JoyClearBC(&MahjongButtons[x]);
	for (x = 0; x < 4; x++)
		JoyClearBC(&HyperShotButtons[x]);
}



static void FCExp(char *text) {
	static char *fccortab[13] = {	"none", "arkanoid", "shadow", "4player",
									"fkb", "suborkb", "pec586kb", "hypershot",
									"mahjong", "quizking", "ftrainera", "ftrainerb",
									"oekakids" };

	static int fccortabi[13] = {	SIFC_NONE, SIFC_ARKANOID, SIFC_SHADOW, SIFC_4PLAYER,
									SIFC_FKB, SIFC_SUBORKB, SIFC_PEC586KB, SIFC_HYPERSHOT,
									SIFC_MAHJONG, SIFC_QUIZKING, SIFC_FTRAINERA, SIFC_FTRAINERB,
									SIFC_OEKAKIDS };
	int y;
	for (y = 0; y < 13; y++)
		if (!strcmp(fccortab[y], text))
			UsrInputType[2] = fccortabi[y];
}

static char *cortab[MAX_EXTENSIONS] = { "none", "gamepad", "zapper", "powerpada", "powerpadb", "arkanoid", "mouse" };
static int cortabi[MAX_EXTENSIONS] = { SI_NONE, SI_GAMEPAD, SI_ZAPPER, SI_POWERPADA, SI_POWERPADB, SI_ARKANOID, SI_MOUSE };

static void Input1(char *text) {
	int y;
	for (y = 0; y < MAX_EXTENSIONS; y++)
		if (!strcmp(cortab[y], text))
			UsrInputType[0] = cortabi[y];
}

static void Input2(char *text) {
	int y;
	for (y = 0; y < MAX_EXTENSIONS; y++)
		if (!strcmp(cortab[y], text))
			UsrInputType[1] = cortabi[y];
}

ARGPSTRUCT InputArgs[] = {
	{ "-fcexp", 0, (void*)FCExp, 0x2000 },
	{ "-input1", 0, (void*)Input1, 0x2000 },
	{ "-input2", 0, (void*)Input2, 0x2000 },
	{ 0, 0, 0, 0 }
};

int DTestButton(ButtConfig *bc) {
	int x;

	for (x = 0; x < bc->NumC; x++) {
		if (bc->ButtType[x] == BUTTC_KEYBOARD) {
			if (keys[bc->ButtonNum[x]]) {
				return(1);
			}
		}
	}
	if (DTestButtonJoy(bc)) return(1);
	return(0);
}

static char *MakeButtString(ButtConfig *bc) {
	int x;
	char tmpstr[512];
	char *astr;

	tmpstr[0] = 0;

	for (x = 0; x < bc->NumC; x++) {
		if (x) strcat(tmpstr, ", ");

		if (bc->ButtType[x] == BUTTC_KEYBOARD) {
			strcat(tmpstr, "KB: ");
			if (!GetKeyNameText(bc->ButtonNum[x] << 16, tmpstr + strlen(tmpstr), 16))
				sprintf(tmpstr + strlen(tmpstr), "%02X", bc->ButtonNum[x]);
			else
				sprintf(tmpstr + strlen(tmpstr), " (%02X)", bc->ButtonNum[x]);
		} else if (bc->ButtType[x] == BUTTC_JOYSTICK) {
			strcat(tmpstr, "JS ");
			sprintf(tmpstr + strlen(tmpstr), "%d ", bc->DeviceNum[x]);
			if (bc->ButtonNum[x] & 0x8000) {
				char *asel[3] = { "x", "y", "z" };
				sprintf(tmpstr + strlen(tmpstr), "axis %s%s", asel[bc->ButtonNum[x] & 3], (bc->ButtonNum[x] & 0x4000) ? "-" : "+");
			} else if (bc->ButtonNum[x] & 0x2000) {
				sprintf(tmpstr + strlen(tmpstr), "hat %d:%d", (bc->ButtonNum[x] >> 4) & 3,
						bc->ButtonNum[x] & 3);
			} else {
				sprintf(tmpstr + strlen(tmpstr), "button %d", bc->ButtonNum[x] & 127);
			}
		}
	}

	astr = malloc(strlen(tmpstr) + 1);
	strcpy(astr, tmpstr);
	return(astr);
}


static int DWBStarted;
static ButtConfig *DWBButtons;
static const uint8 *DWBText;

static HWND die;

static BOOL CALLBACK DWBCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		die = NULL;
		return(0);
	case WM_TIMER:
	{
		uint8 devicenum;
		uint16 buttonnum;
		GUID guid;

		if (DoJoyWaitTest(&guid, &devicenum, &buttonnum)) {
			ButtConfig *bc = DWBButtons;
			char *nstr;
			int wc;
			if (DWBStarted) {
				ButtConfig *bc = DWBButtons;
				bc->NumC = 0;
				DWBStarted = 0;
			}
			wc = bc->NumC;
			//FCEU_printf("%d: %d\n",devicenum,buttonnum);
			bc->ButtType[wc] = BUTTC_JOYSTICK;
			bc->DeviceNum[wc] = devicenum;
			bc->ButtonNum[wc] = buttonnum;
			bc->DeviceInstance[wc] = guid;

			/* Stop config if the user pushes the same button twice in a row. */
			if (wc && bc->ButtType[wc] == bc->ButtType[wc - 1] && bc->DeviceNum[wc] == bc->DeviceNum[wc - 1] &&
				bc->ButtonNum[wc] == bc->ButtonNum[wc - 1])
				goto gornk;

			bc->NumC++;

			/* Stop config if we reached our maximum button limit. */
			if (bc->NumC >= MAXBUTTCONFIG)
				goto gornk;
			nstr = MakeButtString(bc);
			SetDlgItemText(hwndDlg, 100, nstr);
			free(nstr);
		}
	}
	break;
	case WM_USER + 666:
		//SetFocus(GetDlgItem(hwndDlg,100));
		if (DWBStarted) {
			char *nstr;
			ButtConfig *bc = DWBButtons;
			bc->NumC = 0;
			DWBStarted = 0;
			nstr = MakeButtString(bc);
			SetDlgItemText(hwndDlg, 100, nstr);
			free(nstr);
		}

		{
			ButtConfig *bc = DWBButtons;
			int wc = bc->NumC;
			char *nstr;

			bc->ButtType[wc] = BUTTC_KEYBOARD;
			bc->DeviceNum[wc] = 0;
			bc->ButtonNum[wc] = lParam & 255;

			/* Stop config if the user pushes the same button twice in a row. */
			if (wc && bc->ButtType[wc] == bc->ButtType[wc - 1] && bc->DeviceNum[wc] == bc->DeviceNum[wc - 1] &&
				bc->ButtonNum[wc] == bc->ButtonNum[wc - 1])
				goto gornk;

			bc->NumC++;
			/* Stop config if we reached our maximum button limit. */
			if (bc->NumC >= MAXBUTTCONFIG)
				goto gornk;

			nstr = MakeButtString(bc);
			SetDlgItemText(hwndDlg, 100, nstr);
			free(nstr);
		}
		break;
	case WM_INITDIALOG:
		SetWindowText(hwndDlg, DWBText);
		BeginJoyWait(hwndDlg);
		SetTimer(hwndDlg, 666, 25, 0);					/* Every 25ms.*/
		{
			char *nstr = MakeButtString(DWBButtons);
			SetDlgItemText(hwndDlg, 100, nstr);
			free(nstr);
		}
		break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;

	case WM_COMMAND:
		switch (wParam & 0xFFFF) {
		case 200:
		{
			ButtConfig *bc = DWBButtons;
			char *nstr;
			bc->NumC = 0;
			nstr = MakeButtString(bc);
			SetDlgItemText(hwndDlg, 100, nstr);
			free(nstr);
		}
		break;
		case 201:
 gornk:
			KillTimer(hwndDlg, 666);
			EndJoyWait(hAppWnd);
			SetForegroundWindow(GetParent(hwndDlg));
			DestroyWindow(hwndDlg);
			break;
		}
	}
	return 0;
}

int DWaitButton(HWND hParent, const uint8 *text, ButtConfig *bc) {
	DWBText = text;
	DWBButtons = bc;
	DWBStarted = 1;

	die = CreateDialog(fceu_hInstance, "DWBDIALOG", hParent, DWBCallB);

	EnableWindow(hParent, 0);

	ShowWindow(die, 1);

	while (die) {
		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, 0, 0, 0) > 0) {
				if (msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN) {
					LPARAM tmpo;

					tmpo = ((msg.lParam >> 16) & 0x7F) | ((msg.lParam >> 17) & 0x80);
					PostMessage(die, WM_USER + 666, 0, tmpo);
					continue;
				}
				if (msg.message == WM_SYSCOMMAND) continue;
				if (!IsDialogMessage(die, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		Sleep(10);
	}

	EnableWindow(hParent, 1);
	return 0;
}

static ButtConfig *DoTBButtons;
static const char *DoTBTitle;
static int DoTBMax;
static int DoTBType, DoTBPort;

static BOOL CALLBACK DoTBCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		if (DoTBType == SI_GAMEPAD) {
			char buf[32];
			sprintf(buf, "Virtual Gamepad %d", DoTBPort + 1);
			SetDlgItemText(hwndDlg, 100, buf);

			sprintf(buf, "Virtual Gamepad %d", DoTBPort + 3);
			SetDlgItemText(hwndDlg, 101, buf);

			CheckDlgButton(hwndDlg, 400, (eoptions & EO_NOFOURSCORE) ? BST_CHECKED : BST_UNCHECKED);
		}
		SetWindowText(hwndDlg, DoTBTitle);
		break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;

	case WM_COMMAND:
	{
		int b;
		b = wParam & 0xFFFF;
		if (b >= 300 && b < (300 + DoTBMax)) {
			char btext[128];
			btext[0] = 0;
			GetDlgItemText(hwndDlg, b, btext, 128);
			DWaitButton(hwndDlg, btext, &DoTBButtons[b - 300]);
		} else switch (wParam & 0xFFFF) {
			case 1:
 gornk:

				if (DoTBType == SI_GAMEPAD) {
					eoptions &= ~EO_NOFOURSCORE;
					if (IsDlgButtonChecked(hwndDlg, 400) == BST_CHECKED)
						eoptions |= EO_NOFOURSCORE;
				}
				EndDialog(hwndDlg, 0);
				break;
			}
	}
	}
	return 0;
}

static void DoTBConfig(HWND hParent, const char *text, char *template, ButtConfig *buttons, int max) {
	DoTBTitle = text;
	DoTBButtons = buttons;
	DoTBMax = max;
	DialogBox(fceu_hInstance, template, hParent, DoTBCallB);
}

static BOOL CALLBACK InputConCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static const char *strn[MAX_EXTENSIONS] = { "<none>", "Gamepad", "Zapper", "Power Pad A", "Power Pad B", "Arkanoid Paddle", "Mouse" };
	static const char *strf[MAX_EXPANSIONS] =
	{ "<none>", "Arkanoid Paddle", "Hyper Shot gun", "4-Player Adapter",
	  "Family Keyboard", "Subor Keyboard", "PEC586 Keyboard", "HyperShot Pads", "Mahjong", "Quiz King Buzzers",
	  "Family Trainer A", "Family Trainer B", "Oeka Kids Tablet", "Barcode World",
	  "Top Rider" };
	static const int haven[MAX_EXTENSIONS] = { 0, 1, 0, 1, 1, 0, 0 };
	static const int havef[MAX_EXPANSIONS] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0 };
//  int x;

	switch (uMsg) {
	case WM_INITDIALOG:
//                for(x=0;x<2;x++)
	{
		int y;
		for (y = 0; y < MAX_EXTENSIONS - 1; y++)
			SendDlgItemMessage(hwndDlg, 104, CB_ADDSTRING, 0, (LPARAM)(LPSTR)strn[y]);
		SendDlgItemMessage(hwndDlg, 104, CB_SETCURSEL, UsrInputType[0], (LPARAM)(LPSTR)0);
		EnableWindow(GetDlgItem(hwndDlg, 106), haven[InputType[0]]);
		SetDlgItemText(hwndDlg, 200, (LPTSTR)strn[InputType[0]]);
	}
		{
			int y;
			for (y = 0; y < MAX_EXTENSIONS; y++)
				SendDlgItemMessage(hwndDlg, 105, CB_ADDSTRING, 0, (LPARAM)(LPSTR)strn[y]);
			SendDlgItemMessage(hwndDlg, 105, CB_SETCURSEL, UsrInputType[1], (LPARAM)(LPSTR)0);
			EnableWindow(GetDlgItem(hwndDlg, 107), haven[InputType[1]]);
			SetDlgItemText(hwndDlg, 201, (LPTSTR)strn[InputType[1]]);
		}


		{
			int y;
			for (y = 0; y < MAX_EXPANSIONS; y++)
				SendDlgItemMessage(hwndDlg, 110, CB_ADDSTRING, 0, (LPARAM)(LPSTR)strf[y]);
			SendDlgItemMessage(hwndDlg, 110, CB_SETCURSEL, UsrInputType[2], (LPARAM)(LPSTR)0);
			EnableWindow(GetDlgItem(hwndDlg, 111), havef[InputType[2]]);
			SetDlgItemText(hwndDlg, 202, (LPTSTR)strf[InputType[2]]);
		}

		break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;
	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELENDOK) {
			switch (LOWORD(wParam)) {
			case 104:
			case 105: UsrInputType[LOWORD(wParam) - 104] = InputType[LOWORD(wParam) - 104] = SendDlgItemMessage(hwndDlg, LOWORD(wParam), CB_GETCURSEL, 0, (LPARAM)(LPSTR)0);
				EnableWindow(GetDlgItem(hwndDlg, LOWORD(wParam) + 2), haven[InputType[LOWORD(wParam) - 104]]);
				SetDlgItemText(hwndDlg, 200 + LOWORD(wParam) - 104, (LPTSTR)strn[InputType[LOWORD(wParam) - 104]]);
				break;
			case 110: UsrInputType[2] = InputType[2] = SendDlgItemMessage(hwndDlg, 110, CB_GETCURSEL, 0, (LPARAM)(LPSTR)0);
				EnableWindow(GetDlgItem(hwndDlg, 111), havef[InputType[2]]);
				SetDlgItemText(hwndDlg, 202, (LPTSTR)strf[InputType[2]]);
				break;
			}
		}
		if (!(wParam >> 16))
			switch (wParam & 0xFFFF) {
			case 111:
			{
				const char *text = strf[InputType[2]];
				DoTBType = DoTBPort = 0;

				switch (InputType[2]) {
				case SIFC_FTRAINERA:
				case SIFC_FTRAINERB: DoTBConfig(hwndDlg, text, "POWERPADDIALOG", FTrainerButtons, 12); break;
				case SIFC_FKB: DoTBConfig(hwndDlg, text, "FKBDIALOG", fkbmap, sizeof(fkbkeys)); break;
				case SIFC_PEC586KB:
				case SIFC_SUBORKB: DoTBConfig(hwndDlg, text, "SUBORKBDIALOG", suborkbmap, sizeof(suborkbkeys)); break;
				case SIFC_QUIZKING: DoTBConfig(hwndDlg, text, "QUIZKINGDIALOG", QuizKingButtons, 6); break;
				}
			}
			break;

			case 107:
			case 106:
			{
				int which = (wParam & 0xFFFF) - 106;
				const char *text = strn[InputType[which]];

				DoTBType = DoTBPort = 0;
				switch (InputType[which]) {
				case SI_GAMEPAD:
				{
					ButtConfig tmp[10 + 10];

					memcpy(tmp, GamePadConfig[which], 10 * sizeof(ButtConfig));
					memcpy(&tmp[10], GamePadConfig[which + 2], 10 * sizeof(ButtConfig));

					DoTBType = SI_GAMEPAD;
					DoTBPort = which;
					DoTBConfig(hwndDlg, text, "GAMEPADDIALOG", tmp, 10 + 10);

					memcpy(GamePadConfig[which], tmp, 10 * sizeof(ButtConfig));
					memcpy(GamePadConfig[which + 2], &tmp[10], 10 * sizeof(ButtConfig));
				}
				break;

				case SI_POWERPADA:
				case SI_POWERPADB:
					DoTBConfig(hwndDlg, text, "POWERPADDIALOG", powerpadsc[which], 12);
					break;
				}
			}
			break;

			case 1:
 gornk:
				EndDialog(hwndDlg, 0);
				break;
			}
	}
	return 0;
}

void ConfigInput(HWND hParent) {
	DialogBox(fceu_hInstance, "INPUTCONFIG", hParent, InputConCallB);
	if (GI)
		InitOtherInput();
}



void DestroyInput(void) {
	if (lpDI) {
		KillJoysticks();
		KeyboardClose();
		IDirectInput7_Release(lpDI);
	}
}
