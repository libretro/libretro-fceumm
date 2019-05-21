#include <stdint.h>
#include <string.h>

#include "../../fceu.h"
#include "../../fceu-types.h"
#include "../../vsuni.h"
#include "../../git.h"
#include "../../driver.h"
#include "../../cart.h"

#include "dipswitch.h"

extern CartInfo iNESCart;

typedef struct {
   char        *name;
   uint8       value;
} SETTING;

typedef struct {
   const char  *option_name;
   unsigned    settings_size;
   uint8       mask;
   SETTING     settings[8];
} DIPSWITCH;

typedef struct {
   unsigned    gameid;
   char        *romname_short;
   unsigned    dipswitch_size;
   DIPSWITCH   dipswitch_core_options[5];
} VSUNIGAME;

static VSUNIGAME dipswitch_battlecity = {
   VS_BATTLECITY, "battlecity", 3,
   {
      {  "Credits for 2 Players", 2, 0x01,
         {
            { "2", 0x01 },
            { "1", 0x00 }
         }
      },
      {  "Lives", 2, 0x02,
         {
            { "5", 0x02 },
            { "3", 0x00 }
         }
      },
      {  "Demo Sounds", 2, 0x04,
         {
            { "disabled", 0x04 },
            { "enabled",  0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_castlevania = {
   VS_CASTLEVANIA, "castlevania", 4,
   {
      {  "Coinage",
         8,
         0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Lives", 2, 0x08,
         {
            { "3", 0x00 },
            { "2", 0x08 }

         }
      },
      {  "Bonus", 4, 0x30,
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 }
         }
      },
      {  "Difficulty", 2, 0x40,
         {
            { "Normal", 0x00 },
            { "Hard",   0x40 }
         }
      }
   }
};

static VSUNIGAME dipswitch_clucluland = {
   VS_CLUCLULAND, "clucluland", 2,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Lives", 4, 0x60,
         {
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 },
            { "2", 0x60 }
         }
      }
   }
};

static VSUNIGAME dipswitch_drmario = {
   VS_DRMARIO, "drmario", 5,
   {
      {  "Drop Rate Increase After", 4, 0x03,
         {
            { "7 Pills",  0x00 },
            { "8 Pills",  0x01 },
            { "9 Pills",  0x02 },
            { "10 Pills", 0x03 }
         },
      },
      {  "Virus Level", 4, 0x0c,
         {
            { "1", 0x00 },
            { "3", 0x04 },
            { "5", 0x08 },
            { "7", 0x0c }
         },
      },
      {  "Drop Speed Up", 4, 0x30,
         {
            { "Slow",    0x00 },
            { "Medium",  0x10 },
            { "Fast",    0x20 },
            { "Fastest", 0x30 }
         },
      },
      {  "Free Play", 2, 0x40,
         {
            { "disabled", 0x00 },
            { "enabled",  0x40 }
         },
      },
      {  "Demo Sounds", 2, 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_duckhunt = {
   VS_DUCKHUNT, "duckhunt", 4,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Difficulty", 4, 0x18,
         {
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",   0x18 },
            { "Easy",   0x00 }
         }
      },
      {  "Miss per game", 2, 0x20,
         {
            { "5", 0x20 },
            { "3", 0x00 }
         }
      },
      {  "Bonus Life", 4, 0xc0,
         {
            { "30000",  0x00 },
            { "50000",  0x40 },
            { "80000",  0x80 },
            { "100000", 0xc0 }
         }
      }
   }
};

static VSUNIGAME dipswitch_excitebike = {
   VS_EXITEBIKE, "excitebike", 4,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Bonus Bike", 4, 0x18,
         {
            { "100k and Every 50k", 0x00 },
            { "100k Only",          0x08 },
            { "Every 100k",         0x10 },
            { "None",               0x18 }
         }
      },
      {  "1st Qualifying Time", 2, 0x20,
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 }
         }
      },
      {  "2nd Qualifying Time", 2, 0x40,
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 }
         }
      }
   }
};

