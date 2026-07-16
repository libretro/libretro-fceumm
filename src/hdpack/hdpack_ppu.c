/* PPU-side recording for HD pack rendering.
 *
 * fceumm's PPU is scanline based, so instead of Mesen's per-pixel
 * HdNesPpu::DrawPixel hook the data is gathered in three cheap stages:
 *
 *  1. pputile.h records each background tile FETCH (34 slots per line:
 *     CHR pointer, attribute palette, fine-y) and the fine-x scroll
 *     active for each emitted 8-pixel GROUP.  A pixel x then maps back
 *     to fetch slot ((x + fine_x) >> 3), reproducing the two-tile
 *     shift-register overlap of the real PPU.
 *
 *  2. FetchSpriteData records the (up to 64) sprites fetched for the
 *     next scanline: CHR pointer, pattern bytes, attributes, row.
 *
 *  3. DoLine calls HDNes_RecordLine right before CopySprites, when the
 *     line's XBuf still contains pure background pixels (including the
 *     0x40 transparency marker), and the per-pixel hd_ppu_pixel_info
 *     records are assembled from stages 1+2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../fceu-types.h"
#include "../fceu.h"
#include "../ppu.h"
#include "../cart.h"
#include "../ines.h"

#include "hdpack.h"
#include "hdpack_data.h"

/* ppu.c globals that are external but not exported via ppu.h */
extern uint8_t PALRAM[0x20];
extern uint8_t UPALRAM[0x03];
extern uint8_t XOffset;
extern uint32_t TempAddr;
extern uint32_t RefreshAddr;

#define HD_ATR_VFLIP   0x80
#define HD_ATR_HFLIP   0x40
#define HD_ATR_SPBACK  0x20

typedef struct hd_bg_fetch
{
   int32_t abs_index;
   uint8_t data[16];
   uint8_t pal;                  /* 2-bit attribute palette */
   uint8_t fine_y;
   uint8_t valid;
} hd_bg_fetch;

typedef struct hd_sprite_rec
{
   int32_t abs_index;
   uint8_t data[16];
   uint8_t x;
   uint8_t atr;
   uint8_t row;                  /* source row within the 8x8 tile */
   uint8_t ca0;
   uint8_t ca1;
   uint8_t valid;
} hd_sprite_rec;

static hd_bg_fetch hd_bg_slots[HD_BG_FETCH_SLOTS];
static uint8_t hd_bg_group_x[HD_BG_GROUPS];
static hd_sprite_rec hd_sprites[HD_MAX_LINE_SPRITES];
static int hd_sprite_max = 0;
static int hd_cur_line = -1;
static uint32_t hd_frame_counter = 0;

int hd_ppu_init(void)
{
   if (!hd_screen.pixels)
   {
      hd_screen.pixels = (hd_ppu_pixel_info*)calloc(HD_PIXEL_COUNT,
            sizeof(hd_ppu_pixel_info));
      if (!hd_screen.pixels)
         return 0;
   }
   memset(hd_bg_slots, 0, sizeof(hd_bg_slots));
   memset(hd_sprites, 0, sizeof(hd_sprites));
   hd_sprite_max = 0;
   hd_cur_line = -1;
   hd_frame_counter = 0;
   return 1;
}

void hd_ppu_free(void)
{
   /* hd_screen.pixels is freed by HDNes_Unload */
}

/* Resolve a CHR pointer to an absolute CHR ROM tile index; -1 when the
 * pointer does not fall inside CHR ROM. */
static int32_t hd_chr_index(const uint8_t *tile_base)
{
   if (VROM && VROM_size)
   {
      size_t chr_bytes = (size_t)VROM_size * 8192;
      if (tile_base >= VROM && tile_base + 16 <= VROM + chr_bytes)
         return (int32_t)((tile_base - VROM) >> 4);
   }
   return -1;
}

