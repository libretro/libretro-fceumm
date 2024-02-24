#ifndef __LIBRETRO_INPUT_H
#define __LIBRETRO_INPUT_H

#include <libretro.h>

#define fourscore_len   (int)(sizeof(fourscore_db_list)   / sizeof(fourscore_db_list[0]))
#define famicom_4p_len  (int)(sizeof(famicom_4p_db_list) / sizeof(famicom_4p_db_list[0]))

typedef struct cartridge_db {
	uint32_t crc;
	char title[256];
} cartridge_db_t;

static const struct cartridge_db fourscore_db_list[] = {
	{ 0x1EBB5B42, "Bomberman II (USA)" },
#if 0
	{ 0xeac38105, "Championship Bowling (USA)" },
#endif
	{ 0xF99E37EB, "Chris Evert & Ivan Lendl in Top Players' Tennis (USA)" },
#if 0
	{ "Crash 'n' the Boys - Street Challenge (USA)", 0xc7f0c457 },
#endif
	{ 0x48B8EE58, "Four Players' Tennis (Europe)" },
	{ 0x27CA0679, "Danny Sullivan's Indy Heat (Europe)" },
	{ 0x79F688BC, "Gauntlet II (Europe)" },
	{ 0x1B71CCDB, "Gauntlet II (USA)" },
	{ 0x1352F1B9, "Greg Norman's Golf Power (USA)" },
	{ 0x2E6EE98D, "Harlem Globetrotters (USA)" },
	{ 0x05104517, "Ivan 'Ironman' Stewart's Super Off Road (Europe)" },
	{ 0x4B041B6B, "Ivan 'Ironman' Stewart's Super Off Road (USA)" },
	{ 0xF54B34BD, "Kings of the Beach - Professional Beach Volleyball (USA)" },
	{ 0xC6C2EDB5, "Magic Johnson's Fast Break (USA)" },
	{ 0x0939852F, "M.U.L.E. (USA)" },
	{ 0x4E6B9078, "Micro Mages" },
	{ 0x2F698C4D, "Monster Truck Rally (USA)" },
	{ 0xB9B4D9E0, "NES Play Action Football (USA)" },
	{ 0xDA2CB59A, "Nightmare on Elm Street, A (USA)" },
	{ 0x8DA6667D, "Nintendo World Cup (Europe)" },
	{ 0x7C16F819, "Nintendo World Cup (Europe) (Rev A)" },
	{ 0x7F08D0D9, "Nintendo World Cup (Europe) (Rev B)" },
	{ 0xA22657FA, "Nintendo World Cup (USA)" },
	{ 0x308DA987, "R.C. Pro-Am II (Europe)" },
	{ 0x9EDD2159, "R.C. Pro-Am II (USA)" },
	{ 0x8FA6E92C, "Rackets & Rivals (Europe)" },
	{ 0xAD0394F0, "Roundball - 2-on-2 Challenge (Europe)" },
	{ 0x6E4DCFD2, "Roundball - 2-on-2 Challenge (USA)" },
	{ 0x0ABDD5CA, "Spot - The Video Game (Japan)" },
	{ 0xCFAE9DFA, "Spot - The Video Game (USA)" },
	{ 0x0B8F8128, "Smash T.V. (Europe)" },
	{ 0x6EE94D32, "Smash T.V. (USA)" },
	{ 0xCF4487A2, "Super Jeopardy! (USA)" },
	{ 0xC05A63B2, "Super Spike V'Ball (Europe)" },
	{ 0xE840FD21, "Super Spike V'Ball (USA)" },
	{ 0x407D6FFD, "Super Spike V'Ball + Nintendo World Cup (USA)" },
	{ 0xD153CAF6, "Swords and Serpents (Europe)" },
	{ 0x46135141, "Swords and Serpents (France)" },
	{ 0x3417EC46, "Swords and Serpents (USA)" },
	{ 0x69977C9E, "Battle City (Japan) (4 Players Hack) http://www.romhacking.net/hacks/2142/" },
	{ 0x2DA5ECE0, "Bomberman 3 (Homebrew) http://tempect.de/senil/games.html" },
	{ 0x90D2E9F0, "K.Y.F.F. (Homebrew) http://slydogstudios.org/index.php/k-y-f-f/" },
	{ 0x1394DED0, "Super PakPak (Homebrew) http://wiki.nesdev.com/w/index.php/Super_PakPak" },
	{ 0x73298C87, "Super Mario Bros. + Tetris + Nintendo World Cup (Europe)" },
	{ 0xF46EF39A, "Super Mario Bros. + Tetris + Nintendo World Cup (Europe) (Rev A)" },
};