static VSUNIGAME dipswitch_freedomforce = {
   VS_FREEDOMFORCE, "freedomforce", 4,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Health Awarded At", 4, 0x18,
         {
            { "10k 50k",           0x10 },
            { "20k 60k",           0x00 },
            { "20k 60k Every 60k", 0x08 },
            { "10k 50k Every 50k", 0x18 }
         }
      },
      {  "Difficulty (Damage)", 4, 0x60,
         {
            { "Normal",  0x00 },
            { "Easy",    0x40 },
            { "Hard",    0x20 },
            { "Hardest", 0x60 }
         }
      },
      {  "Difficulty (Enemy)", 2, 0x80,
         {
            { "Normal", 0x80 },
            { "Hard",   0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_goonies = {
   VS_GOONIES, "goonies", 5,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Lives", 2, 0x08,
         {
            { "3", 0x08 },
            { "4", 0x00 }
         }
      },
      {  "Bonus", 4, 0x30,
         {
            { "40000", 0x00 },
            { "50000", 0x20 },
            { "60000", 0x10 },
            { "70000", 0x30 }
         }
      },
      {  "Timer", 2, 0x40,
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 }
         }
      },
      { "Demo Sounds", 2, 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_gradius = {
   VS_GRADIUS, "gradius", 5,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Lives", 2, 0x08,
         {
            { "3", 0x08 },
            { "4", 0x00 }
         }
      },
      {  "Bonus", 4, 0x30,
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 }
         }
      },
      {  "Difficulty", 2, 0x40,
         {
            { "Normal", 0x00 },
            { "Hard",   0x40 }
         }
      },
      { "Demo Sounds", 2, 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_gumshoe = {
   VS_GUMSHOE, "gumshoe", 5,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Difficulty", 4, 0x18,
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 }
         }
      },
      {  "Lives", 2, 0x20,
         {
            { "3", 0x20 },
            { "5", 0x00 }
         }
      },
      {  "Bullets per Balloon", 2, 0x40,
         {
            { "3", 0x00 },
            { "2", 0x40 }
         }
      },
      {  "Bonus Man Awarded", 2, 0x80,
         {
            { "At 50000", 0x00 },
            { "None",     0x80 }
         }
      }
   }
};

static VSUNIGAME dipswitch_hogansalley = {
   VS_HOGANSALLEY, "hogansalley", 4,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Difficulty", 4, 0x18,
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 }
         }
      },
      {  "Misses Per Game", 2, 0x20,
         {
            { "5", 0x20 },
            { "3", 0x00 }
         }
      },
      {  "Bonus Life", 4, 0xc0,
         {
            { "30000",  0x00 },
            { "50000",  0x40 },
            { "80000",  0x80 },
            { "100000", 0xc0 }
         }
      }
   }
};

static VSUNIGAME dipswitch_iceclimber = {
   VS_ICECLIMBER, "iceclimber", 4,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Lives", 4, 0x18,
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 },
            { "7", 0x18 }
         }
      },
      {  "Difficulty", 2, 0x20,
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 }
         }
      },
      {  "Time before bear appears", 2, 0x40,
         {
            { "Long",  0x00 },
            { "Short", 0x40 }
         }
      }
   }
};

static VSUNIGAME dipswitch_jajamaru = {
   VS_JAJAMARU, "jajamaru", 3,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Lives", 3, 0x18,
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 }
         }
      },
      {  "Demo Sounds", 2, 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_machrider = {
   VS_MACHRIDER, "machrider", 3,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Time", 4, 0x18,
         {
            { "280 (Easy)", 0x00 },
            { "250",        0x10 },
            { "220",        0x08 },
            { "200 (Hard)", 0x18 }
         }
      },
      {  "Enemies", 2, 0x40,
         {
            { "Less", 0x00 },
            { "More", 0x40 }
         }
      }
   }
};

static VSUNIGAME dipswitch_mightybj = {
   VS_MIGHTYBOMBJACK, "mightybj", 2,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "5 Coins 1 Credit", 0x07 }
         }
      },
      {  "Lives", 4, 0x18,
         {
            { "3", 0x00 },
            { "4", 0x08 },
            { "5", 0x18 },
            { "2", 0x10 },
         }
      }
   }
};

