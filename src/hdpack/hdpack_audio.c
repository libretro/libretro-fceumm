/* HD pack audio: Mesen's HdAudioDevice register interface plus an
 * OGG/WAV mixer built on libretro-common's rvorbis and rwav decoders.
 *
 * Register map (normal range $4100-$4106, or with the
 * "alternateRegisterRange" option writes at $3002+N*$10 and reads at
 * $4018/$4019):
 *   +0 Playback options (bit0: loop BGM)
 *   +1 Playback control (bit0: toggle pause, bit1: stop BGM, bit2: stop SFX)
 *   +2 BGM volume (0-255)
 *   +3 SFX volume (0-255)
 *   +4 Album number
 *   +5 Play BGM track
 *   +6 Play SFX
 * Reads: +0 status (bit0 bgm playing, bit1 sfx playing, bit2 last track
 * flag), +1 revision, +2..+4 'N','E','A' signature.
 *
 * Mixing happens at the libretro output rate: each stream is decoded at
 * its native rate and resampled with the same 4-point Hermite curve
 * Mesen's HermiteResampler uses. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <formats/rvorbis.h>
#include <formats/rwav.h>

#include "../fceu-types.h"
#include "../fceu.h"
#include "../driver.h"

#include "hdpack.h"
#include "hdpack_data.h"

#define HD_MAX_SFX 16
#define HD_PULL_CHUNK 256

enum hd_stream_type
{
   HD_STREAM_NONE = 0,
   HD_STREAM_VORBIS,
   HD_STREAM_WAV
};

typedef struct hd_stream
{
   int type;
   int is_sfx;
   int done;
   int loop;
   uint32_t loop_position;

   /* decode state */
   rvorbis *vorb;
   uint8_t *file_data;           /* kept alive for rvorbis pull API */
   rwav_t wav;
   int wav_loaded;
   uint32_t src_pos;             /* frames consumed at source rate */
   unsigned src_rate;
   unsigned channels;

   /* resampler state (4-point Hermite, per channel) */
   double frac;
   double ratio;                 /* src_rate / out_rate */
   int16_t hist_l[4];
   int16_t hist_r[4];
   int primed;
} hd_stream;

static hd_stream hd_bgm_stream;
static hd_stream hd_sfx_streams[HD_MAX_SFX];
static int hd_audio_installed = 0;

/* register state (serialized) */
hdnes_audio_state hdnes_audio_ss;
static hdnes_audio_state hd_audio_shadow;
static uint8_t hd_track_error = 0;

static writefunc hd_old_write[7];
static readfunc hd_old_read[7];
static uint32_t hd_reg_addrs[7];
static readfunc hd_old_read_alt[2];

/* ---- stream management ---------------------------------------------------- */

static void hd_stream_close(hd_stream *s)
{
   if (s->vorb)
      rvorbis_close(s->vorb);
   if (s->file_data)
      free(s->file_data);
   if (s->wav_loaded)
      rwav_free(&s->wav);
   memset(s, 0, sizeof(*s));
}

static int hd_load_whole_file(const char *path, uint8_t **data,
      size_t *size)
{
   FILE *f = fopen(path, "rb");
   long len;
   uint8_t *buf;
   if (!f)
      return 0;
   fseek(f, 0, SEEK_END);
   len = ftell(f);
   fseek(f, 0, SEEK_SET);
   if (len <= 0)
   {
      fclose(f);
      return 0;
   }
   buf = (uint8_t*)malloc((size_t)len);
   if (!buf || fread(buf, 1, (size_t)len, f) != (size_t)len)
   {
      if (buf)
         free(buf);
      fclose(f);
      return 0;
   }
   fclose(f);
   *data = buf;
   *size = (size_t)len;
   return 1;
}

static int hd_str_iends(const char *s, const char *suffix)
{
   size_t sl = strlen(s);
   size_t xl = strlen(suffix);
   size_t i;
   if (xl > sl)
      return 0;
   s += sl - xl;
   for (i = 0; i < xl; i++)
   {
      char a = s[i], b = suffix[i];
      if (a >= 'A' && a <= 'Z') a = (char)(a - 'A' + 'a');
      if (b >= 'A' && b <= 'Z') b = (char)(b - 'A' + 'a');
      if (a != b)
         return 0;
   }
   return 1;
}

