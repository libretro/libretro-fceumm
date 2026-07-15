/* FCE Ultra - NES/Famicom Emulator
 *
 * Mesen HD texture pack support for the libretro port.
 *
 * Implements the Mesen "hires.txt" HD pack format (versions <= 109):
 * replacement tiles (CHR ROM and CHR RAM), conditions, custom
 * backgrounds, additional sprites, fallback tiles, custom palettes and
 * the $4100-$4106 (or $3002-$3062/$4018-$4019 alternate range) HD audio
 * device with OGG/WAV BGM + SFX playback.
 *
 * As a libretro extension, tile sheets and backgrounds referenced by
 * <img>/<background> tags may be .png, .dds or .webp files (Mesen
 * itself only supports .png).  Image and audio decoding is provided by
 * libretro-common (rpng/rdds/rwebp, rvorbis/rwav).
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _FCEU_HDPACK_H
#define _FCEU_HDPACK_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Nonzero while an HD pack is loaded and enabled.  All PPU-side hooks
 * are guarded on this flag so the hot paths pay a single predictable
 * branch when no pack is in use. */
extern int hdnes_active;

/* ---- lifecycle (libretro.c) ------------------------------------- */

/* Parse <system_dir>/HdPacks/<rom base name>/hires.txt.  Only file
 * parsing and raw file loads happen here (image decode is lazy), so it
 * is safe to call before FCEUI_LoadGame.  Returns 1 when a pack was
 * successfully loaded. */
int HDNes_LoadPack(const char *system_dir, const char *rom_path);

/* Must be called after FCEUI_LoadGame succeeded: resolves CHR ROM
 * information, fallback tiles, sprite-limit option and allocates the
 * screen-info buffers. */
void HDNes_PostLoadInit(void);

void HDNes_Unload(void);

uint32_t HDNes_GetScale(void);
int HDNes_HasAudio(void);

/* ---- video (libretro.c) ------------------------------------------ */

/* Compose the HD frame from the recorded PPU data.  Returns a
 * (256*scale) x (240*scale) XRGB8888 buffer owned by the HD pack
 * module. */
const uint32_t *HDNes_ComposeFrame(void);

/* Fed from FCEUD_SetPalette so HD output follows the user's palette
 * choice unless the pack ships its own palette.dat. */
void HDNes_SetPaletteColor(unsigned index, uint8_t r, uint8_t g, uint8_t b);

/* ---- audio (libretro.c) ------------------------------------------ */

/* (Re)install the HD audio register handlers; call after game load and
 * after every reset/power cycle so mapper handler reinstallation cannot
 * shadow them. */
void HDNes_InstallAudioHandlers(void);

/* Mix BGM/SFX into the core's post-stereo-filter sample buffer (int16
 * stereo pairs packed into int32 slots, `frames` pairs). */
void HDNes_MixAudio(int32_t *stereo_packed, size_t frames, unsigned out_rate);

/* Detect savestate loads of the HD audio register block and resume the
 * saved BGM track/offset.  Called once per retro_run. */
void HDNes_AudioStateSync(void);

/* Serialized HD audio register block, registered with AddExState from
 * libretro.c after game load. */
typedef struct hdnes_audio_state
{
   uint8_t album;
   uint8_t playback_options;
   uint8_t bgm_volume;
   uint8_t sfx_volume;
   int32_t last_bgm_track;
   int32_t bgm_offset;
} hdnes_audio_state;

extern hdnes_audio_state hdnes_audio_ss;

/* ---- PPU hooks (ppu.c / pputile.h) -------------------------------- */

/* Start of a visible scanline: snapshots loopy-t / fine-x scroll and
 * invalidates the per-line background fetch slots. */
void HDNes_LineStart(int line);

/* Called from pputile.h for every background tile fetch.  `slot` is the
 * X1 fetch counter (0..33), `tile_base` points at the 16-byte CHR tile,
 * `attr_pal` is the 2-bit attribute palette, `fine_y` the row within
 * the tile. */
void HDNes_RecordBgFetch(int slot, uint8_t *tile_base, uint8_t attr_pal,
      uint8_t fine_y);

/* Called from pputile.h when an 8-pixel group is emitted; records the
 * fine-x scroll active for that group so pixels can be mapped back to
 * fetch slots. */
void HDNes_RecordBgGroup(int group, uint8_t xoffset);

/* Called at the top of FetchSpriteData to invalidate the previous
 * line's sprite records. */
void HDNes_SpriteFetchStart(void);

/* Called from FetchSpriteData for every sprite fetched for the next
 * scanline. */
void HDNes_RecordSprite(int slot, uint8_t sx, uint8_t atr,
      uint8_t *tile_base, uint8_t row, uint8_t ca0, uint8_t ca1);

/* Called from DoLine after background rendering, before CopySprites:
 * fills the per-pixel HD screen info for the line from the recorded
 * fetch data and the pre-sprite XBuf contents. */
void HDNes_RecordLine(int line, const uint8_t *xbuf_line, int rendis);

/* End of emulated frame: captures watched memory and advances the HD
 * frame counter. */
void HDNes_FrameEnd(void);

#ifdef __cplusplus
}
#endif

#endif /* _FCEU_HDPACK_H */