static const struct cartridge_db famicom_4p_db_list[] = {
	{ 0xC39B3BB2, "Bakutoushi Patton-Kun (Japan) (FDS)" },
	{ 0x0C401790, "Bomber Man II (Japan)" },
	{ 0x9992F445, "Championship Bowling (Japan)" },
	{ 0x3E470FE0, "Downtown - Nekketsu Koushinkyoku - Soreyuke Daiundoukai (Japan)" },
	{ 0x4F032933, "Ike Ike! Nekketsu Hockey-bu - Subette Koronde Dairantou (Japan)" },
	{ 0x4B5177E9, "Kunio-kun no Nekketsu Soccer League (Japan)" },
	{ 0x9F03B11F, "Moero TwinBee - Cinnamon Hakase o Sukue! (Japan)" },
	{ 0x13205221, "Moero TwinBee - Cinnamon Hakase wo Sukue! (Japan) (FDS)" },
	{ 0x37E24797, "Nekketsu Kakutou Densetsu (Japan)" },
	{ 0x62C67984, "Nekketsu Koukou Dodgeball-bu (Japan)" },
	{ 0x88062D9A, "Nekketsu! Street Basket - Ganbare Dunk Heroes (Japan)" },
	{ 0x689971F9, "Super Dodge Ball (USA) (3-4p with Game Genie code GEUOLZZA)" },
	{ 0x4FF17864, "Super Dodge Ball (USA) (patched) http://www.romhacking.net/hacks/71/" },
	{ 0x213CB3FB, "U.S. Championship V'Ball (Japan)" },
	{ 0xD7077D96, "U.S. Championship V'Ball (Japan) (Beta)" },
	{ 0xB1B16B8A, "Wit's (Japan)" },
};

#define MAX_CONTROLLERS 4 /* max supported players */
#define MAX_PORTS       2 /* max controller ports. port 0 for player 1/3, port 1 for player 2/4 */

enum RetroZapperInputModes {
	RetroLightgun,
	RetroSTLightgun,
	RetroMouse,
	RetroPointer
};

enum RetroArkanoidInputModes {
	RetroArkanoidMouse,
	RetroArkanoidPointer,
	RetroArkanoidAbsMouse,
	RetroArkanoidStelladaptor
};

typedef struct NES_INPUT_T {
	bool enable_4player;  /* four-score / 4-player adapter used */
	bool allow_updown_leftright; /* disabled simultaneous up+down and left+right dpad combinations */
	bool needs_update;

	/* turbo related */
	bool turbo_enabled[MAX_CONTROLLERS];
	int turbo_delay;

	int type[MAX_CONTROLLERS + 1]; /* 4-players + famicom expansion */

	/* input data */
	uint32_t JSReturn; /* player input data, 1 byte per player (1-4) */
	uint32_t JoyButtons[2];
	uint32_t MouseData[MAX_PORTS][4]; /* nes mouse data */
	uint32_t FamicomData[3];          /* Famicom expansion port data */
	uint32_t PowerPadData[2];         /* NES Power Pad data */
	uint32_t FCReturn;                /* Expansion Data */

	enum RetroZapperInputModes zapperMode;
	enum RetroArkanoidInputModes arkanoidMode;
	double mouseSensitivity;
} NES_INPUT_T;

void input_init_env(retro_environment_t *_environ_cb);
void input_set_controller_port_device(unsigned port, unsigned device);
void input_update_descriptors(void);
void input_update(retro_input_state_t *input_cb);
void input_set_defaults(void);
void input_palette_switch(bool, bool);

void input_allow_updown_leftright(bool);
void input_set_zapper_mode(enum RetroZapperInputModes mode);
void input_set_arkanoid_mode(enum RetroArkanoidInputModes mode);
void input_set_mousesensitivity(double value);
void input_enable_turbo_buttons(int port, bool enable);
void input_set_turbo_delay(int value);

extern bool libretro_supports_bitmasks;
extern bool palette_switch_enabled;

#endif /* __LIBRETRO_INPUT_H */