#include "libretro_input.h"

#include "../src/fceu.h"
#include "../src/cart.h"
#include "../src/fds.h"
#include "../src/input.h"
#include "../src/vsuni.h"

#define DEVICE_AUTO           RETRO_DEVICE_JOYPAD
#define DEVICE_SI_GAMEPAD     RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 0)
#define DEVICE_SI_SNESGAMEPAD RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 1)
#define DEVICE_SI_VIRTUALBOY  RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 2)
#define DEVICE_SI_POWERPADA   RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 3)
#define DEVICE_SI_POWERPADB   RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 4)
#define DEVICE_SIFC_HYPERSHOT RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 5)
#define DEVICE_SIFC_FTRAINERA RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 6)
#define DEVICE_SIFC_FTRAINERB RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 7)
#define DEVICE_SIFC_PARTYTAP  RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 8)
#define DEVICE_SIFC_EXCITINGBOXING  RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 9)
#define DEVICE_SIFC_4PLAYER   RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 10)
#define DEVICE_SIFC_HORI4PLAYER RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 11)
#define DEVICE_SI_ZAPPER      RETRO_DEVICE_LIGHTGUN
#define DEVICE_SIFC_ZAPPER    RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_LIGHTGUN, 0)
#define DEVICE_SIFC_SHADOW    RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_LIGHTGUN, 1)
#define DEVICE_SI_ARKANOID    RETRO_DEVICE_MOUSE
#define DEVICE_SI_SNESMOUSE   RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE, 0)
#define DEVICE_SIFC_ARKANOID  RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE, 1)
#define DEVICE_SIFC_OEKAKIDS  RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE, 2)

static const struct retro_controller_description pads_nes[10] = {
	{ "Auto", DEVICE_AUTO },
	{ "Gamepad", DEVICE_SI_GAMEPAD },
	{ "Arkanoid", DEVICE_SI_ARKANOID },
	{ "Zapper", DEVICE_SI_ZAPPER },
	{ "Power Pad A", DEVICE_SI_POWERPADA },
	{ "Power Pad B", DEVICE_SI_POWERPADB },
	{ "SNES Pad", DEVICE_SI_SNESGAMEPAD },
	{ "SNES Mouse", DEVICE_SI_SNESMOUSE },
	{ "Virtual Boy", DEVICE_SI_VIRTUALBOY },
	{ NULL, 0 },
};

static const struct retro_controller_description pads_fc[] = {
	{ "Auto", DEVICE_AUTO },
	{ "Arkanoid", DEVICE_SIFC_ARKANOID },
	{ "Hyper Shot Gun", DEVICE_SIFC_SHADOW },
	{ "HyperShot Pads", DEVICE_SIFC_HYPERSHOT },
	{ "Oeka Kids Tablet", DEVICE_SIFC_OEKAKIDS },
	{ "4-Player Adapter (Simple)", DEVICE_SIFC_4PLAYER },
	{ "Hori 4-Player Adapter", DEVICE_SIFC_HORI4PLAYER },
	{ "Family Trainer A", DEVICE_SIFC_FTRAINERA },
	{ "Family Trainer B", DEVICE_SIFC_FTRAINERB },
	{ "Party Tap", DEVICE_SIFC_PARTYTAP },
	{ "Exciting Boxing", DEVICE_SIFC_EXCITINGBOXING },
	{ NULL, 0 },
};

static const struct retro_controller_info ports[6] = {
	{ pads_nes, 10 },
	{ pads_nes, 10 },
	{ pads_nes, 2 },
	{ pads_nes, 2 },
	{ pads_fc, 11 },
	{ 0, 0 },
};

/*
 * Flags to keep track of whether turbo
 * buttons toggled on or off.
 *
 * There are two values in array
 * for Turbo A and Turbo B for
 * each player
 */

#define MAX_BUTTONS   9
#define TURBO_BUTTONS 2

uint8 turbo_button_toggle[MAX_CONTROLLERS][TURBO_BUTTONS] = { { 0 } };

typedef struct {
	unsigned retro;
	unsigned nes;
} keymap;

static const keymap turbomap[] = {
	{ RETRO_DEVICE_ID_JOYPAD_X, JOY_A },
	{ RETRO_DEVICE_ID_JOYPAD_Y, JOY_B },
};

static NES_INPUT_T nes_input = { 0 };

static uint32_t Dummy = 0;

/* cached during input_init_env() */
static retro_environment_t environ_cb;

/* cached during input_update() */
static retro_input_state_t input_cb;

static void addDesc(struct retro_input_descriptor *p, unsigned port, unsigned device, unsigned id, const char *description) {
	p->port        = port;
	p->device      = device;
	p->index       = 0;
	p->id          = id;
	p->description = description;
}

