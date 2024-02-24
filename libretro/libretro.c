#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef _MSC_VER
#include <compat/msvc.h>
#endif

#include <libretro.h>
#include <string/stdstring.h>
#include <file/file_path.h>
#include <streams/file_stream.h>
#include <streams/memory_stream.h>
#include <libretro_dipswitch.h>
#include <libretro_core_options.h>

#include "../src/fceu.h"
#include "../src/fceu-endian.h"
#include "../src/input.h"
#include "../src/state.h"
#include "../src/ppu.h"
#include "../src/cart.h"
#include "../src/x6502.h"
#include "../src/git.h"
#include "../src/palette.h"
#include "../src/sound.h"
#include "../src/file.h"
#include "../src/cheat.h"
#include "../src/ines.h"
#include "../src/unif.h"
#include "../src/fds.h"
#include "../src/vsuni.h"
#include "../src/video.h"
#include "../src/gamegenie.h"

#include "libretro_input.h"
#include "paldef.h"

#if defined(PSP)
#include "pspgu.h"
#endif

#if defined(PS2)
#include "libretro-common/include/libretro_gskit_ps2.h"
#endif

#if defined(PSP)
#define RED_SHIFT    0
#define GREEN_SHIFT  5
#define BLUE_SHIFT   11
#define RED_EXPAND   3
#define GREEN_EXPAND 2
#define BLUE_EXPAND  3
#define BUILD_PIXEL(R, G, B) (((int)((R)&0x1f) << RED_SHIFT) | ((int)((G)&0x3f) << GREEN_SHIFT) | ((int)((B)&0x1f) << BLUE_SHIFT))
typedef uint16 Bpp_t;
#elif defined(FRONTEND_SUPPORTS_ABGR1555) /* PS2 */
typedef uint16 Bpp_t;
#define RED_SHIFT    0
#define GREEN_SHIFT  5
#define BLUE_SHIFT   10
#define RED_EXPAND   3
#define GREEN_EXPAND 3
#define BLUE_EXPAND  3
#define RED_MASK     0x1F
#define GREEN_MASK   0x3E0
#define BLUE_MASK    0x7C00
#define BUILD_PIXEL(R, G, B) (((int)((R)&0x1f) << RED_SHIFT) | ((int)((G)&0x3f) << GREEN_SHIFT) | ((int)((B)&0x1f) << BLUE_SHIFT))
typedef uint16 Bpp_t;
#elif defined(FRONTEND_SUPPORTS_RGB565)
typedef uint16 Bpp_t;
#define RED_SHIFT    11
#define GREEN_SHIFT  5
#define BLUE_SHIFT   0
#define RED_EXPAND   3
#define GREEN_EXPAND 2
#define BLUE_EXPAND  3
#define RED_MASK     0xF800
#define GREEN_MASK   0x7e0
#define BLUE_MASK    0x1f
#define BUILD_PIXEL(R, G, B) (((int)((R)&0x1f) << RED_SHIFT) | ((int)((G)&0x3f) << GREEN_SHIFT) | ((int)((B)&0x1f) << BLUE_SHIFT))
#elif defined(FRONTEND_SUPPORTS_ARGB888)
typedef uint32 Bpp_t;
#define RED_SHIFT    16
#define GREEN_SHIFT  8
#define BLUE_SHIFT   0
#define RED_EXPAND   0
#define GREEN_EXPAND 0
#define BLUE_EXPAND  0
#define ALPHA_SHIFT 24
#define BUILD_PIXEL(R, G, B) (((int)(R) << RED_SHIFT) | ((int)(G) << GREEN_SHIFT) | ((int)(B) << BLUE_SHIFT))
#else
typedef uint16 Bpp_t;
#define RED_SHIFT    10
#define GREEN_SHIFT  5
#define BLUE_SHIFT   0
#define RED_EXPAND   3
#define GREEN_EXPAND 3
#define BLUE_EXPAND  3
#define BUILD_PIXEL(R, G, B) (((int)((R)&0x1f) << RED_SHIFT) | ((int)((G)&0x3f) << GREEN_SHIFT) | ((int)((B)&0x1f) << BLUE_SHIFT))
#endif

#define NES_8_7_PAR       ((NES_WIDTH - (overscan_left + overscan_right)) * (8.0 / 7.0)) / (NES_HEIGHT - (overscan_top + overscan_bottom))
#define NES_4_3           (((NES_WIDTH - (overscan_left + overscan_right)) / ((NES_HEIGHT - (overscan_top + overscan_bottom)) * (NES_WIDTH / NES_HEIGHT))) * 4.0 / 3.0)
#define NES_PP            (((NES_WIDTH - (overscan_left + overscan_right)) / ((NES_HEIGHT - (overscan_top + overscan_bottom)) * (NES_WIDTH / NES_HEIGHT))) * 16.0 / 15.0)
#define NES_TARGET_FPS    ((double)FCEUI_GetDesiredFPS() / 16777215.0)

#if defined(_3DS)
void *linearMemAlign(size_t size, size_t alignment);
void linearFree(void *mem);
#endif

#if defined(PS2)
RETRO_HW_RENDER_INTEFACE_GSKIT_PS2 *ps2 = NULL;
#endif

static retro_video_refresh_t video_cb            = NULL;
static retro_input_poll_t poll_cb                = NULL;
static retro_input_state_t input_cb              = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_environment_t environ_cb            = NULL;
static struct retro_log_callback log_cb          = { 0 };

#if defined(PSP)
static bool crop_overscan;
#endif

static unsigned overscan_left    = 0;
static unsigned overscan_right   = 0;
static unsigned overscan_top     = 0;
static unsigned overscan_bottom  = 0;
static unsigned aspect_ratio_par = 0;

static bool use_raw_palette = false;

bool libretro_supports_bitmasks                 = false;
static bool libretro_supports_option_categories = false;
static unsigned libretro_msg_interface_version  = 0;

/* emulator-specific variables */

const size_t PPU_BIT = 1ULL << 31ULL;

static uint8 opt_region              = 0;
static bool opt_showAdvSoundOptions  = true;
static bool opt_showAdvSystemOptions = true;

#if defined(PSP) || defined(PS2)
static __attribute__((aligned(16))) uint16 retro_palette[1024];
#else
static Bpp_t retro_palette[1024];
#endif
#if defined(PSP) || defined(PS2)
/* not used because of hw buffers? */
/* static uint8* fceu_video_out; */
#else
static Bpp_t *fceu_video_out;
#endif

/* Some timing-related variables. */
static uint8 sndquality;

static uint32 current_palette = 0;
static unsigned serialize_size;

/* extern forward decls.*/
extern uint8 PALRAM[0x20];
extern uint8 SPRAM[0x100];

extern int palette_game_available;
extern int palette_user_available;

/* emulator-specific callback functions */
const char *GetKeyboard(void);
const char *GetKeyboard(void) {
	return "";
}

void FCEUD_SetPalette(int index, uint8 r, uint8 g, uint8 b) {
	unsigned index_to_write = index;
#if defined(PS2)
	/* Index correction for PS2 GS */
	int modi = index & 63;
	if ((modi >= 8 && modi < 16) || (modi >= 40 && modi < 48)) {
		index_to_write += 8;
	} else if ((modi >= 16 && modi < 24) || (modi >= 48 && modi < 56)) {
		index_to_write -= 8;
	}
#endif
    retro_palette[index_to_write] = BUILD_PIXEL(r >> RED_EXPAND, g >> GREEN_EXPAND, b >> BLUE_EXPAND);
}

static void default_logger(enum retro_log_level level, const char *fmt, ...) {
}

void FCEUD_PrintError(char *c) {
	log_cb.log(RETRO_LOG_WARN, "%s", c);
}

void FCEUD_PrintDebug(char *c) {
	log_cb.log(RETRO_LOG_DEBUG, "%s", c);
}

void FCEUD_Message(char *s) {
	log_cb.log(RETRO_LOG_INFO, "%s", s);
}

