#ifndef LIBRETRO_CORE_OPTIONS_H__
#define LIBRETRO_CORE_OPTIONS_H__

#include <stdlib.h>
#include <string.h>

#include <libretro.h>
#include <retro_inline.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 ********************************
 * Core Option Definitions
 ********************************
*/

/* RETRO_LANGUAGE_ENGLISH */

/* Default language:
 * - All other languages must include the same keys and values
 * - Will be used as a fallback in the event that frontend language
 *   is not available
 * - Will be used as a fallback for any missing entries in
 *   frontend language definition */

#define MAX_CORE_OPTIONS 32

struct retro_core_option_definition option_defs_common[] = {
   {
      "fceumm_region",
      "Region",
      "Force core to use NTSC, PAL or Dendy region timings.",
      {
         { "Auto",  NULL },
         { "NTSC",  NULL },
         { "PAL",   NULL },
         { "Dendy", NULL },
         { NULL, NULL},
      },
      "Auto",
   },
   {
      "fceumm_overclocking",
      "Overclock",
      "Enables or disables overclocking which can reduce slowdowns in some games. Postrender method is more compatible with every game with Vblank more effective for games like Contra Force.",
      {
         { "disabled",      NULL },
         { "2x-Postrender", NULL },
         { "2x-VBlank",     NULL },
         { NULL, NULL},
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      "RAM Power-On Fill (Needs Restart)",
      "Choose RAM startup during power up. Some games rely on initial ram values for random generator as an example.",
      {
         { "fill $ff", "$FF" },
         { "fill $00", "$00" },
         { "random",   "random" },
         { NULL, NULL},
      },
      "fill $ff",
   },
   {
      "fceumm_nospritelimit",
      "No Sprite Limit",
      "Removes the 8-per-scanline hardware limit. This reduces sprite flickering but can cause some games to glitch since some use this for effects.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      "NTSC Filter",
      "Enable blargg NTSC filters.",
      {
         { "disabled",   NULL },
         { "composite",  NULL },
         { "svideo",     NULL },
         { "rgb",        NULL },
         { "monochrome", NULL },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_palette",
      "Color Palette",
      "Choose from pre-generated palettes, a custom 64x3 palette from file or raw format (needs to use a nes-decoder shader).",
      {
         { "default",              NULL },
         { "asqrealc",             NULL },
         { "nintendo-vc",          NULL },
         { "rgb",                  NULL },
         { "yuv-v3",               NULL },
         { "unsaturated-final",    NULL },
         { "sony-cxa2025as-us",    NULL },
         { "pal",                  NULL },
         { "bmf-final2",           NULL },
         { "bmf-final3",           NULL },
         { "smooth-fbx",           NULL },
         { "composite-direct-fbx", NULL },
         { "pvm-style-d93-fbx",    NULL },
         { "ntsc-hardware-fbx",    NULL },
         { "nes-classic-fbx-fs",   NULL },
         { "nescap",               NULL },
         { "wavebeam",             NULL },
         { "raw",                  NULL },
         { "custom",               NULL },
         { NULL, NULL},
      },
      "default",
   },
   {
      "fceumm_aspect",
      "Aspect Ratio",
      "Choose preferred aspect ratio.",
      {
         { "8:7 PAR", NULL },
         { "4:3",     NULL },
         { NULL, NULL},
      },
      "8:7 PAR",
   },
#ifdef PSP
   {
      "fceumm_overscan",
      "Crop Overscan",
      "Removes 8 pixel overscan from all sides",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL},
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      "Crop Horizontal Overscan",
      "Removes 8 pixel from left and right side of the screen to simulate overscan seen on standard CRT television.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
      {
      "fceumm_overscan_v",
      "Crop Vertical Overscan",
      "Removes 8 pixel from the top and bottom of the screen to simulate overscan seen on standard CRT television.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "enabled",
   },
#endif /* overscan options */
   {
      "fceumm_up_down_allowed",
      "Allow Opposing Directions",
      "Allows simultaneous UP+DOWN or LEFT+RIGHT button combinations which can create different effects in some games.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
   {
      "fceumm_turbo_enable",
      "Turbo Enable",
      "Enables or disables turbo buttons.",
      {
         { "None",     NULL },
         { "Player 1", NULL },
         { "Player 2", NULL },
         { "Both",     NULL },
         { NULL, NULL},
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      "Turbo Delay (in frames)",
      "Repeat rate of turbo buttons in frames.",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL},
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      "Zapper Mode",
      "Selects device to use for zapper games.",
      {
         { "lightgun",    NULL },
         { "touchscreen", NULL },
         { "mouse",       NULL },
         { NULL, NULL},
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      "Show Crosshair",
      "Shows or hides on-screen crosshairs when using a zapper.",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      "Zapper Tolerance",
      "Sets how many pixels from target area is on target.",
      {
         { "0",  NULL },
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "4",  NULL },
         { "5",  NULL },
         { "6",  NULL },
         { "7",  NULL },
         { "8",  NULL },
         { "9",  NULL },
         { "10", NULL },
         { "11", NULL },
         { "12", NULL },
         { "13", NULL },
         { "14", NULL },
         { "15", NULL },
         { "16", NULL },
         { "17", NULL },
         { "18", NULL },
         { "19", NULL },
         { "20", NULL },
         { NULL, NULL },
      },
      "6",
   },
   {
      "fceumm_sndquality",
      "Sound Quality",
      "Enable higher quality sounds in exchange for more processing power required",
      {
         { "Low",       NULL },
         { "High",      NULL },
         { "Very High", NULL },
         { NULL, NULL},
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      "Swap Duty Cycles",
      "Simulates the sound from famiclones has the pulse wave channels duty cycle bits reversed.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL},
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      "Master Volume",
      "Change master volume level",
      {
         { "0",  NULL },
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "4",  NULL },
         { "5",  NULL },
         { "6",  NULL },
         { "7",  NULL },
         { "8",  NULL },
         { "9",  NULL },
         { "10", NULL },
         { NULL, NULL},
      },
      "7",
   },
   {
      "fceumm_apu_1",
      "Channel 1 (Square 1)",
      "Enables or disables pulse wave generator audio output 1.",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      "Channel 2 (Square 2)",
      "Enables or disables pulse wave generator audio output 2.",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      "Channel 3 (Triangle)",
      "Enables or disables triangle wave generator audio output.",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      "Channel 4 (Noise)",
      "Enables or disables noise generator audio output.",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      "Channel 5 (PCM)",
      "Enables or disables delta modulation channel audio output.",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL},
      },
      "enabled",
   },
   {
      "fceumm_show_adv_system_options",
      "Show Advanced System Options",
      "Show advanced system options and tweaks.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      "Show Advanced Sound Options",
      "Show advanced sound options and tweaks.",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },

   { NULL, NULL, NULL, { {0} }, NULL },
};

struct retro_core_option_definition option_defs_empty = {
   NULL, NULL, NULL, { {0} }, NULL
};

struct retro_core_option_definition option_defs_us[MAX_CORE_OPTIONS];

/* RETRO_LANGUAGE_JAPANESE */

/* RETRO_LANGUAGE_FRENCH */

/* RETRO_LANGUAGE_SPANISH */

/* RETRO_LANGUAGE_GERMAN */

/* RETRO_LANGUAGE_ITALIAN */

/* RETRO_LANGUAGE_DUTCH */

/* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */

/* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */

/* RETRO_LANGUAGE_RUSSIAN */

/* RETRO_LANGUAGE_KOREAN */

/* RETRO_LANGUAGE_CHINESE_TRADITIONAL */

/* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */

/* RETRO_LANGUAGE_ESPERANTO */

/* RETRO_LANGUAGE_POLISH */

/* RETRO_LANGUAGE_VIETNAMESE */

/* RETRO_LANGUAGE_ARABIC */

/* RETRO_LANGUAGE_GREEK */

/* RETRO_LANGUAGE_TURKISH */

/*
 ********************************
 * Language Mapping
 ********************************
*/

struct retro_core_option_definition *option_defs_intl[RETRO_LANGUAGE_LAST] = {
   option_defs_us, /* RETRO_LANGUAGE_ENGLISH */
   NULL,           /* RETRO_LANGUAGE_JAPANESE */
   NULL,           /* RETRO_LANGUAGE_FRENCH */
   NULL,           /* RETRO_LANGUAGE_SPANISH */
   NULL,           /* RETRO_LANGUAGE_GERMAN */
   NULL,           /* RETRO_LANGUAGE_ITALIAN */
   NULL,           /* RETRO_LANGUAGE_DUTCH */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */
   NULL,           /* RETRO_LANGUAGE_RUSSIAN */
   NULL,           /* RETRO_LANGUAGE_KOREAN */
   NULL,           /* RETRO_LANGUAGE_CHINESE_TRADITIONAL */
   NULL,           /* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */
   NULL,           /* RETRO_LANGUAGE_ESPERANTO */
   NULL,           /* RETRO_LANGUAGE_POLISH */
   NULL,           /* RETRO_LANGUAGE_VIETNAMESE */
   NULL,           /* RETRO_LANGUAGE_ARABIC */
   NULL,           /* RETRO_LANGUAGE_GREEK */
   NULL,           /* RETRO_LANGUAGE_TURKISH */
};

/*
 ********************************
 * Functions
 ********************************
*/

/* Handles configuration/setting of core options.
 * Should only be called inside retro_set_environment().
 * > We place the function body in the header to avoid the
 *   necessity of adding more .c files (i.e. want this to
 *   be as painless as possible for core devs)
 */

static INLINE void libretro_set_core_options(retro_environment_t environ_cb)
{
   unsigned version = 0;

   if (!environ_cb)
      return;

   if (environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version) && (version == 1))
   {
      struct retro_core_options_intl core_options_intl;
      unsigned language = 0;

      core_options_intl.us    = option_defs_us;
      core_options_intl.local = NULL;

      if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
          (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH))
         core_options_intl.local = option_defs_intl[language];

      environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL, &core_options_intl);
   }
   else
   {
      size_t i;
      size_t option_index              = 0;
      size_t num_options               = 0;
      struct retro_variable *variables = NULL;
      char **values_buf                = NULL;

      /* Determine number of options
       * > Note: We are going to skip a number of irrelevant
       *   core options when building the retro_variable array,
       *   but we'll allocate space for all of them. The difference
       *   in resource usage is negligible, and this allows us to
       *   keep the code 'cleaner' */
      while (true)
      {
         if (option_defs_us[num_options].key)
            num_options++;
         else
            break;
      }

      /* Allocate arrays */
      variables  = (struct retro_variable *)calloc(num_options + 1, sizeof(struct retro_variable));
      values_buf = (char **)calloc(num_options, sizeof(char *));

      if (!variables || !values_buf)
         goto error;

      /* Copy parameters from option_defs_us array */
      for (i = 0; i < num_options; i++)
      {
         const char *key                        = option_defs_us[i].key;
         const char *desc                       = option_defs_us[i].desc;
         const char *default_value              = option_defs_us[i].default_value;
         struct retro_core_option_value *values = option_defs_us[i].values;
         size_t buf_len                         = 3;
         size_t default_index                   = 0;

         values_buf[i] = NULL;

         /* Skip options that are irrelevant when using the
          * old style core options interface */
         if ((strcmp(key, "fceumm_advance_sound_options") == 0))
            continue;

         if (desc)
         {
            size_t num_values = 0;

            /* Determine number of values */
            while (true)
            {
               if (values[num_values].value)
               {
                  /* Check if this is the default value */
                  if (default_value)
                     if (strcmp(values[num_values].value, default_value) == 0)
                        default_index = num_values;

                  buf_len += strlen(values[num_values].value);
                  num_values++;
               }
               else
                  break;
            }

            /* Build values string */
            if (num_values > 1)
            {
               size_t j;

               buf_len += num_values - 1;
               buf_len += strlen(desc);

               values_buf[i] = (char *)calloc(buf_len, sizeof(char));
               if (!values_buf[i])
                  goto error;

               strcpy(values_buf[i], desc);
               strcat(values_buf[i], "; ");

               /* Default value goes first */
               strcat(values_buf[i], values[default_index].value);

               /* Add remaining values */
               for (j = 0; j < num_values; j++)
               {
                  if (j != default_index)
                  {
                     strcat(values_buf[i], "|");
                     strcat(values_buf[i], values[j].value);
                  }
               }
            }
         }

         variables[option_index].key   = key;
         variables[option_index].value = values_buf[i];
         option_index++;
      }

      /* Set variables */
      environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);

error:

      /* Clean up */
      if (values_buf)
      {
         for (i = 0; i < num_options; i++)
         {
            if (values_buf[i])
            {
               free(values_buf[i]);
               values_buf[i] = NULL;
            }
         }

         free(values_buf);
         values_buf = NULL;
      }

      if (variables)
      {
         free(variables);
         variables = NULL;
      }
   }
}

#ifdef __cplusplus
}
#endif

#endif