static void set_input(unsigned port) {
	int type        = 0;
	int attrib      = 0;
	void *inputDPtr = NULL;

	if (port <= 1) {
		switch (nes_input.type[port]) {
		case RETRO_DEVICE_NONE:
			type      = SI_UNSET;
			inputDPtr = &Dummy;
			FCEU_printf(" Player %u: none\n", port + 1);
			break;
		case RETRO_DEVICE_JOYPAD:
		case DEVICE_SI_GAMEPAD:
			type      = SI_GAMEPAD;
			inputDPtr = &nes_input.JSReturn;
			FCEU_printf(" Player %u: Gamepad\n", port + 1);
			break;
		case DEVICE_SI_ARKANOID:
			type      = SI_ARKANOID;
			inputDPtr = &nes_input.MouseData[port];
			FCEU_printf(" Player %u: Arkanoid Paddle\n", port + 1);
			break;
		case DEVICE_SI_ZAPPER:
			type      = SI_ZAPPER;
			attrib    = 1;
			inputDPtr = &nes_input.MouseData[port];
			FCEU_printf(" Player %u: Zapper\n", port + 1);
			break;
		case DEVICE_SI_POWERPADA:
			type      = SI_POWERPADA;
			inputDPtr = &nes_input.PowerPadData[port];
			FCEU_printf(" Player %u: Power Pad A\n", port + 1);
			break;
		case DEVICE_SI_POWERPADB:
			type      = SI_POWERPADB;
			inputDPtr = &nes_input.PowerPadData[port];
			FCEU_printf(" Player %u: Power Pad B\n", port + 1);
			break;
		case DEVICE_SI_SNESGAMEPAD:
			type      = SI_SNES_GAMEPAD;
			inputDPtr = &nes_input.JoyButtons[port];
			FCEU_printf(" Player %u: SNES Pad\n", port + 1);
			break;
		case DEVICE_SI_SNESMOUSE:
			type      = SI_SNES_MOUSE;
			inputDPtr = &nes_input.MouseData[port];
			FCEU_printf(" Player %u: SNES Mouse\n", port + 1);
			break;
		case DEVICE_SI_VIRTUALBOY:
			type      = SI_VIRTUALBOY;
			inputDPtr = &nes_input.JoyButtons[port];
			FCEU_printf(" Player %u: Virtual Boy\n", port + 1);
			break;
		}

		FCEUI_SetInput(port, type, inputDPtr, attrib);
	} else if (port < MAX_CONTROLLERS) {
		if (nes_input.type[port] == DEVICE_SI_GAMEPAD) {
			FCEU_printf(" Player %u: Gamepad\n", port + 1);
		} else {
			FCEU_printf(" Player %u: None\n", port + 1);
		}
	} else {
		/* famicom expansion port */
		switch (nes_input.type[4]) {
		case DEVICE_SIFC_ARKANOID:
			type      = SIFC_ARKANOID;
			inputDPtr = nes_input.FamicomData;
			FCEU_printf(" Famicom Expansion: Arkanoid Paddle\n");
			break;
		case DEVICE_SIFC_SHADOW:
			type      = SIFC_SHADOW;
			inputDPtr = nes_input.FamicomData;
			attrib    = 1;
			FCEU_printf(" Famicom Expansion: Hyper Shot Gun\n");
			break;
		case DEVICE_SIFC_OEKAKIDS:
			type      = SIFC_OEKAKIDS;
			inputDPtr = nes_input.FamicomData;
			attrib    = 1;
			FCEU_printf(" Famicom Expansion: Oeka Kids Tablet\n");
			break;
		case DEVICE_SIFC_4PLAYER:
			type      = SIFC_4PLAYER;
			inputDPtr = &nes_input.JSReturn;
			FCEU_printf(" Famicom Expansion: 4-Player Adapter\n");
			break;
		case DEVICE_SIFC_HORI4PLAYER:
			type      = SIFC_HORI4PLAYER;
			inputDPtr = &nes_input.JSReturn;
			FCEU_printf(" Famicom Expansion: Hori 4-Player Adapter\n");
			break;
		case DEVICE_SIFC_HYPERSHOT:
			type      = SIFC_HYPERSHOT;
			inputDPtr = nes_input.FamicomData;
			FCEU_printf(" Famicom Expansion: HyperShot Pads\n");
			break;
		case DEVICE_SIFC_FTRAINERA:
			type      = SIFC_FTRAINERA;
			inputDPtr = &nes_input.FCReturn;
			FCEU_printf(" Famicom Expansion: Family Trainer A\n");
			break;
		case DEVICE_SIFC_FTRAINERB:
			type      = SIFC_FTRAINERB;
			inputDPtr = &nes_input.FCReturn;
			FCEU_printf(" Famicom Expansion: Family Trainer B\n");
			break;
		case DEVICE_SIFC_PARTYTAP:
			type      = SIFC_PARTYTAP;
			inputDPtr = &nes_input.FCReturn;
			FCEU_printf(" Famicom Expansion: Party Tap\n");
			break;
		case DEVICE_SIFC_EXCITINGBOXING:
			type      = SIFC_EXCITINGBOXING;
			inputDPtr = &nes_input.FCReturn;
			FCEU_printf(" Famicom Expansion: Exciting Boxing Punching Bag\n");
			break;
		case RETRO_DEVICE_NONE:
		default:
			type = SIFC_NONE, inputDPtr = &Dummy;
			FCEU_printf(" Famicom Expansion: none\n");
			break;
		}

		FCEUI_SetInputFC(type, inputDPtr, attrib);
	}

	FCEUI_SetInputFourScore(FALSE);
	if (iNESCart.InputTypes == 0x02) { /* NES Four Score input is set */
		FCEUI_SetInputFourScore(TRUE);
	}
}