void FCEUD_DispMessage(enum retro_log_level level, unsigned duration, const char *str) {
	if (!environ_cb)
		return;

	if (libretro_msg_interface_version >= 1) {
		struct retro_message_ext msg;
		unsigned priority;

		switch (level) {
		case RETRO_LOG_ERROR:
			priority = 5;
			break;
		case RETRO_LOG_WARN:
			priority = 4;
			break;
		case RETRO_LOG_INFO:
			priority = 3;
			break;
		case RETRO_LOG_DEBUG:
		default:
			priority = 1;
			break;
		}

		msg.msg      = str;
		msg.duration = duration;
		msg.priority = priority;
		msg.level    = level;
		msg.target   = RETRO_MESSAGE_TARGET_OSD;
		msg.type     = RETRO_MESSAGE_TYPE_NOTIFICATION_ALT;
		msg.progress = -1;

		environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE_EXT, &msg);
	} else {
		float fps       = NES_TARGET_FPS;
		unsigned frames = (unsigned)(((float)duration * fps / 1000.0f) + 0.5f);
		struct retro_message msg;

		msg.msg    = str;
		msg.frames = frames;

		environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
	}
}

/*palette for FCEU*/
static bool external_palette_exist = 0;

/* ========================================
 * Palette switching START
 * ======================================== */

/* Period in frames between palette switches
 * when holding RetroPad L2 + Left/Right */
#define PALETTE_SWITCH_PERIOD 30

bool palette_switch_enabled                            = false;
static bool libretro_supports_set_variable             = false;
static unsigned palette_switch_counter                 = 0;
struct retro_core_option_value *palette_opt_values     = NULL;
static const char *palette_labels[PALETTE_TOTAL_COUNT] = { 0 };

static uint32 palette_switch_get_current_index(void) {
	if (current_palette < PALETTE_COUNT)
		return current_palette + 1;

	switch (current_palette) {
	case PALETTE_INTERNAL:
		return 0;
	case PALETTE_RAW:
	case PALETTE_CUSTOM:
		return current_palette - 1;
	default:
		break;
	}

	/* Cannot happen */
	return 0;
}

static void palette_switch_init(void) {
	size_t i;
	struct retro_core_option_v2_definition *opt_defs = option_defs;
	struct retro_core_option_v2_definition *opt_def  = NULL;
#ifndef HAVE_NO_LANGEXTRA
	struct retro_core_option_v2_definition *opt_defs_intl = NULL;
	struct retro_core_option_v2_definition *opt_def_intl  = NULL;
	unsigned language                                     = 0;
#endif

	libretro_supports_set_variable = false;
	if (environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, NULL)) {
		libretro_supports_set_variable = true;
	}

	palette_switch_enabled = libretro_supports_set_variable;
	palette_switch_counter = 0;

#ifndef HAVE_NO_LANGEXTRA
	if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) && (language < RETRO_LANGUAGE_LAST) &&
	    (language != RETRO_LANGUAGE_ENGLISH) && options_intl[language])
		opt_defs_intl = options_intl[language]->definitions;
#endif

	/* Find option corresponding to palettes key */
	for (opt_def = opt_defs; opt_def->key; opt_def++) {
		if (!strcmp(opt_def->key, "fceumm_palette")) {
			break;
		}
	}

	/* Cache option values array for fast access
	 * when setting palette index */
	palette_opt_values = opt_def->values;

	/* Loop over all palette values and fetch
	 * palette labels for notification purposes */
	for (i = 0; i < PALETTE_TOTAL_COUNT; i++) {
		const char *value       = opt_def->values[i].value;
		const char *value_label = NULL;

		/* Check if we have a localised palette label */
#ifndef HAVE_NO_LANGEXTRA
		if (opt_defs_intl) {
			/* Find localised option corresponding to key */
			for (opt_def_intl = opt_defs_intl; opt_def_intl->key; opt_def_intl++) {
				if (!strcmp(opt_def_intl->key, "fceumm_palette")) {
					size_t j = 0;

					/* Search for current option value */
					for (;;) {
						const char *value_intl = opt_def_intl->values[j].value;

						if (!value_intl) {
							break;
						}

						if (!strcmp(value, value_intl)) {
							/* We have a match; fetch localised label */
							value_label = opt_def_intl->values[j].label;
							break;
						}

						j++;
					}

					break;
				}
			}
		}
#endif
		/* If localised palette label is unset,
		 * use label from option_defs_us or fallback
		 * to value itself */
		if (!value_label) {
			value_label = opt_def->values[i].label;
		}
		if (!value_label) {
			value_label = value;
		}

		palette_labels[i] = value_label;
	}
}

static void palette_switch_deinit(void) {
	libretro_supports_set_variable = false;
	palette_switch_enabled         = false;
	palette_switch_counter         = 0;
	palette_opt_values             = NULL;
}

static void palette_switch_set_index(uint32 palette_index) {
	struct retro_variable var = { 0 };

	if (palette_index >= PALETTE_TOTAL_COUNT)
		palette_index = PALETTE_TOTAL_COUNT - 1;

	/* Notify frontend of option value changes */
	var.key   = "fceumm_palette";
	var.value = palette_opt_values[palette_index].value;
	environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var);

	/* Display notification message */
	FCEUD_DispMessage(RETRO_LOG_INFO, 2000, palette_labels[palette_index]);
}

/* ========================================
 * Palette switching END
 * ======================================== */

/* ========================================
 * Stereo Filter START
 * ======================================== */

enum stereo_filter_type { STEREO_FILTER_NULL = 0, STEREO_FILTER_DELAY };
static enum stereo_filter_type current_stereo_filter = STEREO_FILTER_NULL;

#define STEREO_FILTER_DELAY_MS_DEFAULT 15.0f
typedef struct {
	int32 *samples;
	size_t samples_size;
	size_t samples_pos;
	size_t delay_count;
} stereo_filter_delay_t;
static stereo_filter_delay_t stereo_filter_delay;
static float stereo_filter_delay_ms = STEREO_FILTER_DELAY_MS_DEFAULT;

static void stereo_filter_apply_null(int32 *sound_buffer, size_t size) {
	size_t i;
	/* Each element of sound_buffer is a 16 bit mono sample
	 * stored in a 32 bit value. We convert this to stereo
	 * by copying the mono sample to both the high and low
	 * 16 bit regions of the value and casting sound_buffer
	 * to int16 when uploading to the frontend */
	for (i = 0; i < size; i++) {
		sound_buffer[i] = (sound_buffer[i] << 16) | (sound_buffer[i] & 0xFFFF);
	}
}

static void stereo_filter_apply_delay(int32 *sound_buffer, size_t size) {
	size_t delay_capacity = stereo_filter_delay.samples_size - stereo_filter_delay.samples_pos;
	size_t i;

	/* Copy current samples into the delay buffer
	 * (resizing if required) */
	if (delay_capacity < size) {
		int32 *tmp_buffer = NULL;
		size_t tmp_buffer_size;

		tmp_buffer_size = stereo_filter_delay.samples_size + (size - delay_capacity);
		tmp_buffer_size = (tmp_buffer_size << 1) - (tmp_buffer_size >> 1);
		tmp_buffer      = (int32 *)malloc(tmp_buffer_size * sizeof(int32));

		memcpy(tmp_buffer, stereo_filter_delay.samples, stereo_filter_delay.samples_pos * sizeof(int32));

		free(stereo_filter_delay.samples);

		stereo_filter_delay.samples      = tmp_buffer;
		stereo_filter_delay.samples_size = tmp_buffer_size;
	}

	for (i = 0; i < size; i++) {
		stereo_filter_delay.samples[i + stereo_filter_delay.samples_pos] = sound_buffer[i];
	}

	stereo_filter_delay.samples_pos += size;

	/* If we have enough samples in the delay
	 * buffer, mix them into the output */
	if (stereo_filter_delay.samples_pos > stereo_filter_delay.delay_count) {
		size_t delay_index    = 0;
		size_t samples_to_mix = stereo_filter_delay.samples_pos - stereo_filter_delay.delay_count;

		samples_to_mix        = (samples_to_mix > size) ? size : samples_to_mix;

		/* Perform 'null' filtering for any samples for
		 * which a delay buffer entry is unavailable */
		if (size > samples_to_mix) {
			for (i = 0; i < size - samples_to_mix; i++) {
				sound_buffer[i] = (sound_buffer[i] << 16) | (sound_buffer[i] & 0xFFFF);
			}
		}

		/* Each element of sound_buffer is a 16 bit mono sample
		 * stored in a 32 bit value. We convert this to stereo
		 * by copying the mono sample to the high (left channel)
		 * 16 bit region and the delayed sample to the low
		 * (right channel) region, casting sound_buffer
		 * to int16 when uploading to the frontend */
		for (i = size - samples_to_mix; i < size; i++) {
			sound_buffer[i] = (sound_buffer[i] << 16) | (stereo_filter_delay.samples[delay_index++] & 0xFFFF);
		}

		/* Remove the mixed samples from the delay buffer */
		memmove(stereo_filter_delay.samples,
			stereo_filter_delay.samples + samples_to_mix,
		    (stereo_filter_delay.samples_pos - samples_to_mix) * sizeof(int32));
		stereo_filter_delay.samples_pos -= samples_to_mix;
	} else {
		/* Otherwise apply the regular 'null' filter */
		for (i = 0; i < size; i++) {
			sound_buffer[i] = (sound_buffer[i] << 16) | (sound_buffer[i] & 0xFFFF);
		}
	}
}