static int hd_stream_open(hd_stream *s, const char *path, int is_sfx,
      int loop, uint32_t start_offset, uint32_t loop_position)
{
   uint8_t *data = NULL;
   size_t size = 0;

   hd_stream_close(s);

   if (!hd_load_whole_file(path, &data, &size))
   {
      hd_log("Could not open audio file: %s", path);
      return 0;
   }

   if (hd_str_iends(path, ".wav") ||
         (size >= 12 && memcmp(data, "RIFF", 4) == 0 &&
          memcmp(data + 8, "WAVE", 4) == 0))
   {
      if (rwav_load(&s->wav, data, size) != RWAV_ITERATE_DONE)
      {
         hd_log("Invalid WAV file: %s", path);
         free(data);
         return 0;
      }
      free(data);
      if (s->wav.bitspersample != 16 || s->wav.numchannels < 1 ||
            s->wav.numchannels > 2 || !s->wav.samplerate)
      {
         hd_log("Unsupported WAV format (need 16-bit mono/stereo): %s",
               path);
         rwav_free(&s->wav);
         return 0;
      }
#ifdef MSB_FIRST
      /* rwav_load memcpys the PCM chunk verbatim; WAV samples are
       * little-endian on disk, so swap once for big-endian hosts. */
      {
         uint8_t *raw = (uint8_t*)s->wav.samples;
         size_t n = s->wav.subchunk2size & ~(size_t)1;
         size_t k;
         for (k = 0; k < n; k += 2)
         {
            uint8_t tmp = raw[k];
            raw[k] = raw[k + 1];
            raw[k + 1] = tmp;
         }
      }
#endif
      s->wav_loaded = 1;
      s->type = HD_STREAM_WAV;
      s->src_rate = s->wav.samplerate;
      s->channels = s->wav.numchannels;
      s->src_pos = start_offset;
      if (s->src_pos >= s->wav.numsamples)
         s->src_pos = 0;
   }
   else
   {
      int error = 0;
      rvorbis_info info;
      s->vorb = rvorbis_open_memory(data, (int)size, &error, NULL);
      if (!s->vorb)
      {
         hd_log("Invalid OGG file: %s (error %d)", path, error);
         free(data);
         return 0;
      }
      s->file_data = data;
      info = rvorbis_get_info(s->vorb);
      s->type = HD_STREAM_VORBIS;
      s->src_rate = info.sample_rate;
      s->channels = (unsigned)(info.channels < 2 ? 1 : 2);
      if (start_offset)
         rvorbis_seek(s->vorb, start_offset);
      s->src_pos = start_offset;
   }

   s->is_sfx = is_sfx;
   s->loop = loop;
   s->loop_position = loop_position;
   s->done = 0;
   s->frac = 0.0;
   s->primed = 0;
   return 1;
}

/* Pull one source frame (L/R) from the stream.  Returns 0 at end. */
static int hd_stream_pull(hd_stream *s, int16_t *l, int16_t *r)
{
   for (;;)
   {
      if (s->type == HD_STREAM_WAV)
      {
         const int16_t *samples = (const int16_t*)s->wav.samples;
         if (s->src_pos < s->wav.numsamples)
         {
            if (s->wav.numchannels == 2)
            {
               *l = samples[s->src_pos * 2];
               *r = samples[s->src_pos * 2 + 1];
            }
            else
            {
               *l = samples[s->src_pos];
               *r = *l;
            }
            s->src_pos++;
            return 1;
         }
      }
      else if (s->type == HD_STREAM_VORBIS)
      {
         int16_t buf[2];
         int got = rvorbis_get_samples_s16_interleaved(s->vorb,
               (int)s->channels, buf, (int)s->channels);
         if (got > 0)
         {
            *l = buf[0];
            *r = (s->channels == 2) ? buf[1] : buf[0];
            s->src_pos++;
            return 1;
         }
      }
      else
         return 0;

      /* end of stream */
      if (s->loop)
      {
         if (s->type == HD_STREAM_WAV)
            s->src_pos = (s->loop_position < s->wav.numsamples)
                  ? s->loop_position : 0;
         else
         {
            rvorbis_seek(s->vorb, s->loop_position);
            s->src_pos = s->loop_position;
         }
         continue;
      }
      s->done = 1;
      return 0;
   }
}

