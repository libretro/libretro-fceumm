/* HD frame composer: C port of Mesen's HdNesPack renderer, with the
 * template scale parameter replaced by a runtime value.  Consumes the
 * per-pixel screen info recorded by hdpack_ppu.c and produces a
 * (256*scale) x (240*scale) XRGB8888 frame. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../fceu-types.h"
#include "../fceu.h"
#include "../driver.h"

#include "hdpack.h"
#include "hdpack_data.h"

static uint32_t *hd_frame_buf = NULL;
static uint32_t hd_frame_pixels = 0;
static uint32_t hd_palette[64];
static uint32_t hd_user_palette[64];   /* fed from FCEUD_SetPalette */

/* per-frame background layer state */
typedef struct hd_bg_cfg
{
   int32_t bg_index;             /* index into hd.bgs[prio], -1 unused */
   int32_t priority;
   int32_t scroll_x;
   int32_t scroll_y;
   int32_t min_x;
   int32_t max_x;
   /* Per-line cache, filled by hd_on_line_start so the per-pixel draw
    * avoids re-deriving the bitmap, blend mode and brightness. */
   const uint32_t *row;          /* first source pixel for this line, at
                                  * bitmap column (left - scroll_x)*scale */
   uint32_t        bmp_width;
   uint8_t         blend_mode;
   uint8_t         brightness;
   uint8_t         simple_copy;  /* scale==1, ALPHA blend, brightness 255 */
} hd_bg_cfg;

static hd_bg_cfg hd_bg_cfgs[HD_BG_LAYER_COUNT];
static uint8_t hd_active_bg_count[4];

static int32_t hd_scroll_x_line;
static hd_pack_tile *hd_cached_tile;
static int hd_use_cached_tile;
static int hd_cache_enabled;

/* ---- additional-sprite match cache ------------------------------------- */

typedef struct hd_add_node
{
   uint16_t addition_idx;
   int32_t next;
} hd_add_node;

typedef struct hd_add_entry
{
   hd_tile_key key;
   int32_t first;                /* -1 = no matches; index into node pool */
   uint8_t used;
} hd_add_entry;

static hd_add_entry *hd_add_map = NULL;
static uint32_t hd_add_map_cap = 0;
static uint32_t hd_add_map_count = 0;
static hd_add_node *hd_add_nodes = NULL;
static uint32_t hd_add_node_count = 0;
static uint32_t hd_add_node_alloc = 0;

int hd_render_init(void)
{
   uint32_t pixels = (HD_SCREEN_WIDTH * hd.scale) *
         (HD_SCREEN_HEIGHT * hd.scale);
   if (hd_frame_buf && hd_frame_pixels != pixels)
   {
      free(hd_frame_buf);
      hd_frame_buf = NULL;
   }
   if (!hd_frame_buf)
   {
      hd_frame_buf = (uint32_t*)malloc(pixels * sizeof(uint32_t));
      if (!hd_frame_buf)
         return 0;
      hd_frame_pixels = pixels;
   }
   memset(hd_frame_buf, 0, pixels * sizeof(uint32_t));
   return 1;
}

void hd_render_free(void)
{
   if (hd_frame_buf)
      free(hd_frame_buf);
   hd_frame_buf = NULL;
   hd_frame_pixels = 0;
   if (hd_add_map)
      free(hd_add_map);
   hd_add_map = NULL;
   hd_add_map_cap = 0;
   hd_add_map_count = 0;
   if (hd_add_nodes)
      free(hd_add_nodes);
   hd_add_nodes = NULL;
   hd_add_node_count = 0;
   hd_add_node_alloc = 0;
}

