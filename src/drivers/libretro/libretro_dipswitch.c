#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../fceu.h"
#include "../../fceu-types.h"
#include "../../vsuni.h"
#include "../../git.h"
#include "../../driver.h"
#include "../../cart.h"

#include "libretro_dipswitch.h"

extern CartInfo iNESCart;

typedef struct {
   const char  *name;
   uint8       value;
} SETTING;

typedef struct {
   const char  *option_name;
   uint8       mask;
   SETTING     settings[10];
} DIPSWITCH;

typedef struct {
   unsigned    gameid;
   const char  *romname_short;
   DIPSWITCH   dipswitch_core_options[8];
} VSUNIGAME;

static VSUNIGAME dipswitch_battlecity = {
   VS_BATTLECITY, "battlecity",
   {
      {  "Credits for 2 Players", 0x01,
         {
            { "2", 0x01 },
            { "1", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x02,
         {
            { "5", 0x02 },
            { "3", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Demo Sounds", 0x04,
         {
            { "disabled", 0x04 },
            { "enabled",  0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   },
};

static VSUNIGAME dipswitch_castlevania = {
   VS_CASTLEVANIA, "castlevania",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x08,
         {
            { "3", 0x00 },
            { "2", 0x08 }

         }
      },
      {  "Bonus", 0x30,
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x40,
         {
            { "Normal", 0x00 },
            { "Hard",   0x40 },
            { NULL, 0 },
         },
      },

      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_clucluland = {
   VS_CLUCLULAND, "clucluland",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x60,
         {
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 },
            { "2", 0x60 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_drmario = {
   VS_DRMARIO, "drmario",
   {
      {  "Drop Rate Increase After", 0x03,
         {
            { "7 Pills",  0x00 },
            { "8 Pills",  0x01 },
            { "9 Pills",  0x02 },
            { "10 Pills", 0x03 }
         },
      },
      {  "Virus Level", 0x0c,
         {
            { "1", 0x00 },
            { "3", 0x04 },
            { "5", 0x08 },
            { "7", 0x0c }
         },
      },
      {  "Drop Speed Up", 0x30,
         {
            { "Slow",    0x00 },
            { "Medium",  0x10 },
            { "Fast",    0x20 },
            { "Fastest", 0x30 }
         },
      },
      {  "Free Play", 0x40,
         {
            { "disabled", 0x00 },
            { "enabled",  0x40 }
         },
      },
      {  "Demo Sounds", 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_duckhunt = {
   VS_DUCKHUNT, "duckhunt",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x18,
         {
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",   0x18 },
            { "Easy",   0x00 },
            { NULL, 0 },
         },
      },
      {  "Miss per game", 0x20,
         {
            { "5", 0x20 },
            { "3", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life", 0xc0,
         {
            { "30000",  0x00 },
            { "50000",  0x40 },
            { "80000",  0x80 },
            { "100000", 0xc0 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_excitebike = {
   VS_EXITEBIKE, "excitebike",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Bonus Bike", 0x18,
         {
            { "100k and Every 50k", 0x00 },
            { "100k Only",          0x08 },
            { "Every 100k",         0x10 },
            { "None",               0x18 },
            { NULL, 0 },
         },
      },
      {  "1st Qualifying Time", 0x20,
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 },
            { NULL, 0 },
         },
      },
      {  "2nd Qualifying Time", 0x40,
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_freedomforce = {
   VS_FREEDOMFORCE, "freedomforce",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Health Awarded At", 0x18,
         {
            { "10k 50k",           0x10 },
            { "20k 60k",           0x00 },
            { "20k 60k Every 60k", 0x08 },
            { "10k 50k Every 50k", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Difficulty (Damage)", 0x60,
         {
            { "Normal",  0x00 },
            { "Easy",    0x40 },
            { "Hard",    0x20 },
            { "Hardest", 0x60 },
            { NULL, 0 },
         },
      },
      {  "Difficulty (Enemy)", 0x80,
         {
            { "Normal", 0x80 },
            { "Hard",   0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_goonies = {
   VS_GOONIES, "goonies",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x08,
         {
            { "3", 0x08 },
            { "4", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus", 0x30,
         {
            { "40000", 0x00 },
            { "50000", 0x20 },
            { "60000", 0x10 },
            { "70000", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Timer", 0x40,
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 },
            { NULL, 0 },
         },
      },
      { "Demo Sounds", 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_gradius = {
   VS_GRADIUS, "gradius",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x08,
         {
            { "3", 0x08 },
            { "4", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus", 0x30,
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x40,
         {
            { "Normal", 0x00 },
            { "Hard",   0x40 },
            { NULL, 0 },
         },
      },
      { "Demo Sounds", 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_gumshoe = {
   VS_GUMSHOE, "gumshoe",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x18,
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x20,
         {
            { "3", 0x20 },
            { "5", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bullets per Balloon", 0x40,
         {
            { "3", 0x00 },
            { "2", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Bonus Man Awarded", 0x80,
         {
            { "At 50000", 0x00 },
            { "None",     0x80 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_hogansalley = {
   VS_HOGANSALLEY, "hogansalley",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x18,
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Misses Per Game", 0x20,
         {
            { "5", 0x20 },
            { "3", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life", 0xc0,
         {
            { "30000",  0x00 },
            { "50000",  0x40 },
            { "80000",  0x80 },
            { "100000", 0xc0 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_iceclimber = {
   VS_ICECLIMBER, "iceclimber",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x18,
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 },
            { "7", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x20,
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 },
            { NULL, 0 },
         },
      },
      {  "Time before bear appears", 0x40,
         {
            { "Long",  0x00 },
            { "Short", 0x40 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_jajamaru = {
   VS_JAJAMARU, "jajamaru",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x18,
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 },
            { NULL, 0 },
         },
      },
      {  "Demo Sounds", 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_machrider = {
   VS_MACHRIDER, "machrider",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Time", 0x18,
         {
            { "280 (Easy)", 0x00 },
            { "250",        0x10 },
            { "220",        0x08 },
            { "200 (Hard)", 0x18 },
            { NULL, 0 },
         },
      },
      {  "Enemies", 0x40,
         {
            { "Less", 0x00 },
            { "More", 0x40 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_mightybj = {
   VS_MIGHTYBOMBJACK, "mightybj",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "5 Coins 1 Credit", 0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x18,
         {
            { "3", 0x00 },
            { "4", 0x08 },
            { "5", 0x18 },
            { "2", 0x10 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_pinball = {
   VS_PINBALL, "pinball",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x01 },
            { "1 Coin 2 Credits", 0x06 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x04 },
            { "2 Coins 1 Credit", 0x05 },
            { "3 Coins 1 Credit", 0x03 },
            { "4 Coins 1 Credit", 0x07 },
            { "Free Play",        0x00 },
            { NULL, 0 },
         },
      },
      {  "Side Drain Walls", 0x08,
         {
            { "High", 0x00 },
            { "Low",  0x08 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life", 0x10,
         {
            { "50000", 0x00 },
            { "70000", 0x10 },
            { NULL, 0 },
         },
      },
      {  "Balls", 0x60,
         {
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 },
            { "2", 0x60 },
         }
      },
      {  "Ball Speed", 0x80,
         {
            { "Slow", 0x00 },
            { "Fast", 0x80 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_platoon = {
   VS_PLATOON, "platoon",
   {
      {  "Demo Sounds", 0x04,
         {
            { "enabled",  0x00 },
            { "disabled", 0x04 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x18,
         {
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",   0x18 },
            { "Easy",   0x00 },
         }
      },
      {  "Coinage", 0xe0,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x20 },
            { "1 Coin 3 Credits", 0x40 },
            { "2 Coins 1 Credit", 0x60 },
            { "3 Coins 1 Credit", 0x80 },
            { "4 Coins 1 Credit", 0xa0 },
            { "5 Coins 1 Credit", 0xc0 },
            { "Free Play",  0xe0 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_rbibaseball = {
   VS_RBIBASEBALL, "atari_rbibb",
   {
      {  "Coinage", 0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 },
            { NULL, 0 },
         },
      },
      {  "1p-inn 2p-inn time-min", 0x0c,
         {
            { "2 2 4", 0x0c },
            { "3 2 6", 0x00 },
            { "4 3 7", 0x08 },
            { "2 1 3", 0x04 },
         }
      },
      {  "Demo Sounds", 0x10,
         {
            { "enabled",  0x00 },
            { "disabled", 0x10 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_slalom = {
   VS_SLALOM, "slalom",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Freestyle Points", 0x08,
         {
            { "Hold Time",  0x08 },
            { "Left Right", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x30,
         {
            { "Normal", 0x10 },
            { "Medium", 0x20 },
            { "Hard",   0x30 },
            { "Easy",   0x00 },
            { NULL, 0 },
         },
      },
      {  "Allow Continue", 0x40,
         {
            { "enabled",  0x00 },
            { "disabled", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Inverted Input", 0x80,
         {
            { "disabled", 0x00 },
            { "enabled",  0x80 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_smb = {
   VS_SMB, "smb",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x06 },
            { "1 Coin 3 Credits", 0x01 },
            { "1 Coin 4 Credits", 0x05 },
            { "1 Coin 5 Credits", 0x03 },
            { "2 Coins 1 Credit", 0x04 },
            { "3 Coins 1 Credit", 0x02 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives", 0x08,
         {
            { "3", 0x00 },
            { "2", 0x08 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life", 0x30,
         {
            { "100 coins", 0x00 },
            { "150 coins", 0x20 },
            { "200 coins", 0x10 },
            { "250 coins", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Timer", 0x40,
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Continue Lives", 0x80,
         {
            { "3", 0x80 },
            { "4", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_soccer = {
   VS_SOCCER, "soccer",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 },
            { NULL, 0 },
         },
      },
      {  "Points Timer", 0x18,
         {
            { "1000 Pts", 0x08 },
            { "1200 Pts", 0x18 },
            { "600 Pts",  0x00 },
            { "800 Pts",  0x10 },
            { NULL, 0 },
         },
      },
      { "Difficulty", 0x60,
         {
            { "Normal", 0x40 },
            { "Medium", 0x20 },
            { "Hard",   0x60 },
            { "Easy",   0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_starluster = {
   VS_STARLUSTER, "starluster",
   {
      { "Coinage", 0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x03 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_superskykid = {
   VS_SUPERSKYKID, "superskykid",
   {
      {  "Difficulty", 0x03,
         {

            { "Normal", 0x01 },
            { "Medium", 0x02 },
            { "Hard",   0x03 },
            { "Easy",   0x00 },
         }
      },
      {  "Lives", 0x04,
         {
            { "3", 0x04 },
            { "2", 0x00 },
            { NULL, 0 },
         },
      },
      {  "Coinage", 0x18,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x08 },
            { "2 Coins 1 Credit", 0x10 },
            { "3 Coins 1 Credit", 0x18 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_superxevious = {
   VS_SUPERXEVIOUS, "superxevious",
   {
      {  "Coinage", 0x30,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x10 },
            { "2 Coins 1 Credit", 0x20 },
            { "3 Coins 1 Credit", 0x30 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_tetris = {
   VS_TETRIS, "tetris",
   {
      {  "Difficulty", 0x0c,
         {
            { "Normal", 0x04 },
            { "Medium", 0x08 },
            { "Hard",   0x0c },
            { "Easy",   0x00 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_tkoboxing = {
   VS_TKOBOXING, "tkoboxing",
   {
      {  "Coinage", 0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x0c,
         {
            { "Normal",    0x04 },
            { "Hard",      0x08 },
            { "Very Hard", 0x0c },
            { "Easy",      0x00 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

static VSUNIGAME dipswitch_topgun = {
   VS_TOPGUN, "topgun",
   {
      {  "Coinage", 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x06 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",  0x07 },
            { NULL, 0 },
         },
      },
      {  "Lives per Coin", 0x08,
         {
            { "3-12 Max", 0x00 },
            { "2-9 Max",  0x08 },
            { NULL, 0 },
         },
      },
      {  "Bonus Life", 0x30,
         {
            { "30k and Every 50k",   0x00 },
            { "50k and Every 100k",  0x20 },
            { "100k and Every 150k", 0x10 },
            { "200k and Every 200k", 0x30 },
            { NULL, 0 },
         },
      },
      {  "Difficulty", 0x40,
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 },
            { NULL, 0 },
         },
      },
      {  "Demo Sounds", 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 },
            { NULL, 0 },
         },
      },
      {  NULL, 0, { NULL, 0 } }
   }
};

#define DPSW_MAX 6

enum {
   DPSW_NONE = 0,
   DPSW_VSUNI,
   DPSW_NWC,
};

static struct retro_core_option_definition option_defs_empty = {
   NULL, NULL, NULL, { NULL, NULL }, NULL
};

static struct retro_core_option_definition vscoreopt[DPSW_MAX];
static VSUNIGAME *vsgame = NULL;
static char *core_key[DPSW_MAX];
static unsigned num_core_options;
static unsigned dipswitch_type = DPSW_NONE;

static const char *str_to_corekey(char *s)
{
   char *str = s;
   unsigned i = 0;

   while (str[i] != '\0')
   {
      if (str[i] == ' ')
         str[i] = '_';
      i++;
   }
   str[i] = '\0';

   return str;
}

static void make_core_options(struct retro_core_option_definition *opt)
{
   unsigned i, j;

   for (i = 0; i < num_core_options; i++)
   {
      struct retro_core_option_value vars_empty = { NULL, NULL };
      const char *romname_short = vsgame->romname_short;
      const char *option_name = vsgame->dipswitch_core_options[i].option_name;
      unsigned num_values = 0, len = 0;
      char key[100] = {0};

      /* Set core key and sanitize string */
      len = snprintf(key, sizeof(key), "%s%s%c%s", "fceumm_dipswitch_", romname_short, '-', option_name);
      core_key[i] = calloc(len + 1, sizeof(char));
      strcpy(core_key[i], key);
      opt[i].key = str_to_corekey(core_key[i]);

      /* Set desc */
      opt[i].desc = option_name;

      /* Set info */
      opt[i].info = NULL;

      j = 0;
      /* Set core values */
      while (vsgame->dipswitch_core_options[i].settings[j].name) {
         const char *var_value = vsgame->dipswitch_core_options[i].settings[j].name;
         opt[i].values[j].value = var_value;
         j++;
      }
      opt[i].values[j] = vars_empty;

      /* Set default value. Top entry used as default */
      opt[i].default_value = vsgame->dipswitch_core_options[i].settings[0].name;
   }
}

static VSUNIGAME *get_vsuni_dipswitch(unsigned id)
{
   switch (id) {
   case VS_BATTLECITY:
      return (&dipswitch_battlecity);
   case VS_CASTLEVANIA:
      return (&dipswitch_castlevania);
   case VS_CLUCLULAND:
      return (&dipswitch_clucluland);
   case VS_DRMARIO:
      return (&dipswitch_drmario);
   case VS_DUCKHUNT:
      return (&dipswitch_duckhunt);
   case VS_EXITEBIKE:
      return (&dipswitch_excitebike);
   case VS_FREEDOMFORCE:
      return (&dipswitch_freedomforce);
   case VS_GOONIES:
      return (&dipswitch_goonies);
   case VS_GRADIUS:
      return (&dipswitch_gradius);
   case VS_GUMSHOE:
      return (&dipswitch_gumshoe);
   case VS_HOGANSALLEY:
      return (&dipswitch_hogansalley);
   case VS_ICECLIMBER:
      return (&dipswitch_iceclimber);
   case VS_JAJAMARU:
      return (&dipswitch_jajamaru);
   /* case VS_LADIESGOLF: */
   /* case VS_STROKEANDMATCHGOLF: */
   case VS_MACHRIDER:
      return (&dipswitch_machrider);
   case VS_MIGHTYBOMBJACK:
      return (&dipswitch_mightybj);
   case VS_PINBALL:
      return (&dipswitch_pinball);
   case VS_PLATOON:
      return (&dipswitch_platoon);
   case VS_RBIBASEBALL:
      return (&dipswitch_rbibaseball);
   case VS_SLALOM:
      return (&dipswitch_slalom);
   case VS_SMB:
      return (&dipswitch_smb);
   case VS_SOCCER:
      return (&dipswitch_soccer);
   case VS_STARLUSTER:
      return (&dipswitch_starluster);
   case VS_SUPERSKYKID:
      return (&dipswitch_superskykid);
   case VS_SUPERXEVIOUS:
      return (&dipswitch_superxevious);
   case VS_TETRIS:
      return (&dipswitch_tetris);
   case VS_TKOBOXING:
      return (&dipswitch_tkoboxing);
   case VS_TOPGUN:
      return (&dipswitch_topgun);
   }

   FCEU_printf("No dipsettings found. %d\n", id);
   return (NULL);
}

static void update_dipswitch_vsuni(void)
{
   unsigned index_key;
   uint8 vsdip_new = FCEUI_VSUniGetDIPs();

   for (index_key = 0; index_key < num_core_options; index_key++)
   {
      struct retro_variable var = { NULL, NULL };
      const char *key = vscoreopt[index_key].key;
      unsigned index_value;

      var.key = key;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == 0)
         continue;

      index_value = 0;
      while (vscoreopt[index_key].values[index_value].value)
      {
         const char *var_value = vscoreopt[index_key].values[index_value].value;
         uint8 value, mask;

         if (strcmp(var.value, var_value) != 0) {
            index_value++;
            continue;
         }
         value = vsgame->dipswitch_core_options[index_key].settings[index_value].value;
         mask = vsgame->dipswitch_core_options[index_key].mask;
         vsdip_new &= (~mask);
         vsdip_new |= value;
         index_value++;
      }
   }

   if (FCEUI_VSUniGetDIPs() != vsdip_new)
   {
      vsdip = vsdip_new;
#ifdef DEBUG
      FCEU_printf("Dipswitch changed %d%d%d%d%d%d%d%d\n",
         (vsdip >> 0) & 1, (vsdip >> 1) & 1, (vsdip >> 2) & 1, (vsdip >> 3) & 1,
         (vsdip >> 4) & 1, (vsdip >> 5) & 1, (vsdip >> 6) & 1, (vsdip >> 7) & 1);
#endif
   }
}

/* Nintendo World Championship */
static struct retro_core_option_definition dipswitch_nwc[] = {
   {
      "fceumm_dipswitch_nwc_swa",
      "Dipswitch SW1 (+18.7secs)",
      "Adds 18.7 seconds to total time. Total time = 5 mins + enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
   {
      "fceumm_dipswitch_nwc_swb",
      "Dipswitch SW1 (+37.5s)",
      "Adds 37.5 seconds to total time. Total time = 5 mins + enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
   {
      "fceumm_dipswitch_nwc_swc",
      "Dipswitch SW1 (+1m 15s)",
      "Adds 1 minute and 15 seconds to total time. Total time = 5 mins + enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_dipswitch_nwc_swd",
      "Dipswitch SW1 (+2m 30s)",
      "Adds 2 minutes and 30 seconds to total time. Total time = 5 mins + time of enabled dipswitches.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },

   { NULL, NULL, NULL, { NULL, NULL }, NULL },
};

static void update_dipswitch_nwc(void)
{
   unsigned i, dips = 0x00;

   for (i = 0; i < 4; i++)
   {
      struct retro_variable var = { NULL, NULL };
      const char *key = dipswitch_nwc[i].key;

      var.key = key;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == 0)
         continue;
      if (strcmp(var.value, "enabled") != 0)
         continue;
      dips |= (1 << i);
   }

   if (GameInfo->cspecial != dips)
   {
      GameInfo->cspecial = dips;
#ifdef DEBUG
      FCEU_printf("Dipswitch changed = %d%d%d%d\n", (dips >> 0) & 1,
         (dips >> 1) & 1, (dips >> 2) & 1, (dips >> 3) & 1);
#endif
   }
}

size_t set_dipswitch_variables(unsigned current_index, struct retro_core_option_definition *vars)
{
   unsigned index = current_index;
   int dipsw_size = 0;

   /* VSUNI Dipswitch */
   if (GameInfo->type == GIT_VSUNI)
   {
      vsgame = get_vsuni_dipswitch(GameInfo->gameid);
      if (vsgame)
      {
         unsigned i, j;

         num_core_options = 0;
         dipswitch_type = DPSW_VSUNI;

         /* Initialize dipswitch struct and count number of core options */
         for (i = 0; i < DPSW_MAX; i++) {
            vscoreopt[i] = option_defs_empty;
            if (vsgame->dipswitch_core_options[i].option_name)
               num_core_options++;
         }

         make_core_options(vscoreopt);

         /* Copy dpsw struct to main core option struct */
         while (vscoreopt[dipsw_size].key) {
            vars[index] = vscoreopt[dipsw_size];
            index++;
            dipsw_size++;
         }
      }
      return (dipsw_size);
   }

   /* Nintendo World Championship cart (Mapper 105)*/
   if (iNESCart.mapper == 105)
   {
      dipswitch_type = DPSW_NWC;

      while (dipswitch_nwc[dipsw_size].key) {
         vars[index] = dipswitch_nwc[dipsw_size];
         index++;
         dipsw_size++;
      }
      return (dipsw_size);
   }

  return (0);
}

void update_dipswitch(void)
{
   switch (dipswitch_type) {
   case DPSW_VSUNI:
      update_dipswitch_vsuni();
      break;
   case DPSW_NWC:
      update_dipswitch_nwc();
      break;
   }
}

void DPSW_Cleanup(void)
{
   unsigned i;

   for (i = 0; i < DPSW_MAX; i++) {
      if (core_key[i]) {
         free(core_key[i]);
         core_key[i] = NULL;
      }
   }
}