/* 4-point Hermite interpolation (same curve as Mesen's
 * HermiteResampler::HermiteInterpolate). */
static INLINE double hd_hermite(const int16_t *h, double mu)
{
   double m0 = ((double)h[2] - h[0]) * 0.5;
   double m1 = ((double)h[3] - h[1]) * 0.5;
   double mu2 = mu * mu;
   double mu3 = mu2 * mu;
   double a0 = 2 * mu3 - 3 * mu2 + 1;
   double a1 = mu3 - 2 * mu2 + mu;
   double a2 = mu3 - mu2;
   double a3 = -2 * mu3 + 3 * mu2;
   return a0 * h[1] + a1 * m0 + a2 * m1 + a3 * h[2];
}

static INLINE void hd_hist_push(hd_stream *s, int16_t l, int16_t r)
{
   s->hist_l[0] = s->hist_l[1]; s->hist_l[1] = s->hist_l[2];
   s->hist_l[2] = s->hist_l[3]; s->hist_l[3] = l;
   s->hist_r[0] = s->hist_r[1]; s->hist_r[1] = s->hist_r[2];
   s->hist_r[2] = s->hist_r[3]; s->hist_r[3] = r;
}

/* Produce one output frame at out_rate; returns 0 when the stream is
 * finished. */
static int hd_stream_next(hd_stream *s, unsigned out_rate,
      int32_t *out_l, int32_t *out_r)
{
   int16_t l, r;
   double v;

   if (s->done || s->type == HD_STREAM_NONE)
      return 0;

   s->ratio = (double)s->src_rate / (double)out_rate;

   if (!s->primed)
   {
      int i;
      for (i = 0; i < 4; i++)
      {
         if (!hd_stream_pull(s, &l, &r))
            return 0;
         hd_hist_push(s, l, r);
      }
      s->primed = 1;
      s->frac = 0.0;
   }

   v = hd_hermite(s->hist_l, s->frac);
   *out_l = (int32_t)v;
   v = hd_hermite(s->hist_r, s->frac);
   *out_r = (int32_t)v;

   s->frac += s->ratio;
   while (s->frac >= 1.0)
   {
      if (!hd_stream_pull(s, &l, &r))
      {
         /* flush the tail of the history window */
         hd_hist_push(s, 0, 0);
         if (s->hist_l[1] == 0 && s->hist_l[2] == 0 &&
               s->hist_r[1] == 0 && s->hist_r[2] == 0)
         {
            s->done = 1;
            break;
         }
      }
      else
         hd_hist_push(s, l, r);
      s->frac -= 1.0;
   }

   return 1;
}

/* ---- device logic ----------------------------------------------------------- */

static int hd_find_track(const hd_track *list, uint32_t count, int32_t id)
{
   uint32_t i;
   for (i = 0; i < count; i++)
   {
      if (list[i].id == id)
         return (int)i;
   }
   return -1;
}

static int hd_play_bgm_track(int32_t track_id, uint32_t start_offset)
{
   int idx = hd_find_track(hd.bgm, hd.bgm_count, track_id);
   if (idx >= 0)
   {
      int loop = (hdnes_audio_ss.playback_options & 0x01) != 0;
      if (hd_stream_open(&hd_bgm_stream, hd.bgm[idx].path, 0, loop,
            start_offset, hd.bgm[idx].loop_position))
      {
         hdnes_audio_ss.last_bgm_track = track_id;
         return 1;
      }
   }
   else
      hd_log("Invalid album+track combination: %d:%d",
            (int)(track_id >> 8), (int)(track_id & 0xFF));
   return 0;
}

static int hd_play_sfx(uint8_t sfx_number)
{
   int32_t id = (int32_t)hdnes_audio_ss.album * 256 + sfx_number;
   int idx = hd_find_track(hd.sfx, hd.sfx_count, id);
   if (idx >= 0)
   {
      int i;
      for (i = 0; i < HD_MAX_SFX; i++)
      {
         if (hd_sfx_streams[i].type == HD_STREAM_NONE ||
               hd_sfx_streams[i].done)
         {
            /* Mesen's PlaySfx returns !Play(...) (an error flag). */
            return !hd_stream_open(&hd_sfx_streams[i], hd.sfx[idx].path,
                  1, 0, 0, 0);
         }
      }
      return 0; /* all voices busy; drop silently */
   }
   hd_log("Invalid album+sfx number combination: %d:%d",
         (int)hdnes_audio_ss.album, (int)sfx_number);
   return 0;
}