void input_update_descriptors(void) {
	struct retro_input_descriptor desc[128] = { 0 };

	int i, port;

	for (i = 0, port = 0; port < 5; port++) {
		if (nes_input.type[port] == DEVICE_SI_GAMEPAD || nes_input.type[port] == RETRO_DEVICE_JOYPAD ||
		    nes_input.type[port] == DEVICE_SI_SNESGAMEPAD) {
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right");
			if (nes_input.type[port] == DEVICE_SI_SNESGAMEPAD) {
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_A, "A");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B, "B");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_X, "X");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_Y, "Y");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L, "L");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R, "R");
			} else {
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B, "B");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_A, "A");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_X, "Turbo A");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_Y, "Turbo B");
			}

			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_START, "Start");

			if (port == 0) {
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L3, "(Famicom) Microphone (P2)");

				if (GameInfo->type == GIT_FDS) {
					addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L, "(FDS) Disk Side Change");
					addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R, "(FDS) Insert/Eject Disk");
				}

				if (palette_switch_enabled)
					addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L2, "Switch Palette (+ Left/Right)");
			}

			if (GameInfo->type == GIT_VSUNI) {
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L2, "(VS) Insert Coin (slot 1)");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R2, "(VS) Insert Coin (slot 2)");
			}

			if (iNESCart.mapper == 124) {
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L2, "Insert Coin");
				addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R2, "Insert Coin");
			}
		} else if (nes_input.type[port] == DEVICE_SIFC_FTRAINERA || nes_input.type[port] == DEVICE_SIFC_FTRAINERB ||
		           nes_input.type[port] == DEVICE_SI_POWERPADA || nes_input.type[port] == DEVICE_SI_POWERPADB) {
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B, "Powerpad B1");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_A, "Powerpad B2");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_Y, "Powerpad B3");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_X, "Powerpad B4");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L, "Powerpad B5");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R, "Powerpad B6");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_LEFT, "Powerpad B7");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Powerpad B8");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_UP, "Powerpad B9");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_DOWN, "Powerpad B10");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_SELECT, "Powerpad B11");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_START, "Powerpad B12");
		} else if (nes_input.type[port] == DEVICE_SIFC_PARTYTAP) {
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B, "Partytap P1");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_A, "Partytap P2");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_Y, "Partytap P3");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_X, "Partytap P4");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L, "Partytap P5");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R, "Partytap P6");
		} else if (nes_input.type[port] == DEVICE_SI_VIRTUALBOY) {
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B, "Virtual Boy D-Pad 2 Down");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_Y, "Virtual Boy D-Pad 2 Left");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_SELECT, "Virtual Boy Select");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_START, "Virtual Boy Start");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_UP, "Virtual Boy D-Pad 1 Up");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_DOWN, "Virtual Boy D-Pad 1 Down");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_LEFT, "Virtual Boy D-Pad 1 Left");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Virtual Boy D-Pad 1 Right");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_A, "Virtual Boy D-Pad 2 Right");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_X, "Virtual Boy D-Pad 2 Up");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L, "Virtual Boy L");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R, "Virtual Boy R");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L2, "Virtual Boy B");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R2, "Virtual Boy A");
		} else if (nes_input.type[port] == DEVICE_SIFC_EXCITINGBOXING) {
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B, "Left Hook");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_A, "Right Hook");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_Y, "Left Jab");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_X, "Right Jab");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L, "Body");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R, "Straight");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_LEFT, "Move Left");
			addDesc(&desc[i++], port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Move Right");
		}
	}

	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
}

static const unsigned joypad_map[] = {
	RETRO_DEVICE_ID_JOYPAD_B,
	RETRO_DEVICE_ID_JOYPAD_A,
	RETRO_DEVICE_ID_JOYPAD_Y,
	RETRO_DEVICE_ID_JOYPAD_X,
	RETRO_DEVICE_ID_JOYPAD_L,
	RETRO_DEVICE_ID_JOYPAD_R,
	RETRO_DEVICE_ID_JOYPAD_LEFT,
	RETRO_DEVICE_ID_JOYPAD_RIGHT,
	RETRO_DEVICE_ID_JOYPAD_UP,
	RETRO_DEVICE_ID_JOYPAD_DOWN,
	RETRO_DEVICE_ID_JOYPAD_SELECT,
	RETRO_DEVICE_ID_JOYPAD_START,
	RETRO_DEVICE_ID_JOYPAD_L2,
	RETRO_DEVICE_ID_JOYPAD_R2,
	RETRO_DEVICE_ID_JOYPAD_L3,
	RETRO_DEVICE_ID_JOYPAD_R3,
};