static void (*stereo_filter_apply)(int32 *sound_buffer, size_t size) = stereo_filter_apply_null;

static void stereo_filter_deinit_delay(void) {
	if (stereo_filter_delay.samples) {
		free(stereo_filter_delay.samples);
	}

	stereo_filter_delay.samples      = NULL;
	stereo_filter_delay.samples_size = 0;
	stereo_filter_delay.samples_pos  = 0;
	stereo_filter_delay.delay_count  = 0;
}

static void stereo_filter_init_delay(void) {
	size_t initial_samples_size;

	/* Convert delay (ms) to number of samples */
	stereo_filter_delay.delay_count = (size_t)((stereo_filter_delay_ms / 1000.0f) * (float)FSettings.SndRate);

	/* Preallocate delay_count + worst case expected
	 * samples per frame to minimise reallocation of
	 * the samples buffer during runtime */
	initial_samples_size = stereo_filter_delay.delay_count + (size_t)((float)FSettings.SndRate / NES_TARGET_FPS) + 1;

	stereo_filter_delay.samples      = (int32 *)malloc(initial_samples_size * sizeof(int32));
	stereo_filter_delay.samples_size = initial_samples_size;
	stereo_filter_delay.samples_pos  = 0;

	/* Assign function pointer */
	stereo_filter_apply = stereo_filter_apply_delay;
}

static void stereo_filter_deinit(void) {
	/* Clean up */
	stereo_filter_deinit_delay();
	/* Assign default function pointer */
	stereo_filter_apply = stereo_filter_apply_null;
}

static void stereo_filter_init(void) {
	stereo_filter_deinit();

	/* Use a case statement to simplify matters
	 * if more filter types are added in the
	 * future... */
	switch (current_stereo_filter) {
	case STEREO_FILTER_DELAY:
		stereo_filter_init_delay();
		break;
	default:
		break;
	}
}

/* ========================================
 * Stereo Filter END
 * ======================================== */

/* ========================================
 * NTSC Video Filter START
 * ======================================== */

#if defined(HAVE_NTSC_FILTER)
enum nes_ntsc_type { NTSC_NONE, NTSC_COMPOSITE, NTSC_SVIDEO, NTSC_RGB, NTSC_MONOCHROME };
static enum nes_ntsc_type use_ntsc_filter = NTSC_NONE;
#endif

#if defined(HAVE_NTSC_FILTER)
#include "nes_ntsc.h"
static nes_ntsc_t *nes_ntsc = NULL;

static void ntsc_filter_deinit(void) {
	if (nes_ntsc) {
		free(nes_ntsc);
		nes_ntsc = NULL;
	}
}

static void ntsc_set_filter(void) {
	nes_ntsc_setup_t ntsc_setup = { 0 };

	ntsc_filter_deinit();

	switch (use_ntsc_filter) {
	case NTSC_COMPOSITE:
		ntsc_setup = nes_ntsc_composite;
		break;
	case NTSC_SVIDEO:
		ntsc_setup = nes_ntsc_svideo;
		break;
	case NTSC_RGB:
		ntsc_setup = nes_ntsc_rgb;
		break;
	case NTSC_MONOCHROME:
		ntsc_setup = nes_ntsc_monochrome;
		break;
	default:
		break;
	}

	if ((GameInfo && (GameInfo->type == GIT_VSUNI)) || (current_palette < PALETTE_INTERNAL) ||
	    (current_palette == PALETTE_CUSTOM)) {
		ntsc_setup.palette = (unsigned char const *)palo;
	}

	nes_ntsc = (nes_ntsc_t *)malloc(sizeof(nes_ntsc_t));
	if (nes_ntsc) {
		nes_ntsc_init(nes_ntsc, &ntsc_setup);
	}

	if (!nes_ntsc) {
		use_ntsc_filter = NTSC_NONE;
	}
}
#endif

/* ========================================
 * NTSC Video Filter END
 * ======================================== */

static void ResetPalette(void);
static void set_user_palette(void);

static void ResetPalette(void) {
	set_user_palette();
#if defined(HAVE_NTSC_FILTER)
	ntsc_set_filter();
#endif
}

/* Core options 'update display' callback */
static bool update_option_visibility(void) {
	struct retro_variable var = { 0 };
	bool updated              = false;

	/* If frontend supports core option categories,
	 * then fceumm_show_adv_system_options and
	 * fceumm_show_adv_sound_options are ignored
	 * and no options should be hidden */
	if (libretro_supports_option_categories) {
		return false;
	}

	var.key   = "fceumm_show_adv_system_options";
	var.value = NULL;

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		bool opt_showAdvSystemOptions_prev = opt_showAdvSystemOptions;

		opt_showAdvSystemOptions = true;
		if (strcmp(var.value, "disabled") == 0)
			opt_showAdvSystemOptions = false;

		if (opt_showAdvSystemOptions != opt_showAdvSystemOptions_prev) {
			struct retro_core_option_display option_display;
			unsigned i;
			unsigned size;
			char options_list[][25] = {
				"fceumm_overclocking",
				"fceumm_ramstate",
				"fceumm_nospritelimit",
				"fceumm_up_down_allowed",
				"fceumm_show_crosshair"
			};

			option_display.visible = opt_showAdvSystemOptions;
			size                   = sizeof(options_list) / sizeof(options_list[0]);
			for (i = 0; i < size; i++) {
				option_display.key = options_list[i];
				environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			}

			updated = true;
		}
	}

	var.key   = "fceumm_show_adv_sound_options";
	var.value = NULL;

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		bool opt_showAdvSoundOptions_prev = opt_showAdvSoundOptions;

		opt_showAdvSoundOptions = true;
		if (!strcmp(var.value, "disabled")) {
			opt_showAdvSoundOptions = false;
		}

		if (opt_showAdvSoundOptions != opt_showAdvSoundOptions_prev) {
			struct retro_core_option_display option_display;
			unsigned i;
			unsigned size;
			char options_list[][25] = {
				"fceumm_sndvolume",
				"fceumm_sndquality",
				"fceumm_sndlowpass",
				"fceumm_sndstereodelay",
				"fceumm_swapduty",
				"fceumm_apu_1",
				"fceumm_apu_2",
				"fceumm_apu_3",
				"fceumm_apu_4",
				"fceumm_apu_5"
			};

			option_display.visible = opt_showAdvSoundOptions;
			size                   = sizeof(options_list) / sizeof(options_list[0]);
			for (i = 0; i < size; i++) {
				option_display.key = options_list[i];
				environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			}

			updated = true;
		}
	}

	return updated;
}

static void set_variables(void) {
	struct retro_core_option_display option_display;
	unsigned index = 0;

	option_display.visible = false;

	/* Initialize main core option struct */
	memset(&option_defs_us, 0, sizeof(option_defs_us));

	/* Write common core options to main struct */
	while (option_defs[index].key) {
		memcpy(&option_defs_us[index], &option_defs[index], sizeof(struct retro_core_option_v2_definition));
		index++;
	}

	/* Append dipswitch settings to core options if available */
	set_dipswitch_variables(&environ_cb, index, option_defs_us);

	libretro_supports_option_categories = false;
	libretro_set_core_options(environ_cb, &libretro_supports_option_categories);

	/* If frontend supports core option categories,
	 * fceumm_show_adv_system_options and
	 * fceumm_show_adv_sound_options are unused
	 * and should be hidden */
	if (libretro_supports_option_categories) {
		option_display.key = "fceumm_show_adv_system_options";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);

		option_display.key = "fceumm_show_adv_sound_options";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
	}
	/* If frontend does not support core option
	 * categories, core options may be shown/hidden
	 * at runtime. In this case, register 'update
	 * display' callback, so frontend can update
	 * core options menu without calling retro_run() */
	else {
		struct retro_core_options_update_display_callback update_display_cb;

		update_display_cb.callback = update_option_visibility;
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_UPDATE_DISPLAY_CALLBACK, &update_display_cb);
	}

	/* VS UNISystem games use internal palette regardless
	 * of user setting, so hide fceumm_palette option */
	if (GameInfo && (GameInfo->type == GIT_VSUNI)) {
		option_display.key = "fceumm_palette";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);

		/* Additionally disable gamepad palette
		 * switching */
		palette_switch_enabled = false;
	}
}