void HDNes_SetPaletteColor(unsigned index, uint8_t r, uint8_t g, uint8_t b)
{
   if (index < 64)
      hd_user_palette[index] = 0xFF000000u |
            ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

/* ---- condition evaluation ------------------------------------------------ */

static int hd_cond_check(uint16_t idx, int x, int y,
      const hd_ppu_tile_info *tile);

static int hd_tile_matches_cond(const hd_condition *c,
      const hd_ppu_tile_info *target)
{
   if (c->tile_index >= 0)
   {
      if (target->palette_colors != c->palette_colors && !c->ignore_palette)
         return 0;
      return target->tile_index == c->tile_index ||
            hd_get_fallback_tile(target->tile_index) == c->tile_index;
   }
   if (c->ignore_palette)
      return memcmp(target->tile_data, c->tile_data, 16) == 0;
   return target->palette_colors == c->palette_colors &&
         memcmp(target->tile_data, c->tile_data, 16) == 0;
}

static int hd_cond_check_internal(const hd_condition *c, int x, int y,
      const hd_ppu_tile_info *tile)
{
   switch (c->type)
   {
      case HD_COND_HMIRROR:
         return tile && tile->hmirror;
      case HD_COND_VMIRROR:
         return tile && tile->vmirror;
      case HD_COND_BGPRIORITY:
         return tile && tile->bg_priority;
      case HD_COND_SPRITE_PALETTE:
         return tile && tile->palette_offset ==
               (uint8_t)(0x10 + (c->operand_a << 2));

      case HD_COND_FRAME_RANGE:
         if (c->operand_a == 0)
            return 0;
         return (hd_screen.frame_number % c->operand_a) >= c->operand_b;

      case HD_COND_MEMORY_CHECK:
      case HD_COND_MEMORY_CHECK_CONST:
      {
         uint8_t a = 0;
         uint8_t b;
         if (c->watch_slot_a >= 0)
            a = hd_screen.watch_values[c->watch_slot_a] & c->mask;
         if (c->type == HD_COND_MEMORY_CHECK)
         {
            b = 0;
            if (c->watch_slot_b >= 0)
               b = hd_screen.watch_values[c->watch_slot_b] & c->mask;
         }
         else
            b = (uint8_t)c->operand_b;
         switch (c->op)
         {
            case HD_OP_EQ: return a == b;
            case HD_OP_NE: return a != b;
            case HD_OP_GT: return a > b;
            case HD_OP_LT: return a < b;
            case HD_OP_LE: return a <= b;
            case HD_OP_GE: return a >= b;
         }
         return 0;
      }

      case HD_COND_POS_CHECK_X:
      case HD_COND_POS_CHECK_Y:
      case HD_COND_ORIGIN_POS_CHECK_X:
      case HD_COND_ORIGIN_POS_CHECK_Y:
      {
         uint32_t val;
         switch (c->type)
         {
            case HD_COND_POS_CHECK_Y:
               val = (uint32_t)y;
               break;
            case HD_COND_ORIGIN_POS_CHECK_X:
               val = tile->hmirror
                     ? (uint32_t)(x - (7 - tile->offset_x))
                     : (uint32_t)(x - tile->offset_x);
               break;
            case HD_COND_ORIGIN_POS_CHECK_Y:
               val = tile->vmirror
                     ? (uint32_t)(y - (7 - tile->offset_y))
                     : (uint32_t)(y - tile->offset_y);
               break;
            case HD_COND_POS_CHECK_X:
            default:
               val = (uint32_t)x;
               break;
         }
         switch (c->op)
         {
            case HD_OP_EQ: return val == c->operand_b;
            case HD_OP_NE: return val != c->operand_b;
            case HD_OP_GT: return val > c->operand_b;
            case HD_OP_LT: return val < c->operand_b;
            case HD_OP_LE: return val <= c->operand_b;
            case HD_OP_GE: return val >= c->operand_b;
         }
         return 0;
      }

      case HD_COND_TILE_AT_POS:
      {
         const hd_ppu_tile_info *target =
               &hd_screen.pixels[c->pixel_offset].tile;
         return hd_tile_matches_cond(c, target);
      }

      case HD_COND_TILE_NEARBY:
      {
         int pixel_index = c->pixel_offset + (y * 256) + x;
         if (pixel_index < 0 || pixel_index >= HD_PIXEL_COUNT)
            return 0;
         return hd_tile_matches_cond(c,
               &hd_screen.pixels[pixel_index].tile);
      }

      case HD_COND_SPRITE_AT_POS:
      {
         const hd_ppu_pixel_info *px = &hd_screen.pixels[c->pixel_offset];
         int i;
         for (i = 0; i < px->sprite_count; i++)
         {
            if (hd_tile_matches_cond(c, &px->sprite[i]))
               return 1;
         }
         return 0;
      }

      case HD_COND_SPRITE_NEARBY:
      {
         int x_sign = (tile && tile->hmirror) ? -1 : 1;
         int y_sign = (tile && tile->vmirror) ? -1 : 1;
         int pixel_index = ((y + c->tile_y * y_sign) * 256) + x +
               (c->tile_x * x_sign);
         const hd_ppu_pixel_info *px;
         int i;
         if (pixel_index < 0 || pixel_index >= HD_PIXEL_COUNT)
            return 0;
         px = &hd_screen.pixels[pixel_index];
         for (i = 0; i < px->sprite_count; i++)
         {
            if (hd_tile_matches_cond(c, &px->sprite[i]))
               return 1;
         }
         return 0;
      }
   }
   return 0;
}

static int hd_cond_check(uint16_t idx, int x, int y,
      const hd_ppu_tile_info *tile)
{
   hd_condition *c = &hd.conds[idx];
   int result;
   if (c->result_cache >= 0)
      return c->result_cache;
   result = hd_cond_check_internal(c, x, y, tile);
   if (c->negate)
      result = !result;
   if (c->use_cache)
      c->result_cache = (int8_t)(result ? 1 : 0);
   return result;
}

static int hd_tile_matches_conditions(const hd_pack_tile *t, int x, int y,
      const hd_ppu_tile_info *tile)
{
   uint16_t i;
   for (i = 0; i < t->cond_count; i++)
   {
      if (!hd_cond_check(t->cond_idx[i], x, y, tile))
         return 0;
   }
   return 1;
}

/* ---- lazy tile pixel-data init ----------------------------------------- */

static void hd_tile_update_flags(hd_pack_tile *t)
{
   size_t count = (size_t)t->width * t->height;
   size_t i;
   t->blank = 1;
   t->has_transparent = 0;
   t->fully_transparent = 1;
   for (i = 0; i < count; i++)
   {
      if (t->hd_data[i] != t->hd_data[0])
         t->blank = 0;
      if ((t->hd_data[i] & 0xFF000000u) != 0xFF000000u)
         t->has_transparent = 1;
      if (t->hd_data[i] & 0xFF000000u)
         t->fully_transparent = 0;
   }
}

static void hd_tile_init(hd_pack_tile *t)
{
   hd_bitmap *bmp = &hd.bitmaps[t->bitmap_index];
   size_t count = (size_t)t->width * t->height;

   t->need_init = 0;
   t->hd_data = (uint32_t*)calloc(count, sizeof(uint32_t));
   if (!t->hd_data)
   {
      static uint32_t hd_null_tile = 0;
      t->hd_data = &hd_null_tile;
      t->width = 1;
      t->height = 1;
      t->fully_transparent = 1;
      t->blank = 1;
      return;
   }

   if (hd_bitmap_init(bmp))
   {
      uint32_t offset = t->y * bmp->width + t->x;
      if ((size_t)offset + ((size_t)(t->height - 1) * bmp->width) +
            t->width <= (size_t)bmp->width * bmp->height)
      {
         uint32_t y;
         for (y = 0; y < t->height; y++)
         {
            memcpy(t->hd_data + (size_t)y * t->width,
                  bmp->pixels + offset, t->width * sizeof(uint32_t));
            offset += bmp->width;
         }
      }
   }

   hd_tile_update_flags(t);
}

/* ---- colour helpers ------------------------------------------------------ */

static INLINE void hd_blend_alpha(uint8_t *output, const uint8_t *input)
{
   unsigned inverted_alpha = 256 - input[3];
   output[0] = (uint8_t)(input[0] + ((inverted_alpha * output[0]) >> 8));
   output[1] = (uint8_t)(input[1] + ((inverted_alpha * output[1]) >> 8));
   output[2] = (uint8_t)(input[2] + ((inverted_alpha * output[2]) >> 8));
   output[3] = 0xFF;
}

static INLINE void hd_blend_add(uint8_t *output, const uint8_t *input)
{
   int v;
   v = (int)input[0] + output[0]; output[0] = (uint8_t)(v > 255 ? 255 : v);
   v = (int)input[1] + output[1]; output[1] = (uint8_t)(v > 255 ? 255 : v);
   v = (int)input[2] + output[2]; output[2] = (uint8_t)(v > 255 ? 255 : v);
   output[3] = 0xFF;
}

static INLINE void hd_blend_subtract(uint8_t *output, const uint8_t *input)
{
   int v;
   v = (int)output[0] - input[0]; output[0] = (uint8_t)(v < 0 ? 0 : v);
   v = (int)output[1] - input[1]; output[1] = (uint8_t)(v < 0 ? 0 : v);
   v = (int)output[2] - input[2]; output[2] = (uint8_t)(v < 0 ? 0 : v);
   output[3] = 0xFF;
}

static INLINE uint32_t hd_adjust_brightness(const uint8_t *input,
      int brightness)
{
   int r = (brightness * ((int)input[0] + 1)) >> 8;
   int g = (brightness * ((int)input[1] + 1)) >> 8;
   int b = (brightness * ((int)input[2] + 1)) >> 8;
   if (r > 255) r = 255;
   if (g > 255) g = 255;
   if (b > 255) b = 255;
   return (uint32_t)r | ((uint32_t)g << 8) | ((uint32_t)b << 16) |
         ((uint32_t)input[3] << 24);
}

static INLINE void hd_draw_color(uint32_t color, uint32_t *out,
      uint32_t screen_width, uint32_t scale)
{
   uint32_t i, j;
   for (i = 0; i < scale; i++)
   {
      for (j = 0; j < scale; j++)
         out[j] = color;
      out += screen_width;
   }
}

/* ---- HD tile draw --------------------------------------------------------- */

static void hd_draw_tile(const hd_ppu_tile_info *tile_info,
      hd_pack_tile *hd_tile, uint32_t *out, uint32_t screen_width,
      uint32_t scale)
{
   const uint32_t *bitmap = hd_tile->hd_data;
   uint32_t tile_width = 8 * scale;
   uint8_t tile_offset_x = tile_info->hmirror
         ? (uint8_t)(7 - tile_info->offset_x) : tile_info->offset_x;
   int32_t bitmap_offset = (int32_t)((tile_info->offset_y * scale) *
         tile_width + tile_offset_x * scale);
   int32_t small_inc = 1;
   int32_t large_inc = (int32_t)(tile_width - scale);
   uint32_t x, y;

   if (hd_tile->fully_transparent)
      return;
   /* fallback dummy tile from failed alloc */
   if (hd_tile->width != tile_width)
      return;

   if (tile_info->hmirror)
   {
      bitmap_offset += (int32_t)scale - 1;
      small_inc = -1;
      large_inc = (int32_t)(tile_width + scale);
   }
   if (tile_info->vmirror)
   {
      bitmap_offset += (int32_t)(tile_width * (scale - 1));
      large_inc = (tile_info->hmirror ? (int32_t)scale : -(int32_t)scale)
            - (int32_t)tile_width;
   }

   if (hd_tile->has_transparent || hd_tile->brightness != 255)
   {
      for (y = 0; y < scale; y++)
      {
         for (x = 0; x < scale; x++)
         {
            uint32_t rgb;
            uint32_t src = bitmap[bitmap_offset];
            if (hd_tile->brightness == 255)
               rgb = src;
            else
               rgb = hd_adjust_brightness(
                     (const uint8_t*)&bitmap[bitmap_offset],
                     hd_tile->brightness);

            if (!hd_tile->has_transparent ||
                  (src & 0xFF000000u) == 0xFF000000u)
               *out = rgb;
            else if (src & 0xFF000000u)
               hd_blend_alpha((uint8_t*)out, (const uint8_t*)&rgb);

            out++;
            bitmap_offset += small_inc;
         }
         bitmap_offset += large_inc;
         out += screen_width - scale;
      }
   }
   else
   {
      for (y = 0; y < scale; y++)
      {
         for (x = 0; x < scale; x++)
         {
            *out = bitmap[bitmap_offset];
            out++;
            bitmap_offset += small_inc;
         }
         bitmap_offset += large_inc;
         out += screen_width - scale;
      }
   }
}

/* ---- custom backgrounds ---------------------------------------------------- */

static void hd_draw_custom_background(const hd_background *bg,
      uint32_t *out, uint32_t x, uint32_t y, uint32_t screen_width,
      uint32_t scale)
{
   hd_bitmap *bmp = &hd.bg_bitmaps[bg->bitmap_index];
   uint32_t width = bmp->width;
   const uint32_t *png = bmp->pixels +
         (size_t)((bg->top + y) * scale) * width +
         (size_t)((bg->left + x) * scale);
   uint32_t i, j;

   for (i = 0; i < scale; i++)
   {
      for (j = 0; j < scale; j++)
      {
         uint32_t pixel = png[j];
         if (bg->brightness != 255)
            pixel = hd_adjust_brightness((const uint8_t*)&png[j],
                  bg->brightness);
         switch (bg->blend_mode)
         {
            case HD_BLEND_ALPHA:
               if (pixel >= 0xFF000000u)
                  out[j] = pixel;
               else if (pixel >= 0x01000000u)
                  hd_blend_alpha((uint8_t*)&out[j],
                        (const uint8_t*)&pixel);
               break;
            case HD_BLEND_ADD:
               hd_blend_add((uint8_t*)&out[j], (const uint8_t*)&pixel);
               break;
            case HD_BLEND_SUBTRACT:
               hd_blend_subtract((uint8_t*)&out[j],
                     (const uint8_t*)&pixel);
               break;
         }
      }
      out += screen_width;
      png += width;
   }
}

static int32_t hd_get_layer_index(uint8_t priority)
{
   uint32_t i;
   for (i = 0; i < hd.bg_count[priority]; i++)
   {
      const hd_background *bg = &hd.bgs[priority][i];
      uint16_t k;
      int is_match = 1;
      for (k = 0; k < bg->cond_count; k++)
      {
         if (!hd_cond_check(bg->cond_idx[k], 0, 0, NULL))
         {
            is_match = 0;
            break;
         }
      }
      if (is_match)
         return (int32_t)i;
   }
   return -1;
}

static void hd_on_line_start(int y)
{
   uint16_t t = hd_screen.line_tmp_vram_addr[y];
   uint8_t xscroll = hd_screen.line_x_scroll[y];
   int32_t scroll_y;
   int layer;
   int i;

   hd_scroll_x_line = ((t & 0x1F) << 3) | xscroll |
         ((t & 0x400) ? 0x100 : 0);
   hd_use_cached_tile = 0;

   scroll_y = (int32_t)((((t & 0x3E0) >> 2) | ((t & 0x7000) >> 12)) +
         ((t & 0x800) ? 240 : 0));

   for (layer = 0; layer < 4; layer++)
   {
      for (i = 0; i < hd_active_bg_count[layer]; i++)
      {
         hd_bg_cfg *cfg = &hd_bg_cfgs[layer * HD_PRIO_PER_LAYER + i];
         const hd_background *bg;
         hd_bitmap *bmp;
         if (cfg->bg_index < 0)
            continue;
         bg = &hd.bgs[cfg->priority][cfg->bg_index];
         bmp = &hd.bg_bitmaps[bg->bitmap_index];
         if (!hd_bitmap_init(bmp))
         {
            cfg->min_x = -1;
            cfg->max_x = -1;
            continue;
         }

         cfg->scroll_x = (int32_t)(hd_scroll_x_line * bg->h_scroll_ratio);
         cfg->scroll_y = (int32_t)(scroll_y * bg->v_scroll_ratio);
         if (y >= -cfg->scroll_y &&
               (uint32_t)((y + (int32_t)bg->top + cfg->scroll_y + 1) *
                  (int32_t)hd.scale) <= bmp->height)
         {
            uint32_t sc = hd.scale;
            cfg->min_x = -cfg->scroll_x;
            cfg->max_x = (int32_t)(bmp->width / sc) -
                  (int32_t)bg->left - cfg->scroll_x - 1;
            /* Cache the source row and draw parameters for the per-pixel
             * loop. The source column for screen x is
             * (bg->left + x + scroll_x)*scale; store the base at x==0 so
             * the draw can index by (x + scroll_x). */
            cfg->bmp_width  = bmp->width;
            cfg->row = bmp->pixels +
                  (size_t)((bg->top + y + cfg->scroll_y) * sc) *
                     bmp->width +
                  (size_t)((int32_t)bg->left) * sc;
            cfg->blend_mode = bg->blend_mode;
            cfg->brightness = bg->brightness;
            cfg->simple_copy = (uint8_t)(sc == 1 &&
                  bg->blend_mode == HD_BLEND_ALPHA &&
                  bg->brightness == 255);
         }
         else
         {
            cfg->min_x = -1;
            cfg->max_x = -1;
         }
      }
   }
}

static INLINE int hd_draw_background_layer(uint8_t cfg_index, uint32_t x,
      uint32_t y, uint32_t *out, uint32_t screen_width)
{
   const hd_bg_cfg *cfg = &hd_bg_cfgs[cfg_index];
   const uint32_t *png;
   uint32_t pixel;

   if ((int32_t)x < cfg->min_x || (int32_t)x > cfg->max_x)
      return 0;

   /* Fast path: scale 1, ALPHA blend, no brightness adjust. Most
    * backgrounds (full-screen backdrops, HUD overlays) are exactly
    * this, so specialise it to a single load + opaque store/alpha
    * blend with no inner scale loops and no blend-mode switch. */
   png = cfg->row + (size_t)((int32_t)x + cfg->scroll_x);
   if (cfg->simple_copy)
   {
      pixel = *png;
      if (pixel >= 0xFF000000u)
         *out = pixel;
      else if (pixel >= 0x01000000u)
         hd_blend_alpha((uint8_t*)out, (const uint8_t*)&pixel);
      return 1;
   }

   /* General path (scale > 1, non-alpha blend, or brightness). */
   {
      const hd_background *bg = &hd.bgs[cfg->priority][cfg->bg_index];
      hd_draw_custom_background(bg, out, x + cfg->scroll_x,
            y + cfg->scroll_y, screen_width, hd.scale);
   }
   return 1;
}

/* ---- tile lookup ------------------------------------------------------------ */

static hd_pack_tile *hd_get_matching_tile(uint32_t x, uint32_t y,
      hd_ppu_tile_info *tile, int *disable_cache)
{
   hd_tile_key key;
   int32_t first;

   memcpy(&key, tile, sizeof(hd_tile_key));
   first = hd_map_find(&hd.tile_map, &key);

   if (first < 0)
   {
      int32_t fallback = hd_get_fallback_tile(tile->tile_index);
      if (fallback >= 0)
      {
         int32_t org = key.tile_index;
         key.tile_index = fallback;
         first = hd_map_find(&hd.tile_map, &key);
         if (first < 0)
         {
            hd_tile_key dk = key;
            dk.palette_colors = 0xFFFFFFFFu;
            first = hd_map_find(&hd.default_map, &dk);
            if (first >= 0)
            {
               tile->tile_index = fallback;
               goto walk_default;
            }
            key.tile_index = org;
         }
         else
            tile->tile_index = fallback;
      }

      if (first < 0)
      {
         hd_tile_key dk = key;
         dk.palette_colors = 0xFFFFFFFFu;
         first = hd_map_find(&hd.default_map, &dk);
         if (first >= 0)
            goto walk_default;
      }
   }

   while (first >= 0)
   {
      hd_pack_tile *t = &hd.tiles[first];
      if (disable_cache && t->force_disable_cache)
         *disable_cache = 1;
      if (hd_tile_matches_conditions(t, (int)x, (int)y, tile))
      {
         if (t->need_init)
            hd_tile_init(t);
         return t;
      }
      first = t->next_in_key;
   }
   return NULL;

walk_default:
   while (first >= 0)
   {
      hd_pack_tile *t = &hd.tiles[first];
      if (disable_cache && t->force_disable_cache)
         *disable_cache = 1;
      if (hd_tile_matches_conditions(t, (int)x, (int)y, tile))
      {
         if (t->need_init)
            hd_tile_init(t);
         return t;
      }
      first = t->next_in_default;
   }
   return NULL;
}

static hd_pack_tile *hd_get_cached_matching_tile(uint32_t x, uint32_t y,
      hd_ppu_tile_info *tile)
{
   hd_pack_tile *result;
   int disable_cache = 0;

   if (((hd_scroll_x_line + (int32_t)x) & 0x07) == 0)
      hd_use_cached_tile = 0;

   if (hd_use_cached_tile)
      return hd_cached_tile;

   result = hd_get_matching_tile(x, y, tile, &disable_cache);
   if (!disable_cache && hd_cache_enabled)
   {
      hd_cached_tile = result;
      hd_use_cached_tile = 1;
   }
   return result;
}

/* ---- additional sprites ------------------------------------------------------ */

static int hd_add_map_grow(void)
{
   uint32_t new_cap = hd_add_map_cap ? hd_add_map_cap * 2 : 256;
   hd_add_entry *ne = (hd_add_entry*)calloc(new_cap, sizeof(hd_add_entry));
   uint32_t i;
   if (!ne)
      return 0;
   for (i = 0; i < hd_add_map_cap; i++)
   {
      if (hd_add_map[i].used)
      {
         uint32_t mask = new_cap - 1;
         uint32_t idx = hd_tile_key_hash(&hd_add_map[i].key) & mask;
         while (ne[idx].used)
            idx = (idx + 1) & mask;
         ne[idx] = hd_add_map[i];
      }
   }
   if (hd_add_map)
      free(hd_add_map);
   hd_add_map = ne;
   hd_add_map_cap = new_cap;
   return 1;
}

static int32_t hd_add_cache_lookup(const hd_ppu_tile_info *tile,
      int *found)
{
   hd_tile_key key;
   uint32_t mask, idx;
   *found = 0;
   if (!hd_add_map_cap)
      return -1;
   memcpy(&key, tile, sizeof(hd_tile_key));
   mask = hd_add_map_cap - 1;
   idx = hd_tile_key_hash(&key) & mask;
   for (;;)
   {
      hd_add_entry *e = &hd_add_map[idx];
      if (!e->used)
         return -1;
      if (hd_tile_key_equal(&e->key, &key))
      {
         *found = 1;
         return e->first;
      }
      idx = (idx + 1) & mask;
   }
}

static int32_t hd_add_cache_build(const hd_ppu_tile_info *tile,
      int check_fallback)
{
   hd_tile_key key;
   hd_tile_key def_key;
   int32_t first = -1;
   int32_t last = -1;
   uint32_t i;
   uint32_t mask, idx;

   memcpy(&key, tile, sizeof(hd_tile_key));
   def_key = key;
   def_key.palette_colors = 0xFFFFFFFFu;

   for (i = 0; i < hd.addition_count; i++)
   {
      const hd_addition *a = &hd.additions[i];
      int match = 0;
      if (!a->ignore_palette && hd_tile_key_equal(&key, &a->original))
         match = 1;
      else if (a->ignore_palette)
      {
         hd_tile_key ok = a->original;
         ok.palette_colors = 0xFFFFFFFFu;
         if (hd_tile_key_equal(&def_key, &ok))
            match = 1;
      }
      if (!match && check_fallback)
      {
         if (a->ignore_palette ||
               key.palette_colors == a->original.palette_colors)
         {
            if (hd_get_fallback_tile(key.tile_index) ==
                  a->original.tile_index)
               match = 1;
         }
      }
      if (match)
      {
         int32_t node;
         if (hd_add_node_count == hd_add_node_alloc)
         {
            uint32_t na = hd_add_node_alloc ? hd_add_node_alloc * 2 : 64;
            hd_add_node *nn = (hd_add_node*)realloc(hd_add_nodes,
                  na * sizeof(hd_add_node));
            if (!nn)
               break;
            hd_add_nodes = nn;
            hd_add_node_alloc = na;
         }
         node = (int32_t)hd_add_node_count++;
         hd_add_nodes[node].addition_idx = (uint16_t)i;
         hd_add_nodes[node].next = -1;
         if (last >= 0)
            hd_add_nodes[last].next = node;
         else
            first = node;
         last = node;
      }
   }

   /* Insert into the cache map (grow / reset when oversized). */
   if (hd_add_map_count * 2 >= hd_add_map_cap)
   {
      if (hd_add_map_cap >= 8192)
      {
         /* CHR RAM games can generate unbounded unique keys; reset the
          * cache instead of growing forever. */
         memset(hd_add_map, 0, hd_add_map_cap * sizeof(hd_add_entry));
         hd_add_map_count = 0;
      }
      else if (!hd_add_map_grow())
         return first;
   }
   mask = hd_add_map_cap - 1;
   idx = hd_tile_key_hash(&key) & mask;
   while (hd_add_map[idx].used)
      idx = (idx + 1) & mask;
   hd_add_map[idx].used = 1;
   hd_add_map[idx].key = key;
   hd_add_map[idx].first = first;
   hd_add_map_count++;

   return first;
}

static void hd_insert_additional_sprite(int32_t source_x, int32_t source_y,
      const hd_ppu_tile_info *sprite, const hd_addition *a)
{
   int32_t target_x = source_x +
         (sprite->hmirror ? -a->offset_x : a->offset_x);
   int32_t target_y = source_y +
         (sprite->vmirror ? -a->offset_y : a->offset_y);

   if (target_x >= 0 && target_x < HD_SCREEN_WIDTH &&
         target_y >= 0 && target_y < HD_SCREEN_HEIGHT)
   {
      hd_ppu_pixel_info *px =
            &hd_screen.pixels[(target_y << 8) | target_x];
      if (px->sprite_count < HD_MAX_SPRITES_PIXEL)
      {
         hd_ppu_tile_info *ns = &px->sprite[px->sprite_count];
         memcpy(ns, sprite, sizeof(hd_ppu_tile_info));
         ns->sprite_color_index = 0;
         ns->tile_index = a->additional.tile_index;
         ns->palette_colors = a->additional.palette_colors;
         memcpy(ns->tile_data, a->additional.tile_data, 16);
         px->sprite_count++;
      }
   }
}

static int hd_draw_additional_tiles(int32_t x, int32_t y,
      const hd_ppu_tile_info *tile, int check_fallback)
{
   int found = 0;
   int32_t node = hd_add_cache_lookup(tile, &found);
   int any = 0;
   if (!found)
      node = hd_add_cache_build(tile, check_fallback);
   while (node >= 0)
   {
      hd_insert_additional_sprite(x, y, tile,
            &hd.additions[hd_add_nodes[node].addition_idx]);
      any = 1;
      node = hd_add_nodes[node].next;
   }
   return any;
}

static void hd_process_additional_sprites(void)
{
   int check_fallback;
   uint32_t y_scroll;
   uint16_t tmp_vram;
   int process_bg_next_row = 1;
   int32_t x, y;

   if (!hd.addition_count)
      return;

   check_fallback = !hd.is_chr_ram_game && hd.fallback_map_count > 0;
   tmp_vram = hd_screen.line_tmp_vram_addr[0];
   y_scroll = (((tmp_vram & 0x3E0) >> 2) | ((tmp_vram & 0x7000) >> 12)) +
         ((tmp_vram & 0x800) ? 240 : 0);

   for (y = 0; y < HD_SCREEN_HEIGHT; y++)
   {
      uint16_t line_t = hd_screen.line_tmp_vram_addr[y];

      if (((y_scroll + y) & 0x07) == 0 || tmp_vram != line_t)
      {
         y_scroll = (((line_t & 0x3E0) >> 2) | ((line_t & 0x7000) >> 12)) +
               ((line_t & 0x800) ? 240 : 0);
         tmp_vram = line_t;
         process_bg_next_row = 1;
      }

      if (process_bg_next_row)
      {
         process_bg_next_row = 0;
         for (x = 0; x < HD_SCREEN_WIDTH; x++)
         {
            hd_ppu_pixel_info *px = &hd_screen.pixels[(y << 8) | x];
            int i;
            int count = px->sprite_count;
            for (i = 0; i < count; i++)
               hd_draw_additional_tiles(x, y, &px->sprite[i],
                     check_fallback);
            process_bg_next_row |= hd_draw_additional_tiles(x, y,
                  &px->tile, check_fallback);
         }
      }
      else
      {
         for (x = 0; x < HD_SCREEN_WIDTH; x++)
         {
            hd_ppu_pixel_info *px = &hd_screen.pixels[(y << 8) | x];
            int i;
            int count = px->sprite_count;
            for (i = 0; i < count; i++)
               hd_draw_additional_tiles(x, y, &px->sprite[i],
                     check_fallback);
         }
      }
   }
}

/* ---- per-pixel composition ------------------------------------------------- */

static void hd_get_pixels(uint32_t x, uint32_t y, hd_ppu_pixel_info *px,
      uint32_t *out, uint32_t screen_width)
{
   hd_pack_tile *hd_tile = NULL;
   hd_pack_tile *hd_sprite;
   int has_sprite = px->sprite_count > 0;
   int render_original =
         (hd.option_flags & HD_OPT_NO_ORIGINAL_TILES) == 0;
   int lowest_bg_sprite = 999;
   int has_background = 0;
   uint32_t scale = hd.scale;
   int i;
   int k;

   /* CHR-RAM tiles are keyed by tile data, not index, and the PPU
    * recorder legitimately leaves tile_index at HD_NO_TILE for them
    * (e.g. the partial tile in the leftmost fine-x-scrolled column).
    * Gating the lookup on tile_index != HD_NO_TILE therefore skipped
    * CHR-RAM matches for that column, leaving the raw SD tile visible
    * as a moving vertical strip. Match by data for CHR-RAM regardless
    * of index (Mesen looks up unconditionally by key). */
   if (px->tile.is_chr_ram || px->tile.tile_index != HD_NO_TILE)
      hd_tile = hd_get_cached_matching_tile(x, y, &px->tile);




   hd_draw_color(hd_palette[px->tile.ppu_bg_color], out, screen_width,
         scale);

   for (i = 0; i < hd_active_bg_count[0]; i++)
      has_background |= hd_draw_background_layer(
            (uint8_t)(0 * HD_PRIO_PER_LAYER + i),
            x, y, out, screen_width);

   if (has_sprite)
   {
      for (k = px->sprite_count - 1; k >= 0; k--)
      {
         if (px->sprite[k].bg_priority)
         {
            if (px->sprite[k].sprite_color_index != 0)
               lowest_bg_sprite = k;
            hd_sprite = hd_get_matching_tile(x, y, &px->sprite[k], NULL);
            if (hd_sprite)
               hd_draw_tile(&px->sprite[k], hd_sprite, out,
                     screen_width, scale);
            else if (px->sprite[k].sprite_color_index != 0)
               hd_draw_color(hd_palette[px->sprite[k].sprite_color],
                     out, screen_width, scale);
         }
      }
   }

   for (i = 0; i < hd_active_bg_count[1]; i++)
      has_background |= hd_draw_background_layer(
            (uint8_t)(1 * HD_PRIO_PER_LAYER + i),
            x, y, out, screen_width);

   if (hd_tile)
      hd_draw_tile(&px->tile, hd_tile, out, screen_width, scale);
   else if (render_original)
   {
      /* Mesen parity: without an active custom background at this
       * pixel the SD colour is drawn even for colour index 0 (the
       * backdrop) - this is what hides behind-background sprite
       * pixels where the BG is transparent. */
      if (!has_background || px->tile.bg_color_index != 0)
         hd_draw_color(hd_palette[px->tile.bg_color], out,
               screen_width, scale);
   }

   for (i = 0; i < hd_active_bg_count[2]; i++)
      hd_draw_background_layer((uint8_t)(2 * HD_PRIO_PER_LAYER + i),
            x, y, out, screen_width);

   if (has_sprite)
   {
      for (k = px->sprite_count - 1; k >= 0; k--)
      {
         if (!px->sprite[k].bg_priority && lowest_bg_sprite > k)
         {
            hd_sprite = hd_get_matching_tile(x, y, &px->sprite[k], NULL);
            if (hd_sprite)
               hd_draw_tile(&px->sprite[k], hd_sprite, out,
                     screen_width, scale);
            else if (px->sprite[k].sprite_color_index != 0)
               hd_draw_color(hd_palette[px->sprite[k].sprite_color],
                     out, screen_width, scale);
         }
      }
   }

   for (i = 0; i < hd_active_bg_count[3]; i++)
      hd_draw_background_layer((uint8_t)(3 * HD_PRIO_PER_LAYER + i),
            x, y, out, screen_width);
}

static void hd_process_grayscale_emphasis(int line, uint32_t *out,
      uint32_t hd_width)
{
   uint32_t scale = hd.scale;
   uint32_t x, y;

   if (hd_screen.line_grayscale[line])
   {
      uint32_t *p = out;
      for (y = 0; y < scale; y++)
      {
         for (x = 0; x < hd_width; x++)
         {
            uint32_t rgb = p[x];
            uint8_t average = (uint8_t)((((rgb >> 16) & 0xFF) +
                  ((rgb >> 8) & 0xFF) + (rgb & 0xFF)) / 3);
            p[x] = (rgb & 0xFF000000u) | ((uint32_t)average << 16) |
                  ((uint32_t)average << 8) | average;
         }
         p += hd_width;
      }
   }

   if (hd_screen.line_emphasis[line])
   {
      uint8_t emphasis = hd_screen.line_emphasis[line];
      double red = 1.0, green = 1.0, blue = 1.0;
      uint32_t *p = out;

      if (emphasis & 0x01)
      {
         red *= 1.1; green *= 0.9; blue *= 0.9;
      }
      if (emphasis & 0x02)
      {
         green *= 1.1; red *= 0.9; blue *= 0.9;
      }
      if (emphasis & 0x04)
      {
         blue *= 1.1; red *= 0.9; green *= 0.9;
      }

      for (y = 0; y < scale; y++)
      {
         for (x = 0; x < hd_width; x++)
         {
            uint32_t rgb = p[x];
            uint32_t r = (uint32_t)(((rgb >> 16) & 0xFF) * red);
            uint32_t g = (uint32_t)(((rgb >> 8) & 0xFF) * green);
            uint32_t b = (uint32_t)((rgb & 0xFF) * blue);
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            p[x] = 0xFF000000u | (r << 16) | (g << 8) | b;
         }
         p += hd_width;
      }
   }
}

const uint32_t *HDNes_ComposeFrame(void)
{

   uint32_t screen_width = HD_SCREEN_WIDTH * hd.scale;
   uint32_t i;
   int layer;
   int y;
   uint32_t x;

   if (!hdnes_active || !hd_frame_buf || !hd_screen.pixels)
      return NULL;

   /* per-frame condition cache reset */
   for (i = 0; i < hd.cond_count; i++)
      hd.conds[i].result_cache = -1;

   /* palette */
   if (hd.has_custom_palette)
      memcpy(hd_palette, hd.custom_palette, sizeof(hd_palette));
   else
      memcpy(hd_palette, hd_user_palette, sizeof(hd_palette));

   hd_cache_enabled = (hd.option_flags & HD_OPT_DISABLE_CACHE) == 0;
   hd_cached_tile = NULL;
   hd_use_cached_tile = 0;

   /* resolve active background layers */
   for (layer = 0; layer < 4; layer++)
   {
      uint32_t active = 0;
      int p;
      for (p = 0; p < HD_PRIO_PER_LAYER; p++)
      {
         uint8_t prio = (uint8_t)(layer * HD_PRIO_PER_LAYER + p);
         int32_t index = hd_get_layer_index(prio);
         if (index >= 0)
         {
            hd_bg_cfgs[layer * HD_PRIO_PER_LAYER + active].priority = prio;
            hd_bg_cfgs[layer * HD_PRIO_PER_LAYER + active].bg_index = index;
            active++;
         }
      }
      hd_active_bg_count[layer] = (uint8_t)active;
      for (; active < HD_PRIO_PER_LAYER; active++)
         hd_bg_cfgs[layer * HD_PRIO_PER_LAYER + active].bg_index = -1;
   }

   hd_process_additional_sprites();

   for (y = 0; y < HD_SCREEN_HEIGHT; y++)
   {
      uint32_t buffer_index = (uint32_t)y * screen_width * hd.scale;
      uint32_t line_start = buffer_index;

      hd_on_line_start(y);
      for (x = 0; x < HD_SCREEN_WIDTH; x++)
      {
         hd_get_pixels(x, (uint32_t)y,
               &hd_screen.pixels[(y << 8) | x],
               hd_frame_buf + buffer_index, screen_width);
         buffer_index += hd.scale;
      }

      hd_process_grayscale_emphasis(y, hd_frame_buf + line_start,
            screen_width);
   }

   return hd_frame_buf;
}