static uint32 update_PowerPad(int w) {
	int x;
	uint32 r = 0;

	for (x = 0; x < 12; x++)
		r |= input_cb(w, RETRO_DEVICE_JOYPAD, 0, joypad_map[x]) ? (1 << x) : 0;

	return r;
}

static void update_FTrainer(void) {
	int x;
	uint32 r = 0;

	for (x = 0; x < 12; x++)
		r |= input_cb(4, RETRO_DEVICE_JOYPAD, 0, joypad_map[x]) ? (1 << x) : 0;

	nes_input.FCReturn = r;
}

static void update_PartyTap(void) {
	int x;
	uint8 r = 0;

	for (x = 0; x < 8; x++)
		r |= input_cb(4, RETRO_DEVICE_JOYPAD, 0, joypad_map[x]) ? (1 << x) : 0;

	nes_input.FCReturn = r;
}

static void update_ExcitingBoxing(void) {
	int x;
	uint8 r = 0;

	for (x = 0; x < 8; x++)
		r |= input_cb(4, RETRO_DEVICE_JOYPAD, 0, joypad_map[x]) ? (1 << x) : 0;

	nes_input.FCReturn = r;
}

static void update_HyperShotPad(void) {
	static int toggle;
	int i;

	nes_input.FamicomData[0] = 0;
	toggle ^= 1;
	for (i = 0; i < 2; i++) {
		if (input_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B))
			nes_input.FamicomData[0] |= 0x02 << (i * 2);
		else if (input_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)) {
			if (toggle)
				nes_input.FamicomData[0] |= 0x02 << (i * 2);
			else
				nes_input.FamicomData[0] &= ~(0x02 << (i * 2));
		}
		if (input_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A))
			nes_input.FamicomData[0] |= 0x04 << (i * 2);
		else if (input_cb(i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)) {
			if (toggle)
				nes_input.FamicomData[0] |= 0x04 << (i * 2);
			else
				nes_input.FamicomData[0] &= ~(0x04 << (i * 2));
		}
	}
}

static void update_Arkanoid(int port, enum RetroArkanoidInputModes mode, uint32_t *data) {
	static int mzx = 0, mzy = 0;
	int _x, _y, _pressed;

	data[2] = 0;

	switch (mode) {
	case RetroArkanoidMouse:
		mzx += (int)((double)nes_input.mouseSensitivity *
		             (double)input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X) / 100.0);
		mzy += (int)((double)nes_input.mouseSensitivity *
		             (double)input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y) / 100.0);

		if (mzx < 0)
			mzx = 0;
		else if (mzx > 240)
			mzx = 240;

		if (mzy < 0)
			mzy = 0;
		else if (mzy > NES_HEIGHT)
			mzy = NES_HEIGHT;

		data[0] = mzx;
		data[1] = mzy;

		if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT))
			data[2] |= 0x1;
		if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT))
			data[2] |= 0x2;
		break;

	case RetroArkanoidAbsMouse:
	case RetroArkanoidPointer:
		_x = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
		_y = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

		if (_x != 0 || _y != 0) {
			int32 raw = (_x + 0x7FFF) * NES_WIDTH / (0x7FFF * 2);
			if (nes_input.arkanoidMode == RetroArkanoidAbsMouse) {
				/* remap so full screen movement ends up within the encoder range 0-240 */
				/* game board: 176 wide */
				/* paddle: 32 */
				/* range of movement: 176-32 = 144 */
				/* left edge: 16 */
				/* right edge: 64 */

				/* increase movement by 10 to allow edges to be reached in case of problems */
				raw = (raw - 128) * 140 / 128 + 128;
				if (raw < 0)
					raw = 0;
				else if (raw > 255)
					raw = 255;

				data[0] = raw * 240 / 255;
			} else {
				/* remap so full board movement ends up within the encoder range 0-240 */
				if (data[0] < 16 + (32 / 2))
					data[0] = 0;
				else
					data[0] -= 16 + (32 / 2);
				if (data[0] > 144)
					data[0] = 144;
				data[0] = raw * 240 / 144;
			}
		}

		if (input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED))
			data[2] |= 0x1;

		break;

	case RetroArkanoidStelladaptor:
		_x       = input_cb(port, RETRO_DEVICE_ANALOG, 0, RETRO_DEVICE_ID_ANALOG_X);
		_pressed = input_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) |
		           input_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);

		data[0] = (_x + 32768) * 240 / 65535;

		if (_pressed)
			data[2] |= 0x1;

		break;
	}
}