static VSUNIGAME dipswitch_pinball = {
   VS_PINBALL, "pinball", 5,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x01 },
            { "1 Coin 2 Credits", 0x06 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x04 },
            { "2 Coins 1 Credit", 0x05 },
            { "3 Coins 1 Credit", 0x03 },
            { "4 Coins 1 Credit", 0x07 },
            { "Free Play",        0x00 }
         }
      },
      {  "Side Drain Walls", 2, 0x08,
         {
            { "High", 0x00 },
            { "Low",  0x08 }
         }
      },
      {  "Bonus Life", 2, 0x10,
         {
            { "50000", 0x00 },
            { "70000", 0x10 }
         }
      },
      {  "Balls", 4, 0x60,
         {
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 },
            { "2", 0x60 },
         }
      },
      {  "Ball Speed", 2, 0x80,
         {
            { "Slow", 0x00 },
            { "Fast", 0x80 }
         }
      }
   }
};

static VSUNIGAME dipswitch_platoon = {
   VS_PLATOON, "platoon", 3,
   {
      {  "Demo Sounds", 2, 0x04,
         {
            { "enabled",  0x00 },
            { "disabled", 0x04 }
         }
      },
      {  "Difficulty", 4, 0x18,
         {
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",   0x18 },
            { "Easy",   0x00 },
         }
      },
      {  "Coinage", 8, 0xe0,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x20 },
            { "1 Coin 3 Credits", 0x40 },
            { "2 Coins 1 Credit", 0x60 },
            { "3 Coins 1 Credit", 0x80 },
            { "4 Coins 1 Credit", 0xa0 },
            { "5 Coins 1 Credit", 0xc0 },
            { "Free Play",  0xe0 }
         }
      }
   }
};

static VSUNIGAME dipswitch_rbibaseball = {
   VS_RBIBASEBALL, "atari_rbibb", 3,
   {
      {  "Coinage", 4, 0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 }
         }
      },
      {  "1p-inn 2p-inn time-min", 4, 0x0c,
         {
            { "2 2 4", 0x0c },
            { "3 2 6", 0x00 },
            { "4 3 7", 0x08 },
            { "2 1 3", 0x04 },
         }
      },
      {  "Demo Sounds", 2, 0x10,
         {
            { "enabled",  0x00 },
            { "disabled", 0x10 }
         }
      }
   }
};

static VSUNIGAME dipswitch_slalom = {
   VS_SLALOM, "slalom", 5,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x06 },
            { "3 Coins 1 Credit", 0x01 },
            { "4 Coins 1 Credit", 0x05 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Freestyle Points", 2, 0x08,
         {
            { "Hold Time",  0x08 },
            { "Left Right", 0x00 }
         }
      },
      {  "Difficulty", 4, 0x30,
         {
            { "Normal", 0x10 },
            { "Medium", 0x20 },
            { "Hard",   0x30 },
            { "Easy",   0x00 }
         }
      },
      {  "Allow Continue", 2, 0x40,
         {
            { "enabled",  0x00 },
            { "disabled", 0x40 }
         }
      },
      {  "Inverted Input", 2, 0x80,
         {
            { "disabled", 0x00 },
            { "enabled",  0x80 }
         }
      }
   }
};