/* For CHR RAM games the tile index is informational only (chr-ram tile
 * keys hash/compare on the tile data, not the index), but it must not
 * collide with the HD_NO_TILE marker (-1) which means "no background
 * tile on this pixel".  Mirror Mesen by deriving it from the CHR RAM
 * offset when possible. */
static int32_t hd_chr_ram_index(const uint8_t *tile_base)
{
   if (CHRptr[0] && CHRsize[0])
   {
      if (tile_base >= CHRptr[0] && tile_base + 16 <= CHRptr[0] + CHRsize[0])
         return (int32_t)((tile_base - CHRptr[0]) >> 4);
   }
   return 0;
}

void HDNes_LineStart(int line)
{
   int i;
   if (line < 0 || line >= HD_SCREEN_HEIGHT)
      return;
   hd_cur_line = line;
   /* Snapshot the *active* loopy-V scroll (RefreshAddr), not the T
    * latch: fceumm restores V's horizontal bits from T each scanline
    * and clears TempAddr, so TempAddr reads 0 mid-frame. The HD
    * compositor decodes this value as loopy-V (coarse/fine X and Y,
    * nametable select), matching the SD renderer which scrolls from
    * RefreshAddr. Reading TempAddr made every line scroll as 0, so the
    * rightmost columns scrolled in showed the backdrop's leftmost
    * (unscrolled) pixels - residue during rightward scroll. */
   hd_screen.line_tmp_vram_addr[line] = (uint16_t)(RefreshAddr & 0x7FFF);
   hd_screen.line_x_scroll[line] = (uint8_t)(XOffset & 7);
   for (i = 0; i < HD_BG_FETCH_SLOTS; i++)
      hd_bg_slots[i].valid = 0;
   for (i = 0; i < HD_BG_GROUPS; i++)
      hd_bg_group_x[i] = XOffset & 7;
}

void HDNes_RecordBgFetch(int slot, uint8_t *tile_base, uint8_t attr_pal,
      uint8_t fine_y)
{
   hd_bg_fetch *f;
   if (slot < 0 || slot >= HD_BG_FETCH_SLOTS)
      return;
   f = &hd_bg_slots[slot];
   f->pal = attr_pal & 3;
   f->fine_y = fine_y & 7;
   if (hd.is_chr_ram_game)
   {
      f->abs_index = hd_chr_ram_index(tile_base);
      memcpy(f->data, tile_base, 16);
   }
   else
      f->abs_index = hd_chr_index(tile_base);
   f->valid = 1;
}

void HDNes_RecordBgGroup(int group, uint8_t xoffset)
{
   if (group >= 0 && group < HD_BG_GROUPS)
      hd_bg_group_x[group] = xoffset & 7;
}

void HDNes_SpriteFetchStart(void)
{
   int i;
   for (i = 0; i < hd_sprite_max; i++)
      hd_sprites[i].valid = 0;
   hd_sprite_max = 0;
}

void HDNes_RecordSprite(int slot, uint8_t sx, uint8_t atr,
      uint8_t *tile_base, uint8_t row, uint8_t ca0, uint8_t ca1)
{
   hd_sprite_rec *s;
   if (slot < 0 || slot >= HD_MAX_LINE_SPRITES)
      return;
   s = &hd_sprites[slot];
   s->x = sx;
   s->atr = atr;
   s->row = row & 7;
   s->ca0 = ca0;
   s->ca1 = ca1;
   if (hd.is_chr_ram_game)
   {
      s->abs_index = hd_chr_ram_index(tile_base);
      memcpy(s->data, tile_base, 16);
   }
   else
      s->abs_index = hd_chr_index(tile_base);
   s->valid = 1;
   if (slot + 1 > hd_sprite_max)
      hd_sprite_max = slot + 1;
}

static uint8_t hd_read_palette_ram(uint32_t addr)
{
   addr &= 0x1F;
   if (addr & 3)
      return PALRAM[addr] & 0x3F;
   if (addr & 0xC)
      return UPALRAM[((addr & 0xC) >> 2) - 1] & 0x3F;
   return PALRAM[0] & 0x3F;
}