static void update_Zapper(unsigned port, enum RetroZapperInputModes mode, uint32_t *data) {
	static int mzx = 0, mzy = 0;
	int _x, _y;

	data[2] = 0; /* reset click state */

	switch (mode) {
	case RetroMouse: /* mouse device */
		/* TODO: Add some sort of mouse sensitivity */
		mzx += (int)((double)nes_input.mouseSensitivity *
		             (double)input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X) / 100.0);
		mzy += (int)((double)nes_input.mouseSensitivity *
		             (double)input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y) / 100.0);

		/* Set crosshair within the limits of current screen resolution */
		if (mzx < 0)
			mzx = 0;
		else if (mzx > NES_WIDTH)
			mzx = NES_WIDTH;

		if (mzy < 0)
			mzy = 0;
		else if (mzy > NES_HEIGHT)
			mzy = NES_HEIGHT;

		data[0] = mzx;
		data[1] = mzy;

		if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT))
			data[2] |= 0x1;
		if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT))
			data[2] |= 0x2;
		break;

	case RetroPointer:
		_x = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
		_y = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

		if (_x == 0 && _y == 0) {
			data[0] = 0;
			data[1] = 0;
		} else {
			data[0] = (_x + 0x7FFF) * NES_WIDTH / (0x7FFF * 2);
			data[1] = (_y + 0x7FFF) * NES_HEIGHT / (0x7FFF * 2);
		}

		if (input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED))
			data[2] |= 0x1;
		break;

	case RetroLightgun: /* crosshair lightgun device */
	{
		int offscreen      = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN);
		int offscreen_shot = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_RELOAD);
		int trigger        = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER);

		if (offscreen || offscreen_shot) {
			data[0] = 0;
			data[1] = 0;
		} else {
			_x = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X);
			_y = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y);

			data[0] = (_x + 0x7FFF) * NES_WIDTH / (0x7FFF * 2);
			data[1] = (_y + 0x7FFF) * NES_HEIGHT / (0x7FFF * 2);
		}

		if (trigger || offscreen_shot)
			data[2] |= 0x1;

		break;
	}

	case RetroSTLightgun: /* Sequential targets lightgun device integration */
		data[2] = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER);
		data[3] = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_AUX_A);
		break;
	}
}