/* Game Genie add-on must be enabled before
 * loading content, so we cannot parse this
 * option inside check_variables() */
static void check_game_genie_variable(void) {
	struct retro_variable var = { 0 };
	int game_genie_enabled    = 0;

	var.key = "fceumm_game_genie";

	/* Game Genie is only enabled for regular
	 * cartridges (excludes arcade content,
	 * FDS games, etc.) */
	if ((GameInfo && (GameInfo->type == GIT_CART)) &&
	    (iNESCart.mapper != 105) && /* Nintendo World Championship cart (Mapper 105)*/
	    environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
	    var.value && !strcmp(var.value, "enabled")) {
		game_genie_enabled = 1;
	}

	FCEUI_SetGameGenie(game_genie_enabled);
}

/* Callback passed to FCEUI_LoadGame()
 * > Required since we must set and check
 *   core options immediately after ROM
 *   is loaded, before FCEUI_LoadGame()
 *   returns */
static void frontend_post_load_init(void) {
	set_variables();
	check_game_genie_variable();
}

static double get_aspect_ratio(void) {
	if (aspect_ratio_par == 2) {
		return (double)NES_4_3;
	} else if (aspect_ratio_par == 3) {
		return (double)NES_PP;
	} else {
		return (double)NES_8_7_PAR;
	}
}

static void set_user_palette(void) {
	unsigned i;

	palette_game_available = false;
	palette_user_available = false;
	use_raw_palette        = false;

	/* VS UNISystem uses internal palette presets regardless of options */
	if (GameInfo && (GameInfo->type == GIT_VSUNI)) {
		FCEU_ResetPalette();
	}
	/* Reset and choose between default internal or external custom palette */
	/* if palette_game_available is set to 1, external palette
	 * is loaded, else it will load default NES palette.
	 * FCEUI_SetPaletteUser() both resets the palette array to
	 * internal default palette and then chooses which one to use. */
	else if (current_palette == PALETTE_INTERNAL) {
		FCEUI_SetPaletteUser(NULL, 0);
	} else if ((current_palette == PALETTE_CUSTOM) && external_palette_exist) {
		palette_game_available = true;
		FCEUI_SetPaletteUser(NULL, 0);
	}

	/* setup raw palette */
	else if (current_palette == PALETTE_RAW) {
		pal color;
		use_raw_palette = true;
		for (i = 0; i < 64; i++) {
			color.r = (((i >> 0) & 0xF) * 255) / 15;
			color.g = (((i >> 4) & 0x3) * 255) / 3;
			color.b = 0;
			FCEUD_SetPalette(i, color.r, color.g, color.b);
		}
	}

	/* setup palette presets */
	else {
		FCEUI_SetPaletteUser(palettes[current_palette].data, 64);
	}
}

/* Set variables for NTSC(1) / PAL(2) / Dendy(3)
 * Dendy has PAL framerate and resolution, but ~NTSC timings,
 * and has 50 dummy scanlines to force 50 fps.
 */
static void set_system_region(unsigned region) {
	bool nespal = false;
	bool dendy  = false;

	switch (region) {
	case 0: /* AUTO */
		if (iNESCart.region == DENDY) {
			dendy = true;
		} else {
			nespal = iNESCart.region == NES_PAL;
		}
		break;
	case 1: /* NTSC */
		FCEUD_DispMessage(RETRO_LOG_INFO, 2000, "System: NTSC");
		break;
	case 2: /* PAL */
		nespal = true;
		FCEUD_DispMessage(RETRO_LOG_INFO, 2000, "System: PAL");
		break;
	case 3: /* Dendy */
		dendy = true;
		FCEUD_DispMessage(RETRO_LOG_INFO, 2000, "System: Dendy");
		break;
	}

	isDendy = dendy;
	FCEUI_SetVidSystem(nespal);
	ResetPalette();
}

#define VOLUME_MAX 256

static void check_variables_volume_levels(void) {
	struct {
		int channel;
		char name[25];
	} apu_channels[] = {
		{ SND_SQUARE1, "fceumm_apu_square_1" },
		{ SND_SQUARE2, "fceumm_apu_square_2" },
		{ SND_TRIANGLE, "fceumm_apu_triangle" },
		{ SND_NOISE, "fceumm_apu_noise" },
		{ SND_DMC, "fceumm_apu_dpcm" },
		{ SND_FDS, "fceumm_apu_fds" },
		{ SND_S5B, "fceumm_apu_s5b" },
		{ SND_N163, "fceumm_apu_n163" },
		{ SND_VRC6, "fceumm_apu_vrc6" },
		{ SND_VRC7, "fceumm_apu_vrc7" },
		{ SND_MMC5, "fceumm_apu_mmc5" },
	};
	struct retro_variable var = { 0 };
	int i = 0;
	int ssize = sizeof(apu_channels) / sizeof(apu_channels[0]);

	for (i = 0; i < ssize; i++) {
		int channel = apu_channels[i].channel;
		var.key     = apu_channels[i].name;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
			int newval = VOLUME_MAX * atoi(var.value) / 100;
			if (FCEUI_GetSoundVolume(channel) != newval) {
				FCEUI_SetSoundVolume(channel, newval);
			}
		}
	}
}

static void check_variables(bool startup) {
	struct retro_variable var  = { 0 };
	bool stereo_filter_updated = false;
	int nes_sprites = 1, nes_background = 1;

	/* 1 = Performs only geometry update: e.g. overscans */
	/* 2 = Performs video/geometry update when needed and timing changes: e.g. region and filter change */
	int audio_video_updated = 0;

	var.key = "fceumm_sound_rate";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		int value = atoi(var.value);
		if (value != FSettings.SndRate) {
			FCEUI_Sound(value);
			audio_video_updated |= 2;
		}
	}

	var.key = "fceumm_ramstate";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		if (!strcmp(var.value, "random")) {
			FSettings.RamInitState = 2;
		} else if (!strcmp(var.value, "fill $00")) {
			FSettings.RamInitState = 1;
		} else {
			FSettings.RamInitState = 0;
		}
	}

#if defined(HAVE_NTSC_FILTER)
	var.key = "fceumm_ntsc_filter";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && GameInfo && GameInfo->type != GIT_NSF) {
		unsigned orig_value = use_ntsc_filter;
		if (!strcmp(var.value, "disabled")) {
			use_ntsc_filter = NTSC_NONE;
		} else if (!strcmp(var.value, "composite")) {
			use_ntsc_filter = NTSC_COMPOSITE;
		} else if (!strcmp(var.value, "svideo")) {
			use_ntsc_filter = NTSC_SVIDEO;
		} else if (!strcmp(var.value, "rgb")) {
			use_ntsc_filter = NTSC_RGB;
		} else if (!strcmp(var.value, "monochrome")) {
			use_ntsc_filter = NTSC_MONOCHROME;
		}
		if (use_ntsc_filter != orig_value) {
			ResetPalette();
			audio_video_updated = 2;
		}
	}