void HDNes_RecordLine(int line, const uint8_t *xbuf_line, int rendis)
{
   hd_ppu_pixel_info *row;
   uint32_t bg_pal_colors[4];
   uint32_t spr_pal_colors[4];
   uint8_t sprite_colors[HD_MAX_LINE_SPRITES][4];
   uint8_t ppu1;
   uint8_t backdrop;
   int bg_enabled;
   int spr_enabled;
   int bg_left_clip;
   int spr_left_clip;
   int no_bg_tiles;
   int x;
   int p;
   int n;

   if (line < 0 || line >= HD_SCREEN_HEIGHT || !hd_screen.pixels)
      return;

   ppu1 = PPU[1];
   hd_screen.line_grayscale[line] = ppu1 & 0x01;
   hd_screen.line_emphasis[line] = (uint8_t)(ppu1 >> 5);

   bg_enabled = (ppu1 & 0x08) != 0;
   spr_enabled = (ppu1 & 0x10) != 0;
   bg_left_clip = (ppu1 & 0x02) == 0;
   spr_left_clip = (ppu1 & 0x04) == 0;
   no_bg_tiles = !bg_enabled || (rendis & 2);
   if (rendis & 1)
      spr_enabled = 0;

   backdrop = PALRAM[0] & 0x3F;

   /* Palette colour words for the 4 background / 4 sprite palettes,
    * resolved once per line. */
   for (p = 0; p < 4; p++)
   {
      uint32_t base = (uint32_t)p << 2;
      bg_pal_colors[p] = (uint32_t)PALRAM[base + 3] |
            ((uint32_t)PALRAM[base + 2] << 8) |
            ((uint32_t)PALRAM[base + 1] << 16);
      spr_pal_colors[p] = (uint32_t)PALRAM[0x10 + base + 3] |
            ((uint32_t)PALRAM[0x10 + base + 2] << 8) |
            ((uint32_t)PALRAM[0x10 + base + 1] << 16);
      if (hd.version >= 100)
      {
         bg_pal_colors[p] |= (uint32_t)PALRAM[0] << 24;
         spr_pal_colors[p] |= 0xFF000000u;
      }
   }

   /* Per-sprite colour table: sprite_colors[n][ci] */
   if (spr_enabled)
   {
      for (n = 0; n < hd_sprite_max; n++)
      {
         uint32_t po = 0x10 + (uint32_t)((hd_sprites[n].atr & 3) << 2);
         sprite_colors[n][0] = hd_read_palette_ram(0);
         sprite_colors[n][1] = hd_read_palette_ram(po + 1);
         sprite_colors[n][2] = hd_read_palette_ram(po + 2);
         sprite_colors[n][3] = hd_read_palette_ram(po + 3);
      }
   }

   row = &hd_screen.pixels[line << 8];

   for (x = 0; x < HD_SCREEN_WIDTH; x++)
   {
      hd_ppu_pixel_info *px = &row[x];
      uint8_t xbuf = xbuf_line[x];

      /* ---- background tile ---- */
      if (no_bg_tiles || (bg_left_clip && x < 8))
         px->tile.tile_index = HD_NO_TILE;
      else
      {
         int group = x >> 3;
         uint8_t xoff = hd_bg_group_x[group];
         int slot = (x + xoff) >> 3;
         hd_bg_fetch *f = (slot < HD_BG_FETCH_SLOTS)
               ? &hd_bg_slots[slot] : NULL;

         if (!f || !f->valid ||
               (!hd.is_chr_ram_game && f->abs_index < 0))
            px->tile.tile_index = HD_NO_TILE;
         else
         {
            px->tile.tile_index = f->abs_index;
            px->tile.is_chr_ram = hd.is_chr_ram_game;
            if (hd.is_chr_ram_game)
               memcpy(px->tile.tile_data, f->data, 16);
            px->tile.palette_colors = bg_pal_colors[f->pal];
            px->tile.offset_y = f->fine_y;
            px->tile.offset_x = (uint8_t)((x + xoff) & 7);
            px->tile.hmirror = 0;
            px->tile.vmirror = 0;
            px->tile.bg_priority = 0;
            px->tile.palette_offset = (uint8_t)(f->pal << 2);
         }
      }

      px->tile.ppu_bg_color = backdrop;
      px->tile.bg_color_index = (uint8_t)((xbuf & 0x40) ? 0 : 1);
      px->tile.bg_color = (uint8_t)(xbuf & 0x3F);

      /* ---- sprites ---- */
      px->sprite_count = 0;
      if (spr_enabled && !(spr_left_clip && x < 8))
      {
         int j = 0;
         for (n = 0; n < hd_sprite_max && j < HD_MAX_SPRITES_PIXEL; n++)
         {
            hd_sprite_rec *s = &hd_sprites[n];
            int shift = x - (int)s->x;
            hd_ppu_tile_info *sp;
            uint8_t ci;
            if (!s->valid || shift < 0 || shift >= 8)
               continue;

            sp = &px->sprite[j];
            sp->tile_index = s->abs_index;
            sp->is_chr_ram = hd.is_chr_ram_game;
            if (hd.is_chr_ram_game)
               memcpy(sp->tile_data, s->data, 16);
            else if (s->abs_index < 0)
               continue;
            sp->palette_colors = spr_pal_colors[s->atr & 3];
            sp->offset_x = (uint8_t)shift;
            sp->offset_y = s->row;
            sp->hmirror = (s->atr & HD_ATR_HFLIP) ? 1 : 0;
            sp->vmirror = (s->atr & HD_ATR_VFLIP) ? 1 : 0;
            sp->bg_priority = (s->atr & HD_ATR_SPBACK) ? 1 : 0;
            sp->palette_offset =
                  (uint8_t)(0x10 + ((s->atr & 3) << 2));

            if (s->atr & HD_ATR_HFLIP)
               ci = (uint8_t)((((s->ca0 >> shift) & 1)) |
                     (((s->ca1 >> shift) & 1) << 1));
            else
               ci = (uint8_t)((((s->ca0 >> (7 - shift)) & 1)) |
                     (((s->ca1 >> (7 - shift)) & 1) << 1));
            sp->sprite_color_index = ci;
            sp->sprite_color = sprite_colors[n][ci];
            sp->ppu_bg_color = backdrop;
            sp->bg_color_index = px->tile.bg_color_index;
            j++;
         }
         px->sprite_count = (uint8_t)j;
      }
   }
}