void input_update(retro_input_state_t *_input_cb) {
	unsigned player, port;
	bool palette_prev = FALSE;
	bool palette_next = FALSE;

	if (nes_input.needs_update) {
		/* since you can only update input descriptors all at once, its better
		 * to place this callback here, so descriptor labels can be updated in
		 * real-time when inputs gets changed */
		nes_input.needs_update = FALSE;
		input_update_descriptors();
	}

	input_cb = *_input_cb;

	/* Reset input states */
	nes_input.JSReturn = 0;

	/* nes gamepad */
	for (player = 0; player < MAX_CONTROLLERS; player++) {
		int i             = 0;
		uint8_t input_buf = 0;
		bool player_enabled =
		    (nes_input.type[player] == DEVICE_SI_GAMEPAD) || (nes_input.type[player] == RETRO_DEVICE_JOYPAD);

		if (player_enabled) {
			int16_t ret                  = 0;
			bool dpad_enabled            = TRUE;
			static int last_pressed_keys = 0;

			if (libretro_supports_bitmasks) {
				ret = input_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);
			} else {
				for (i = 0; i <= RETRO_DEVICE_ID_JOYPAD_R3; i++)
					ret |= input_cb(player, RETRO_DEVICE_JOYPAD, 0, i) ? (1 << i) : 0;
			}

			/* If palette switching is enabled, check if
			 * player 1 has the L2 button held down */
			if ((player == 0) && palette_switch_enabled && (ret & (1 << RETRO_DEVICE_ID_JOYPAD_L2))) {
				/* D-Pad left/right are used to switch palettes */
				palette_prev = (bool)(ret & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT));
				palette_next = (bool)(ret & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT));

				/* Regular D-Pad input is disabled */
				dpad_enabled = FALSE;
			}

			if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_A))
				input_buf |= JOY_A;
			if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_B))
				input_buf |= JOY_B;
			if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_SELECT))
				input_buf |= JOY_SELECT;
			if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_START))
				input_buf |= JOY_START;

			if (dpad_enabled) {
				if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_UP))
					input_buf |= JOY_UP;
				if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_DOWN))
					input_buf |= JOY_DOWN;
				if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT))
					input_buf |= JOY_LEFT;
				if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT))
					input_buf |= JOY_RIGHT;
			}

			if (player == 0) {
				if (!(last_pressed_keys & (1 << RETRO_DEVICE_ID_JOYPAD_L3)) && (ret & (1 << RETRO_DEVICE_ID_JOYPAD_L3)))
					FSettings.ReplaceP2StartWithMicrophone = !FSettings.ReplaceP2StartWithMicrophone;
				last_pressed_keys = ret;
			}

			/* Turbo A and Turbo B buttons are
			 * mapped to Joypad X and Joypad Y
			 * in RetroArch joypad.
			 *
			 * We achieve this by keeping track of
			 * the number of times it increments
			 * the toggle counter and fire or not fire
			 * depending on whether the delay value has
			 * been reached.
			 *
			 * Each turbo button is activated by
			 * corresponding mapped button
			 * OR mapped Turbo A+B button.
			 * This allows Turbo A+B button to use
			 * the same toggle counters as Turbo A
			 * and Turbo B buttons use separately.
			 */

			if (nes_input.turbo_enabled[player]) {
				/* Handle Turbo A, B & A+B buttons */
				for (i = 0; i < TURBO_BUTTONS; i++) {
					if (input_cb(player, RETRO_DEVICE_JOYPAD, 0, turbomap[i].retro)) {
						if (!turbo_button_toggle[player][i])
							input_buf |= turbomap[i].nes;
						turbo_button_toggle[player][i]++;
						turbo_button_toggle[player][i] %= nes_input.turbo_delay + 1;
					} else
						/* If the button is not pressed, just reset the toggle */
						turbo_button_toggle[player][i] = 0;
				}
			}
		}

		if (!nes_input.allow_updown_leftright) {
			if ((input_buf & JOY_UP) && (input_buf & JOY_DOWN))
				input_buf &= ~(JOY_UP | JOY_DOWN);
			if ((input_buf & JOY_LEFT) && (input_buf & JOY_RIGHT))
				input_buf &= ~(JOY_LEFT | JOY_RIGHT);
		}

		nes_input.JSReturn |= (input_buf & 0xff) << (player << 3);
	}

	/* other inputs*/
	for (port = 0; port < MAX_PORTS; port++) {
		int device = nes_input.type[port];

		switch (device) {
		case DEVICE_SI_SNESMOUSE: {
			int dx = input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
			int dy = input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
			int mb = ((input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT) ? 1 : 0) |
			          (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT) ? 2 : 0));
			nes_input.MouseData[port][0] = dx;
			nes_input.MouseData[port][1] = dy;
			nes_input.MouseData[port][2] = mb;
		} break;
		case DEVICE_SI_ARKANOID:
			update_Arkanoid(port, nes_input.arkanoidMode, nes_input.MouseData[port]);
			break;
		case DEVICE_SI_ZAPPER:
			update_Zapper(port, nes_input.zapperMode, nes_input.MouseData[port]);
			break;
		case DEVICE_SI_POWERPADA:
		case DEVICE_SI_POWERPADB:
			nes_input.PowerPadData[port] = update_PowerPad(port);
			break;
		case DEVICE_SI_SNESGAMEPAD:
		case DEVICE_SI_VIRTUALBOY: {
			int i;
			int ret                    = 0;
			nes_input.JoyButtons[port] = 0;
			for (i = 0; i <= RETRO_DEVICE_ID_JOYPAD_R3; i++)
				ret |= input_cb(port, RETRO_DEVICE_JOYPAD, 0, i) ? (1 << i) : 0;
			nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_UP)) ? JOY_UP : 0;
			nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_DOWN)) ? JOY_DOWN : 0;
			nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT)) ? JOY_LEFT : 0;
			nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT)) ? JOY_RIGHT : 0;
			if (!nes_input.allow_updown_leftright) {
				if ((nes_input.JoyButtons[port] & JOY_UP) && (nes_input.JoyButtons[port] & JOY_DOWN))
					nes_input.JoyButtons[port] &= ~(JOY_UP | JOY_DOWN);
				if ((nes_input.JoyButtons[port] & JOY_LEFT) && (nes_input.JoyButtons[port] & JOY_RIGHT))
					nes_input.JoyButtons[port] &= ~(JOY_LEFT | JOY_RIGHT);
			}
			if (nes_input.type[port] == DEVICE_SI_SNESGAMEPAD) {
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_B)) ? (1 << 0) : 0;
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_Y)) ? (1 << 1) : 0;
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_A)) ? (1 << 8) : 0;
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_X)) ? (1 << 9) : 0;
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_L)) ? (1 << 10) : 0;
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_R)) ? (1 << 11) : 0;
			} else if (nes_input.type[port] == DEVICE_SI_VIRTUALBOY) {
#define RIGHT_DPAD_DOWN  (1 << 0)
#define RIGHT_DPAD_LEFT  (1 << 1)
#define RIGHT_DPAD_RIGHT (1 << 8)
#define RIGHT_DPAD_UP    (1 << 9)
				nes_input.JoyButtons[port] |=
				    (ret & (1 << RETRO_DEVICE_ID_JOYPAD_B)) ? (1 << 0) : 0; /* Right D-pad Down */
				nes_input.JoyButtons[port] |=
				    (ret & (1 << RETRO_DEVICE_ID_JOYPAD_Y)) ? (1 << 1) : 0; /* Right D-pad Left */
				nes_input.JoyButtons[port] |=
				    (ret & (1 << RETRO_DEVICE_ID_JOYPAD_A)) ? (1 << 8) : 0; /* Right D-pad Right */
				nes_input.JoyButtons[port] |=
				    (ret & (1 << RETRO_DEVICE_ID_JOYPAD_X)) ? (1 << 9) : 0; /* Right D-pad Up */
				nes_input.JoyButtons[port] |=
				    (ret & (1 << RETRO_DEVICE_ID_JOYPAD_L)) ? (1 << 10) : 0; /* Rear Left Trigger */
				nes_input.JoyButtons[port] |=
				    (ret & (1 << RETRO_DEVICE_ID_JOYPAD_R)) ? (1 << 11) : 0; /* Rear Left Trigger */
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_L2)) ? (1 << 12) : 0; /* B */
				nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_R2)) ? (1 << 13) : 0; /* A */

				if (!nes_input.allow_updown_leftright) {
					if ((nes_input.JoyButtons[port] & RIGHT_DPAD_DOWN) && (nes_input.JoyButtons[port] & RIGHT_DPAD_UP))
						nes_input.JoyButtons[port] &= ~(RIGHT_DPAD_DOWN | RIGHT_DPAD_UP);
					if ((nes_input.JoyButtons[port] & RIGHT_DPAD_LEFT) &&
					    (nes_input.JoyButtons[port] & RIGHT_DPAD_RIGHT))
						nes_input.JoyButtons[port] &= ~(RIGHT_DPAD_LEFT | RIGHT_DPAD_RIGHT);
				}