#endif /* HAVE_NTSC_FILTER */

	FCEUI_GetRenderPlanes(&nes_sprites, &nes_background);

	var.key = "fceumm_hide_sprites";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		nes_sprites = strcmp(var.value, "enabled");
	}

	var.key = "fceumm_hide_background";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		nes_background = strcmp(var.value, "enabled");
	}

	FCEUI_SetRenderPlanes(nes_sprites, nes_background);

	var.key = "fceumm_palette";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		unsigned orig_value = current_palette;

		if (!strcmp(var.value, "default"))
			current_palette = PALETTE_INTERNAL;
		else if (!strcmp(var.value, "raw"))
			current_palette = PALETTE_RAW;
		else if (!strcmp(var.value, "custom"))
			current_palette = PALETTE_CUSTOM;
		else if (!strcmp(var.value, PAL_ASQREALC))
			current_palette = 0;
		else if (!strcmp(var.value, PAL_VIRTUALCONSOLE))
			current_palette = 1;
		else if (!strcmp(var.value, PAL_NESRGB))
			current_palette = 2;
		else if (!strcmp(var.value, PAL_CXA2025AS))
			current_palette = 3;
		else if (!strcmp(var.value, PAL_NESPAL))
			current_palette = 4;
		else if (!strcmp(var.value, PAL_BMF_FINAL2))
			current_palette = 5;
		else if (!strcmp(var.value, PAL_BMF_FINAL3))
			current_palette = 6;
		else if (!strcmp(var.value, PAL_NESCAP))
			current_palette = 7;
		else if (!strcmp(var.value, PAL_WAVEBEAM))
			current_palette = 8;
		else if (!strcmp(var.value, PAL_FBX_DIGITAL_PRIME))
			current_palette = 9;
		else if (!strcmp(var.value, PAL_FBX_MAGNUM))
			current_palette = 10;
		else if (!strcmp(var.value, PAL_FBX_SMOOTH_V2))
			current_palette = 11;
		else if (!strcmp(var.value, PAL_FBX_NES_CLASSIC))
			current_palette = 12;
		else if (!strcmp(var.value, PAL_ROYAL_TEA))
			current_palette = 13;

		if (current_palette != orig_value) {
			audio_video_updated = 1;
			ResetPalette();
		}
	}

	var.key = "fceumm_up_down_allowed";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		bool value = !strcmp(var.value, "enabled") ? true : false;
		input_allow_updown_leftright(value);
	}

	var.key = "fceumm_nospritelimit";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		int no_sprite_limit = (!strcmp(var.value, "enabled")) ? 1 : 0;
		FCEUI_DisableSpriteLimitation(no_sprite_limit);
	}

	var.key = "fceumm_overclocking";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {

		if (!strcmp(var.value, "disabled")) {
			FSettings.PPUOverclockEnabled      = 0;
			FSettings.SkipDMC7BitOverclock     = 1;
			ppu.overclock.postrender_scanlines = 0;
			ppu.overclock.vblank_scanlines     = 0;
		} else if (!strcmp(var.value, "2x-Postrender")) {
			FSettings.PPUOverclockEnabled      = 1;
			FSettings.SkipDMC7BitOverclock     = 1;
			ppu.overclock.postrender_scanlines = 266;
			ppu.overclock.vblank_scanlines     = 0;
		} else if (!strcmp(var.value, "2x-VBlank")) {
			FSettings.PPUOverclockEnabled      = 1;
			FSettings.SkipDMC7BitOverclock     = 1;
			ppu.overclock.postrender_scanlines = 0;
			ppu.overclock.vblank_scanlines     = 266;
		}

		ppu.normal_scanlines = isDendy ? 290 : 240;
		ppu.totalscanlines = ppu.normal_scanlines + (FSettings.PPUOverclockEnabled ? ppu.overclock.postrender_scanlines : 0);
	}

	var.key = "fceumm_zapper_mode";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		FCEU_ZapperSetSTMode(false);
		if (!strcmp(var.value, "mouse")) {
			input_set_zapper_mode(RetroMouse);
		} else if (!strcmp(var.value, "touchscreen")) {
			input_set_zapper_mode(RetroPointer);
		} else if (!strcmp(var.value, "stlightgun")) {
			input_set_zapper_mode(RetroSTLightgun);
			FCEU_ZapperSetSTMode(true);
		} else {
			input_set_zapper_mode(RetroLightgun); /*default setting*/
		}
	}

	var.key = "fceumm_zapper_tolerance";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		FCEU_ZapperSetTolerance(atoi(var.value));
	}

	var.key = "fceumm_zapper_trigger";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		if (!strcmp(var.value, "enabled")) {
			FCEU_ZapperInvertTrigger(true);
		} else if (!strcmp(var.value, "disabled")) {
			FCEU_ZapperInvertTrigger(false);
		}
	}

	var.key = "fceumm_zapper_sensor";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		if (!strcmp(var.value, "enabled")) {
			FCEU_ZapperInvertSensor(true);
		} else if (!strcmp(var.value, "disabled")) {
			FCEU_ZapperInvertSensor(false);
		}
	}

	var.key = "fceumm_arkanoid_mode";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		if (!strcmp(var.value, "touchscreen")) {
			input_set_arkanoid_mode(RetroArkanoidPointer);
		} else if (!strcmp(var.value, "abs_mouse")) {
			input_set_arkanoid_mode(RetroArkanoidAbsMouse);
		} else if (!strcmp(var.value, "stelladaptor")) {
			input_set_arkanoid_mode(RetroArkanoidStelladaptor);
		} else {
			input_set_arkanoid_mode(RetroArkanoidMouse); /*default setting*/
		}
	}

	var.key = "fceumm_mouse_sensitivity";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		double value = atof(var.value);
		input_set_mousesensitivity(value);
	}

	var.key = "fceumm_show_crosshair";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		if (!strcmp(var.value, "enabled")) {
			FSettings.ShowCrosshair = 1;
		} else if (!strcmp(var.value, "disabled")) {
			FSettings.ShowCrosshair = 0;
		}
	}

#if defined(PSP)
	var.key = "fceumm_overscan";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		bool newval = (!strcmp(var.value, "enabled"));
		if (newval != crop_overscan) {
			overscan_left   = (newval == true ? 8 : 0);
			overscan_right  = (newval == true ? 8 : 0);
			overscan_top    = (newval == true ? 8 : 0);
			overscan_bottom = (newval == true ? 8 : 0);

			crop_overscan       = newval;
			audio_video_updated = 1;
		}
	}
#else
	var.key = "fceumm_overscan_left";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		unsigned newval = atoi(var.value);
		if (newval != overscan_left) {
			overscan_left       = newval;
			audio_video_updated = 1;
		}
	}

	var.key = "fceumm_overscan_right";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		unsigned newval = atoi(var.value);
		if (newval != overscan_right) {
			overscan_right      = newval;
			audio_video_updated = 1;
		}
	}

	var.key = "fceumm_overscan_top";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		unsigned newval = atoi(var.value);
		if (newval != overscan_top) {
			overscan_top        = newval;
			audio_video_updated = 1;
		}
	}

	var.key = "fceumm_overscan_bottom";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		unsigned newval = atoi(var.value);
		if (newval != overscan_bottom) {
			overscan_bottom     = newval;
			audio_video_updated = 1;
		}
	}
#endif

	var.key = "fceumm_aspect";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		unsigned oldval = aspect_ratio_par;
		if (!strcmp(var.value, "8:7 PAR")) {
			aspect_ratio_par = 1;
		} else if (!strcmp(var.value, "4:3")) {
			aspect_ratio_par = 2;
		} else if (!strcmp(var.value, "PP")) {
			aspect_ratio_par = 3;
		}
		if (aspect_ratio_par != oldval) {
			audio_video_updated = 1;
		}
	}

	var.key = "fceumm_turbo_enable";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		if (!strcmp(var.value, "Player 1")) {
			input_enable_turbo_buttons(0, true);
		} else if (!strcmp(var.value, "Player 2")) {
			input_enable_turbo_buttons(1, true);
		} else if (!strcmp(var.value, "Players 1 and 2")) {
			input_enable_turbo_buttons(0, true);
			input_enable_turbo_buttons(1, true);
		} else {
			input_enable_turbo_buttons(0, false);
			input_enable_turbo_buttons(1, false);
		}
	}

	var.key = "fceumm_turbo_delay";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		int value = atoi(var.value);
		input_set_turbo_delay(value);
	}

	var.key = "fceumm_region";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		uint8 oldval = opt_region;
		if (!strcmp(var.value, "Auto")) {
			opt_region = 0;
		} else if (!strcmp(var.value, "NTSC")) {
			opt_region = 1;
		} else if (!strcmp(var.value, "PAL")) {
			opt_region = 2;
		} else if (!strcmp(var.value, "Dendy")) {
			opt_region = 3;
		}
		if (opt_region != oldval) {
			set_system_region(opt_region);
			audio_video_updated = 2;
		}
	}

	var.key = "fceumm_sndquality";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		uint8 oldval = sndquality;
		if (!strcmp(var.value, "Low")) {
			sndquality = 0;
		} else if (!strcmp(var.value, "High")) {
			sndquality = 1;
		} else if (!strcmp(var.value, "Very High")) {
			sndquality = 2;
		}
		if (sndquality != oldval) {
			FCEUI_SetSoundQuality(sndquality);
		}
	}

	var.key = "fceumm_sndlowpass";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		int lowpass = (!strcmp(var.value, "disabled")) ? 0 : atoi(var.value);
		FCEUI_SetLowPass(lowpass);
	}

	var.key = "fceumm_reducedmcpopping";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		bool newval = (!strcmp(var.value, "enabled"));
		FCEUI_ReduceDmcPopping(newval);
	}

	var.key = "fceumm_sndstereodelay";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		enum stereo_filter_type filter_type = STEREO_FILTER_NULL;
		float filter_delay_ms               = STEREO_FILTER_DELAY_MS_DEFAULT;

		if (strcmp(var.value, "disabled") && (strlen(var.value) > 1)) {
			filter_type     = STEREO_FILTER_DELAY;
			filter_delay_ms = (float)atoi(var.value);

			filter_delay_ms = (filter_delay_ms < 1.0f) ? 1.0f : filter_delay_ms;
			filter_delay_ms = (filter_delay_ms > 32.0f) ? 32.0f : filter_delay_ms;
		}

		if ((filter_type != current_stereo_filter) ||
		    ((filter_type == STEREO_FILTER_DELAY) && (filter_delay_ms != stereo_filter_delay_ms))) {
			current_stereo_filter  = filter_type;
			stereo_filter_delay_ms = filter_delay_ms;
			stereo_filter_updated  = true;
		}
	}

	var.key = "fceumm_sndvolume";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		int val = (int)((float)VOLUME_MAX * atof(var.value) / 200.0f);
		FCEUI_SetSoundVolume(SND_MASTER, val);
	}

	var.key = "fceumm_swapduty";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
		bool newval = (!strcmp(var.value, "enabled"));
		FSettings.SwapDutyCycles = newval;
	}

	if ((stereo_filter_updated || (audio_video_updated == 2)) && !startup) {
		stereo_filter_init();
	}

	if (audio_video_updated && !startup) {
		struct retro_system_av_info av_info;

		retro_get_system_av_info(&av_info);
		if (audio_video_updated == 2) {
			environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &av_info);
		} else {
			environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &av_info);
		}
	}

	check_variables_volume_levels();
	check_dipswitch_variables();
	update_option_visibility();
}

