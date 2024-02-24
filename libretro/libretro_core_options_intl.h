#ifndef LIBRETRO_CORE_OPTIONS_INTL_H__
#define LIBRETRO_CORE_OPTIONS_INTL_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1500 && _MSC_VER < 1900)
/* https://support.microsoft.com/en-us/kb/980263 */
#pragma execution_character_set("utf-8")
#pragma warning(disable:4566)
#endif

#include <libretro.h>

/*
 ********************************
 * VERSION: 2.0
 ********************************
 *
 * - 2.0: Add support for core options v2 interface
 * - 1.3: Move translations to libretro_core_options_intl.h
 *        - libretro_core_options_intl.h includes BOM and utf-8
 *          fix for MSVC 2010-2013
 *        - Added HAVE_NO_LANGEXTRA flag to disable translations
 *          on platforms/compilers without BOM support
 * - 1.2: Use core options v1 interface when
 *        RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION is >= 1
 *        (previously required RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION == 1)
 * - 1.1: Support generation of core options v0 retro_core_option_value
 *        arrays containing options with a single value
 * - 1.0: First commit
*/

#ifdef __cplusplus
extern "C" {
#endif

/*
 ********************************
 * Core Option Definitions
 ********************************
*/
/* RETRO_LANGUAGE_AR */

#define CATEGORY_SYSTEM_LABEL_AR NULL
#define CATEGORY_SYSTEM_INFO_0_AR NULL
#define CATEGORY_VIDEO_LABEL_AR "فيديو"
#define CATEGORY_VIDEO_INFO_0_AR NULL
#define CATEGORY_VIDEO_INFO_1_AR NULL
#define CATEGORY_AUDIO_LABEL_AR "نظام تشغيل الصوت"
#define CATEGORY_AUDIO_INFO_0_AR NULL
#define CATEGORY_INPUT_LABEL_AR "الإدخال"
#define CATEGORY_INPUT_INFO_0_AR NULL
#define CATEGORY_HACKS_LABEL_AR NULL
#define CATEGORY_HACKS_INFO_0_AR NULL
#define CATEGORY_DIP_SWITCH_LABEL_AR NULL
#define CATEGORY_DIP_SWITCH_INFO_0_AR NULL
#define FCEUMM_REGION_LABEL_AR NULL
#define FCEUMM_REGION_INFO_0_AR NULL
#define OPTION_VAL_AUTO_AR NULL
#define OPTION_VAL_NTSC_AR NULL
#define OPTION_VAL_PAL_AR NULL
#define OPTION_VAL_DENDY_AR NULL
#define FCEUMM_GAME_GENIE_LABEL_AR NULL
#define FCEUMM_GAME_GENIE_INFO_0_AR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_AR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_AR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_AR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_AR NULL
#define FCEUMM_ASPECT_LABEL_AR NULL
#define FCEUMM_ASPECT_INFO_0_AR NULL
#define OPTION_VAL_8_7_PAR_AR NULL
#define OPTION_VAL_4_3_AR NULL
#define OPTION_VAL_PP_AR NULL
#define FCEUMM_OVERSCAN_LABEL_AR NULL
#define FCEUMM_OVERSCAN_INFO_0_AR NULL
#define FCEUMM_OVERSCAN_H_LABEL_AR NULL
#define FCEUMM_OVERSCAN_H_INFO_0_AR NULL
#define FCEUMM_OVERSCAN_V_LABEL_AR NULL
#define FCEUMM_OVERSCAN_V_INFO_0_AR NULL
#define FCEUMM_PALETTE_LABEL_AR NULL
#define FCEUMM_PALETTE_INFO_0_AR NULL
#define OPTION_VAL_DEFAULT_AR NULL
#define OPTION_VAL_ASQREALC_AR NULL
#define OPTION_VAL_NINTENDO_VC_AR NULL
#define OPTION_VAL_RGB_AR NULL
#define OPTION_VAL_YUV_V3_AR NULL
#define OPTION_VAL_UNSATURATED_FINAL_AR NULL
#define OPTION_VAL_SONY_CXA2025AS_US_AR NULL
#define OPTION_VAL_BMF_FINAL2_AR NULL
#define OPTION_VAL_BMF_FINAL3_AR NULL
#define OPTION_VAL_SMOOTH_FBX_AR NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_AR NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_AR NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_AR NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_AR NULL
#define OPTION_VAL_NESCAP_AR NULL
#define OPTION_VAL_WAVEBEAM_AR NULL
#define OPTION_VAL_RAW_AR NULL
#define OPTION_VAL_CUSTOM_AR NULL
#define FCEUMM_NTSC_FILTER_LABEL_AR NULL
#define FCEUMM_NTSC_FILTER_INFO_0_AR NULL
#define OPTION_VAL_COMPOSITE_AR NULL
#define OPTION_VAL_SVIDEO_AR NULL
#define OPTION_VAL_RGB_O9_AR NULL
#define OPTION_VAL_MONOCHROME_AR NULL
#define FCEUMM_SNDQUALITY_LABEL_AR NULL
#define FCEUMM_SNDQUALITY_INFO_0_AR NULL
#define OPTION_VAL_LOW_AR NULL
#define OPTION_VAL_HIGH_AR NULL
#define OPTION_VAL_VERY_HIGH_AR NULL
#define FCEUMM_SWAPDUTY_LABEL_AR NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_AR NULL
#define FCEUMM_SWAPDUTY_INFO_0_AR NULL
#define FCEUMM_SNDVOLUME_LABEL_AR NULL
#define FCEUMM_SNDVOLUME_INFO_0_AR NULL
#define OPTION_VAL_0_AR NULL
#define OPTION_VAL_1_AR NULL
#define OPTION_VAL_2_AR NULL
#define OPTION_VAL_3_AR NULL
#define OPTION_VAL_4_AR NULL
#define OPTION_VAL_5_AR NULL
#define OPTION_VAL_6_AR NULL
#define OPTION_VAL_7_AR NULL
#define OPTION_VAL_8_AR NULL
#define OPTION_VAL_9_AR NULL
#define OPTION_VAL_10_AR NULL
#define FCEUMM_APU_1_LABEL_AR NULL
#define FCEUMM_APU_1_LABEL_CAT_AR NULL
#define FCEUMM_APU_1_INFO_0_AR NULL
#define FCEUMM_APU_2_LABEL_AR NULL
#define FCEUMM_APU_2_LABEL_CAT_AR NULL
#define FCEUMM_APU_2_INFO_0_AR NULL
#define FCEUMM_APU_3_LABEL_AR NULL
#define FCEUMM_APU_3_LABEL_CAT_AR NULL
#define FCEUMM_APU_3_INFO_0_AR NULL
#define FCEUMM_APU_4_LABEL_AR NULL
#define FCEUMM_APU_4_LABEL_CAT_AR NULL
#define FCEUMM_APU_4_INFO_0_AR NULL
#define FCEUMM_APU_5_LABEL_AR NULL
#define FCEUMM_APU_5_LABEL_CAT_AR NULL
#define FCEUMM_APU_5_INFO_0_AR NULL
#define FCEUMM_TURBO_ENABLE_LABEL_AR NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_AR NULL
#define OPTION_VAL_NONE_AR NULL
#define OPTION_VAL_PLAYER_1_AR NULL
#define OPTION_VAL_PLAYER_2_AR NULL
#define OPTION_VAL_BOTH_AR NULL
#define FCEUMM_TURBO_DELAY_LABEL_AR NULL
#define FCEUMM_TURBO_DELAY_INFO_0_AR NULL
#define FCEUMM_ZAPPER_MODE_LABEL_AR NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_AR NULL
#define OPTION_VAL_LIGHTGUN_AR NULL
#define OPTION_VAL_TOUCHSCREEN_AR NULL
#define OPTION_VAL_MOUSE_AR NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_AR NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_AR NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_AR NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_AR NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_AR NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_AR NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_AR NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_AR NULL
#define FCEUMM_OVERCLOCKING_LABEL_AR NULL
#define FCEUMM_OVERCLOCKING_INFO_0_AR NULL
#define OPTION_VAL_2X_POSTRENDER_AR NULL
#define OPTION_VAL_2X_VBLANK_AR NULL
#define FCEUMM_RAMSTATE_LABEL_AR NULL
#define FCEUMM_RAMSTATE_INFO_0_AR NULL
#define OPTION_VAL_FILL_FF_AR NULL
#define OPTION_VAL_FILL_00_AR NULL
#define OPTION_VAL_RANDOM_AR NULL

struct retro_core_option_v2_category option_cats_ar[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_AR,
      CATEGORY_SYSTEM_INFO_0_AR
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_AR,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_AR
#else
      CATEGORY_VIDEO_INFO_1_AR
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_AR,
      CATEGORY_AUDIO_INFO_0_AR
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_AR,
      CATEGORY_INPUT_INFO_0_AR
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_AR,
      CATEGORY_HACKS_INFO_0_AR
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_AR,
      CATEGORY_DIP_SWITCH_INFO_0_AR
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_ar[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_AR,
      NULL,
      FCEUMM_REGION_INFO_0_AR,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_AR },
         { "NTSC",  OPTION_VAL_NTSC_AR },
         { "PAL",   OPTION_VAL_PAL_AR },
         { "Dendy", OPTION_VAL_DENDY_AR },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_AR,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_AR,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_AR,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_AR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_AR,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_AR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_AR,
      NULL,
      FCEUMM_ASPECT_INFO_0_AR,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_AR },
         { "4:3",     OPTION_VAL_4_3_AR },
         { "PP",      OPTION_VAL_PP_AR },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_AR,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_AR,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_AR,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_AR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_AR,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_AR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_AR,
      NULL,
      FCEUMM_PALETTE_INFO_0_AR,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_AR },
         { "asqrealc",             OPTION_VAL_ASQREALC_AR },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_AR },
         { "rgb",                  OPTION_VAL_RGB_AR },
         { "yuv-v3",               OPTION_VAL_YUV_V3_AR },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_AR },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_AR },
         { "pal",                  OPTION_VAL_PAL_AR },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_AR },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_AR },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_AR },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_AR },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_AR },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_AR },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_AR },
         { "nescap",               OPTION_VAL_NESCAP_AR },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_AR },
         { "raw",                  OPTION_VAL_RAW_AR },
         { "custom",               OPTION_VAL_CUSTOM_AR },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_AR,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_AR,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_AR },
         { "svideo",     OPTION_VAL_SVIDEO_AR },
         { "rgb",        OPTION_VAL_RGB_O9_AR },
         { "monochrome", OPTION_VAL_MONOCHROME_AR },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_AR,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_AR,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_AR },
         { "High",      OPTION_VAL_HIGH_AR },
         { "Very High", OPTION_VAL_VERY_HIGH_AR },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_AR,
      FCEUMM_SWAPDUTY_LABEL_CAT_AR,
      FCEUMM_SWAPDUTY_INFO_0_AR,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_AR,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_AR,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_AR },
         { "1",  OPTION_VAL_1_AR },
         { "2",  OPTION_VAL_2_AR },
         { "3",  OPTION_VAL_3_AR },
         { "4",  OPTION_VAL_4_AR },
         { "5",  OPTION_VAL_5_AR },
         { "6",  OPTION_VAL_6_AR },
         { "7",  OPTION_VAL_7_AR },
         { "8",  OPTION_VAL_8_AR },
         { "9",  OPTION_VAL_9_AR },
         { "10", OPTION_VAL_10_AR },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_AR,
      FCEUMM_APU_1_LABEL_CAT_AR,
      FCEUMM_APU_1_INFO_0_AR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_AR,
      FCEUMM_APU_2_LABEL_CAT_AR,
      FCEUMM_APU_2_INFO_0_AR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_AR,
      FCEUMM_APU_3_LABEL_CAT_AR,
      FCEUMM_APU_3_INFO_0_AR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_AR,
      FCEUMM_APU_4_LABEL_CAT_AR,
      FCEUMM_APU_4_INFO_0_AR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_AR,
      FCEUMM_APU_5_LABEL_CAT_AR,
      FCEUMM_APU_5_INFO_0_AR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_AR,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_AR,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_AR },
         { "Player 1", OPTION_VAL_PLAYER_1_AR },
         { "Player 2", OPTION_VAL_PLAYER_2_AR },
         { "Both",     OPTION_VAL_BOTH_AR },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_AR,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_AR,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_AR,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_AR,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_AR },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_AR },
         { "mouse",       OPTION_VAL_MOUSE_AR },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_AR,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_AR,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_AR,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_AR,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_AR,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_AR,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_AR,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_AR,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_AR,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_AR,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_AR },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_AR },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_AR,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_AR,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_AR },
         { "fill $00", OPTION_VAL_FILL_00_AR },
         { "random",   OPTION_VAL_RANDOM_AR },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_ar = {
   option_cats_ar,
   option_defs_ar
};

/* RETRO_LANGUAGE_AST */

#define CATEGORY_SYSTEM_LABEL_AST "Sistema"
#define CATEGORY_SYSTEM_INFO_0_AST NULL
#define CATEGORY_VIDEO_LABEL_AST "Videu"
#define CATEGORY_VIDEO_INFO_0_AST NULL
#define CATEGORY_VIDEO_INFO_1_AST NULL
#define CATEGORY_AUDIO_LABEL_AST "Audiu"
#define CATEGORY_AUDIO_INFO_0_AST NULL
#define CATEGORY_INPUT_LABEL_AST "Entrada"
#define CATEGORY_INPUT_INFO_0_AST NULL
#define CATEGORY_HACKS_LABEL_AST NULL
#define CATEGORY_HACKS_INFO_0_AST NULL
#define CATEGORY_DIP_SWITCH_LABEL_AST NULL
#define CATEGORY_DIP_SWITCH_INFO_0_AST NULL
#define FCEUMM_REGION_LABEL_AST NULL
#define FCEUMM_REGION_INFO_0_AST NULL
#define OPTION_VAL_AUTO_AST NULL
#define OPTION_VAL_NTSC_AST NULL
#define OPTION_VAL_PAL_AST NULL
#define OPTION_VAL_DENDY_AST NULL
#define FCEUMM_GAME_GENIE_LABEL_AST NULL
#define FCEUMM_GAME_GENIE_INFO_0_AST NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_AST NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_AST NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_AST NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_AST NULL
#define FCEUMM_ASPECT_LABEL_AST NULL
#define FCEUMM_ASPECT_INFO_0_AST NULL
#define OPTION_VAL_8_7_PAR_AST NULL
#define OPTION_VAL_4_3_AST NULL
#define OPTION_VAL_PP_AST NULL
#define FCEUMM_OVERSCAN_LABEL_AST NULL
#define FCEUMM_OVERSCAN_INFO_0_AST NULL
#define FCEUMM_OVERSCAN_H_LABEL_AST NULL
#define FCEUMM_OVERSCAN_H_INFO_0_AST NULL
#define FCEUMM_OVERSCAN_V_LABEL_AST NULL
#define FCEUMM_OVERSCAN_V_INFO_0_AST NULL
#define FCEUMM_PALETTE_LABEL_AST NULL
#define FCEUMM_PALETTE_INFO_0_AST NULL
#define OPTION_VAL_DEFAULT_AST NULL
#define OPTION_VAL_ASQREALC_AST NULL
#define OPTION_VAL_NINTENDO_VC_AST NULL
#define OPTION_VAL_RGB_AST NULL
#define OPTION_VAL_YUV_V3_AST NULL
#define OPTION_VAL_UNSATURATED_FINAL_AST NULL
#define OPTION_VAL_SONY_CXA2025AS_US_AST NULL
#define OPTION_VAL_BMF_FINAL2_AST NULL
#define OPTION_VAL_BMF_FINAL3_AST NULL
#define OPTION_VAL_SMOOTH_FBX_AST NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_AST NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_AST NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_AST NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_AST NULL
#define OPTION_VAL_NESCAP_AST NULL
#define OPTION_VAL_WAVEBEAM_AST NULL
#define OPTION_VAL_RAW_AST NULL
#define OPTION_VAL_CUSTOM_AST NULL
#define FCEUMM_NTSC_FILTER_LABEL_AST NULL
#define FCEUMM_NTSC_FILTER_INFO_0_AST NULL
#define OPTION_VAL_COMPOSITE_AST NULL
#define OPTION_VAL_SVIDEO_AST NULL
#define OPTION_VAL_RGB_O9_AST NULL
#define OPTION_VAL_MONOCHROME_AST NULL
#define FCEUMM_SNDQUALITY_LABEL_AST NULL
#define FCEUMM_SNDQUALITY_INFO_0_AST NULL
#define OPTION_VAL_LOW_AST NULL
#define OPTION_VAL_HIGH_AST NULL
#define OPTION_VAL_VERY_HIGH_AST NULL
#define FCEUMM_SWAPDUTY_LABEL_AST NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_AST NULL
#define FCEUMM_SWAPDUTY_INFO_0_AST NULL
#define FCEUMM_SNDVOLUME_LABEL_AST NULL
#define FCEUMM_SNDVOLUME_INFO_0_AST NULL
#define OPTION_VAL_0_AST NULL
#define OPTION_VAL_1_AST NULL
#define OPTION_VAL_2_AST NULL
#define OPTION_VAL_3_AST NULL
#define OPTION_VAL_4_AST NULL
#define OPTION_VAL_5_AST NULL
#define OPTION_VAL_6_AST NULL
#define OPTION_VAL_7_AST NULL
#define OPTION_VAL_8_AST NULL
#define OPTION_VAL_9_AST NULL
#define OPTION_VAL_10_AST NULL
#define FCEUMM_APU_1_LABEL_AST NULL
#define FCEUMM_APU_1_LABEL_CAT_AST NULL
#define FCEUMM_APU_1_INFO_0_AST NULL
#define FCEUMM_APU_2_LABEL_AST NULL
#define FCEUMM_APU_2_LABEL_CAT_AST NULL
#define FCEUMM_APU_2_INFO_0_AST NULL
#define FCEUMM_APU_3_LABEL_AST NULL
#define FCEUMM_APU_3_LABEL_CAT_AST NULL
#define FCEUMM_APU_3_INFO_0_AST NULL
#define FCEUMM_APU_4_LABEL_AST NULL
#define FCEUMM_APU_4_LABEL_CAT_AST NULL
#define FCEUMM_APU_4_INFO_0_AST NULL
#define FCEUMM_APU_5_LABEL_AST NULL
#define FCEUMM_APU_5_LABEL_CAT_AST NULL
#define FCEUMM_APU_5_INFO_0_AST NULL
#define FCEUMM_TURBO_ENABLE_LABEL_AST NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_AST NULL
#define OPTION_VAL_NONE_AST NULL
#define OPTION_VAL_PLAYER_1_AST NULL
#define OPTION_VAL_PLAYER_2_AST NULL
#define OPTION_VAL_BOTH_AST NULL
#define FCEUMM_TURBO_DELAY_LABEL_AST NULL
#define FCEUMM_TURBO_DELAY_INFO_0_AST NULL
#define FCEUMM_ZAPPER_MODE_LABEL_AST NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_AST NULL
#define OPTION_VAL_LIGHTGUN_AST NULL
#define OPTION_VAL_TOUCHSCREEN_AST NULL
#define OPTION_VAL_MOUSE_AST NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_AST NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_AST NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_AST NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_AST NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_AST NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_AST NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_AST NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_AST NULL
#define FCEUMM_OVERCLOCKING_LABEL_AST NULL
#define FCEUMM_OVERCLOCKING_INFO_0_AST NULL
#define OPTION_VAL_2X_POSTRENDER_AST NULL
#define OPTION_VAL_2X_VBLANK_AST NULL
#define FCEUMM_RAMSTATE_LABEL_AST NULL
#define FCEUMM_RAMSTATE_INFO_0_AST NULL
#define OPTION_VAL_FILL_FF_AST NULL
#define OPTION_VAL_FILL_00_AST NULL
#define OPTION_VAL_RANDOM_AST NULL

struct retro_core_option_v2_category option_cats_ast[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_AST,
      CATEGORY_SYSTEM_INFO_0_AST
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_AST,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_AST
#else
      CATEGORY_VIDEO_INFO_1_AST
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_AST,
      CATEGORY_AUDIO_INFO_0_AST
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_AST,
      CATEGORY_INPUT_INFO_0_AST
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_AST,
      CATEGORY_HACKS_INFO_0_AST
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_AST,
      CATEGORY_DIP_SWITCH_INFO_0_AST
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_ast[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_AST,
      NULL,
      FCEUMM_REGION_INFO_0_AST,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_AST },
         { "NTSC",  OPTION_VAL_NTSC_AST },
         { "PAL",   OPTION_VAL_PAL_AST },
         { "Dendy", OPTION_VAL_DENDY_AST },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_AST,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_AST,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_AST,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_AST,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_AST,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_AST,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_AST,
      NULL,
      FCEUMM_ASPECT_INFO_0_AST,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_AST },
         { "4:3",     OPTION_VAL_4_3_AST },
         { "PP",      OPTION_VAL_PP_AST },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_AST,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_AST,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_AST,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_AST,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_AST,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_AST,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_AST,
      NULL,
      FCEUMM_PALETTE_INFO_0_AST,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_AST },
         { "asqrealc",             OPTION_VAL_ASQREALC_AST },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_AST },
         { "rgb",                  OPTION_VAL_RGB_AST },
         { "yuv-v3",               OPTION_VAL_YUV_V3_AST },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_AST },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_AST },
         { "pal",                  OPTION_VAL_PAL_AST },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_AST },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_AST },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_AST },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_AST },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_AST },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_AST },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_AST },
         { "nescap",               OPTION_VAL_NESCAP_AST },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_AST },
         { "raw",                  OPTION_VAL_RAW_AST },
         { "custom",               OPTION_VAL_CUSTOM_AST },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_AST,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_AST,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_AST },
         { "svideo",     OPTION_VAL_SVIDEO_AST },
         { "rgb",        OPTION_VAL_RGB_O9_AST },
         { "monochrome", OPTION_VAL_MONOCHROME_AST },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_AST,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_AST,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_AST },
         { "High",      OPTION_VAL_HIGH_AST },
         { "Very High", OPTION_VAL_VERY_HIGH_AST },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_AST,
      FCEUMM_SWAPDUTY_LABEL_CAT_AST,
      FCEUMM_SWAPDUTY_INFO_0_AST,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_AST,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_AST,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_AST },
         { "1",  OPTION_VAL_1_AST },
         { "2",  OPTION_VAL_2_AST },
         { "3",  OPTION_VAL_3_AST },
         { "4",  OPTION_VAL_4_AST },
         { "5",  OPTION_VAL_5_AST },
         { "6",  OPTION_VAL_6_AST },
         { "7",  OPTION_VAL_7_AST },
         { "8",  OPTION_VAL_8_AST },
         { "9",  OPTION_VAL_9_AST },
         { "10", OPTION_VAL_10_AST },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_AST,
      FCEUMM_APU_1_LABEL_CAT_AST,
      FCEUMM_APU_1_INFO_0_AST,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_AST,
      FCEUMM_APU_2_LABEL_CAT_AST,
      FCEUMM_APU_2_INFO_0_AST,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_AST,
      FCEUMM_APU_3_LABEL_CAT_AST,
      FCEUMM_APU_3_INFO_0_AST,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_AST,
      FCEUMM_APU_4_LABEL_CAT_AST,
      FCEUMM_APU_4_INFO_0_AST,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_AST,
      FCEUMM_APU_5_LABEL_CAT_AST,
      FCEUMM_APU_5_INFO_0_AST,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_AST,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_AST,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_AST },
         { "Player 1", OPTION_VAL_PLAYER_1_AST },
         { "Player 2", OPTION_VAL_PLAYER_2_AST },
         { "Both",     OPTION_VAL_BOTH_AST },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_AST,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_AST,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_AST,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_AST,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_AST },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_AST },
         { "mouse",       OPTION_VAL_MOUSE_AST },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_AST,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_AST,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_AST,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_AST,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_AST,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_AST,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_AST,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_AST,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_AST,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_AST,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_AST },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_AST },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_AST,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_AST,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_AST },
         { "fill $00", OPTION_VAL_FILL_00_AST },
         { "random",   OPTION_VAL_RANDOM_AST },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_ast = {
   option_cats_ast,
   option_defs_ast
};

/* RETRO_LANGUAGE_CA */

#define CATEGORY_SYSTEM_LABEL_CA NULL
#define CATEGORY_SYSTEM_INFO_0_CA NULL
#define CATEGORY_VIDEO_LABEL_CA NULL
#define CATEGORY_VIDEO_INFO_0_CA NULL
#define CATEGORY_VIDEO_INFO_1_CA NULL
#define CATEGORY_AUDIO_LABEL_CA NULL
#define CATEGORY_AUDIO_INFO_0_CA NULL
#define CATEGORY_INPUT_LABEL_CA NULL
#define CATEGORY_INPUT_INFO_0_CA NULL
#define CATEGORY_HACKS_LABEL_CA NULL
#define CATEGORY_HACKS_INFO_0_CA NULL
#define CATEGORY_DIP_SWITCH_LABEL_CA NULL
#define CATEGORY_DIP_SWITCH_INFO_0_CA NULL
#define FCEUMM_REGION_LABEL_CA NULL
#define FCEUMM_REGION_INFO_0_CA NULL
#define OPTION_VAL_AUTO_CA NULL
#define OPTION_VAL_NTSC_CA NULL
#define OPTION_VAL_PAL_CA NULL
#define OPTION_VAL_DENDY_CA NULL
#define FCEUMM_GAME_GENIE_LABEL_CA NULL
#define FCEUMM_GAME_GENIE_INFO_0_CA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CA NULL
#define FCEUMM_ASPECT_LABEL_CA NULL
#define FCEUMM_ASPECT_INFO_0_CA NULL
#define OPTION_VAL_8_7_PAR_CA NULL
#define OPTION_VAL_4_3_CA NULL
#define OPTION_VAL_PP_CA NULL
#define FCEUMM_OVERSCAN_LABEL_CA NULL
#define FCEUMM_OVERSCAN_INFO_0_CA NULL
#define FCEUMM_OVERSCAN_H_LABEL_CA NULL
#define FCEUMM_OVERSCAN_H_INFO_0_CA NULL
#define FCEUMM_OVERSCAN_V_LABEL_CA NULL
#define FCEUMM_OVERSCAN_V_INFO_0_CA NULL
#define FCEUMM_PALETTE_LABEL_CA NULL
#define FCEUMM_PALETTE_INFO_0_CA NULL
#define OPTION_VAL_DEFAULT_CA NULL
#define OPTION_VAL_ASQREALC_CA NULL
#define OPTION_VAL_NINTENDO_VC_CA NULL
#define OPTION_VAL_RGB_CA NULL
#define OPTION_VAL_YUV_V3_CA NULL
#define OPTION_VAL_UNSATURATED_FINAL_CA NULL
#define OPTION_VAL_SONY_CXA2025AS_US_CA NULL
#define OPTION_VAL_BMF_FINAL2_CA NULL
#define OPTION_VAL_BMF_FINAL3_CA NULL
#define OPTION_VAL_SMOOTH_FBX_CA NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_CA NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_CA NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_CA NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_CA NULL
#define OPTION_VAL_NESCAP_CA NULL
#define OPTION_VAL_WAVEBEAM_CA NULL
#define OPTION_VAL_RAW_CA NULL
#define OPTION_VAL_CUSTOM_CA NULL
#define FCEUMM_NTSC_FILTER_LABEL_CA NULL
#define FCEUMM_NTSC_FILTER_INFO_0_CA NULL
#define OPTION_VAL_COMPOSITE_CA NULL
#define OPTION_VAL_SVIDEO_CA NULL
#define OPTION_VAL_RGB_O9_CA NULL
#define OPTION_VAL_MONOCHROME_CA NULL
#define FCEUMM_SNDQUALITY_LABEL_CA NULL
#define FCEUMM_SNDQUALITY_INFO_0_CA NULL
#define OPTION_VAL_LOW_CA NULL
#define OPTION_VAL_HIGH_CA NULL
#define OPTION_VAL_VERY_HIGH_CA NULL
#define FCEUMM_SWAPDUTY_LABEL_CA NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_CA NULL
#define FCEUMM_SWAPDUTY_INFO_0_CA NULL
#define FCEUMM_SNDVOLUME_LABEL_CA NULL
#define FCEUMM_SNDVOLUME_INFO_0_CA NULL
#define OPTION_VAL_0_CA NULL
#define OPTION_VAL_1_CA NULL
#define OPTION_VAL_2_CA NULL
#define OPTION_VAL_3_CA NULL
#define OPTION_VAL_4_CA NULL
#define OPTION_VAL_5_CA NULL
#define OPTION_VAL_6_CA NULL
#define OPTION_VAL_7_CA NULL
#define OPTION_VAL_8_CA NULL
#define OPTION_VAL_9_CA NULL
#define OPTION_VAL_10_CA NULL
#define FCEUMM_APU_1_LABEL_CA NULL
#define FCEUMM_APU_1_LABEL_CAT_CA NULL
#define FCEUMM_APU_1_INFO_0_CA NULL
#define FCEUMM_APU_2_LABEL_CA NULL
#define FCEUMM_APU_2_LABEL_CAT_CA NULL
#define FCEUMM_APU_2_INFO_0_CA NULL
#define FCEUMM_APU_3_LABEL_CA NULL
#define FCEUMM_APU_3_LABEL_CAT_CA NULL
#define FCEUMM_APU_3_INFO_0_CA NULL
#define FCEUMM_APU_4_LABEL_CA NULL
#define FCEUMM_APU_4_LABEL_CAT_CA NULL
#define FCEUMM_APU_4_INFO_0_CA NULL
#define FCEUMM_APU_5_LABEL_CA NULL
#define FCEUMM_APU_5_LABEL_CAT_CA NULL
#define FCEUMM_APU_5_INFO_0_CA NULL
#define FCEUMM_TURBO_ENABLE_LABEL_CA NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_CA NULL
#define OPTION_VAL_NONE_CA NULL
#define OPTION_VAL_PLAYER_1_CA NULL
#define OPTION_VAL_PLAYER_2_CA NULL
#define OPTION_VAL_BOTH_CA NULL
#define FCEUMM_TURBO_DELAY_LABEL_CA NULL
#define FCEUMM_TURBO_DELAY_INFO_0_CA NULL
#define FCEUMM_ZAPPER_MODE_LABEL_CA NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_CA NULL
#define OPTION_VAL_LIGHTGUN_CA NULL
#define OPTION_VAL_TOUCHSCREEN_CA NULL
#define OPTION_VAL_MOUSE_CA NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_CA NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_CA NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_CA NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_CA NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_CA NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_CA NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_CA NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_CA NULL
#define FCEUMM_OVERCLOCKING_LABEL_CA NULL
#define FCEUMM_OVERCLOCKING_INFO_0_CA NULL
#define OPTION_VAL_2X_POSTRENDER_CA NULL
#define OPTION_VAL_2X_VBLANK_CA NULL
#define FCEUMM_RAMSTATE_LABEL_CA NULL
#define FCEUMM_RAMSTATE_INFO_0_CA NULL
#define OPTION_VAL_FILL_FF_CA NULL
#define OPTION_VAL_FILL_00_CA NULL
#define OPTION_VAL_RANDOM_CA NULL

struct retro_core_option_v2_category option_cats_ca[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_CA,
      CATEGORY_SYSTEM_INFO_0_CA
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_CA,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_CA
#else
      CATEGORY_VIDEO_INFO_1_CA
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_CA,
      CATEGORY_AUDIO_INFO_0_CA
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_CA,
      CATEGORY_INPUT_INFO_0_CA
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_CA,
      CATEGORY_HACKS_INFO_0_CA
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_CA,
      CATEGORY_DIP_SWITCH_INFO_0_CA
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_ca[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_CA,
      NULL,
      FCEUMM_REGION_INFO_0_CA,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_CA },
         { "NTSC",  OPTION_VAL_NTSC_CA },
         { "PAL",   OPTION_VAL_PAL_CA },
         { "Dendy", OPTION_VAL_DENDY_CA },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_CA,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_CA,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CA,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CA,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_CA,
      NULL,
      FCEUMM_ASPECT_INFO_0_CA,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_CA },
         { "4:3",     OPTION_VAL_4_3_CA },
         { "PP",      OPTION_VAL_PP_CA },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_CA,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_CA,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_CA,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_CA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_CA,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_CA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_CA,
      NULL,
      FCEUMM_PALETTE_INFO_0_CA,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_CA },
         { "asqrealc",             OPTION_VAL_ASQREALC_CA },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_CA },
         { "rgb",                  OPTION_VAL_RGB_CA },
         { "yuv-v3",               OPTION_VAL_YUV_V3_CA },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_CA },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_CA },
         { "pal",                  OPTION_VAL_PAL_CA },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_CA },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_CA },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_CA },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_CA },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_CA },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_CA },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_CA },
         { "nescap",               OPTION_VAL_NESCAP_CA },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_CA },
         { "raw",                  OPTION_VAL_RAW_CA },
         { "custom",               OPTION_VAL_CUSTOM_CA },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_CA,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_CA,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_CA },
         { "svideo",     OPTION_VAL_SVIDEO_CA },
         { "rgb",        OPTION_VAL_RGB_O9_CA },
         { "monochrome", OPTION_VAL_MONOCHROME_CA },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_CA,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_CA,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_CA },
         { "High",      OPTION_VAL_HIGH_CA },
         { "Very High", OPTION_VAL_VERY_HIGH_CA },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_CA,
      FCEUMM_SWAPDUTY_LABEL_CAT_CA,
      FCEUMM_SWAPDUTY_INFO_0_CA,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_CA,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_CA,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_CA },
         { "1",  OPTION_VAL_1_CA },
         { "2",  OPTION_VAL_2_CA },
         { "3",  OPTION_VAL_3_CA },
         { "4",  OPTION_VAL_4_CA },
         { "5",  OPTION_VAL_5_CA },
         { "6",  OPTION_VAL_6_CA },
         { "7",  OPTION_VAL_7_CA },
         { "8",  OPTION_VAL_8_CA },
         { "9",  OPTION_VAL_9_CA },
         { "10", OPTION_VAL_10_CA },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_CA,
      FCEUMM_APU_1_LABEL_CAT_CA,
      FCEUMM_APU_1_INFO_0_CA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_CA,
      FCEUMM_APU_2_LABEL_CAT_CA,
      FCEUMM_APU_2_INFO_0_CA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_CA,
      FCEUMM_APU_3_LABEL_CAT_CA,
      FCEUMM_APU_3_INFO_0_CA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_CA,
      FCEUMM_APU_4_LABEL_CAT_CA,
      FCEUMM_APU_4_INFO_0_CA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_CA,
      FCEUMM_APU_5_LABEL_CAT_CA,
      FCEUMM_APU_5_INFO_0_CA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_CA,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_CA,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_CA },
         { "Player 1", OPTION_VAL_PLAYER_1_CA },
         { "Player 2", OPTION_VAL_PLAYER_2_CA },
         { "Both",     OPTION_VAL_BOTH_CA },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_CA,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_CA,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_CA,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_CA,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_CA },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_CA },
         { "mouse",       OPTION_VAL_MOUSE_CA },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_CA,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_CA,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_CA,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_CA,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_CA,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_CA,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_CA,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_CA,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_CA,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_CA,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_CA },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_CA },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_CA,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_CA,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_CA },
         { "fill $00", OPTION_VAL_FILL_00_CA },
         { "random",   OPTION_VAL_RANDOM_CA },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_ca = {
   option_cats_ca,
   option_defs_ca
};

/* RETRO_LANGUAGE_CHS */

#define CATEGORY_SYSTEM_LABEL_CHS "系统"
#define CATEGORY_SYSTEM_INFO_0_CHS NULL
#define CATEGORY_VIDEO_LABEL_CHS "视频"
#define CATEGORY_VIDEO_INFO_0_CHS NULL
#define CATEGORY_VIDEO_INFO_1_CHS NULL
#define CATEGORY_AUDIO_LABEL_CHS "音频"
#define CATEGORY_AUDIO_INFO_0_CHS NULL
#define CATEGORY_INPUT_LABEL_CHS "输入"
#define CATEGORY_INPUT_INFO_0_CHS NULL
#define CATEGORY_HACKS_LABEL_CHS "模拟修改"
#define CATEGORY_HACKS_INFO_0_CHS "配置处理器超时和模拟精确参数，影响低级性能和兼容性。"
#define CATEGORY_DIP_SWITCH_LABEL_CHS NULL
#define CATEGORY_DIP_SWITCH_INFO_0_CHS NULL
#define FCEUMM_REGION_LABEL_CHS NULL
#define FCEUMM_REGION_INFO_0_CHS NULL
#define OPTION_VAL_AUTO_CHS NULL
#define OPTION_VAL_NTSC_CHS NULL
#define OPTION_VAL_PAL_CHS NULL
#define OPTION_VAL_DENDY_CHS NULL
#define FCEUMM_GAME_GENIE_LABEL_CHS NULL
#define FCEUMM_GAME_GENIE_INFO_0_CHS NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CHS NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CHS NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CHS NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CHS NULL
#define FCEUMM_ASPECT_LABEL_CHS NULL
#define FCEUMM_ASPECT_INFO_0_CHS NULL
#define OPTION_VAL_8_7_PAR_CHS NULL
#define OPTION_VAL_4_3_CHS NULL
#define OPTION_VAL_PP_CHS NULL
#define FCEUMM_OVERSCAN_LABEL_CHS NULL
#define FCEUMM_OVERSCAN_INFO_0_CHS NULL
#define FCEUMM_OVERSCAN_H_LABEL_CHS NULL
#define FCEUMM_OVERSCAN_H_INFO_0_CHS NULL
#define FCEUMM_OVERSCAN_V_LABEL_CHS NULL
#define FCEUMM_OVERSCAN_V_INFO_0_CHS NULL
#define FCEUMM_PALETTE_LABEL_CHS NULL
#define FCEUMM_PALETTE_INFO_0_CHS NULL
#define OPTION_VAL_DEFAULT_CHS NULL
#define OPTION_VAL_ASQREALC_CHS NULL
#define OPTION_VAL_NINTENDO_VC_CHS NULL
#define OPTION_VAL_RGB_CHS NULL
#define OPTION_VAL_YUV_V3_CHS NULL
#define OPTION_VAL_UNSATURATED_FINAL_CHS NULL
#define OPTION_VAL_SONY_CXA2025AS_US_CHS NULL
#define OPTION_VAL_BMF_FINAL2_CHS NULL
#define OPTION_VAL_BMF_FINAL3_CHS NULL
#define OPTION_VAL_SMOOTH_FBX_CHS NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_CHS NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_CHS NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_CHS NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_CHS NULL
#define OPTION_VAL_NESCAP_CHS NULL
#define OPTION_VAL_WAVEBEAM_CHS NULL
#define OPTION_VAL_RAW_CHS NULL
#define OPTION_VAL_CUSTOM_CHS NULL
#define FCEUMM_NTSC_FILTER_LABEL_CHS NULL
#define FCEUMM_NTSC_FILTER_INFO_0_CHS NULL
#define OPTION_VAL_COMPOSITE_CHS NULL
#define OPTION_VAL_SVIDEO_CHS NULL
#define OPTION_VAL_RGB_O9_CHS NULL
#define OPTION_VAL_MONOCHROME_CHS NULL
#define FCEUMM_SNDQUALITY_LABEL_CHS NULL
#define FCEUMM_SNDQUALITY_INFO_0_CHS NULL
#define OPTION_VAL_LOW_CHS NULL
#define OPTION_VAL_HIGH_CHS NULL
#define OPTION_VAL_VERY_HIGH_CHS NULL
#define FCEUMM_SWAPDUTY_LABEL_CHS NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_CHS NULL
#define FCEUMM_SWAPDUTY_INFO_0_CHS NULL
#define FCEUMM_SNDVOLUME_LABEL_CHS NULL
#define FCEUMM_SNDVOLUME_INFO_0_CHS NULL
#define OPTION_VAL_0_CHS NULL
#define OPTION_VAL_1_CHS NULL
#define OPTION_VAL_2_CHS NULL
#define OPTION_VAL_3_CHS NULL
#define OPTION_VAL_4_CHS NULL
#define OPTION_VAL_5_CHS NULL
#define OPTION_VAL_6_CHS NULL
#define OPTION_VAL_7_CHS NULL
#define OPTION_VAL_8_CHS NULL
#define OPTION_VAL_9_CHS NULL
#define OPTION_VAL_10_CHS NULL
#define FCEUMM_APU_1_LABEL_CHS NULL
#define FCEUMM_APU_1_LABEL_CAT_CHS NULL
#define FCEUMM_APU_1_INFO_0_CHS NULL
#define FCEUMM_APU_2_LABEL_CHS NULL
#define FCEUMM_APU_2_LABEL_CAT_CHS NULL
#define FCEUMM_APU_2_INFO_0_CHS NULL
#define FCEUMM_APU_3_LABEL_CHS NULL
#define FCEUMM_APU_3_LABEL_CAT_CHS NULL
#define FCEUMM_APU_3_INFO_0_CHS NULL
#define FCEUMM_APU_4_LABEL_CHS NULL
#define FCEUMM_APU_4_LABEL_CAT_CHS NULL
#define FCEUMM_APU_4_INFO_0_CHS NULL
#define FCEUMM_APU_5_LABEL_CHS NULL
#define FCEUMM_APU_5_LABEL_CAT_CHS NULL
#define FCEUMM_APU_5_INFO_0_CHS NULL
#define FCEUMM_TURBO_ENABLE_LABEL_CHS NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_CHS NULL
#define OPTION_VAL_NONE_CHS NULL
#define OPTION_VAL_PLAYER_1_CHS NULL
#define OPTION_VAL_PLAYER_2_CHS NULL
#define OPTION_VAL_BOTH_CHS NULL
#define FCEUMM_TURBO_DELAY_LABEL_CHS NULL
#define FCEUMM_TURBO_DELAY_INFO_0_CHS NULL
#define FCEUMM_ZAPPER_MODE_LABEL_CHS NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_CHS NULL
#define OPTION_VAL_LIGHTGUN_CHS NULL
#define OPTION_VAL_TOUCHSCREEN_CHS NULL
#define OPTION_VAL_MOUSE_CHS NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_CHS NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_CHS NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_CHS NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_CHS NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_CHS NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_CHS NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_CHS NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_CHS NULL
#define FCEUMM_OVERCLOCKING_LABEL_CHS NULL
#define FCEUMM_OVERCLOCKING_INFO_0_CHS NULL
#define OPTION_VAL_2X_POSTRENDER_CHS NULL
#define OPTION_VAL_2X_VBLANK_CHS NULL
#define FCEUMM_RAMSTATE_LABEL_CHS NULL
#define FCEUMM_RAMSTATE_INFO_0_CHS NULL
#define OPTION_VAL_FILL_FF_CHS NULL
#define OPTION_VAL_FILL_00_CHS NULL
#define OPTION_VAL_RANDOM_CHS NULL

struct retro_core_option_v2_category option_cats_chs[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_CHS,
      CATEGORY_SYSTEM_INFO_0_CHS
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_CHS,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_CHS
#else
      CATEGORY_VIDEO_INFO_1_CHS
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_CHS,
      CATEGORY_AUDIO_INFO_0_CHS
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_CHS,
      CATEGORY_INPUT_INFO_0_CHS
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_CHS,
      CATEGORY_HACKS_INFO_0_CHS
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_CHS,
      CATEGORY_DIP_SWITCH_INFO_0_CHS
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_chs[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_CHS,
      NULL,
      FCEUMM_REGION_INFO_0_CHS,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_CHS },
         { "NTSC",  OPTION_VAL_NTSC_CHS },
         { "PAL",   OPTION_VAL_PAL_CHS },
         { "Dendy", OPTION_VAL_DENDY_CHS },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_CHS,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_CHS,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CHS,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CHS,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CHS,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CHS,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_CHS,
      NULL,
      FCEUMM_ASPECT_INFO_0_CHS,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_CHS },
         { "4:3",     OPTION_VAL_4_3_CHS },
         { "PP",      OPTION_VAL_PP_CHS },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_CHS,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_CHS,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_CHS,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_CHS,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_CHS,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_CHS,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_CHS,
      NULL,
      FCEUMM_PALETTE_INFO_0_CHS,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_CHS },
         { "asqrealc",             OPTION_VAL_ASQREALC_CHS },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_CHS },
         { "rgb",                  OPTION_VAL_RGB_CHS },
         { "yuv-v3",               OPTION_VAL_YUV_V3_CHS },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_CHS },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_CHS },
         { "pal",                  OPTION_VAL_PAL_CHS },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_CHS },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_CHS },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_CHS },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_CHS },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_CHS },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_CHS },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_CHS },
         { "nescap",               OPTION_VAL_NESCAP_CHS },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_CHS },
         { "raw",                  OPTION_VAL_RAW_CHS },
         { "custom",               OPTION_VAL_CUSTOM_CHS },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_CHS,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_CHS,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_CHS },
         { "svideo",     OPTION_VAL_SVIDEO_CHS },
         { "rgb",        OPTION_VAL_RGB_O9_CHS },
         { "monochrome", OPTION_VAL_MONOCHROME_CHS },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_CHS,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_CHS },
         { "High",      OPTION_VAL_HIGH_CHS },
         { "Very High", OPTION_VAL_VERY_HIGH_CHS },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_CHS,
      FCEUMM_SWAPDUTY_LABEL_CAT_CHS,
      FCEUMM_SWAPDUTY_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_CHS,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_CHS },
         { "1",  OPTION_VAL_1_CHS },
         { "2",  OPTION_VAL_2_CHS },
         { "3",  OPTION_VAL_3_CHS },
         { "4",  OPTION_VAL_4_CHS },
         { "5",  OPTION_VAL_5_CHS },
         { "6",  OPTION_VAL_6_CHS },
         { "7",  OPTION_VAL_7_CHS },
         { "8",  OPTION_VAL_8_CHS },
         { "9",  OPTION_VAL_9_CHS },
         { "10", OPTION_VAL_10_CHS },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_CHS,
      FCEUMM_APU_1_LABEL_CAT_CHS,
      FCEUMM_APU_1_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_CHS,
      FCEUMM_APU_2_LABEL_CAT_CHS,
      FCEUMM_APU_2_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_CHS,
      FCEUMM_APU_3_LABEL_CAT_CHS,
      FCEUMM_APU_3_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_CHS,
      FCEUMM_APU_4_LABEL_CAT_CHS,
      FCEUMM_APU_4_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_CHS,
      FCEUMM_APU_5_LABEL_CAT_CHS,
      FCEUMM_APU_5_INFO_0_CHS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_CHS,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_CHS,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_CHS },
         { "Player 1", OPTION_VAL_PLAYER_1_CHS },
         { "Player 2", OPTION_VAL_PLAYER_2_CHS },
         { "Both",     OPTION_VAL_BOTH_CHS },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_CHS,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_CHS,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_CHS,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_CHS,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_CHS },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_CHS },
         { "mouse",       OPTION_VAL_MOUSE_CHS },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_CHS,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_CHS,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_CHS,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_CHS,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_CHS,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_CHS,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_CHS,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_CHS,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_CHS,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_CHS,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_CHS },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_CHS },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_CHS,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_CHS,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_CHS },
         { "fill $00", OPTION_VAL_FILL_00_CHS },
         { "random",   OPTION_VAL_RANDOM_CHS },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_chs = {
   option_cats_chs,
   option_defs_chs
};

/* RETRO_LANGUAGE_CHT */

#define CATEGORY_SYSTEM_LABEL_CHT "系統"
#define CATEGORY_SYSTEM_INFO_0_CHT NULL
#define CATEGORY_VIDEO_LABEL_CHT "視訊"
#define CATEGORY_VIDEO_INFO_0_CHT NULL
#define CATEGORY_VIDEO_INFO_1_CHT NULL
#define CATEGORY_AUDIO_LABEL_CHT "音訊"
#define CATEGORY_AUDIO_INFO_0_CHT NULL
#define CATEGORY_INPUT_LABEL_CHT "輸入"
#define CATEGORY_INPUT_INFO_0_CHT NULL
#define CATEGORY_HACKS_LABEL_CHT NULL
#define CATEGORY_HACKS_INFO_0_CHT NULL
#define CATEGORY_DIP_SWITCH_LABEL_CHT NULL
#define CATEGORY_DIP_SWITCH_INFO_0_CHT NULL
#define FCEUMM_REGION_LABEL_CHT NULL
#define FCEUMM_REGION_INFO_0_CHT NULL
#define OPTION_VAL_AUTO_CHT NULL
#define OPTION_VAL_NTSC_CHT NULL
#define OPTION_VAL_PAL_CHT NULL
#define OPTION_VAL_DENDY_CHT NULL
#define FCEUMM_GAME_GENIE_LABEL_CHT NULL
#define FCEUMM_GAME_GENIE_INFO_0_CHT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CHT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CHT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CHT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CHT NULL
#define FCEUMM_ASPECT_LABEL_CHT NULL
#define FCEUMM_ASPECT_INFO_0_CHT NULL
#define OPTION_VAL_8_7_PAR_CHT NULL
#define OPTION_VAL_4_3_CHT NULL
#define OPTION_VAL_PP_CHT NULL
#define FCEUMM_OVERSCAN_LABEL_CHT NULL
#define FCEUMM_OVERSCAN_INFO_0_CHT NULL
#define FCEUMM_OVERSCAN_H_LABEL_CHT NULL
#define FCEUMM_OVERSCAN_H_INFO_0_CHT NULL
#define FCEUMM_OVERSCAN_V_LABEL_CHT NULL
#define FCEUMM_OVERSCAN_V_INFO_0_CHT NULL
#define FCEUMM_PALETTE_LABEL_CHT NULL
#define FCEUMM_PALETTE_INFO_0_CHT NULL
#define OPTION_VAL_DEFAULT_CHT NULL
#define OPTION_VAL_ASQREALC_CHT NULL
#define OPTION_VAL_NINTENDO_VC_CHT NULL
#define OPTION_VAL_RGB_CHT NULL
#define OPTION_VAL_YUV_V3_CHT NULL
#define OPTION_VAL_UNSATURATED_FINAL_CHT NULL
#define OPTION_VAL_SONY_CXA2025AS_US_CHT NULL
#define OPTION_VAL_BMF_FINAL2_CHT NULL
#define OPTION_VAL_BMF_FINAL3_CHT NULL
#define OPTION_VAL_SMOOTH_FBX_CHT NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_CHT NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_CHT NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_CHT NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_CHT NULL
#define OPTION_VAL_NESCAP_CHT NULL
#define OPTION_VAL_WAVEBEAM_CHT NULL
#define OPTION_VAL_RAW_CHT NULL
#define OPTION_VAL_CUSTOM_CHT NULL
#define FCEUMM_NTSC_FILTER_LABEL_CHT NULL
#define FCEUMM_NTSC_FILTER_INFO_0_CHT NULL
#define OPTION_VAL_COMPOSITE_CHT NULL
#define OPTION_VAL_SVIDEO_CHT NULL
#define OPTION_VAL_RGB_O9_CHT NULL
#define OPTION_VAL_MONOCHROME_CHT NULL
#define FCEUMM_SNDQUALITY_LABEL_CHT NULL
#define FCEUMM_SNDQUALITY_INFO_0_CHT NULL
#define OPTION_VAL_LOW_CHT NULL
#define OPTION_VAL_HIGH_CHT NULL
#define OPTION_VAL_VERY_HIGH_CHT NULL
#define FCEUMM_SWAPDUTY_LABEL_CHT NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_CHT NULL
#define FCEUMM_SWAPDUTY_INFO_0_CHT NULL
#define FCEUMM_SNDVOLUME_LABEL_CHT NULL
#define FCEUMM_SNDVOLUME_INFO_0_CHT NULL
#define OPTION_VAL_0_CHT NULL
#define OPTION_VAL_1_CHT NULL
#define OPTION_VAL_2_CHT NULL
#define OPTION_VAL_3_CHT NULL
#define OPTION_VAL_4_CHT NULL
#define OPTION_VAL_5_CHT NULL
#define OPTION_VAL_6_CHT NULL
#define OPTION_VAL_7_CHT NULL
#define OPTION_VAL_8_CHT NULL
#define OPTION_VAL_9_CHT NULL
#define OPTION_VAL_10_CHT NULL
#define FCEUMM_APU_1_LABEL_CHT NULL
#define FCEUMM_APU_1_LABEL_CAT_CHT NULL
#define FCEUMM_APU_1_INFO_0_CHT NULL
#define FCEUMM_APU_2_LABEL_CHT NULL
#define FCEUMM_APU_2_LABEL_CAT_CHT NULL
#define FCEUMM_APU_2_INFO_0_CHT NULL
#define FCEUMM_APU_3_LABEL_CHT NULL
#define FCEUMM_APU_3_LABEL_CAT_CHT NULL
#define FCEUMM_APU_3_INFO_0_CHT NULL
#define FCEUMM_APU_4_LABEL_CHT NULL
#define FCEUMM_APU_4_LABEL_CAT_CHT NULL
#define FCEUMM_APU_4_INFO_0_CHT NULL
#define FCEUMM_APU_5_LABEL_CHT NULL
#define FCEUMM_APU_5_LABEL_CAT_CHT NULL
#define FCEUMM_APU_5_INFO_0_CHT NULL
#define FCEUMM_TURBO_ENABLE_LABEL_CHT NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_CHT NULL
#define OPTION_VAL_NONE_CHT NULL
#define OPTION_VAL_PLAYER_1_CHT NULL
#define OPTION_VAL_PLAYER_2_CHT NULL
#define OPTION_VAL_BOTH_CHT NULL
#define FCEUMM_TURBO_DELAY_LABEL_CHT NULL
#define FCEUMM_TURBO_DELAY_INFO_0_CHT NULL
#define FCEUMM_ZAPPER_MODE_LABEL_CHT NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_CHT NULL
#define OPTION_VAL_LIGHTGUN_CHT NULL
#define OPTION_VAL_TOUCHSCREEN_CHT NULL
#define OPTION_VAL_MOUSE_CHT NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_CHT NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_CHT NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_CHT NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_CHT NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_CHT NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_CHT NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_CHT NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_CHT NULL
#define FCEUMM_OVERCLOCKING_LABEL_CHT NULL
#define FCEUMM_OVERCLOCKING_INFO_0_CHT NULL
#define OPTION_VAL_2X_POSTRENDER_CHT NULL
#define OPTION_VAL_2X_VBLANK_CHT NULL
#define FCEUMM_RAMSTATE_LABEL_CHT NULL
#define FCEUMM_RAMSTATE_INFO_0_CHT NULL
#define OPTION_VAL_FILL_FF_CHT NULL
#define OPTION_VAL_FILL_00_CHT NULL
#define OPTION_VAL_RANDOM_CHT NULL

struct retro_core_option_v2_category option_cats_cht[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_CHT,
      CATEGORY_SYSTEM_INFO_0_CHT
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_CHT,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_CHT
#else
      CATEGORY_VIDEO_INFO_1_CHT
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_CHT,
      CATEGORY_AUDIO_INFO_0_CHT
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_CHT,
      CATEGORY_INPUT_INFO_0_CHT
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_CHT,
      CATEGORY_HACKS_INFO_0_CHT
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_CHT,
      CATEGORY_DIP_SWITCH_INFO_0_CHT
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_cht[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_CHT,
      NULL,
      FCEUMM_REGION_INFO_0_CHT,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_CHT },
         { "NTSC",  OPTION_VAL_NTSC_CHT },
         { "PAL",   OPTION_VAL_PAL_CHT },
         { "Dendy", OPTION_VAL_DENDY_CHT },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_CHT,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_CHT,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CHT,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CHT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CHT,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CHT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_CHT,
      NULL,
      FCEUMM_ASPECT_INFO_0_CHT,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_CHT },
         { "4:3",     OPTION_VAL_4_3_CHT },
         { "PP",      OPTION_VAL_PP_CHT },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_CHT,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_CHT,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_CHT,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_CHT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_CHT,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_CHT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_CHT,
      NULL,
      FCEUMM_PALETTE_INFO_0_CHT,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_CHT },
         { "asqrealc",             OPTION_VAL_ASQREALC_CHT },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_CHT },
         { "rgb",                  OPTION_VAL_RGB_CHT },
         { "yuv-v3",               OPTION_VAL_YUV_V3_CHT },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_CHT },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_CHT },
         { "pal",                  OPTION_VAL_PAL_CHT },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_CHT },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_CHT },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_CHT },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_CHT },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_CHT },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_CHT },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_CHT },
         { "nescap",               OPTION_VAL_NESCAP_CHT },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_CHT },
         { "raw",                  OPTION_VAL_RAW_CHT },
         { "custom",               OPTION_VAL_CUSTOM_CHT },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_CHT,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_CHT,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_CHT },
         { "svideo",     OPTION_VAL_SVIDEO_CHT },
         { "rgb",        OPTION_VAL_RGB_O9_CHT },
         { "monochrome", OPTION_VAL_MONOCHROME_CHT },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_CHT,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_CHT },
         { "High",      OPTION_VAL_HIGH_CHT },
         { "Very High", OPTION_VAL_VERY_HIGH_CHT },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_CHT,
      FCEUMM_SWAPDUTY_LABEL_CAT_CHT,
      FCEUMM_SWAPDUTY_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_CHT,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_CHT },
         { "1",  OPTION_VAL_1_CHT },
         { "2",  OPTION_VAL_2_CHT },
         { "3",  OPTION_VAL_3_CHT },
         { "4",  OPTION_VAL_4_CHT },
         { "5",  OPTION_VAL_5_CHT },
         { "6",  OPTION_VAL_6_CHT },
         { "7",  OPTION_VAL_7_CHT },
         { "8",  OPTION_VAL_8_CHT },
         { "9",  OPTION_VAL_9_CHT },
         { "10", OPTION_VAL_10_CHT },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_CHT,
      FCEUMM_APU_1_LABEL_CAT_CHT,
      FCEUMM_APU_1_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_CHT,
      FCEUMM_APU_2_LABEL_CAT_CHT,
      FCEUMM_APU_2_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_CHT,
      FCEUMM_APU_3_LABEL_CAT_CHT,
      FCEUMM_APU_3_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_CHT,
      FCEUMM_APU_4_LABEL_CAT_CHT,
      FCEUMM_APU_4_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_CHT,
      FCEUMM_APU_5_LABEL_CAT_CHT,
      FCEUMM_APU_5_INFO_0_CHT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_CHT,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_CHT,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_CHT },
         { "Player 1", OPTION_VAL_PLAYER_1_CHT },
         { "Player 2", OPTION_VAL_PLAYER_2_CHT },
         { "Both",     OPTION_VAL_BOTH_CHT },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_CHT,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_CHT,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_CHT,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_CHT,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_CHT },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_CHT },
         { "mouse",       OPTION_VAL_MOUSE_CHT },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_CHT,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_CHT,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_CHT,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_CHT,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_CHT,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_CHT,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_CHT,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_CHT,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_CHT,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_CHT,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_CHT },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_CHT },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_CHT,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_CHT,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_CHT },
         { "fill $00", OPTION_VAL_FILL_00_CHT },
         { "random",   OPTION_VAL_RANDOM_CHT },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_cht = {
   option_cats_cht,
   option_defs_cht
};

/* RETRO_LANGUAGE_CS */

#define CATEGORY_SYSTEM_LABEL_CS NULL
#define CATEGORY_SYSTEM_INFO_0_CS NULL
#define CATEGORY_VIDEO_LABEL_CS NULL
#define CATEGORY_VIDEO_INFO_0_CS NULL
#define CATEGORY_VIDEO_INFO_1_CS NULL
#define CATEGORY_AUDIO_LABEL_CS "Zvuk"
#define CATEGORY_AUDIO_INFO_0_CS NULL
#define CATEGORY_INPUT_LABEL_CS "Vstup"
#define CATEGORY_INPUT_INFO_0_CS NULL
#define CATEGORY_HACKS_LABEL_CS NULL
#define CATEGORY_HACKS_INFO_0_CS NULL
#define CATEGORY_DIP_SWITCH_LABEL_CS NULL
#define CATEGORY_DIP_SWITCH_INFO_0_CS NULL
#define FCEUMM_REGION_LABEL_CS NULL
#define FCEUMM_REGION_INFO_0_CS NULL
#define OPTION_VAL_AUTO_CS NULL
#define OPTION_VAL_NTSC_CS NULL
#define OPTION_VAL_PAL_CS NULL
#define OPTION_VAL_DENDY_CS NULL
#define FCEUMM_GAME_GENIE_LABEL_CS NULL
#define FCEUMM_GAME_GENIE_INFO_0_CS NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CS NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CS NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CS NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CS NULL
#define FCEUMM_ASPECT_LABEL_CS NULL
#define FCEUMM_ASPECT_INFO_0_CS NULL
#define OPTION_VAL_8_7_PAR_CS NULL
#define OPTION_VAL_4_3_CS NULL
#define OPTION_VAL_PP_CS NULL
#define FCEUMM_OVERSCAN_LABEL_CS NULL
#define FCEUMM_OVERSCAN_INFO_0_CS NULL
#define FCEUMM_OVERSCAN_H_LABEL_CS NULL
#define FCEUMM_OVERSCAN_H_INFO_0_CS NULL
#define FCEUMM_OVERSCAN_V_LABEL_CS NULL
#define FCEUMM_OVERSCAN_V_INFO_0_CS NULL
#define FCEUMM_PALETTE_LABEL_CS NULL
#define FCEUMM_PALETTE_INFO_0_CS NULL
#define OPTION_VAL_DEFAULT_CS NULL
#define OPTION_VAL_ASQREALC_CS NULL
#define OPTION_VAL_NINTENDO_VC_CS NULL
#define OPTION_VAL_RGB_CS NULL
#define OPTION_VAL_YUV_V3_CS NULL
#define OPTION_VAL_UNSATURATED_FINAL_CS NULL
#define OPTION_VAL_SONY_CXA2025AS_US_CS NULL
#define OPTION_VAL_BMF_FINAL2_CS NULL
#define OPTION_VAL_BMF_FINAL3_CS NULL
#define OPTION_VAL_SMOOTH_FBX_CS NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_CS NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_CS NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_CS NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_CS NULL
#define OPTION_VAL_NESCAP_CS NULL
#define OPTION_VAL_WAVEBEAM_CS NULL
#define OPTION_VAL_RAW_CS NULL
#define OPTION_VAL_CUSTOM_CS NULL
#define FCEUMM_NTSC_FILTER_LABEL_CS NULL
#define FCEUMM_NTSC_FILTER_INFO_0_CS NULL
#define OPTION_VAL_COMPOSITE_CS NULL
#define OPTION_VAL_SVIDEO_CS NULL
#define OPTION_VAL_RGB_O9_CS NULL
#define OPTION_VAL_MONOCHROME_CS NULL
#define FCEUMM_SNDQUALITY_LABEL_CS NULL
#define FCEUMM_SNDQUALITY_INFO_0_CS NULL
#define OPTION_VAL_LOW_CS NULL
#define OPTION_VAL_HIGH_CS NULL
#define OPTION_VAL_VERY_HIGH_CS NULL
#define FCEUMM_SWAPDUTY_LABEL_CS NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_CS NULL
#define FCEUMM_SWAPDUTY_INFO_0_CS NULL
#define FCEUMM_SNDVOLUME_LABEL_CS NULL
#define FCEUMM_SNDVOLUME_INFO_0_CS NULL
#define OPTION_VAL_0_CS NULL
#define OPTION_VAL_1_CS NULL
#define OPTION_VAL_2_CS NULL
#define OPTION_VAL_3_CS NULL
#define OPTION_VAL_4_CS NULL
#define OPTION_VAL_5_CS NULL
#define OPTION_VAL_6_CS NULL
#define OPTION_VAL_7_CS NULL
#define OPTION_VAL_8_CS NULL
#define OPTION_VAL_9_CS NULL
#define OPTION_VAL_10_CS NULL
#define FCEUMM_APU_1_LABEL_CS NULL
#define FCEUMM_APU_1_LABEL_CAT_CS NULL
#define FCEUMM_APU_1_INFO_0_CS NULL
#define FCEUMM_APU_2_LABEL_CS NULL
#define FCEUMM_APU_2_LABEL_CAT_CS NULL
#define FCEUMM_APU_2_INFO_0_CS NULL
#define FCEUMM_APU_3_LABEL_CS NULL
#define FCEUMM_APU_3_LABEL_CAT_CS NULL
#define FCEUMM_APU_3_INFO_0_CS NULL
#define FCEUMM_APU_4_LABEL_CS NULL
#define FCEUMM_APU_4_LABEL_CAT_CS NULL
#define FCEUMM_APU_4_INFO_0_CS NULL
#define FCEUMM_APU_5_LABEL_CS NULL
#define FCEUMM_APU_5_LABEL_CAT_CS NULL
#define FCEUMM_APU_5_INFO_0_CS NULL
#define FCEUMM_TURBO_ENABLE_LABEL_CS NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_CS NULL
#define OPTION_VAL_NONE_CS NULL
#define OPTION_VAL_PLAYER_1_CS NULL
#define OPTION_VAL_PLAYER_2_CS NULL
#define OPTION_VAL_BOTH_CS NULL
#define FCEUMM_TURBO_DELAY_LABEL_CS NULL
#define FCEUMM_TURBO_DELAY_INFO_0_CS NULL
#define FCEUMM_ZAPPER_MODE_LABEL_CS NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_CS NULL
#define OPTION_VAL_LIGHTGUN_CS NULL
#define OPTION_VAL_TOUCHSCREEN_CS NULL
#define OPTION_VAL_MOUSE_CS NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_CS NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_CS NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_CS NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_CS NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_CS NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_CS NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_CS NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_CS NULL
#define FCEUMM_OVERCLOCKING_LABEL_CS NULL
#define FCEUMM_OVERCLOCKING_INFO_0_CS NULL
#define OPTION_VAL_2X_POSTRENDER_CS NULL
#define OPTION_VAL_2X_VBLANK_CS NULL
#define FCEUMM_RAMSTATE_LABEL_CS NULL
#define FCEUMM_RAMSTATE_INFO_0_CS NULL
#define OPTION_VAL_FILL_FF_CS NULL
#define OPTION_VAL_FILL_00_CS NULL
#define OPTION_VAL_RANDOM_CS NULL

struct retro_core_option_v2_category option_cats_cs[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_CS,
      CATEGORY_SYSTEM_INFO_0_CS
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_CS,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_CS
#else
      CATEGORY_VIDEO_INFO_1_CS
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_CS,
      CATEGORY_AUDIO_INFO_0_CS
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_CS,
      CATEGORY_INPUT_INFO_0_CS
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_CS,
      CATEGORY_HACKS_INFO_0_CS
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_CS,
      CATEGORY_DIP_SWITCH_INFO_0_CS
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_cs[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_CS,
      NULL,
      FCEUMM_REGION_INFO_0_CS,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_CS },
         { "NTSC",  OPTION_VAL_NTSC_CS },
         { "PAL",   OPTION_VAL_PAL_CS },
         { "Dendy", OPTION_VAL_DENDY_CS },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_CS,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_CS,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CS,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CS,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CS,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CS,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_CS,
      NULL,
      FCEUMM_ASPECT_INFO_0_CS,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_CS },
         { "4:3",     OPTION_VAL_4_3_CS },
         { "PP",      OPTION_VAL_PP_CS },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_CS,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_CS,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_CS,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_CS,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_CS,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_CS,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_CS,
      NULL,
      FCEUMM_PALETTE_INFO_0_CS,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_CS },
         { "asqrealc",             OPTION_VAL_ASQREALC_CS },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_CS },
         { "rgb",                  OPTION_VAL_RGB_CS },
         { "yuv-v3",               OPTION_VAL_YUV_V3_CS },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_CS },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_CS },
         { "pal",                  OPTION_VAL_PAL_CS },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_CS },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_CS },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_CS },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_CS },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_CS },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_CS },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_CS },
         { "nescap",               OPTION_VAL_NESCAP_CS },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_CS },
         { "raw",                  OPTION_VAL_RAW_CS },
         { "custom",               OPTION_VAL_CUSTOM_CS },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_CS,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_CS,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_CS },
         { "svideo",     OPTION_VAL_SVIDEO_CS },
         { "rgb",        OPTION_VAL_RGB_O9_CS },
         { "monochrome", OPTION_VAL_MONOCHROME_CS },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_CS,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_CS,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_CS },
         { "High",      OPTION_VAL_HIGH_CS },
         { "Very High", OPTION_VAL_VERY_HIGH_CS },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_CS,
      FCEUMM_SWAPDUTY_LABEL_CAT_CS,
      FCEUMM_SWAPDUTY_INFO_0_CS,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_CS,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_CS,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_CS },
         { "1",  OPTION_VAL_1_CS },
         { "2",  OPTION_VAL_2_CS },
         { "3",  OPTION_VAL_3_CS },
         { "4",  OPTION_VAL_4_CS },
         { "5",  OPTION_VAL_5_CS },
         { "6",  OPTION_VAL_6_CS },
         { "7",  OPTION_VAL_7_CS },
         { "8",  OPTION_VAL_8_CS },
         { "9",  OPTION_VAL_9_CS },
         { "10", OPTION_VAL_10_CS },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_CS,
      FCEUMM_APU_1_LABEL_CAT_CS,
      FCEUMM_APU_1_INFO_0_CS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_CS,
      FCEUMM_APU_2_LABEL_CAT_CS,
      FCEUMM_APU_2_INFO_0_CS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_CS,
      FCEUMM_APU_3_LABEL_CAT_CS,
      FCEUMM_APU_3_INFO_0_CS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_CS,
      FCEUMM_APU_4_LABEL_CAT_CS,
      FCEUMM_APU_4_INFO_0_CS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_CS,
      FCEUMM_APU_5_LABEL_CAT_CS,
      FCEUMM_APU_5_INFO_0_CS,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_CS,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_CS,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_CS },
         { "Player 1", OPTION_VAL_PLAYER_1_CS },
         { "Player 2", OPTION_VAL_PLAYER_2_CS },
         { "Both",     OPTION_VAL_BOTH_CS },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_CS,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_CS,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_CS,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_CS,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_CS },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_CS },
         { "mouse",       OPTION_VAL_MOUSE_CS },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_CS,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_CS,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_CS,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_CS,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_CS,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_CS,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_CS,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_CS,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_CS,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_CS,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_CS },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_CS },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_CS,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_CS,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_CS },
         { "fill $00", OPTION_VAL_FILL_00_CS },
         { "random",   OPTION_VAL_RANDOM_CS },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_cs = {
   option_cats_cs,
   option_defs_cs
};

/* RETRO_LANGUAGE_CY */

#define CATEGORY_SYSTEM_LABEL_CY NULL
#define CATEGORY_SYSTEM_INFO_0_CY NULL
#define CATEGORY_VIDEO_LABEL_CY NULL
#define CATEGORY_VIDEO_INFO_0_CY NULL
#define CATEGORY_VIDEO_INFO_1_CY NULL
#define CATEGORY_AUDIO_LABEL_CY NULL
#define CATEGORY_AUDIO_INFO_0_CY NULL
#define CATEGORY_INPUT_LABEL_CY "Mewnbwn"
#define CATEGORY_INPUT_INFO_0_CY NULL
#define CATEGORY_HACKS_LABEL_CY NULL
#define CATEGORY_HACKS_INFO_0_CY NULL
#define CATEGORY_DIP_SWITCH_LABEL_CY NULL
#define CATEGORY_DIP_SWITCH_INFO_0_CY NULL
#define FCEUMM_REGION_LABEL_CY NULL
#define FCEUMM_REGION_INFO_0_CY NULL
#define OPTION_VAL_AUTO_CY NULL
#define OPTION_VAL_NTSC_CY NULL
#define OPTION_VAL_PAL_CY NULL
#define OPTION_VAL_DENDY_CY NULL
#define FCEUMM_GAME_GENIE_LABEL_CY NULL
#define FCEUMM_GAME_GENIE_INFO_0_CY NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CY NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CY NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CY NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CY NULL
#define FCEUMM_ASPECT_LABEL_CY NULL
#define FCEUMM_ASPECT_INFO_0_CY NULL
#define OPTION_VAL_8_7_PAR_CY NULL
#define OPTION_VAL_4_3_CY NULL
#define OPTION_VAL_PP_CY NULL
#define FCEUMM_OVERSCAN_LABEL_CY NULL
#define FCEUMM_OVERSCAN_INFO_0_CY NULL
#define FCEUMM_OVERSCAN_H_LABEL_CY NULL
#define FCEUMM_OVERSCAN_H_INFO_0_CY NULL
#define FCEUMM_OVERSCAN_V_LABEL_CY NULL
#define FCEUMM_OVERSCAN_V_INFO_0_CY NULL
#define FCEUMM_PALETTE_LABEL_CY NULL
#define FCEUMM_PALETTE_INFO_0_CY NULL
#define OPTION_VAL_DEFAULT_CY NULL
#define OPTION_VAL_ASQREALC_CY NULL
#define OPTION_VAL_NINTENDO_VC_CY NULL
#define OPTION_VAL_RGB_CY NULL
#define OPTION_VAL_YUV_V3_CY NULL
#define OPTION_VAL_UNSATURATED_FINAL_CY NULL
#define OPTION_VAL_SONY_CXA2025AS_US_CY NULL
#define OPTION_VAL_BMF_FINAL2_CY NULL
#define OPTION_VAL_BMF_FINAL3_CY NULL
#define OPTION_VAL_SMOOTH_FBX_CY NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_CY NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_CY NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_CY NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_CY NULL
#define OPTION_VAL_NESCAP_CY NULL
#define OPTION_VAL_WAVEBEAM_CY NULL
#define OPTION_VAL_RAW_CY NULL
#define OPTION_VAL_CUSTOM_CY NULL
#define FCEUMM_NTSC_FILTER_LABEL_CY NULL
#define FCEUMM_NTSC_FILTER_INFO_0_CY NULL
#define OPTION_VAL_COMPOSITE_CY NULL
#define OPTION_VAL_SVIDEO_CY NULL
#define OPTION_VAL_RGB_O9_CY NULL
#define OPTION_VAL_MONOCHROME_CY NULL
#define FCEUMM_SNDQUALITY_LABEL_CY NULL
#define FCEUMM_SNDQUALITY_INFO_0_CY NULL
#define OPTION_VAL_LOW_CY NULL
#define OPTION_VAL_HIGH_CY NULL
#define OPTION_VAL_VERY_HIGH_CY NULL
#define FCEUMM_SWAPDUTY_LABEL_CY NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_CY NULL
#define FCEUMM_SWAPDUTY_INFO_0_CY NULL
#define FCEUMM_SNDVOLUME_LABEL_CY NULL
#define FCEUMM_SNDVOLUME_INFO_0_CY NULL
#define OPTION_VAL_0_CY NULL
#define OPTION_VAL_1_CY NULL
#define OPTION_VAL_2_CY NULL
#define OPTION_VAL_3_CY NULL
#define OPTION_VAL_4_CY NULL
#define OPTION_VAL_5_CY NULL
#define OPTION_VAL_6_CY NULL
#define OPTION_VAL_7_CY NULL
#define OPTION_VAL_8_CY NULL
#define OPTION_VAL_9_CY NULL
#define OPTION_VAL_10_CY NULL
#define FCEUMM_APU_1_LABEL_CY NULL
#define FCEUMM_APU_1_LABEL_CAT_CY NULL
#define FCEUMM_APU_1_INFO_0_CY NULL
#define FCEUMM_APU_2_LABEL_CY NULL
#define FCEUMM_APU_2_LABEL_CAT_CY NULL
#define FCEUMM_APU_2_INFO_0_CY NULL
#define FCEUMM_APU_3_LABEL_CY NULL
#define FCEUMM_APU_3_LABEL_CAT_CY NULL
#define FCEUMM_APU_3_INFO_0_CY NULL
#define FCEUMM_APU_4_LABEL_CY NULL
#define FCEUMM_APU_4_LABEL_CAT_CY NULL
#define FCEUMM_APU_4_INFO_0_CY NULL
#define FCEUMM_APU_5_LABEL_CY NULL
#define FCEUMM_APU_5_LABEL_CAT_CY NULL
#define FCEUMM_APU_5_INFO_0_CY NULL
#define FCEUMM_TURBO_ENABLE_LABEL_CY NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_CY NULL
#define OPTION_VAL_NONE_CY NULL
#define OPTION_VAL_PLAYER_1_CY NULL
#define OPTION_VAL_PLAYER_2_CY NULL
#define OPTION_VAL_BOTH_CY NULL
#define FCEUMM_TURBO_DELAY_LABEL_CY NULL
#define FCEUMM_TURBO_DELAY_INFO_0_CY NULL
#define FCEUMM_ZAPPER_MODE_LABEL_CY NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_CY NULL
#define OPTION_VAL_LIGHTGUN_CY NULL
#define OPTION_VAL_TOUCHSCREEN_CY NULL
#define OPTION_VAL_MOUSE_CY NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_CY NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_CY NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_CY NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_CY NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_CY NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_CY NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_CY NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_CY NULL
#define FCEUMM_OVERCLOCKING_LABEL_CY NULL
#define FCEUMM_OVERCLOCKING_INFO_0_CY NULL
#define OPTION_VAL_2X_POSTRENDER_CY NULL
#define OPTION_VAL_2X_VBLANK_CY NULL
#define FCEUMM_RAMSTATE_LABEL_CY NULL
#define FCEUMM_RAMSTATE_INFO_0_CY NULL
#define OPTION_VAL_FILL_FF_CY NULL
#define OPTION_VAL_FILL_00_CY NULL
#define OPTION_VAL_RANDOM_CY NULL

struct retro_core_option_v2_category option_cats_cy[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_CY,
      CATEGORY_SYSTEM_INFO_0_CY
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_CY,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_CY
#else
      CATEGORY_VIDEO_INFO_1_CY
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_CY,
      CATEGORY_AUDIO_INFO_0_CY
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_CY,
      CATEGORY_INPUT_INFO_0_CY
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_CY,
      CATEGORY_HACKS_INFO_0_CY
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_CY,
      CATEGORY_DIP_SWITCH_INFO_0_CY
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_cy[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_CY,
      NULL,
      FCEUMM_REGION_INFO_0_CY,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_CY },
         { "NTSC",  OPTION_VAL_NTSC_CY },
         { "PAL",   OPTION_VAL_PAL_CY },
         { "Dendy", OPTION_VAL_DENDY_CY },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_CY,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_CY,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_CY,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_CY,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_CY,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_CY,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_CY,
      NULL,
      FCEUMM_ASPECT_INFO_0_CY,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_CY },
         { "4:3",     OPTION_VAL_4_3_CY },
         { "PP",      OPTION_VAL_PP_CY },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_CY,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_CY,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_CY,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_CY,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_CY,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_CY,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_CY,
      NULL,
      FCEUMM_PALETTE_INFO_0_CY,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_CY },
         { "asqrealc",             OPTION_VAL_ASQREALC_CY },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_CY },
         { "rgb",                  OPTION_VAL_RGB_CY },
         { "yuv-v3",               OPTION_VAL_YUV_V3_CY },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_CY },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_CY },
         { "pal",                  OPTION_VAL_PAL_CY },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_CY },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_CY },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_CY },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_CY },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_CY },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_CY },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_CY },
         { "nescap",               OPTION_VAL_NESCAP_CY },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_CY },
         { "raw",                  OPTION_VAL_RAW_CY },
         { "custom",               OPTION_VAL_CUSTOM_CY },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_CY,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_CY,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_CY },
         { "svideo",     OPTION_VAL_SVIDEO_CY },
         { "rgb",        OPTION_VAL_RGB_O9_CY },
         { "monochrome", OPTION_VAL_MONOCHROME_CY },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_CY,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_CY,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_CY },
         { "High",      OPTION_VAL_HIGH_CY },
         { "Very High", OPTION_VAL_VERY_HIGH_CY },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_CY,
      FCEUMM_SWAPDUTY_LABEL_CAT_CY,
      FCEUMM_SWAPDUTY_INFO_0_CY,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_CY,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_CY,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_CY },
         { "1",  OPTION_VAL_1_CY },
         { "2",  OPTION_VAL_2_CY },
         { "3",  OPTION_VAL_3_CY },
         { "4",  OPTION_VAL_4_CY },
         { "5",  OPTION_VAL_5_CY },
         { "6",  OPTION_VAL_6_CY },
         { "7",  OPTION_VAL_7_CY },
         { "8",  OPTION_VAL_8_CY },
         { "9",  OPTION_VAL_9_CY },
         { "10", OPTION_VAL_10_CY },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_CY,
      FCEUMM_APU_1_LABEL_CAT_CY,
      FCEUMM_APU_1_INFO_0_CY,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_CY,
      FCEUMM_APU_2_LABEL_CAT_CY,
      FCEUMM_APU_2_INFO_0_CY,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_CY,
      FCEUMM_APU_3_LABEL_CAT_CY,
      FCEUMM_APU_3_INFO_0_CY,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_CY,
      FCEUMM_APU_4_LABEL_CAT_CY,
      FCEUMM_APU_4_INFO_0_CY,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_CY,
      FCEUMM_APU_5_LABEL_CAT_CY,
      FCEUMM_APU_5_INFO_0_CY,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_CY,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_CY,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_CY },
         { "Player 1", OPTION_VAL_PLAYER_1_CY },
         { "Player 2", OPTION_VAL_PLAYER_2_CY },
         { "Both",     OPTION_VAL_BOTH_CY },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_CY,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_CY,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_CY,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_CY,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_CY },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_CY },
         { "mouse",       OPTION_VAL_MOUSE_CY },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_CY,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_CY,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_CY,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_CY,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_CY,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_CY,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_CY,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_CY,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_CY,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_CY,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_CY },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_CY },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_CY,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_CY,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_CY },
         { "fill $00", OPTION_VAL_FILL_00_CY },
         { "random",   OPTION_VAL_RANDOM_CY },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_cy = {
   option_cats_cy,
   option_defs_cy
};

/* RETRO_LANGUAGE_DA */

#define CATEGORY_SYSTEM_LABEL_DA NULL
#define CATEGORY_SYSTEM_INFO_0_DA NULL
#define CATEGORY_VIDEO_LABEL_DA NULL
#define CATEGORY_VIDEO_INFO_0_DA NULL
#define CATEGORY_VIDEO_INFO_1_DA NULL
#define CATEGORY_AUDIO_LABEL_DA "Lyd"
#define CATEGORY_AUDIO_INFO_0_DA NULL
#define CATEGORY_INPUT_LABEL_DA NULL
#define CATEGORY_INPUT_INFO_0_DA NULL
#define CATEGORY_HACKS_LABEL_DA NULL
#define CATEGORY_HACKS_INFO_0_DA NULL
#define CATEGORY_DIP_SWITCH_LABEL_DA NULL
#define CATEGORY_DIP_SWITCH_INFO_0_DA NULL
#define FCEUMM_REGION_LABEL_DA NULL
#define FCEUMM_REGION_INFO_0_DA NULL
#define OPTION_VAL_AUTO_DA NULL
#define OPTION_VAL_NTSC_DA NULL
#define OPTION_VAL_PAL_DA NULL
#define OPTION_VAL_DENDY_DA NULL
#define FCEUMM_GAME_GENIE_LABEL_DA NULL
#define FCEUMM_GAME_GENIE_INFO_0_DA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_DA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_DA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_DA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_DA NULL
#define FCEUMM_ASPECT_LABEL_DA NULL
#define FCEUMM_ASPECT_INFO_0_DA NULL
#define OPTION_VAL_8_7_PAR_DA NULL
#define OPTION_VAL_4_3_DA NULL
#define OPTION_VAL_PP_DA NULL
#define FCEUMM_OVERSCAN_LABEL_DA NULL
#define FCEUMM_OVERSCAN_INFO_0_DA NULL
#define FCEUMM_OVERSCAN_H_LABEL_DA NULL
#define FCEUMM_OVERSCAN_H_INFO_0_DA NULL
#define FCEUMM_OVERSCAN_V_LABEL_DA NULL
#define FCEUMM_OVERSCAN_V_INFO_0_DA NULL
#define FCEUMM_PALETTE_LABEL_DA NULL
#define FCEUMM_PALETTE_INFO_0_DA NULL
#define OPTION_VAL_DEFAULT_DA NULL
#define OPTION_VAL_ASQREALC_DA NULL
#define OPTION_VAL_NINTENDO_VC_DA NULL
#define OPTION_VAL_RGB_DA NULL
#define OPTION_VAL_YUV_V3_DA NULL
#define OPTION_VAL_UNSATURATED_FINAL_DA NULL
#define OPTION_VAL_SONY_CXA2025AS_US_DA NULL
#define OPTION_VAL_BMF_FINAL2_DA NULL
#define OPTION_VAL_BMF_FINAL3_DA NULL
#define OPTION_VAL_SMOOTH_FBX_DA NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_DA NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_DA NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_DA NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_DA NULL
#define OPTION_VAL_NESCAP_DA NULL
#define OPTION_VAL_WAVEBEAM_DA NULL
#define OPTION_VAL_RAW_DA NULL
#define OPTION_VAL_CUSTOM_DA NULL
#define FCEUMM_NTSC_FILTER_LABEL_DA NULL
#define FCEUMM_NTSC_FILTER_INFO_0_DA NULL
#define OPTION_VAL_COMPOSITE_DA NULL
#define OPTION_VAL_SVIDEO_DA NULL
#define OPTION_VAL_RGB_O9_DA NULL
#define OPTION_VAL_MONOCHROME_DA NULL
#define FCEUMM_SNDQUALITY_LABEL_DA NULL
#define FCEUMM_SNDQUALITY_INFO_0_DA NULL
#define OPTION_VAL_LOW_DA NULL
#define OPTION_VAL_HIGH_DA NULL
#define OPTION_VAL_VERY_HIGH_DA NULL
#define FCEUMM_SWAPDUTY_LABEL_DA NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_DA NULL
#define FCEUMM_SWAPDUTY_INFO_0_DA NULL
#define FCEUMM_SNDVOLUME_LABEL_DA NULL
#define FCEUMM_SNDVOLUME_INFO_0_DA NULL
#define OPTION_VAL_0_DA NULL
#define OPTION_VAL_1_DA NULL
#define OPTION_VAL_2_DA NULL
#define OPTION_VAL_3_DA NULL
#define OPTION_VAL_4_DA NULL
#define OPTION_VAL_5_DA NULL
#define OPTION_VAL_6_DA NULL
#define OPTION_VAL_7_DA NULL
#define OPTION_VAL_8_DA NULL
#define OPTION_VAL_9_DA NULL
#define OPTION_VAL_10_DA NULL
#define FCEUMM_APU_1_LABEL_DA NULL
#define FCEUMM_APU_1_LABEL_CAT_DA NULL
#define FCEUMM_APU_1_INFO_0_DA NULL
#define FCEUMM_APU_2_LABEL_DA NULL
#define FCEUMM_APU_2_LABEL_CAT_DA NULL
#define FCEUMM_APU_2_INFO_0_DA NULL
#define FCEUMM_APU_3_LABEL_DA NULL
#define FCEUMM_APU_3_LABEL_CAT_DA NULL
#define FCEUMM_APU_3_INFO_0_DA NULL
#define FCEUMM_APU_4_LABEL_DA NULL
#define FCEUMM_APU_4_LABEL_CAT_DA NULL
#define FCEUMM_APU_4_INFO_0_DA NULL
#define FCEUMM_APU_5_LABEL_DA NULL
#define FCEUMM_APU_5_LABEL_CAT_DA NULL
#define FCEUMM_APU_5_INFO_0_DA NULL
#define FCEUMM_TURBO_ENABLE_LABEL_DA NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_DA NULL
#define OPTION_VAL_NONE_DA NULL
#define OPTION_VAL_PLAYER_1_DA NULL
#define OPTION_VAL_PLAYER_2_DA NULL
#define OPTION_VAL_BOTH_DA NULL
#define FCEUMM_TURBO_DELAY_LABEL_DA NULL
#define FCEUMM_TURBO_DELAY_INFO_0_DA NULL
#define FCEUMM_ZAPPER_MODE_LABEL_DA NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_DA NULL
#define OPTION_VAL_LIGHTGUN_DA NULL
#define OPTION_VAL_TOUCHSCREEN_DA NULL
#define OPTION_VAL_MOUSE_DA NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_DA NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_DA NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_DA NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_DA NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_DA NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_DA NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_DA NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_DA NULL
#define FCEUMM_OVERCLOCKING_LABEL_DA NULL
#define FCEUMM_OVERCLOCKING_INFO_0_DA NULL
#define OPTION_VAL_2X_POSTRENDER_DA NULL
#define OPTION_VAL_2X_VBLANK_DA NULL
#define FCEUMM_RAMSTATE_LABEL_DA NULL
#define FCEUMM_RAMSTATE_INFO_0_DA NULL
#define OPTION_VAL_FILL_FF_DA NULL
#define OPTION_VAL_FILL_00_DA NULL
#define OPTION_VAL_RANDOM_DA NULL

struct retro_core_option_v2_category option_cats_da[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_DA,
      CATEGORY_SYSTEM_INFO_0_DA
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_DA,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_DA
#else
      CATEGORY_VIDEO_INFO_1_DA
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_DA,
      CATEGORY_AUDIO_INFO_0_DA
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_DA,
      CATEGORY_INPUT_INFO_0_DA
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_DA,
      CATEGORY_HACKS_INFO_0_DA
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_DA,
      CATEGORY_DIP_SWITCH_INFO_0_DA
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_da[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_DA,
      NULL,
      FCEUMM_REGION_INFO_0_DA,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_DA },
         { "NTSC",  OPTION_VAL_NTSC_DA },
         { "PAL",   OPTION_VAL_PAL_DA },
         { "Dendy", OPTION_VAL_DENDY_DA },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_DA,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_DA,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_DA,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_DA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_DA,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_DA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_DA,
      NULL,
      FCEUMM_ASPECT_INFO_0_DA,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_DA },
         { "4:3",     OPTION_VAL_4_3_DA },
         { "PP",      OPTION_VAL_PP_DA },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_DA,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_DA,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_DA,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_DA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_DA,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_DA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_DA,
      NULL,
      FCEUMM_PALETTE_INFO_0_DA,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_DA },
         { "asqrealc",             OPTION_VAL_ASQREALC_DA },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_DA },
         { "rgb",                  OPTION_VAL_RGB_DA },
         { "yuv-v3",               OPTION_VAL_YUV_V3_DA },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_DA },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_DA },
         { "pal",                  OPTION_VAL_PAL_DA },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_DA },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_DA },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_DA },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_DA },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_DA },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_DA },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_DA },
         { "nescap",               OPTION_VAL_NESCAP_DA },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_DA },
         { "raw",                  OPTION_VAL_RAW_DA },
         { "custom",               OPTION_VAL_CUSTOM_DA },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_DA,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_DA,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_DA },
         { "svideo",     OPTION_VAL_SVIDEO_DA },
         { "rgb",        OPTION_VAL_RGB_O9_DA },
         { "monochrome", OPTION_VAL_MONOCHROME_DA },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_DA,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_DA,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_DA },
         { "High",      OPTION_VAL_HIGH_DA },
         { "Very High", OPTION_VAL_VERY_HIGH_DA },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_DA,
      FCEUMM_SWAPDUTY_LABEL_CAT_DA,
      FCEUMM_SWAPDUTY_INFO_0_DA,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_DA,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_DA,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_DA },
         { "1",  OPTION_VAL_1_DA },
         { "2",  OPTION_VAL_2_DA },
         { "3",  OPTION_VAL_3_DA },
         { "4",  OPTION_VAL_4_DA },
         { "5",  OPTION_VAL_5_DA },
         { "6",  OPTION_VAL_6_DA },
         { "7",  OPTION_VAL_7_DA },
         { "8",  OPTION_VAL_8_DA },
         { "9",  OPTION_VAL_9_DA },
         { "10", OPTION_VAL_10_DA },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_DA,
      FCEUMM_APU_1_LABEL_CAT_DA,
      FCEUMM_APU_1_INFO_0_DA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_DA,
      FCEUMM_APU_2_LABEL_CAT_DA,
      FCEUMM_APU_2_INFO_0_DA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_DA,
      FCEUMM_APU_3_LABEL_CAT_DA,
      FCEUMM_APU_3_INFO_0_DA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_DA,
      FCEUMM_APU_4_LABEL_CAT_DA,
      FCEUMM_APU_4_INFO_0_DA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_DA,
      FCEUMM_APU_5_LABEL_CAT_DA,
      FCEUMM_APU_5_INFO_0_DA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_DA,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_DA,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_DA },
         { "Player 1", OPTION_VAL_PLAYER_1_DA },
         { "Player 2", OPTION_VAL_PLAYER_2_DA },
         { "Both",     OPTION_VAL_BOTH_DA },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_DA,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_DA,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_DA,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_DA,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_DA },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_DA },
         { "mouse",       OPTION_VAL_MOUSE_DA },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_DA,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_DA,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_DA,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_DA,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_DA,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_DA,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_DA,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_DA,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_DA,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_DA,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_DA },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_DA },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_DA,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_DA,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_DA },
         { "fill $00", OPTION_VAL_FILL_00_DA },
         { "random",   OPTION_VAL_RANDOM_DA },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_da = {
   option_cats_da,
   option_defs_da
};

/* RETRO_LANGUAGE_DE */

#define CATEGORY_SYSTEM_LABEL_DE NULL
#define CATEGORY_SYSTEM_INFO_0_DE NULL
#define CATEGORY_VIDEO_LABEL_DE NULL
#define CATEGORY_VIDEO_INFO_0_DE NULL
#define CATEGORY_VIDEO_INFO_1_DE NULL
#define CATEGORY_AUDIO_LABEL_DE NULL
#define CATEGORY_AUDIO_INFO_0_DE NULL
#define CATEGORY_INPUT_LABEL_DE NULL
#define CATEGORY_INPUT_INFO_0_DE NULL
#define CATEGORY_HACKS_LABEL_DE "Emulations-Hacks"
#define CATEGORY_HACKS_INFO_0_DE "Parameter für Prozessorübertaktung und Emulationsgenauigkeit, die sich auf die Low-Level-Leistung und Kompatibilität auswirken, anpassen."
#define CATEGORY_DIP_SWITCH_LABEL_DE NULL
#define CATEGORY_DIP_SWITCH_INFO_0_DE NULL
#define FCEUMM_REGION_LABEL_DE NULL
#define FCEUMM_REGION_INFO_0_DE NULL
#define OPTION_VAL_AUTO_DE NULL
#define OPTION_VAL_NTSC_DE NULL
#define OPTION_VAL_PAL_DE NULL
#define OPTION_VAL_DENDY_DE NULL
#define FCEUMM_GAME_GENIE_LABEL_DE NULL
#define FCEUMM_GAME_GENIE_INFO_0_DE NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_DE NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_DE NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_DE NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_DE NULL
#define FCEUMM_ASPECT_LABEL_DE NULL
#define FCEUMM_ASPECT_INFO_0_DE NULL
#define OPTION_VAL_8_7_PAR_DE NULL
#define OPTION_VAL_4_3_DE NULL
#define OPTION_VAL_PP_DE NULL
#define FCEUMM_OVERSCAN_LABEL_DE NULL
#define FCEUMM_OVERSCAN_INFO_0_DE NULL
#define FCEUMM_OVERSCAN_H_LABEL_DE NULL
#define FCEUMM_OVERSCAN_H_INFO_0_DE NULL
#define FCEUMM_OVERSCAN_V_LABEL_DE NULL
#define FCEUMM_OVERSCAN_V_INFO_0_DE NULL
#define FCEUMM_PALETTE_LABEL_DE NULL
#define FCEUMM_PALETTE_INFO_0_DE NULL
#define OPTION_VAL_DEFAULT_DE NULL
#define OPTION_VAL_ASQREALC_DE NULL
#define OPTION_VAL_NINTENDO_VC_DE NULL
#define OPTION_VAL_RGB_DE NULL
#define OPTION_VAL_YUV_V3_DE NULL
#define OPTION_VAL_UNSATURATED_FINAL_DE NULL
#define OPTION_VAL_SONY_CXA2025AS_US_DE NULL
#define OPTION_VAL_BMF_FINAL2_DE NULL
#define OPTION_VAL_BMF_FINAL3_DE NULL
#define OPTION_VAL_SMOOTH_FBX_DE NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_DE NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_DE NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_DE NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_DE NULL
#define OPTION_VAL_NESCAP_DE NULL
#define OPTION_VAL_WAVEBEAM_DE NULL
#define OPTION_VAL_RAW_DE NULL
#define OPTION_VAL_CUSTOM_DE NULL
#define FCEUMM_NTSC_FILTER_LABEL_DE NULL
#define FCEUMM_NTSC_FILTER_INFO_0_DE NULL
#define OPTION_VAL_COMPOSITE_DE NULL
#define OPTION_VAL_SVIDEO_DE NULL
#define OPTION_VAL_RGB_O9_DE NULL
#define OPTION_VAL_MONOCHROME_DE NULL
#define FCEUMM_SNDQUALITY_LABEL_DE NULL
#define FCEUMM_SNDQUALITY_INFO_0_DE NULL
#define OPTION_VAL_LOW_DE NULL
#define OPTION_VAL_HIGH_DE NULL
#define OPTION_VAL_VERY_HIGH_DE NULL
#define FCEUMM_SWAPDUTY_LABEL_DE NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_DE NULL
#define FCEUMM_SWAPDUTY_INFO_0_DE NULL
#define FCEUMM_SNDVOLUME_LABEL_DE NULL
#define FCEUMM_SNDVOLUME_INFO_0_DE NULL
#define OPTION_VAL_0_DE NULL
#define OPTION_VAL_1_DE NULL
#define OPTION_VAL_2_DE NULL
#define OPTION_VAL_3_DE NULL
#define OPTION_VAL_4_DE NULL
#define OPTION_VAL_5_DE NULL
#define OPTION_VAL_6_DE NULL
#define OPTION_VAL_7_DE NULL
#define OPTION_VAL_8_DE NULL
#define OPTION_VAL_9_DE NULL
#define OPTION_VAL_10_DE NULL
#define FCEUMM_APU_1_LABEL_DE NULL
#define FCEUMM_APU_1_LABEL_CAT_DE NULL
#define FCEUMM_APU_1_INFO_0_DE NULL
#define FCEUMM_APU_2_LABEL_DE NULL
#define FCEUMM_APU_2_LABEL_CAT_DE NULL
#define FCEUMM_APU_2_INFO_0_DE NULL
#define FCEUMM_APU_3_LABEL_DE NULL
#define FCEUMM_APU_3_LABEL_CAT_DE NULL
#define FCEUMM_APU_3_INFO_0_DE NULL
#define FCEUMM_APU_4_LABEL_DE NULL
#define FCEUMM_APU_4_LABEL_CAT_DE NULL
#define FCEUMM_APU_4_INFO_0_DE NULL
#define FCEUMM_APU_5_LABEL_DE NULL
#define FCEUMM_APU_5_LABEL_CAT_DE NULL
#define FCEUMM_APU_5_INFO_0_DE NULL
#define FCEUMM_TURBO_ENABLE_LABEL_DE NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_DE NULL
#define OPTION_VAL_NONE_DE NULL
#define OPTION_VAL_PLAYER_1_DE NULL
#define OPTION_VAL_PLAYER_2_DE NULL
#define OPTION_VAL_BOTH_DE NULL
#define FCEUMM_TURBO_DELAY_LABEL_DE NULL
#define FCEUMM_TURBO_DELAY_INFO_0_DE NULL
#define FCEUMM_ZAPPER_MODE_LABEL_DE NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_DE NULL
#define OPTION_VAL_LIGHTGUN_DE NULL
#define OPTION_VAL_TOUCHSCREEN_DE NULL
#define OPTION_VAL_MOUSE_DE NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_DE NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_DE NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_DE NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_DE NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_DE NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_DE NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_DE NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_DE NULL
#define FCEUMM_OVERCLOCKING_LABEL_DE NULL
#define FCEUMM_OVERCLOCKING_INFO_0_DE NULL
#define OPTION_VAL_2X_POSTRENDER_DE NULL
#define OPTION_VAL_2X_VBLANK_DE NULL
#define FCEUMM_RAMSTATE_LABEL_DE NULL
#define FCEUMM_RAMSTATE_INFO_0_DE NULL
#define OPTION_VAL_FILL_FF_DE NULL
#define OPTION_VAL_FILL_00_DE NULL
#define OPTION_VAL_RANDOM_DE NULL

struct retro_core_option_v2_category option_cats_de[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_DE,
      CATEGORY_SYSTEM_INFO_0_DE
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_DE,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_DE
#else
      CATEGORY_VIDEO_INFO_1_DE
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_DE,
      CATEGORY_AUDIO_INFO_0_DE
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_DE,
      CATEGORY_INPUT_INFO_0_DE
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_DE,
      CATEGORY_HACKS_INFO_0_DE
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_DE,
      CATEGORY_DIP_SWITCH_INFO_0_DE
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_de[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_DE,
      NULL,
      FCEUMM_REGION_INFO_0_DE,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_DE },
         { "NTSC",  OPTION_VAL_NTSC_DE },
         { "PAL",   OPTION_VAL_PAL_DE },
         { "Dendy", OPTION_VAL_DENDY_DE },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_DE,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_DE,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_DE,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_DE,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_DE,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_DE,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_DE,
      NULL,
      FCEUMM_ASPECT_INFO_0_DE,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_DE },
         { "4:3",     OPTION_VAL_4_3_DE },
         { "PP",      OPTION_VAL_PP_DE },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_DE,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_DE,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_DE,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_DE,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_DE,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_DE,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_DE,
      NULL,
      FCEUMM_PALETTE_INFO_0_DE,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_DE },
         { "asqrealc",             OPTION_VAL_ASQREALC_DE },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_DE },
         { "rgb",                  OPTION_VAL_RGB_DE },
         { "yuv-v3",               OPTION_VAL_YUV_V3_DE },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_DE },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_DE },
         { "pal",                  OPTION_VAL_PAL_DE },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_DE },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_DE },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_DE },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_DE },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_DE },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_DE },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_DE },
         { "nescap",               OPTION_VAL_NESCAP_DE },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_DE },
         { "raw",                  OPTION_VAL_RAW_DE },
         { "custom",               OPTION_VAL_CUSTOM_DE },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_DE,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_DE,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_DE },
         { "svideo",     OPTION_VAL_SVIDEO_DE },
         { "rgb",        OPTION_VAL_RGB_O9_DE },
         { "monochrome", OPTION_VAL_MONOCHROME_DE },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_DE,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_DE,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_DE },
         { "High",      OPTION_VAL_HIGH_DE },
         { "Very High", OPTION_VAL_VERY_HIGH_DE },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_DE,
      FCEUMM_SWAPDUTY_LABEL_CAT_DE,
      FCEUMM_SWAPDUTY_INFO_0_DE,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_DE,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_DE,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_DE },
         { "1",  OPTION_VAL_1_DE },
         { "2",  OPTION_VAL_2_DE },
         { "3",  OPTION_VAL_3_DE },
         { "4",  OPTION_VAL_4_DE },
         { "5",  OPTION_VAL_5_DE },
         { "6",  OPTION_VAL_6_DE },
         { "7",  OPTION_VAL_7_DE },
         { "8",  OPTION_VAL_8_DE },
         { "9",  OPTION_VAL_9_DE },
         { "10", OPTION_VAL_10_DE },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_DE,
      FCEUMM_APU_1_LABEL_CAT_DE,
      FCEUMM_APU_1_INFO_0_DE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_DE,
      FCEUMM_APU_2_LABEL_CAT_DE,
      FCEUMM_APU_2_INFO_0_DE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_DE,
      FCEUMM_APU_3_LABEL_CAT_DE,
      FCEUMM_APU_3_INFO_0_DE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_DE,
      FCEUMM_APU_4_LABEL_CAT_DE,
      FCEUMM_APU_4_INFO_0_DE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_DE,
      FCEUMM_APU_5_LABEL_CAT_DE,
      FCEUMM_APU_5_INFO_0_DE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_DE,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_DE,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_DE },
         { "Player 1", OPTION_VAL_PLAYER_1_DE },
         { "Player 2", OPTION_VAL_PLAYER_2_DE },
         { "Both",     OPTION_VAL_BOTH_DE },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_DE,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_DE,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_DE,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_DE,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_DE },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_DE },
         { "mouse",       OPTION_VAL_MOUSE_DE },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_DE,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_DE,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_DE,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_DE,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_DE,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_DE,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_DE,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_DE,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_DE,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_DE,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_DE },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_DE },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_DE,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_DE,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_DE },
         { "fill $00", OPTION_VAL_FILL_00_DE },
         { "random",   OPTION_VAL_RANDOM_DE },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_de = {
   option_cats_de,
   option_defs_de
};

/* RETRO_LANGUAGE_EL */

#define CATEGORY_SYSTEM_LABEL_EL NULL
#define CATEGORY_SYSTEM_INFO_0_EL NULL
#define CATEGORY_VIDEO_LABEL_EL "Οδηγός Βίντεο"
#define CATEGORY_VIDEO_INFO_0_EL NULL
#define CATEGORY_VIDEO_INFO_1_EL NULL
#define CATEGORY_AUDIO_LABEL_EL "Οδηγός Ήχου"
#define CATEGORY_AUDIO_INFO_0_EL NULL
#define CATEGORY_INPUT_LABEL_EL "Οδηγός Εισαγωγής"
#define CATEGORY_INPUT_INFO_0_EL NULL
#define CATEGORY_HACKS_LABEL_EL NULL
#define CATEGORY_HACKS_INFO_0_EL NULL
#define CATEGORY_DIP_SWITCH_LABEL_EL NULL
#define CATEGORY_DIP_SWITCH_INFO_0_EL NULL
#define FCEUMM_REGION_LABEL_EL NULL
#define FCEUMM_REGION_INFO_0_EL NULL
#define OPTION_VAL_AUTO_EL NULL
#define OPTION_VAL_NTSC_EL NULL
#define OPTION_VAL_PAL_EL NULL
#define OPTION_VAL_DENDY_EL NULL
#define FCEUMM_GAME_GENIE_LABEL_EL NULL
#define FCEUMM_GAME_GENIE_INFO_0_EL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_EL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_EL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_EL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_EL NULL
#define FCEUMM_ASPECT_LABEL_EL NULL
#define FCEUMM_ASPECT_INFO_0_EL NULL
#define OPTION_VAL_8_7_PAR_EL NULL
#define OPTION_VAL_4_3_EL NULL
#define OPTION_VAL_PP_EL NULL
#define FCEUMM_OVERSCAN_LABEL_EL NULL
#define FCEUMM_OVERSCAN_INFO_0_EL NULL
#define FCEUMM_OVERSCAN_H_LABEL_EL NULL
#define FCEUMM_OVERSCAN_H_INFO_0_EL NULL
#define FCEUMM_OVERSCAN_V_LABEL_EL NULL
#define FCEUMM_OVERSCAN_V_INFO_0_EL NULL
#define FCEUMM_PALETTE_LABEL_EL NULL
#define FCEUMM_PALETTE_INFO_0_EL NULL
#define OPTION_VAL_DEFAULT_EL NULL
#define OPTION_VAL_ASQREALC_EL NULL
#define OPTION_VAL_NINTENDO_VC_EL NULL
#define OPTION_VAL_RGB_EL NULL
#define OPTION_VAL_YUV_V3_EL NULL
#define OPTION_VAL_UNSATURATED_FINAL_EL NULL
#define OPTION_VAL_SONY_CXA2025AS_US_EL NULL
#define OPTION_VAL_BMF_FINAL2_EL NULL
#define OPTION_VAL_BMF_FINAL3_EL NULL
#define OPTION_VAL_SMOOTH_FBX_EL NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_EL NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_EL NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_EL NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_EL NULL
#define OPTION_VAL_NESCAP_EL NULL
#define OPTION_VAL_WAVEBEAM_EL NULL
#define OPTION_VAL_RAW_EL NULL
#define OPTION_VAL_CUSTOM_EL NULL
#define FCEUMM_NTSC_FILTER_LABEL_EL NULL
#define FCEUMM_NTSC_FILTER_INFO_0_EL NULL
#define OPTION_VAL_COMPOSITE_EL NULL
#define OPTION_VAL_SVIDEO_EL NULL
#define OPTION_VAL_RGB_O9_EL NULL
#define OPTION_VAL_MONOCHROME_EL NULL
#define FCEUMM_SNDQUALITY_LABEL_EL NULL
#define FCEUMM_SNDQUALITY_INFO_0_EL NULL
#define OPTION_VAL_LOW_EL NULL
#define OPTION_VAL_HIGH_EL NULL
#define OPTION_VAL_VERY_HIGH_EL NULL
#define FCEUMM_SWAPDUTY_LABEL_EL NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_EL NULL
#define FCEUMM_SWAPDUTY_INFO_0_EL NULL
#define FCEUMM_SNDVOLUME_LABEL_EL NULL
#define FCEUMM_SNDVOLUME_INFO_0_EL NULL
#define OPTION_VAL_0_EL NULL
#define OPTION_VAL_1_EL NULL
#define OPTION_VAL_2_EL NULL
#define OPTION_VAL_3_EL NULL
#define OPTION_VAL_4_EL NULL
#define OPTION_VAL_5_EL NULL
#define OPTION_VAL_6_EL NULL
#define OPTION_VAL_7_EL NULL
#define OPTION_VAL_8_EL NULL
#define OPTION_VAL_9_EL NULL
#define OPTION_VAL_10_EL NULL
#define FCEUMM_APU_1_LABEL_EL NULL
#define FCEUMM_APU_1_LABEL_CAT_EL NULL
#define FCEUMM_APU_1_INFO_0_EL NULL
#define FCEUMM_APU_2_LABEL_EL NULL
#define FCEUMM_APU_2_LABEL_CAT_EL NULL
#define FCEUMM_APU_2_INFO_0_EL NULL
#define FCEUMM_APU_3_LABEL_EL NULL
#define FCEUMM_APU_3_LABEL_CAT_EL NULL
#define FCEUMM_APU_3_INFO_0_EL NULL
#define FCEUMM_APU_4_LABEL_EL NULL
#define FCEUMM_APU_4_LABEL_CAT_EL NULL
#define FCEUMM_APU_4_INFO_0_EL NULL
#define FCEUMM_APU_5_LABEL_EL NULL
#define FCEUMM_APU_5_LABEL_CAT_EL NULL
#define FCEUMM_APU_5_INFO_0_EL NULL
#define FCEUMM_TURBO_ENABLE_LABEL_EL NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_EL NULL
#define OPTION_VAL_NONE_EL NULL
#define OPTION_VAL_PLAYER_1_EL NULL
#define OPTION_VAL_PLAYER_2_EL NULL
#define OPTION_VAL_BOTH_EL NULL
#define FCEUMM_TURBO_DELAY_LABEL_EL NULL
#define FCEUMM_TURBO_DELAY_INFO_0_EL NULL
#define FCEUMM_ZAPPER_MODE_LABEL_EL NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_EL NULL
#define OPTION_VAL_LIGHTGUN_EL NULL
#define OPTION_VAL_TOUCHSCREEN_EL NULL
#define OPTION_VAL_MOUSE_EL NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_EL NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_EL NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_EL NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_EL NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_EL NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_EL NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_EL NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_EL NULL
#define FCEUMM_OVERCLOCKING_LABEL_EL NULL
#define FCEUMM_OVERCLOCKING_INFO_0_EL NULL
#define OPTION_VAL_2X_POSTRENDER_EL NULL
#define OPTION_VAL_2X_VBLANK_EL NULL
#define FCEUMM_RAMSTATE_LABEL_EL NULL
#define FCEUMM_RAMSTATE_INFO_0_EL NULL
#define OPTION_VAL_FILL_FF_EL NULL
#define OPTION_VAL_FILL_00_EL NULL
#define OPTION_VAL_RANDOM_EL NULL

struct retro_core_option_v2_category option_cats_el[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_EL,
      CATEGORY_SYSTEM_INFO_0_EL
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_EL,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_EL
#else
      CATEGORY_VIDEO_INFO_1_EL
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_EL,
      CATEGORY_AUDIO_INFO_0_EL
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_EL,
      CATEGORY_INPUT_INFO_0_EL
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_EL,
      CATEGORY_HACKS_INFO_0_EL
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_EL,
      CATEGORY_DIP_SWITCH_INFO_0_EL
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_el[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_EL,
      NULL,
      FCEUMM_REGION_INFO_0_EL,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_EL },
         { "NTSC",  OPTION_VAL_NTSC_EL },
         { "PAL",   OPTION_VAL_PAL_EL },
         { "Dendy", OPTION_VAL_DENDY_EL },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_EL,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_EL,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_EL,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_EL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_EL,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_EL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_EL,
      NULL,
      FCEUMM_ASPECT_INFO_0_EL,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_EL },
         { "4:3",     OPTION_VAL_4_3_EL },
         { "PP",      OPTION_VAL_PP_EL },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_EL,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_EL,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_EL,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_EL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_EL,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_EL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_EL,
      NULL,
      FCEUMM_PALETTE_INFO_0_EL,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_EL },
         { "asqrealc",             OPTION_VAL_ASQREALC_EL },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_EL },
         { "rgb",                  OPTION_VAL_RGB_EL },
         { "yuv-v3",               OPTION_VAL_YUV_V3_EL },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_EL },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_EL },
         { "pal",                  OPTION_VAL_PAL_EL },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_EL },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_EL },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_EL },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_EL },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_EL },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_EL },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_EL },
         { "nescap",               OPTION_VAL_NESCAP_EL },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_EL },
         { "raw",                  OPTION_VAL_RAW_EL },
         { "custom",               OPTION_VAL_CUSTOM_EL },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_EL,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_EL,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_EL },
         { "svideo",     OPTION_VAL_SVIDEO_EL },
         { "rgb",        OPTION_VAL_RGB_O9_EL },
         { "monochrome", OPTION_VAL_MONOCHROME_EL },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_EL,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_EL,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_EL },
         { "High",      OPTION_VAL_HIGH_EL },
         { "Very High", OPTION_VAL_VERY_HIGH_EL },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_EL,
      FCEUMM_SWAPDUTY_LABEL_CAT_EL,
      FCEUMM_SWAPDUTY_INFO_0_EL,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_EL,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_EL,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_EL },
         { "1",  OPTION_VAL_1_EL },
         { "2",  OPTION_VAL_2_EL },
         { "3",  OPTION_VAL_3_EL },
         { "4",  OPTION_VAL_4_EL },
         { "5",  OPTION_VAL_5_EL },
         { "6",  OPTION_VAL_6_EL },
         { "7",  OPTION_VAL_7_EL },
         { "8",  OPTION_VAL_8_EL },
         { "9",  OPTION_VAL_9_EL },
         { "10", OPTION_VAL_10_EL },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_EL,
      FCEUMM_APU_1_LABEL_CAT_EL,
      FCEUMM_APU_1_INFO_0_EL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_EL,
      FCEUMM_APU_2_LABEL_CAT_EL,
      FCEUMM_APU_2_INFO_0_EL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_EL,
      FCEUMM_APU_3_LABEL_CAT_EL,
      FCEUMM_APU_3_INFO_0_EL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_EL,
      FCEUMM_APU_4_LABEL_CAT_EL,
      FCEUMM_APU_4_INFO_0_EL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_EL,
      FCEUMM_APU_5_LABEL_CAT_EL,
      FCEUMM_APU_5_INFO_0_EL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_EL,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_EL,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_EL },
         { "Player 1", OPTION_VAL_PLAYER_1_EL },
         { "Player 2", OPTION_VAL_PLAYER_2_EL },
         { "Both",     OPTION_VAL_BOTH_EL },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_EL,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_EL,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_EL,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_EL,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_EL },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_EL },
         { "mouse",       OPTION_VAL_MOUSE_EL },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_EL,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_EL,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_EL,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_EL,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_EL,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_EL,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_EL,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_EL,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_EL,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_EL,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_EL },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_EL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_EL,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_EL,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_EL },
         { "fill $00", OPTION_VAL_FILL_00_EL },
         { "random",   OPTION_VAL_RANDOM_EL },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_el = {
   option_cats_el,
   option_defs_el
};

/* RETRO_LANGUAGE_EO */

#define CATEGORY_SYSTEM_LABEL_EO NULL
#define CATEGORY_SYSTEM_INFO_0_EO NULL
#define CATEGORY_VIDEO_LABEL_EO "Video Driver"
#define CATEGORY_VIDEO_INFO_0_EO NULL
#define CATEGORY_VIDEO_INFO_1_EO NULL
#define CATEGORY_AUDIO_LABEL_EO "Audio Driver"
#define CATEGORY_AUDIO_INFO_0_EO NULL
#define CATEGORY_INPUT_LABEL_EO "Input Driver"
#define CATEGORY_INPUT_INFO_0_EO NULL
#define CATEGORY_HACKS_LABEL_EO NULL
#define CATEGORY_HACKS_INFO_0_EO NULL
#define CATEGORY_DIP_SWITCH_LABEL_EO NULL
#define CATEGORY_DIP_SWITCH_INFO_0_EO NULL
#define FCEUMM_REGION_LABEL_EO NULL
#define FCEUMM_REGION_INFO_0_EO NULL
#define OPTION_VAL_AUTO_EO NULL
#define OPTION_VAL_NTSC_EO NULL
#define OPTION_VAL_PAL_EO NULL
#define OPTION_VAL_DENDY_EO NULL
#define FCEUMM_GAME_GENIE_LABEL_EO NULL
#define FCEUMM_GAME_GENIE_INFO_0_EO NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_EO NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_EO NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_EO NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_EO NULL
#define FCEUMM_ASPECT_LABEL_EO NULL
#define FCEUMM_ASPECT_INFO_0_EO NULL
#define OPTION_VAL_8_7_PAR_EO NULL
#define OPTION_VAL_4_3_EO NULL
#define OPTION_VAL_PP_EO NULL
#define FCEUMM_OVERSCAN_LABEL_EO NULL
#define FCEUMM_OVERSCAN_INFO_0_EO NULL
#define FCEUMM_OVERSCAN_H_LABEL_EO NULL
#define FCEUMM_OVERSCAN_H_INFO_0_EO NULL
#define FCEUMM_OVERSCAN_V_LABEL_EO NULL
#define FCEUMM_OVERSCAN_V_INFO_0_EO NULL
#define FCEUMM_PALETTE_LABEL_EO NULL
#define FCEUMM_PALETTE_INFO_0_EO NULL
#define OPTION_VAL_DEFAULT_EO NULL
#define OPTION_VAL_ASQREALC_EO NULL
#define OPTION_VAL_NINTENDO_VC_EO NULL
#define OPTION_VAL_RGB_EO NULL
#define OPTION_VAL_YUV_V3_EO NULL
#define OPTION_VAL_UNSATURATED_FINAL_EO NULL
#define OPTION_VAL_SONY_CXA2025AS_US_EO NULL
#define OPTION_VAL_BMF_FINAL2_EO NULL
#define OPTION_VAL_BMF_FINAL3_EO NULL
#define OPTION_VAL_SMOOTH_FBX_EO NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_EO NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_EO NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_EO NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_EO NULL
#define OPTION_VAL_NESCAP_EO NULL
#define OPTION_VAL_WAVEBEAM_EO NULL
#define OPTION_VAL_RAW_EO NULL
#define OPTION_VAL_CUSTOM_EO NULL
#define FCEUMM_NTSC_FILTER_LABEL_EO NULL
#define FCEUMM_NTSC_FILTER_INFO_0_EO NULL
#define OPTION_VAL_COMPOSITE_EO NULL
#define OPTION_VAL_SVIDEO_EO NULL
#define OPTION_VAL_RGB_O9_EO NULL
#define OPTION_VAL_MONOCHROME_EO NULL
#define FCEUMM_SNDQUALITY_LABEL_EO NULL
#define FCEUMM_SNDQUALITY_INFO_0_EO NULL
#define OPTION_VAL_LOW_EO NULL
#define OPTION_VAL_HIGH_EO NULL
#define OPTION_VAL_VERY_HIGH_EO NULL
#define FCEUMM_SWAPDUTY_LABEL_EO NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_EO NULL
#define FCEUMM_SWAPDUTY_INFO_0_EO NULL
#define FCEUMM_SNDVOLUME_LABEL_EO NULL
#define FCEUMM_SNDVOLUME_INFO_0_EO NULL
#define OPTION_VAL_0_EO NULL
#define OPTION_VAL_1_EO NULL
#define OPTION_VAL_2_EO NULL
#define OPTION_VAL_3_EO NULL
#define OPTION_VAL_4_EO NULL
#define OPTION_VAL_5_EO NULL
#define OPTION_VAL_6_EO NULL
#define OPTION_VAL_7_EO NULL
#define OPTION_VAL_8_EO NULL
#define OPTION_VAL_9_EO NULL
#define OPTION_VAL_10_EO NULL
#define FCEUMM_APU_1_LABEL_EO NULL
#define FCEUMM_APU_1_LABEL_CAT_EO NULL
#define FCEUMM_APU_1_INFO_0_EO NULL
#define FCEUMM_APU_2_LABEL_EO NULL
#define FCEUMM_APU_2_LABEL_CAT_EO NULL
#define FCEUMM_APU_2_INFO_0_EO NULL
#define FCEUMM_APU_3_LABEL_EO NULL
#define FCEUMM_APU_3_LABEL_CAT_EO NULL
#define FCEUMM_APU_3_INFO_0_EO NULL
#define FCEUMM_APU_4_LABEL_EO NULL
#define FCEUMM_APU_4_LABEL_CAT_EO NULL
#define FCEUMM_APU_4_INFO_0_EO NULL
#define FCEUMM_APU_5_LABEL_EO NULL
#define FCEUMM_APU_5_LABEL_CAT_EO NULL
#define FCEUMM_APU_5_INFO_0_EO NULL
#define FCEUMM_TURBO_ENABLE_LABEL_EO NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_EO NULL
#define OPTION_VAL_NONE_EO NULL
#define OPTION_VAL_PLAYER_1_EO NULL
#define OPTION_VAL_PLAYER_2_EO NULL
#define OPTION_VAL_BOTH_EO NULL
#define FCEUMM_TURBO_DELAY_LABEL_EO NULL
#define FCEUMM_TURBO_DELAY_INFO_0_EO NULL
#define FCEUMM_ZAPPER_MODE_LABEL_EO NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_EO NULL
#define OPTION_VAL_LIGHTGUN_EO NULL
#define OPTION_VAL_TOUCHSCREEN_EO NULL
#define OPTION_VAL_MOUSE_EO NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_EO NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_EO NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_EO NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_EO NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_EO NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_EO NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_EO NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_EO NULL
#define FCEUMM_OVERCLOCKING_LABEL_EO NULL
#define FCEUMM_OVERCLOCKING_INFO_0_EO NULL
#define OPTION_VAL_2X_POSTRENDER_EO NULL
#define OPTION_VAL_2X_VBLANK_EO NULL
#define FCEUMM_RAMSTATE_LABEL_EO NULL
#define FCEUMM_RAMSTATE_INFO_0_EO NULL
#define OPTION_VAL_FILL_FF_EO NULL
#define OPTION_VAL_FILL_00_EO NULL
#define OPTION_VAL_RANDOM_EO NULL

struct retro_core_option_v2_category option_cats_eo[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_EO,
      CATEGORY_SYSTEM_INFO_0_EO
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_EO,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_EO
#else
      CATEGORY_VIDEO_INFO_1_EO
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_EO,
      CATEGORY_AUDIO_INFO_0_EO
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_EO,
      CATEGORY_INPUT_INFO_0_EO
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_EO,
      CATEGORY_HACKS_INFO_0_EO
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_EO,
      CATEGORY_DIP_SWITCH_INFO_0_EO
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_eo[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_EO,
      NULL,
      FCEUMM_REGION_INFO_0_EO,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_EO },
         { "NTSC",  OPTION_VAL_NTSC_EO },
         { "PAL",   OPTION_VAL_PAL_EO },
         { "Dendy", OPTION_VAL_DENDY_EO },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_EO,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_EO,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_EO,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_EO,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_EO,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_EO,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_EO,
      NULL,
      FCEUMM_ASPECT_INFO_0_EO,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_EO },
         { "4:3",     OPTION_VAL_4_3_EO },
         { "PP",      OPTION_VAL_PP_EO },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_EO,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_EO,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_EO,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_EO,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_EO,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_EO,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_EO,
      NULL,
      FCEUMM_PALETTE_INFO_0_EO,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_EO },
         { "asqrealc",             OPTION_VAL_ASQREALC_EO },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_EO },
         { "rgb",                  OPTION_VAL_RGB_EO },
         { "yuv-v3",               OPTION_VAL_YUV_V3_EO },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_EO },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_EO },
         { "pal",                  OPTION_VAL_PAL_EO },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_EO },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_EO },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_EO },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_EO },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_EO },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_EO },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_EO },
         { "nescap",               OPTION_VAL_NESCAP_EO },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_EO },
         { "raw",                  OPTION_VAL_RAW_EO },
         { "custom",               OPTION_VAL_CUSTOM_EO },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_EO,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_EO,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_EO },
         { "svideo",     OPTION_VAL_SVIDEO_EO },
         { "rgb",        OPTION_VAL_RGB_O9_EO },
         { "monochrome", OPTION_VAL_MONOCHROME_EO },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_EO,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_EO,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_EO },
         { "High",      OPTION_VAL_HIGH_EO },
         { "Very High", OPTION_VAL_VERY_HIGH_EO },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_EO,
      FCEUMM_SWAPDUTY_LABEL_CAT_EO,
      FCEUMM_SWAPDUTY_INFO_0_EO,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_EO,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_EO,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_EO },
         { "1",  OPTION_VAL_1_EO },
         { "2",  OPTION_VAL_2_EO },
         { "3",  OPTION_VAL_3_EO },
         { "4",  OPTION_VAL_4_EO },
         { "5",  OPTION_VAL_5_EO },
         { "6",  OPTION_VAL_6_EO },
         { "7",  OPTION_VAL_7_EO },
         { "8",  OPTION_VAL_8_EO },
         { "9",  OPTION_VAL_9_EO },
         { "10", OPTION_VAL_10_EO },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_EO,
      FCEUMM_APU_1_LABEL_CAT_EO,
      FCEUMM_APU_1_INFO_0_EO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_EO,
      FCEUMM_APU_2_LABEL_CAT_EO,
      FCEUMM_APU_2_INFO_0_EO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_EO,
      FCEUMM_APU_3_LABEL_CAT_EO,
      FCEUMM_APU_3_INFO_0_EO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_EO,
      FCEUMM_APU_4_LABEL_CAT_EO,
      FCEUMM_APU_4_INFO_0_EO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_EO,
      FCEUMM_APU_5_LABEL_CAT_EO,
      FCEUMM_APU_5_INFO_0_EO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_EO,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_EO,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_EO },
         { "Player 1", OPTION_VAL_PLAYER_1_EO },
         { "Player 2", OPTION_VAL_PLAYER_2_EO },
         { "Both",     OPTION_VAL_BOTH_EO },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_EO,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_EO,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_EO,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_EO,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_EO },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_EO },
         { "mouse",       OPTION_VAL_MOUSE_EO },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_EO,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_EO,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_EO,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_EO,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_EO,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_EO,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_EO,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_EO,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_EO,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_EO,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_EO },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_EO },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_EO,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_EO,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_EO },
         { "fill $00", OPTION_VAL_FILL_00_EO },
         { "random",   OPTION_VAL_RANDOM_EO },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_eo = {
   option_cats_eo,
   option_defs_eo
};

/* RETRO_LANGUAGE_ES */

#define CATEGORY_SYSTEM_LABEL_ES "Sistema"
#define CATEGORY_SYSTEM_INFO_0_ES "Cambia los ajustes de región y de expansiones de hardware."
#define CATEGORY_VIDEO_LABEL_ES "Vídeo"
#define CATEGORY_VIDEO_INFO_0_ES "Cambia los ajustes de relación de aspecto, recorte de pantalla, paleta de colores y los filtros de vídeo."
#define CATEGORY_VIDEO_INFO_1_ES "Cambia los ajustes de relación de aspecto, recorte de pantalla y paleta de colores."
#define CATEGORY_AUDIO_LABEL_ES NULL
#define CATEGORY_AUDIO_INFO_0_ES "Cambia los ajustes de calidad de sonido, volumen y canales."
#define CATEGORY_INPUT_LABEL_ES "Entrada"
#define CATEGORY_INPUT_INFO_0_ES "Cambia los ajustes del turbo y de las «lightguns»."
#define CATEGORY_HACKS_LABEL_ES "Arreglos de emulación"
#define CATEGORY_HACKS_INFO_0_ES "Cambia las opciones de overclockeo del procesador y de precisión de la emulación que afectan al rendimiento a bajo nivel y a la compatibilidad."
#define CATEGORY_DIP_SWITCH_LABEL_ES "Interruptores DIP"
#define CATEGORY_DIP_SWITCH_INFO_0_ES "Cambia los ajustes para juegos de recreativa o arcade."
#define FCEUMM_REGION_LABEL_ES NULL
#define FCEUMM_REGION_INFO_0_ES NULL
#define OPTION_VAL_AUTO_ES NULL
#define OPTION_VAL_NTSC_ES NULL
#define OPTION_VAL_PAL_ES NULL
#define OPTION_VAL_DENDY_ES NULL
#define FCEUMM_GAME_GENIE_LABEL_ES NULL
#define FCEUMM_GAME_GENIE_INFO_0_ES NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_ES NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_ES NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_ES NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_ES NULL
#define FCEUMM_ASPECT_LABEL_ES NULL
#define FCEUMM_ASPECT_INFO_0_ES NULL
#define OPTION_VAL_8_7_PAR_ES NULL
#define OPTION_VAL_4_3_ES NULL
#define OPTION_VAL_PP_ES NULL
#define FCEUMM_OVERSCAN_LABEL_ES NULL
#define FCEUMM_OVERSCAN_INFO_0_ES NULL
#define FCEUMM_OVERSCAN_H_LABEL_ES NULL
#define FCEUMM_OVERSCAN_H_INFO_0_ES NULL
#define FCEUMM_OVERSCAN_V_LABEL_ES NULL
#define FCEUMM_OVERSCAN_V_INFO_0_ES NULL
#define FCEUMM_PALETTE_LABEL_ES NULL
#define FCEUMM_PALETTE_INFO_0_ES NULL
#define OPTION_VAL_DEFAULT_ES NULL
#define OPTION_VAL_ASQREALC_ES NULL
#define OPTION_VAL_NINTENDO_VC_ES NULL
#define OPTION_VAL_RGB_ES NULL
#define OPTION_VAL_YUV_V3_ES NULL
#define OPTION_VAL_UNSATURATED_FINAL_ES NULL
#define OPTION_VAL_SONY_CXA2025AS_US_ES NULL
#define OPTION_VAL_BMF_FINAL2_ES NULL
#define OPTION_VAL_BMF_FINAL3_ES NULL
#define OPTION_VAL_SMOOTH_FBX_ES NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_ES NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_ES NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_ES NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_ES NULL
#define OPTION_VAL_NESCAP_ES NULL
#define OPTION_VAL_WAVEBEAM_ES NULL
#define OPTION_VAL_RAW_ES NULL
#define OPTION_VAL_CUSTOM_ES NULL
#define FCEUMM_NTSC_FILTER_LABEL_ES NULL
#define FCEUMM_NTSC_FILTER_INFO_0_ES NULL
#define OPTION_VAL_COMPOSITE_ES NULL
#define OPTION_VAL_SVIDEO_ES NULL
#define OPTION_VAL_RGB_O9_ES NULL
#define OPTION_VAL_MONOCHROME_ES NULL
#define FCEUMM_SNDQUALITY_LABEL_ES NULL
#define FCEUMM_SNDQUALITY_INFO_0_ES NULL
#define OPTION_VAL_LOW_ES NULL
#define OPTION_VAL_HIGH_ES NULL
#define OPTION_VAL_VERY_HIGH_ES NULL
#define FCEUMM_SWAPDUTY_LABEL_ES NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_ES NULL
#define FCEUMM_SWAPDUTY_INFO_0_ES NULL
#define FCEUMM_SNDVOLUME_LABEL_ES NULL
#define FCEUMM_SNDVOLUME_INFO_0_ES NULL
#define OPTION_VAL_0_ES NULL
#define OPTION_VAL_1_ES NULL
#define OPTION_VAL_2_ES NULL
#define OPTION_VAL_3_ES NULL
#define OPTION_VAL_4_ES NULL
#define OPTION_VAL_5_ES NULL
#define OPTION_VAL_6_ES NULL
#define OPTION_VAL_7_ES NULL
#define OPTION_VAL_8_ES NULL
#define OPTION_VAL_9_ES NULL
#define OPTION_VAL_10_ES NULL
#define FCEUMM_APU_1_LABEL_ES NULL
#define FCEUMM_APU_1_LABEL_CAT_ES NULL
#define FCEUMM_APU_1_INFO_0_ES NULL
#define FCEUMM_APU_2_LABEL_ES NULL
#define FCEUMM_APU_2_LABEL_CAT_ES NULL
#define FCEUMM_APU_2_INFO_0_ES NULL
#define FCEUMM_APU_3_LABEL_ES NULL
#define FCEUMM_APU_3_LABEL_CAT_ES NULL
#define FCEUMM_APU_3_INFO_0_ES NULL
#define FCEUMM_APU_4_LABEL_ES NULL
#define FCEUMM_APU_4_LABEL_CAT_ES NULL
#define FCEUMM_APU_4_INFO_0_ES NULL
#define FCEUMM_APU_5_LABEL_ES NULL
#define FCEUMM_APU_5_LABEL_CAT_ES NULL
#define FCEUMM_APU_5_INFO_0_ES NULL
#define FCEUMM_TURBO_ENABLE_LABEL_ES NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_ES NULL
#define OPTION_VAL_NONE_ES NULL
#define OPTION_VAL_PLAYER_1_ES NULL
#define OPTION_VAL_PLAYER_2_ES NULL
#define OPTION_VAL_BOTH_ES NULL
#define FCEUMM_TURBO_DELAY_LABEL_ES NULL
#define FCEUMM_TURBO_DELAY_INFO_0_ES NULL
#define FCEUMM_ZAPPER_MODE_LABEL_ES NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_ES NULL
#define OPTION_VAL_LIGHTGUN_ES NULL
#define OPTION_VAL_TOUCHSCREEN_ES NULL
#define OPTION_VAL_MOUSE_ES NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_ES NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_ES NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_ES NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_ES NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_ES NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_ES NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_ES NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_ES NULL
#define FCEUMM_OVERCLOCKING_LABEL_ES NULL
#define FCEUMM_OVERCLOCKING_INFO_0_ES NULL
#define OPTION_VAL_2X_POSTRENDER_ES NULL
#define OPTION_VAL_2X_VBLANK_ES NULL
#define FCEUMM_RAMSTATE_LABEL_ES NULL
#define FCEUMM_RAMSTATE_INFO_0_ES NULL
#define OPTION_VAL_FILL_FF_ES NULL
#define OPTION_VAL_FILL_00_ES NULL
#define OPTION_VAL_RANDOM_ES NULL

struct retro_core_option_v2_category option_cats_es[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_ES,
      CATEGORY_SYSTEM_INFO_0_ES
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_ES,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_ES
#else
      CATEGORY_VIDEO_INFO_1_ES
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_ES,
      CATEGORY_AUDIO_INFO_0_ES
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_ES,
      CATEGORY_INPUT_INFO_0_ES
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_ES,
      CATEGORY_HACKS_INFO_0_ES
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_ES,
      CATEGORY_DIP_SWITCH_INFO_0_ES
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_es[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_ES,
      NULL,
      FCEUMM_REGION_INFO_0_ES,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_ES },
         { "NTSC",  OPTION_VAL_NTSC_ES },
         { "PAL",   OPTION_VAL_PAL_ES },
         { "Dendy", OPTION_VAL_DENDY_ES },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_ES,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_ES,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_ES,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_ES,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_ES,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_ES,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_ES,
      NULL,
      FCEUMM_ASPECT_INFO_0_ES,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_ES },
         { "4:3",     OPTION_VAL_4_3_ES },
         { "PP",      OPTION_VAL_PP_ES },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_ES,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_ES,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_ES,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_ES,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_ES,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_ES,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_ES,
      NULL,
      FCEUMM_PALETTE_INFO_0_ES,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_ES },
         { "asqrealc",             OPTION_VAL_ASQREALC_ES },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_ES },
         { "rgb",                  OPTION_VAL_RGB_ES },
         { "yuv-v3",               OPTION_VAL_YUV_V3_ES },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_ES },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_ES },
         { "pal",                  OPTION_VAL_PAL_ES },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_ES },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_ES },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_ES },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_ES },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_ES },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_ES },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_ES },
         { "nescap",               OPTION_VAL_NESCAP_ES },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_ES },
         { "raw",                  OPTION_VAL_RAW_ES },
         { "custom",               OPTION_VAL_CUSTOM_ES },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_ES,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_ES,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_ES },
         { "svideo",     OPTION_VAL_SVIDEO_ES },
         { "rgb",        OPTION_VAL_RGB_O9_ES },
         { "monochrome", OPTION_VAL_MONOCHROME_ES },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_ES,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_ES,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_ES },
         { "High",      OPTION_VAL_HIGH_ES },
         { "Very High", OPTION_VAL_VERY_HIGH_ES },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_ES,
      FCEUMM_SWAPDUTY_LABEL_CAT_ES,
      FCEUMM_SWAPDUTY_INFO_0_ES,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_ES,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_ES,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_ES },
         { "1",  OPTION_VAL_1_ES },
         { "2",  OPTION_VAL_2_ES },
         { "3",  OPTION_VAL_3_ES },
         { "4",  OPTION_VAL_4_ES },
         { "5",  OPTION_VAL_5_ES },
         { "6",  OPTION_VAL_6_ES },
         { "7",  OPTION_VAL_7_ES },
         { "8",  OPTION_VAL_8_ES },
         { "9",  OPTION_VAL_9_ES },
         { "10", OPTION_VAL_10_ES },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_ES,
      FCEUMM_APU_1_LABEL_CAT_ES,
      FCEUMM_APU_1_INFO_0_ES,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_ES,
      FCEUMM_APU_2_LABEL_CAT_ES,
      FCEUMM_APU_2_INFO_0_ES,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_ES,
      FCEUMM_APU_3_LABEL_CAT_ES,
      FCEUMM_APU_3_INFO_0_ES,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_ES,
      FCEUMM_APU_4_LABEL_CAT_ES,
      FCEUMM_APU_4_INFO_0_ES,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_ES,
      FCEUMM_APU_5_LABEL_CAT_ES,
      FCEUMM_APU_5_INFO_0_ES,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_ES,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_ES,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_ES },
         { "Player 1", OPTION_VAL_PLAYER_1_ES },
         { "Player 2", OPTION_VAL_PLAYER_2_ES },
         { "Both",     OPTION_VAL_BOTH_ES },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_ES,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_ES,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_ES,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_ES,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_ES },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_ES },
         { "mouse",       OPTION_VAL_MOUSE_ES },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_ES,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_ES,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_ES,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_ES,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_ES,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_ES,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_ES,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_ES,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_ES,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_ES,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_ES },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_ES },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_ES,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_ES,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_ES },
         { "fill $00", OPTION_VAL_FILL_00_ES },
         { "random",   OPTION_VAL_RANDOM_ES },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_es = {
   option_cats_es,
   option_defs_es
};

/* RETRO_LANGUAGE_FA */

#define CATEGORY_SYSTEM_LABEL_FA NULL
#define CATEGORY_SYSTEM_INFO_0_FA NULL
#define CATEGORY_VIDEO_LABEL_FA "ویدیو"
#define CATEGORY_VIDEO_INFO_0_FA NULL
#define CATEGORY_VIDEO_INFO_1_FA NULL
#define CATEGORY_AUDIO_LABEL_FA "صدا"
#define CATEGORY_AUDIO_INFO_0_FA NULL
#define CATEGORY_INPUT_LABEL_FA "داده"
#define CATEGORY_INPUT_INFO_0_FA NULL
#define CATEGORY_HACKS_LABEL_FA NULL
#define CATEGORY_HACKS_INFO_0_FA NULL
#define CATEGORY_DIP_SWITCH_LABEL_FA NULL
#define CATEGORY_DIP_SWITCH_INFO_0_FA NULL
#define FCEUMM_REGION_LABEL_FA NULL
#define FCEUMM_REGION_INFO_0_FA NULL
#define OPTION_VAL_AUTO_FA NULL
#define OPTION_VAL_NTSC_FA NULL
#define OPTION_VAL_PAL_FA NULL
#define OPTION_VAL_DENDY_FA NULL
#define FCEUMM_GAME_GENIE_LABEL_FA NULL
#define FCEUMM_GAME_GENIE_INFO_0_FA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_FA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_FA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_FA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_FA NULL
#define FCEUMM_ASPECT_LABEL_FA NULL
#define FCEUMM_ASPECT_INFO_0_FA NULL
#define OPTION_VAL_8_7_PAR_FA NULL
#define OPTION_VAL_4_3_FA NULL
#define OPTION_VAL_PP_FA NULL
#define FCEUMM_OVERSCAN_LABEL_FA NULL
#define FCEUMM_OVERSCAN_INFO_0_FA NULL
#define FCEUMM_OVERSCAN_H_LABEL_FA NULL
#define FCEUMM_OVERSCAN_H_INFO_0_FA NULL
#define FCEUMM_OVERSCAN_V_LABEL_FA NULL
#define FCEUMM_OVERSCAN_V_INFO_0_FA NULL
#define FCEUMM_PALETTE_LABEL_FA NULL
#define FCEUMM_PALETTE_INFO_0_FA NULL
#define OPTION_VAL_DEFAULT_FA NULL
#define OPTION_VAL_ASQREALC_FA NULL
#define OPTION_VAL_NINTENDO_VC_FA NULL
#define OPTION_VAL_RGB_FA NULL
#define OPTION_VAL_YUV_V3_FA NULL
#define OPTION_VAL_UNSATURATED_FINAL_FA NULL
#define OPTION_VAL_SONY_CXA2025AS_US_FA NULL
#define OPTION_VAL_BMF_FINAL2_FA NULL
#define OPTION_VAL_BMF_FINAL3_FA NULL
#define OPTION_VAL_SMOOTH_FBX_FA NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_FA NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_FA NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_FA NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_FA NULL
#define OPTION_VAL_NESCAP_FA NULL
#define OPTION_VAL_WAVEBEAM_FA NULL
#define OPTION_VAL_RAW_FA NULL
#define OPTION_VAL_CUSTOM_FA NULL
#define FCEUMM_NTSC_FILTER_LABEL_FA NULL
#define FCEUMM_NTSC_FILTER_INFO_0_FA NULL
#define OPTION_VAL_COMPOSITE_FA NULL
#define OPTION_VAL_SVIDEO_FA NULL
#define OPTION_VAL_RGB_O9_FA NULL
#define OPTION_VAL_MONOCHROME_FA NULL
#define FCEUMM_SNDQUALITY_LABEL_FA NULL
#define FCEUMM_SNDQUALITY_INFO_0_FA NULL
#define OPTION_VAL_LOW_FA NULL
#define OPTION_VAL_HIGH_FA NULL
#define OPTION_VAL_VERY_HIGH_FA NULL
#define FCEUMM_SWAPDUTY_LABEL_FA NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_FA NULL
#define FCEUMM_SWAPDUTY_INFO_0_FA NULL
#define FCEUMM_SNDVOLUME_LABEL_FA NULL
#define FCEUMM_SNDVOLUME_INFO_0_FA NULL
#define OPTION_VAL_0_FA NULL
#define OPTION_VAL_1_FA NULL
#define OPTION_VAL_2_FA NULL
#define OPTION_VAL_3_FA NULL
#define OPTION_VAL_4_FA NULL
#define OPTION_VAL_5_FA NULL
#define OPTION_VAL_6_FA NULL
#define OPTION_VAL_7_FA NULL
#define OPTION_VAL_8_FA NULL
#define OPTION_VAL_9_FA NULL
#define OPTION_VAL_10_FA NULL
#define FCEUMM_APU_1_LABEL_FA NULL
#define FCEUMM_APU_1_LABEL_CAT_FA NULL
#define FCEUMM_APU_1_INFO_0_FA NULL
#define FCEUMM_APU_2_LABEL_FA NULL
#define FCEUMM_APU_2_LABEL_CAT_FA NULL
#define FCEUMM_APU_2_INFO_0_FA NULL
#define FCEUMM_APU_3_LABEL_FA NULL
#define FCEUMM_APU_3_LABEL_CAT_FA NULL
#define FCEUMM_APU_3_INFO_0_FA NULL
#define FCEUMM_APU_4_LABEL_FA NULL
#define FCEUMM_APU_4_LABEL_CAT_FA NULL
#define FCEUMM_APU_4_INFO_0_FA NULL
#define FCEUMM_APU_5_LABEL_FA NULL
#define FCEUMM_APU_5_LABEL_CAT_FA NULL
#define FCEUMM_APU_5_INFO_0_FA NULL
#define FCEUMM_TURBO_ENABLE_LABEL_FA NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_FA NULL
#define OPTION_VAL_NONE_FA NULL
#define OPTION_VAL_PLAYER_1_FA NULL
#define OPTION_VAL_PLAYER_2_FA NULL
#define OPTION_VAL_BOTH_FA NULL
#define FCEUMM_TURBO_DELAY_LABEL_FA NULL
#define FCEUMM_TURBO_DELAY_INFO_0_FA NULL
#define FCEUMM_ZAPPER_MODE_LABEL_FA NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_FA NULL
#define OPTION_VAL_LIGHTGUN_FA NULL
#define OPTION_VAL_TOUCHSCREEN_FA NULL
#define OPTION_VAL_MOUSE_FA NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_FA NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_FA NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_FA NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_FA NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_FA NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_FA NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_FA NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_FA NULL
#define FCEUMM_OVERCLOCKING_LABEL_FA NULL
#define FCEUMM_OVERCLOCKING_INFO_0_FA NULL
#define OPTION_VAL_2X_POSTRENDER_FA NULL
#define OPTION_VAL_2X_VBLANK_FA NULL
#define FCEUMM_RAMSTATE_LABEL_FA NULL
#define FCEUMM_RAMSTATE_INFO_0_FA NULL
#define OPTION_VAL_FILL_FF_FA NULL
#define OPTION_VAL_FILL_00_FA NULL
#define OPTION_VAL_RANDOM_FA NULL

struct retro_core_option_v2_category option_cats_fa[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_FA,
      CATEGORY_SYSTEM_INFO_0_FA
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_FA,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_FA
#else
      CATEGORY_VIDEO_INFO_1_FA
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_FA,
      CATEGORY_AUDIO_INFO_0_FA
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_FA,
      CATEGORY_INPUT_INFO_0_FA
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_FA,
      CATEGORY_HACKS_INFO_0_FA
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_FA,
      CATEGORY_DIP_SWITCH_INFO_0_FA
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_fa[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_FA,
      NULL,
      FCEUMM_REGION_INFO_0_FA,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_FA },
         { "NTSC",  OPTION_VAL_NTSC_FA },
         { "PAL",   OPTION_VAL_PAL_FA },
         { "Dendy", OPTION_VAL_DENDY_FA },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_FA,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_FA,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_FA,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_FA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_FA,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_FA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_FA,
      NULL,
      FCEUMM_ASPECT_INFO_0_FA,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_FA },
         { "4:3",     OPTION_VAL_4_3_FA },
         { "PP",      OPTION_VAL_PP_FA },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_FA,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_FA,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_FA,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_FA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_FA,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_FA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_FA,
      NULL,
      FCEUMM_PALETTE_INFO_0_FA,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_FA },
         { "asqrealc",             OPTION_VAL_ASQREALC_FA },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_FA },
         { "rgb",                  OPTION_VAL_RGB_FA },
         { "yuv-v3",               OPTION_VAL_YUV_V3_FA },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_FA },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_FA },
         { "pal",                  OPTION_VAL_PAL_FA },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_FA },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_FA },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_FA },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_FA },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_FA },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_FA },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_FA },
         { "nescap",               OPTION_VAL_NESCAP_FA },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_FA },
         { "raw",                  OPTION_VAL_RAW_FA },
         { "custom",               OPTION_VAL_CUSTOM_FA },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_FA,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_FA,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_FA },
         { "svideo",     OPTION_VAL_SVIDEO_FA },
         { "rgb",        OPTION_VAL_RGB_O9_FA },
         { "monochrome", OPTION_VAL_MONOCHROME_FA },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_FA,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_FA,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_FA },
         { "High",      OPTION_VAL_HIGH_FA },
         { "Very High", OPTION_VAL_VERY_HIGH_FA },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_FA,
      FCEUMM_SWAPDUTY_LABEL_CAT_FA,
      FCEUMM_SWAPDUTY_INFO_0_FA,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_FA,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_FA,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_FA },
         { "1",  OPTION_VAL_1_FA },
         { "2",  OPTION_VAL_2_FA },
         { "3",  OPTION_VAL_3_FA },
         { "4",  OPTION_VAL_4_FA },
         { "5",  OPTION_VAL_5_FA },
         { "6",  OPTION_VAL_6_FA },
         { "7",  OPTION_VAL_7_FA },
         { "8",  OPTION_VAL_8_FA },
         { "9",  OPTION_VAL_9_FA },
         { "10", OPTION_VAL_10_FA },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_FA,
      FCEUMM_APU_1_LABEL_CAT_FA,
      FCEUMM_APU_1_INFO_0_FA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_FA,
      FCEUMM_APU_2_LABEL_CAT_FA,
      FCEUMM_APU_2_INFO_0_FA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_FA,
      FCEUMM_APU_3_LABEL_CAT_FA,
      FCEUMM_APU_3_INFO_0_FA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_FA,
      FCEUMM_APU_4_LABEL_CAT_FA,
      FCEUMM_APU_4_INFO_0_FA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_FA,
      FCEUMM_APU_5_LABEL_CAT_FA,
      FCEUMM_APU_5_INFO_0_FA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_FA,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_FA,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_FA },
         { "Player 1", OPTION_VAL_PLAYER_1_FA },
         { "Player 2", OPTION_VAL_PLAYER_2_FA },
         { "Both",     OPTION_VAL_BOTH_FA },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_FA,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_FA,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_FA,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_FA,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_FA },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_FA },
         { "mouse",       OPTION_VAL_MOUSE_FA },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_FA,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_FA,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_FA,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_FA,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_FA,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_FA,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_FA,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_FA,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_FA,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_FA,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_FA },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_FA },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_FA,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_FA,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_FA },
         { "fill $00", OPTION_VAL_FILL_00_FA },
         { "random",   OPTION_VAL_RANDOM_FA },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_fa = {
   option_cats_fa,
   option_defs_fa
};

/* RETRO_LANGUAGE_FI */

#define CATEGORY_SYSTEM_LABEL_FI "Järjestelmä"
#define CATEGORY_SYSTEM_INFO_0_FI NULL
#define CATEGORY_VIDEO_LABEL_FI NULL
#define CATEGORY_VIDEO_INFO_0_FI NULL
#define CATEGORY_VIDEO_INFO_1_FI NULL
#define CATEGORY_AUDIO_LABEL_FI "Ääni"
#define CATEGORY_AUDIO_INFO_0_FI NULL
#define CATEGORY_INPUT_LABEL_FI "Syöte"
#define CATEGORY_INPUT_INFO_0_FI NULL
#define CATEGORY_HACKS_LABEL_FI "Emulointikikat"
#define CATEGORY_HACKS_INFO_0_FI "Määritä prosessorin ylikellotus- ja emuloinnin tarkkuuden asetukset, jotka vaikuttavat matalan tason suorituskykyyn ja yhteensopivuuteen."
#define CATEGORY_DIP_SWITCH_LABEL_FI NULL
#define CATEGORY_DIP_SWITCH_INFO_0_FI NULL
#define FCEUMM_REGION_LABEL_FI NULL
#define FCEUMM_REGION_INFO_0_FI NULL
#define OPTION_VAL_AUTO_FI NULL
#define OPTION_VAL_NTSC_FI NULL
#define OPTION_VAL_PAL_FI NULL
#define OPTION_VAL_DENDY_FI NULL
#define FCEUMM_GAME_GENIE_LABEL_FI NULL
#define FCEUMM_GAME_GENIE_INFO_0_FI NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_FI NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_FI NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_FI NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_FI NULL
#define FCEUMM_ASPECT_LABEL_FI NULL
#define FCEUMM_ASPECT_INFO_0_FI NULL
#define OPTION_VAL_8_7_PAR_FI NULL
#define OPTION_VAL_4_3_FI NULL
#define OPTION_VAL_PP_FI NULL
#define FCEUMM_OVERSCAN_LABEL_FI NULL
#define FCEUMM_OVERSCAN_INFO_0_FI NULL
#define FCEUMM_OVERSCAN_H_LABEL_FI NULL
#define FCEUMM_OVERSCAN_H_INFO_0_FI NULL
#define FCEUMM_OVERSCAN_V_LABEL_FI NULL
#define FCEUMM_OVERSCAN_V_INFO_0_FI NULL
#define FCEUMM_PALETTE_LABEL_FI NULL
#define FCEUMM_PALETTE_INFO_0_FI NULL
#define OPTION_VAL_DEFAULT_FI NULL
#define OPTION_VAL_ASQREALC_FI NULL
#define OPTION_VAL_NINTENDO_VC_FI NULL
#define OPTION_VAL_RGB_FI NULL
#define OPTION_VAL_YUV_V3_FI NULL
#define OPTION_VAL_UNSATURATED_FINAL_FI NULL
#define OPTION_VAL_SONY_CXA2025AS_US_FI NULL
#define OPTION_VAL_BMF_FINAL2_FI NULL
#define OPTION_VAL_BMF_FINAL3_FI NULL
#define OPTION_VAL_SMOOTH_FBX_FI NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_FI NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_FI NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_FI NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_FI NULL
#define OPTION_VAL_NESCAP_FI NULL
#define OPTION_VAL_WAVEBEAM_FI NULL
#define OPTION_VAL_RAW_FI NULL
#define OPTION_VAL_CUSTOM_FI NULL
#define FCEUMM_NTSC_FILTER_LABEL_FI NULL
#define FCEUMM_NTSC_FILTER_INFO_0_FI NULL
#define OPTION_VAL_COMPOSITE_FI NULL
#define OPTION_VAL_SVIDEO_FI NULL
#define OPTION_VAL_RGB_O9_FI NULL
#define OPTION_VAL_MONOCHROME_FI NULL
#define FCEUMM_SNDQUALITY_LABEL_FI NULL
#define FCEUMM_SNDQUALITY_INFO_0_FI NULL
#define OPTION_VAL_LOW_FI NULL
#define OPTION_VAL_HIGH_FI NULL
#define OPTION_VAL_VERY_HIGH_FI NULL
#define FCEUMM_SWAPDUTY_LABEL_FI NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_FI NULL
#define FCEUMM_SWAPDUTY_INFO_0_FI NULL
#define FCEUMM_SNDVOLUME_LABEL_FI NULL
#define FCEUMM_SNDVOLUME_INFO_0_FI NULL
#define OPTION_VAL_0_FI NULL
#define OPTION_VAL_1_FI NULL
#define OPTION_VAL_2_FI NULL
#define OPTION_VAL_3_FI NULL
#define OPTION_VAL_4_FI NULL
#define OPTION_VAL_5_FI NULL
#define OPTION_VAL_6_FI NULL
#define OPTION_VAL_7_FI NULL
#define OPTION_VAL_8_FI NULL
#define OPTION_VAL_9_FI NULL
#define OPTION_VAL_10_FI NULL
#define FCEUMM_APU_1_LABEL_FI NULL
#define FCEUMM_APU_1_LABEL_CAT_FI NULL
#define FCEUMM_APU_1_INFO_0_FI NULL
#define FCEUMM_APU_2_LABEL_FI NULL
#define FCEUMM_APU_2_LABEL_CAT_FI NULL
#define FCEUMM_APU_2_INFO_0_FI NULL
#define FCEUMM_APU_3_LABEL_FI NULL
#define FCEUMM_APU_3_LABEL_CAT_FI NULL
#define FCEUMM_APU_3_INFO_0_FI NULL
#define FCEUMM_APU_4_LABEL_FI NULL
#define FCEUMM_APU_4_LABEL_CAT_FI NULL
#define FCEUMM_APU_4_INFO_0_FI NULL
#define FCEUMM_APU_5_LABEL_FI NULL
#define FCEUMM_APU_5_LABEL_CAT_FI NULL
#define FCEUMM_APU_5_INFO_0_FI NULL
#define FCEUMM_TURBO_ENABLE_LABEL_FI NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_FI NULL
#define OPTION_VAL_NONE_FI NULL
#define OPTION_VAL_PLAYER_1_FI NULL
#define OPTION_VAL_PLAYER_2_FI NULL
#define OPTION_VAL_BOTH_FI NULL
#define FCEUMM_TURBO_DELAY_LABEL_FI NULL
#define FCEUMM_TURBO_DELAY_INFO_0_FI NULL
#define FCEUMM_ZAPPER_MODE_LABEL_FI NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_FI NULL
#define OPTION_VAL_LIGHTGUN_FI NULL
#define OPTION_VAL_TOUCHSCREEN_FI NULL
#define OPTION_VAL_MOUSE_FI NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_FI NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_FI NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_FI NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_FI NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_FI NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_FI NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_FI NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_FI NULL
#define FCEUMM_OVERCLOCKING_LABEL_FI NULL
#define FCEUMM_OVERCLOCKING_INFO_0_FI NULL
#define OPTION_VAL_2X_POSTRENDER_FI NULL
#define OPTION_VAL_2X_VBLANK_FI NULL
#define FCEUMM_RAMSTATE_LABEL_FI NULL
#define FCEUMM_RAMSTATE_INFO_0_FI NULL
#define OPTION_VAL_FILL_FF_FI NULL
#define OPTION_VAL_FILL_00_FI NULL
#define OPTION_VAL_RANDOM_FI NULL

struct retro_core_option_v2_category option_cats_fi[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_FI,
      CATEGORY_SYSTEM_INFO_0_FI
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_FI,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_FI
#else
      CATEGORY_VIDEO_INFO_1_FI
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_FI,
      CATEGORY_AUDIO_INFO_0_FI
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_FI,
      CATEGORY_INPUT_INFO_0_FI
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_FI,
      CATEGORY_HACKS_INFO_0_FI
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_FI,
      CATEGORY_DIP_SWITCH_INFO_0_FI
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_fi[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_FI,
      NULL,
      FCEUMM_REGION_INFO_0_FI,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_FI },
         { "NTSC",  OPTION_VAL_NTSC_FI },
         { "PAL",   OPTION_VAL_PAL_FI },
         { "Dendy", OPTION_VAL_DENDY_FI },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_FI,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_FI,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_FI,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_FI,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_FI,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_FI,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_FI,
      NULL,
      FCEUMM_ASPECT_INFO_0_FI,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_FI },
         { "4:3",     OPTION_VAL_4_3_FI },
         { "PP",      OPTION_VAL_PP_FI },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_FI,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_FI,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_FI,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_FI,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_FI,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_FI,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_FI,
      NULL,
      FCEUMM_PALETTE_INFO_0_FI,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_FI },
         { "asqrealc",             OPTION_VAL_ASQREALC_FI },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_FI },
         { "rgb",                  OPTION_VAL_RGB_FI },
         { "yuv-v3",               OPTION_VAL_YUV_V3_FI },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_FI },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_FI },
         { "pal",                  OPTION_VAL_PAL_FI },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_FI },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_FI },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_FI },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_FI },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_FI },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_FI },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_FI },
         { "nescap",               OPTION_VAL_NESCAP_FI },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_FI },
         { "raw",                  OPTION_VAL_RAW_FI },
         { "custom",               OPTION_VAL_CUSTOM_FI },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_FI,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_FI,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_FI },
         { "svideo",     OPTION_VAL_SVIDEO_FI },
         { "rgb",        OPTION_VAL_RGB_O9_FI },
         { "monochrome", OPTION_VAL_MONOCHROME_FI },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_FI,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_FI,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_FI },
         { "High",      OPTION_VAL_HIGH_FI },
         { "Very High", OPTION_VAL_VERY_HIGH_FI },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_FI,
      FCEUMM_SWAPDUTY_LABEL_CAT_FI,
      FCEUMM_SWAPDUTY_INFO_0_FI,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_FI,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_FI,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_FI },
         { "1",  OPTION_VAL_1_FI },
         { "2",  OPTION_VAL_2_FI },
         { "3",  OPTION_VAL_3_FI },
         { "4",  OPTION_VAL_4_FI },
         { "5",  OPTION_VAL_5_FI },
         { "6",  OPTION_VAL_6_FI },
         { "7",  OPTION_VAL_7_FI },
         { "8",  OPTION_VAL_8_FI },
         { "9",  OPTION_VAL_9_FI },
         { "10", OPTION_VAL_10_FI },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_FI,
      FCEUMM_APU_1_LABEL_CAT_FI,
      FCEUMM_APU_1_INFO_0_FI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_FI,
      FCEUMM_APU_2_LABEL_CAT_FI,
      FCEUMM_APU_2_INFO_0_FI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_FI,
      FCEUMM_APU_3_LABEL_CAT_FI,
      FCEUMM_APU_3_INFO_0_FI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_FI,
      FCEUMM_APU_4_LABEL_CAT_FI,
      FCEUMM_APU_4_INFO_0_FI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_FI,
      FCEUMM_APU_5_LABEL_CAT_FI,
      FCEUMM_APU_5_INFO_0_FI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_FI,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_FI,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_FI },
         { "Player 1", OPTION_VAL_PLAYER_1_FI },
         { "Player 2", OPTION_VAL_PLAYER_2_FI },
         { "Both",     OPTION_VAL_BOTH_FI },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_FI,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_FI,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_FI,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_FI,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_FI },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_FI },
         { "mouse",       OPTION_VAL_MOUSE_FI },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_FI,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_FI,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_FI,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_FI,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_FI,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_FI,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_FI,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_FI,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_FI,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_FI,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_FI },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_FI },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_FI,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_FI,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_FI },
         { "fill $00", OPTION_VAL_FILL_00_FI },
         { "random",   OPTION_VAL_RANDOM_FI },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_fi = {
   option_cats_fi,
   option_defs_fi
};

/* RETRO_LANGUAGE_FR */

#define CATEGORY_SYSTEM_LABEL_FR "Système"
#define CATEGORY_SYSTEM_INFO_0_FR NULL
#define CATEGORY_VIDEO_LABEL_FR "Vidéo"
#define CATEGORY_VIDEO_INFO_0_FR NULL
#define CATEGORY_VIDEO_INFO_1_FR NULL
#define CATEGORY_AUDIO_LABEL_FR NULL
#define CATEGORY_AUDIO_INFO_0_FR NULL
#define CATEGORY_INPUT_LABEL_FR "Entrées"
#define CATEGORY_INPUT_INFO_0_FR NULL
#define CATEGORY_HACKS_LABEL_FR "Hacks d'émulation"
#define CATEGORY_HACKS_INFO_0_FR "Configurer les paramètres d'overclocking du processeur et de la précision d'émulation qui affectent les performances de bas niveau et la compatibilité."
#define CATEGORY_DIP_SWITCH_LABEL_FR NULL
#define CATEGORY_DIP_SWITCH_INFO_0_FR NULL
#define FCEUMM_REGION_LABEL_FR NULL
#define FCEUMM_REGION_INFO_0_FR NULL
#define OPTION_VAL_AUTO_FR NULL
#define OPTION_VAL_NTSC_FR NULL
#define OPTION_VAL_PAL_FR NULL
#define OPTION_VAL_DENDY_FR NULL
#define FCEUMM_GAME_GENIE_LABEL_FR NULL
#define FCEUMM_GAME_GENIE_INFO_0_FR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_FR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_FR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_FR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_FR NULL
#define FCEUMM_ASPECT_LABEL_FR NULL
#define FCEUMM_ASPECT_INFO_0_FR NULL
#define OPTION_VAL_8_7_PAR_FR NULL
#define OPTION_VAL_4_3_FR NULL
#define OPTION_VAL_PP_FR NULL
#define FCEUMM_OVERSCAN_LABEL_FR NULL
#define FCEUMM_OVERSCAN_INFO_0_FR NULL
#define FCEUMM_OVERSCAN_H_LABEL_FR NULL
#define FCEUMM_OVERSCAN_H_INFO_0_FR NULL
#define FCEUMM_OVERSCAN_V_LABEL_FR NULL
#define FCEUMM_OVERSCAN_V_INFO_0_FR NULL
#define FCEUMM_PALETTE_LABEL_FR NULL
#define FCEUMM_PALETTE_INFO_0_FR NULL
#define OPTION_VAL_DEFAULT_FR NULL
#define OPTION_VAL_ASQREALC_FR NULL
#define OPTION_VAL_NINTENDO_VC_FR NULL
#define OPTION_VAL_RGB_FR NULL
#define OPTION_VAL_YUV_V3_FR NULL
#define OPTION_VAL_UNSATURATED_FINAL_FR NULL
#define OPTION_VAL_SONY_CXA2025AS_US_FR NULL
#define OPTION_VAL_BMF_FINAL2_FR NULL
#define OPTION_VAL_BMF_FINAL3_FR NULL
#define OPTION_VAL_SMOOTH_FBX_FR NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_FR NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_FR NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_FR NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_FR NULL
#define OPTION_VAL_NESCAP_FR NULL
#define OPTION_VAL_WAVEBEAM_FR NULL
#define OPTION_VAL_RAW_FR NULL
#define OPTION_VAL_CUSTOM_FR NULL
#define FCEUMM_NTSC_FILTER_LABEL_FR NULL
#define FCEUMM_NTSC_FILTER_INFO_0_FR NULL
#define OPTION_VAL_COMPOSITE_FR NULL
#define OPTION_VAL_SVIDEO_FR NULL
#define OPTION_VAL_RGB_O9_FR NULL
#define OPTION_VAL_MONOCHROME_FR NULL
#define FCEUMM_SNDQUALITY_LABEL_FR NULL
#define FCEUMM_SNDQUALITY_INFO_0_FR NULL
#define OPTION_VAL_LOW_FR NULL
#define OPTION_VAL_HIGH_FR NULL
#define OPTION_VAL_VERY_HIGH_FR NULL
#define FCEUMM_SWAPDUTY_LABEL_FR NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_FR NULL
#define FCEUMM_SWAPDUTY_INFO_0_FR NULL
#define FCEUMM_SNDVOLUME_LABEL_FR NULL
#define FCEUMM_SNDVOLUME_INFO_0_FR NULL
#define OPTION_VAL_0_FR NULL
#define OPTION_VAL_1_FR NULL
#define OPTION_VAL_2_FR NULL
#define OPTION_VAL_3_FR NULL
#define OPTION_VAL_4_FR NULL
#define OPTION_VAL_5_FR NULL
#define OPTION_VAL_6_FR NULL
#define OPTION_VAL_7_FR NULL
#define OPTION_VAL_8_FR NULL
#define OPTION_VAL_9_FR NULL
#define OPTION_VAL_10_FR NULL
#define FCEUMM_APU_1_LABEL_FR NULL
#define FCEUMM_APU_1_LABEL_CAT_FR NULL
#define FCEUMM_APU_1_INFO_0_FR NULL
#define FCEUMM_APU_2_LABEL_FR NULL
#define FCEUMM_APU_2_LABEL_CAT_FR NULL
#define FCEUMM_APU_2_INFO_0_FR NULL
#define FCEUMM_APU_3_LABEL_FR NULL
#define FCEUMM_APU_3_LABEL_CAT_FR NULL
#define FCEUMM_APU_3_INFO_0_FR NULL
#define FCEUMM_APU_4_LABEL_FR NULL
#define FCEUMM_APU_4_LABEL_CAT_FR NULL
#define FCEUMM_APU_4_INFO_0_FR NULL
#define FCEUMM_APU_5_LABEL_FR NULL
#define FCEUMM_APU_5_LABEL_CAT_FR NULL
#define FCEUMM_APU_5_INFO_0_FR NULL
#define FCEUMM_TURBO_ENABLE_LABEL_FR NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_FR NULL
#define OPTION_VAL_NONE_FR NULL
#define OPTION_VAL_PLAYER_1_FR NULL
#define OPTION_VAL_PLAYER_2_FR NULL
#define OPTION_VAL_BOTH_FR NULL
#define FCEUMM_TURBO_DELAY_LABEL_FR NULL
#define FCEUMM_TURBO_DELAY_INFO_0_FR NULL
#define FCEUMM_ZAPPER_MODE_LABEL_FR NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_FR NULL
#define OPTION_VAL_LIGHTGUN_FR NULL
#define OPTION_VAL_TOUCHSCREEN_FR NULL
#define OPTION_VAL_MOUSE_FR NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_FR NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_FR NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_FR NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_FR NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_FR NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_FR NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_FR NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_FR NULL
#define FCEUMM_OVERCLOCKING_LABEL_FR NULL
#define FCEUMM_OVERCLOCKING_INFO_0_FR NULL
#define OPTION_VAL_2X_POSTRENDER_FR NULL
#define OPTION_VAL_2X_VBLANK_FR NULL
#define FCEUMM_RAMSTATE_LABEL_FR NULL
#define FCEUMM_RAMSTATE_INFO_0_FR NULL
#define OPTION_VAL_FILL_FF_FR NULL
#define OPTION_VAL_FILL_00_FR NULL
#define OPTION_VAL_RANDOM_FR NULL

struct retro_core_option_v2_category option_cats_fr[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_FR,
      CATEGORY_SYSTEM_INFO_0_FR
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_FR,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_FR
#else
      CATEGORY_VIDEO_INFO_1_FR
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_FR,
      CATEGORY_AUDIO_INFO_0_FR
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_FR,
      CATEGORY_INPUT_INFO_0_FR
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_FR,
      CATEGORY_HACKS_INFO_0_FR
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_FR,
      CATEGORY_DIP_SWITCH_INFO_0_FR
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_fr[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_FR,
      NULL,
      FCEUMM_REGION_INFO_0_FR,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_FR },
         { "NTSC",  OPTION_VAL_NTSC_FR },
         { "PAL",   OPTION_VAL_PAL_FR },
         { "Dendy", OPTION_VAL_DENDY_FR },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_FR,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_FR,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_FR,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_FR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_FR,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_FR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_FR,
      NULL,
      FCEUMM_ASPECT_INFO_0_FR,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_FR },
         { "4:3",     OPTION_VAL_4_3_FR },
         { "PP",      OPTION_VAL_PP_FR },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_FR,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_FR,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_FR,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_FR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_FR,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_FR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_FR,
      NULL,
      FCEUMM_PALETTE_INFO_0_FR,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_FR },
         { "asqrealc",             OPTION_VAL_ASQREALC_FR },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_FR },
         { "rgb",                  OPTION_VAL_RGB_FR },
         { "yuv-v3",               OPTION_VAL_YUV_V3_FR },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_FR },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_FR },
         { "pal",                  OPTION_VAL_PAL_FR },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_FR },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_FR },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_FR },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_FR },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_FR },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_FR },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_FR },
         { "nescap",               OPTION_VAL_NESCAP_FR },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_FR },
         { "raw",                  OPTION_VAL_RAW_FR },
         { "custom",               OPTION_VAL_CUSTOM_FR },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_FR,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_FR,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_FR },
         { "svideo",     OPTION_VAL_SVIDEO_FR },
         { "rgb",        OPTION_VAL_RGB_O9_FR },
         { "monochrome", OPTION_VAL_MONOCHROME_FR },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_FR,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_FR,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_FR },
         { "High",      OPTION_VAL_HIGH_FR },
         { "Very High", OPTION_VAL_VERY_HIGH_FR },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_FR,
      FCEUMM_SWAPDUTY_LABEL_CAT_FR,
      FCEUMM_SWAPDUTY_INFO_0_FR,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_FR,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_FR,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_FR },
         { "1",  OPTION_VAL_1_FR },
         { "2",  OPTION_VAL_2_FR },
         { "3",  OPTION_VAL_3_FR },
         { "4",  OPTION_VAL_4_FR },
         { "5",  OPTION_VAL_5_FR },
         { "6",  OPTION_VAL_6_FR },
         { "7",  OPTION_VAL_7_FR },
         { "8",  OPTION_VAL_8_FR },
         { "9",  OPTION_VAL_9_FR },
         { "10", OPTION_VAL_10_FR },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_FR,
      FCEUMM_APU_1_LABEL_CAT_FR,
      FCEUMM_APU_1_INFO_0_FR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_FR,
      FCEUMM_APU_2_LABEL_CAT_FR,
      FCEUMM_APU_2_INFO_0_FR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_FR,
      FCEUMM_APU_3_LABEL_CAT_FR,
      FCEUMM_APU_3_INFO_0_FR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_FR,
      FCEUMM_APU_4_LABEL_CAT_FR,
      FCEUMM_APU_4_INFO_0_FR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_FR,
      FCEUMM_APU_5_LABEL_CAT_FR,
      FCEUMM_APU_5_INFO_0_FR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_FR,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_FR,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_FR },
         { "Player 1", OPTION_VAL_PLAYER_1_FR },
         { "Player 2", OPTION_VAL_PLAYER_2_FR },
         { "Both",     OPTION_VAL_BOTH_FR },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_FR,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_FR,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_FR,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_FR,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_FR },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_FR },
         { "mouse",       OPTION_VAL_MOUSE_FR },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_FR,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_FR,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_FR,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_FR,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_FR,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_FR,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_FR,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_FR,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_FR,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_FR,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_FR },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_FR },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_FR,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_FR,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_FR },
         { "fill $00", OPTION_VAL_FILL_00_FR },
         { "random",   OPTION_VAL_RANDOM_FR },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_fr = {
   option_cats_fr,
   option_defs_fr
};

/* RETRO_LANGUAGE_GL */

#define CATEGORY_SYSTEM_LABEL_GL NULL
#define CATEGORY_SYSTEM_INFO_0_GL NULL
#define CATEGORY_VIDEO_LABEL_GL "Vídeo"
#define CATEGORY_VIDEO_INFO_0_GL NULL
#define CATEGORY_VIDEO_INFO_1_GL NULL
#define CATEGORY_AUDIO_LABEL_GL "Son"
#define CATEGORY_AUDIO_INFO_0_GL NULL
#define CATEGORY_INPUT_LABEL_GL "Entrada"
#define CATEGORY_INPUT_INFO_0_GL NULL
#define CATEGORY_HACKS_LABEL_GL "Hacks de emulación"
#define CATEGORY_HACKS_INFO_0_GL "Configurar os parámetros do overclocking e a precisión da emulación do procesador que afecten ó rendemento e á compatibilidade de baixo nivel."
#define CATEGORY_DIP_SWITCH_LABEL_GL NULL
#define CATEGORY_DIP_SWITCH_INFO_0_GL NULL
#define FCEUMM_REGION_LABEL_GL NULL
#define FCEUMM_REGION_INFO_0_GL NULL
#define OPTION_VAL_AUTO_GL NULL
#define OPTION_VAL_NTSC_GL NULL
#define OPTION_VAL_PAL_GL NULL
#define OPTION_VAL_DENDY_GL NULL
#define FCEUMM_GAME_GENIE_LABEL_GL NULL
#define FCEUMM_GAME_GENIE_INFO_0_GL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_GL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_GL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_GL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_GL NULL
#define FCEUMM_ASPECT_LABEL_GL NULL
#define FCEUMM_ASPECT_INFO_0_GL NULL
#define OPTION_VAL_8_7_PAR_GL NULL
#define OPTION_VAL_4_3_GL NULL
#define OPTION_VAL_PP_GL NULL
#define FCEUMM_OVERSCAN_LABEL_GL NULL
#define FCEUMM_OVERSCAN_INFO_0_GL NULL
#define FCEUMM_OVERSCAN_H_LABEL_GL NULL
#define FCEUMM_OVERSCAN_H_INFO_0_GL NULL
#define FCEUMM_OVERSCAN_V_LABEL_GL NULL
#define FCEUMM_OVERSCAN_V_INFO_0_GL NULL
#define FCEUMM_PALETTE_LABEL_GL NULL
#define FCEUMM_PALETTE_INFO_0_GL NULL
#define OPTION_VAL_DEFAULT_GL NULL
#define OPTION_VAL_ASQREALC_GL NULL
#define OPTION_VAL_NINTENDO_VC_GL NULL
#define OPTION_VAL_RGB_GL NULL
#define OPTION_VAL_YUV_V3_GL NULL
#define OPTION_VAL_UNSATURATED_FINAL_GL NULL
#define OPTION_VAL_SONY_CXA2025AS_US_GL NULL
#define OPTION_VAL_BMF_FINAL2_GL NULL
#define OPTION_VAL_BMF_FINAL3_GL NULL
#define OPTION_VAL_SMOOTH_FBX_GL NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_GL NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_GL NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_GL NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_GL NULL
#define OPTION_VAL_NESCAP_GL NULL
#define OPTION_VAL_WAVEBEAM_GL NULL
#define OPTION_VAL_RAW_GL NULL
#define OPTION_VAL_CUSTOM_GL NULL
#define FCEUMM_NTSC_FILTER_LABEL_GL NULL
#define FCEUMM_NTSC_FILTER_INFO_0_GL NULL
#define OPTION_VAL_COMPOSITE_GL NULL
#define OPTION_VAL_SVIDEO_GL NULL
#define OPTION_VAL_RGB_O9_GL NULL
#define OPTION_VAL_MONOCHROME_GL NULL
#define FCEUMM_SNDQUALITY_LABEL_GL NULL
#define FCEUMM_SNDQUALITY_INFO_0_GL NULL
#define OPTION_VAL_LOW_GL NULL
#define OPTION_VAL_HIGH_GL NULL
#define OPTION_VAL_VERY_HIGH_GL NULL
#define FCEUMM_SWAPDUTY_LABEL_GL NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_GL NULL
#define FCEUMM_SWAPDUTY_INFO_0_GL NULL
#define FCEUMM_SNDVOLUME_LABEL_GL NULL
#define FCEUMM_SNDVOLUME_INFO_0_GL NULL
#define OPTION_VAL_0_GL NULL
#define OPTION_VAL_1_GL NULL
#define OPTION_VAL_2_GL NULL
#define OPTION_VAL_3_GL NULL
#define OPTION_VAL_4_GL NULL
#define OPTION_VAL_5_GL NULL
#define OPTION_VAL_6_GL NULL
#define OPTION_VAL_7_GL NULL
#define OPTION_VAL_8_GL NULL
#define OPTION_VAL_9_GL NULL
#define OPTION_VAL_10_GL NULL
#define FCEUMM_APU_1_LABEL_GL NULL
#define FCEUMM_APU_1_LABEL_CAT_GL NULL
#define FCEUMM_APU_1_INFO_0_GL NULL
#define FCEUMM_APU_2_LABEL_GL NULL
#define FCEUMM_APU_2_LABEL_CAT_GL NULL
#define FCEUMM_APU_2_INFO_0_GL NULL
#define FCEUMM_APU_3_LABEL_GL NULL
#define FCEUMM_APU_3_LABEL_CAT_GL NULL
#define FCEUMM_APU_3_INFO_0_GL NULL
#define FCEUMM_APU_4_LABEL_GL NULL
#define FCEUMM_APU_4_LABEL_CAT_GL NULL
#define FCEUMM_APU_4_INFO_0_GL NULL
#define FCEUMM_APU_5_LABEL_GL NULL
#define FCEUMM_APU_5_LABEL_CAT_GL NULL
#define FCEUMM_APU_5_INFO_0_GL NULL
#define FCEUMM_TURBO_ENABLE_LABEL_GL NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_GL NULL
#define OPTION_VAL_NONE_GL NULL
#define OPTION_VAL_PLAYER_1_GL NULL
#define OPTION_VAL_PLAYER_2_GL NULL
#define OPTION_VAL_BOTH_GL NULL
#define FCEUMM_TURBO_DELAY_LABEL_GL NULL
#define FCEUMM_TURBO_DELAY_INFO_0_GL NULL
#define FCEUMM_ZAPPER_MODE_LABEL_GL NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_GL NULL
#define OPTION_VAL_LIGHTGUN_GL NULL
#define OPTION_VAL_TOUCHSCREEN_GL NULL
#define OPTION_VAL_MOUSE_GL NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_GL NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_GL NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_GL NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_GL NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_GL NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_GL NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_GL NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_GL NULL
#define FCEUMM_OVERCLOCKING_LABEL_GL NULL
#define FCEUMM_OVERCLOCKING_INFO_0_GL NULL
#define OPTION_VAL_2X_POSTRENDER_GL NULL
#define OPTION_VAL_2X_VBLANK_GL NULL
#define FCEUMM_RAMSTATE_LABEL_GL NULL
#define FCEUMM_RAMSTATE_INFO_0_GL NULL
#define OPTION_VAL_FILL_FF_GL NULL
#define OPTION_VAL_FILL_00_GL NULL
#define OPTION_VAL_RANDOM_GL NULL

struct retro_core_option_v2_category option_cats_gl[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_GL,
      CATEGORY_SYSTEM_INFO_0_GL
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_GL,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_GL
#else
      CATEGORY_VIDEO_INFO_1_GL
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_GL,
      CATEGORY_AUDIO_INFO_0_GL
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_GL,
      CATEGORY_INPUT_INFO_0_GL
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_GL,
      CATEGORY_HACKS_INFO_0_GL
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_GL,
      CATEGORY_DIP_SWITCH_INFO_0_GL
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_gl[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_GL,
      NULL,
      FCEUMM_REGION_INFO_0_GL,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_GL },
         { "NTSC",  OPTION_VAL_NTSC_GL },
         { "PAL",   OPTION_VAL_PAL_GL },
         { "Dendy", OPTION_VAL_DENDY_GL },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_GL,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_GL,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_GL,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_GL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_GL,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_GL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_GL,
      NULL,
      FCEUMM_ASPECT_INFO_0_GL,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_GL },
         { "4:3",     OPTION_VAL_4_3_GL },
         { "PP",      OPTION_VAL_PP_GL },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_GL,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_GL,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_GL,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_GL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_GL,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_GL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_GL,
      NULL,
      FCEUMM_PALETTE_INFO_0_GL,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_GL },
         { "asqrealc",             OPTION_VAL_ASQREALC_GL },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_GL },
         { "rgb",                  OPTION_VAL_RGB_GL },
         { "yuv-v3",               OPTION_VAL_YUV_V3_GL },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_GL },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_GL },
         { "pal",                  OPTION_VAL_PAL_GL },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_GL },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_GL },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_GL },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_GL },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_GL },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_GL },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_GL },
         { "nescap",               OPTION_VAL_NESCAP_GL },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_GL },
         { "raw",                  OPTION_VAL_RAW_GL },
         { "custom",               OPTION_VAL_CUSTOM_GL },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_GL,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_GL,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_GL },
         { "svideo",     OPTION_VAL_SVIDEO_GL },
         { "rgb",        OPTION_VAL_RGB_O9_GL },
         { "monochrome", OPTION_VAL_MONOCHROME_GL },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_GL,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_GL,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_GL },
         { "High",      OPTION_VAL_HIGH_GL },
         { "Very High", OPTION_VAL_VERY_HIGH_GL },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_GL,
      FCEUMM_SWAPDUTY_LABEL_CAT_GL,
      FCEUMM_SWAPDUTY_INFO_0_GL,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_GL,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_GL,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_GL },
         { "1",  OPTION_VAL_1_GL },
         { "2",  OPTION_VAL_2_GL },
         { "3",  OPTION_VAL_3_GL },
         { "4",  OPTION_VAL_4_GL },
         { "5",  OPTION_VAL_5_GL },
         { "6",  OPTION_VAL_6_GL },
         { "7",  OPTION_VAL_7_GL },
         { "8",  OPTION_VAL_8_GL },
         { "9",  OPTION_VAL_9_GL },
         { "10", OPTION_VAL_10_GL },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_GL,
      FCEUMM_APU_1_LABEL_CAT_GL,
      FCEUMM_APU_1_INFO_0_GL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_GL,
      FCEUMM_APU_2_LABEL_CAT_GL,
      FCEUMM_APU_2_INFO_0_GL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_GL,
      FCEUMM_APU_3_LABEL_CAT_GL,
      FCEUMM_APU_3_INFO_0_GL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_GL,
      FCEUMM_APU_4_LABEL_CAT_GL,
      FCEUMM_APU_4_INFO_0_GL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_GL,
      FCEUMM_APU_5_LABEL_CAT_GL,
      FCEUMM_APU_5_INFO_0_GL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_GL,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_GL,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_GL },
         { "Player 1", OPTION_VAL_PLAYER_1_GL },
         { "Player 2", OPTION_VAL_PLAYER_2_GL },
         { "Both",     OPTION_VAL_BOTH_GL },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_GL,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_GL,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_GL,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_GL,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_GL },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_GL },
         { "mouse",       OPTION_VAL_MOUSE_GL },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_GL,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_GL,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_GL,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_GL,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_GL,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_GL,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_GL,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_GL,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_GL,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_GL,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_GL },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_GL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_GL,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_GL,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_GL },
         { "fill $00", OPTION_VAL_FILL_00_GL },
         { "random",   OPTION_VAL_RANDOM_GL },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_gl = {
   option_cats_gl,
   option_defs_gl
};

/* RETRO_LANGUAGE_HE */

#define CATEGORY_SYSTEM_LABEL_HE NULL
#define CATEGORY_SYSTEM_INFO_0_HE NULL
#define CATEGORY_VIDEO_LABEL_HE "וידאו"
#define CATEGORY_VIDEO_INFO_0_HE NULL
#define CATEGORY_VIDEO_INFO_1_HE NULL
#define CATEGORY_AUDIO_LABEL_HE "שמע"
#define CATEGORY_AUDIO_INFO_0_HE NULL
#define CATEGORY_INPUT_LABEL_HE "קלט"
#define CATEGORY_INPUT_INFO_0_HE NULL
#define CATEGORY_HACKS_LABEL_HE NULL
#define CATEGORY_HACKS_INFO_0_HE NULL
#define CATEGORY_DIP_SWITCH_LABEL_HE NULL
#define CATEGORY_DIP_SWITCH_INFO_0_HE NULL
#define FCEUMM_REGION_LABEL_HE NULL
#define FCEUMM_REGION_INFO_0_HE NULL
#define OPTION_VAL_AUTO_HE NULL
#define OPTION_VAL_NTSC_HE NULL
#define OPTION_VAL_PAL_HE NULL
#define OPTION_VAL_DENDY_HE NULL
#define FCEUMM_GAME_GENIE_LABEL_HE NULL
#define FCEUMM_GAME_GENIE_INFO_0_HE NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_HE NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_HE NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_HE NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_HE NULL
#define FCEUMM_ASPECT_LABEL_HE NULL
#define FCEUMM_ASPECT_INFO_0_HE NULL
#define OPTION_VAL_8_7_PAR_HE NULL
#define OPTION_VAL_4_3_HE NULL
#define OPTION_VAL_PP_HE NULL
#define FCEUMM_OVERSCAN_LABEL_HE NULL
#define FCEUMM_OVERSCAN_INFO_0_HE NULL
#define FCEUMM_OVERSCAN_H_LABEL_HE NULL
#define FCEUMM_OVERSCAN_H_INFO_0_HE NULL
#define FCEUMM_OVERSCAN_V_LABEL_HE NULL
#define FCEUMM_OVERSCAN_V_INFO_0_HE NULL
#define FCEUMM_PALETTE_LABEL_HE NULL
#define FCEUMM_PALETTE_INFO_0_HE NULL
#define OPTION_VAL_DEFAULT_HE NULL
#define OPTION_VAL_ASQREALC_HE NULL
#define OPTION_VAL_NINTENDO_VC_HE NULL
#define OPTION_VAL_RGB_HE NULL
#define OPTION_VAL_YUV_V3_HE NULL
#define OPTION_VAL_UNSATURATED_FINAL_HE NULL
#define OPTION_VAL_SONY_CXA2025AS_US_HE NULL
#define OPTION_VAL_BMF_FINAL2_HE NULL
#define OPTION_VAL_BMF_FINAL3_HE NULL
#define OPTION_VAL_SMOOTH_FBX_HE NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_HE NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_HE NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_HE NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_HE NULL
#define OPTION_VAL_NESCAP_HE NULL
#define OPTION_VAL_WAVEBEAM_HE NULL
#define OPTION_VAL_RAW_HE NULL
#define OPTION_VAL_CUSTOM_HE NULL
#define FCEUMM_NTSC_FILTER_LABEL_HE NULL
#define FCEUMM_NTSC_FILTER_INFO_0_HE NULL
#define OPTION_VAL_COMPOSITE_HE NULL
#define OPTION_VAL_SVIDEO_HE NULL
#define OPTION_VAL_RGB_O9_HE NULL
#define OPTION_VAL_MONOCHROME_HE NULL
#define FCEUMM_SNDQUALITY_LABEL_HE NULL
#define FCEUMM_SNDQUALITY_INFO_0_HE NULL
#define OPTION_VAL_LOW_HE NULL
#define OPTION_VAL_HIGH_HE NULL
#define OPTION_VAL_VERY_HIGH_HE NULL
#define FCEUMM_SWAPDUTY_LABEL_HE NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_HE NULL
#define FCEUMM_SWAPDUTY_INFO_0_HE NULL
#define FCEUMM_SNDVOLUME_LABEL_HE NULL
#define FCEUMM_SNDVOLUME_INFO_0_HE NULL
#define OPTION_VAL_0_HE NULL
#define OPTION_VAL_1_HE NULL
#define OPTION_VAL_2_HE NULL
#define OPTION_VAL_3_HE NULL
#define OPTION_VAL_4_HE NULL
#define OPTION_VAL_5_HE NULL
#define OPTION_VAL_6_HE NULL
#define OPTION_VAL_7_HE NULL
#define OPTION_VAL_8_HE NULL
#define OPTION_VAL_9_HE NULL
#define OPTION_VAL_10_HE NULL
#define FCEUMM_APU_1_LABEL_HE NULL
#define FCEUMM_APU_1_LABEL_CAT_HE NULL
#define FCEUMM_APU_1_INFO_0_HE NULL
#define FCEUMM_APU_2_LABEL_HE NULL
#define FCEUMM_APU_2_LABEL_CAT_HE NULL
#define FCEUMM_APU_2_INFO_0_HE NULL
#define FCEUMM_APU_3_LABEL_HE NULL
#define FCEUMM_APU_3_LABEL_CAT_HE NULL
#define FCEUMM_APU_3_INFO_0_HE NULL
#define FCEUMM_APU_4_LABEL_HE NULL
#define FCEUMM_APU_4_LABEL_CAT_HE NULL
#define FCEUMM_APU_4_INFO_0_HE NULL
#define FCEUMM_APU_5_LABEL_HE NULL
#define FCEUMM_APU_5_LABEL_CAT_HE NULL
#define FCEUMM_APU_5_INFO_0_HE NULL
#define FCEUMM_TURBO_ENABLE_LABEL_HE NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_HE NULL
#define OPTION_VAL_NONE_HE NULL
#define OPTION_VAL_PLAYER_1_HE NULL
#define OPTION_VAL_PLAYER_2_HE NULL
#define OPTION_VAL_BOTH_HE NULL
#define FCEUMM_TURBO_DELAY_LABEL_HE NULL
#define FCEUMM_TURBO_DELAY_INFO_0_HE NULL
#define FCEUMM_ZAPPER_MODE_LABEL_HE NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_HE NULL
#define OPTION_VAL_LIGHTGUN_HE NULL
#define OPTION_VAL_TOUCHSCREEN_HE NULL
#define OPTION_VAL_MOUSE_HE NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_HE NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_HE NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_HE NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_HE NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_HE NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_HE NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_HE NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_HE NULL
#define FCEUMM_OVERCLOCKING_LABEL_HE NULL
#define FCEUMM_OVERCLOCKING_INFO_0_HE NULL
#define OPTION_VAL_2X_POSTRENDER_HE NULL
#define OPTION_VAL_2X_VBLANK_HE NULL
#define FCEUMM_RAMSTATE_LABEL_HE NULL
#define FCEUMM_RAMSTATE_INFO_0_HE NULL
#define OPTION_VAL_FILL_FF_HE NULL
#define OPTION_VAL_FILL_00_HE NULL
#define OPTION_VAL_RANDOM_HE NULL

struct retro_core_option_v2_category option_cats_he[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_HE,
      CATEGORY_SYSTEM_INFO_0_HE
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_HE,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_HE
#else
      CATEGORY_VIDEO_INFO_1_HE
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_HE,
      CATEGORY_AUDIO_INFO_0_HE
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_HE,
      CATEGORY_INPUT_INFO_0_HE
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_HE,
      CATEGORY_HACKS_INFO_0_HE
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_HE,
      CATEGORY_DIP_SWITCH_INFO_0_HE
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_he[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_HE,
      NULL,
      FCEUMM_REGION_INFO_0_HE,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_HE },
         { "NTSC",  OPTION_VAL_NTSC_HE },
         { "PAL",   OPTION_VAL_PAL_HE },
         { "Dendy", OPTION_VAL_DENDY_HE },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_HE,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_HE,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_HE,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_HE,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_HE,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_HE,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_HE,
      NULL,
      FCEUMM_ASPECT_INFO_0_HE,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_HE },
         { "4:3",     OPTION_VAL_4_3_HE },
         { "PP",      OPTION_VAL_PP_HE },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_HE,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_HE,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_HE,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_HE,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_HE,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_HE,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_HE,
      NULL,
      FCEUMM_PALETTE_INFO_0_HE,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_HE },
         { "asqrealc",             OPTION_VAL_ASQREALC_HE },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_HE },
         { "rgb",                  OPTION_VAL_RGB_HE },
         { "yuv-v3",               OPTION_VAL_YUV_V3_HE },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_HE },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_HE },
         { "pal",                  OPTION_VAL_PAL_HE },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_HE },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_HE },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_HE },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_HE },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_HE },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_HE },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_HE },
         { "nescap",               OPTION_VAL_NESCAP_HE },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_HE },
         { "raw",                  OPTION_VAL_RAW_HE },
         { "custom",               OPTION_VAL_CUSTOM_HE },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_HE,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_HE,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_HE },
         { "svideo",     OPTION_VAL_SVIDEO_HE },
         { "rgb",        OPTION_VAL_RGB_O9_HE },
         { "monochrome", OPTION_VAL_MONOCHROME_HE },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_HE,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_HE,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_HE },
         { "High",      OPTION_VAL_HIGH_HE },
         { "Very High", OPTION_VAL_VERY_HIGH_HE },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_HE,
      FCEUMM_SWAPDUTY_LABEL_CAT_HE,
      FCEUMM_SWAPDUTY_INFO_0_HE,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_HE,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_HE,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_HE },
         { "1",  OPTION_VAL_1_HE },
         { "2",  OPTION_VAL_2_HE },
         { "3",  OPTION_VAL_3_HE },
         { "4",  OPTION_VAL_4_HE },
         { "5",  OPTION_VAL_5_HE },
         { "6",  OPTION_VAL_6_HE },
         { "7",  OPTION_VAL_7_HE },
         { "8",  OPTION_VAL_8_HE },
         { "9",  OPTION_VAL_9_HE },
         { "10", OPTION_VAL_10_HE },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_HE,
      FCEUMM_APU_1_LABEL_CAT_HE,
      FCEUMM_APU_1_INFO_0_HE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_HE,
      FCEUMM_APU_2_LABEL_CAT_HE,
      FCEUMM_APU_2_INFO_0_HE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_HE,
      FCEUMM_APU_3_LABEL_CAT_HE,
      FCEUMM_APU_3_INFO_0_HE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_HE,
      FCEUMM_APU_4_LABEL_CAT_HE,
      FCEUMM_APU_4_INFO_0_HE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_HE,
      FCEUMM_APU_5_LABEL_CAT_HE,
      FCEUMM_APU_5_INFO_0_HE,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_HE,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_HE,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_HE },
         { "Player 1", OPTION_VAL_PLAYER_1_HE },
         { "Player 2", OPTION_VAL_PLAYER_2_HE },
         { "Both",     OPTION_VAL_BOTH_HE },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_HE,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_HE,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_HE,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_HE,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_HE },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_HE },
         { "mouse",       OPTION_VAL_MOUSE_HE },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_HE,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_HE,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_HE,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_HE,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_HE,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_HE,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_HE,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_HE,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_HE,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_HE,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_HE },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_HE },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_HE,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_HE,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_HE },
         { "fill $00", OPTION_VAL_FILL_00_HE },
         { "random",   OPTION_VAL_RANDOM_HE },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_he = {
   option_cats_he,
   option_defs_he
};

/* RETRO_LANGUAGE_HU */

#define CATEGORY_SYSTEM_LABEL_HU NULL
#define CATEGORY_SYSTEM_INFO_0_HU NULL
#define CATEGORY_VIDEO_LABEL_HU "Videó"
#define CATEGORY_VIDEO_INFO_0_HU NULL
#define CATEGORY_VIDEO_INFO_1_HU NULL
#define CATEGORY_AUDIO_LABEL_HU "Hang"
#define CATEGORY_AUDIO_INFO_0_HU NULL
#define CATEGORY_INPUT_LABEL_HU "Bevitel"
#define CATEGORY_INPUT_INFO_0_HU NULL
#define CATEGORY_HACKS_LABEL_HU NULL
#define CATEGORY_HACKS_INFO_0_HU NULL
#define CATEGORY_DIP_SWITCH_LABEL_HU NULL
#define CATEGORY_DIP_SWITCH_INFO_0_HU NULL
#define FCEUMM_REGION_LABEL_HU NULL
#define FCEUMM_REGION_INFO_0_HU NULL
#define OPTION_VAL_AUTO_HU NULL
#define OPTION_VAL_NTSC_HU NULL
#define OPTION_VAL_PAL_HU NULL
#define OPTION_VAL_DENDY_HU NULL
#define FCEUMM_GAME_GENIE_LABEL_HU NULL
#define FCEUMM_GAME_GENIE_INFO_0_HU NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_HU NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_HU NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_HU NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_HU NULL
#define FCEUMM_ASPECT_LABEL_HU NULL
#define FCEUMM_ASPECT_INFO_0_HU NULL
#define OPTION_VAL_8_7_PAR_HU NULL
#define OPTION_VAL_4_3_HU NULL
#define OPTION_VAL_PP_HU NULL
#define FCEUMM_OVERSCAN_LABEL_HU NULL
#define FCEUMM_OVERSCAN_INFO_0_HU NULL
#define FCEUMM_OVERSCAN_H_LABEL_HU NULL
#define FCEUMM_OVERSCAN_H_INFO_0_HU NULL
#define FCEUMM_OVERSCAN_V_LABEL_HU NULL
#define FCEUMM_OVERSCAN_V_INFO_0_HU NULL
#define FCEUMM_PALETTE_LABEL_HU NULL
#define FCEUMM_PALETTE_INFO_0_HU NULL
#define OPTION_VAL_DEFAULT_HU NULL
#define OPTION_VAL_ASQREALC_HU NULL
#define OPTION_VAL_NINTENDO_VC_HU NULL
#define OPTION_VAL_RGB_HU NULL
#define OPTION_VAL_YUV_V3_HU NULL
#define OPTION_VAL_UNSATURATED_FINAL_HU NULL
#define OPTION_VAL_SONY_CXA2025AS_US_HU NULL
#define OPTION_VAL_BMF_FINAL2_HU NULL
#define OPTION_VAL_BMF_FINAL3_HU NULL
#define OPTION_VAL_SMOOTH_FBX_HU NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_HU NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_HU NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_HU NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_HU NULL
#define OPTION_VAL_NESCAP_HU NULL
#define OPTION_VAL_WAVEBEAM_HU NULL
#define OPTION_VAL_RAW_HU NULL
#define OPTION_VAL_CUSTOM_HU NULL
#define FCEUMM_NTSC_FILTER_LABEL_HU NULL
#define FCEUMM_NTSC_FILTER_INFO_0_HU NULL
#define OPTION_VAL_COMPOSITE_HU NULL
#define OPTION_VAL_SVIDEO_HU NULL
#define OPTION_VAL_RGB_O9_HU NULL
#define OPTION_VAL_MONOCHROME_HU NULL
#define FCEUMM_SNDQUALITY_LABEL_HU NULL
#define FCEUMM_SNDQUALITY_INFO_0_HU NULL
#define OPTION_VAL_LOW_HU NULL
#define OPTION_VAL_HIGH_HU NULL
#define OPTION_VAL_VERY_HIGH_HU NULL
#define FCEUMM_SWAPDUTY_LABEL_HU NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_HU NULL
#define FCEUMM_SWAPDUTY_INFO_0_HU NULL
#define FCEUMM_SNDVOLUME_LABEL_HU NULL
#define FCEUMM_SNDVOLUME_INFO_0_HU NULL
#define OPTION_VAL_0_HU NULL
#define OPTION_VAL_1_HU NULL
#define OPTION_VAL_2_HU NULL
#define OPTION_VAL_3_HU NULL
#define OPTION_VAL_4_HU NULL
#define OPTION_VAL_5_HU NULL
#define OPTION_VAL_6_HU NULL
#define OPTION_VAL_7_HU NULL
#define OPTION_VAL_8_HU NULL
#define OPTION_VAL_9_HU NULL
#define OPTION_VAL_10_HU NULL
#define FCEUMM_APU_1_LABEL_HU NULL
#define FCEUMM_APU_1_LABEL_CAT_HU NULL
#define FCEUMM_APU_1_INFO_0_HU NULL
#define FCEUMM_APU_2_LABEL_HU NULL
#define FCEUMM_APU_2_LABEL_CAT_HU NULL
#define FCEUMM_APU_2_INFO_0_HU NULL
#define FCEUMM_APU_3_LABEL_HU NULL
#define FCEUMM_APU_3_LABEL_CAT_HU NULL
#define FCEUMM_APU_3_INFO_0_HU NULL
#define FCEUMM_APU_4_LABEL_HU NULL
#define FCEUMM_APU_4_LABEL_CAT_HU NULL
#define FCEUMM_APU_4_INFO_0_HU NULL
#define FCEUMM_APU_5_LABEL_HU NULL
#define FCEUMM_APU_5_LABEL_CAT_HU NULL
#define FCEUMM_APU_5_INFO_0_HU NULL
#define FCEUMM_TURBO_ENABLE_LABEL_HU NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_HU NULL
#define OPTION_VAL_NONE_HU NULL
#define OPTION_VAL_PLAYER_1_HU NULL
#define OPTION_VAL_PLAYER_2_HU NULL
#define OPTION_VAL_BOTH_HU NULL
#define FCEUMM_TURBO_DELAY_LABEL_HU NULL
#define FCEUMM_TURBO_DELAY_INFO_0_HU NULL
#define FCEUMM_ZAPPER_MODE_LABEL_HU NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_HU NULL
#define OPTION_VAL_LIGHTGUN_HU NULL
#define OPTION_VAL_TOUCHSCREEN_HU NULL
#define OPTION_VAL_MOUSE_HU NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_HU NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_HU NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_HU NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_HU NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_HU NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_HU NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_HU NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_HU NULL
#define FCEUMM_OVERCLOCKING_LABEL_HU NULL
#define FCEUMM_OVERCLOCKING_INFO_0_HU NULL
#define OPTION_VAL_2X_POSTRENDER_HU NULL
#define OPTION_VAL_2X_VBLANK_HU NULL
#define FCEUMM_RAMSTATE_LABEL_HU NULL
#define FCEUMM_RAMSTATE_INFO_0_HU NULL
#define OPTION_VAL_FILL_FF_HU NULL
#define OPTION_VAL_FILL_00_HU NULL
#define OPTION_VAL_RANDOM_HU NULL

struct retro_core_option_v2_category option_cats_hu[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_HU,
      CATEGORY_SYSTEM_INFO_0_HU
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_HU,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_HU
#else
      CATEGORY_VIDEO_INFO_1_HU
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_HU,
      CATEGORY_AUDIO_INFO_0_HU
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_HU,
      CATEGORY_INPUT_INFO_0_HU
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_HU,
      CATEGORY_HACKS_INFO_0_HU
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_HU,
      CATEGORY_DIP_SWITCH_INFO_0_HU
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_hu[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_HU,
      NULL,
      FCEUMM_REGION_INFO_0_HU,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_HU },
         { "NTSC",  OPTION_VAL_NTSC_HU },
         { "PAL",   OPTION_VAL_PAL_HU },
         { "Dendy", OPTION_VAL_DENDY_HU },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_HU,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_HU,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_HU,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_HU,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_HU,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_HU,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_HU,
      NULL,
      FCEUMM_ASPECT_INFO_0_HU,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_HU },
         { "4:3",     OPTION_VAL_4_3_HU },
         { "PP",      OPTION_VAL_PP_HU },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_HU,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_HU,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_HU,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_HU,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_HU,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_HU,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_HU,
      NULL,
      FCEUMM_PALETTE_INFO_0_HU,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_HU },
         { "asqrealc",             OPTION_VAL_ASQREALC_HU },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_HU },
         { "rgb",                  OPTION_VAL_RGB_HU },
         { "yuv-v3",               OPTION_VAL_YUV_V3_HU },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_HU },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_HU },
         { "pal",                  OPTION_VAL_PAL_HU },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_HU },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_HU },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_HU },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_HU },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_HU },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_HU },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_HU },
         { "nescap",               OPTION_VAL_NESCAP_HU },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_HU },
         { "raw",                  OPTION_VAL_RAW_HU },
         { "custom",               OPTION_VAL_CUSTOM_HU },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_HU,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_HU,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_HU },
         { "svideo",     OPTION_VAL_SVIDEO_HU },
         { "rgb",        OPTION_VAL_RGB_O9_HU },
         { "monochrome", OPTION_VAL_MONOCHROME_HU },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_HU,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_HU,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_HU },
         { "High",      OPTION_VAL_HIGH_HU },
         { "Very High", OPTION_VAL_VERY_HIGH_HU },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_HU,
      FCEUMM_SWAPDUTY_LABEL_CAT_HU,
      FCEUMM_SWAPDUTY_INFO_0_HU,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_HU,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_HU,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_HU },
         { "1",  OPTION_VAL_1_HU },
         { "2",  OPTION_VAL_2_HU },
         { "3",  OPTION_VAL_3_HU },
         { "4",  OPTION_VAL_4_HU },
         { "5",  OPTION_VAL_5_HU },
         { "6",  OPTION_VAL_6_HU },
         { "7",  OPTION_VAL_7_HU },
         { "8",  OPTION_VAL_8_HU },
         { "9",  OPTION_VAL_9_HU },
         { "10", OPTION_VAL_10_HU },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_HU,
      FCEUMM_APU_1_LABEL_CAT_HU,
      FCEUMM_APU_1_INFO_0_HU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_HU,
      FCEUMM_APU_2_LABEL_CAT_HU,
      FCEUMM_APU_2_INFO_0_HU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_HU,
      FCEUMM_APU_3_LABEL_CAT_HU,
      FCEUMM_APU_3_INFO_0_HU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_HU,
      FCEUMM_APU_4_LABEL_CAT_HU,
      FCEUMM_APU_4_INFO_0_HU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_HU,
      FCEUMM_APU_5_LABEL_CAT_HU,
      FCEUMM_APU_5_INFO_0_HU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_HU,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_HU,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_HU },
         { "Player 1", OPTION_VAL_PLAYER_1_HU },
         { "Player 2", OPTION_VAL_PLAYER_2_HU },
         { "Both",     OPTION_VAL_BOTH_HU },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_HU,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_HU,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_HU,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_HU,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_HU },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_HU },
         { "mouse",       OPTION_VAL_MOUSE_HU },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_HU,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_HU,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_HU,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_HU,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_HU,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_HU,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_HU,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_HU,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_HU,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_HU,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_HU },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_HU },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_HU,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_HU,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_HU },
         { "fill $00", OPTION_VAL_FILL_00_HU },
         { "random",   OPTION_VAL_RANDOM_HU },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_hu = {
   option_cats_hu,
   option_defs_hu
};

/* RETRO_LANGUAGE_ID */

#define CATEGORY_SYSTEM_LABEL_ID NULL
#define CATEGORY_SYSTEM_INFO_0_ID NULL
#define CATEGORY_VIDEO_LABEL_ID NULL
#define CATEGORY_VIDEO_INFO_0_ID NULL
#define CATEGORY_VIDEO_INFO_1_ID NULL
#define CATEGORY_AUDIO_LABEL_ID "Suara"
#define CATEGORY_AUDIO_INFO_0_ID NULL
#define CATEGORY_INPUT_LABEL_ID "Masukan"
#define CATEGORY_INPUT_INFO_0_ID NULL
#define CATEGORY_HACKS_LABEL_ID NULL
#define CATEGORY_HACKS_INFO_0_ID NULL
#define CATEGORY_DIP_SWITCH_LABEL_ID NULL
#define CATEGORY_DIP_SWITCH_INFO_0_ID NULL
#define FCEUMM_REGION_LABEL_ID NULL
#define FCEUMM_REGION_INFO_0_ID NULL
#define OPTION_VAL_AUTO_ID NULL
#define OPTION_VAL_NTSC_ID NULL
#define OPTION_VAL_PAL_ID NULL
#define OPTION_VAL_DENDY_ID NULL
#define FCEUMM_GAME_GENIE_LABEL_ID NULL
#define FCEUMM_GAME_GENIE_INFO_0_ID NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_ID NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_ID NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_ID NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_ID NULL
#define FCEUMM_ASPECT_LABEL_ID NULL
#define FCEUMM_ASPECT_INFO_0_ID NULL
#define OPTION_VAL_8_7_PAR_ID NULL
#define OPTION_VAL_4_3_ID NULL
#define OPTION_VAL_PP_ID NULL
#define FCEUMM_OVERSCAN_LABEL_ID NULL
#define FCEUMM_OVERSCAN_INFO_0_ID NULL
#define FCEUMM_OVERSCAN_H_LABEL_ID NULL
#define FCEUMM_OVERSCAN_H_INFO_0_ID NULL
#define FCEUMM_OVERSCAN_V_LABEL_ID NULL
#define FCEUMM_OVERSCAN_V_INFO_0_ID NULL
#define FCEUMM_PALETTE_LABEL_ID NULL
#define FCEUMM_PALETTE_INFO_0_ID NULL
#define OPTION_VAL_DEFAULT_ID NULL
#define OPTION_VAL_ASQREALC_ID NULL
#define OPTION_VAL_NINTENDO_VC_ID NULL
#define OPTION_VAL_RGB_ID NULL
#define OPTION_VAL_YUV_V3_ID NULL
#define OPTION_VAL_UNSATURATED_FINAL_ID NULL
#define OPTION_VAL_SONY_CXA2025AS_US_ID NULL
#define OPTION_VAL_BMF_FINAL2_ID NULL
#define OPTION_VAL_BMF_FINAL3_ID NULL
#define OPTION_VAL_SMOOTH_FBX_ID NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_ID NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_ID NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_ID NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_ID NULL
#define OPTION_VAL_NESCAP_ID NULL
#define OPTION_VAL_WAVEBEAM_ID NULL
#define OPTION_VAL_RAW_ID NULL
#define OPTION_VAL_CUSTOM_ID NULL
#define FCEUMM_NTSC_FILTER_LABEL_ID NULL
#define FCEUMM_NTSC_FILTER_INFO_0_ID NULL
#define OPTION_VAL_COMPOSITE_ID NULL
#define OPTION_VAL_SVIDEO_ID NULL
#define OPTION_VAL_RGB_O9_ID NULL
#define OPTION_VAL_MONOCHROME_ID NULL
#define FCEUMM_SNDQUALITY_LABEL_ID NULL
#define FCEUMM_SNDQUALITY_INFO_0_ID NULL
#define OPTION_VAL_LOW_ID NULL
#define OPTION_VAL_HIGH_ID NULL
#define OPTION_VAL_VERY_HIGH_ID NULL
#define FCEUMM_SWAPDUTY_LABEL_ID NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_ID NULL
#define FCEUMM_SWAPDUTY_INFO_0_ID NULL
#define FCEUMM_SNDVOLUME_LABEL_ID NULL
#define FCEUMM_SNDVOLUME_INFO_0_ID NULL
#define OPTION_VAL_0_ID NULL
#define OPTION_VAL_1_ID NULL
#define OPTION_VAL_2_ID NULL
#define OPTION_VAL_3_ID NULL
#define OPTION_VAL_4_ID NULL
#define OPTION_VAL_5_ID NULL
#define OPTION_VAL_6_ID NULL
#define OPTION_VAL_7_ID NULL
#define OPTION_VAL_8_ID NULL
#define OPTION_VAL_9_ID NULL
#define OPTION_VAL_10_ID NULL
#define FCEUMM_APU_1_LABEL_ID NULL
#define FCEUMM_APU_1_LABEL_CAT_ID NULL
#define FCEUMM_APU_1_INFO_0_ID NULL
#define FCEUMM_APU_2_LABEL_ID NULL
#define FCEUMM_APU_2_LABEL_CAT_ID NULL
#define FCEUMM_APU_2_INFO_0_ID NULL
#define FCEUMM_APU_3_LABEL_ID NULL
#define FCEUMM_APU_3_LABEL_CAT_ID NULL
#define FCEUMM_APU_3_INFO_0_ID NULL
#define FCEUMM_APU_4_LABEL_ID NULL
#define FCEUMM_APU_4_LABEL_CAT_ID NULL
#define FCEUMM_APU_4_INFO_0_ID NULL
#define FCEUMM_APU_5_LABEL_ID NULL
#define FCEUMM_APU_5_LABEL_CAT_ID NULL
#define FCEUMM_APU_5_INFO_0_ID NULL
#define FCEUMM_TURBO_ENABLE_LABEL_ID NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_ID NULL
#define OPTION_VAL_NONE_ID NULL
#define OPTION_VAL_PLAYER_1_ID NULL
#define OPTION_VAL_PLAYER_2_ID NULL
#define OPTION_VAL_BOTH_ID NULL
#define FCEUMM_TURBO_DELAY_LABEL_ID NULL
#define FCEUMM_TURBO_DELAY_INFO_0_ID NULL
#define FCEUMM_ZAPPER_MODE_LABEL_ID NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_ID NULL
#define OPTION_VAL_LIGHTGUN_ID NULL
#define OPTION_VAL_TOUCHSCREEN_ID NULL
#define OPTION_VAL_MOUSE_ID NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_ID NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_ID NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_ID NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_ID NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_ID NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_ID NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_ID NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_ID NULL
#define FCEUMM_OVERCLOCKING_LABEL_ID NULL
#define FCEUMM_OVERCLOCKING_INFO_0_ID NULL
#define OPTION_VAL_2X_POSTRENDER_ID NULL
#define OPTION_VAL_2X_VBLANK_ID NULL
#define FCEUMM_RAMSTATE_LABEL_ID NULL
#define FCEUMM_RAMSTATE_INFO_0_ID NULL
#define OPTION_VAL_FILL_FF_ID NULL
#define OPTION_VAL_FILL_00_ID NULL
#define OPTION_VAL_RANDOM_ID NULL

struct retro_core_option_v2_category option_cats_id[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_ID,
      CATEGORY_SYSTEM_INFO_0_ID
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_ID,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_ID
#else
      CATEGORY_VIDEO_INFO_1_ID
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_ID,
      CATEGORY_AUDIO_INFO_0_ID
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_ID,
      CATEGORY_INPUT_INFO_0_ID
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_ID,
      CATEGORY_HACKS_INFO_0_ID
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_ID,
      CATEGORY_DIP_SWITCH_INFO_0_ID
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_id[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_ID,
      NULL,
      FCEUMM_REGION_INFO_0_ID,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_ID },
         { "NTSC",  OPTION_VAL_NTSC_ID },
         { "PAL",   OPTION_VAL_PAL_ID },
         { "Dendy", OPTION_VAL_DENDY_ID },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_ID,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_ID,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_ID,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_ID,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_ID,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_ID,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_ID,
      NULL,
      FCEUMM_ASPECT_INFO_0_ID,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_ID },
         { "4:3",     OPTION_VAL_4_3_ID },
         { "PP",      OPTION_VAL_PP_ID },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_ID,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_ID,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_ID,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_ID,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_ID,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_ID,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_ID,
      NULL,
      FCEUMM_PALETTE_INFO_0_ID,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_ID },
         { "asqrealc",             OPTION_VAL_ASQREALC_ID },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_ID },
         { "rgb",                  OPTION_VAL_RGB_ID },
         { "yuv-v3",               OPTION_VAL_YUV_V3_ID },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_ID },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_ID },
         { "pal",                  OPTION_VAL_PAL_ID },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_ID },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_ID },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_ID },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_ID },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_ID },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_ID },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_ID },
         { "nescap",               OPTION_VAL_NESCAP_ID },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_ID },
         { "raw",                  OPTION_VAL_RAW_ID },
         { "custom",               OPTION_VAL_CUSTOM_ID },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_ID,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_ID,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_ID },
         { "svideo",     OPTION_VAL_SVIDEO_ID },
         { "rgb",        OPTION_VAL_RGB_O9_ID },
         { "monochrome", OPTION_VAL_MONOCHROME_ID },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_ID,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_ID,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_ID },
         { "High",      OPTION_VAL_HIGH_ID },
         { "Very High", OPTION_VAL_VERY_HIGH_ID },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_ID,
      FCEUMM_SWAPDUTY_LABEL_CAT_ID,
      FCEUMM_SWAPDUTY_INFO_0_ID,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_ID,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_ID,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_ID },
         { "1",  OPTION_VAL_1_ID },
         { "2",  OPTION_VAL_2_ID },
         { "3",  OPTION_VAL_3_ID },
         { "4",  OPTION_VAL_4_ID },
         { "5",  OPTION_VAL_5_ID },
         { "6",  OPTION_VAL_6_ID },
         { "7",  OPTION_VAL_7_ID },
         { "8",  OPTION_VAL_8_ID },
         { "9",  OPTION_VAL_9_ID },
         { "10", OPTION_VAL_10_ID },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_ID,
      FCEUMM_APU_1_LABEL_CAT_ID,
      FCEUMM_APU_1_INFO_0_ID,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_ID,
      FCEUMM_APU_2_LABEL_CAT_ID,
      FCEUMM_APU_2_INFO_0_ID,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_ID,
      FCEUMM_APU_3_LABEL_CAT_ID,
      FCEUMM_APU_3_INFO_0_ID,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_ID,
      FCEUMM_APU_4_LABEL_CAT_ID,
      FCEUMM_APU_4_INFO_0_ID,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_ID,
      FCEUMM_APU_5_LABEL_CAT_ID,
      FCEUMM_APU_5_INFO_0_ID,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_ID,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_ID,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_ID },
         { "Player 1", OPTION_VAL_PLAYER_1_ID },
         { "Player 2", OPTION_VAL_PLAYER_2_ID },
         { "Both",     OPTION_VAL_BOTH_ID },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_ID,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_ID,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_ID,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_ID,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_ID },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_ID },
         { "mouse",       OPTION_VAL_MOUSE_ID },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_ID,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_ID,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_ID,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_ID,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_ID,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_ID,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_ID,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_ID,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_ID,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_ID,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_ID },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_ID },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_ID,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_ID,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_ID },
         { "fill $00", OPTION_VAL_FILL_00_ID },
         { "random",   OPTION_VAL_RANDOM_ID },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_id = {
   option_cats_id,
   option_defs_id
};

/* RETRO_LANGUAGE_IT */

#define CATEGORY_SYSTEM_LABEL_IT "Sistema"
#define CATEGORY_SYSTEM_INFO_0_IT "Configurare la regione e i parametri dell'hardware d'espanzione."
#define CATEGORY_VIDEO_LABEL_IT NULL
#define CATEGORY_VIDEO_INFO_0_IT "Configurare le proporzioni, visualizzare il ritaglio, la tavolozza dei colori, le opzioni del filtro video."
#define CATEGORY_VIDEO_INFO_1_IT "Configurare le proporzioni, visualizzare il ritaglio, le opzioni della tavolozza colori."
#define CATEGORY_AUDIO_LABEL_IT NULL
#define CATEGORY_AUDIO_INFO_0_IT "Configura la qualità sonora, il volume, le impostazioni di abilitazione del canale."
#define CATEGORY_INPUT_LABEL_IT NULL
#define CATEGORY_INPUT_INFO_0_IT "Configura i parametri turbo e pistola ottica."
#define CATEGORY_HACKS_LABEL_IT "Hack di Emulazione"
#define CATEGORY_HACKS_INFO_0_IT "Configura i parametri di precisione di overclocking e emulazione del processore che influenzano prestazioni e compatibilità di basso livello."
#define CATEGORY_DIP_SWITCH_LABEL_IT "Interruttori DIP"
#define CATEGORY_DIP_SWITCH_INFO_0_IT "Configura le impostazioni di gioco arcade."
#define FCEUMM_REGION_LABEL_IT NULL
#define FCEUMM_REGION_INFO_0_IT NULL
#define OPTION_VAL_AUTO_IT NULL
#define OPTION_VAL_NTSC_IT NULL
#define OPTION_VAL_PAL_IT NULL
#define OPTION_VAL_DENDY_IT NULL
#define FCEUMM_GAME_GENIE_LABEL_IT NULL
#define FCEUMM_GAME_GENIE_INFO_0_IT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_IT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_IT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_IT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_IT NULL
#define FCEUMM_ASPECT_LABEL_IT NULL
#define FCEUMM_ASPECT_INFO_0_IT NULL
#define OPTION_VAL_8_7_PAR_IT NULL
#define OPTION_VAL_4_3_IT NULL
#define OPTION_VAL_PP_IT NULL
#define FCEUMM_OVERSCAN_LABEL_IT NULL
#define FCEUMM_OVERSCAN_INFO_0_IT NULL
#define FCEUMM_OVERSCAN_H_LABEL_IT NULL
#define FCEUMM_OVERSCAN_H_INFO_0_IT NULL
#define FCEUMM_OVERSCAN_V_LABEL_IT NULL
#define FCEUMM_OVERSCAN_V_INFO_0_IT NULL
#define FCEUMM_PALETTE_LABEL_IT NULL
#define FCEUMM_PALETTE_INFO_0_IT NULL
#define OPTION_VAL_DEFAULT_IT NULL
#define OPTION_VAL_ASQREALC_IT NULL
#define OPTION_VAL_NINTENDO_VC_IT NULL
#define OPTION_VAL_RGB_IT NULL
#define OPTION_VAL_YUV_V3_IT NULL
#define OPTION_VAL_UNSATURATED_FINAL_IT NULL
#define OPTION_VAL_SONY_CXA2025AS_US_IT NULL
#define OPTION_VAL_BMF_FINAL2_IT NULL
#define OPTION_VAL_BMF_FINAL3_IT NULL
#define OPTION_VAL_SMOOTH_FBX_IT NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_IT NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_IT NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_IT NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_IT NULL
#define OPTION_VAL_NESCAP_IT NULL
#define OPTION_VAL_WAVEBEAM_IT NULL
#define OPTION_VAL_RAW_IT NULL
#define OPTION_VAL_CUSTOM_IT NULL
#define FCEUMM_NTSC_FILTER_LABEL_IT NULL
#define FCEUMM_NTSC_FILTER_INFO_0_IT NULL
#define OPTION_VAL_COMPOSITE_IT NULL
#define OPTION_VAL_SVIDEO_IT NULL
#define OPTION_VAL_RGB_O9_IT NULL
#define OPTION_VAL_MONOCHROME_IT NULL
#define FCEUMM_SNDQUALITY_LABEL_IT NULL
#define FCEUMM_SNDQUALITY_INFO_0_IT NULL
#define OPTION_VAL_LOW_IT NULL
#define OPTION_VAL_HIGH_IT NULL
#define OPTION_VAL_VERY_HIGH_IT NULL
#define FCEUMM_SWAPDUTY_LABEL_IT NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_IT NULL
#define FCEUMM_SWAPDUTY_INFO_0_IT NULL
#define FCEUMM_SNDVOLUME_LABEL_IT NULL
#define FCEUMM_SNDVOLUME_INFO_0_IT NULL
#define OPTION_VAL_0_IT NULL
#define OPTION_VAL_1_IT NULL
#define OPTION_VAL_2_IT NULL
#define OPTION_VAL_3_IT NULL
#define OPTION_VAL_4_IT NULL
#define OPTION_VAL_5_IT NULL
#define OPTION_VAL_6_IT NULL
#define OPTION_VAL_7_IT NULL
#define OPTION_VAL_8_IT NULL
#define OPTION_VAL_9_IT NULL
#define OPTION_VAL_10_IT NULL
#define FCEUMM_APU_1_LABEL_IT NULL
#define FCEUMM_APU_1_LABEL_CAT_IT NULL
#define FCEUMM_APU_1_INFO_0_IT NULL
#define FCEUMM_APU_2_LABEL_IT NULL
#define FCEUMM_APU_2_LABEL_CAT_IT NULL
#define FCEUMM_APU_2_INFO_0_IT NULL
#define FCEUMM_APU_3_LABEL_IT NULL
#define FCEUMM_APU_3_LABEL_CAT_IT NULL
#define FCEUMM_APU_3_INFO_0_IT NULL
#define FCEUMM_APU_4_LABEL_IT NULL
#define FCEUMM_APU_4_LABEL_CAT_IT NULL
#define FCEUMM_APU_4_INFO_0_IT NULL
#define FCEUMM_APU_5_LABEL_IT NULL
#define FCEUMM_APU_5_LABEL_CAT_IT NULL
#define FCEUMM_APU_5_INFO_0_IT NULL
#define FCEUMM_TURBO_ENABLE_LABEL_IT NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_IT NULL
#define OPTION_VAL_NONE_IT NULL
#define OPTION_VAL_PLAYER_1_IT NULL
#define OPTION_VAL_PLAYER_2_IT NULL
#define OPTION_VAL_BOTH_IT NULL
#define FCEUMM_TURBO_DELAY_LABEL_IT NULL
#define FCEUMM_TURBO_DELAY_INFO_0_IT NULL
#define FCEUMM_ZAPPER_MODE_LABEL_IT NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_IT NULL
#define OPTION_VAL_LIGHTGUN_IT NULL
#define OPTION_VAL_TOUCHSCREEN_IT NULL
#define OPTION_VAL_MOUSE_IT NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_IT NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_IT NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_IT NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_IT NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_IT NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_IT NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_IT NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_IT NULL
#define FCEUMM_OVERCLOCKING_LABEL_IT NULL
#define FCEUMM_OVERCLOCKING_INFO_0_IT NULL
#define OPTION_VAL_2X_POSTRENDER_IT NULL
#define OPTION_VAL_2X_VBLANK_IT NULL
#define FCEUMM_RAMSTATE_LABEL_IT NULL
#define FCEUMM_RAMSTATE_INFO_0_IT NULL
#define OPTION_VAL_FILL_FF_IT NULL
#define OPTION_VAL_FILL_00_IT NULL
#define OPTION_VAL_RANDOM_IT NULL

struct retro_core_option_v2_category option_cats_it[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_IT,
      CATEGORY_SYSTEM_INFO_0_IT
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_IT,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_IT
#else
      CATEGORY_VIDEO_INFO_1_IT
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_IT,
      CATEGORY_AUDIO_INFO_0_IT
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_IT,
      CATEGORY_INPUT_INFO_0_IT
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_IT,
      CATEGORY_HACKS_INFO_0_IT
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_IT,
      CATEGORY_DIP_SWITCH_INFO_0_IT
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_it[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_IT,
      NULL,
      FCEUMM_REGION_INFO_0_IT,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_IT },
         { "NTSC",  OPTION_VAL_NTSC_IT },
         { "PAL",   OPTION_VAL_PAL_IT },
         { "Dendy", OPTION_VAL_DENDY_IT },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_IT,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_IT,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_IT,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_IT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_IT,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_IT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_IT,
      NULL,
      FCEUMM_ASPECT_INFO_0_IT,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_IT },
         { "4:3",     OPTION_VAL_4_3_IT },
         { "PP",      OPTION_VAL_PP_IT },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_IT,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_IT,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_IT,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_IT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_IT,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_IT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_IT,
      NULL,
      FCEUMM_PALETTE_INFO_0_IT,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_IT },
         { "asqrealc",             OPTION_VAL_ASQREALC_IT },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_IT },
         { "rgb",                  OPTION_VAL_RGB_IT },
         { "yuv-v3",               OPTION_VAL_YUV_V3_IT },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_IT },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_IT },
         { "pal",                  OPTION_VAL_PAL_IT },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_IT },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_IT },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_IT },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_IT },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_IT },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_IT },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_IT },
         { "nescap",               OPTION_VAL_NESCAP_IT },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_IT },
         { "raw",                  OPTION_VAL_RAW_IT },
         { "custom",               OPTION_VAL_CUSTOM_IT },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_IT,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_IT,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_IT },
         { "svideo",     OPTION_VAL_SVIDEO_IT },
         { "rgb",        OPTION_VAL_RGB_O9_IT },
         { "monochrome", OPTION_VAL_MONOCHROME_IT },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_IT,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_IT,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_IT },
         { "High",      OPTION_VAL_HIGH_IT },
         { "Very High", OPTION_VAL_VERY_HIGH_IT },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_IT,
      FCEUMM_SWAPDUTY_LABEL_CAT_IT,
      FCEUMM_SWAPDUTY_INFO_0_IT,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_IT,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_IT,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_IT },
         { "1",  OPTION_VAL_1_IT },
         { "2",  OPTION_VAL_2_IT },
         { "3",  OPTION_VAL_3_IT },
         { "4",  OPTION_VAL_4_IT },
         { "5",  OPTION_VAL_5_IT },
         { "6",  OPTION_VAL_6_IT },
         { "7",  OPTION_VAL_7_IT },
         { "8",  OPTION_VAL_8_IT },
         { "9",  OPTION_VAL_9_IT },
         { "10", OPTION_VAL_10_IT },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_IT,
      FCEUMM_APU_1_LABEL_CAT_IT,
      FCEUMM_APU_1_INFO_0_IT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_IT,
      FCEUMM_APU_2_LABEL_CAT_IT,
      FCEUMM_APU_2_INFO_0_IT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_IT,
      FCEUMM_APU_3_LABEL_CAT_IT,
      FCEUMM_APU_3_INFO_0_IT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_IT,
      FCEUMM_APU_4_LABEL_CAT_IT,
      FCEUMM_APU_4_INFO_0_IT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_IT,
      FCEUMM_APU_5_LABEL_CAT_IT,
      FCEUMM_APU_5_INFO_0_IT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_IT,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_IT,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_IT },
         { "Player 1", OPTION_VAL_PLAYER_1_IT },
         { "Player 2", OPTION_VAL_PLAYER_2_IT },
         { "Both",     OPTION_VAL_BOTH_IT },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_IT,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_IT,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_IT,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_IT,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_IT },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_IT },
         { "mouse",       OPTION_VAL_MOUSE_IT },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_IT,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_IT,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_IT,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_IT,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_IT,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_IT,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_IT,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_IT,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_IT,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_IT,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_IT },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_IT },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_IT,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_IT,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_IT },
         { "fill $00", OPTION_VAL_FILL_00_IT },
         { "random",   OPTION_VAL_RANDOM_IT },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_it = {
   option_cats_it,
   option_defs_it
};

/* RETRO_LANGUAGE_JA */

#define CATEGORY_SYSTEM_LABEL_JA "システム"
#define CATEGORY_SYSTEM_INFO_0_JA NULL
#define CATEGORY_VIDEO_LABEL_JA "ビデオのドライバ"
#define CATEGORY_VIDEO_INFO_0_JA NULL
#define CATEGORY_VIDEO_INFO_1_JA NULL
#define CATEGORY_AUDIO_LABEL_JA "オーディオのドライバ"
#define CATEGORY_AUDIO_INFO_0_JA NULL
#define CATEGORY_INPUT_LABEL_JA "入力"
#define CATEGORY_INPUT_INFO_0_JA NULL
#define CATEGORY_HACKS_LABEL_JA NULL
#define CATEGORY_HACKS_INFO_0_JA NULL
#define CATEGORY_DIP_SWITCH_LABEL_JA NULL
#define CATEGORY_DIP_SWITCH_INFO_0_JA NULL
#define FCEUMM_REGION_LABEL_JA NULL
#define FCEUMM_REGION_INFO_0_JA NULL
#define OPTION_VAL_AUTO_JA NULL
#define OPTION_VAL_NTSC_JA NULL
#define OPTION_VAL_PAL_JA NULL
#define OPTION_VAL_DENDY_JA NULL
#define FCEUMM_GAME_GENIE_LABEL_JA NULL
#define FCEUMM_GAME_GENIE_INFO_0_JA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_JA NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_JA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_JA NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_JA NULL
#define FCEUMM_ASPECT_LABEL_JA NULL
#define FCEUMM_ASPECT_INFO_0_JA NULL
#define OPTION_VAL_8_7_PAR_JA NULL
#define OPTION_VAL_4_3_JA NULL
#define OPTION_VAL_PP_JA NULL
#define FCEUMM_OVERSCAN_LABEL_JA NULL
#define FCEUMM_OVERSCAN_INFO_0_JA NULL
#define FCEUMM_OVERSCAN_H_LABEL_JA NULL
#define FCEUMM_OVERSCAN_H_INFO_0_JA NULL
#define FCEUMM_OVERSCAN_V_LABEL_JA NULL
#define FCEUMM_OVERSCAN_V_INFO_0_JA NULL
#define FCEUMM_PALETTE_LABEL_JA NULL
#define FCEUMM_PALETTE_INFO_0_JA NULL
#define OPTION_VAL_DEFAULT_JA NULL
#define OPTION_VAL_ASQREALC_JA NULL
#define OPTION_VAL_NINTENDO_VC_JA NULL
#define OPTION_VAL_RGB_JA NULL
#define OPTION_VAL_YUV_V3_JA NULL
#define OPTION_VAL_UNSATURATED_FINAL_JA NULL
#define OPTION_VAL_SONY_CXA2025AS_US_JA NULL
#define OPTION_VAL_BMF_FINAL2_JA NULL
#define OPTION_VAL_BMF_FINAL3_JA NULL
#define OPTION_VAL_SMOOTH_FBX_JA NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_JA NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_JA NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_JA NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_JA NULL
#define OPTION_VAL_NESCAP_JA NULL
#define OPTION_VAL_WAVEBEAM_JA NULL
#define OPTION_VAL_RAW_JA NULL
#define OPTION_VAL_CUSTOM_JA NULL
#define FCEUMM_NTSC_FILTER_LABEL_JA NULL
#define FCEUMM_NTSC_FILTER_INFO_0_JA NULL
#define OPTION_VAL_COMPOSITE_JA NULL
#define OPTION_VAL_SVIDEO_JA NULL
#define OPTION_VAL_RGB_O9_JA NULL
#define OPTION_VAL_MONOCHROME_JA NULL
#define FCEUMM_SNDQUALITY_LABEL_JA NULL
#define FCEUMM_SNDQUALITY_INFO_0_JA NULL
#define OPTION_VAL_LOW_JA NULL
#define OPTION_VAL_HIGH_JA NULL
#define OPTION_VAL_VERY_HIGH_JA NULL
#define FCEUMM_SWAPDUTY_LABEL_JA NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_JA NULL
#define FCEUMM_SWAPDUTY_INFO_0_JA NULL
#define FCEUMM_SNDVOLUME_LABEL_JA NULL
#define FCEUMM_SNDVOLUME_INFO_0_JA NULL
#define OPTION_VAL_0_JA NULL
#define OPTION_VAL_1_JA NULL
#define OPTION_VAL_2_JA NULL
#define OPTION_VAL_3_JA NULL
#define OPTION_VAL_4_JA NULL
#define OPTION_VAL_5_JA NULL
#define OPTION_VAL_6_JA NULL
#define OPTION_VAL_7_JA NULL
#define OPTION_VAL_8_JA NULL
#define OPTION_VAL_9_JA NULL
#define OPTION_VAL_10_JA NULL
#define FCEUMM_APU_1_LABEL_JA NULL
#define FCEUMM_APU_1_LABEL_CAT_JA NULL
#define FCEUMM_APU_1_INFO_0_JA NULL
#define FCEUMM_APU_2_LABEL_JA NULL
#define FCEUMM_APU_2_LABEL_CAT_JA NULL
#define FCEUMM_APU_2_INFO_0_JA NULL
#define FCEUMM_APU_3_LABEL_JA NULL
#define FCEUMM_APU_3_LABEL_CAT_JA NULL
#define FCEUMM_APU_3_INFO_0_JA NULL
#define FCEUMM_APU_4_LABEL_JA NULL
#define FCEUMM_APU_4_LABEL_CAT_JA NULL
#define FCEUMM_APU_4_INFO_0_JA NULL
#define FCEUMM_APU_5_LABEL_JA NULL
#define FCEUMM_APU_5_LABEL_CAT_JA NULL
#define FCEUMM_APU_5_INFO_0_JA NULL
#define FCEUMM_TURBO_ENABLE_LABEL_JA NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_JA NULL
#define OPTION_VAL_NONE_JA NULL
#define OPTION_VAL_PLAYER_1_JA NULL
#define OPTION_VAL_PLAYER_2_JA NULL
#define OPTION_VAL_BOTH_JA NULL
#define FCEUMM_TURBO_DELAY_LABEL_JA NULL
#define FCEUMM_TURBO_DELAY_INFO_0_JA NULL
#define FCEUMM_ZAPPER_MODE_LABEL_JA NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_JA NULL
#define OPTION_VAL_LIGHTGUN_JA NULL
#define OPTION_VAL_TOUCHSCREEN_JA NULL
#define OPTION_VAL_MOUSE_JA NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_JA NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_JA NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_JA NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_JA NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_JA NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_JA NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_JA NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_JA NULL
#define FCEUMM_OVERCLOCKING_LABEL_JA NULL
#define FCEUMM_OVERCLOCKING_INFO_0_JA NULL
#define OPTION_VAL_2X_POSTRENDER_JA NULL
#define OPTION_VAL_2X_VBLANK_JA NULL
#define FCEUMM_RAMSTATE_LABEL_JA NULL
#define FCEUMM_RAMSTATE_INFO_0_JA NULL
#define OPTION_VAL_FILL_FF_JA NULL
#define OPTION_VAL_FILL_00_JA NULL
#define OPTION_VAL_RANDOM_JA NULL

struct retro_core_option_v2_category option_cats_ja[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_JA,
      CATEGORY_SYSTEM_INFO_0_JA
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_JA,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_JA
#else
      CATEGORY_VIDEO_INFO_1_JA
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_JA,
      CATEGORY_AUDIO_INFO_0_JA
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_JA,
      CATEGORY_INPUT_INFO_0_JA
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_JA,
      CATEGORY_HACKS_INFO_0_JA
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_JA,
      CATEGORY_DIP_SWITCH_INFO_0_JA
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_ja[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_JA,
      NULL,
      FCEUMM_REGION_INFO_0_JA,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_JA },
         { "NTSC",  OPTION_VAL_NTSC_JA },
         { "PAL",   OPTION_VAL_PAL_JA },
         { "Dendy", OPTION_VAL_DENDY_JA },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_JA,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_JA,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_JA,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_JA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_JA,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_JA,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_JA,
      NULL,
      FCEUMM_ASPECT_INFO_0_JA,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_JA },
         { "4:3",     OPTION_VAL_4_3_JA },
         { "PP",      OPTION_VAL_PP_JA },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_JA,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_JA,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_JA,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_JA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_JA,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_JA,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_JA,
      NULL,
      FCEUMM_PALETTE_INFO_0_JA,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_JA },
         { "asqrealc",             OPTION_VAL_ASQREALC_JA },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_JA },
         { "rgb",                  OPTION_VAL_RGB_JA },
         { "yuv-v3",               OPTION_VAL_YUV_V3_JA },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_JA },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_JA },
         { "pal",                  OPTION_VAL_PAL_JA },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_JA },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_JA },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_JA },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_JA },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_JA },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_JA },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_JA },
         { "nescap",               OPTION_VAL_NESCAP_JA },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_JA },
         { "raw",                  OPTION_VAL_RAW_JA },
         { "custom",               OPTION_VAL_CUSTOM_JA },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_JA,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_JA,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_JA },
         { "svideo",     OPTION_VAL_SVIDEO_JA },
         { "rgb",        OPTION_VAL_RGB_O9_JA },
         { "monochrome", OPTION_VAL_MONOCHROME_JA },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_JA,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_JA,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_JA },
         { "High",      OPTION_VAL_HIGH_JA },
         { "Very High", OPTION_VAL_VERY_HIGH_JA },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_JA,
      FCEUMM_SWAPDUTY_LABEL_CAT_JA,
      FCEUMM_SWAPDUTY_INFO_0_JA,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_JA,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_JA,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_JA },
         { "1",  OPTION_VAL_1_JA },
         { "2",  OPTION_VAL_2_JA },
         { "3",  OPTION_VAL_3_JA },
         { "4",  OPTION_VAL_4_JA },
         { "5",  OPTION_VAL_5_JA },
         { "6",  OPTION_VAL_6_JA },
         { "7",  OPTION_VAL_7_JA },
         { "8",  OPTION_VAL_8_JA },
         { "9",  OPTION_VAL_9_JA },
         { "10", OPTION_VAL_10_JA },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_JA,
      FCEUMM_APU_1_LABEL_CAT_JA,
      FCEUMM_APU_1_INFO_0_JA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_JA,
      FCEUMM_APU_2_LABEL_CAT_JA,
      FCEUMM_APU_2_INFO_0_JA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_JA,
      FCEUMM_APU_3_LABEL_CAT_JA,
      FCEUMM_APU_3_INFO_0_JA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_JA,
      FCEUMM_APU_4_LABEL_CAT_JA,
      FCEUMM_APU_4_INFO_0_JA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_JA,
      FCEUMM_APU_5_LABEL_CAT_JA,
      FCEUMM_APU_5_INFO_0_JA,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_JA,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_JA,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_JA },
         { "Player 1", OPTION_VAL_PLAYER_1_JA },
         { "Player 2", OPTION_VAL_PLAYER_2_JA },
         { "Both",     OPTION_VAL_BOTH_JA },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_JA,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_JA,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_JA,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_JA,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_JA },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_JA },
         { "mouse",       OPTION_VAL_MOUSE_JA },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_JA,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_JA,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_JA,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_JA,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_JA,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_JA,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_JA,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_JA,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_JA,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_JA,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_JA },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_JA },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_JA,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_JA,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_JA },
         { "fill $00", OPTION_VAL_FILL_00_JA },
         { "random",   OPTION_VAL_RANDOM_JA },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_ja = {
   option_cats_ja,
   option_defs_ja
};

/* RETRO_LANGUAGE_KO */

#define CATEGORY_SYSTEM_LABEL_KO "시스템"
#define CATEGORY_SYSTEM_INFO_0_KO NULL
#define CATEGORY_VIDEO_LABEL_KO "비디오"
#define CATEGORY_VIDEO_INFO_0_KO NULL
#define CATEGORY_VIDEO_INFO_1_KO NULL
#define CATEGORY_AUDIO_LABEL_KO "오디오"
#define CATEGORY_AUDIO_INFO_0_KO NULL
#define CATEGORY_INPUT_LABEL_KO "입력"
#define CATEGORY_INPUT_INFO_0_KO NULL
#define CATEGORY_HACKS_LABEL_KO "에뮬레이션 핵"
#define CATEGORY_HACKS_INFO_0_KO "성능과 호환성에 영향을 끼치는 프로세서 오버클러킹 및 에뮬레이션 정확도 옵션 등을 설정합니다."
#define CATEGORY_DIP_SWITCH_LABEL_KO NULL
#define CATEGORY_DIP_SWITCH_INFO_0_KO NULL
#define FCEUMM_REGION_LABEL_KO NULL
#define FCEUMM_REGION_INFO_0_KO NULL
#define OPTION_VAL_AUTO_KO NULL
#define OPTION_VAL_NTSC_KO NULL
#define OPTION_VAL_PAL_KO NULL
#define OPTION_VAL_DENDY_KO NULL
#define FCEUMM_GAME_GENIE_LABEL_KO NULL
#define FCEUMM_GAME_GENIE_INFO_0_KO NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_KO NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_KO NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_KO NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_KO NULL
#define FCEUMM_ASPECT_LABEL_KO NULL
#define FCEUMM_ASPECT_INFO_0_KO NULL
#define OPTION_VAL_8_7_PAR_KO NULL
#define OPTION_VAL_4_3_KO NULL
#define OPTION_VAL_PP_KO NULL
#define FCEUMM_OVERSCAN_LABEL_KO NULL
#define FCEUMM_OVERSCAN_INFO_0_KO NULL
#define FCEUMM_OVERSCAN_H_LABEL_KO NULL
#define FCEUMM_OVERSCAN_H_INFO_0_KO NULL
#define FCEUMM_OVERSCAN_V_LABEL_KO NULL
#define FCEUMM_OVERSCAN_V_INFO_0_KO NULL
#define FCEUMM_PALETTE_LABEL_KO NULL
#define FCEUMM_PALETTE_INFO_0_KO NULL
#define OPTION_VAL_DEFAULT_KO NULL
#define OPTION_VAL_ASQREALC_KO NULL
#define OPTION_VAL_NINTENDO_VC_KO NULL
#define OPTION_VAL_RGB_KO NULL
#define OPTION_VAL_YUV_V3_KO NULL
#define OPTION_VAL_UNSATURATED_FINAL_KO NULL
#define OPTION_VAL_SONY_CXA2025AS_US_KO NULL
#define OPTION_VAL_BMF_FINAL2_KO NULL
#define OPTION_VAL_BMF_FINAL3_KO NULL
#define OPTION_VAL_SMOOTH_FBX_KO NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_KO NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_KO NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_KO NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_KO NULL
#define OPTION_VAL_NESCAP_KO NULL
#define OPTION_VAL_WAVEBEAM_KO NULL
#define OPTION_VAL_RAW_KO NULL
#define OPTION_VAL_CUSTOM_KO NULL
#define FCEUMM_NTSC_FILTER_LABEL_KO NULL
#define FCEUMM_NTSC_FILTER_INFO_0_KO NULL
#define OPTION_VAL_COMPOSITE_KO NULL
#define OPTION_VAL_SVIDEO_KO NULL
#define OPTION_VAL_RGB_O9_KO NULL
#define OPTION_VAL_MONOCHROME_KO NULL
#define FCEUMM_SNDQUALITY_LABEL_KO NULL
#define FCEUMM_SNDQUALITY_INFO_0_KO NULL
#define OPTION_VAL_LOW_KO NULL
#define OPTION_VAL_HIGH_KO NULL
#define OPTION_VAL_VERY_HIGH_KO NULL
#define FCEUMM_SWAPDUTY_LABEL_KO NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_KO NULL
#define FCEUMM_SWAPDUTY_INFO_0_KO NULL
#define FCEUMM_SNDVOLUME_LABEL_KO NULL
#define FCEUMM_SNDVOLUME_INFO_0_KO NULL
#define OPTION_VAL_0_KO NULL
#define OPTION_VAL_1_KO NULL
#define OPTION_VAL_2_KO NULL
#define OPTION_VAL_3_KO NULL
#define OPTION_VAL_4_KO NULL
#define OPTION_VAL_5_KO NULL
#define OPTION_VAL_6_KO NULL
#define OPTION_VAL_7_KO NULL
#define OPTION_VAL_8_KO NULL
#define OPTION_VAL_9_KO NULL
#define OPTION_VAL_10_KO NULL
#define FCEUMM_APU_1_LABEL_KO NULL
#define FCEUMM_APU_1_LABEL_CAT_KO NULL
#define FCEUMM_APU_1_INFO_0_KO NULL
#define FCEUMM_APU_2_LABEL_KO NULL
#define FCEUMM_APU_2_LABEL_CAT_KO NULL
#define FCEUMM_APU_2_INFO_0_KO NULL
#define FCEUMM_APU_3_LABEL_KO NULL
#define FCEUMM_APU_3_LABEL_CAT_KO NULL
#define FCEUMM_APU_3_INFO_0_KO NULL
#define FCEUMM_APU_4_LABEL_KO NULL
#define FCEUMM_APU_4_LABEL_CAT_KO NULL
#define FCEUMM_APU_4_INFO_0_KO NULL
#define FCEUMM_APU_5_LABEL_KO NULL
#define FCEUMM_APU_5_LABEL_CAT_KO NULL
#define FCEUMM_APU_5_INFO_0_KO NULL
#define FCEUMM_TURBO_ENABLE_LABEL_KO NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_KO NULL
#define OPTION_VAL_NONE_KO NULL
#define OPTION_VAL_PLAYER_1_KO NULL
#define OPTION_VAL_PLAYER_2_KO NULL
#define OPTION_VAL_BOTH_KO NULL
#define FCEUMM_TURBO_DELAY_LABEL_KO NULL
#define FCEUMM_TURBO_DELAY_INFO_0_KO NULL
#define FCEUMM_ZAPPER_MODE_LABEL_KO NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_KO NULL
#define OPTION_VAL_LIGHTGUN_KO NULL
#define OPTION_VAL_TOUCHSCREEN_KO NULL
#define OPTION_VAL_MOUSE_KO NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_KO NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_KO NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_KO NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_KO NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_KO NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_KO NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_KO NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_KO NULL
#define FCEUMM_OVERCLOCKING_LABEL_KO NULL
#define FCEUMM_OVERCLOCKING_INFO_0_KO NULL
#define OPTION_VAL_2X_POSTRENDER_KO NULL
#define OPTION_VAL_2X_VBLANK_KO NULL
#define FCEUMM_RAMSTATE_LABEL_KO NULL
#define FCEUMM_RAMSTATE_INFO_0_KO NULL
#define OPTION_VAL_FILL_FF_KO NULL
#define OPTION_VAL_FILL_00_KO NULL
#define OPTION_VAL_RANDOM_KO NULL

struct retro_core_option_v2_category option_cats_ko[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_KO,
      CATEGORY_SYSTEM_INFO_0_KO
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_KO,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_KO
#else
      CATEGORY_VIDEO_INFO_1_KO
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_KO,
      CATEGORY_AUDIO_INFO_0_KO
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_KO,
      CATEGORY_INPUT_INFO_0_KO
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_KO,
      CATEGORY_HACKS_INFO_0_KO
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_KO,
      CATEGORY_DIP_SWITCH_INFO_0_KO
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_ko[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_KO,
      NULL,
      FCEUMM_REGION_INFO_0_KO,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_KO },
         { "NTSC",  OPTION_VAL_NTSC_KO },
         { "PAL",   OPTION_VAL_PAL_KO },
         { "Dendy", OPTION_VAL_DENDY_KO },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_KO,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_KO,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_KO,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_KO,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_KO,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_KO,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_KO,
      NULL,
      FCEUMM_ASPECT_INFO_0_KO,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_KO },
         { "4:3",     OPTION_VAL_4_3_KO },
         { "PP",      OPTION_VAL_PP_KO },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_KO,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_KO,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_KO,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_KO,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_KO,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_KO,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_KO,
      NULL,
      FCEUMM_PALETTE_INFO_0_KO,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_KO },
         { "asqrealc",             OPTION_VAL_ASQREALC_KO },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_KO },
         { "rgb",                  OPTION_VAL_RGB_KO },
         { "yuv-v3",               OPTION_VAL_YUV_V3_KO },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_KO },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_KO },
         { "pal",                  OPTION_VAL_PAL_KO },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_KO },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_KO },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_KO },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_KO },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_KO },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_KO },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_KO },
         { "nescap",               OPTION_VAL_NESCAP_KO },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_KO },
         { "raw",                  OPTION_VAL_RAW_KO },
         { "custom",               OPTION_VAL_CUSTOM_KO },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_KO,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_KO,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_KO },
         { "svideo",     OPTION_VAL_SVIDEO_KO },
         { "rgb",        OPTION_VAL_RGB_O9_KO },
         { "monochrome", OPTION_VAL_MONOCHROME_KO },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_KO,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_KO,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_KO },
         { "High",      OPTION_VAL_HIGH_KO },
         { "Very High", OPTION_VAL_VERY_HIGH_KO },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_KO,
      FCEUMM_SWAPDUTY_LABEL_CAT_KO,
      FCEUMM_SWAPDUTY_INFO_0_KO,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_KO,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_KO,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_KO },
         { "1",  OPTION_VAL_1_KO },
         { "2",  OPTION_VAL_2_KO },
         { "3",  OPTION_VAL_3_KO },
         { "4",  OPTION_VAL_4_KO },
         { "5",  OPTION_VAL_5_KO },
         { "6",  OPTION_VAL_6_KO },
         { "7",  OPTION_VAL_7_KO },
         { "8",  OPTION_VAL_8_KO },
         { "9",  OPTION_VAL_9_KO },
         { "10", OPTION_VAL_10_KO },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_KO,
      FCEUMM_APU_1_LABEL_CAT_KO,
      FCEUMM_APU_1_INFO_0_KO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_KO,
      FCEUMM_APU_2_LABEL_CAT_KO,
      FCEUMM_APU_2_INFO_0_KO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_KO,
      FCEUMM_APU_3_LABEL_CAT_KO,
      FCEUMM_APU_3_INFO_0_KO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_KO,
      FCEUMM_APU_4_LABEL_CAT_KO,
      FCEUMM_APU_4_INFO_0_KO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_KO,
      FCEUMM_APU_5_LABEL_CAT_KO,
      FCEUMM_APU_5_INFO_0_KO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_KO,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_KO,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_KO },
         { "Player 1", OPTION_VAL_PLAYER_1_KO },
         { "Player 2", OPTION_VAL_PLAYER_2_KO },
         { "Both",     OPTION_VAL_BOTH_KO },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_KO,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_KO,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_KO,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_KO,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_KO },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_KO },
         { "mouse",       OPTION_VAL_MOUSE_KO },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_KO,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_KO,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_KO,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_KO,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_KO,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_KO,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_KO,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_KO,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_KO,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_KO,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_KO },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_KO },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_KO,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_KO,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_KO },
         { "fill $00", OPTION_VAL_FILL_00_KO },
         { "random",   OPTION_VAL_RANDOM_KO },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_ko = {
   option_cats_ko,
   option_defs_ko
};

/* RETRO_LANGUAGE_MT */

#define CATEGORY_SYSTEM_LABEL_MT NULL
#define CATEGORY_SYSTEM_INFO_0_MT NULL
#define CATEGORY_VIDEO_LABEL_MT NULL
#define CATEGORY_VIDEO_INFO_0_MT NULL
#define CATEGORY_VIDEO_INFO_1_MT NULL
#define CATEGORY_AUDIO_LABEL_MT NULL
#define CATEGORY_AUDIO_INFO_0_MT NULL
#define CATEGORY_INPUT_LABEL_MT NULL
#define CATEGORY_INPUT_INFO_0_MT NULL
#define CATEGORY_HACKS_LABEL_MT NULL
#define CATEGORY_HACKS_INFO_0_MT NULL
#define CATEGORY_DIP_SWITCH_LABEL_MT NULL
#define CATEGORY_DIP_SWITCH_INFO_0_MT NULL
#define FCEUMM_REGION_LABEL_MT NULL
#define FCEUMM_REGION_INFO_0_MT NULL
#define OPTION_VAL_AUTO_MT NULL
#define OPTION_VAL_NTSC_MT NULL
#define OPTION_VAL_PAL_MT NULL
#define OPTION_VAL_DENDY_MT NULL
#define FCEUMM_GAME_GENIE_LABEL_MT NULL
#define FCEUMM_GAME_GENIE_INFO_0_MT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_MT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_MT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_MT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_MT NULL
#define FCEUMM_ASPECT_LABEL_MT NULL
#define FCEUMM_ASPECT_INFO_0_MT NULL
#define OPTION_VAL_8_7_PAR_MT NULL
#define OPTION_VAL_4_3_MT NULL
#define OPTION_VAL_PP_MT NULL
#define FCEUMM_OVERSCAN_LABEL_MT NULL
#define FCEUMM_OVERSCAN_INFO_0_MT NULL
#define FCEUMM_OVERSCAN_H_LABEL_MT NULL
#define FCEUMM_OVERSCAN_H_INFO_0_MT NULL
#define FCEUMM_OVERSCAN_V_LABEL_MT NULL
#define FCEUMM_OVERSCAN_V_INFO_0_MT NULL
#define FCEUMM_PALETTE_LABEL_MT NULL
#define FCEUMM_PALETTE_INFO_0_MT NULL
#define OPTION_VAL_DEFAULT_MT NULL
#define OPTION_VAL_ASQREALC_MT NULL
#define OPTION_VAL_NINTENDO_VC_MT NULL
#define OPTION_VAL_RGB_MT NULL
#define OPTION_VAL_YUV_V3_MT NULL
#define OPTION_VAL_UNSATURATED_FINAL_MT NULL
#define OPTION_VAL_SONY_CXA2025AS_US_MT NULL
#define OPTION_VAL_BMF_FINAL2_MT NULL
#define OPTION_VAL_BMF_FINAL3_MT NULL
#define OPTION_VAL_SMOOTH_FBX_MT NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_MT NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_MT NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_MT NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_MT NULL
#define OPTION_VAL_NESCAP_MT NULL
#define OPTION_VAL_WAVEBEAM_MT NULL
#define OPTION_VAL_RAW_MT NULL
#define OPTION_VAL_CUSTOM_MT NULL
#define FCEUMM_NTSC_FILTER_LABEL_MT NULL
#define FCEUMM_NTSC_FILTER_INFO_0_MT NULL
#define OPTION_VAL_COMPOSITE_MT NULL
#define OPTION_VAL_SVIDEO_MT NULL
#define OPTION_VAL_RGB_O9_MT NULL
#define OPTION_VAL_MONOCHROME_MT NULL
#define FCEUMM_SNDQUALITY_LABEL_MT NULL
#define FCEUMM_SNDQUALITY_INFO_0_MT NULL
#define OPTION_VAL_LOW_MT NULL
#define OPTION_VAL_HIGH_MT NULL
#define OPTION_VAL_VERY_HIGH_MT NULL
#define FCEUMM_SWAPDUTY_LABEL_MT NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_MT NULL
#define FCEUMM_SWAPDUTY_INFO_0_MT NULL
#define FCEUMM_SNDVOLUME_LABEL_MT NULL
#define FCEUMM_SNDVOLUME_INFO_0_MT NULL
#define OPTION_VAL_0_MT NULL
#define OPTION_VAL_1_MT NULL
#define OPTION_VAL_2_MT NULL
#define OPTION_VAL_3_MT NULL
#define OPTION_VAL_4_MT NULL
#define OPTION_VAL_5_MT NULL
#define OPTION_VAL_6_MT NULL
#define OPTION_VAL_7_MT NULL
#define OPTION_VAL_8_MT NULL
#define OPTION_VAL_9_MT NULL
#define OPTION_VAL_10_MT NULL
#define FCEUMM_APU_1_LABEL_MT NULL
#define FCEUMM_APU_1_LABEL_CAT_MT NULL
#define FCEUMM_APU_1_INFO_0_MT NULL
#define FCEUMM_APU_2_LABEL_MT NULL
#define FCEUMM_APU_2_LABEL_CAT_MT NULL
#define FCEUMM_APU_2_INFO_0_MT NULL
#define FCEUMM_APU_3_LABEL_MT NULL
#define FCEUMM_APU_3_LABEL_CAT_MT NULL
#define FCEUMM_APU_3_INFO_0_MT NULL
#define FCEUMM_APU_4_LABEL_MT NULL
#define FCEUMM_APU_4_LABEL_CAT_MT NULL
#define FCEUMM_APU_4_INFO_0_MT NULL
#define FCEUMM_APU_5_LABEL_MT NULL
#define FCEUMM_APU_5_LABEL_CAT_MT NULL
#define FCEUMM_APU_5_INFO_0_MT NULL
#define FCEUMM_TURBO_ENABLE_LABEL_MT NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_MT NULL
#define OPTION_VAL_NONE_MT NULL
#define OPTION_VAL_PLAYER_1_MT NULL
#define OPTION_VAL_PLAYER_2_MT NULL
#define OPTION_VAL_BOTH_MT NULL
#define FCEUMM_TURBO_DELAY_LABEL_MT NULL
#define FCEUMM_TURBO_DELAY_INFO_0_MT NULL
#define FCEUMM_ZAPPER_MODE_LABEL_MT NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_MT NULL
#define OPTION_VAL_LIGHTGUN_MT NULL
#define OPTION_VAL_TOUCHSCREEN_MT NULL
#define OPTION_VAL_MOUSE_MT NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_MT NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_MT NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_MT NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_MT NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_MT NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_MT NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_MT NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_MT NULL
#define FCEUMM_OVERCLOCKING_LABEL_MT NULL
#define FCEUMM_OVERCLOCKING_INFO_0_MT NULL
#define OPTION_VAL_2X_POSTRENDER_MT NULL
#define OPTION_VAL_2X_VBLANK_MT NULL
#define FCEUMM_RAMSTATE_LABEL_MT NULL
#define FCEUMM_RAMSTATE_INFO_0_MT NULL
#define OPTION_VAL_FILL_FF_MT NULL
#define OPTION_VAL_FILL_00_MT NULL
#define OPTION_VAL_RANDOM_MT NULL

struct retro_core_option_v2_category option_cats_mt[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_MT,
      CATEGORY_SYSTEM_INFO_0_MT
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_MT,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_MT
#else
      CATEGORY_VIDEO_INFO_1_MT
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_MT,
      CATEGORY_AUDIO_INFO_0_MT
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_MT,
      CATEGORY_INPUT_INFO_0_MT
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_MT,
      CATEGORY_HACKS_INFO_0_MT
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_MT,
      CATEGORY_DIP_SWITCH_INFO_0_MT
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_mt[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_MT,
      NULL,
      FCEUMM_REGION_INFO_0_MT,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_MT },
         { "NTSC",  OPTION_VAL_NTSC_MT },
         { "PAL",   OPTION_VAL_PAL_MT },
         { "Dendy", OPTION_VAL_DENDY_MT },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_MT,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_MT,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_MT,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_MT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_MT,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_MT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_MT,
      NULL,
      FCEUMM_ASPECT_INFO_0_MT,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_MT },
         { "4:3",     OPTION_VAL_4_3_MT },
         { "PP",      OPTION_VAL_PP_MT },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_MT,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_MT,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_MT,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_MT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_MT,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_MT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_MT,
      NULL,
      FCEUMM_PALETTE_INFO_0_MT,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_MT },
         { "asqrealc",             OPTION_VAL_ASQREALC_MT },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_MT },
         { "rgb",                  OPTION_VAL_RGB_MT },
         { "yuv-v3",               OPTION_VAL_YUV_V3_MT },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_MT },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_MT },
         { "pal",                  OPTION_VAL_PAL_MT },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_MT },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_MT },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_MT },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_MT },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_MT },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_MT },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_MT },
         { "nescap",               OPTION_VAL_NESCAP_MT },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_MT },
         { "raw",                  OPTION_VAL_RAW_MT },
         { "custom",               OPTION_VAL_CUSTOM_MT },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_MT,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_MT,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_MT },
         { "svideo",     OPTION_VAL_SVIDEO_MT },
         { "rgb",        OPTION_VAL_RGB_O9_MT },
         { "monochrome", OPTION_VAL_MONOCHROME_MT },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_MT,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_MT,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_MT },
         { "High",      OPTION_VAL_HIGH_MT },
         { "Very High", OPTION_VAL_VERY_HIGH_MT },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_MT,
      FCEUMM_SWAPDUTY_LABEL_CAT_MT,
      FCEUMM_SWAPDUTY_INFO_0_MT,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_MT,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_MT,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_MT },
         { "1",  OPTION_VAL_1_MT },
         { "2",  OPTION_VAL_2_MT },
         { "3",  OPTION_VAL_3_MT },
         { "4",  OPTION_VAL_4_MT },
         { "5",  OPTION_VAL_5_MT },
         { "6",  OPTION_VAL_6_MT },
         { "7",  OPTION_VAL_7_MT },
         { "8",  OPTION_VAL_8_MT },
         { "9",  OPTION_VAL_9_MT },
         { "10", OPTION_VAL_10_MT },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_MT,
      FCEUMM_APU_1_LABEL_CAT_MT,
      FCEUMM_APU_1_INFO_0_MT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_MT,
      FCEUMM_APU_2_LABEL_CAT_MT,
      FCEUMM_APU_2_INFO_0_MT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_MT,
      FCEUMM_APU_3_LABEL_CAT_MT,
      FCEUMM_APU_3_INFO_0_MT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_MT,
      FCEUMM_APU_4_LABEL_CAT_MT,
      FCEUMM_APU_4_INFO_0_MT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_MT,
      FCEUMM_APU_5_LABEL_CAT_MT,
      FCEUMM_APU_5_INFO_0_MT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_MT,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_MT,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_MT },
         { "Player 1", OPTION_VAL_PLAYER_1_MT },
         { "Player 2", OPTION_VAL_PLAYER_2_MT },
         { "Both",     OPTION_VAL_BOTH_MT },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_MT,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_MT,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_MT,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_MT,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_MT },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_MT },
         { "mouse",       OPTION_VAL_MOUSE_MT },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_MT,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_MT,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_MT,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_MT,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_MT,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_MT,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_MT,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_MT,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_MT,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_MT,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_MT },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_MT },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_MT,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_MT,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_MT },
         { "fill $00", OPTION_VAL_FILL_00_MT },
         { "random",   OPTION_VAL_RANDOM_MT },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_mt = {
   option_cats_mt,
   option_defs_mt
};

/* RETRO_LANGUAGE_NL */

#define CATEGORY_SYSTEM_LABEL_NL NULL
#define CATEGORY_SYSTEM_INFO_0_NL NULL
#define CATEGORY_VIDEO_LABEL_NL NULL
#define CATEGORY_VIDEO_INFO_0_NL NULL
#define CATEGORY_VIDEO_INFO_1_NL NULL
#define CATEGORY_AUDIO_LABEL_NL "Geluid"
#define CATEGORY_AUDIO_INFO_0_NL NULL
#define CATEGORY_INPUT_LABEL_NL "Invoer"
#define CATEGORY_INPUT_INFO_0_NL NULL
#define CATEGORY_HACKS_LABEL_NL NULL
#define CATEGORY_HACKS_INFO_0_NL NULL
#define CATEGORY_DIP_SWITCH_LABEL_NL NULL
#define CATEGORY_DIP_SWITCH_INFO_0_NL NULL
#define FCEUMM_REGION_LABEL_NL NULL
#define FCEUMM_REGION_INFO_0_NL NULL
#define OPTION_VAL_AUTO_NL NULL
#define OPTION_VAL_NTSC_NL NULL
#define OPTION_VAL_PAL_NL NULL
#define OPTION_VAL_DENDY_NL NULL
#define FCEUMM_GAME_GENIE_LABEL_NL NULL
#define FCEUMM_GAME_GENIE_INFO_0_NL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_NL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_NL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_NL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_NL NULL
#define FCEUMM_ASPECT_LABEL_NL NULL
#define FCEUMM_ASPECT_INFO_0_NL NULL
#define OPTION_VAL_8_7_PAR_NL NULL
#define OPTION_VAL_4_3_NL NULL
#define OPTION_VAL_PP_NL NULL
#define FCEUMM_OVERSCAN_LABEL_NL NULL
#define FCEUMM_OVERSCAN_INFO_0_NL NULL
#define FCEUMM_OVERSCAN_H_LABEL_NL NULL
#define FCEUMM_OVERSCAN_H_INFO_0_NL NULL
#define FCEUMM_OVERSCAN_V_LABEL_NL NULL
#define FCEUMM_OVERSCAN_V_INFO_0_NL NULL
#define FCEUMM_PALETTE_LABEL_NL NULL
#define FCEUMM_PALETTE_INFO_0_NL NULL
#define OPTION_VAL_DEFAULT_NL NULL
#define OPTION_VAL_ASQREALC_NL NULL
#define OPTION_VAL_NINTENDO_VC_NL NULL
#define OPTION_VAL_RGB_NL NULL
#define OPTION_VAL_YUV_V3_NL NULL
#define OPTION_VAL_UNSATURATED_FINAL_NL NULL
#define OPTION_VAL_SONY_CXA2025AS_US_NL NULL
#define OPTION_VAL_BMF_FINAL2_NL NULL
#define OPTION_VAL_BMF_FINAL3_NL NULL
#define OPTION_VAL_SMOOTH_FBX_NL NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_NL NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_NL NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_NL NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_NL NULL
#define OPTION_VAL_NESCAP_NL NULL
#define OPTION_VAL_WAVEBEAM_NL NULL
#define OPTION_VAL_RAW_NL NULL
#define OPTION_VAL_CUSTOM_NL NULL
#define FCEUMM_NTSC_FILTER_LABEL_NL NULL
#define FCEUMM_NTSC_FILTER_INFO_0_NL NULL
#define OPTION_VAL_COMPOSITE_NL NULL
#define OPTION_VAL_SVIDEO_NL NULL
#define OPTION_VAL_RGB_O9_NL NULL
#define OPTION_VAL_MONOCHROME_NL NULL
#define FCEUMM_SNDQUALITY_LABEL_NL NULL
#define FCEUMM_SNDQUALITY_INFO_0_NL NULL
#define OPTION_VAL_LOW_NL NULL
#define OPTION_VAL_HIGH_NL NULL
#define OPTION_VAL_VERY_HIGH_NL NULL
#define FCEUMM_SWAPDUTY_LABEL_NL NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_NL NULL
#define FCEUMM_SWAPDUTY_INFO_0_NL NULL
#define FCEUMM_SNDVOLUME_LABEL_NL NULL
#define FCEUMM_SNDVOLUME_INFO_0_NL NULL
#define OPTION_VAL_0_NL NULL
#define OPTION_VAL_1_NL NULL
#define OPTION_VAL_2_NL NULL
#define OPTION_VAL_3_NL NULL
#define OPTION_VAL_4_NL NULL
#define OPTION_VAL_5_NL NULL
#define OPTION_VAL_6_NL NULL
#define OPTION_VAL_7_NL NULL
#define OPTION_VAL_8_NL NULL
#define OPTION_VAL_9_NL NULL
#define OPTION_VAL_10_NL NULL
#define FCEUMM_APU_1_LABEL_NL NULL
#define FCEUMM_APU_1_LABEL_CAT_NL NULL
#define FCEUMM_APU_1_INFO_0_NL NULL
#define FCEUMM_APU_2_LABEL_NL NULL
#define FCEUMM_APU_2_LABEL_CAT_NL NULL
#define FCEUMM_APU_2_INFO_0_NL NULL
#define FCEUMM_APU_3_LABEL_NL NULL
#define FCEUMM_APU_3_LABEL_CAT_NL NULL
#define FCEUMM_APU_3_INFO_0_NL NULL
#define FCEUMM_APU_4_LABEL_NL NULL
#define FCEUMM_APU_4_LABEL_CAT_NL NULL
#define FCEUMM_APU_4_INFO_0_NL NULL
#define FCEUMM_APU_5_LABEL_NL NULL
#define FCEUMM_APU_5_LABEL_CAT_NL NULL
#define FCEUMM_APU_5_INFO_0_NL NULL
#define FCEUMM_TURBO_ENABLE_LABEL_NL NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_NL NULL
#define OPTION_VAL_NONE_NL NULL
#define OPTION_VAL_PLAYER_1_NL NULL
#define OPTION_VAL_PLAYER_2_NL NULL
#define OPTION_VAL_BOTH_NL NULL
#define FCEUMM_TURBO_DELAY_LABEL_NL NULL
#define FCEUMM_TURBO_DELAY_INFO_0_NL NULL
#define FCEUMM_ZAPPER_MODE_LABEL_NL NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_NL NULL
#define OPTION_VAL_LIGHTGUN_NL NULL
#define OPTION_VAL_TOUCHSCREEN_NL NULL
#define OPTION_VAL_MOUSE_NL NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_NL NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_NL NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_NL NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_NL NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_NL NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_NL NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_NL NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_NL NULL
#define FCEUMM_OVERCLOCKING_LABEL_NL NULL
#define FCEUMM_OVERCLOCKING_INFO_0_NL NULL
#define OPTION_VAL_2X_POSTRENDER_NL NULL
#define OPTION_VAL_2X_VBLANK_NL NULL
#define FCEUMM_RAMSTATE_LABEL_NL NULL
#define FCEUMM_RAMSTATE_INFO_0_NL NULL
#define OPTION_VAL_FILL_FF_NL NULL
#define OPTION_VAL_FILL_00_NL NULL
#define OPTION_VAL_RANDOM_NL NULL

struct retro_core_option_v2_category option_cats_nl[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_NL,
      CATEGORY_SYSTEM_INFO_0_NL
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_NL,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_NL
#else
      CATEGORY_VIDEO_INFO_1_NL
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_NL,
      CATEGORY_AUDIO_INFO_0_NL
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_NL,
      CATEGORY_INPUT_INFO_0_NL
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_NL,
      CATEGORY_HACKS_INFO_0_NL
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_NL,
      CATEGORY_DIP_SWITCH_INFO_0_NL
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_nl[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_NL,
      NULL,
      FCEUMM_REGION_INFO_0_NL,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_NL },
         { "NTSC",  OPTION_VAL_NTSC_NL },
         { "PAL",   OPTION_VAL_PAL_NL },
         { "Dendy", OPTION_VAL_DENDY_NL },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_NL,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_NL,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_NL,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_NL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_NL,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_NL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_NL,
      NULL,
      FCEUMM_ASPECT_INFO_0_NL,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_NL },
         { "4:3",     OPTION_VAL_4_3_NL },
         { "PP",      OPTION_VAL_PP_NL },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_NL,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_NL,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_NL,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_NL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_NL,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_NL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_NL,
      NULL,
      FCEUMM_PALETTE_INFO_0_NL,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_NL },
         { "asqrealc",             OPTION_VAL_ASQREALC_NL },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_NL },
         { "rgb",                  OPTION_VAL_RGB_NL },
         { "yuv-v3",               OPTION_VAL_YUV_V3_NL },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_NL },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_NL },
         { "pal",                  OPTION_VAL_PAL_NL },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_NL },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_NL },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_NL },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_NL },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_NL },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_NL },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_NL },
         { "nescap",               OPTION_VAL_NESCAP_NL },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_NL },
         { "raw",                  OPTION_VAL_RAW_NL },
         { "custom",               OPTION_VAL_CUSTOM_NL },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_NL,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_NL,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_NL },
         { "svideo",     OPTION_VAL_SVIDEO_NL },
         { "rgb",        OPTION_VAL_RGB_O9_NL },
         { "monochrome", OPTION_VAL_MONOCHROME_NL },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_NL,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_NL,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_NL },
         { "High",      OPTION_VAL_HIGH_NL },
         { "Very High", OPTION_VAL_VERY_HIGH_NL },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_NL,
      FCEUMM_SWAPDUTY_LABEL_CAT_NL,
      FCEUMM_SWAPDUTY_INFO_0_NL,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_NL,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_NL,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_NL },
         { "1",  OPTION_VAL_1_NL },
         { "2",  OPTION_VAL_2_NL },
         { "3",  OPTION_VAL_3_NL },
         { "4",  OPTION_VAL_4_NL },
         { "5",  OPTION_VAL_5_NL },
         { "6",  OPTION_VAL_6_NL },
         { "7",  OPTION_VAL_7_NL },
         { "8",  OPTION_VAL_8_NL },
         { "9",  OPTION_VAL_9_NL },
         { "10", OPTION_VAL_10_NL },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_NL,
      FCEUMM_APU_1_LABEL_CAT_NL,
      FCEUMM_APU_1_INFO_0_NL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_NL,
      FCEUMM_APU_2_LABEL_CAT_NL,
      FCEUMM_APU_2_INFO_0_NL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_NL,
      FCEUMM_APU_3_LABEL_CAT_NL,
      FCEUMM_APU_3_INFO_0_NL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_NL,
      FCEUMM_APU_4_LABEL_CAT_NL,
      FCEUMM_APU_4_INFO_0_NL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_NL,
      FCEUMM_APU_5_LABEL_CAT_NL,
      FCEUMM_APU_5_INFO_0_NL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_NL,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_NL,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_NL },
         { "Player 1", OPTION_VAL_PLAYER_1_NL },
         { "Player 2", OPTION_VAL_PLAYER_2_NL },
         { "Both",     OPTION_VAL_BOTH_NL },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_NL,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_NL,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_NL,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_NL,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_NL },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_NL },
         { "mouse",       OPTION_VAL_MOUSE_NL },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_NL,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_NL,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_NL,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_NL,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_NL,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_NL,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_NL,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_NL,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_NL,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_NL,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_NL },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_NL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_NL,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_NL,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_NL },
         { "fill $00", OPTION_VAL_FILL_00_NL },
         { "random",   OPTION_VAL_RANDOM_NL },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_nl = {
   option_cats_nl,
   option_defs_nl
};

/* RETRO_LANGUAGE_OC */

#define CATEGORY_SYSTEM_LABEL_OC NULL
#define CATEGORY_SYSTEM_INFO_0_OC NULL
#define CATEGORY_VIDEO_LABEL_OC "Vidèo"
#define CATEGORY_VIDEO_INFO_0_OC NULL
#define CATEGORY_VIDEO_INFO_1_OC NULL
#define CATEGORY_AUDIO_LABEL_OC NULL
#define CATEGORY_AUDIO_INFO_0_OC NULL
#define CATEGORY_INPUT_LABEL_OC NULL
#define CATEGORY_INPUT_INFO_0_OC NULL
#define CATEGORY_HACKS_LABEL_OC NULL
#define CATEGORY_HACKS_INFO_0_OC NULL
#define CATEGORY_DIP_SWITCH_LABEL_OC NULL
#define CATEGORY_DIP_SWITCH_INFO_0_OC NULL
#define FCEUMM_REGION_LABEL_OC NULL
#define FCEUMM_REGION_INFO_0_OC NULL
#define OPTION_VAL_AUTO_OC NULL
#define OPTION_VAL_NTSC_OC NULL
#define OPTION_VAL_PAL_OC NULL
#define OPTION_VAL_DENDY_OC NULL
#define FCEUMM_GAME_GENIE_LABEL_OC NULL
#define FCEUMM_GAME_GENIE_INFO_0_OC NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_OC NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_OC NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_OC NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_OC NULL
#define FCEUMM_ASPECT_LABEL_OC NULL
#define FCEUMM_ASPECT_INFO_0_OC NULL
#define OPTION_VAL_8_7_PAR_OC NULL
#define OPTION_VAL_4_3_OC NULL
#define OPTION_VAL_PP_OC NULL
#define FCEUMM_OVERSCAN_LABEL_OC NULL
#define FCEUMM_OVERSCAN_INFO_0_OC NULL
#define FCEUMM_OVERSCAN_H_LABEL_OC NULL
#define FCEUMM_OVERSCAN_H_INFO_0_OC NULL
#define FCEUMM_OVERSCAN_V_LABEL_OC NULL
#define FCEUMM_OVERSCAN_V_INFO_0_OC NULL
#define FCEUMM_PALETTE_LABEL_OC NULL
#define FCEUMM_PALETTE_INFO_0_OC NULL
#define OPTION_VAL_DEFAULT_OC NULL
#define OPTION_VAL_ASQREALC_OC NULL
#define OPTION_VAL_NINTENDO_VC_OC NULL
#define OPTION_VAL_RGB_OC NULL
#define OPTION_VAL_YUV_V3_OC NULL
#define OPTION_VAL_UNSATURATED_FINAL_OC NULL
#define OPTION_VAL_SONY_CXA2025AS_US_OC NULL
#define OPTION_VAL_BMF_FINAL2_OC NULL
#define OPTION_VAL_BMF_FINAL3_OC NULL
#define OPTION_VAL_SMOOTH_FBX_OC NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_OC NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_OC NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_OC NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_OC NULL
#define OPTION_VAL_NESCAP_OC NULL
#define OPTION_VAL_WAVEBEAM_OC NULL
#define OPTION_VAL_RAW_OC NULL
#define OPTION_VAL_CUSTOM_OC NULL
#define FCEUMM_NTSC_FILTER_LABEL_OC NULL
#define FCEUMM_NTSC_FILTER_INFO_0_OC NULL
#define OPTION_VAL_COMPOSITE_OC NULL
#define OPTION_VAL_SVIDEO_OC NULL
#define OPTION_VAL_RGB_O9_OC NULL
#define OPTION_VAL_MONOCHROME_OC NULL
#define FCEUMM_SNDQUALITY_LABEL_OC NULL
#define FCEUMM_SNDQUALITY_INFO_0_OC NULL
#define OPTION_VAL_LOW_OC NULL
#define OPTION_VAL_HIGH_OC NULL
#define OPTION_VAL_VERY_HIGH_OC NULL
#define FCEUMM_SWAPDUTY_LABEL_OC NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_OC NULL
#define FCEUMM_SWAPDUTY_INFO_0_OC NULL
#define FCEUMM_SNDVOLUME_LABEL_OC NULL
#define FCEUMM_SNDVOLUME_INFO_0_OC NULL
#define OPTION_VAL_0_OC NULL
#define OPTION_VAL_1_OC NULL
#define OPTION_VAL_2_OC NULL
#define OPTION_VAL_3_OC NULL
#define OPTION_VAL_4_OC NULL
#define OPTION_VAL_5_OC NULL
#define OPTION_VAL_6_OC NULL
#define OPTION_VAL_7_OC NULL
#define OPTION_VAL_8_OC NULL
#define OPTION_VAL_9_OC NULL
#define OPTION_VAL_10_OC NULL
#define FCEUMM_APU_1_LABEL_OC NULL
#define FCEUMM_APU_1_LABEL_CAT_OC NULL
#define FCEUMM_APU_1_INFO_0_OC NULL
#define FCEUMM_APU_2_LABEL_OC NULL
#define FCEUMM_APU_2_LABEL_CAT_OC NULL
#define FCEUMM_APU_2_INFO_0_OC NULL
#define FCEUMM_APU_3_LABEL_OC NULL
#define FCEUMM_APU_3_LABEL_CAT_OC NULL
#define FCEUMM_APU_3_INFO_0_OC NULL
#define FCEUMM_APU_4_LABEL_OC NULL
#define FCEUMM_APU_4_LABEL_CAT_OC NULL
#define FCEUMM_APU_4_INFO_0_OC NULL
#define FCEUMM_APU_5_LABEL_OC NULL
#define FCEUMM_APU_5_LABEL_CAT_OC NULL
#define FCEUMM_APU_5_INFO_0_OC NULL
#define FCEUMM_TURBO_ENABLE_LABEL_OC NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_OC NULL
#define OPTION_VAL_NONE_OC NULL
#define OPTION_VAL_PLAYER_1_OC NULL
#define OPTION_VAL_PLAYER_2_OC NULL
#define OPTION_VAL_BOTH_OC NULL
#define FCEUMM_TURBO_DELAY_LABEL_OC NULL
#define FCEUMM_TURBO_DELAY_INFO_0_OC NULL
#define FCEUMM_ZAPPER_MODE_LABEL_OC NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_OC NULL
#define OPTION_VAL_LIGHTGUN_OC NULL
#define OPTION_VAL_TOUCHSCREEN_OC NULL
#define OPTION_VAL_MOUSE_OC NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_OC NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_OC NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_OC NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_OC NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_OC NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_OC NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_OC NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_OC NULL
#define FCEUMM_OVERCLOCKING_LABEL_OC NULL
#define FCEUMM_OVERCLOCKING_INFO_0_OC NULL
#define OPTION_VAL_2X_POSTRENDER_OC NULL
#define OPTION_VAL_2X_VBLANK_OC NULL
#define FCEUMM_RAMSTATE_LABEL_OC NULL
#define FCEUMM_RAMSTATE_INFO_0_OC NULL
#define OPTION_VAL_FILL_FF_OC NULL
#define OPTION_VAL_FILL_00_OC NULL
#define OPTION_VAL_RANDOM_OC NULL

struct retro_core_option_v2_category option_cats_oc[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_OC,
      CATEGORY_SYSTEM_INFO_0_OC
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_OC,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_OC
#else
      CATEGORY_VIDEO_INFO_1_OC
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_OC,
      CATEGORY_AUDIO_INFO_0_OC
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_OC,
      CATEGORY_INPUT_INFO_0_OC
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_OC,
      CATEGORY_HACKS_INFO_0_OC
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_OC,
      CATEGORY_DIP_SWITCH_INFO_0_OC
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_oc[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_OC,
      NULL,
      FCEUMM_REGION_INFO_0_OC,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_OC },
         { "NTSC",  OPTION_VAL_NTSC_OC },
         { "PAL",   OPTION_VAL_PAL_OC },
         { "Dendy", OPTION_VAL_DENDY_OC },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_OC,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_OC,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_OC,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_OC,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_OC,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_OC,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_OC,
      NULL,
      FCEUMM_ASPECT_INFO_0_OC,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_OC },
         { "4:3",     OPTION_VAL_4_3_OC },
         { "PP",      OPTION_VAL_PP_OC },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_OC,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_OC,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_OC,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_OC,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_OC,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_OC,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_OC,
      NULL,
      FCEUMM_PALETTE_INFO_0_OC,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_OC },
         { "asqrealc",             OPTION_VAL_ASQREALC_OC },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_OC },
         { "rgb",                  OPTION_VAL_RGB_OC },
         { "yuv-v3",               OPTION_VAL_YUV_V3_OC },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_OC },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_OC },
         { "pal",                  OPTION_VAL_PAL_OC },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_OC },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_OC },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_OC },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_OC },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_OC },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_OC },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_OC },
         { "nescap",               OPTION_VAL_NESCAP_OC },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_OC },
         { "raw",                  OPTION_VAL_RAW_OC },
         { "custom",               OPTION_VAL_CUSTOM_OC },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_OC,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_OC,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_OC },
         { "svideo",     OPTION_VAL_SVIDEO_OC },
         { "rgb",        OPTION_VAL_RGB_O9_OC },
         { "monochrome", OPTION_VAL_MONOCHROME_OC },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_OC,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_OC,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_OC },
         { "High",      OPTION_VAL_HIGH_OC },
         { "Very High", OPTION_VAL_VERY_HIGH_OC },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_OC,
      FCEUMM_SWAPDUTY_LABEL_CAT_OC,
      FCEUMM_SWAPDUTY_INFO_0_OC,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_OC,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_OC,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_OC },
         { "1",  OPTION_VAL_1_OC },
         { "2",  OPTION_VAL_2_OC },
         { "3",  OPTION_VAL_3_OC },
         { "4",  OPTION_VAL_4_OC },
         { "5",  OPTION_VAL_5_OC },
         { "6",  OPTION_VAL_6_OC },
         { "7",  OPTION_VAL_7_OC },
         { "8",  OPTION_VAL_8_OC },
         { "9",  OPTION_VAL_9_OC },
         { "10", OPTION_VAL_10_OC },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_OC,
      FCEUMM_APU_1_LABEL_CAT_OC,
      FCEUMM_APU_1_INFO_0_OC,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_OC,
      FCEUMM_APU_2_LABEL_CAT_OC,
      FCEUMM_APU_2_INFO_0_OC,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_OC,
      FCEUMM_APU_3_LABEL_CAT_OC,
      FCEUMM_APU_3_INFO_0_OC,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_OC,
      FCEUMM_APU_4_LABEL_CAT_OC,
      FCEUMM_APU_4_INFO_0_OC,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_OC,
      FCEUMM_APU_5_LABEL_CAT_OC,
      FCEUMM_APU_5_INFO_0_OC,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_OC,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_OC,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_OC },
         { "Player 1", OPTION_VAL_PLAYER_1_OC },
         { "Player 2", OPTION_VAL_PLAYER_2_OC },
         { "Both",     OPTION_VAL_BOTH_OC },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_OC,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_OC,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_OC,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_OC,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_OC },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_OC },
         { "mouse",       OPTION_VAL_MOUSE_OC },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_OC,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_OC,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_OC,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_OC,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_OC,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_OC,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_OC,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_OC,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_OC,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_OC,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_OC },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_OC },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_OC,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_OC,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_OC },
         { "fill $00", OPTION_VAL_FILL_00_OC },
         { "random",   OPTION_VAL_RANDOM_OC },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_oc = {
   option_cats_oc,
   option_defs_oc
};

/* RETRO_LANGUAGE_PL */

#define CATEGORY_SYSTEM_LABEL_PL NULL
#define CATEGORY_SYSTEM_INFO_0_PL "Skonfiguruj parametry dodatku regionu i sprzętu."
#define CATEGORY_VIDEO_LABEL_PL "Wideo"
#define CATEGORY_VIDEO_INFO_0_PL "Skonfiguruj proporcje obrazów, wyświetlanie przycinania, palety kolorów, opcje filtra wideo."
#define CATEGORY_VIDEO_INFO_1_PL "Skonfiguruj proporcje obrazów, wyświetlanie przycinania, opcje palety kolorów."
#define CATEGORY_AUDIO_LABEL_PL NULL
#define CATEGORY_AUDIO_INFO_0_PL "Skonfiguruj jakość dźwięku, głośność, włączanie ustawień kanału."
#define CATEGORY_INPUT_LABEL_PL "Wejście"
#define CATEGORY_INPUT_INFO_0_PL "Skonfiguruj parametry turbo i pistoletu świetlnego."
#define CATEGORY_HACKS_LABEL_PL "Hacki emulacyjne"
#define CATEGORY_HACKS_INFO_0_PL "Skonfiguruj przetaktowanie procesora i dokładność emulacji wpływające na niską wydajność i kompatybilność."
#define CATEGORY_DIP_SWITCH_LABEL_PL "Przełączniki DIP"
#define CATEGORY_DIP_SWITCH_INFO_0_PL "Skonfiguruj ustawienia gry arcade."
#define FCEUMM_REGION_LABEL_PL NULL
#define FCEUMM_REGION_INFO_0_PL NULL
#define OPTION_VAL_AUTO_PL NULL
#define OPTION_VAL_NTSC_PL NULL
#define OPTION_VAL_PAL_PL NULL
#define OPTION_VAL_DENDY_PL NULL
#define FCEUMM_GAME_GENIE_LABEL_PL NULL
#define FCEUMM_GAME_GENIE_INFO_0_PL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_PL NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_PL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_PL NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_PL NULL
#define FCEUMM_ASPECT_LABEL_PL NULL
#define FCEUMM_ASPECT_INFO_0_PL NULL
#define OPTION_VAL_8_7_PAR_PL NULL
#define OPTION_VAL_4_3_PL NULL
#define OPTION_VAL_PP_PL NULL
#define FCEUMM_OVERSCAN_LABEL_PL NULL
#define FCEUMM_OVERSCAN_INFO_0_PL NULL
#define FCEUMM_OVERSCAN_H_LABEL_PL NULL
#define FCEUMM_OVERSCAN_H_INFO_0_PL NULL
#define FCEUMM_OVERSCAN_V_LABEL_PL NULL
#define FCEUMM_OVERSCAN_V_INFO_0_PL NULL
#define FCEUMM_PALETTE_LABEL_PL NULL
#define FCEUMM_PALETTE_INFO_0_PL NULL
#define OPTION_VAL_DEFAULT_PL NULL
#define OPTION_VAL_ASQREALC_PL NULL
#define OPTION_VAL_NINTENDO_VC_PL NULL
#define OPTION_VAL_RGB_PL NULL
#define OPTION_VAL_YUV_V3_PL NULL
#define OPTION_VAL_UNSATURATED_FINAL_PL NULL
#define OPTION_VAL_SONY_CXA2025AS_US_PL NULL
#define OPTION_VAL_BMF_FINAL2_PL NULL
#define OPTION_VAL_BMF_FINAL3_PL NULL
#define OPTION_VAL_SMOOTH_FBX_PL NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_PL NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_PL NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_PL NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_PL NULL
#define OPTION_VAL_NESCAP_PL NULL
#define OPTION_VAL_WAVEBEAM_PL NULL
#define OPTION_VAL_RAW_PL NULL
#define OPTION_VAL_CUSTOM_PL NULL
#define FCEUMM_NTSC_FILTER_LABEL_PL NULL
#define FCEUMM_NTSC_FILTER_INFO_0_PL NULL
#define OPTION_VAL_COMPOSITE_PL NULL
#define OPTION_VAL_SVIDEO_PL NULL
#define OPTION_VAL_RGB_O9_PL NULL
#define OPTION_VAL_MONOCHROME_PL NULL
#define FCEUMM_SNDQUALITY_LABEL_PL NULL
#define FCEUMM_SNDQUALITY_INFO_0_PL NULL
#define OPTION_VAL_LOW_PL NULL
#define OPTION_VAL_HIGH_PL NULL
#define OPTION_VAL_VERY_HIGH_PL NULL
#define FCEUMM_SWAPDUTY_LABEL_PL NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_PL NULL
#define FCEUMM_SWAPDUTY_INFO_0_PL NULL
#define FCEUMM_SNDVOLUME_LABEL_PL NULL
#define FCEUMM_SNDVOLUME_INFO_0_PL NULL
#define OPTION_VAL_0_PL NULL
#define OPTION_VAL_1_PL NULL
#define OPTION_VAL_2_PL NULL
#define OPTION_VAL_3_PL NULL
#define OPTION_VAL_4_PL NULL
#define OPTION_VAL_5_PL NULL
#define OPTION_VAL_6_PL NULL
#define OPTION_VAL_7_PL NULL
#define OPTION_VAL_8_PL NULL
#define OPTION_VAL_9_PL NULL
#define OPTION_VAL_10_PL NULL
#define FCEUMM_APU_1_LABEL_PL NULL
#define FCEUMM_APU_1_LABEL_CAT_PL NULL
#define FCEUMM_APU_1_INFO_0_PL NULL
#define FCEUMM_APU_2_LABEL_PL NULL
#define FCEUMM_APU_2_LABEL_CAT_PL NULL
#define FCEUMM_APU_2_INFO_0_PL NULL
#define FCEUMM_APU_3_LABEL_PL NULL
#define FCEUMM_APU_3_LABEL_CAT_PL NULL
#define FCEUMM_APU_3_INFO_0_PL NULL
#define FCEUMM_APU_4_LABEL_PL NULL
#define FCEUMM_APU_4_LABEL_CAT_PL NULL
#define FCEUMM_APU_4_INFO_0_PL NULL
#define FCEUMM_APU_5_LABEL_PL NULL
#define FCEUMM_APU_5_LABEL_CAT_PL NULL
#define FCEUMM_APU_5_INFO_0_PL NULL
#define FCEUMM_TURBO_ENABLE_LABEL_PL NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_PL NULL
#define OPTION_VAL_NONE_PL NULL
#define OPTION_VAL_PLAYER_1_PL NULL
#define OPTION_VAL_PLAYER_2_PL NULL
#define OPTION_VAL_BOTH_PL NULL
#define FCEUMM_TURBO_DELAY_LABEL_PL NULL
#define FCEUMM_TURBO_DELAY_INFO_0_PL NULL
#define FCEUMM_ZAPPER_MODE_LABEL_PL NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_PL NULL
#define OPTION_VAL_LIGHTGUN_PL NULL
#define OPTION_VAL_TOUCHSCREEN_PL NULL
#define OPTION_VAL_MOUSE_PL NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_PL NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_PL NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_PL NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_PL NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_PL NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_PL NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_PL NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_PL NULL
#define FCEUMM_OVERCLOCKING_LABEL_PL NULL
#define FCEUMM_OVERCLOCKING_INFO_0_PL NULL
#define OPTION_VAL_2X_POSTRENDER_PL NULL
#define OPTION_VAL_2X_VBLANK_PL NULL
#define FCEUMM_RAMSTATE_LABEL_PL NULL
#define FCEUMM_RAMSTATE_INFO_0_PL NULL
#define OPTION_VAL_FILL_FF_PL NULL
#define OPTION_VAL_FILL_00_PL NULL
#define OPTION_VAL_RANDOM_PL NULL

struct retro_core_option_v2_category option_cats_pl[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_PL,
      CATEGORY_SYSTEM_INFO_0_PL
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_PL,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_PL
#else
      CATEGORY_VIDEO_INFO_1_PL
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_PL,
      CATEGORY_AUDIO_INFO_0_PL
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_PL,
      CATEGORY_INPUT_INFO_0_PL
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_PL,
      CATEGORY_HACKS_INFO_0_PL
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_PL,
      CATEGORY_DIP_SWITCH_INFO_0_PL
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_pl[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_PL,
      NULL,
      FCEUMM_REGION_INFO_0_PL,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_PL },
         { "NTSC",  OPTION_VAL_NTSC_PL },
         { "PAL",   OPTION_VAL_PAL_PL },
         { "Dendy", OPTION_VAL_DENDY_PL },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_PL,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_PL,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_PL,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_PL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_PL,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_PL,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_PL,
      NULL,
      FCEUMM_ASPECT_INFO_0_PL,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_PL },
         { "4:3",     OPTION_VAL_4_3_PL },
         { "PP",      OPTION_VAL_PP_PL },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_PL,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_PL,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_PL,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_PL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_PL,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_PL,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_PL,
      NULL,
      FCEUMM_PALETTE_INFO_0_PL,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_PL },
         { "asqrealc",             OPTION_VAL_ASQREALC_PL },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_PL },
         { "rgb",                  OPTION_VAL_RGB_PL },
         { "yuv-v3",               OPTION_VAL_YUV_V3_PL },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_PL },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_PL },
         { "pal",                  OPTION_VAL_PAL_PL },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_PL },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_PL },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_PL },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_PL },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_PL },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_PL },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_PL },
         { "nescap",               OPTION_VAL_NESCAP_PL },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_PL },
         { "raw",                  OPTION_VAL_RAW_PL },
         { "custom",               OPTION_VAL_CUSTOM_PL },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_PL,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_PL,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_PL },
         { "svideo",     OPTION_VAL_SVIDEO_PL },
         { "rgb",        OPTION_VAL_RGB_O9_PL },
         { "monochrome", OPTION_VAL_MONOCHROME_PL },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_PL,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_PL,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_PL },
         { "High",      OPTION_VAL_HIGH_PL },
         { "Very High", OPTION_VAL_VERY_HIGH_PL },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_PL,
      FCEUMM_SWAPDUTY_LABEL_CAT_PL,
      FCEUMM_SWAPDUTY_INFO_0_PL,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_PL,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_PL,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_PL },
         { "1",  OPTION_VAL_1_PL },
         { "2",  OPTION_VAL_2_PL },
         { "3",  OPTION_VAL_3_PL },
         { "4",  OPTION_VAL_4_PL },
         { "5",  OPTION_VAL_5_PL },
         { "6",  OPTION_VAL_6_PL },
         { "7",  OPTION_VAL_7_PL },
         { "8",  OPTION_VAL_8_PL },
         { "9",  OPTION_VAL_9_PL },
         { "10", OPTION_VAL_10_PL },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_PL,
      FCEUMM_APU_1_LABEL_CAT_PL,
      FCEUMM_APU_1_INFO_0_PL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_PL,
      FCEUMM_APU_2_LABEL_CAT_PL,
      FCEUMM_APU_2_INFO_0_PL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_PL,
      FCEUMM_APU_3_LABEL_CAT_PL,
      FCEUMM_APU_3_INFO_0_PL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_PL,
      FCEUMM_APU_4_LABEL_CAT_PL,
      FCEUMM_APU_4_INFO_0_PL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_PL,
      FCEUMM_APU_5_LABEL_CAT_PL,
      FCEUMM_APU_5_INFO_0_PL,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_PL,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_PL,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_PL },
         { "Player 1", OPTION_VAL_PLAYER_1_PL },
         { "Player 2", OPTION_VAL_PLAYER_2_PL },
         { "Both",     OPTION_VAL_BOTH_PL },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_PL,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_PL,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_PL,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_PL,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_PL },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_PL },
         { "mouse",       OPTION_VAL_MOUSE_PL },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_PL,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_PL,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_PL,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_PL,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_PL,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_PL,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_PL,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_PL,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_PL,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_PL,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_PL },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_PL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_PL,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_PL,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_PL },
         { "fill $00", OPTION_VAL_FILL_00_PL },
         { "random",   OPTION_VAL_RANDOM_PL },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_pl = {
   option_cats_pl,
   option_defs_pl
};

/* RETRO_LANGUAGE_PT_BR */

#define CATEGORY_SYSTEM_LABEL_PT_BR "Sistema"
#define CATEGORY_SYSTEM_INFO_0_PT_BR NULL
#define CATEGORY_VIDEO_LABEL_PT_BR "Vídeo"
#define CATEGORY_VIDEO_INFO_0_PT_BR NULL
#define CATEGORY_VIDEO_INFO_1_PT_BR NULL
#define CATEGORY_AUDIO_LABEL_PT_BR "Áudio"
#define CATEGORY_AUDIO_INFO_0_PT_BR NULL
#define CATEGORY_INPUT_LABEL_PT_BR "Entrada"
#define CATEGORY_INPUT_INFO_0_PT_BR NULL
#define CATEGORY_HACKS_LABEL_PT_BR "Hacks de emulação"
#define CATEGORY_HACKS_INFO_0_PT_BR "Configure os parâmetros de precisão de overclock e emulação do processador que afetam o baixo desempenho e a compatibilidade."
#define CATEGORY_DIP_SWITCH_LABEL_PT_BR NULL
#define CATEGORY_DIP_SWITCH_INFO_0_PT_BR NULL
#define FCEUMM_REGION_LABEL_PT_BR NULL
#define FCEUMM_REGION_INFO_0_PT_BR NULL
#define OPTION_VAL_AUTO_PT_BR NULL
#define OPTION_VAL_NTSC_PT_BR NULL
#define OPTION_VAL_PAL_PT_BR NULL
#define OPTION_VAL_DENDY_PT_BR NULL
#define FCEUMM_GAME_GENIE_LABEL_PT_BR NULL
#define FCEUMM_GAME_GENIE_INFO_0_PT_BR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_PT_BR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_PT_BR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_PT_BR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_PT_BR NULL
#define FCEUMM_ASPECT_LABEL_PT_BR NULL
#define FCEUMM_ASPECT_INFO_0_PT_BR NULL
#define OPTION_VAL_8_7_PAR_PT_BR NULL
#define OPTION_VAL_4_3_PT_BR NULL
#define OPTION_VAL_PP_PT_BR NULL
#define FCEUMM_OVERSCAN_LABEL_PT_BR NULL
#define FCEUMM_OVERSCAN_INFO_0_PT_BR NULL
#define FCEUMM_OVERSCAN_H_LABEL_PT_BR NULL
#define FCEUMM_OVERSCAN_H_INFO_0_PT_BR NULL
#define FCEUMM_OVERSCAN_V_LABEL_PT_BR NULL
#define FCEUMM_OVERSCAN_V_INFO_0_PT_BR NULL
#define FCEUMM_PALETTE_LABEL_PT_BR NULL
#define FCEUMM_PALETTE_INFO_0_PT_BR NULL
#define OPTION_VAL_DEFAULT_PT_BR NULL
#define OPTION_VAL_ASQREALC_PT_BR NULL
#define OPTION_VAL_NINTENDO_VC_PT_BR NULL
#define OPTION_VAL_RGB_PT_BR NULL
#define OPTION_VAL_YUV_V3_PT_BR NULL
#define OPTION_VAL_UNSATURATED_FINAL_PT_BR NULL
#define OPTION_VAL_SONY_CXA2025AS_US_PT_BR NULL
#define OPTION_VAL_BMF_FINAL2_PT_BR NULL
#define OPTION_VAL_BMF_FINAL3_PT_BR NULL
#define OPTION_VAL_SMOOTH_FBX_PT_BR NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_PT_BR NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_PT_BR NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_PT_BR NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_PT_BR NULL
#define OPTION_VAL_NESCAP_PT_BR NULL
#define OPTION_VAL_WAVEBEAM_PT_BR NULL
#define OPTION_VAL_RAW_PT_BR NULL
#define OPTION_VAL_CUSTOM_PT_BR NULL
#define FCEUMM_NTSC_FILTER_LABEL_PT_BR NULL
#define FCEUMM_NTSC_FILTER_INFO_0_PT_BR NULL
#define OPTION_VAL_COMPOSITE_PT_BR NULL
#define OPTION_VAL_SVIDEO_PT_BR NULL
#define OPTION_VAL_RGB_O9_PT_BR NULL
#define OPTION_VAL_MONOCHROME_PT_BR NULL
#define FCEUMM_SNDQUALITY_LABEL_PT_BR NULL
#define FCEUMM_SNDQUALITY_INFO_0_PT_BR NULL
#define OPTION_VAL_LOW_PT_BR NULL
#define OPTION_VAL_HIGH_PT_BR NULL
#define OPTION_VAL_VERY_HIGH_PT_BR NULL
#define FCEUMM_SWAPDUTY_LABEL_PT_BR NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_PT_BR NULL
#define FCEUMM_SWAPDUTY_INFO_0_PT_BR NULL
#define FCEUMM_SNDVOLUME_LABEL_PT_BR NULL
#define FCEUMM_SNDVOLUME_INFO_0_PT_BR NULL
#define OPTION_VAL_0_PT_BR NULL
#define OPTION_VAL_1_PT_BR NULL
#define OPTION_VAL_2_PT_BR NULL
#define OPTION_VAL_3_PT_BR NULL
#define OPTION_VAL_4_PT_BR NULL
#define OPTION_VAL_5_PT_BR NULL
#define OPTION_VAL_6_PT_BR NULL
#define OPTION_VAL_7_PT_BR NULL
#define OPTION_VAL_8_PT_BR NULL
#define OPTION_VAL_9_PT_BR NULL
#define OPTION_VAL_10_PT_BR NULL
#define FCEUMM_APU_1_LABEL_PT_BR NULL
#define FCEUMM_APU_1_LABEL_CAT_PT_BR NULL
#define FCEUMM_APU_1_INFO_0_PT_BR NULL
#define FCEUMM_APU_2_LABEL_PT_BR NULL
#define FCEUMM_APU_2_LABEL_CAT_PT_BR NULL
#define FCEUMM_APU_2_INFO_0_PT_BR NULL
#define FCEUMM_APU_3_LABEL_PT_BR NULL
#define FCEUMM_APU_3_LABEL_CAT_PT_BR NULL
#define FCEUMM_APU_3_INFO_0_PT_BR NULL
#define FCEUMM_APU_4_LABEL_PT_BR NULL
#define FCEUMM_APU_4_LABEL_CAT_PT_BR NULL
#define FCEUMM_APU_4_INFO_0_PT_BR NULL
#define FCEUMM_APU_5_LABEL_PT_BR NULL
#define FCEUMM_APU_5_LABEL_CAT_PT_BR NULL
#define FCEUMM_APU_5_INFO_0_PT_BR NULL
#define FCEUMM_TURBO_ENABLE_LABEL_PT_BR NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_PT_BR NULL
#define OPTION_VAL_NONE_PT_BR NULL
#define OPTION_VAL_PLAYER_1_PT_BR NULL
#define OPTION_VAL_PLAYER_2_PT_BR NULL
#define OPTION_VAL_BOTH_PT_BR NULL
#define FCEUMM_TURBO_DELAY_LABEL_PT_BR NULL
#define FCEUMM_TURBO_DELAY_INFO_0_PT_BR NULL
#define FCEUMM_ZAPPER_MODE_LABEL_PT_BR NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_PT_BR NULL
#define OPTION_VAL_LIGHTGUN_PT_BR NULL
#define OPTION_VAL_TOUCHSCREEN_PT_BR NULL
#define OPTION_VAL_MOUSE_PT_BR NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_PT_BR NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_PT_BR NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_PT_BR NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_PT_BR NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_PT_BR NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_PT_BR NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_PT_BR NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_PT_BR NULL
#define FCEUMM_OVERCLOCKING_LABEL_PT_BR NULL
#define FCEUMM_OVERCLOCKING_INFO_0_PT_BR NULL
#define OPTION_VAL_2X_POSTRENDER_PT_BR NULL
#define OPTION_VAL_2X_VBLANK_PT_BR NULL
#define FCEUMM_RAMSTATE_LABEL_PT_BR NULL
#define FCEUMM_RAMSTATE_INFO_0_PT_BR NULL
#define OPTION_VAL_FILL_FF_PT_BR NULL
#define OPTION_VAL_FILL_00_PT_BR NULL
#define OPTION_VAL_RANDOM_PT_BR NULL

struct retro_core_option_v2_category option_cats_pt_br[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_PT_BR,
      CATEGORY_SYSTEM_INFO_0_PT_BR
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_PT_BR,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_PT_BR
#else
      CATEGORY_VIDEO_INFO_1_PT_BR
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_PT_BR,
      CATEGORY_AUDIO_INFO_0_PT_BR
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_PT_BR,
      CATEGORY_INPUT_INFO_0_PT_BR
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_PT_BR,
      CATEGORY_HACKS_INFO_0_PT_BR
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_PT_BR,
      CATEGORY_DIP_SWITCH_INFO_0_PT_BR
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_pt_br[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_PT_BR,
      NULL,
      FCEUMM_REGION_INFO_0_PT_BR,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_PT_BR },
         { "NTSC",  OPTION_VAL_NTSC_PT_BR },
         { "PAL",   OPTION_VAL_PAL_PT_BR },
         { "Dendy", OPTION_VAL_DENDY_PT_BR },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_PT_BR,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_PT_BR,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_PT_BR,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_PT_BR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_PT_BR,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_PT_BR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_PT_BR,
      NULL,
      FCEUMM_ASPECT_INFO_0_PT_BR,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_PT_BR },
         { "4:3",     OPTION_VAL_4_3_PT_BR },
         { "PP",      OPTION_VAL_PP_PT_BR },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_PT_BR,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_PT_BR,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_PT_BR,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_PT_BR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_PT_BR,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_PT_BR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_PT_BR,
      NULL,
      FCEUMM_PALETTE_INFO_0_PT_BR,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_PT_BR },
         { "asqrealc",             OPTION_VAL_ASQREALC_PT_BR },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_PT_BR },
         { "rgb",                  OPTION_VAL_RGB_PT_BR },
         { "yuv-v3",               OPTION_VAL_YUV_V3_PT_BR },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_PT_BR },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_PT_BR },
         { "pal",                  OPTION_VAL_PAL_PT_BR },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_PT_BR },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_PT_BR },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_PT_BR },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_PT_BR },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_PT_BR },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_PT_BR },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_PT_BR },
         { "nescap",               OPTION_VAL_NESCAP_PT_BR },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_PT_BR },
         { "raw",                  OPTION_VAL_RAW_PT_BR },
         { "custom",               OPTION_VAL_CUSTOM_PT_BR },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_PT_BR,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_PT_BR,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_PT_BR },
         { "svideo",     OPTION_VAL_SVIDEO_PT_BR },
         { "rgb",        OPTION_VAL_RGB_O9_PT_BR },
         { "monochrome", OPTION_VAL_MONOCHROME_PT_BR },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_PT_BR,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_PT_BR },
         { "High",      OPTION_VAL_HIGH_PT_BR },
         { "Very High", OPTION_VAL_VERY_HIGH_PT_BR },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_PT_BR,
      FCEUMM_SWAPDUTY_LABEL_CAT_PT_BR,
      FCEUMM_SWAPDUTY_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_PT_BR,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_PT_BR },
         { "1",  OPTION_VAL_1_PT_BR },
         { "2",  OPTION_VAL_2_PT_BR },
         { "3",  OPTION_VAL_3_PT_BR },
         { "4",  OPTION_VAL_4_PT_BR },
         { "5",  OPTION_VAL_5_PT_BR },
         { "6",  OPTION_VAL_6_PT_BR },
         { "7",  OPTION_VAL_7_PT_BR },
         { "8",  OPTION_VAL_8_PT_BR },
         { "9",  OPTION_VAL_9_PT_BR },
         { "10", OPTION_VAL_10_PT_BR },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_PT_BR,
      FCEUMM_APU_1_LABEL_CAT_PT_BR,
      FCEUMM_APU_1_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_PT_BR,
      FCEUMM_APU_2_LABEL_CAT_PT_BR,
      FCEUMM_APU_2_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_PT_BR,
      FCEUMM_APU_3_LABEL_CAT_PT_BR,
      FCEUMM_APU_3_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_PT_BR,
      FCEUMM_APU_4_LABEL_CAT_PT_BR,
      FCEUMM_APU_4_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_PT_BR,
      FCEUMM_APU_5_LABEL_CAT_PT_BR,
      FCEUMM_APU_5_INFO_0_PT_BR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_PT_BR,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_PT_BR,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_PT_BR },
         { "Player 1", OPTION_VAL_PLAYER_1_PT_BR },
         { "Player 2", OPTION_VAL_PLAYER_2_PT_BR },
         { "Both",     OPTION_VAL_BOTH_PT_BR },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_PT_BR,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_PT_BR,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_PT_BR,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_PT_BR,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_PT_BR },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_PT_BR },
         { "mouse",       OPTION_VAL_MOUSE_PT_BR },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_PT_BR,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_PT_BR,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_PT_BR,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_PT_BR,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_PT_BR,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_PT_BR,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_PT_BR,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_PT_BR,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_PT_BR,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_PT_BR,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_PT_BR },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_PT_BR },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_PT_BR,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_PT_BR,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_PT_BR },
         { "fill $00", OPTION_VAL_FILL_00_PT_BR },
         { "random",   OPTION_VAL_RANDOM_PT_BR },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_pt_br = {
   option_cats_pt_br,
   option_defs_pt_br
};

/* RETRO_LANGUAGE_PT_PT */

#define CATEGORY_SYSTEM_LABEL_PT_PT NULL
#define CATEGORY_SYSTEM_INFO_0_PT_PT NULL
#define CATEGORY_VIDEO_LABEL_PT_PT "Vídeo"
#define CATEGORY_VIDEO_INFO_0_PT_PT NULL
#define CATEGORY_VIDEO_INFO_1_PT_PT NULL
#define CATEGORY_AUDIO_LABEL_PT_PT "Áudio"
#define CATEGORY_AUDIO_INFO_0_PT_PT NULL
#define CATEGORY_INPUT_LABEL_PT_PT "Entrada"
#define CATEGORY_INPUT_INFO_0_PT_PT NULL
#define CATEGORY_HACKS_LABEL_PT_PT NULL
#define CATEGORY_HACKS_INFO_0_PT_PT NULL
#define CATEGORY_DIP_SWITCH_LABEL_PT_PT NULL
#define CATEGORY_DIP_SWITCH_INFO_0_PT_PT NULL
#define FCEUMM_REGION_LABEL_PT_PT NULL
#define FCEUMM_REGION_INFO_0_PT_PT NULL
#define OPTION_VAL_AUTO_PT_PT NULL
#define OPTION_VAL_NTSC_PT_PT NULL
#define OPTION_VAL_PAL_PT_PT NULL
#define OPTION_VAL_DENDY_PT_PT NULL
#define FCEUMM_GAME_GENIE_LABEL_PT_PT NULL
#define FCEUMM_GAME_GENIE_INFO_0_PT_PT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_PT_PT NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_PT_PT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_PT_PT NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_PT_PT NULL
#define FCEUMM_ASPECT_LABEL_PT_PT NULL
#define FCEUMM_ASPECT_INFO_0_PT_PT NULL
#define OPTION_VAL_8_7_PAR_PT_PT NULL
#define OPTION_VAL_4_3_PT_PT NULL
#define OPTION_VAL_PP_PT_PT NULL
#define FCEUMM_OVERSCAN_LABEL_PT_PT NULL
#define FCEUMM_OVERSCAN_INFO_0_PT_PT NULL
#define FCEUMM_OVERSCAN_H_LABEL_PT_PT NULL
#define FCEUMM_OVERSCAN_H_INFO_0_PT_PT NULL
#define FCEUMM_OVERSCAN_V_LABEL_PT_PT NULL
#define FCEUMM_OVERSCAN_V_INFO_0_PT_PT NULL
#define FCEUMM_PALETTE_LABEL_PT_PT NULL
#define FCEUMM_PALETTE_INFO_0_PT_PT NULL
#define OPTION_VAL_DEFAULT_PT_PT NULL
#define OPTION_VAL_ASQREALC_PT_PT NULL
#define OPTION_VAL_NINTENDO_VC_PT_PT NULL
#define OPTION_VAL_RGB_PT_PT NULL
#define OPTION_VAL_YUV_V3_PT_PT NULL
#define OPTION_VAL_UNSATURATED_FINAL_PT_PT NULL
#define OPTION_VAL_SONY_CXA2025AS_US_PT_PT NULL
#define OPTION_VAL_BMF_FINAL2_PT_PT NULL
#define OPTION_VAL_BMF_FINAL3_PT_PT NULL
#define OPTION_VAL_SMOOTH_FBX_PT_PT NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_PT_PT NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_PT_PT NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_PT_PT NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_PT_PT NULL
#define OPTION_VAL_NESCAP_PT_PT NULL
#define OPTION_VAL_WAVEBEAM_PT_PT NULL
#define OPTION_VAL_RAW_PT_PT NULL
#define OPTION_VAL_CUSTOM_PT_PT NULL
#define FCEUMM_NTSC_FILTER_LABEL_PT_PT NULL
#define FCEUMM_NTSC_FILTER_INFO_0_PT_PT NULL
#define OPTION_VAL_COMPOSITE_PT_PT NULL
#define OPTION_VAL_SVIDEO_PT_PT NULL
#define OPTION_VAL_RGB_O9_PT_PT NULL
#define OPTION_VAL_MONOCHROME_PT_PT NULL
#define FCEUMM_SNDQUALITY_LABEL_PT_PT NULL
#define FCEUMM_SNDQUALITY_INFO_0_PT_PT NULL
#define OPTION_VAL_LOW_PT_PT NULL
#define OPTION_VAL_HIGH_PT_PT NULL
#define OPTION_VAL_VERY_HIGH_PT_PT NULL
#define FCEUMM_SWAPDUTY_LABEL_PT_PT NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_PT_PT NULL
#define FCEUMM_SWAPDUTY_INFO_0_PT_PT NULL
#define FCEUMM_SNDVOLUME_LABEL_PT_PT NULL
#define FCEUMM_SNDVOLUME_INFO_0_PT_PT NULL
#define OPTION_VAL_0_PT_PT NULL
#define OPTION_VAL_1_PT_PT NULL
#define OPTION_VAL_2_PT_PT NULL
#define OPTION_VAL_3_PT_PT NULL
#define OPTION_VAL_4_PT_PT NULL
#define OPTION_VAL_5_PT_PT NULL
#define OPTION_VAL_6_PT_PT NULL
#define OPTION_VAL_7_PT_PT NULL
#define OPTION_VAL_8_PT_PT NULL
#define OPTION_VAL_9_PT_PT NULL
#define OPTION_VAL_10_PT_PT NULL
#define FCEUMM_APU_1_LABEL_PT_PT NULL
#define FCEUMM_APU_1_LABEL_CAT_PT_PT NULL
#define FCEUMM_APU_1_INFO_0_PT_PT NULL
#define FCEUMM_APU_2_LABEL_PT_PT NULL
#define FCEUMM_APU_2_LABEL_CAT_PT_PT NULL
#define FCEUMM_APU_2_INFO_0_PT_PT NULL
#define FCEUMM_APU_3_LABEL_PT_PT NULL
#define FCEUMM_APU_3_LABEL_CAT_PT_PT NULL
#define FCEUMM_APU_3_INFO_0_PT_PT NULL
#define FCEUMM_APU_4_LABEL_PT_PT NULL
#define FCEUMM_APU_4_LABEL_CAT_PT_PT NULL
#define FCEUMM_APU_4_INFO_0_PT_PT NULL
#define FCEUMM_APU_5_LABEL_PT_PT NULL
#define FCEUMM_APU_5_LABEL_CAT_PT_PT NULL
#define FCEUMM_APU_5_INFO_0_PT_PT NULL
#define FCEUMM_TURBO_ENABLE_LABEL_PT_PT NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_PT_PT NULL
#define OPTION_VAL_NONE_PT_PT NULL
#define OPTION_VAL_PLAYER_1_PT_PT NULL
#define OPTION_VAL_PLAYER_2_PT_PT NULL
#define OPTION_VAL_BOTH_PT_PT NULL
#define FCEUMM_TURBO_DELAY_LABEL_PT_PT NULL
#define FCEUMM_TURBO_DELAY_INFO_0_PT_PT NULL
#define FCEUMM_ZAPPER_MODE_LABEL_PT_PT NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_PT_PT NULL
#define OPTION_VAL_LIGHTGUN_PT_PT NULL
#define OPTION_VAL_TOUCHSCREEN_PT_PT NULL
#define OPTION_VAL_MOUSE_PT_PT NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_PT_PT NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_PT_PT NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_PT_PT NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_PT_PT NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_PT_PT NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_PT_PT NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_PT_PT NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_PT_PT NULL
#define FCEUMM_OVERCLOCKING_LABEL_PT_PT NULL
#define FCEUMM_OVERCLOCKING_INFO_0_PT_PT NULL
#define OPTION_VAL_2X_POSTRENDER_PT_PT NULL
#define OPTION_VAL_2X_VBLANK_PT_PT NULL
#define FCEUMM_RAMSTATE_LABEL_PT_PT NULL
#define FCEUMM_RAMSTATE_INFO_0_PT_PT NULL
#define OPTION_VAL_FILL_FF_PT_PT NULL
#define OPTION_VAL_FILL_00_PT_PT NULL
#define OPTION_VAL_RANDOM_PT_PT NULL

struct retro_core_option_v2_category option_cats_pt_pt[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_PT_PT,
      CATEGORY_SYSTEM_INFO_0_PT_PT
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_PT_PT,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_PT_PT
#else
      CATEGORY_VIDEO_INFO_1_PT_PT
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_PT_PT,
      CATEGORY_AUDIO_INFO_0_PT_PT
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_PT_PT,
      CATEGORY_INPUT_INFO_0_PT_PT
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_PT_PT,
      CATEGORY_HACKS_INFO_0_PT_PT
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_PT_PT,
      CATEGORY_DIP_SWITCH_INFO_0_PT_PT
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_pt_pt[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_PT_PT,
      NULL,
      FCEUMM_REGION_INFO_0_PT_PT,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_PT_PT },
         { "NTSC",  OPTION_VAL_NTSC_PT_PT },
         { "PAL",   OPTION_VAL_PAL_PT_PT },
         { "Dendy", OPTION_VAL_DENDY_PT_PT },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_PT_PT,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_PT_PT,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_PT_PT,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_PT_PT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_PT_PT,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_PT_PT,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_PT_PT,
      NULL,
      FCEUMM_ASPECT_INFO_0_PT_PT,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_PT_PT },
         { "4:3",     OPTION_VAL_4_3_PT_PT },
         { "PP",      OPTION_VAL_PP_PT_PT },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_PT_PT,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_PT_PT,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_PT_PT,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_PT_PT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_PT_PT,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_PT_PT,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_PT_PT,
      NULL,
      FCEUMM_PALETTE_INFO_0_PT_PT,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_PT_PT },
         { "asqrealc",             OPTION_VAL_ASQREALC_PT_PT },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_PT_PT },
         { "rgb",                  OPTION_VAL_RGB_PT_PT },
         { "yuv-v3",               OPTION_VAL_YUV_V3_PT_PT },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_PT_PT },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_PT_PT },
         { "pal",                  OPTION_VAL_PAL_PT_PT },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_PT_PT },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_PT_PT },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_PT_PT },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_PT_PT },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_PT_PT },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_PT_PT },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_PT_PT },
         { "nescap",               OPTION_VAL_NESCAP_PT_PT },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_PT_PT },
         { "raw",                  OPTION_VAL_RAW_PT_PT },
         { "custom",               OPTION_VAL_CUSTOM_PT_PT },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_PT_PT,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_PT_PT,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_PT_PT },
         { "svideo",     OPTION_VAL_SVIDEO_PT_PT },
         { "rgb",        OPTION_VAL_RGB_O9_PT_PT },
         { "monochrome", OPTION_VAL_MONOCHROME_PT_PT },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_PT_PT,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_PT_PT },
         { "High",      OPTION_VAL_HIGH_PT_PT },
         { "Very High", OPTION_VAL_VERY_HIGH_PT_PT },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_PT_PT,
      FCEUMM_SWAPDUTY_LABEL_CAT_PT_PT,
      FCEUMM_SWAPDUTY_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_PT_PT,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_PT_PT },
         { "1",  OPTION_VAL_1_PT_PT },
         { "2",  OPTION_VAL_2_PT_PT },
         { "3",  OPTION_VAL_3_PT_PT },
         { "4",  OPTION_VAL_4_PT_PT },
         { "5",  OPTION_VAL_5_PT_PT },
         { "6",  OPTION_VAL_6_PT_PT },
         { "7",  OPTION_VAL_7_PT_PT },
         { "8",  OPTION_VAL_8_PT_PT },
         { "9",  OPTION_VAL_9_PT_PT },
         { "10", OPTION_VAL_10_PT_PT },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_PT_PT,
      FCEUMM_APU_1_LABEL_CAT_PT_PT,
      FCEUMM_APU_1_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_PT_PT,
      FCEUMM_APU_2_LABEL_CAT_PT_PT,
      FCEUMM_APU_2_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_PT_PT,
      FCEUMM_APU_3_LABEL_CAT_PT_PT,
      FCEUMM_APU_3_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_PT_PT,
      FCEUMM_APU_4_LABEL_CAT_PT_PT,
      FCEUMM_APU_4_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_PT_PT,
      FCEUMM_APU_5_LABEL_CAT_PT_PT,
      FCEUMM_APU_5_INFO_0_PT_PT,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_PT_PT,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_PT_PT,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_PT_PT },
         { "Player 1", OPTION_VAL_PLAYER_1_PT_PT },
         { "Player 2", OPTION_VAL_PLAYER_2_PT_PT },
         { "Both",     OPTION_VAL_BOTH_PT_PT },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_PT_PT,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_PT_PT,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_PT_PT,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_PT_PT,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_PT_PT },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_PT_PT },
         { "mouse",       OPTION_VAL_MOUSE_PT_PT },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_PT_PT,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_PT_PT,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_PT_PT,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_PT_PT,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_PT_PT,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_PT_PT,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_PT_PT,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_PT_PT,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_PT_PT,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_PT_PT,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_PT_PT },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_PT_PT },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_PT_PT,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_PT_PT,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_PT_PT },
         { "fill $00", OPTION_VAL_FILL_00_PT_PT },
         { "random",   OPTION_VAL_RANDOM_PT_PT },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_pt_pt = {
   option_cats_pt_pt,
   option_defs_pt_pt
};

/* RETRO_LANGUAGE_RO */

#define CATEGORY_SYSTEM_LABEL_RO NULL
#define CATEGORY_SYSTEM_INFO_0_RO NULL
#define CATEGORY_VIDEO_LABEL_RO NULL
#define CATEGORY_VIDEO_INFO_0_RO NULL
#define CATEGORY_VIDEO_INFO_1_RO NULL
#define CATEGORY_AUDIO_LABEL_RO NULL
#define CATEGORY_AUDIO_INFO_0_RO NULL
#define CATEGORY_INPUT_LABEL_RO NULL
#define CATEGORY_INPUT_INFO_0_RO NULL
#define CATEGORY_HACKS_LABEL_RO NULL
#define CATEGORY_HACKS_INFO_0_RO NULL
#define CATEGORY_DIP_SWITCH_LABEL_RO NULL
#define CATEGORY_DIP_SWITCH_INFO_0_RO NULL
#define FCEUMM_REGION_LABEL_RO NULL
#define FCEUMM_REGION_INFO_0_RO NULL
#define OPTION_VAL_AUTO_RO NULL
#define OPTION_VAL_NTSC_RO NULL
#define OPTION_VAL_PAL_RO NULL
#define OPTION_VAL_DENDY_RO NULL
#define FCEUMM_GAME_GENIE_LABEL_RO NULL
#define FCEUMM_GAME_GENIE_INFO_0_RO NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_RO NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_RO NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_RO NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_RO NULL
#define FCEUMM_ASPECT_LABEL_RO NULL
#define FCEUMM_ASPECT_INFO_0_RO NULL
#define OPTION_VAL_8_7_PAR_RO NULL
#define OPTION_VAL_4_3_RO NULL
#define OPTION_VAL_PP_RO NULL
#define FCEUMM_OVERSCAN_LABEL_RO NULL
#define FCEUMM_OVERSCAN_INFO_0_RO NULL
#define FCEUMM_OVERSCAN_H_LABEL_RO NULL
#define FCEUMM_OVERSCAN_H_INFO_0_RO NULL
#define FCEUMM_OVERSCAN_V_LABEL_RO NULL
#define FCEUMM_OVERSCAN_V_INFO_0_RO NULL
#define FCEUMM_PALETTE_LABEL_RO NULL
#define FCEUMM_PALETTE_INFO_0_RO NULL
#define OPTION_VAL_DEFAULT_RO NULL
#define OPTION_VAL_ASQREALC_RO NULL
#define OPTION_VAL_NINTENDO_VC_RO NULL
#define OPTION_VAL_RGB_RO NULL
#define OPTION_VAL_YUV_V3_RO NULL
#define OPTION_VAL_UNSATURATED_FINAL_RO NULL
#define OPTION_VAL_SONY_CXA2025AS_US_RO NULL
#define OPTION_VAL_BMF_FINAL2_RO NULL
#define OPTION_VAL_BMF_FINAL3_RO NULL
#define OPTION_VAL_SMOOTH_FBX_RO NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_RO NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_RO NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_RO NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_RO NULL
#define OPTION_VAL_NESCAP_RO NULL
#define OPTION_VAL_WAVEBEAM_RO NULL
#define OPTION_VAL_RAW_RO NULL
#define OPTION_VAL_CUSTOM_RO NULL
#define FCEUMM_NTSC_FILTER_LABEL_RO NULL
#define FCEUMM_NTSC_FILTER_INFO_0_RO NULL
#define OPTION_VAL_COMPOSITE_RO NULL
#define OPTION_VAL_SVIDEO_RO NULL
#define OPTION_VAL_RGB_O9_RO NULL
#define OPTION_VAL_MONOCHROME_RO NULL
#define FCEUMM_SNDQUALITY_LABEL_RO NULL
#define FCEUMM_SNDQUALITY_INFO_0_RO NULL
#define OPTION_VAL_LOW_RO NULL
#define OPTION_VAL_HIGH_RO NULL
#define OPTION_VAL_VERY_HIGH_RO NULL
#define FCEUMM_SWAPDUTY_LABEL_RO NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_RO NULL
#define FCEUMM_SWAPDUTY_INFO_0_RO NULL
#define FCEUMM_SNDVOLUME_LABEL_RO NULL
#define FCEUMM_SNDVOLUME_INFO_0_RO NULL
#define OPTION_VAL_0_RO NULL
#define OPTION_VAL_1_RO NULL
#define OPTION_VAL_2_RO NULL
#define OPTION_VAL_3_RO NULL
#define OPTION_VAL_4_RO NULL
#define OPTION_VAL_5_RO NULL
#define OPTION_VAL_6_RO NULL
#define OPTION_VAL_7_RO NULL
#define OPTION_VAL_8_RO NULL
#define OPTION_VAL_9_RO NULL
#define OPTION_VAL_10_RO NULL
#define FCEUMM_APU_1_LABEL_RO NULL
#define FCEUMM_APU_1_LABEL_CAT_RO NULL
#define FCEUMM_APU_1_INFO_0_RO NULL
#define FCEUMM_APU_2_LABEL_RO NULL
#define FCEUMM_APU_2_LABEL_CAT_RO NULL
#define FCEUMM_APU_2_INFO_0_RO NULL
#define FCEUMM_APU_3_LABEL_RO NULL
#define FCEUMM_APU_3_LABEL_CAT_RO NULL
#define FCEUMM_APU_3_INFO_0_RO NULL
#define FCEUMM_APU_4_LABEL_RO NULL
#define FCEUMM_APU_4_LABEL_CAT_RO NULL
#define FCEUMM_APU_4_INFO_0_RO NULL
#define FCEUMM_APU_5_LABEL_RO NULL
#define FCEUMM_APU_5_LABEL_CAT_RO NULL
#define FCEUMM_APU_5_INFO_0_RO NULL
#define FCEUMM_TURBO_ENABLE_LABEL_RO NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_RO NULL
#define OPTION_VAL_NONE_RO NULL
#define OPTION_VAL_PLAYER_1_RO NULL
#define OPTION_VAL_PLAYER_2_RO NULL
#define OPTION_VAL_BOTH_RO NULL
#define FCEUMM_TURBO_DELAY_LABEL_RO NULL
#define FCEUMM_TURBO_DELAY_INFO_0_RO NULL
#define FCEUMM_ZAPPER_MODE_LABEL_RO NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_RO NULL
#define OPTION_VAL_LIGHTGUN_RO NULL
#define OPTION_VAL_TOUCHSCREEN_RO NULL
#define OPTION_VAL_MOUSE_RO NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_RO NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_RO NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_RO NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_RO NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_RO NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_RO NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_RO NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_RO NULL
#define FCEUMM_OVERCLOCKING_LABEL_RO NULL
#define FCEUMM_OVERCLOCKING_INFO_0_RO NULL
#define OPTION_VAL_2X_POSTRENDER_RO NULL
#define OPTION_VAL_2X_VBLANK_RO NULL
#define FCEUMM_RAMSTATE_LABEL_RO NULL
#define FCEUMM_RAMSTATE_INFO_0_RO NULL
#define OPTION_VAL_FILL_FF_RO NULL
#define OPTION_VAL_FILL_00_RO NULL
#define OPTION_VAL_RANDOM_RO NULL

struct retro_core_option_v2_category option_cats_ro[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_RO,
      CATEGORY_SYSTEM_INFO_0_RO
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_RO,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_RO
#else
      CATEGORY_VIDEO_INFO_1_RO
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_RO,
      CATEGORY_AUDIO_INFO_0_RO
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_RO,
      CATEGORY_INPUT_INFO_0_RO
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_RO,
      CATEGORY_HACKS_INFO_0_RO
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_RO,
      CATEGORY_DIP_SWITCH_INFO_0_RO
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_ro[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_RO,
      NULL,
      FCEUMM_REGION_INFO_0_RO,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_RO },
         { "NTSC",  OPTION_VAL_NTSC_RO },
         { "PAL",   OPTION_VAL_PAL_RO },
         { "Dendy", OPTION_VAL_DENDY_RO },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_RO,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_RO,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_RO,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_RO,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_RO,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_RO,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_RO,
      NULL,
      FCEUMM_ASPECT_INFO_0_RO,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_RO },
         { "4:3",     OPTION_VAL_4_3_RO },
         { "PP",      OPTION_VAL_PP_RO },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_RO,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_RO,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_RO,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_RO,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_RO,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_RO,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_RO,
      NULL,
      FCEUMM_PALETTE_INFO_0_RO,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_RO },
         { "asqrealc",             OPTION_VAL_ASQREALC_RO },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_RO },
         { "rgb",                  OPTION_VAL_RGB_RO },
         { "yuv-v3",               OPTION_VAL_YUV_V3_RO },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_RO },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_RO },
         { "pal",                  OPTION_VAL_PAL_RO },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_RO },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_RO },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_RO },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_RO },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_RO },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_RO },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_RO },
         { "nescap",               OPTION_VAL_NESCAP_RO },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_RO },
         { "raw",                  OPTION_VAL_RAW_RO },
         { "custom",               OPTION_VAL_CUSTOM_RO },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_RO,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_RO,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_RO },
         { "svideo",     OPTION_VAL_SVIDEO_RO },
         { "rgb",        OPTION_VAL_RGB_O9_RO },
         { "monochrome", OPTION_VAL_MONOCHROME_RO },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_RO,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_RO,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_RO },
         { "High",      OPTION_VAL_HIGH_RO },
         { "Very High", OPTION_VAL_VERY_HIGH_RO },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_RO,
      FCEUMM_SWAPDUTY_LABEL_CAT_RO,
      FCEUMM_SWAPDUTY_INFO_0_RO,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_RO,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_RO,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_RO },
         { "1",  OPTION_VAL_1_RO },
         { "2",  OPTION_VAL_2_RO },
         { "3",  OPTION_VAL_3_RO },
         { "4",  OPTION_VAL_4_RO },
         { "5",  OPTION_VAL_5_RO },
         { "6",  OPTION_VAL_6_RO },
         { "7",  OPTION_VAL_7_RO },
         { "8",  OPTION_VAL_8_RO },
         { "9",  OPTION_VAL_9_RO },
         { "10", OPTION_VAL_10_RO },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_RO,
      FCEUMM_APU_1_LABEL_CAT_RO,
      FCEUMM_APU_1_INFO_0_RO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_RO,
      FCEUMM_APU_2_LABEL_CAT_RO,
      FCEUMM_APU_2_INFO_0_RO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_RO,
      FCEUMM_APU_3_LABEL_CAT_RO,
      FCEUMM_APU_3_INFO_0_RO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_RO,
      FCEUMM_APU_4_LABEL_CAT_RO,
      FCEUMM_APU_4_INFO_0_RO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_RO,
      FCEUMM_APU_5_LABEL_CAT_RO,
      FCEUMM_APU_5_INFO_0_RO,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_RO,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_RO,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_RO },
         { "Player 1", OPTION_VAL_PLAYER_1_RO },
         { "Player 2", OPTION_VAL_PLAYER_2_RO },
         { "Both",     OPTION_VAL_BOTH_RO },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_RO,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_RO,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_RO,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_RO,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_RO },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_RO },
         { "mouse",       OPTION_VAL_MOUSE_RO },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_RO,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_RO,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_RO,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_RO,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_RO,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_RO,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_RO,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_RO,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_RO,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_RO,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_RO },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_RO },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_RO,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_RO,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_RO },
         { "fill $00", OPTION_VAL_FILL_00_RO },
         { "random",   OPTION_VAL_RANDOM_RO },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_ro = {
   option_cats_ro,
   option_defs_ro
};

/* RETRO_LANGUAGE_RU */

#define CATEGORY_SYSTEM_LABEL_RU "Системные"
#define CATEGORY_SYSTEM_INFO_0_RU NULL
#define CATEGORY_VIDEO_LABEL_RU "Видео"
#define CATEGORY_VIDEO_INFO_0_RU NULL
#define CATEGORY_VIDEO_INFO_1_RU NULL
#define CATEGORY_AUDIO_LABEL_RU "Аудио"
#define CATEGORY_AUDIO_INFO_0_RU NULL
#define CATEGORY_INPUT_LABEL_RU "Ввод"
#define CATEGORY_INPUT_INFO_0_RU NULL
#define CATEGORY_HACKS_LABEL_RU "Хаки эмуляции"
#define CATEGORY_HACKS_INFO_0_RU "Настройка разгона процессора и параметров точности эмуляции, влияющих на производительность и совместимость."
#define CATEGORY_DIP_SWITCH_LABEL_RU NULL
#define CATEGORY_DIP_SWITCH_INFO_0_RU NULL
#define FCEUMM_REGION_LABEL_RU NULL
#define FCEUMM_REGION_INFO_0_RU NULL
#define OPTION_VAL_AUTO_RU NULL
#define OPTION_VAL_NTSC_RU NULL
#define OPTION_VAL_PAL_RU NULL
#define OPTION_VAL_DENDY_RU NULL
#define FCEUMM_GAME_GENIE_LABEL_RU NULL
#define FCEUMM_GAME_GENIE_INFO_0_RU NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_RU NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_RU NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_RU NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_RU NULL
#define FCEUMM_ASPECT_LABEL_RU NULL
#define FCEUMM_ASPECT_INFO_0_RU NULL
#define OPTION_VAL_8_7_PAR_RU NULL
#define OPTION_VAL_4_3_RU NULL
#define OPTION_VAL_PP_RU NULL
#define FCEUMM_OVERSCAN_LABEL_RU NULL
#define FCEUMM_OVERSCAN_INFO_0_RU NULL
#define FCEUMM_OVERSCAN_H_LABEL_RU NULL
#define FCEUMM_OVERSCAN_H_INFO_0_RU NULL
#define FCEUMM_OVERSCAN_V_LABEL_RU NULL
#define FCEUMM_OVERSCAN_V_INFO_0_RU NULL
#define FCEUMM_PALETTE_LABEL_RU NULL
#define FCEUMM_PALETTE_INFO_0_RU NULL
#define OPTION_VAL_DEFAULT_RU NULL
#define OPTION_VAL_ASQREALC_RU NULL
#define OPTION_VAL_NINTENDO_VC_RU NULL
#define OPTION_VAL_RGB_RU NULL
#define OPTION_VAL_YUV_V3_RU NULL
#define OPTION_VAL_UNSATURATED_FINAL_RU NULL
#define OPTION_VAL_SONY_CXA2025AS_US_RU NULL
#define OPTION_VAL_BMF_FINAL2_RU NULL
#define OPTION_VAL_BMF_FINAL3_RU NULL
#define OPTION_VAL_SMOOTH_FBX_RU NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_RU NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_RU NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_RU NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_RU NULL
#define OPTION_VAL_NESCAP_RU NULL
#define OPTION_VAL_WAVEBEAM_RU NULL
#define OPTION_VAL_RAW_RU NULL
#define OPTION_VAL_CUSTOM_RU NULL
#define FCEUMM_NTSC_FILTER_LABEL_RU NULL
#define FCEUMM_NTSC_FILTER_INFO_0_RU NULL
#define OPTION_VAL_COMPOSITE_RU NULL
#define OPTION_VAL_SVIDEO_RU NULL
#define OPTION_VAL_RGB_O9_RU NULL
#define OPTION_VAL_MONOCHROME_RU NULL
#define FCEUMM_SNDQUALITY_LABEL_RU NULL
#define FCEUMM_SNDQUALITY_INFO_0_RU NULL
#define OPTION_VAL_LOW_RU NULL
#define OPTION_VAL_HIGH_RU NULL
#define OPTION_VAL_VERY_HIGH_RU NULL
#define FCEUMM_SWAPDUTY_LABEL_RU NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_RU NULL
#define FCEUMM_SWAPDUTY_INFO_0_RU NULL
#define FCEUMM_SNDVOLUME_LABEL_RU NULL
#define FCEUMM_SNDVOLUME_INFO_0_RU NULL
#define OPTION_VAL_0_RU NULL
#define OPTION_VAL_1_RU NULL
#define OPTION_VAL_2_RU NULL
#define OPTION_VAL_3_RU NULL
#define OPTION_VAL_4_RU NULL
#define OPTION_VAL_5_RU NULL
#define OPTION_VAL_6_RU NULL
#define OPTION_VAL_7_RU NULL
#define OPTION_VAL_8_RU NULL
#define OPTION_VAL_9_RU NULL
#define OPTION_VAL_10_RU NULL
#define FCEUMM_APU_1_LABEL_RU NULL
#define FCEUMM_APU_1_LABEL_CAT_RU NULL
#define FCEUMM_APU_1_INFO_0_RU NULL
#define FCEUMM_APU_2_LABEL_RU NULL
#define FCEUMM_APU_2_LABEL_CAT_RU NULL
#define FCEUMM_APU_2_INFO_0_RU NULL
#define FCEUMM_APU_3_LABEL_RU NULL
#define FCEUMM_APU_3_LABEL_CAT_RU NULL
#define FCEUMM_APU_3_INFO_0_RU NULL
#define FCEUMM_APU_4_LABEL_RU NULL
#define FCEUMM_APU_4_LABEL_CAT_RU NULL
#define FCEUMM_APU_4_INFO_0_RU NULL
#define FCEUMM_APU_5_LABEL_RU NULL
#define FCEUMM_APU_5_LABEL_CAT_RU NULL
#define FCEUMM_APU_5_INFO_0_RU NULL
#define FCEUMM_TURBO_ENABLE_LABEL_RU NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_RU NULL
#define OPTION_VAL_NONE_RU NULL
#define OPTION_VAL_PLAYER_1_RU NULL
#define OPTION_VAL_PLAYER_2_RU NULL
#define OPTION_VAL_BOTH_RU NULL
#define FCEUMM_TURBO_DELAY_LABEL_RU NULL
#define FCEUMM_TURBO_DELAY_INFO_0_RU NULL
#define FCEUMM_ZAPPER_MODE_LABEL_RU NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_RU NULL
#define OPTION_VAL_LIGHTGUN_RU NULL
#define OPTION_VAL_TOUCHSCREEN_RU NULL
#define OPTION_VAL_MOUSE_RU NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_RU NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_RU NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_RU NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_RU NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_RU NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_RU NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_RU NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_RU NULL
#define FCEUMM_OVERCLOCKING_LABEL_RU NULL
#define FCEUMM_OVERCLOCKING_INFO_0_RU NULL
#define OPTION_VAL_2X_POSTRENDER_RU NULL
#define OPTION_VAL_2X_VBLANK_RU NULL
#define FCEUMM_RAMSTATE_LABEL_RU NULL
#define FCEUMM_RAMSTATE_INFO_0_RU NULL
#define OPTION_VAL_FILL_FF_RU NULL
#define OPTION_VAL_FILL_00_RU NULL
#define OPTION_VAL_RANDOM_RU NULL

struct retro_core_option_v2_category option_cats_ru[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_RU,
      CATEGORY_SYSTEM_INFO_0_RU
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_RU,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_RU
#else
      CATEGORY_VIDEO_INFO_1_RU
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_RU,
      CATEGORY_AUDIO_INFO_0_RU
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_RU,
      CATEGORY_INPUT_INFO_0_RU
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_RU,
      CATEGORY_HACKS_INFO_0_RU
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_RU,
      CATEGORY_DIP_SWITCH_INFO_0_RU
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_ru[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_RU,
      NULL,
      FCEUMM_REGION_INFO_0_RU,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_RU },
         { "NTSC",  OPTION_VAL_NTSC_RU },
         { "PAL",   OPTION_VAL_PAL_RU },
         { "Dendy", OPTION_VAL_DENDY_RU },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_RU,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_RU,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_RU,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_RU,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_RU,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_RU,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_RU,
      NULL,
      FCEUMM_ASPECT_INFO_0_RU,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_RU },
         { "4:3",     OPTION_VAL_4_3_RU },
         { "PP",      OPTION_VAL_PP_RU },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_RU,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_RU,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_RU,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_RU,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_RU,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_RU,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_RU,
      NULL,
      FCEUMM_PALETTE_INFO_0_RU,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_RU },
         { "asqrealc",             OPTION_VAL_ASQREALC_RU },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_RU },
         { "rgb",                  OPTION_VAL_RGB_RU },
         { "yuv-v3",               OPTION_VAL_YUV_V3_RU },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_RU },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_RU },
         { "pal",                  OPTION_VAL_PAL_RU },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_RU },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_RU },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_RU },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_RU },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_RU },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_RU },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_RU },
         { "nescap",               OPTION_VAL_NESCAP_RU },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_RU },
         { "raw",                  OPTION_VAL_RAW_RU },
         { "custom",               OPTION_VAL_CUSTOM_RU },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_RU,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_RU,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_RU },
         { "svideo",     OPTION_VAL_SVIDEO_RU },
         { "rgb",        OPTION_VAL_RGB_O9_RU },
         { "monochrome", OPTION_VAL_MONOCHROME_RU },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_RU,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_RU,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_RU },
         { "High",      OPTION_VAL_HIGH_RU },
         { "Very High", OPTION_VAL_VERY_HIGH_RU },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_RU,
      FCEUMM_SWAPDUTY_LABEL_CAT_RU,
      FCEUMM_SWAPDUTY_INFO_0_RU,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_RU,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_RU,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_RU },
         { "1",  OPTION_VAL_1_RU },
         { "2",  OPTION_VAL_2_RU },
         { "3",  OPTION_VAL_3_RU },
         { "4",  OPTION_VAL_4_RU },
         { "5",  OPTION_VAL_5_RU },
         { "6",  OPTION_VAL_6_RU },
         { "7",  OPTION_VAL_7_RU },
         { "8",  OPTION_VAL_8_RU },
         { "9",  OPTION_VAL_9_RU },
         { "10", OPTION_VAL_10_RU },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_RU,
      FCEUMM_APU_1_LABEL_CAT_RU,
      FCEUMM_APU_1_INFO_0_RU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_RU,
      FCEUMM_APU_2_LABEL_CAT_RU,
      FCEUMM_APU_2_INFO_0_RU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_RU,
      FCEUMM_APU_3_LABEL_CAT_RU,
      FCEUMM_APU_3_INFO_0_RU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_RU,
      FCEUMM_APU_4_LABEL_CAT_RU,
      FCEUMM_APU_4_INFO_0_RU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_RU,
      FCEUMM_APU_5_LABEL_CAT_RU,
      FCEUMM_APU_5_INFO_0_RU,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_RU,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_RU,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_RU },
         { "Player 1", OPTION_VAL_PLAYER_1_RU },
         { "Player 2", OPTION_VAL_PLAYER_2_RU },
         { "Both",     OPTION_VAL_BOTH_RU },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_RU,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_RU,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_RU,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_RU,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_RU },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_RU },
         { "mouse",       OPTION_VAL_MOUSE_RU },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_RU,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_RU,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_RU,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_RU,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_RU,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_RU,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_RU,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_RU,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_RU,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_RU,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_RU },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_RU },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_RU,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_RU,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_RU },
         { "fill $00", OPTION_VAL_FILL_00_RU },
         { "random",   OPTION_VAL_RANDOM_RU },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_ru = {
   option_cats_ru,
   option_defs_ru
};

/* RETRO_LANGUAGE_SI */

#define CATEGORY_SYSTEM_LABEL_SI NULL
#define CATEGORY_SYSTEM_INFO_0_SI NULL
#define CATEGORY_VIDEO_LABEL_SI NULL
#define CATEGORY_VIDEO_INFO_0_SI NULL
#define CATEGORY_VIDEO_INFO_1_SI NULL
#define CATEGORY_AUDIO_LABEL_SI NULL
#define CATEGORY_AUDIO_INFO_0_SI NULL
#define CATEGORY_INPUT_LABEL_SI NULL
#define CATEGORY_INPUT_INFO_0_SI NULL
#define CATEGORY_HACKS_LABEL_SI NULL
#define CATEGORY_HACKS_INFO_0_SI NULL
#define CATEGORY_DIP_SWITCH_LABEL_SI NULL
#define CATEGORY_DIP_SWITCH_INFO_0_SI NULL
#define FCEUMM_REGION_LABEL_SI NULL
#define FCEUMM_REGION_INFO_0_SI NULL
#define OPTION_VAL_AUTO_SI NULL
#define OPTION_VAL_NTSC_SI NULL
#define OPTION_VAL_PAL_SI NULL
#define OPTION_VAL_DENDY_SI NULL
#define FCEUMM_GAME_GENIE_LABEL_SI NULL
#define FCEUMM_GAME_GENIE_INFO_0_SI NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SI NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SI NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SI NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SI NULL
#define FCEUMM_ASPECT_LABEL_SI NULL
#define FCEUMM_ASPECT_INFO_0_SI NULL
#define OPTION_VAL_8_7_PAR_SI NULL
#define OPTION_VAL_4_3_SI NULL
#define OPTION_VAL_PP_SI NULL
#define FCEUMM_OVERSCAN_LABEL_SI NULL
#define FCEUMM_OVERSCAN_INFO_0_SI NULL
#define FCEUMM_OVERSCAN_H_LABEL_SI NULL
#define FCEUMM_OVERSCAN_H_INFO_0_SI NULL
#define FCEUMM_OVERSCAN_V_LABEL_SI NULL
#define FCEUMM_OVERSCAN_V_INFO_0_SI NULL
#define FCEUMM_PALETTE_LABEL_SI NULL
#define FCEUMM_PALETTE_INFO_0_SI NULL
#define OPTION_VAL_DEFAULT_SI NULL
#define OPTION_VAL_ASQREALC_SI NULL
#define OPTION_VAL_NINTENDO_VC_SI NULL
#define OPTION_VAL_RGB_SI NULL
#define OPTION_VAL_YUV_V3_SI NULL
#define OPTION_VAL_UNSATURATED_FINAL_SI NULL
#define OPTION_VAL_SONY_CXA2025AS_US_SI NULL
#define OPTION_VAL_BMF_FINAL2_SI NULL
#define OPTION_VAL_BMF_FINAL3_SI NULL
#define OPTION_VAL_SMOOTH_FBX_SI NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_SI NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_SI NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_SI NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_SI NULL
#define OPTION_VAL_NESCAP_SI NULL
#define OPTION_VAL_WAVEBEAM_SI NULL
#define OPTION_VAL_RAW_SI NULL
#define OPTION_VAL_CUSTOM_SI NULL
#define FCEUMM_NTSC_FILTER_LABEL_SI NULL
#define FCEUMM_NTSC_FILTER_INFO_0_SI NULL
#define OPTION_VAL_COMPOSITE_SI NULL
#define OPTION_VAL_SVIDEO_SI NULL
#define OPTION_VAL_RGB_O9_SI NULL
#define OPTION_VAL_MONOCHROME_SI NULL
#define FCEUMM_SNDQUALITY_LABEL_SI NULL
#define FCEUMM_SNDQUALITY_INFO_0_SI NULL
#define OPTION_VAL_LOW_SI NULL
#define OPTION_VAL_HIGH_SI NULL
#define OPTION_VAL_VERY_HIGH_SI NULL
#define FCEUMM_SWAPDUTY_LABEL_SI NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_SI NULL
#define FCEUMM_SWAPDUTY_INFO_0_SI NULL
#define FCEUMM_SNDVOLUME_LABEL_SI NULL
#define FCEUMM_SNDVOLUME_INFO_0_SI NULL
#define OPTION_VAL_0_SI NULL
#define OPTION_VAL_1_SI NULL
#define OPTION_VAL_2_SI NULL
#define OPTION_VAL_3_SI NULL
#define OPTION_VAL_4_SI NULL
#define OPTION_VAL_5_SI NULL
#define OPTION_VAL_6_SI NULL
#define OPTION_VAL_7_SI NULL
#define OPTION_VAL_8_SI NULL
#define OPTION_VAL_9_SI NULL
#define OPTION_VAL_10_SI NULL
#define FCEUMM_APU_1_LABEL_SI NULL
#define FCEUMM_APU_1_LABEL_CAT_SI NULL
#define FCEUMM_APU_1_INFO_0_SI NULL
#define FCEUMM_APU_2_LABEL_SI NULL
#define FCEUMM_APU_2_LABEL_CAT_SI NULL
#define FCEUMM_APU_2_INFO_0_SI NULL
#define FCEUMM_APU_3_LABEL_SI NULL
#define FCEUMM_APU_3_LABEL_CAT_SI NULL
#define FCEUMM_APU_3_INFO_0_SI NULL
#define FCEUMM_APU_4_LABEL_SI NULL
#define FCEUMM_APU_4_LABEL_CAT_SI NULL
#define FCEUMM_APU_4_INFO_0_SI NULL
#define FCEUMM_APU_5_LABEL_SI NULL
#define FCEUMM_APU_5_LABEL_CAT_SI NULL
#define FCEUMM_APU_5_INFO_0_SI NULL
#define FCEUMM_TURBO_ENABLE_LABEL_SI NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_SI NULL
#define OPTION_VAL_NONE_SI NULL
#define OPTION_VAL_PLAYER_1_SI NULL
#define OPTION_VAL_PLAYER_2_SI NULL
#define OPTION_VAL_BOTH_SI NULL
#define FCEUMM_TURBO_DELAY_LABEL_SI NULL
#define FCEUMM_TURBO_DELAY_INFO_0_SI NULL
#define FCEUMM_ZAPPER_MODE_LABEL_SI NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_SI NULL
#define OPTION_VAL_LIGHTGUN_SI NULL
#define OPTION_VAL_TOUCHSCREEN_SI NULL
#define OPTION_VAL_MOUSE_SI NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_SI NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_SI NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_SI NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_SI NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_SI NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_SI NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_SI NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_SI NULL
#define FCEUMM_OVERCLOCKING_LABEL_SI NULL
#define FCEUMM_OVERCLOCKING_INFO_0_SI NULL
#define OPTION_VAL_2X_POSTRENDER_SI NULL
#define OPTION_VAL_2X_VBLANK_SI NULL
#define FCEUMM_RAMSTATE_LABEL_SI NULL
#define FCEUMM_RAMSTATE_INFO_0_SI NULL
#define OPTION_VAL_FILL_FF_SI NULL
#define OPTION_VAL_FILL_00_SI NULL
#define OPTION_VAL_RANDOM_SI NULL

struct retro_core_option_v2_category option_cats_si[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_SI,
      CATEGORY_SYSTEM_INFO_0_SI
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_SI,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_SI
#else
      CATEGORY_VIDEO_INFO_1_SI
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_SI,
      CATEGORY_AUDIO_INFO_0_SI
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_SI,
      CATEGORY_INPUT_INFO_0_SI
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_SI,
      CATEGORY_HACKS_INFO_0_SI
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_SI,
      CATEGORY_DIP_SWITCH_INFO_0_SI
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_si[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_SI,
      NULL,
      FCEUMM_REGION_INFO_0_SI,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_SI },
         { "NTSC",  OPTION_VAL_NTSC_SI },
         { "PAL",   OPTION_VAL_PAL_SI },
         { "Dendy", OPTION_VAL_DENDY_SI },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_SI,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_SI,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SI,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SI,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SI,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SI,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_SI,
      NULL,
      FCEUMM_ASPECT_INFO_0_SI,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_SI },
         { "4:3",     OPTION_VAL_4_3_SI },
         { "PP",      OPTION_VAL_PP_SI },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_SI,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_SI,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_SI,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_SI,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_SI,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_SI,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_SI,
      NULL,
      FCEUMM_PALETTE_INFO_0_SI,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_SI },
         { "asqrealc",             OPTION_VAL_ASQREALC_SI },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_SI },
         { "rgb",                  OPTION_VAL_RGB_SI },
         { "yuv-v3",               OPTION_VAL_YUV_V3_SI },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_SI },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_SI },
         { "pal",                  OPTION_VAL_PAL_SI },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_SI },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_SI },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_SI },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_SI },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_SI },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_SI },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_SI },
         { "nescap",               OPTION_VAL_NESCAP_SI },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_SI },
         { "raw",                  OPTION_VAL_RAW_SI },
         { "custom",               OPTION_VAL_CUSTOM_SI },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_SI,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_SI,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_SI },
         { "svideo",     OPTION_VAL_SVIDEO_SI },
         { "rgb",        OPTION_VAL_RGB_O9_SI },
         { "monochrome", OPTION_VAL_MONOCHROME_SI },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_SI,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_SI,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_SI },
         { "High",      OPTION_VAL_HIGH_SI },
         { "Very High", OPTION_VAL_VERY_HIGH_SI },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_SI,
      FCEUMM_SWAPDUTY_LABEL_CAT_SI,
      FCEUMM_SWAPDUTY_INFO_0_SI,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_SI,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_SI,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_SI },
         { "1",  OPTION_VAL_1_SI },
         { "2",  OPTION_VAL_2_SI },
         { "3",  OPTION_VAL_3_SI },
         { "4",  OPTION_VAL_4_SI },
         { "5",  OPTION_VAL_5_SI },
         { "6",  OPTION_VAL_6_SI },
         { "7",  OPTION_VAL_7_SI },
         { "8",  OPTION_VAL_8_SI },
         { "9",  OPTION_VAL_9_SI },
         { "10", OPTION_VAL_10_SI },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_SI,
      FCEUMM_APU_1_LABEL_CAT_SI,
      FCEUMM_APU_1_INFO_0_SI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_SI,
      FCEUMM_APU_2_LABEL_CAT_SI,
      FCEUMM_APU_2_INFO_0_SI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_SI,
      FCEUMM_APU_3_LABEL_CAT_SI,
      FCEUMM_APU_3_INFO_0_SI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_SI,
      FCEUMM_APU_4_LABEL_CAT_SI,
      FCEUMM_APU_4_INFO_0_SI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_SI,
      FCEUMM_APU_5_LABEL_CAT_SI,
      FCEUMM_APU_5_INFO_0_SI,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_SI,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_SI,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_SI },
         { "Player 1", OPTION_VAL_PLAYER_1_SI },
         { "Player 2", OPTION_VAL_PLAYER_2_SI },
         { "Both",     OPTION_VAL_BOTH_SI },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_SI,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_SI,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_SI,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_SI,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_SI },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_SI },
         { "mouse",       OPTION_VAL_MOUSE_SI },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_SI,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_SI,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_SI,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_SI,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_SI,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_SI,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_SI,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_SI,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_SI,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_SI,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_SI },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_SI },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_SI,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_SI,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_SI },
         { "fill $00", OPTION_VAL_FILL_00_SI },
         { "random",   OPTION_VAL_RANDOM_SI },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_si = {
   option_cats_si,
   option_defs_si
};

/* RETRO_LANGUAGE_SK */

#define CATEGORY_SYSTEM_LABEL_SK NULL
#define CATEGORY_SYSTEM_INFO_0_SK NULL
#define CATEGORY_VIDEO_LABEL_SK NULL
#define CATEGORY_VIDEO_INFO_0_SK NULL
#define CATEGORY_VIDEO_INFO_1_SK NULL
#define CATEGORY_AUDIO_LABEL_SK "Zvuk"
#define CATEGORY_AUDIO_INFO_0_SK NULL
#define CATEGORY_INPUT_LABEL_SK "Vstup"
#define CATEGORY_INPUT_INFO_0_SK NULL
#define CATEGORY_HACKS_LABEL_SK NULL
#define CATEGORY_HACKS_INFO_0_SK NULL
#define CATEGORY_DIP_SWITCH_LABEL_SK NULL
#define CATEGORY_DIP_SWITCH_INFO_0_SK NULL
#define FCEUMM_REGION_LABEL_SK NULL
#define FCEUMM_REGION_INFO_0_SK NULL
#define OPTION_VAL_AUTO_SK NULL
#define OPTION_VAL_NTSC_SK NULL
#define OPTION_VAL_PAL_SK NULL
#define OPTION_VAL_DENDY_SK NULL
#define FCEUMM_GAME_GENIE_LABEL_SK NULL
#define FCEUMM_GAME_GENIE_INFO_0_SK NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SK NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SK NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SK NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SK NULL
#define FCEUMM_ASPECT_LABEL_SK NULL
#define FCEUMM_ASPECT_INFO_0_SK NULL
#define OPTION_VAL_8_7_PAR_SK NULL
#define OPTION_VAL_4_3_SK NULL
#define OPTION_VAL_PP_SK NULL
#define FCEUMM_OVERSCAN_LABEL_SK NULL
#define FCEUMM_OVERSCAN_INFO_0_SK NULL
#define FCEUMM_OVERSCAN_H_LABEL_SK NULL
#define FCEUMM_OVERSCAN_H_INFO_0_SK NULL
#define FCEUMM_OVERSCAN_V_LABEL_SK NULL
#define FCEUMM_OVERSCAN_V_INFO_0_SK NULL
#define FCEUMM_PALETTE_LABEL_SK NULL
#define FCEUMM_PALETTE_INFO_0_SK NULL
#define OPTION_VAL_DEFAULT_SK NULL
#define OPTION_VAL_ASQREALC_SK NULL
#define OPTION_VAL_NINTENDO_VC_SK NULL
#define OPTION_VAL_RGB_SK NULL
#define OPTION_VAL_YUV_V3_SK NULL
#define OPTION_VAL_UNSATURATED_FINAL_SK NULL
#define OPTION_VAL_SONY_CXA2025AS_US_SK NULL
#define OPTION_VAL_BMF_FINAL2_SK NULL
#define OPTION_VAL_BMF_FINAL3_SK NULL
#define OPTION_VAL_SMOOTH_FBX_SK NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_SK NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_SK NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_SK NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_SK NULL
#define OPTION_VAL_NESCAP_SK NULL
#define OPTION_VAL_WAVEBEAM_SK NULL
#define OPTION_VAL_RAW_SK NULL
#define OPTION_VAL_CUSTOM_SK NULL
#define FCEUMM_NTSC_FILTER_LABEL_SK NULL
#define FCEUMM_NTSC_FILTER_INFO_0_SK NULL
#define OPTION_VAL_COMPOSITE_SK NULL
#define OPTION_VAL_SVIDEO_SK NULL
#define OPTION_VAL_RGB_O9_SK NULL
#define OPTION_VAL_MONOCHROME_SK NULL
#define FCEUMM_SNDQUALITY_LABEL_SK NULL
#define FCEUMM_SNDQUALITY_INFO_0_SK NULL
#define OPTION_VAL_LOW_SK NULL
#define OPTION_VAL_HIGH_SK NULL
#define OPTION_VAL_VERY_HIGH_SK NULL
#define FCEUMM_SWAPDUTY_LABEL_SK NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_SK NULL
#define FCEUMM_SWAPDUTY_INFO_0_SK NULL
#define FCEUMM_SNDVOLUME_LABEL_SK NULL
#define FCEUMM_SNDVOLUME_INFO_0_SK NULL
#define OPTION_VAL_0_SK NULL
#define OPTION_VAL_1_SK NULL
#define OPTION_VAL_2_SK NULL
#define OPTION_VAL_3_SK NULL
#define OPTION_VAL_4_SK NULL
#define OPTION_VAL_5_SK NULL
#define OPTION_VAL_6_SK NULL
#define OPTION_VAL_7_SK NULL
#define OPTION_VAL_8_SK NULL
#define OPTION_VAL_9_SK NULL
#define OPTION_VAL_10_SK NULL
#define FCEUMM_APU_1_LABEL_SK NULL
#define FCEUMM_APU_1_LABEL_CAT_SK NULL
#define FCEUMM_APU_1_INFO_0_SK NULL
#define FCEUMM_APU_2_LABEL_SK NULL
#define FCEUMM_APU_2_LABEL_CAT_SK NULL
#define FCEUMM_APU_2_INFO_0_SK NULL
#define FCEUMM_APU_3_LABEL_SK NULL
#define FCEUMM_APU_3_LABEL_CAT_SK NULL
#define FCEUMM_APU_3_INFO_0_SK NULL
#define FCEUMM_APU_4_LABEL_SK NULL
#define FCEUMM_APU_4_LABEL_CAT_SK NULL
#define FCEUMM_APU_4_INFO_0_SK NULL
#define FCEUMM_APU_5_LABEL_SK NULL
#define FCEUMM_APU_5_LABEL_CAT_SK NULL
#define FCEUMM_APU_5_INFO_0_SK NULL
#define FCEUMM_TURBO_ENABLE_LABEL_SK NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_SK NULL
#define OPTION_VAL_NONE_SK NULL
#define OPTION_VAL_PLAYER_1_SK NULL
#define OPTION_VAL_PLAYER_2_SK NULL
#define OPTION_VAL_BOTH_SK NULL
#define FCEUMM_TURBO_DELAY_LABEL_SK NULL
#define FCEUMM_TURBO_DELAY_INFO_0_SK NULL
#define FCEUMM_ZAPPER_MODE_LABEL_SK NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_SK NULL
#define OPTION_VAL_LIGHTGUN_SK NULL
#define OPTION_VAL_TOUCHSCREEN_SK NULL
#define OPTION_VAL_MOUSE_SK NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_SK NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_SK NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_SK NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_SK NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_SK NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_SK NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_SK NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_SK NULL
#define FCEUMM_OVERCLOCKING_LABEL_SK NULL
#define FCEUMM_OVERCLOCKING_INFO_0_SK NULL
#define OPTION_VAL_2X_POSTRENDER_SK NULL
#define OPTION_VAL_2X_VBLANK_SK NULL
#define FCEUMM_RAMSTATE_LABEL_SK NULL
#define FCEUMM_RAMSTATE_INFO_0_SK NULL
#define OPTION_VAL_FILL_FF_SK NULL
#define OPTION_VAL_FILL_00_SK NULL
#define OPTION_VAL_RANDOM_SK NULL

struct retro_core_option_v2_category option_cats_sk[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_SK,
      CATEGORY_SYSTEM_INFO_0_SK
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_SK,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_SK
#else
      CATEGORY_VIDEO_INFO_1_SK
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_SK,
      CATEGORY_AUDIO_INFO_0_SK
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_SK,
      CATEGORY_INPUT_INFO_0_SK
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_SK,
      CATEGORY_HACKS_INFO_0_SK
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_SK,
      CATEGORY_DIP_SWITCH_INFO_0_SK
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_sk[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_SK,
      NULL,
      FCEUMM_REGION_INFO_0_SK,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_SK },
         { "NTSC",  OPTION_VAL_NTSC_SK },
         { "PAL",   OPTION_VAL_PAL_SK },
         { "Dendy", OPTION_VAL_DENDY_SK },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_SK,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_SK,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SK,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SK,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SK,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SK,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_SK,
      NULL,
      FCEUMM_ASPECT_INFO_0_SK,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_SK },
         { "4:3",     OPTION_VAL_4_3_SK },
         { "PP",      OPTION_VAL_PP_SK },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_SK,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_SK,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_SK,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_SK,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_SK,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_SK,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_SK,
      NULL,
      FCEUMM_PALETTE_INFO_0_SK,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_SK },
         { "asqrealc",             OPTION_VAL_ASQREALC_SK },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_SK },
         { "rgb",                  OPTION_VAL_RGB_SK },
         { "yuv-v3",               OPTION_VAL_YUV_V3_SK },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_SK },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_SK },
         { "pal",                  OPTION_VAL_PAL_SK },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_SK },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_SK },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_SK },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_SK },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_SK },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_SK },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_SK },
         { "nescap",               OPTION_VAL_NESCAP_SK },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_SK },
         { "raw",                  OPTION_VAL_RAW_SK },
         { "custom",               OPTION_VAL_CUSTOM_SK },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_SK,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_SK,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_SK },
         { "svideo",     OPTION_VAL_SVIDEO_SK },
         { "rgb",        OPTION_VAL_RGB_O9_SK },
         { "monochrome", OPTION_VAL_MONOCHROME_SK },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_SK,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_SK,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_SK },
         { "High",      OPTION_VAL_HIGH_SK },
         { "Very High", OPTION_VAL_VERY_HIGH_SK },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_SK,
      FCEUMM_SWAPDUTY_LABEL_CAT_SK,
      FCEUMM_SWAPDUTY_INFO_0_SK,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_SK,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_SK,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_SK },
         { "1",  OPTION_VAL_1_SK },
         { "2",  OPTION_VAL_2_SK },
         { "3",  OPTION_VAL_3_SK },
         { "4",  OPTION_VAL_4_SK },
         { "5",  OPTION_VAL_5_SK },
         { "6",  OPTION_VAL_6_SK },
         { "7",  OPTION_VAL_7_SK },
         { "8",  OPTION_VAL_8_SK },
         { "9",  OPTION_VAL_9_SK },
         { "10", OPTION_VAL_10_SK },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_SK,
      FCEUMM_APU_1_LABEL_CAT_SK,
      FCEUMM_APU_1_INFO_0_SK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_SK,
      FCEUMM_APU_2_LABEL_CAT_SK,
      FCEUMM_APU_2_INFO_0_SK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_SK,
      FCEUMM_APU_3_LABEL_CAT_SK,
      FCEUMM_APU_3_INFO_0_SK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_SK,
      FCEUMM_APU_4_LABEL_CAT_SK,
      FCEUMM_APU_4_INFO_0_SK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_SK,
      FCEUMM_APU_5_LABEL_CAT_SK,
      FCEUMM_APU_5_INFO_0_SK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_SK,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_SK,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_SK },
         { "Player 1", OPTION_VAL_PLAYER_1_SK },
         { "Player 2", OPTION_VAL_PLAYER_2_SK },
         { "Both",     OPTION_VAL_BOTH_SK },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_SK,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_SK,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_SK,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_SK,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_SK },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_SK },
         { "mouse",       OPTION_VAL_MOUSE_SK },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_SK,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_SK,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_SK,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_SK,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_SK,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_SK,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_SK,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_SK,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_SK,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_SK,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_SK },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_SK },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_SK,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_SK,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_SK },
         { "fill $00", OPTION_VAL_FILL_00_SK },
         { "random",   OPTION_VAL_RANDOM_SK },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_sk = {
   option_cats_sk,
   option_defs_sk
};

/* RETRO_LANGUAGE_SR */

#define CATEGORY_SYSTEM_LABEL_SR NULL
#define CATEGORY_SYSTEM_INFO_0_SR NULL
#define CATEGORY_VIDEO_LABEL_SR NULL
#define CATEGORY_VIDEO_INFO_0_SR NULL
#define CATEGORY_VIDEO_INFO_1_SR NULL
#define CATEGORY_AUDIO_LABEL_SR "Zvuk"
#define CATEGORY_AUDIO_INFO_0_SR NULL
#define CATEGORY_INPUT_LABEL_SR "Ulaz"
#define CATEGORY_INPUT_INFO_0_SR NULL
#define CATEGORY_HACKS_LABEL_SR NULL
#define CATEGORY_HACKS_INFO_0_SR NULL
#define CATEGORY_DIP_SWITCH_LABEL_SR NULL
#define CATEGORY_DIP_SWITCH_INFO_0_SR NULL
#define FCEUMM_REGION_LABEL_SR NULL
#define FCEUMM_REGION_INFO_0_SR NULL
#define OPTION_VAL_AUTO_SR NULL
#define OPTION_VAL_NTSC_SR NULL
#define OPTION_VAL_PAL_SR NULL
#define OPTION_VAL_DENDY_SR NULL
#define FCEUMM_GAME_GENIE_LABEL_SR NULL
#define FCEUMM_GAME_GENIE_INFO_0_SR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SR NULL
#define FCEUMM_ASPECT_LABEL_SR NULL
#define FCEUMM_ASPECT_INFO_0_SR NULL
#define OPTION_VAL_8_7_PAR_SR NULL
#define OPTION_VAL_4_3_SR NULL
#define OPTION_VAL_PP_SR NULL
#define FCEUMM_OVERSCAN_LABEL_SR NULL
#define FCEUMM_OVERSCAN_INFO_0_SR NULL
#define FCEUMM_OVERSCAN_H_LABEL_SR NULL
#define FCEUMM_OVERSCAN_H_INFO_0_SR NULL
#define FCEUMM_OVERSCAN_V_LABEL_SR NULL
#define FCEUMM_OVERSCAN_V_INFO_0_SR NULL
#define FCEUMM_PALETTE_LABEL_SR NULL
#define FCEUMM_PALETTE_INFO_0_SR NULL
#define OPTION_VAL_DEFAULT_SR NULL
#define OPTION_VAL_ASQREALC_SR NULL
#define OPTION_VAL_NINTENDO_VC_SR NULL
#define OPTION_VAL_RGB_SR NULL
#define OPTION_VAL_YUV_V3_SR NULL
#define OPTION_VAL_UNSATURATED_FINAL_SR NULL
#define OPTION_VAL_SONY_CXA2025AS_US_SR NULL
#define OPTION_VAL_BMF_FINAL2_SR NULL
#define OPTION_VAL_BMF_FINAL3_SR NULL
#define OPTION_VAL_SMOOTH_FBX_SR NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_SR NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_SR NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_SR NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_SR NULL
#define OPTION_VAL_NESCAP_SR NULL
#define OPTION_VAL_WAVEBEAM_SR NULL
#define OPTION_VAL_RAW_SR NULL
#define OPTION_VAL_CUSTOM_SR NULL
#define FCEUMM_NTSC_FILTER_LABEL_SR NULL
#define FCEUMM_NTSC_FILTER_INFO_0_SR NULL
#define OPTION_VAL_COMPOSITE_SR NULL
#define OPTION_VAL_SVIDEO_SR NULL
#define OPTION_VAL_RGB_O9_SR NULL
#define OPTION_VAL_MONOCHROME_SR NULL
#define FCEUMM_SNDQUALITY_LABEL_SR NULL
#define FCEUMM_SNDQUALITY_INFO_0_SR NULL
#define OPTION_VAL_LOW_SR NULL
#define OPTION_VAL_HIGH_SR NULL
#define OPTION_VAL_VERY_HIGH_SR NULL
#define FCEUMM_SWAPDUTY_LABEL_SR NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_SR NULL
#define FCEUMM_SWAPDUTY_INFO_0_SR NULL
#define FCEUMM_SNDVOLUME_LABEL_SR NULL
#define FCEUMM_SNDVOLUME_INFO_0_SR NULL
#define OPTION_VAL_0_SR NULL
#define OPTION_VAL_1_SR NULL
#define OPTION_VAL_2_SR NULL
#define OPTION_VAL_3_SR NULL
#define OPTION_VAL_4_SR NULL
#define OPTION_VAL_5_SR NULL
#define OPTION_VAL_6_SR NULL
#define OPTION_VAL_7_SR NULL
#define OPTION_VAL_8_SR NULL
#define OPTION_VAL_9_SR NULL
#define OPTION_VAL_10_SR NULL
#define FCEUMM_APU_1_LABEL_SR NULL
#define FCEUMM_APU_1_LABEL_CAT_SR NULL
#define FCEUMM_APU_1_INFO_0_SR NULL
#define FCEUMM_APU_2_LABEL_SR NULL
#define FCEUMM_APU_2_LABEL_CAT_SR NULL
#define FCEUMM_APU_2_INFO_0_SR NULL
#define FCEUMM_APU_3_LABEL_SR NULL
#define FCEUMM_APU_3_LABEL_CAT_SR NULL
#define FCEUMM_APU_3_INFO_0_SR NULL
#define FCEUMM_APU_4_LABEL_SR NULL
#define FCEUMM_APU_4_LABEL_CAT_SR NULL
#define FCEUMM_APU_4_INFO_0_SR NULL
#define FCEUMM_APU_5_LABEL_SR NULL
#define FCEUMM_APU_5_LABEL_CAT_SR NULL
#define FCEUMM_APU_5_INFO_0_SR NULL
#define FCEUMM_TURBO_ENABLE_LABEL_SR NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_SR NULL
#define OPTION_VAL_NONE_SR NULL
#define OPTION_VAL_PLAYER_1_SR NULL
#define OPTION_VAL_PLAYER_2_SR NULL
#define OPTION_VAL_BOTH_SR NULL
#define FCEUMM_TURBO_DELAY_LABEL_SR NULL
#define FCEUMM_TURBO_DELAY_INFO_0_SR NULL
#define FCEUMM_ZAPPER_MODE_LABEL_SR NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_SR NULL
#define OPTION_VAL_LIGHTGUN_SR NULL
#define OPTION_VAL_TOUCHSCREEN_SR NULL
#define OPTION_VAL_MOUSE_SR NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_SR NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_SR NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_SR NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_SR NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_SR NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_SR NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_SR NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_SR NULL
#define FCEUMM_OVERCLOCKING_LABEL_SR NULL
#define FCEUMM_OVERCLOCKING_INFO_0_SR NULL
#define OPTION_VAL_2X_POSTRENDER_SR NULL
#define OPTION_VAL_2X_VBLANK_SR NULL
#define FCEUMM_RAMSTATE_LABEL_SR NULL
#define FCEUMM_RAMSTATE_INFO_0_SR NULL
#define OPTION_VAL_FILL_FF_SR NULL
#define OPTION_VAL_FILL_00_SR NULL
#define OPTION_VAL_RANDOM_SR NULL

struct retro_core_option_v2_category option_cats_sr[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_SR,
      CATEGORY_SYSTEM_INFO_0_SR
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_SR,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_SR
#else
      CATEGORY_VIDEO_INFO_1_SR
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_SR,
      CATEGORY_AUDIO_INFO_0_SR
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_SR,
      CATEGORY_INPUT_INFO_0_SR
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_SR,
      CATEGORY_HACKS_INFO_0_SR
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_SR,
      CATEGORY_DIP_SWITCH_INFO_0_SR
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_sr[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_SR,
      NULL,
      FCEUMM_REGION_INFO_0_SR,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_SR },
         { "NTSC",  OPTION_VAL_NTSC_SR },
         { "PAL",   OPTION_VAL_PAL_SR },
         { "Dendy", OPTION_VAL_DENDY_SR },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_SR,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_SR,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SR,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SR,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_SR,
      NULL,
      FCEUMM_ASPECT_INFO_0_SR,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_SR },
         { "4:3",     OPTION_VAL_4_3_SR },
         { "PP",      OPTION_VAL_PP_SR },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_SR,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_SR,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_SR,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_SR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_SR,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_SR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_SR,
      NULL,
      FCEUMM_PALETTE_INFO_0_SR,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_SR },
         { "asqrealc",             OPTION_VAL_ASQREALC_SR },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_SR },
         { "rgb",                  OPTION_VAL_RGB_SR },
         { "yuv-v3",               OPTION_VAL_YUV_V3_SR },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_SR },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_SR },
         { "pal",                  OPTION_VAL_PAL_SR },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_SR },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_SR },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_SR },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_SR },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_SR },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_SR },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_SR },
         { "nescap",               OPTION_VAL_NESCAP_SR },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_SR },
         { "raw",                  OPTION_VAL_RAW_SR },
         { "custom",               OPTION_VAL_CUSTOM_SR },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_SR,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_SR,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_SR },
         { "svideo",     OPTION_VAL_SVIDEO_SR },
         { "rgb",        OPTION_VAL_RGB_O9_SR },
         { "monochrome", OPTION_VAL_MONOCHROME_SR },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_SR,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_SR,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_SR },
         { "High",      OPTION_VAL_HIGH_SR },
         { "Very High", OPTION_VAL_VERY_HIGH_SR },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_SR,
      FCEUMM_SWAPDUTY_LABEL_CAT_SR,
      FCEUMM_SWAPDUTY_INFO_0_SR,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_SR,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_SR,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_SR },
         { "1",  OPTION_VAL_1_SR },
         { "2",  OPTION_VAL_2_SR },
         { "3",  OPTION_VAL_3_SR },
         { "4",  OPTION_VAL_4_SR },
         { "5",  OPTION_VAL_5_SR },
         { "6",  OPTION_VAL_6_SR },
         { "7",  OPTION_VAL_7_SR },
         { "8",  OPTION_VAL_8_SR },
         { "9",  OPTION_VAL_9_SR },
         { "10", OPTION_VAL_10_SR },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_SR,
      FCEUMM_APU_1_LABEL_CAT_SR,
      FCEUMM_APU_1_INFO_0_SR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_SR,
      FCEUMM_APU_2_LABEL_CAT_SR,
      FCEUMM_APU_2_INFO_0_SR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_SR,
      FCEUMM_APU_3_LABEL_CAT_SR,
      FCEUMM_APU_3_INFO_0_SR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_SR,
      FCEUMM_APU_4_LABEL_CAT_SR,
      FCEUMM_APU_4_INFO_0_SR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_SR,
      FCEUMM_APU_5_LABEL_CAT_SR,
      FCEUMM_APU_5_INFO_0_SR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_SR,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_SR,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_SR },
         { "Player 1", OPTION_VAL_PLAYER_1_SR },
         { "Player 2", OPTION_VAL_PLAYER_2_SR },
         { "Both",     OPTION_VAL_BOTH_SR },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_SR,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_SR,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_SR,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_SR,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_SR },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_SR },
         { "mouse",       OPTION_VAL_MOUSE_SR },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_SR,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_SR,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_SR,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_SR,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_SR,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_SR,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_SR,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_SR,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_SR,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_SR,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_SR },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_SR },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_SR,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_SR,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_SR },
         { "fill $00", OPTION_VAL_FILL_00_SR },
         { "random",   OPTION_VAL_RANDOM_SR },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_sr = {
   option_cats_sr,
   option_defs_sr
};

/* RETRO_LANGUAGE_SV */

#define CATEGORY_SYSTEM_LABEL_SV NULL
#define CATEGORY_SYSTEM_INFO_0_SV NULL
#define CATEGORY_VIDEO_LABEL_SV "Bild"
#define CATEGORY_VIDEO_INFO_0_SV NULL
#define CATEGORY_VIDEO_INFO_1_SV NULL
#define CATEGORY_AUDIO_LABEL_SV "Ljud"
#define CATEGORY_AUDIO_INFO_0_SV NULL
#define CATEGORY_INPUT_LABEL_SV "Inmatning"
#define CATEGORY_INPUT_INFO_0_SV NULL
#define CATEGORY_HACKS_LABEL_SV NULL
#define CATEGORY_HACKS_INFO_0_SV NULL
#define CATEGORY_DIP_SWITCH_LABEL_SV NULL
#define CATEGORY_DIP_SWITCH_INFO_0_SV NULL
#define FCEUMM_REGION_LABEL_SV NULL
#define FCEUMM_REGION_INFO_0_SV NULL
#define OPTION_VAL_AUTO_SV NULL
#define OPTION_VAL_NTSC_SV NULL
#define OPTION_VAL_PAL_SV NULL
#define OPTION_VAL_DENDY_SV NULL
#define FCEUMM_GAME_GENIE_LABEL_SV NULL
#define FCEUMM_GAME_GENIE_INFO_0_SV NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SV NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SV NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SV NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SV NULL
#define FCEUMM_ASPECT_LABEL_SV NULL
#define FCEUMM_ASPECT_INFO_0_SV NULL
#define OPTION_VAL_8_7_PAR_SV NULL
#define OPTION_VAL_4_3_SV NULL
#define OPTION_VAL_PP_SV NULL
#define FCEUMM_OVERSCAN_LABEL_SV NULL
#define FCEUMM_OVERSCAN_INFO_0_SV NULL
#define FCEUMM_OVERSCAN_H_LABEL_SV NULL
#define FCEUMM_OVERSCAN_H_INFO_0_SV NULL
#define FCEUMM_OVERSCAN_V_LABEL_SV NULL
#define FCEUMM_OVERSCAN_V_INFO_0_SV NULL
#define FCEUMM_PALETTE_LABEL_SV NULL
#define FCEUMM_PALETTE_INFO_0_SV NULL
#define OPTION_VAL_DEFAULT_SV NULL
#define OPTION_VAL_ASQREALC_SV NULL
#define OPTION_VAL_NINTENDO_VC_SV NULL
#define OPTION_VAL_RGB_SV NULL
#define OPTION_VAL_YUV_V3_SV NULL
#define OPTION_VAL_UNSATURATED_FINAL_SV NULL
#define OPTION_VAL_SONY_CXA2025AS_US_SV NULL
#define OPTION_VAL_BMF_FINAL2_SV NULL
#define OPTION_VAL_BMF_FINAL3_SV NULL
#define OPTION_VAL_SMOOTH_FBX_SV NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_SV NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_SV NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_SV NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_SV NULL
#define OPTION_VAL_NESCAP_SV NULL
#define OPTION_VAL_WAVEBEAM_SV NULL
#define OPTION_VAL_RAW_SV NULL
#define OPTION_VAL_CUSTOM_SV NULL
#define FCEUMM_NTSC_FILTER_LABEL_SV NULL
#define FCEUMM_NTSC_FILTER_INFO_0_SV NULL
#define OPTION_VAL_COMPOSITE_SV NULL
#define OPTION_VAL_SVIDEO_SV NULL
#define OPTION_VAL_RGB_O9_SV NULL
#define OPTION_VAL_MONOCHROME_SV NULL
#define FCEUMM_SNDQUALITY_LABEL_SV NULL
#define FCEUMM_SNDQUALITY_INFO_0_SV NULL
#define OPTION_VAL_LOW_SV NULL
#define OPTION_VAL_HIGH_SV NULL
#define OPTION_VAL_VERY_HIGH_SV NULL
#define FCEUMM_SWAPDUTY_LABEL_SV NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_SV NULL
#define FCEUMM_SWAPDUTY_INFO_0_SV NULL
#define FCEUMM_SNDVOLUME_LABEL_SV NULL
#define FCEUMM_SNDVOLUME_INFO_0_SV NULL
#define OPTION_VAL_0_SV NULL
#define OPTION_VAL_1_SV NULL
#define OPTION_VAL_2_SV NULL
#define OPTION_VAL_3_SV NULL
#define OPTION_VAL_4_SV NULL
#define OPTION_VAL_5_SV NULL
#define OPTION_VAL_6_SV NULL
#define OPTION_VAL_7_SV NULL
#define OPTION_VAL_8_SV NULL
#define OPTION_VAL_9_SV NULL
#define OPTION_VAL_10_SV NULL
#define FCEUMM_APU_1_LABEL_SV NULL
#define FCEUMM_APU_1_LABEL_CAT_SV NULL
#define FCEUMM_APU_1_INFO_0_SV NULL
#define FCEUMM_APU_2_LABEL_SV NULL
#define FCEUMM_APU_2_LABEL_CAT_SV NULL
#define FCEUMM_APU_2_INFO_0_SV NULL
#define FCEUMM_APU_3_LABEL_SV NULL
#define FCEUMM_APU_3_LABEL_CAT_SV NULL
#define FCEUMM_APU_3_INFO_0_SV NULL
#define FCEUMM_APU_4_LABEL_SV NULL
#define FCEUMM_APU_4_LABEL_CAT_SV NULL
#define FCEUMM_APU_4_INFO_0_SV NULL
#define FCEUMM_APU_5_LABEL_SV NULL
#define FCEUMM_APU_5_LABEL_CAT_SV NULL
#define FCEUMM_APU_5_INFO_0_SV NULL
#define FCEUMM_TURBO_ENABLE_LABEL_SV NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_SV NULL
#define OPTION_VAL_NONE_SV NULL
#define OPTION_VAL_PLAYER_1_SV NULL
#define OPTION_VAL_PLAYER_2_SV NULL
#define OPTION_VAL_BOTH_SV NULL
#define FCEUMM_TURBO_DELAY_LABEL_SV NULL
#define FCEUMM_TURBO_DELAY_INFO_0_SV NULL
#define FCEUMM_ZAPPER_MODE_LABEL_SV NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_SV NULL
#define OPTION_VAL_LIGHTGUN_SV NULL
#define OPTION_VAL_TOUCHSCREEN_SV NULL
#define OPTION_VAL_MOUSE_SV NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_SV NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_SV NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_SV NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_SV NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_SV NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_SV NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_SV NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_SV NULL
#define FCEUMM_OVERCLOCKING_LABEL_SV NULL
#define FCEUMM_OVERCLOCKING_INFO_0_SV NULL
#define OPTION_VAL_2X_POSTRENDER_SV NULL
#define OPTION_VAL_2X_VBLANK_SV NULL
#define FCEUMM_RAMSTATE_LABEL_SV NULL
#define FCEUMM_RAMSTATE_INFO_0_SV NULL
#define OPTION_VAL_FILL_FF_SV NULL
#define OPTION_VAL_FILL_00_SV NULL
#define OPTION_VAL_RANDOM_SV NULL

struct retro_core_option_v2_category option_cats_sv[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_SV,
      CATEGORY_SYSTEM_INFO_0_SV
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_SV,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_SV
#else
      CATEGORY_VIDEO_INFO_1_SV
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_SV,
      CATEGORY_AUDIO_INFO_0_SV
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_SV,
      CATEGORY_INPUT_INFO_0_SV
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_SV,
      CATEGORY_HACKS_INFO_0_SV
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_SV,
      CATEGORY_DIP_SWITCH_INFO_0_SV
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_sv[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_SV,
      NULL,
      FCEUMM_REGION_INFO_0_SV,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_SV },
         { "NTSC",  OPTION_VAL_NTSC_SV },
         { "PAL",   OPTION_VAL_PAL_SV },
         { "Dendy", OPTION_VAL_DENDY_SV },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_SV,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_SV,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_SV,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_SV,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_SV,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_SV,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_SV,
      NULL,
      FCEUMM_ASPECT_INFO_0_SV,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_SV },
         { "4:3",     OPTION_VAL_4_3_SV },
         { "PP",      OPTION_VAL_PP_SV },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_SV,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_SV,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_SV,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_SV,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_SV,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_SV,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_SV,
      NULL,
      FCEUMM_PALETTE_INFO_0_SV,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_SV },
         { "asqrealc",             OPTION_VAL_ASQREALC_SV },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_SV },
         { "rgb",                  OPTION_VAL_RGB_SV },
         { "yuv-v3",               OPTION_VAL_YUV_V3_SV },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_SV },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_SV },
         { "pal",                  OPTION_VAL_PAL_SV },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_SV },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_SV },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_SV },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_SV },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_SV },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_SV },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_SV },
         { "nescap",               OPTION_VAL_NESCAP_SV },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_SV },
         { "raw",                  OPTION_VAL_RAW_SV },
         { "custom",               OPTION_VAL_CUSTOM_SV },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_SV,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_SV,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_SV },
         { "svideo",     OPTION_VAL_SVIDEO_SV },
         { "rgb",        OPTION_VAL_RGB_O9_SV },
         { "monochrome", OPTION_VAL_MONOCHROME_SV },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_SV,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_SV,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_SV },
         { "High",      OPTION_VAL_HIGH_SV },
         { "Very High", OPTION_VAL_VERY_HIGH_SV },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_SV,
      FCEUMM_SWAPDUTY_LABEL_CAT_SV,
      FCEUMM_SWAPDUTY_INFO_0_SV,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_SV,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_SV,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_SV },
         { "1",  OPTION_VAL_1_SV },
         { "2",  OPTION_VAL_2_SV },
         { "3",  OPTION_VAL_3_SV },
         { "4",  OPTION_VAL_4_SV },
         { "5",  OPTION_VAL_5_SV },
         { "6",  OPTION_VAL_6_SV },
         { "7",  OPTION_VAL_7_SV },
         { "8",  OPTION_VAL_8_SV },
         { "9",  OPTION_VAL_9_SV },
         { "10", OPTION_VAL_10_SV },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_SV,
      FCEUMM_APU_1_LABEL_CAT_SV,
      FCEUMM_APU_1_INFO_0_SV,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_SV,
      FCEUMM_APU_2_LABEL_CAT_SV,
      FCEUMM_APU_2_INFO_0_SV,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_SV,
      FCEUMM_APU_3_LABEL_CAT_SV,
      FCEUMM_APU_3_INFO_0_SV,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_SV,
      FCEUMM_APU_4_LABEL_CAT_SV,
      FCEUMM_APU_4_INFO_0_SV,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_SV,
      FCEUMM_APU_5_LABEL_CAT_SV,
      FCEUMM_APU_5_INFO_0_SV,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_SV,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_SV,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_SV },
         { "Player 1", OPTION_VAL_PLAYER_1_SV },
         { "Player 2", OPTION_VAL_PLAYER_2_SV },
         { "Both",     OPTION_VAL_BOTH_SV },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_SV,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_SV,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_SV,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_SV,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_SV },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_SV },
         { "mouse",       OPTION_VAL_MOUSE_SV },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_SV,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_SV,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_SV,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_SV,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_SV,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_SV,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_SV,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_SV,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_SV,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_SV,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_SV },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_SV },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_SV,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_SV,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_SV },
         { "fill $00", OPTION_VAL_FILL_00_SV },
         { "random",   OPTION_VAL_RANDOM_SV },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_sv = {
   option_cats_sv,
   option_defs_sv
};

/* RETRO_LANGUAGE_TR */

#define CATEGORY_SYSTEM_LABEL_TR "Sistem"
#define CATEGORY_SYSTEM_INFO_0_TR "Bölge ve donanım eklenti parametrelerini yapılandırın."
#define CATEGORY_VIDEO_LABEL_TR NULL
#define CATEGORY_VIDEO_INFO_0_TR "En boy oranını, görüntü kırpmayı, renk paletini, video filtresi seçeneklerini yapılandırın."
#define CATEGORY_VIDEO_INFO_1_TR "En boy oranını, görüntü kırpmayı, renk paleti seçeneklerini yapılandırın."
#define CATEGORY_AUDIO_LABEL_TR "Ses"
#define CATEGORY_AUDIO_INFO_0_TR NULL
#define CATEGORY_INPUT_LABEL_TR "Girdi"
#define CATEGORY_INPUT_INFO_0_TR NULL
#define CATEGORY_HACKS_LABEL_TR "Emülatör Geliştirmeleri"
#define CATEGORY_HACKS_INFO_0_TR "Düşük seviyeli performans ve uyumluluğu etkileyen işlemci hız aşırtma ve emülatör doğruluğu parametrelerini yapılandırın."
#define CATEGORY_DIP_SWITCH_LABEL_TR NULL
#define CATEGORY_DIP_SWITCH_INFO_0_TR NULL
#define FCEUMM_REGION_LABEL_TR NULL
#define FCEUMM_REGION_INFO_0_TR NULL
#define OPTION_VAL_AUTO_TR NULL
#define OPTION_VAL_NTSC_TR NULL
#define OPTION_VAL_PAL_TR NULL
#define OPTION_VAL_DENDY_TR NULL
#define FCEUMM_GAME_GENIE_LABEL_TR NULL
#define FCEUMM_GAME_GENIE_INFO_0_TR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_TR NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_TR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_TR NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_TR NULL
#define FCEUMM_ASPECT_LABEL_TR NULL
#define FCEUMM_ASPECT_INFO_0_TR NULL
#define OPTION_VAL_8_7_PAR_TR NULL
#define OPTION_VAL_4_3_TR NULL
#define OPTION_VAL_PP_TR NULL
#define FCEUMM_OVERSCAN_LABEL_TR NULL
#define FCEUMM_OVERSCAN_INFO_0_TR NULL
#define FCEUMM_OVERSCAN_H_LABEL_TR NULL
#define FCEUMM_OVERSCAN_H_INFO_0_TR NULL
#define FCEUMM_OVERSCAN_V_LABEL_TR NULL
#define FCEUMM_OVERSCAN_V_INFO_0_TR NULL
#define FCEUMM_PALETTE_LABEL_TR NULL
#define FCEUMM_PALETTE_INFO_0_TR NULL
#define OPTION_VAL_DEFAULT_TR NULL
#define OPTION_VAL_ASQREALC_TR NULL
#define OPTION_VAL_NINTENDO_VC_TR NULL
#define OPTION_VAL_RGB_TR NULL
#define OPTION_VAL_YUV_V3_TR NULL
#define OPTION_VAL_UNSATURATED_FINAL_TR NULL
#define OPTION_VAL_SONY_CXA2025AS_US_TR NULL
#define OPTION_VAL_BMF_FINAL2_TR NULL
#define OPTION_VAL_BMF_FINAL3_TR NULL
#define OPTION_VAL_SMOOTH_FBX_TR NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_TR NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_TR NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_TR NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_TR NULL
#define OPTION_VAL_NESCAP_TR NULL
#define OPTION_VAL_WAVEBEAM_TR NULL
#define OPTION_VAL_RAW_TR NULL
#define OPTION_VAL_CUSTOM_TR NULL
#define FCEUMM_NTSC_FILTER_LABEL_TR NULL
#define FCEUMM_NTSC_FILTER_INFO_0_TR NULL
#define OPTION_VAL_COMPOSITE_TR NULL
#define OPTION_VAL_SVIDEO_TR NULL
#define OPTION_VAL_RGB_O9_TR NULL
#define OPTION_VAL_MONOCHROME_TR NULL
#define FCEUMM_SNDQUALITY_LABEL_TR NULL
#define FCEUMM_SNDQUALITY_INFO_0_TR NULL
#define OPTION_VAL_LOW_TR NULL
#define OPTION_VAL_HIGH_TR NULL
#define OPTION_VAL_VERY_HIGH_TR NULL
#define FCEUMM_SWAPDUTY_LABEL_TR NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_TR NULL
#define FCEUMM_SWAPDUTY_INFO_0_TR NULL
#define FCEUMM_SNDVOLUME_LABEL_TR NULL
#define FCEUMM_SNDVOLUME_INFO_0_TR NULL
#define OPTION_VAL_0_TR NULL
#define OPTION_VAL_1_TR NULL
#define OPTION_VAL_2_TR NULL
#define OPTION_VAL_3_TR NULL
#define OPTION_VAL_4_TR NULL
#define OPTION_VAL_5_TR NULL
#define OPTION_VAL_6_TR NULL
#define OPTION_VAL_7_TR NULL
#define OPTION_VAL_8_TR NULL
#define OPTION_VAL_9_TR NULL
#define OPTION_VAL_10_TR NULL
#define FCEUMM_APU_1_LABEL_TR NULL
#define FCEUMM_APU_1_LABEL_CAT_TR NULL
#define FCEUMM_APU_1_INFO_0_TR NULL
#define FCEUMM_APU_2_LABEL_TR NULL
#define FCEUMM_APU_2_LABEL_CAT_TR NULL
#define FCEUMM_APU_2_INFO_0_TR NULL
#define FCEUMM_APU_3_LABEL_TR NULL
#define FCEUMM_APU_3_LABEL_CAT_TR NULL
#define FCEUMM_APU_3_INFO_0_TR NULL
#define FCEUMM_APU_4_LABEL_TR NULL
#define FCEUMM_APU_4_LABEL_CAT_TR NULL
#define FCEUMM_APU_4_INFO_0_TR NULL
#define FCEUMM_APU_5_LABEL_TR NULL
#define FCEUMM_APU_5_LABEL_CAT_TR NULL
#define FCEUMM_APU_5_INFO_0_TR NULL
#define FCEUMM_TURBO_ENABLE_LABEL_TR NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_TR NULL
#define OPTION_VAL_NONE_TR NULL
#define OPTION_VAL_PLAYER_1_TR NULL
#define OPTION_VAL_PLAYER_2_TR NULL
#define OPTION_VAL_BOTH_TR NULL
#define FCEUMM_TURBO_DELAY_LABEL_TR NULL
#define FCEUMM_TURBO_DELAY_INFO_0_TR NULL
#define FCEUMM_ZAPPER_MODE_LABEL_TR NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_TR NULL
#define OPTION_VAL_LIGHTGUN_TR NULL
#define OPTION_VAL_TOUCHSCREEN_TR NULL
#define OPTION_VAL_MOUSE_TR NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_TR NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_TR NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_TR NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_TR NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_TR NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_TR NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_TR NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_TR NULL
#define FCEUMM_OVERCLOCKING_LABEL_TR NULL
#define FCEUMM_OVERCLOCKING_INFO_0_TR NULL
#define OPTION_VAL_2X_POSTRENDER_TR NULL
#define OPTION_VAL_2X_VBLANK_TR NULL
#define FCEUMM_RAMSTATE_LABEL_TR NULL
#define FCEUMM_RAMSTATE_INFO_0_TR NULL
#define OPTION_VAL_FILL_FF_TR NULL
#define OPTION_VAL_FILL_00_TR NULL
#define OPTION_VAL_RANDOM_TR NULL

struct retro_core_option_v2_category option_cats_tr[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_TR,
      CATEGORY_SYSTEM_INFO_0_TR
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_TR,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_TR
#else
      CATEGORY_VIDEO_INFO_1_TR
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_TR,
      CATEGORY_AUDIO_INFO_0_TR
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_TR,
      CATEGORY_INPUT_INFO_0_TR
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_TR,
      CATEGORY_HACKS_INFO_0_TR
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_TR,
      CATEGORY_DIP_SWITCH_INFO_0_TR
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_tr[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_TR,
      NULL,
      FCEUMM_REGION_INFO_0_TR,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_TR },
         { "NTSC",  OPTION_VAL_NTSC_TR },
         { "PAL",   OPTION_VAL_PAL_TR },
         { "Dendy", OPTION_VAL_DENDY_TR },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_TR,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_TR,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_TR,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_TR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_TR,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_TR,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_TR,
      NULL,
      FCEUMM_ASPECT_INFO_0_TR,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_TR },
         { "4:3",     OPTION_VAL_4_3_TR },
         { "PP",      OPTION_VAL_PP_TR },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_TR,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_TR,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_TR,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_TR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_TR,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_TR,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_TR,
      NULL,
      FCEUMM_PALETTE_INFO_0_TR,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_TR },
         { "asqrealc",             OPTION_VAL_ASQREALC_TR },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_TR },
         { "rgb",                  OPTION_VAL_RGB_TR },
         { "yuv-v3",               OPTION_VAL_YUV_V3_TR },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_TR },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_TR },
         { "pal",                  OPTION_VAL_PAL_TR },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_TR },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_TR },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_TR },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_TR },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_TR },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_TR },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_TR },
         { "nescap",               OPTION_VAL_NESCAP_TR },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_TR },
         { "raw",                  OPTION_VAL_RAW_TR },
         { "custom",               OPTION_VAL_CUSTOM_TR },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_TR,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_TR,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_TR },
         { "svideo",     OPTION_VAL_SVIDEO_TR },
         { "rgb",        OPTION_VAL_RGB_O9_TR },
         { "monochrome", OPTION_VAL_MONOCHROME_TR },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_TR,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_TR,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_TR },
         { "High",      OPTION_VAL_HIGH_TR },
         { "Very High", OPTION_VAL_VERY_HIGH_TR },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_TR,
      FCEUMM_SWAPDUTY_LABEL_CAT_TR,
      FCEUMM_SWAPDUTY_INFO_0_TR,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_TR,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_TR,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_TR },
         { "1",  OPTION_VAL_1_TR },
         { "2",  OPTION_VAL_2_TR },
         { "3",  OPTION_VAL_3_TR },
         { "4",  OPTION_VAL_4_TR },
         { "5",  OPTION_VAL_5_TR },
         { "6",  OPTION_VAL_6_TR },
         { "7",  OPTION_VAL_7_TR },
         { "8",  OPTION_VAL_8_TR },
         { "9",  OPTION_VAL_9_TR },
         { "10", OPTION_VAL_10_TR },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_TR,
      FCEUMM_APU_1_LABEL_CAT_TR,
      FCEUMM_APU_1_INFO_0_TR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_TR,
      FCEUMM_APU_2_LABEL_CAT_TR,
      FCEUMM_APU_2_INFO_0_TR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_TR,
      FCEUMM_APU_3_LABEL_CAT_TR,
      FCEUMM_APU_3_INFO_0_TR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_TR,
      FCEUMM_APU_4_LABEL_CAT_TR,
      FCEUMM_APU_4_INFO_0_TR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_TR,
      FCEUMM_APU_5_LABEL_CAT_TR,
      FCEUMM_APU_5_INFO_0_TR,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_TR,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_TR,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_TR },
         { "Player 1", OPTION_VAL_PLAYER_1_TR },
         { "Player 2", OPTION_VAL_PLAYER_2_TR },
         { "Both",     OPTION_VAL_BOTH_TR },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_TR,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_TR,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_TR,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_TR,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_TR },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_TR },
         { "mouse",       OPTION_VAL_MOUSE_TR },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_TR,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_TR,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_TR,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_TR,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_TR,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_TR,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_TR,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_TR,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_TR,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_TR,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_TR },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_TR },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_TR,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_TR,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_TR },
         { "fill $00", OPTION_VAL_FILL_00_TR },
         { "random",   OPTION_VAL_RANDOM_TR },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_tr = {
   option_cats_tr,
   option_defs_tr
};

/* RETRO_LANGUAGE_UK */

#define CATEGORY_SYSTEM_LABEL_UK "Система"
#define CATEGORY_SYSTEM_INFO_0_UK NULL
#define CATEGORY_VIDEO_LABEL_UK "Відео"
#define CATEGORY_VIDEO_INFO_0_UK NULL
#define CATEGORY_VIDEO_INFO_1_UK NULL
#define CATEGORY_AUDIO_LABEL_UK "Аудіо"
#define CATEGORY_AUDIO_INFO_0_UK NULL
#define CATEGORY_INPUT_LABEL_UK "Ввід"
#define CATEGORY_INPUT_INFO_0_UK NULL
#define CATEGORY_HACKS_LABEL_UK "Гаки емуляції"
#define CATEGORY_HACKS_INFO_0_UK "Налаштування розгону процесора та точності емуляції, що впливають на низькорівневу продуктивність та сумісність."
#define CATEGORY_DIP_SWITCH_LABEL_UK NULL
#define CATEGORY_DIP_SWITCH_INFO_0_UK NULL
#define FCEUMM_REGION_LABEL_UK NULL
#define FCEUMM_REGION_INFO_0_UK NULL
#define OPTION_VAL_AUTO_UK NULL
#define OPTION_VAL_NTSC_UK NULL
#define OPTION_VAL_PAL_UK NULL
#define OPTION_VAL_DENDY_UK NULL
#define FCEUMM_GAME_GENIE_LABEL_UK NULL
#define FCEUMM_GAME_GENIE_INFO_0_UK NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_UK NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_UK NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_UK NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_UK NULL
#define FCEUMM_ASPECT_LABEL_UK NULL
#define FCEUMM_ASPECT_INFO_0_UK NULL
#define OPTION_VAL_8_7_PAR_UK NULL
#define OPTION_VAL_4_3_UK NULL
#define OPTION_VAL_PP_UK NULL
#define FCEUMM_OVERSCAN_LABEL_UK NULL
#define FCEUMM_OVERSCAN_INFO_0_UK NULL
#define FCEUMM_OVERSCAN_H_LABEL_UK NULL
#define FCEUMM_OVERSCAN_H_INFO_0_UK NULL
#define FCEUMM_OVERSCAN_V_LABEL_UK NULL
#define FCEUMM_OVERSCAN_V_INFO_0_UK NULL
#define FCEUMM_PALETTE_LABEL_UK NULL
#define FCEUMM_PALETTE_INFO_0_UK NULL
#define OPTION_VAL_DEFAULT_UK NULL
#define OPTION_VAL_ASQREALC_UK NULL
#define OPTION_VAL_NINTENDO_VC_UK NULL
#define OPTION_VAL_RGB_UK NULL
#define OPTION_VAL_YUV_V3_UK NULL
#define OPTION_VAL_UNSATURATED_FINAL_UK NULL
#define OPTION_VAL_SONY_CXA2025AS_US_UK NULL
#define OPTION_VAL_BMF_FINAL2_UK NULL
#define OPTION_VAL_BMF_FINAL3_UK NULL
#define OPTION_VAL_SMOOTH_FBX_UK NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_UK NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_UK NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_UK NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_UK NULL
#define OPTION_VAL_NESCAP_UK NULL
#define OPTION_VAL_WAVEBEAM_UK NULL
#define OPTION_VAL_RAW_UK NULL
#define OPTION_VAL_CUSTOM_UK NULL
#define FCEUMM_NTSC_FILTER_LABEL_UK NULL
#define FCEUMM_NTSC_FILTER_INFO_0_UK NULL
#define OPTION_VAL_COMPOSITE_UK NULL
#define OPTION_VAL_SVIDEO_UK NULL
#define OPTION_VAL_RGB_O9_UK NULL
#define OPTION_VAL_MONOCHROME_UK NULL
#define FCEUMM_SNDQUALITY_LABEL_UK NULL
#define FCEUMM_SNDQUALITY_INFO_0_UK NULL
#define OPTION_VAL_LOW_UK NULL
#define OPTION_VAL_HIGH_UK NULL
#define OPTION_VAL_VERY_HIGH_UK NULL
#define FCEUMM_SWAPDUTY_LABEL_UK NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_UK NULL
#define FCEUMM_SWAPDUTY_INFO_0_UK NULL
#define FCEUMM_SNDVOLUME_LABEL_UK NULL
#define FCEUMM_SNDVOLUME_INFO_0_UK NULL
#define OPTION_VAL_0_UK NULL
#define OPTION_VAL_1_UK NULL
#define OPTION_VAL_2_UK NULL
#define OPTION_VAL_3_UK NULL
#define OPTION_VAL_4_UK NULL
#define OPTION_VAL_5_UK NULL
#define OPTION_VAL_6_UK NULL
#define OPTION_VAL_7_UK NULL
#define OPTION_VAL_8_UK NULL
#define OPTION_VAL_9_UK NULL
#define OPTION_VAL_10_UK NULL
#define FCEUMM_APU_1_LABEL_UK NULL
#define FCEUMM_APU_1_LABEL_CAT_UK NULL
#define FCEUMM_APU_1_INFO_0_UK NULL
#define FCEUMM_APU_2_LABEL_UK NULL
#define FCEUMM_APU_2_LABEL_CAT_UK NULL
#define FCEUMM_APU_2_INFO_0_UK NULL
#define FCEUMM_APU_3_LABEL_UK NULL
#define FCEUMM_APU_3_LABEL_CAT_UK NULL
#define FCEUMM_APU_3_INFO_0_UK NULL
#define FCEUMM_APU_4_LABEL_UK NULL
#define FCEUMM_APU_4_LABEL_CAT_UK NULL
#define FCEUMM_APU_4_INFO_0_UK NULL
#define FCEUMM_APU_5_LABEL_UK NULL
#define FCEUMM_APU_5_LABEL_CAT_UK NULL
#define FCEUMM_APU_5_INFO_0_UK NULL
#define FCEUMM_TURBO_ENABLE_LABEL_UK NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_UK NULL
#define OPTION_VAL_NONE_UK NULL
#define OPTION_VAL_PLAYER_1_UK NULL
#define OPTION_VAL_PLAYER_2_UK NULL
#define OPTION_VAL_BOTH_UK NULL
#define FCEUMM_TURBO_DELAY_LABEL_UK NULL
#define FCEUMM_TURBO_DELAY_INFO_0_UK NULL
#define FCEUMM_ZAPPER_MODE_LABEL_UK NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_UK NULL
#define OPTION_VAL_LIGHTGUN_UK NULL
#define OPTION_VAL_TOUCHSCREEN_UK NULL
#define OPTION_VAL_MOUSE_UK NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_UK NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_UK NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_UK NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_UK NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_UK NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_UK NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_UK NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_UK NULL
#define FCEUMM_OVERCLOCKING_LABEL_UK NULL
#define FCEUMM_OVERCLOCKING_INFO_0_UK NULL
#define OPTION_VAL_2X_POSTRENDER_UK NULL
#define OPTION_VAL_2X_VBLANK_UK NULL
#define FCEUMM_RAMSTATE_LABEL_UK NULL
#define FCEUMM_RAMSTATE_INFO_0_UK NULL
#define OPTION_VAL_FILL_FF_UK NULL
#define OPTION_VAL_FILL_00_UK NULL
#define OPTION_VAL_RANDOM_UK NULL

struct retro_core_option_v2_category option_cats_uk[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_UK,
      CATEGORY_SYSTEM_INFO_0_UK
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_UK,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_UK
#else
      CATEGORY_VIDEO_INFO_1_UK
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_UK,
      CATEGORY_AUDIO_INFO_0_UK
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_UK,
      CATEGORY_INPUT_INFO_0_UK
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_UK,
      CATEGORY_HACKS_INFO_0_UK
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_UK,
      CATEGORY_DIP_SWITCH_INFO_0_UK
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_uk[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_UK,
      NULL,
      FCEUMM_REGION_INFO_0_UK,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_UK },
         { "NTSC",  OPTION_VAL_NTSC_UK },
         { "PAL",   OPTION_VAL_PAL_UK },
         { "Dendy", OPTION_VAL_DENDY_UK },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_UK,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_UK,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_UK,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_UK,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_UK,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_UK,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_UK,
      NULL,
      FCEUMM_ASPECT_INFO_0_UK,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_UK },
         { "4:3",     OPTION_VAL_4_3_UK },
         { "PP",      OPTION_VAL_PP_UK },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_UK,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_UK,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_UK,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_UK,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_UK,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_UK,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_UK,
      NULL,
      FCEUMM_PALETTE_INFO_0_UK,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_UK },
         { "asqrealc",             OPTION_VAL_ASQREALC_UK },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_UK },
         { "rgb",                  OPTION_VAL_RGB_UK },
         { "yuv-v3",               OPTION_VAL_YUV_V3_UK },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_UK },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_UK },
         { "pal",                  OPTION_VAL_PAL_UK },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_UK },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_UK },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_UK },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_UK },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_UK },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_UK },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_UK },
         { "nescap",               OPTION_VAL_NESCAP_UK },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_UK },
         { "raw",                  OPTION_VAL_RAW_UK },
         { "custom",               OPTION_VAL_CUSTOM_UK },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_UK,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_UK,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_UK },
         { "svideo",     OPTION_VAL_SVIDEO_UK },
         { "rgb",        OPTION_VAL_RGB_O9_UK },
         { "monochrome", OPTION_VAL_MONOCHROME_UK },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_UK,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_UK,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_UK },
         { "High",      OPTION_VAL_HIGH_UK },
         { "Very High", OPTION_VAL_VERY_HIGH_UK },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_UK,
      FCEUMM_SWAPDUTY_LABEL_CAT_UK,
      FCEUMM_SWAPDUTY_INFO_0_UK,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_UK,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_UK,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_UK },
         { "1",  OPTION_VAL_1_UK },
         { "2",  OPTION_VAL_2_UK },
         { "3",  OPTION_VAL_3_UK },
         { "4",  OPTION_VAL_4_UK },
         { "5",  OPTION_VAL_5_UK },
         { "6",  OPTION_VAL_6_UK },
         { "7",  OPTION_VAL_7_UK },
         { "8",  OPTION_VAL_8_UK },
         { "9",  OPTION_VAL_9_UK },
         { "10", OPTION_VAL_10_UK },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_UK,
      FCEUMM_APU_1_LABEL_CAT_UK,
      FCEUMM_APU_1_INFO_0_UK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_UK,
      FCEUMM_APU_2_LABEL_CAT_UK,
      FCEUMM_APU_2_INFO_0_UK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_UK,
      FCEUMM_APU_3_LABEL_CAT_UK,
      FCEUMM_APU_3_INFO_0_UK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_UK,
      FCEUMM_APU_4_LABEL_CAT_UK,
      FCEUMM_APU_4_INFO_0_UK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_UK,
      FCEUMM_APU_5_LABEL_CAT_UK,
      FCEUMM_APU_5_INFO_0_UK,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_UK,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_UK,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_UK },
         { "Player 1", OPTION_VAL_PLAYER_1_UK },
         { "Player 2", OPTION_VAL_PLAYER_2_UK },
         { "Both",     OPTION_VAL_BOTH_UK },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_UK,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_UK,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_UK,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_UK,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_UK },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_UK },
         { "mouse",       OPTION_VAL_MOUSE_UK },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_UK,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_UK,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_UK,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_UK,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_UK,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_UK,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_UK,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_UK,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_UK,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_UK,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_UK },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_UK },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_UK,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_UK,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_UK },
         { "fill $00", OPTION_VAL_FILL_00_UK },
         { "random",   OPTION_VAL_RANDOM_UK },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_uk = {
   option_cats_uk,
   option_defs_uk
};

/* RETRO_LANGUAGE_VN */

#define CATEGORY_SYSTEM_LABEL_VN "Hệ thống"
#define CATEGORY_SYSTEM_INFO_0_VN NULL
#define CATEGORY_VIDEO_LABEL_VN "Hình ảnh"
#define CATEGORY_VIDEO_INFO_0_VN NULL
#define CATEGORY_VIDEO_INFO_1_VN NULL
#define CATEGORY_AUDIO_LABEL_VN "Âm thanh"
#define CATEGORY_AUDIO_INFO_0_VN NULL
#define CATEGORY_INPUT_LABEL_VN "Input Driver"
#define CATEGORY_INPUT_INFO_0_VN NULL
#define CATEGORY_HACKS_LABEL_VN NULL
#define CATEGORY_HACKS_INFO_0_VN NULL
#define CATEGORY_DIP_SWITCH_LABEL_VN NULL
#define CATEGORY_DIP_SWITCH_INFO_0_VN NULL
#define FCEUMM_REGION_LABEL_VN NULL
#define FCEUMM_REGION_INFO_0_VN NULL
#define OPTION_VAL_AUTO_VN NULL
#define OPTION_VAL_NTSC_VN NULL
#define OPTION_VAL_PAL_VN NULL
#define OPTION_VAL_DENDY_VN NULL
#define FCEUMM_GAME_GENIE_LABEL_VN NULL
#define FCEUMM_GAME_GENIE_INFO_0_VN NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_VN NULL
#define FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_VN NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_VN NULL
#define FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_VN NULL
#define FCEUMM_ASPECT_LABEL_VN NULL
#define FCEUMM_ASPECT_INFO_0_VN NULL
#define OPTION_VAL_8_7_PAR_VN NULL
#define OPTION_VAL_4_3_VN NULL
#define OPTION_VAL_PP_VN NULL
#define FCEUMM_OVERSCAN_LABEL_VN NULL
#define FCEUMM_OVERSCAN_INFO_0_VN NULL
#define FCEUMM_OVERSCAN_H_LABEL_VN NULL
#define FCEUMM_OVERSCAN_H_INFO_0_VN NULL
#define FCEUMM_OVERSCAN_V_LABEL_VN NULL
#define FCEUMM_OVERSCAN_V_INFO_0_VN NULL
#define FCEUMM_PALETTE_LABEL_VN NULL
#define FCEUMM_PALETTE_INFO_0_VN NULL
#define OPTION_VAL_DEFAULT_VN NULL
#define OPTION_VAL_ASQREALC_VN NULL
#define OPTION_VAL_NINTENDO_VC_VN NULL
#define OPTION_VAL_RGB_VN NULL
#define OPTION_VAL_YUV_V3_VN NULL
#define OPTION_VAL_UNSATURATED_FINAL_VN NULL
#define OPTION_VAL_SONY_CXA2025AS_US_VN NULL
#define OPTION_VAL_BMF_FINAL2_VN NULL
#define OPTION_VAL_BMF_FINAL3_VN NULL
#define OPTION_VAL_SMOOTH_FBX_VN NULL
#define OPTION_VAL_COMPOSITE_DIRECT_FBX_VN NULL
#define OPTION_VAL_PVM_STYLE_D93_FBX_VN NULL
#define OPTION_VAL_NTSC_HARDWARE_FBX_VN NULL
#define OPTION_VAL_NES_CLASSIC_FBX_FS_VN NULL
#define OPTION_VAL_NESCAP_VN NULL
#define OPTION_VAL_WAVEBEAM_VN NULL
#define OPTION_VAL_RAW_VN NULL
#define OPTION_VAL_CUSTOM_VN NULL
#define FCEUMM_NTSC_FILTER_LABEL_VN NULL
#define FCEUMM_NTSC_FILTER_INFO_0_VN NULL
#define OPTION_VAL_COMPOSITE_VN NULL
#define OPTION_VAL_SVIDEO_VN NULL
#define OPTION_VAL_RGB_O9_VN NULL
#define OPTION_VAL_MONOCHROME_VN NULL
#define FCEUMM_SNDQUALITY_LABEL_VN NULL
#define FCEUMM_SNDQUALITY_INFO_0_VN NULL
#define OPTION_VAL_LOW_VN NULL
#define OPTION_VAL_HIGH_VN NULL
#define OPTION_VAL_VERY_HIGH_VN NULL
#define FCEUMM_SWAPDUTY_LABEL_VN NULL
#define FCEUMM_SWAPDUTY_LABEL_CAT_VN NULL
#define FCEUMM_SWAPDUTY_INFO_0_VN NULL
#define FCEUMM_SNDVOLUME_LABEL_VN NULL
#define FCEUMM_SNDVOLUME_INFO_0_VN NULL
#define OPTION_VAL_0_VN NULL
#define OPTION_VAL_1_VN NULL
#define OPTION_VAL_2_VN NULL
#define OPTION_VAL_3_VN NULL
#define OPTION_VAL_4_VN NULL
#define OPTION_VAL_5_VN NULL
#define OPTION_VAL_6_VN NULL
#define OPTION_VAL_7_VN NULL
#define OPTION_VAL_8_VN NULL
#define OPTION_VAL_9_VN NULL
#define OPTION_VAL_10_VN NULL
#define FCEUMM_APU_1_LABEL_VN NULL
#define FCEUMM_APU_1_LABEL_CAT_VN NULL
#define FCEUMM_APU_1_INFO_0_VN NULL
#define FCEUMM_APU_2_LABEL_VN NULL
#define FCEUMM_APU_2_LABEL_CAT_VN NULL
#define FCEUMM_APU_2_INFO_0_VN NULL
#define FCEUMM_APU_3_LABEL_VN NULL
#define FCEUMM_APU_3_LABEL_CAT_VN NULL
#define FCEUMM_APU_3_INFO_0_VN NULL
#define FCEUMM_APU_4_LABEL_VN NULL
#define FCEUMM_APU_4_LABEL_CAT_VN NULL
#define FCEUMM_APU_4_INFO_0_VN NULL
#define FCEUMM_APU_5_LABEL_VN NULL
#define FCEUMM_APU_5_LABEL_CAT_VN NULL
#define FCEUMM_APU_5_INFO_0_VN NULL
#define FCEUMM_TURBO_ENABLE_LABEL_VN NULL
#define FCEUMM_TURBO_ENABLE_INFO_0_VN NULL
#define OPTION_VAL_NONE_VN NULL
#define OPTION_VAL_PLAYER_1_VN NULL
#define OPTION_VAL_PLAYER_2_VN NULL
#define OPTION_VAL_BOTH_VN NULL
#define FCEUMM_TURBO_DELAY_LABEL_VN NULL
#define FCEUMM_TURBO_DELAY_INFO_0_VN NULL
#define FCEUMM_ZAPPER_MODE_LABEL_VN NULL
#define FCEUMM_ZAPPER_MODE_INFO_0_VN NULL
#define OPTION_VAL_LIGHTGUN_VN NULL
#define OPTION_VAL_TOUCHSCREEN_VN NULL
#define OPTION_VAL_MOUSE_VN NULL
#define FCEUMM_SHOW_CROSSHAIR_LABEL_VN NULL
#define FCEUMM_SHOW_CROSSHAIR_INFO_0_VN NULL
#define FCEUMM_ZAPPER_TOLERANCE_LABEL_VN NULL
#define FCEUMM_ZAPPER_TOLERANCE_INFO_0_VN NULL
#define FCEUMM_UP_DOWN_ALLOWED_LABEL_VN NULL
#define FCEUMM_UP_DOWN_ALLOWED_INFO_0_VN NULL
#define FCEUMM_NOSPRITELIMIT_LABEL_VN NULL
#define FCEUMM_NOSPRITELIMIT_INFO_0_VN NULL
#define FCEUMM_OVERCLOCKING_LABEL_VN NULL
#define FCEUMM_OVERCLOCKING_INFO_0_VN NULL
#define OPTION_VAL_2X_POSTRENDER_VN NULL
#define OPTION_VAL_2X_VBLANK_VN NULL
#define FCEUMM_RAMSTATE_LABEL_VN NULL
#define FCEUMM_RAMSTATE_INFO_0_VN NULL
#define OPTION_VAL_FILL_FF_VN NULL
#define OPTION_VAL_FILL_00_VN NULL
#define OPTION_VAL_RANDOM_VN NULL

struct retro_core_option_v2_category option_cats_vn[] = {
   {
      "system",
      CATEGORY_SYSTEM_LABEL_VN,
      CATEGORY_SYSTEM_INFO_0_VN
   },
   {
      "video",
      CATEGORY_VIDEO_LABEL_VN,
#ifdef HAVE_NTSC_FILTER
      CATEGORY_VIDEO_INFO_0_VN
#else
      CATEGORY_VIDEO_INFO_1_VN
#endif
   },
   {
      "audio",
      CATEGORY_AUDIO_LABEL_VN,
      CATEGORY_AUDIO_INFO_0_VN
   },
   {
      "input",
      CATEGORY_INPUT_LABEL_VN,
      CATEGORY_INPUT_INFO_0_VN
   },
   {
      "hacks",
      CATEGORY_HACKS_LABEL_VN,
      CATEGORY_HACKS_INFO_0_VN
   },
   {
      "dip_switch",
      CATEGORY_DIP_SWITCH_LABEL_VN,
      CATEGORY_DIP_SWITCH_INFO_0_VN
   },
   { NULL, NULL, NULL },
};
struct retro_core_option_v2_definition option_defs_vn[] = {
   {
      "fceumm_region",
      FCEUMM_REGION_LABEL_VN,
      NULL,
      FCEUMM_REGION_INFO_0_VN,
      NULL,
      "system",
      {
         { "Auto",  OPTION_VAL_AUTO_VN },
         { "NTSC",  OPTION_VAL_NTSC_VN },
         { "PAL",   OPTION_VAL_PAL_VN },
         { "Dendy", OPTION_VAL_DENDY_VN },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_game_genie",
      FCEUMM_GAME_GENIE_LABEL_VN,
      NULL,
      FCEUMM_GAME_GENIE_INFO_0_VN,
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_show_adv_system_options",
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_LABEL_VN,
      NULL,
      FCEUMM_SHOW_ADV_SYSTEM_OPTIONS_INFO_0_VN,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_show_adv_sound_options",
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_LABEL_VN,
      NULL,
      FCEUMM_SHOW_ADV_SOUND_OPTIONS_INFO_0_VN,
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_aspect",
      FCEUMM_ASPECT_LABEL_VN,
      NULL,
      FCEUMM_ASPECT_INFO_0_VN,
      NULL,
      "video",
      {
         { "8:7 PAR", OPTION_VAL_8_7_PAR_VN },
         { "4:3",     OPTION_VAL_4_3_VN },
         { "PP",      OPTION_VAL_PP_VN },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_overscan",
      FCEUMM_OVERSCAN_LABEL_VN,
      NULL,
      FCEUMM_OVERSCAN_INFO_0_VN,
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_overscan_h",
      FCEUMM_OVERSCAN_H_LABEL_VN,
      NULL,
      FCEUMM_OVERSCAN_H_INFO_0_VN,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_overscan_v",
      FCEUMM_OVERSCAN_V_LABEL_VN,
      NULL,
      FCEUMM_OVERSCAN_V_INFO_0_VN,
      NULL,
      "video",
      {
         { "0",  "None" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_palette",
      FCEUMM_PALETTE_LABEL_VN,
      NULL,
      FCEUMM_PALETTE_INFO_0_VN,
      NULL,
      "video",
      {
         { "default",              OPTION_VAL_DEFAULT_VN },
         { "asqrealc",             OPTION_VAL_ASQREALC_VN },
         { "nintendo-vc",          OPTION_VAL_NINTENDO_VC_VN },
         { "rgb",                  OPTION_VAL_RGB_VN },
         { "yuv-v3",               OPTION_VAL_YUV_V3_VN },
         { "unsaturated-final",    OPTION_VAL_UNSATURATED_FINAL_VN },
         { "sony-cxa2025as-us",    OPTION_VAL_SONY_CXA2025AS_US_VN },
         { "pal",                  OPTION_VAL_PAL_VN },
         { "bmf-final2",           OPTION_VAL_BMF_FINAL2_VN },
         { "bmf-final3",           OPTION_VAL_BMF_FINAL3_VN },
         { "smooth-fbx",           OPTION_VAL_SMOOTH_FBX_VN },
         { "composite-direct-fbx", OPTION_VAL_COMPOSITE_DIRECT_FBX_VN },
         { "pvm-style-d93-fbx",    OPTION_VAL_PVM_STYLE_D93_FBX_VN },
         { "ntsc-hardware-fbx",    OPTION_VAL_NTSC_HARDWARE_FBX_VN },
         { "nes-classic-fbx-fs",   OPTION_VAL_NES_CLASSIC_FBX_FS_VN },
         { "nescap",               OPTION_VAL_NESCAP_VN },
         { "wavebeam",             OPTION_VAL_WAVEBEAM_VN },
         { "raw",                  OPTION_VAL_RAW_VN },
         { "custom",               OPTION_VAL_CUSTOM_VN },
         { NULL, NULL },
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_ntsc_filter",
      FCEUMM_NTSC_FILTER_LABEL_VN,
      NULL,
      FCEUMM_NTSC_FILTER_INFO_0_VN,
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  OPTION_VAL_COMPOSITE_VN },
         { "svideo",     OPTION_VAL_SVIDEO_VN },
         { "rgb",        OPTION_VAL_RGB_O9_VN },
         { "monochrome", OPTION_VAL_MONOCHROME_VN },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_sndquality",
      FCEUMM_SNDQUALITY_LABEL_VN,
      NULL,
      FCEUMM_SNDQUALITY_INFO_0_VN,
      NULL,
      "audio",
      {
         { "Low",       OPTION_VAL_LOW_VN },
         { "High",      OPTION_VAL_HIGH_VN },
         { "Very High", OPTION_VAL_VERY_HIGH_VN },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_swapduty",
      FCEUMM_SWAPDUTY_LABEL_VN,
      FCEUMM_SWAPDUTY_LABEL_CAT_VN,
      FCEUMM_SWAPDUTY_INFO_0_VN,
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_sndvolume",
      FCEUMM_SNDVOLUME_LABEL_VN,
      NULL,
      FCEUMM_SNDVOLUME_INFO_0_VN,
      NULL,
      "audio",
      {
         { "0",  OPTION_VAL_0_VN },
         { "1",  OPTION_VAL_1_VN },
         { "2",  OPTION_VAL_2_VN },
         { "3",  OPTION_VAL_3_VN },
         { "4",  OPTION_VAL_4_VN },
         { "5",  OPTION_VAL_5_VN },
         { "6",  OPTION_VAL_6_VN },
         { "7",  OPTION_VAL_7_VN },
         { "8",  OPTION_VAL_8_VN },
         { "9",  OPTION_VAL_9_VN },
         { "10", OPTION_VAL_10_VN },
         { NULL, NULL },
      },
      "7",
   },
   {
      "fceumm_apu_1",
      FCEUMM_APU_1_LABEL_VN,
      FCEUMM_APU_1_LABEL_CAT_VN,
      FCEUMM_APU_1_INFO_0_VN,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_2",
      FCEUMM_APU_2_LABEL_VN,
      FCEUMM_APU_2_LABEL_CAT_VN,
      FCEUMM_APU_2_INFO_0_VN,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_3",
      FCEUMM_APU_3_LABEL_VN,
      FCEUMM_APU_3_LABEL_CAT_VN,
      FCEUMM_APU_3_INFO_0_VN,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_4",
      FCEUMM_APU_4_LABEL_VN,
      FCEUMM_APU_4_LABEL_CAT_VN,
      FCEUMM_APU_4_INFO_0_VN,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_apu_5",
      FCEUMM_APU_5_LABEL_VN,
      FCEUMM_APU_5_LABEL_CAT_VN,
      FCEUMM_APU_5_INFO_0_VN,
      NULL,
      "audio",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_turbo_enable",
      FCEUMM_TURBO_ENABLE_LABEL_VN,
      NULL,
      FCEUMM_TURBO_ENABLE_INFO_0_VN,
      NULL,
      "input",
      {
         { "None",     OPTION_VAL_NONE_VN },
         { "Player 1", OPTION_VAL_PLAYER_1_VN },
         { "Player 2", OPTION_VAL_PLAYER_2_VN },
         { "Both",     OPTION_VAL_BOTH_VN },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_turbo_delay",
      FCEUMM_TURBO_DELAY_LABEL_VN,
      NULL,
      FCEUMM_TURBO_DELAY_INFO_0_VN,
      NULL,
      "input",
      {
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "5",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_zapper_mode",
      FCEUMM_ZAPPER_MODE_LABEL_VN,
      NULL,
      FCEUMM_ZAPPER_MODE_INFO_0_VN,
      NULL,
      "input",
      {
         { "lightgun",    OPTION_VAL_LIGHTGUN_VN },
         { "touchscreen", OPTION_VAL_TOUCHSCREEN_VN },
         { "mouse",       OPTION_VAL_MOUSE_VN },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_show_crosshair",
      FCEUMM_SHOW_CROSSHAIR_LABEL_VN,
      NULL,
      FCEUMM_SHOW_CROSSHAIR_INFO_0_VN,
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_zapper_tolerance",
      FCEUMM_ZAPPER_TOLERANCE_LABEL_VN,
      NULL,
      FCEUMM_ZAPPER_TOLERANCE_INFO_0_VN,
      NULL,
      "input",
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
      "fceumm_up_down_allowed",
      FCEUMM_UP_DOWN_ALLOWED_LABEL_VN,
      NULL,
      FCEUMM_UP_DOWN_ALLOWED_INFO_0_VN,
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_nospritelimit",
      FCEUMM_NOSPRITELIMIT_LABEL_VN,
      NULL,
      FCEUMM_NOSPRITELIMIT_INFO_0_VN,
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_overclocking",
      FCEUMM_OVERCLOCKING_LABEL_VN,
      NULL,
      FCEUMM_OVERCLOCKING_INFO_0_VN,
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", OPTION_VAL_2X_POSTRENDER_VN },
         { "2x-VBlank",     OPTION_VAL_2X_VBLANK_VN },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_ramstate",
      FCEUMM_RAMSTATE_LABEL_VN,
      NULL,
      FCEUMM_RAMSTATE_INFO_0_VN,
      NULL,
      "hacks",
      {
         { "fill $ff", OPTION_VAL_FILL_FF_VN },
         { "fill $00", OPTION_VAL_FILL_00_VN },
         { "random",   OPTION_VAL_RANDOM_VN },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};
struct retro_core_options_v2 options_vn = {
   option_cats_vn,
   option_defs_vn
};


#ifdef __cplusplus
}
#endif

#endif