#undef RIGHT_DPAD_DOWN
#undef RIGHT_DPAD_LEFT
#undef RIGHT_DPAD_RIGHT
#undef RIGHT_DPAD_UP
			}

			nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_SELECT)) ? JOY_SELECT : 0;
			nes_input.JoyButtons[port] |= (ret & (1 << RETRO_DEVICE_ID_JOYPAD_START)) ? JOY_START : 0;

		} break;
		default:
			break;
		}
	}

	/* famicom inputs */
	switch (nes_input.type[4]) {
	case DEVICE_SIFC_ARKANOID:
		update_Arkanoid(0, nes_input.arkanoidMode, nes_input.FamicomData);
		break;
	case DEVICE_SIFC_OEKAKIDS: /* mouse device */
	case DEVICE_SIFC_SHADOW:
		update_Zapper(0, nes_input.zapperMode, nes_input.FamicomData);
		break;
	case DEVICE_SIFC_FTRAINERA:
	case DEVICE_SIFC_FTRAINERB:
		update_FTrainer();
		break;
	case DEVICE_SIFC_PARTYTAP:
		update_PartyTap();
		break;
	case DEVICE_SIFC_EXCITINGBOXING:
		update_ExcitingBoxing();
		break;
	case DEVICE_SIFC_HYPERSHOT: {
		update_HyperShotPad();
		break;
	}
	}

	for (port = 0; port < 2; port++) {
		if (input_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2))
			FCEUI_VSUniCoin(); /* Insert Coin VS System slot 1 */
		if (input_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2))
			FCEUI_VSUniCoin2(); /* Insert Coin VS System slot 2 */
	}

	if (GameInfo->type == GIT_FDS) /* Famicom Disk System */
	{
		bool curL         = input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
		bool curR         = input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R);
		static bool prevL = FALSE, prevR = FALSE;

		if (curL && !prevL)
			FCEUI_FDSSelect(); /* Swap FDisk side */
		prevL = curL;

		if (curR && !prevR)
			FCEUI_FDSInsert(); /* Insert or eject the disk */
		prevR = curR;
	}

	/* Handle internal palette switching */
	input_palette_switch(palette_next, palette_prev);

	/* Power Switch on F12 */
	if (input_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F12)) {
		FCEUI_PowerNES();
	}
}

void input_init_env(retro_environment_t *_environ_cb) {
	environ_cb = *_environ_cb;
	environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void *)ports);
}