static VSUNIGAME dipswitch_smb = {
   VS_SMB, "smb", 5,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x06 },
            { "1 Coin 3 Credits", 0x01 },
            { "1 Coin 4 Credits", 0x05 },
            { "1 Coin 5 Credits", 0x03 },
            { "2 Coins 1 Credit", 0x04 },
            { "3 Coins 1 Credit", 0x02 },
            { "Free Play",        0x07 }
         }
      },
      {  "Lives", 2, 0x08,
         {
            { "3", 0x00 },
            { "2", 0x08 }
         }
      },
      {  "Bonus Life", 4, 0x30,
         {
            { "100 coins", 0x00 },
            { "150 coins", 0x20 },
            { "200 coins", 0x10 },
            { "250 coins", 0x30 }
         }
      },
      {  "Timer", 2, 0x40,
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 }
         }
      },
      {  "Continue Lives", 2, 0x80,
         {
            { "3", 0x80 },
            { "4", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_soccer = {
   VS_SOCCER, "soccer", 3,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "1 Coin 4 Credits", 0x06 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x03 },
            { "Free Play",        0x07 }
         }
      },
      {  "Points Timer", 4, 0x18,
         {
            { "1000 Pts", 0x08 },
            { "1200 Pts", 0x18 },
            { "600 Pts",  0x00 },
            { "800 Pts",  0x10 }
         }
      },
      { "Difficulty", 4, 0x60,
         {
            { "Normal", 0x40 },
            { "Medium", 0x20 },
            { "Hard",   0x60 },
            { "Easy",   0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_starluster = {
   VS_STARLUSTER, "starluster", 1,
   {
      { "Coinage", 4, 0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x03 }
         }
      }
   }
};

static VSUNIGAME dipswitch_superskykid = {
   VS_SUPERSKYKID, "superskykid", 3,
   {
      {  "Difficulty", 4, 0x03,
         {

            { "Normal", 0x01 },
            { "Medium", 0x02 },
            { "Hard",   0x03 },
            { "Easy",   0x00 },
         }
      },
      {  "Lives", 2, 0x04,
         {
            { "3", 0x04 },
            { "2", 0x00 }
         }
      },
      {  "Coinage", 4, 0x18,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x08 },
            { "2 Coins 1 Credit", 0x10 },
            { "3 Coins 1 Credit", 0x18 }
         }
      }
   }
};

static VSUNIGAME dipswitch_superxevious = {
   VS_SUPERXEVIOUS, "superxevious", 1,
   {
      {  "Coinage", 4, 0x30,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x10 },
            { "2 Coins 1 Credit", 0x20 },
            { "3 Coins 1 Credit", 0x30 }
         }
      }
   }
};

static VSUNIGAME dipswitch_tetris = {
   VS_TETRIS, "tetris", 1,
   {
      {  "Difficulty", 4, 0x0c,
         {
            { "Normal", 0x04 },
            { "Medium", 0x08 },
            { "Hard",   0x0c },
            { "Easy",   0x00 },
         }
      }
   }
};

static VSUNIGAME dipswitch_tkoboxing = {
   VS_TKOBOXING, "tkoboxing", 2,
   {
      {  "Coinage", 4, 0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 }
         }
      },
      {  "Difficulty", 4, 0x0c,
         {
            { "Normal",    0x04 },
            { "Hard",      0x08 },
            { "Very Hard", 0x0c },
            { "Easy",      0x00 },
         }
      }
   }
};