void input_palette_switch(bool palette_next, bool palette_prev) {
	if (palette_prev || palette_next) {
		if (palette_switch_counter == 0) {
			unsigned new_palette_index = palette_switch_get_current_index();

			if (palette_prev) {
				if (new_palette_index > 0) {
					new_palette_index--;
				} else {
					new_palette_index = PALETTE_TOTAL_COUNT - 1;
				}
			} else { /* palette_next */
				if (new_palette_index < PALETTE_TOTAL_COUNT - 1) {
					new_palette_index++;
				} else {
					new_palette_index = 0;
				}
			}

			palette_switch_set_index(new_palette_index);
		}

		palette_switch_counter++;
		if (palette_switch_counter >= PALETTE_SWITCH_PERIOD) {
			palette_switch_counter = 0;
		}
	} else {
		palette_switch_counter = 0;
	}
}

#if defined(PSP)
static void retro_run_blit_psp(uint8 *gfx) {
	static unsigned int __attribute__((aligned(16))) d_list[32];
	void *texture_vram_p = NULL;

	unsigned width  = 256;
	unsigned height = 240;

	int x, y;

	if (crop_overscan) {
		width -= 16;
		height -= 16;
	}
	texture_vram_p = (void *)(0x44200000 - (256 * 256)); /* max VRAM address - frame size */

	sceKernelDcacheWritebackRange(retro_palette, 256 * 2);
	sceKernelDcacheWritebackRange(gfx, 256 * 240);

	sceGuStart(GU_DIRECT, d_list);

	/* sceGuCopyImage doesnt seem to work correctly with GU_PSM_T8
	 * so we use GU_PSM_4444 ( 2 Bytes per pixel ) instead
	 * with half the values for pitch / width / x offset
	 */
	if (crop_overscan)
		sceGuCopyImage(GU_PSM_4444, 4, 4, 120, 224, 128, gfx, 0, 0, 128, texture_vram_p);
	else
		sceGuCopyImage(GU_PSM_4444, 0, 0, 128, 240, 128, gfx, 0, 0, 128, texture_vram_p);

	sceGuTexSync();
	sceGuTexImage(0, 256, 256, 256, texture_vram_p);
	sceGuTexMode(GU_PSM_T8, 0, 0, GU_FALSE);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
	sceGuDisable(GU_BLEND);
	sceGuClutMode(GU_PSM_5650, 0, 0xFF, 0);
	sceGuClutLoad(32, retro_palette);

	sceGuFinish();

	video_cb(texture_vram_p, width, height, 256);
}
#elif defined(PS2)

static void retro_run_blit_ps2(uint8 *gfx) {
	unsigned width  = 256;
	unsigned height = 240;
	unsigned pitch  = 512;

	uint32 *buf = (uint32 *)RETRO_HW_FRAME_BUFFER_VALID;

	if (!ps2) {
		if (!environ_cb(RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE, (void **)&ps2) || !ps2) {
			FCEU_printf(" Failed to get HW rendering interface!\n");
			return;
		}

		if (ps2->interface_version != RETRO_HW_RENDER_INTERFACE_GSKIT_PS2_VERSION) {
			FCEU_printf(" HW render interface mismatch, expected %u, got %u!\n",
			    RETRO_HW_RENDER_INTERFACE_GSKIT_PS2_VERSION, ps2->interface_version);
			return;
		}

		ps2->coreTexture->Width   = width;
		ps2->coreTexture->Height  = height;
		ps2->coreTexture->PSM     = GS_PSM_T8;
		ps2->coreTexture->ClutPSM = GS_PSM_CT16;
		ps2->coreTexture->Filter  = GS_FILTER_LINEAR;
		ps2->padding = (struct retro_hw_ps2_insets) { (float)overscan_top, (float)overscan_left, (float)overscan_bottom,
			(float)overscan_right };
	}

	ps2->coreTexture->Clut = (u32 *)retro_palette;
	ps2->coreTexture->Mem  = (u32 *)gfx;

	video_cb(buf, width, height, pitch);
}
#else
#if defined(HAVE_NTSC_FILTER)
static void retro_run_blit_ntsc(uint8 *gfx, uint8 *emp) {
	static unsigned burst_count = 0;
	static unsigned burst_phase = 0;
	double div = 1.5f;

	nes_ntsc_blit(nes_ntsc,
		(NES_NTSC_IN_T const *)gfx,
		(NES_NTSC_IN_T const *)emp,
		NES_WIDTH, burst_phase,
		NES_WIDTH, NES_HEIGHT, fceu_video_out, NTSC_WIDTH * sizeof(Bpp_t));

	burst_phase = (unsigned)((double)burst_count / div);
	burst_count = (burst_count + 1) % 3;

	video_cb(fceu_video_out + ((overscan_left * 9) / 4) + (overscan_top * NTSC_WIDTH),
		NTSC_WIDTH - (((overscan_left + overscan_right) * 9) / 4),
		NES_HEIGHT - overscan_top - overscan_bottom,
		NTSC_WIDTH * sizeof(Bpp_t));
}
#endif /* HAVE_NTSC_FILTER */

static INLINE int get_pixel_color(const uint8 *in, const uint8 *inD) {
	uint8 pixel = *in, deemp = *inD;
	int color = retro_palette[pixel];

	if (use_raw_palette) {
		return retro_palette[(pixel & 0x3F) | (deemp * 4)];
	} else if (deemp) {
		return retro_palette[256 + (pixel & 0x3F) + (deemp * 64)];
	}
	return color;
}

static void retro_run_blit(uint8 *gfx, uint8 *emp) {
	int width  = NES_WIDTH - overscan_left - overscan_right;
	int height = NES_HEIGHT - overscan_top - overscan_bottom;
	int x = 0, y = 0;

	Bpp_t *out_scanline      = fceu_video_out;
	const uint8 *in_scanline = gfx + overscan_left + overscan_top * NES_WIDTH;
	const uint8 *in_emp      = emp + overscan_left + overscan_top * NES_WIDTH;

	for (y = height; --y >= 0; out_scanline += width, in_scanline += NES_WIDTH, in_emp += NES_WIDTH) {
		for (x = 0; x < width; x++) {
			out_scanline[x] = get_pixel_color(&in_scanline[x], &in_emp[x]);
		}
	}

	video_cb(fceu_video_out, width, height, width * sizeof(Bpp_t));
}
#endif

static bool checkGG(char c) {
	static const char lets[16] = { 'A', 'P', 'Z', 'L', 'G', 'I', 'T', 'Y', 'E', 'O', 'X', 'U', 'K', 'S', 'V', 'N' };
	int x;

	for (x = 0; x < 16; x++) {
		if (lets[x] == toupper(c)) {
			return true;
		}
	}
	return false;
}