void input_set_controller_port_device(unsigned port, unsigned device) {
	if (device == DEVICE_AUTO) {
		if (port <= 1) {
			switch (GameInfo->input[port]) {
			case SI_GAMEPAD:
				nes_input.type[port] = DEVICE_SI_GAMEPAD;
				break;
			case SI_ZAPPER:
				nes_input.type[port] = DEVICE_SI_ZAPPER;
				break;
			case SI_ARKANOID:
				nes_input.type[port] = DEVICE_SI_ARKANOID;
				break;
			case SI_POWERPADA:
				nes_input.type[port] = DEVICE_SI_POWERPADA;
				break;
			case SI_POWERPADB:
				nes_input.type[port] = DEVICE_SI_POWERPADB;
				break;
			case SI_SNES_GAMEPAD:
				nes_input.type[port] = DEVICE_SI_SNESGAMEPAD;
				break;
			case SI_SNES_MOUSE:
				nes_input.type[port] = DEVICE_SI_SNESMOUSE;
				break;
			case SI_VIRTUALBOY:
				nes_input.type[port] = DEVICE_SI_VIRTUALBOY;
				break;
			default:
			case SI_UNSET:
			case SI_NONE:
			case SI_MOUSE:
				/* unsupported devices */
				nes_input.type[port] = DEVICE_SI_GAMEPAD;
				break;
			}
		} else if (port <= 3) {
			if (!nes_input.enable_4player &&
			    ((nes_input.type[4] == DEVICE_SIFC_4PLAYER) ||
				 (nes_input.type[4] == DEVICE_SIFC_HORI4PLAYER) ||
				 (iNESCart.InputTypes == 0x02))) {
				nes_input.enable_4player = TRUE;
			}
			if (nes_input.enable_4player) {
				nes_input.type[port] = DEVICE_SI_GAMEPAD;
			} else {
				nes_input.type[port] = RETRO_DEVICE_NONE;
			}
		} else {
			/* famicom expansion port */
			switch (GameInfo->inputfc) {
			case SIFC_UNSET:
			case SIFC_NONE:
				nes_input.type[4] = RETRO_DEVICE_NONE;
				break;
			case SIFC_ARKANOID:
				nes_input.type[4] = DEVICE_SI_ARKANOID;
				break;
			case SIFC_SHADOW:
				nes_input.type[4] = DEVICE_SIFC_SHADOW;
				break;
			case SIFC_4PLAYER:
				nes_input.type[4] = DEVICE_SIFC_4PLAYER;
				break;
			case SIFC_HORI4PLAYER:
				nes_input.type[4] = DEVICE_SIFC_HORI4PLAYER;
				break;
			case SIFC_HYPERSHOT:
				nes_input.type[4] = DEVICE_SIFC_HYPERSHOT;
				break;
			case SIFC_OEKAKIDS:
				nes_input.type[4] = DEVICE_SIFC_OEKAKIDS;
				break;
			case SIFC_FTRAINERA:
				nes_input.type[4] = DEVICE_SIFC_FTRAINERA;
				break;
			case SIFC_FTRAINERB:
				nes_input.type[4] = DEVICE_SIFC_FTRAINERB;
				break;
			case SIFC_PARTYTAP:
				nes_input.type[4] = DEVICE_SIFC_PARTYTAP;
				break;
			case SIFC_EXCITINGBOXING:
				nes_input.type[4] = DEVICE_SIFC_EXCITINGBOXING;
				break;
			default:
				/* unsupported input device */
				nes_input.type[4] = RETRO_DEVICE_NONE;
				break;
			}
		}
	} else {
		nes_input.type[port] = device;
	}

	set_input(port);

	nes_input.needs_update = TRUE;
}

void input_set_defaults(void) {
	int i;
	for (i = 0; i < MAX_PORTS; i++) {
		FCEUI_SetInput(i, SI_GAMEPAD, &nes_input.JSReturn, 0);
		nes_input.type[i] = RETRO_DEVICE_JOYPAD;
	}

	FCEUI_SetInputFourScore(FALSE);

	for (i = 0; i < fourscore_len; i++) {
		if (fourscore_db_list[i].crc == iNESCart.CRC32) {
			FCEUI_SetInputFourScore(TRUE);
			nes_input.enable_4player = TRUE;
			FCEU_printf(" NES Four Score compatible game found.\n");
			FCEU_printf(" Name: %s\n", fourscore_db_list[i].title);
			break;
		}
	}

	for (i = 0; i < famicom_4p_len; i++) {
		if (famicom_4p_db_list[i].crc == iNESCart.CRC32) {
			GameInfo->inputfc = SIFC_4PLAYER;
			FCEUI_SetInputFC(SIFC_4PLAYER, &nes_input.JSReturn, 0);
			nes_input.enable_4player = TRUE;
			FCEU_printf(" Famicom 4-Player Adaptor compatible game found.\n");
			FCEU_printf(" Name: %s\n", famicom_4p_db_list[i].title);
			break;
		}
	}

	nes_input.needs_update = TRUE;
}

void input_allow_updown_leftright(bool value) {
	nes_input.allow_updown_leftright = value;
}

void input_set_zapper_mode(enum RetroZapperInputModes value) {
	nes_input.zapperMode = value;
}

void input_set_arkanoid_mode(enum RetroArkanoidInputModes value) {
	nes_input.arkanoidMode = value;
}

void input_set_mousesensitivity(double value) {
	nes_input.mouseSensitivity = value;
}

void input_enable_turbo_buttons(int port, bool value) {
	nes_input.turbo_enabled[port] = value;
}

void input_set_turbo_delay(int value) {
	nes_input.turbo_delay = value;
}