static int hd_bgm_paused = 0;

static void hd_process_control_flags(uint8_t flags)
{
   if (flags & 0x01)
      hd_bgm_paused = !hd_bgm_paused;
   if (flags & 0x02)
      hd_stream_close(&hd_bgm_stream);
   if (flags & 0x04)
   {
      int i;
      for (i = 0; i < HD_MAX_SFX; i++)
         hd_stream_close(&hd_sfx_streams[i]);
   }
}

static void hd_audio_reg_write(int reg, uint8_t value)
{
   switch (reg)
   {
      case 0:
         hdnes_audio_ss.playback_options = value;
         hd_bgm_stream.loop = (value & 0x01) != 0;
         break;
      case 1:
         hd_process_control_flags(value);
         break;
      case 2:
         hdnes_audio_ss.bgm_volume = value;
         break;
      case 3:
         hdnes_audio_ss.sfx_volume = value;
         break;
      case 4:
         hdnes_audio_ss.album = value;
         break;
      case 5:
         /* Mesen quirk kept as-is: the BGM path assigns the *success*
          * flag to _trackError, while the SFX path assigns an actual
          * error flag.  Homebrew written against Mesen expects this. */
         hd_track_error = (uint8_t)hd_play_bgm_track(
               (int32_t)hdnes_audio_ss.album * 256 + value, 0);
         break;
      case 6:
         hd_track_error = (uint8_t)hd_play_sfx(value);
         break;
   }
   memcpy(&hd_audio_shadow, &hdnes_audio_ss, sizeof(hd_audio_shadow));
}

static uint8_t hd_audio_reg_read(uint32_t addr)
{
   int sfx_playing = 0;
   int i;
   switch (addr & 0x7)
   {
      case 0:
         for (i = 0; i < HD_MAX_SFX; i++)
         {
            if (hd_sfx_streams[i].type != HD_STREAM_NONE &&
                  !hd_sfx_streams[i].done)
            {
               sfx_playing = 1;
               break;
            }
         }
         return (uint8_t)(
               ((hd_bgm_stream.type != HD_STREAM_NONE &&
                  !hd_bgm_stream.done) ? 1 : 0) |
               (sfx_playing ? 2 : 0) |
               (hd_track_error ? 4 : 0));
      case 1: return 1;   /* revision */
      case 2: return 'N';
      case 3: return 'E';
      case 4: return 'A';
   }
   return 0;
}

/* ---- fceumm bus handlers ------------------------------------------------------ */

static DECLFW(hd_bus_write)
{
   int reg = (A > 0x4100) ? (int)(A & 0xF) : (int)((A & 0xF0) >> 4);
   hd_audio_reg_write(reg, V);
}

static DECLFR(hd_bus_read)
{
   return hd_audio_reg_read(A);
}

void HDNes_InstallAudioHandlers(void)
{
   int i;
   if (!hdnes_active || !HDNes_HasAudio())
      return;

   if (hd.option_flags & HD_OPT_ALT_REGISTER_RANGE)
   {
      for (i = 0; i < 7; i++)
      {
         uint32_t addr = 0x3002 + (uint32_t)i * 0x10;
         hd_reg_addrs[i] = addr;
         hd_old_write[i] = GetWriteHandler((int32_t)addr);
         SetWriteHandler((int32_t)addr, (int32_t)addr, hd_bus_write);
      }
      hd_old_read_alt[0] = GetReadHandler(0x4018);
      hd_old_read_alt[1] = GetReadHandler(0x4019);
      SetReadHandler(0x4018, 0x4019, hd_bus_read);
   }
   else
   {
      for (i = 0; i < 7; i++)
      {
         uint32_t addr = 0x4100 + (uint32_t)i;
         hd_reg_addrs[i] = addr;
         hd_old_write[i] = GetWriteHandler((int32_t)addr);
         hd_old_read[i] = GetReadHandler((int32_t)addr);
      }
      SetWriteHandler(0x4100, 0x4106, hd_bus_write);
      SetReadHandler(0x4100, 0x4106, hd_bus_read);
   }
   hd_audio_installed = 1;
}