/* ---- watched memory --------------------------------------------------- */

static uint8_t hd_debug_cpu_read(uint32_t addr)
{
   addr &= 0xFFFF;
   if (addr < 0x2000)
      return RAM[addr & 0x7FF];
   if (addr < 0x4020)
      return 0; /* register space reads have side effects; refuse */
   if (ARead[addr])
      return ARead[addr](addr);
   return 0;
}

static uint8_t hd_debug_ppu_read(uint32_t addr)
{
   addr &= 0x3FFF;
   if (addr >= 0x3F00)
      return hd_read_palette_ram(addr);
   if (addr >= 0x2000)
   {
      uint32_t a = addr & 0xFFF;
      if (vnapage[(a >> 10) & 3])
         return vnapage[(a >> 10) & 3][a & 0x3FF];
      return 0;
   }
   if (VPage[addr >> 10])
      return VPage[addr >> 10][addr];
   return 0;
}

void HDNes_FrameEnd(void)
{
   uint32_t i;
   if (!hdnes_active)
      return;
   hd_screen.frame_number = ++hd_frame_counter;
   for (i = 0; i < hd.watch_count; i++)
   {
      uint32_t addr = hd.watch_addrs[i];
      if (addr & HD_PPU_MEM_MARKER)
         hd_screen.watch_values[i] = hd_debug_ppu_read(addr & 0x3FFF);
      else
         hd_screen.watch_values[i] = hd_debug_cpu_read(addr);
   }
}