static VSUNIGAME dipswitch_topgun = {
   VS_TOPGUN, "topgun", 5,
   {
      {  "Coinage", 8, 0x07,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x04 },
            { "1 Coin 3 Credits", 0x02 },
            { "2 Coins 1 Credit", 0x01 },
            { "3 Coins 1 Credit", 0x05 },
            { "4 Coins 1 Credit", 0x06 },
            { "5 Coins 1 Credit", 0x03 },
            { "Free Play",  0x07 }
         }
      },
      {  "Lives per Coin", 2, 0x08,
         {
            { "3-12 Max", 0x00 },
            { "2-9 Max",  0x08 }
         }
      },
      {  "Bonus Life", 4, 0x30,
         {
            { "30k and Every 50k",   0x00 },
            { "50k and Every 100k",  0x20 },
            { "100k and Every 150k", 0x10 },
            { "200k and Every 200k", 0x30 }
         }
      },
      {  "Difficulty", 2, 0x40,
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 }
         }
      },
      {  "Demo Sounds", 2, 0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

typedef struct {
   char key[64];
   char val[150];
} VSUNIGAMEOPT;

static VSUNIGAME *vsgame = NULL;
static VSUNIGAMEOPT vscoreopt[5];

static void str_to_corekey_label(char *str)
{
   unsigned i = 0;

   while (str[i] != '\0')
   {
      if (str[i] == ' ')
         str[i] = '_';
      i++;
   }
   str[i] = '\0';
}

static void make_core_options(void)
{
   unsigned i, dipswitch_size;
   char core_prefix[32];

   dipswitch_size = vsgame->dipswitch_size;
   sprintf(core_prefix, "fceumm_dipswitch_%s", vsgame->romname_short);   
   for (i = 0; i < dipswitch_size; i++)
   {
      unsigned x;
      char core_key[64], core_values[150];
      DIPSWITCH *coreoptions = &vsgame->dipswitch_core_options[i];

      /* make var key string from list */
      sprintf(core_key, "%s%c%s", core_prefix, '-', coreoptions->option_name);
      str_to_corekey_label(core_key);

      /* make var values string from lists of values */
      sprintf(core_values, "%s; ", coreoptions->option_name);
      for (x = 0; x < coreoptions->settings_size; x++)
      {
         SETTING *corevalues = &coreoptions->settings[x];

         strcat(core_values, corevalues->name);
         if ((coreoptions->settings_size - 1) > x)
            strcat(core_values, "|");
      }

      sprintf(vscoreopt[i].key, "%s", core_key);
      sprintf(vscoreopt[i].val, "%s", core_values);
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
   struct retro_variable var = {0};
   unsigned idx_dips, idx_var;
   uint8 vsdip_new = 0x00;

   if (vsgame == NULL)
      return;

   vsdip_new = FCEUI_VSUniGetDIPs();
   for (idx_dips = 0; idx_dips < vsgame->dipswitch_size; idx_dips++)
   {
      DIPSWITCH *core_option = &vsgame->dipswitch_core_options[idx_dips];
      var.key = vscoreopt[idx_dips].key;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == 0)
         continue;
      for (idx_var = 0; idx_var < core_option->settings_size; idx_var++)
      {
         SETTING *vs_settings = &core_option->settings[idx_var];
         if (strcmp(var.value, vs_settings->name) != 0)
            continue;
         vsdip_new &= ~core_option->mask;
         vsdip_new |= vs_settings->value;
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
static struct retro_variable dipswitch_nwc[] = {
   { "fceumm_dipswitch_nwc_swa", "Dipswitch SW1 (18.7s); disabled|enabled" },
   { "fceumm_dipswitch_nwc_swb", "Dipswitch SW2 (37.5s); disabled|enabled" },
   { "fceumm_dipswitch_nwc_swc", "Dipswitch SW3 (1m 15s); enabled|disabled" },
   { "fceumm_dipswitch_nwc_swd", "Dipswitch SW4 (2m 30s); disabled|enabled" },
   { NULL, NULL }
};

static void update_dipswitch_nwc(void)
{
   struct retro_variable var = {0};
   unsigned dips = 0x00;
   unsigned index;

   for (index = 0; index < 4; index++)
   {
      struct retro_variable *nwc = &dipswitch_nwc[index];
      var.key = nwc->key;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         if (strcmp(var.value, "enabled") == 0)
            dips |= (1 << index);
      }
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

void set_dipswitch_variables(unsigned *index, struct retro_variable *vars)
{
   unsigned i, idx = *index;

   /* VSUNI Dipswitch */
   if (GameInfo->type == GIT_VSUNI)
   {
      vsgame = get_vsuni_dipswitch(GameInfo->gameid);
      if (vsgame)
      {
         make_core_options();
         for (i = 0; i < vsgame->dipswitch_size; i++, idx++) {
            vars[idx].key = vscoreopt[i].key;
            vars[idx].value = vscoreopt[i].val;
         }
      }
   }

   /* Nintendo World Championship cart (Mapper 105)*/
   if (iNESCart.mapper == 105)
   {
      i = 0;
      while (dipswitch_nwc[i].key) {
         vars[idx] = dipswitch_nwc[i];
         idx++;
         i++;
      }
   }

   *index = idx;
}

void update_dipswitch(void)
{
   if (iNESCart.mapper == 105)
      update_dipswitch_nwc();

   if (GameInfo->type == GIT_VSUNI)
      update_dipswitch_vsuni();
}