/* ---- mixing / state ------------------------------------------------------------ */

void HDNes_MixAudio(int32_t *stereo_packed, size_t frames,
      unsigned out_rate)
{
   int16_t *samples = (int16_t*)stereo_packed;
   size_t f;
   int i;

   if (!hdnes_active || !hd_audio_installed || !out_rate)
      return;

   for (f = 0; f < frames; f++)
   {
      int32_t acc_l = samples[f * 2];
      int32_t acc_r = samples[f * 2 + 1];
      int32_t l, r;

      if (!hd_bgm_paused &&
            hd_stream_next(&hd_bgm_stream, out_rate, &l, &r))
      {
         acc_l += (l * (int32_t)hdnes_audio_ss.bgm_volume) >> 8;
         acc_r += (r * (int32_t)hdnes_audio_ss.bgm_volume) >> 8;
      }
      for (i = 0; i < HD_MAX_SFX; i++)
      {
         if (hd_sfx_streams[i].type != HD_STREAM_NONE &&
               hd_stream_next(&hd_sfx_streams[i], out_rate, &l, &r))
         {
            acc_l += (l * (int32_t)hdnes_audio_ss.sfx_volume) >> 8;
            acc_r += (r * (int32_t)hdnes_audio_ss.sfx_volume) >> 8;
         }
      }

      if (acc_l > 32767) acc_l = 32767;
      if (acc_l < -32768) acc_l = -32768;
      if (acc_r > 32767) acc_r = 32767;
      if (acc_r < -32768) acc_r = -32768;
      samples[f * 2] = (int16_t)acc_l;
      samples[f * 2 + 1] = (int16_t)acc_r;
   }

   /* keep the serialized BGM offset current for savestates */
   hdnes_audio_ss.bgm_offset =
         (hd_bgm_stream.type != HD_STREAM_NONE && !hd_bgm_stream.done)
         ? (int32_t)hd_bgm_stream.src_pos : -1;
   hd_audio_shadow.bgm_offset = hdnes_audio_ss.bgm_offset;
}

void HDNes_AudioStateSync(void)
{
   if (!hdnes_active || !hd_audio_installed)
      return;
   if (memcmp(&hdnes_audio_ss, &hd_audio_shadow,
         sizeof(hdnes_audio_ss)) == 0)
      return;

   /* A savestate load replaced the register block: resume playback. */
   hd_bgm_stream.loop = (hdnes_audio_ss.playback_options & 0x01) != 0;
   if (hdnes_audio_ss.last_bgm_track >= 0 &&
         hdnes_audio_ss.bgm_offset > 0)
      hd_play_bgm_track(hdnes_audio_ss.last_bgm_track,
            (uint32_t)hdnes_audio_ss.bgm_offset);
   else
      hd_stream_close(&hd_bgm_stream);
   memcpy(&hd_audio_shadow, &hdnes_audio_ss, sizeof(hd_audio_shadow));
}

void hd_audio_init(void)
{
   int i;
   memset(&hd_bgm_stream, 0, sizeof(hd_bgm_stream));
   for (i = 0; i < HD_MAX_SFX; i++)
      memset(&hd_sfx_streams[i], 0, sizeof(hd_stream));
   memset(&hdnes_audio_ss, 0, sizeof(hdnes_audio_ss));
   hdnes_audio_ss.bgm_volume = 128;
   hdnes_audio_ss.sfx_volume = 128;
   hdnes_audio_ss.last_bgm_track = -1;
   hdnes_audio_ss.bgm_offset = -1;
   memcpy(&hd_audio_shadow, &hdnes_audio_ss, sizeof(hd_audio_shadow));
   hd_track_error = 0;
   hd_bgm_paused = 0;
   hd_audio_installed = 0;
}

void hd_audio_free(void)
{
   int i;
   hd_stream_close(&hd_bgm_stream);
   for (i = 0; i < HD_MAX_SFX; i++)
      hd_stream_close(&hd_sfx_streams[i]);
   hd_audio_installed = 0;
}