static bool GGisvalid(const char *code) {
	size_t len = strlen(code);
	uint32 i;

	if (len != 6 && len != 8) {
		return false;
	}

	for (i = 0; i < len; i++) {
		if (!checkGG(code[i])) {
			return false;
		}
	}
	return true;
}

/* Start of Libretro API */

void retro_set_environment(retro_environment_t cb) {
	struct retro_vfs_interface_info vfs_iface_info;
	static const struct retro_system_content_info_override content_overrides[] = {
		{
		    "fds|nes|unf|unif", /* extensions */
		    false,              /* need_fullpath */
		    false               /* persistent_data */
		},
		{ NULL, false, false }
	};

	environ_cb = cb;

	vfs_iface_info.required_interface_version = 1;
	vfs_iface_info.iface                      = NULL;

	if (environ_cb(RETRO_ENVIRONMENT_GET_VFS_INTERFACE, &vfs_iface_info)) {
		filestream_vfs_init(&vfs_iface_info);
	}

	environ_cb(RETRO_ENVIRONMENT_SET_CONTENT_INFO_OVERRIDE, (void *)content_overrides);
}

void retro_set_video_refresh(retro_video_refresh_t cb) {
	video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb) { }

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {
	audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb) {
	poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb) {
	input_cb = cb;
}

void retro_init(void) {
	bool achievements = true;

	log_cb.log = default_logger;
	environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log_cb);

	environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS, &achievements);

	if (environ_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL)) {
		libretro_supports_bitmasks = true;
	}

	environ_cb(RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION, &libretro_msg_interface_version);

	palette_switch_init();
}

void retro_deinit(void) {
	FCEUI_CloseGame();
	FCEUI_Sound(0);
	FCEUI_Kill();
#if defined(_3DS)
	linearFree(fceu_video_out);
#elif !defined(PS2) && !defined(PSP)
	if (fceu_video_out) {
		FCEU_afree(fceu_video_out);
	}
	fceu_video_out = NULL;
#endif
#if defined(PS2)
	ps2 = NULL;
#endif
	libretro_supports_bitmasks     = false;
	libretro_msg_interface_version = 0;
	dipswitch_close();
#if defined(HAVE_NTSC_FILTER)
	ntsc_filter_deinit();
#endif
	palette_switch_deinit();
	stereo_filter_deinit();
}

unsigned retro_api_version(void) {
	return RETRO_API_VERSION;
}

void retro_get_system_info(struct retro_system_info *info) {
	info->need_fullpath    = true;
	info->valid_extensions = "fds|nes|unf|unif";
#ifdef GIT_VERSION
	info->library_version = "(SVN)" GIT_VERSION;
#else
	info->library_version = "(SVN)";
#endif
	info->library_name  = "FCEUmm";
	info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info) {
	info->geometry.base_width = NES_WIDTH - overscan_left - overscan_right;
	info->geometry.base_height = NES_HEIGHT - overscan_top - overscan_bottom;
#if defined(HAVE_NTSC_FILTER)
	info->geometry.max_width = NTSC_WIDTH;
#else
	info->geometry.max_width = NES_WIDTH;
#endif
	info->geometry.max_height = NES_HEIGHT;
	info->geometry.aspect_ratio = get_aspect_ratio();
	info->timing.fps = NES_TARGET_FPS;
	info->timing.sample_rate = (double)FSettings.SndRate;
}

void retro_set_controller_port_device(unsigned port, unsigned device) {
	if (port < 5) {
		input_set_controller_port_device(port, device);
	}
}

void retro_reset(void) {
	FCEUI_ResetNES();
}

void retro_run(void) {
	uint8 *gfx;
	uint8 *emp;
	int32 *sound;
	int32 ssize;
	bool updated = false;

	poll_cb();

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated) {
		check_variables(false);
	}

	input_update(&input_cb);
	FCEUI_Emulate(&gfx, &emp, &sound, &ssize, 0);

#if defined(PSP)
	retro_run_blit_psp(gfx);
#elif defined(PS2)
	retro_run_blit_ps2(gfx);
#else
#if defined(HAVE_NTSC_FILTER)
	if (use_ntsc_filter != NTSC_NONE) {
		retro_run_blit_ntsc(gfx, emp);
	} else
#endif /* HAVE_NTSC_FILTER */
	{
		retro_run_blit(gfx, emp);
	}
#endif

	stereo_filter_apply(sound, ssize);
	audio_batch_cb((const int16 *)sound, ssize);
}

size_t retro_serialize_size(void) {
	if (serialize_size == 0) {
		/* Something arbitrarily big.*/
		uint8 *buffer = (uint8 *)malloc(1000000);
		memstream_set_buffer(buffer, 1000000);

		FCEUSS_Save_Mem();
		serialize_size = memstream_get_last_size();
		free(buffer);
	}

	return serialize_size;
}

bool retro_serialize(void *data, size_t size) {
	/* Cannot save state while Game Genie
	 * screen is open */
	if (geniestage == 1) {
		return false;
	}

	if (size != retro_serialize_size()) {
		return false;
	}

	memstream_set_buffer((uint8 *)data, size);
	FCEUSS_Save_Mem();
	return true;
}

bool retro_unserialize(const void *data, size_t size) {
	/* Cannot load state while Game Genie
	 * screen is open */
	if (geniestage == 1) {
		return false;
	}

	if (size != retro_serialize_size()) {
		return false;
	}

	memstream_set_buffer((uint8 *)data, size);
	FCEUSS_Load_Mem();
	return true;
}

void retro_cheat_reset(void) {
	FCEU_ResetCheats();
}

void retro_cheat_set(unsigned index, bool enabled, const char *code) {
	char name[256];
	char temp[256];
	char *codepart;
	uint16 a;
	uint8 v;
	int c;
	int type = 1;

	if (code == NULL) {
		return;
	}

	sprintf(name, "N/A");
	strcpy(temp, code);
	codepart = strtok(temp, "+,;._ ");

	while (codepart) {
		if ((strlen(codepart) == 7) && (codepart[4] == ':')) {
			/* raw code in xxxx:xx format */
			log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (Raw)\n", codepart);
			codepart[4] = '\0';
			a           = strtoul(codepart, NULL, 16);
			v           = strtoul(codepart + 5, NULL, 16);
			c           = -1;
			/* Zero-page addressing modes don't go through the normal read/write handlers in FCEU, so
			 * we must do the old hacky method of RAM cheats. */
			if (a < 0x0100) {
				type = 0;
			}
			FCEUI_AddCheat(name, a, v, c, type);
		} else if ((strlen(codepart) == 10) && (codepart[4] == '?') && (codepart[7] == ':')) {
			/* raw code in xxxx?xx:xx */
			log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (Raw)\n", codepart);
			codepart[4] = '\0';
			codepart[7] = '\0';
			a           = strtoul(codepart, NULL, 16);
			v           = strtoul(codepart + 8, NULL, 16);
			c           = strtoul(codepart + 5, NULL, 16);
			/* Zero-page addressing modes don't go through the normal read/write handlers in FCEU, so
			 * we must do the old hacky method of RAM cheats. */
			if (a < 0x0100) {
				type = 0;
			}
			FCEUI_AddCheat(name, a, v, c, type);
		} else if (GGisvalid(codepart) && FCEUI_DecodeGG(codepart, &a, &v, &c)) {
			FCEUI_AddCheat(name, a, v, c, type);
			log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (GG)\n", codepart);
		} else if (FCEUI_DecodePAR(codepart, &a, &v, &c, &type)) {
			FCEUI_AddCheat(name, a, v, c, type);
			log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (PAR)\n", codepart);
		} else {
			log_cb.log(RETRO_LOG_DEBUG, "Invalid or unknown code: '%s'\n", codepart);
		}
		codepart = strtok(NULL, "+,;._ ");
	}
}

static void check_system_specs(void) {
	/* TODO - when we get it running at fullspeed on PSP, set to 4 */
	unsigned level = 5;
	environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
}

static void init_blit_buffer(void) {
#if defined(_3DS)
	fceu_video_out = (uint16 *)linearMemAlign(256 * 240 * sizeof(uint16), 128);
#elif !defined(PS2) && !defined(PSP) /* PS2 targets uses hw buffers for video */
#if defined(HAVE_NTSC_FILTER)
#define FB_WIDTH  NTSC_WIDTH
#define FB_HEIGHT NES_HEIGHT
#else /* !HAVE_NTSC_FILTER */
#define FB_WIDTH  NES_WIDTH
#define FB_HEIGHT NES_HEIGHT
#endif
	fceu_video_out = (Bpp_t *)FCEU_amalloc(FB_WIDTH * FB_HEIGHT * sizeof(Bpp_t));
#endif /* !PS2 */
}

