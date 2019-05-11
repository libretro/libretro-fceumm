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

/* VSUNI */

typedef struct {
   char        name[20];
   uint8       value;
} SETTING;

typedef struct {
   char        var_name[50];
   char        var_value[150];
   unsigned    settings_size;
   uint8       mask;
   SETTING     settings[8];
} DIPSWITCH;

typedef struct {
   unsigned    gameid;
   char        romname[100];
   unsigned    dipswitch_size;
   DIPSWITCH   dipswitch_core_options[8];
} VSUNIGAME;

static VSUNIGAME dipswitch_battlecity = {
   VS_BATTLECITY,
   "VS. Battle City",
   3,
   {
      {  "fceumm_dipswitch_battlecity_credits",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         2,
         0x01,
         {
            { "2", 0x01 },
            { "1", 0x00 }
         }
      },
      {  "fceumm_dipswitch_battlecity_lives",
         "Lives; 3|5",
         2,
         0x02,
         {
            { "5", 0x02 },
            { "3", 0x00 }
         }
      },
      {  "fceumm_dipswitch_battlecity_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x04,
         {
            { "disabled", 0x04 },
            { "enabled",  0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_castlevania = {
   VS_CASTLEVANIA,
   "VS. Castlevania",
   4,
   {
      {  "fceumm_dipswitch_castlevania_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_castlevania_lives",
         "Lives; 3|2",
         2,
         0x08,
         {
            { "2", 0x08 },
            { "3", 0x00 }
         }
      },
      {  "fceumm_dipswitch_castlevania_bonus",
         "Bonus; 100k|200k|300k|400k",
         4,
         0x30,
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 }
         }
      },
      {  "fceumm_dipswitch_castlevania_difficulty",
         "Difficulty; Normal|Hard",
         2,
         0x40,
         {
            { "Hard",   0x40 },
            { "Normal", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_clucluland = {
   VS_CLUCLULAND,
   "VS. Clu Clu Land",
   2,
   {
      {  "fceumm_dipswitch_clucluland_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_clucluland_lives",
         "Lives; 3|4|5|2",
         4,
         0x60,
         {
            { "2", 0x60 },
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 }
         }
      }
   }
};

static VSUNIGAME dipswitch_drmario = {
   VS_DRMARIO,
   "VS. Dr. Mario",
   5,
   {
      {  "fceumm_dipswitch_drmario_droprateinc",
         "Drop Rate Increase After; 7 Pills|8 Pills|9 Pills|10 Pills",
         4,
         0x03,
         {
            { "7 Pills",  0x00 },
            { "8 Pills",  0x01 },
            { "9 Pills",  0x02 },
            { "10 Pills", 0x03 }
         },
      },
      {  "fceumm_dipswitch_drmario_viruslevel",
         "Virus Level; 1|3|5|7",
         4,
         0x0c,
         {
            { "1", 0x00 },
            { "3", 0x04 },
            { "5", 0x08 },
            { "7", 0x0c }
         },
      },
      {  "fceumm_dipswitch_drmario_dropspeedup",
         "Drop Speed Up; Slow|Medium|Fast|Fastest",
         4,
         0x30,
         {
            { "Slow",    0x00 },
            { "Medium",  0x10 },
            { "Fast",    0x20 },
            { "Fastest", 0x30 }
         },
      },
      {  "fceumm_dipswitch_drmario_freeplay",
         "Free Play; disabled|enabled",
         2,
         0x40,
         {
            { "disabled", 0x00 },
            { "enabled",  0x40 }
         },
      },
      {  "fceumm_dipswitch_drmario_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x80,
         {
            { "disabled", 0x00 },
            { "enabled",  0x80 }
         }
      }
   }
};

static VSUNIGAME dipswitch_duckhunt = {
   VS_DUCKHUNT,
   "VS. Duck Hunt",
   4,
   {
      {  "fceumm_dipswitch_duckhunt_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|5 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_duckhunt_difficulty",
         "Difficulty; Normal|Easy|Hard|Medium",
         4,
         0x18,
         {
            { "Easy",   0x00 },
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",   0x18 }
         }
      },
      {  "fceumm_dipswitch_duckhunt_misspergame",
         "Miss per game; 5|3",
         2,
         0x20,
         {
            { "3", 0x00 },
            { "5", 0x20 }
         }
      },
      {  "fceumm_dipswitch_duckhunt_bonus",
         "Bonus Life; 30000|50000|80000|100000",
         4,
         0xc0,
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
   VS_EXITEBIKE,
   "VS. Excite Bike",
   4,
   {
      {  "fceumm_dipswitch_excitebike_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_excitebike_bonusbike",
         "Bonus Bike; 100k and Every 50k|100k Only|Every 100k|None",
         4,
         0x18,
         {
            { "100k and Every 50k", 0x00 },
            { "100k Only",          0x08 },
            { "Every 100k",         0x10 },
            { "None",               0x18 }
         }
      },
      {  "fceumm_dipswitch_excitebike_1stqtime",
         "1st Qualifying Time; Easy|Hard",
         2,
         0x20,
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 }
         }
      },
      {  "fceumm_dipswitch_excitebike_2ndqtime",
         "2st Qualifying Time; Easy|Hard",
         2,
         0x40,
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 }
         }
      }
   }
};

static VSUNIGAME dipswitch_freedomforce = {
   VS_FREEDOMFORCE,
   "VS. Freedom Force",
   4,
   {
      {  "fceumm_dipswitch_freedomforce_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|2 Coins 1 Credit|3 Coins 1 Credit|4 Coins 1 Credit|5 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_freedomforce_healthawd",
         "Health Awarded At; 10k 50k|10k 50k Every 50k|50k 60k|20k 60k Every 60k",
         4,
         0x18,
         {
            { "10k 50k",           0x10 },
            { "20k 60k",           0x00 },
            { "20k 60k Every 60k", 0x08 },
            { "10k 50k Every 50k", 0x18 }
         }
      },
      {  "fceumm_dipswitch_freedomforce_damage",
         "Difficulty (Damage); Normal|Easy|Hard|Hardest",
         4,
         0x60,
         {
            { "Normal",  0x00 },
            { "Easy",    0x40 },
            { "Hard",    0x20 },
            { "Hardest", 0x60 }
         }
      },
      {  "fceumm_dipswitch_freedomforce_enemy",
         "Difficulty (Enemy); Normal|Hard",
         2,
         0x80,
         {
            { "Normal", 0x80 },
            { "Hard",   0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_goonies = {
   VS_GOONIES,
   "VS. Goonies",
   5,
   {
      {  "fceumm_dipswitch_goonies_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_goonies_lives",
         "Lives; 4|3",
         2,
         0x08,
         {
            { "3", 0x08 },
            { "4", 0x00 }
         }
      },
      {  "fceumm_dipswitch_goonies_bonus",
         "Bonus; 40000|50000|60000|70000",
         4,
         0x30,
         {
            { "40000", 0x00 },
            { "50000", 0x20 },
            { "60000", 0x10 },
            { "70000", 0x30 }
         }
      },
      {  "fceumm_dipswitch_goonies_timer",
         "Timer; Slow|Fast",
         2,
         0x40,
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 }
         }
      },
      { "fceumm_dipswitch_goonies_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_gradius = {
   VS_GRADIUS,
   "VS. Gradius",
   5,
   {
      {  "fceumm_dipswitch_gradius_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_gradius_lives",
         "Lives; 3|4",
         2,
         0x08,
         {
            { "3", 0x08 },
            { "4", 0x00 }
         }
      },
      {  "fceumm_dipswitch_gradius_bonus",
         "Bonus; 100k|200k|300k|400k",
         4,
         0x30,
         {
            { "100k", 0x00 },
            { "200k", 0x20 },
            { "300k", 0x10 },
            { "400k", 0x30 }
         }
      },
      {  "fceumm_dipswitch_gradius_difficulty",
         "Difficulty; Normal|Hard",
         2,
         0x40,
         {
            { "Hard",   0x40 },
            { "Normal", 0x00 }
         }
      },
      { "fceumm_dipswitch_gradius_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_gumshoe = {
   VS_GUMSHOE,
   "VS. Gumshoe",
   5,
   {
      {  "fceumm_dipswitch_gumshoe_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_gumshoe_difficulty",
         "Difficulty; Normal|Easy|Hard|Hardest",
         4,
         0x18,
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 }
         }
      },
      {  "fceumm_dipswitch_gumshoe_lives",
         "Lives; 3|5",
         2,
         0x20,
         {
            { "3", 0x20 },
            { "5", 0x00 }
         }
      },
      {  "fceumm_dipswitch_gumshoe_bullets",
         "Bullets per Balloon; 3|2",
         2,
         0x40,
         {
            { "3", 0x00 },
            { "2", 0x40 }
         }
      },
      {  "fceumm_dipswitch_gumshoe_bonus",
         "Bonus Man Awarded; At 50000|None",
         2,
         0x80,
         {
            { "At 50000", 0x00 },
            { "None",     0x80 }
         }
      }
   }
};

static VSUNIGAME dipswitch_hogansalley = {
   VS_HOGANSALLEY,
   "VS. Hogan Alley",
   4,
   {
      {  "fceumm_dipswitch_hogansalley_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_hogansalley_difficulty",
         "Difficulty; Normal|Easy|Hard|Hardest",
         4,
         0x18,
         {
            { "Normal",  0x08 },
            { "Easy",    0x00 },
            { "Hard",    0x10 },
            { "Hardest", 0x18 }
         }
      },
      {  "fceumm_dipswitch_hogansalley_misspergame",
         "Misses Per Game; 5|3",
         2,
         0x20,
         {
            { "3", 0x00 },
            { "5", 0x20 }
         }
      },
      {  "fceumm_dipswitch_hogansalley_bonuslife",
         "Bonus Life; 30000|50000|80000|100000",
         4,
         0xc0,
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
   VS_ICECLIMBER,
   "VS. Ice Climber",
   4,
   {
      {  "fceumm_dipswitch_iceclimber_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_iceclimber_lives",
         "Lives; 3|4|5|7",
         4,
         0x18,
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 },
            { "7", 0x18 }
         }
      },
      {  "fceumm_dipswitch_iceclimber_difficulty",
         "Difficulty; Easy|Hard",
         2,
         0x20,
         {
            { "Easy", 0x00 },
            { "Hard", 0x20 }
         }
      },
      {  "fceumm_dipswitch_iceclimber_time",
         "Time before bear appears; Long|Short",
         2,
         0x40,
         {
            { "Long",  0x00 },
            { "Short", 0x40 }
         }
      }
   }
};

static VSUNIGAME dipswitch_jajamaru = {
   VS_JAJAMARU,
   "VS. Jajamaru",
   3,
   {
      {  "fceumm_dipswitch_jajamaru_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_jajamaru_lives",
         "Lives; 3|4|5",
         3,
         0x18,
         {
            { "3", 0x00 },
            { "4", 0x10 },
            { "5", 0x08 }
         }
      },
      {  "fceumm_dipswitch_jajamaru_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_mightybj = {
   VS_MIGHTYBOMBJACK,
   "VS. Mighty Bomb Jack",
   2,
   {
      {  "fceumm_dipswitch_mightybj_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit",
         8,
         0x07,
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
      {  "fceumm_dipswitch_mightybj_lives",
         "Lives; 3|4|5|2",
         4,
         0x18,
         {
            { "2", 0x10 },
            { "3", 0x00 },
            { "4", 0x08 },
            { "5", 0x18 }
         }
      }
   }
};

static VSUNIGAME dipswitch_pinball = {
   VS_PINBALL,
   "VS. Pinball",
   5,
   {
      {  "fceumm_dipswitch_pinball_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_pinball_sidedrainwalls",
         "Side Drain Walls; High|Low",
         2,
         0x08,
         {
            { "High", 0x00 },
            { "Low",  0x08 }
         }
      },
      {  "fceumm_dipswitch_pinball_bonus",
         "Bonus Life; 50000|70000",
         2,
         0x10,
         {
            { "50000", 0x00 },
            { "70000", 0x10 }
         }
      },
      {  "fceumm_dipswitch_pinball_balls",
         "Balls; 3|4|5|2",
         4,
         0x60,
         {
            { "2", 0x60 },
            { "3", 0x00 },
            { "4", 0x40 },
            { "5", 0x20 }
         }
      },
      {  "fceumm_dipswitch_pinball_ballspeed",
         "Ball Speed; Slow|Fast",
         2,
         0x80,
         {
            { "Slow", 0x00 },
            { "Fast", 0x80 }
         }
      }
   }
};

static VSUNIGAME dipswitch_platoon = {
   VS_PLATOON,
   "VS. Platoon",
   3,
   {
      {  "fceumm_dipswitch_platoon_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x04,
         {
            { "enabled",  0x00 },
            { "disabled", 0x04 }
         }
      },
      {  "fceumm_dipswitch_platoon_difficulty",
         "Difficulty; Normal|Medium|Hard|Easy",
         4,
         0x18,
         {
            { "Easy",0x00 },
            { "Normal", 0x08 },
            { "Medium", 0x10 },
            { "Hard",0x18 }
         }
      },
      {  "fceumm_dipswitch_platoon_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         8,
         0xe0,
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
   VS_RBIBASEBALL,
   "VS. Atari R.B.I. Baseball",
   3,
   {
      {  "fceumm_dipswitch_rbibaseball_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit",
         4,
         0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 }
         }
      },
      {  "fceumm_dipswitch_rbibaseball_inn",
         "1p-inn 2p-inn time-min; 2 2 4|3 2 6|4 3 7|2 1 3",
         4,
         0x0c,
         {
            { "2 1 3", 0x04 },
            { "2 2 4", 0x0c },
            { "3 2 6", 0x00 },
            { "4 3 7", 0x08 }
         }
      },
      {  "fceumm_dipswitch_rbibaseball_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x10,
         {
            { "enabled",  0x00 },
            { "disabled", 0x10 }
         }
      }
   }
};

static VSUNIGAME dipswitch_slalom = {
   VS_SLALOM,
   "VS. Slalom",
   5,
   {
      {  "fceumm_dipswitch_slalom_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
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
      {  "fceumm_dipswitch_slalom_freestylepts",
         "Freestyle Points; Hold Time|Left Right",
         2,
         0x08,
         {
            { "Hold Time",  0x08 },
            { "Left Right", 0x00 }
         }
      },
      {  "fceumm_dipswitch_slalom_difficulty",
         "Difficulty; Normal|Medium|Hard|Easy",
         4,
         0x30,
         {
            { "Easy",   0x00 },
            { "Normal", 0x10 },
            { "Medium", 0x20 },
            { "Hard",   0x30 }
         }
      },
      {  "fceumm_dipswitch_slalom_allowcontinue",
         "Allow Continue; enabled|disabled",
         2,
         0x40,
         {
            { "enabled",  0x00 },
            { "disabled", 0x40 }
         }
      },
      {  "fceumm_dipswitch_slalom_inverted_input",
         "Inverted Input; disabled|enabled",
         2,
         0x80,
         {
            { "enabled",  0x80 },
            { "disabled", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_smb = {
   VS_SMB,
   "VS. Super Mario Bros.",
   5,
   {
      {  "fceumm_dipswitch_smb_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|1 Coin 5 Credits|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
          8,
          0x07,
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
      {  "fceumm_dipswitch_smb_lives",
         "Lives; 3|2",
         2,
         0x08,
         {
            { "2", 0x08 },
            { "3", 0x00 }
         }
      },
      {  "fceumm_dipswitch_smb_bonuslife",
         "Bonus Life (coins); 100|150|200|250",
         4,
         0x30,
         {
            { "100", 0x00 },
            { "150", 0x20 },
            { "200", 0x10 },
            { "250", 0x30 }
         }
      },
      {  "fceumm_dipswitch_smb_timer",
         "Timer; Slow|Fast",
         2,
         0x40,
         {
            { "Slow", 0x00 },
            { "Fast", 0x40 }
         }
      },
      {  "fceumm_dipswitch_smb_continuelives",
         "Continue Lives; 3|4",
         2,
         0x80,
         {
            { "3", 0x80 },
            { "4", 0x00 }
         }
      }
   }
};

static VSUNIGAME dipswitch_soccer = {
   VS_SOCCER,
   "VS. Soccer",
   3,
   {
      {  "fceumm_dipswitch_soccer_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_soccer_pts_timer",
         "Points Timer; 1000 Pts|1200 Pts|600 Pts|800 Pts",
         4,
         0x18,
         {
            { "600 Pts",  0x00 },
            { "800 Pts",  0x10 },
            { "1000 Pts", 0x08 },
            { "1200 Pts", 0x18 }
         }
      },
      { "fceumm_dipswitch_soccer_difficulty",
         "Difficulty; Normal|Medium|Hard|Easy",
         4,
         0x60,
         {
            { "Easy",0x00 },
            { "Normal", 0x40 },
            { "Medium", 0x20 },
            { "Hard",0x60 }
         }
      }
   }
};

static VSUNIGAME dipswitch_starluster = {
   VS_STARLUSTER,
   "VS. Star Luster",
   1,
   {
      { "fceumm_dipswitch_starluster_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit",
         4,
         0x03,
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
   VS_SUPERSKYKID,
   "VS. Super Sky Kid",
   3,
   {
      {  "fceumm_dipswitch_superskykid_difficulty",
         "Difficulty; Normal|Medium|Hard|Easy",
         4,
         0x03,
         {
            { "Easy",0x00 },
            { "Normal", 0x01 },
            { "Medium", 0x02 },
            { "Hard",0x03 }
         }
      },
      {  "fceumm_dipswitch_superskykid_lives",
         "Lives; 3|2",
         2,
         0x04,
         {
            { "2", 0x00 },
            { "3", 0x04 }
         }
      },
      {  "fceumm_dipswitch_superskykid_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit",
         4,
         0x18,
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
   VS_SUPERXEVIOUS,
   "VS. Super Xevious",
   1,
   {
      {  "fceumm_dipswitch_superxevious_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit",
         4,
         0x30,
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
   VS_TETRIS,
   "VS. Tetris",
   1,
   {
      {  "fceumm_dipswitch_tetris_difficulty",
         "Difficulty; Normal|Medium|Hard|Easy",
         4,
         0x0c,
         {
            { "Easy",0x00 },
            { "Normal", 0x04 },
            { "Medium", 0x08 },
            { "Hard",0x0c }
         }
      }
   }
};

static VSUNIGAME dipswitch_tkoboxing = {
   VS_TKOBOXING,
   "VS. T.K.O. Boxing",
   2,
   {
      {  "fceumm_dipswitch_tkoboxing_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|3 Coins 1 Credit|2 Coins 1 Credit",
         4,
         0x03,
         {
            { "1 Coin 1 Credit",  0x00 },
            { "1 Coin 2 Credits", 0x01 },
            { "2 Coins 1 Credit", 0x02 },
            { "3 Coins 1 Credit", 0x03 }
         }
      },
      {  "fceumm_dipswitch_tkoboxing_difficulty",
         "Difficulty; Easy|Normal|Hard|Very Hard",
         4,
         0x0c,
         {
            { "Easy",0x00 },
            { "Normal", 0x04 },
            { "Hard",0x08 },
            { "Very Hard", 0x0c }
         }
      }
   }
};

static VSUNIGAME dipswitch_topgun = {
   VS_TOPGUN,
   "VS. Top Gun",
   5,
   {
      {  "fceumm_dipswitch_topgun_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|5 Coins 1 Credit|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_topgun_livespercoin",
         "Live per Coin; 3-12 Max|2-9 Max",
         2,
         0x08,
         {
            { "3-12 Max", 0x00 },
            { "2-9 Max",  0x08 }
         }
      },
      {  "fceumm_dipswitch_topgun_bonuslife",
         "Bonus Life; 30k and Every 50k|50k and Every 100k|100k and Every 150k|200k and Every 200k",
         4,
         0x30,
         {
            { "30k and Every 50k",   0x00 },
            { "50k and Every 100k",  0x20 },
            { "100k and Every 150k", 0x10 },
            { "200k and Every 200k", 0x30 }
         }
      },
      {  "fceumm_dipswitch_topgun_difficulty",
         "Difficulty; Easy|Hard",
         2,
         0x40,
         {
            { "Easy", 0x00 },
            { "Hard", 0x40 }
         }
      },
      {  "fceumm_dipswitch_topgun_demosounds",
         "Demo Sounds; enabled|disabled",
         2,
         0x80,
         {
            { "disabled", 0x00 },
            { "enabled",  0x80 }
         }
      }
   }
};

static VSUNIGAME dipswitch_machrider = {
   VS_MACHRIDER,
   "VS. Mach Rider",
   3,
   {
      {  "fceumm_dipswitch_machrider_coinage",
         "Coinage; 1 Coin 1 Credit|1 Coin 2 Credits|1 Coin 3 Credits|1 Coin 4 Credits|4 Coins 1 Credit|3 Coins 1 Credit|2 Coins 1 Credit|Free Play",
         8,
         0x07,
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
      {  "fceumm_dipswitch_machrider_time",
         "Time; 280 (Easy)|250|220|200 (Hard)",
         4,
         0x18,
         {
            { "280 (Easy)", 0x00 },
            { "250",        0x10 },
            { "220",        0x08 },
            { "200 (Hard)", 0x18 }
         }
      },
      {  "fceumm_dipswitch_machrider_enemies",
         "Enemies; Less|More",
         2,
         0x40,
         {
            { "Less", 0x00 },
            { "More", 0x40 }
         }
      }
   }
};

static struct retro_variable var_empty = { NULL, NULL };
static struct retro_variable dipswitch_vsuni[8];
static VSUNIGAME *vsgame = NULL;

static int make_core_options(VSUNIGAME *tmp)
{
   unsigned ret = 0;
   VSUNIGAME *vs = tmp;

   if (vs->gameid != GameInfo->gameid)
      return 0;

   for (ret = 0; ret < vs->dipswitch_size; ret++) {
      DIPSWITCH *coreoptions = &vs->dipswitch_core_options[ret];
      dipswitch_vsuni[ret].key = coreoptions->var_name;
      dipswitch_vsuni[ret].value = coreoptions->var_value;
   }

   dipswitch_vsuni[ret] = var_empty;
   vsgame = vs;

   return (ret);
}

static int get_vsuni_dipswitch(unsigned id)
{
   unsigned ret = 0;

   if (id == 0)
      return 0;

   switch (id) {
   case VS_BATTLECITY:
      ret = make_core_options(&dipswitch_battlecity);
      break;
   case VS_CASTLEVANIA:
      ret = make_core_options(&dipswitch_castlevania);
      break;
   case VS_CLUCLULAND:
      ret = make_core_options(&dipswitch_clucluland);
      break;
   case VS_DRMARIO:
      ret = make_core_options(&dipswitch_drmario);
      break;
   case VS_DUCKHUNT:
      ret = make_core_options(&dipswitch_duckhunt);
      break;
   case VS_EXITEBIKE:
      ret = make_core_options(&dipswitch_excitebike);
      break;
   case VS_FREEDOMFORCE:
      ret = make_core_options(&dipswitch_freedomforce);
      break;
   case VS_GOONIES:
      ret = make_core_options(&dipswitch_goonies);
      break;
   case VS_GRADIUS:
      ret = make_core_options(&dipswitch_gradius);
      break;
   case VS_GUMSHOE:
      ret = make_core_options(&dipswitch_gumshoe);
      break;
   case VS_HOGANSALLEY:
      ret = make_core_options(&dipswitch_hogansalley);
      break;
   case VS_ICECLIMBER:
      ret = make_core_options(&dipswitch_iceclimber);
      break;
   case VS_JAJAMARU:
      ret = make_core_options(&dipswitch_jajamaru);
      break;
   /* case VS_LADIESGOLF: */
   /* case VS_STROKEANDMATCHGOLF: */
   case VS_MACHRIDER:
      ret = make_core_options(&dipswitch_machrider);
      break;
   case VS_MIGHTYBOMBJACK:
      ret = make_core_options(&dipswitch_mightybj);
      break;
   case VS_PINBALL:
      ret = make_core_options(&dipswitch_pinball);
      break;
   case VS_PLATOON:
      ret = make_core_options(&dipswitch_platoon);
      break;
   case VS_RBIBASEBALL:
      ret = make_core_options(&dipswitch_rbibaseball);
      break;
   case VS_SLALOM:
      ret = make_core_options(&dipswitch_slalom);
      break;
   case VS_SMB:
      ret = make_core_options(&dipswitch_smb);
      break;
   case VS_SOCCER:
      ret = make_core_options(&dipswitch_soccer);
      break;
   case VS_STARLUSTER:
      ret = make_core_options(&dipswitch_starluster);
      break;
   case VS_SUPERSKYKID:
      ret = make_core_options(&dipswitch_superskykid);
      break;
   case VS_SUPERXEVIOUS:
      ret = make_core_options(&dipswitch_superxevious);
      break;
   case VS_TETRIS:
      ret = make_core_options(&dipswitch_tetris);
      break;
   case VS_TKOBOXING:
      ret = make_core_options(&dipswitch_tkoboxing);
      break;
   case VS_TOPGUN:
      ret = make_core_options(&dipswitch_topgun);
      break;
   default:
      FCEU_printf("No dipsettings found. %d\n", id);
      break;
   }

   return (ret);
}

void update_dipswitch_vsuni(void)
{
   struct retro_variable var = {0};
   unsigned idx_dips, idx_var;
   uint8 vsdip_new = 0x00;

   if (GameInfo->type != GIT_VSUNI)
      return;

   if (vsgame == NULL)
      return;

   vsdip_new = FCEUI_VSUniGetDIPs();
   for (idx_dips = 0; idx_dips < vsgame->dipswitch_size; idx_dips++)
   {
      DIPSWITCH *core_option = &vsgame->dipswitch_core_options[idx_dips];
      var.key = core_option->var_name;
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

   if (FCEUI_VSUniGetDIPs() != (uint8)vsdip_new)
   {
      vsdip = (uint8)vsdip_new;
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

   if (iNESCart.mapper != 105)
      return;

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

void set_dipswitch_variables(unsigned *index, struct retro_variable vars[])
{
   unsigned i, idx = *index;

   /* VSUNI Dipswitch */
   if (GameInfo->type == GIT_VSUNI)
   {
      unsigned size = 0;
      size = get_vsuni_dipswitch(GameInfo->gameid);
      if (size)
      {
         i = 0;
         while (dipswitch_vsuni[i].key) {
            vars[idx] = dipswitch_vsuni[i];
            idx++;
            i++;
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