static void set_memory_maps(void) {
	size_t desc_base = 64;
	struct retro_memory_descriptor descs[64 + 4] = { 0 };
	struct retro_memory_map mmaps = { 0 };
	int i, j;

	for (i = 0, j = 0; j < (int)desc_base; j++) {
		if (MMapPtrs[j] != NULL) {
			struct retro_memory_descriptor *tmpdesc = &descs[i++];
			tmpdesc->ptr = MMapPtrs[j];
			tmpdesc->start = j * 1024;
			tmpdesc->len = 1024;
			tmpdesc->select = 0;
			if (tmpdesc->start < 0x2000) {
				tmpdesc->addrspace = "RAM";
			} else if (tmpdesc->start < 0x4000) {
				tmpdesc->addrspace = "PPU";
			} else if (tmpdesc->start < 0x4020) {
				tmpdesc->addrspace = "APU/IO";
			} else {
				tmpdesc->addrspace = "WRAM/CART";
			}
		}
	}
	/* This doesn't map in 2004--2007 but those aren't really
	 * worthwhile to read from on a vblank anyway
	 */
	descs[i].flags = 0;
	descs[i].ptr = PPU;
	descs[i].offset = 0;
	descs[i].start = 0x2000;
	descs[i].select = 0;
	descs[i].disconnect = 0;
	descs[i].len = 4;
	descs[i].addrspace = "PPUREG";
	i++;
	/* In the future, it would be good to map pattern tables 1 and 2,
	 * but these must be remapped often
	 */
	/* descs[i] = (struct retro_memory_descriptor){0, ????, 0, 0x0000 | PPU_BIT,
	 * PPU_BIT, PPU_BIT, 0x1000, "PAT0"}; */
	/* i++; */
	/* descs[i] = (struct retro_memory_descriptor){0, ????, 0, 0x1000 | PPU_BIT,
	 * PPU_BIT, PPU_BIT, 0x1000, "PAT1"}; */
	/* i++; */
	/* Likewise it would be better to use "vnapage" for this but
	 * RetroArch API is inconvenient for handles like that, so we'll
	 * just blithely assume the client will handle mapping and that
	 * we'll ignore those carts that have extra NTARAM.
	 */
	descs[i].flags = 0;
	descs[i].ptr = NTARAM;
	descs[i].offset = 0;
	descs[i].start = PPU_BIT | 0x2000;
	descs[i].select = PPU_BIT;
	descs[i].disconnect = PPU_BIT;
	descs[i].len = 0x1000;
	descs[i].addrspace = "NTARAM";
	i++;
	descs[i].flags = 0;
	descs[i].ptr = PALRAM;
	descs[i].offset = 0;
	descs[i].start = PPU_BIT | 0x3000;
	descs[i].select = PPU_BIT;
	descs[i].disconnect = PPU_BIT;
	descs[i].len = 0x020;
	descs[i].addrspace = "PALRAM";
	i++;
	/* OAM doesn't really live anywhere in address space so I'll put it at
	 * 0x4000. */
	descs[i].flags = 0;
	descs[i].ptr = SPRAM;
	descs[i].offset = 0;
	descs[i].start = PPU_BIT | 0x4000;
	descs[i].select = PPU_BIT;
	descs[i].disconnect = PPU_BIT;
	descs[i].len = 0x100;
	descs[i].addrspace = "OAM";
	i++;
	mmaps.descriptors = descs;
	mmaps.num_descriptors = i;

	environ_cb(RETRO_ENVIRONMENT_SET_MEMORY_MAPS, &mmaps);
}

bool retro_load_game(const struct retro_game_info *info) {
	const char *system_dir = NULL;
	enum retro_pixel_format rgb565;

	struct retro_game_info_ext *info_ext = NULL;
	const uint8 *content_data          = NULL;
	size_t content_size                  = 0;
	char content_path[2048]              = { 0 };

#if defined(FRONTEND_SUPPORTS_RGB565)
	rgb565 = RETRO_PIXEL_FORMAT_RGB565;
	if (environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &rgb565))
		log_cb.log(RETRO_LOG_INFO, " Frontend supports RGB565 - will use that instead of XRGB1555.\n");
#elif defined(FRONTEND_SUPPORTS_ARGB888)
	rgb565 = RETRO_PIXEL_FORMAT_XRGB8888;
	if (environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &rgb565))
		log_cb.log(RETRO_LOG_INFO, " Frontend supports xRGB888 - will use that instead of XRGB1555.\n");
#endif

	if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir) && system_dir) {
		FCEUI_SetBaseDirectory(system_dir);
	}

	/* Attempt to fetch extended game info */
	if (environ_cb(RETRO_ENVIRONMENT_GET_GAME_INFO_EXT, &info_ext) && info_ext) {
		content_data = (const uint8 *)info_ext->data;
		content_size = info_ext->size;

		if (info_ext->file_in_archive) {
			/* We don't have a 'physical' file in this
			 * case, but the core still needs a filename
			 * in order to detect the region of iNES v1.0
			 * ROMs. We therefore fake it, using the content
			 * directory, canonical content name, and content
			 * file extension */
			snprintf(content_path, sizeof(content_path), "%s%c%s.%s", info_ext->dir, PATH_DEFAULT_SLASH_C(),
			    info_ext->name, info_ext->ext);
		} else {
			strlcpy(content_path, info_ext->full_path, sizeof(content_path));
		}
	} else {
		if (!info || string_is_empty(info->path)) {
			return false;
		}

		strlcpy(content_path, info->path, sizeof(content_path));
	}

	check_system_specs();

	init_blit_buffer();
	if (!FCEUI_Initialize()) {
		return false;
	}

	/* initialize some of the default variables */
	sndquality = -1;
	opt_region = -1;
	isDendy  = false;

	/* Wii: initialize this or else last variable is passed through
	 * when loading another rom causing save state size change. */
	serialize_size = 0;

	set_variables();
	check_variables(true);

	FCEUI_SetSoundVolume(SND_MASTER, 100);
	FCEUI_Sound(44100);

	if (!(FCEUI_LoadGame(content_path, content_data, content_size, frontend_post_load_init))) {
		return false;
	}

	external_palette_exist = palette_game_available ? true : false;
	if (external_palette_exist) {
		FCEU_printf(" Loading custom palette: %s%cnes.pal\n", (char *)system_dir, PATH_DEFAULT_SLASH_C());
	}
	current_palette = 0;

	check_variables(true);
	stereo_filter_init();

	input_init_env(&environ_cb);
	input_set_defaults();
	input_update_descriptors();

	set_memory_maps();

	return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) {
	return false;
}

void retro_unload_game(void) {
	FCEUI_CloseGame();
#if defined(_3DS)
	if (fceu_video_out) {
		linearFree(fceu_video_out);
	}
#elif !defined(PS2) && !defined(PSP)
	if (fceu_video_out) {
		FCEU_afree(fceu_video_out);
	}
	fceu_video_out = NULL;
#endif
#if defined(PS2)
	ps2 = NULL;
#endif
#if defined(HAVE_NTSC_FILTER)
	ntsc_filter_deinit();
#endif
}

unsigned retro_get_region(void) {
	return FSettings.PAL ? RETRO_REGION_PAL : RETRO_REGION_NTSC;
}

void *retro_get_memory_data(unsigned type) {
	switch (type) {
	case RETRO_MEMORY_SAVE_RAM:
		if (GameInfo->type == GIT_FDS) {
			return FDSROM_ptr();
		}
		if (iNESCart.SaveGame[0]) {
			return iNESCart.SaveGame[0];
		}
		break;
	case RETRO_MEMORY_SYSTEM_RAM:
		return RAM;
	}

	return NULL;
}

size_t retro_get_memory_size(unsigned type) {
	switch (type) {
	case RETRO_MEMORY_SAVE_RAM:
		if (GameInfo->type == GIT_FDS) {
			return FDSROM_size();
		}
		if (iNESCart.SaveGameLen[0]) {
			return iNESCart.SaveGameLen[0];
		}
		break;
	case RETRO_MEMORY_SYSTEM_RAM:
		return RAM_SIZE;
	}

	return 0;
}
