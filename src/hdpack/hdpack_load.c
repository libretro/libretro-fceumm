/* Mesen HD pack loader: parses hires.txt and builds the in-memory pack
 * representation.  Faithful port of Mesen's HdPackLoader for format
 * versions <= 109, extended so <img>/<background> may reference .dds
 * and .webp images in addition to .png. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <compat/msvc.h>
#include <streams/file_stream.h>

#include "../fceu-types.h"
#include "../fceu.h"
#include "../driver.h"
#include "../cart.h"
#include "../ines.h"

#include "hdpack.h"
#include "hdpack_data.h"

hd_pack hd;
hd_screen_info hd_screen;
int hdnes_active = 0;

static int hd_current_line = 0;
static int hd_error_count = 0;

void hd_log(const char *fmt, ...)
{
   char buf[512];
   va_list ap;
   va_start(ap, fmt);
   vsnprintf(buf, sizeof(buf), fmt, ap);
   va_end(ap);
   FCEU_printf("[HDPack] %s\n", buf);
}

static void hd_parse_error(const char *msg)
{
   FCEU_printf("[HDPack - Line %d] %s\n", hd_current_line, msg);
   hd_error_count++;
}

/* ---- small helpers -------------------------------------------------- */

static int hd_hex_digit(char c)
{
   if (c >= '0' && c <= '9') return c - '0';
   if (c >= 'a' && c <= 'f') return c - 'a' + 10;
   if (c >= 'A' && c <= 'F') return c - 'A' + 10;
   return -1;
}

static uint32_t hd_from_hex(const char *s)
{
   uint32_t v = 0;
   while (*s)
   {
      int d = hd_hex_digit(*s);
      if (d < 0)
         break;
      v = (v << 4) | (uint32_t)d;
      s++;
   }
   return v;
}

static int hd_parse_bool(const char *s)
{
   if (strcmp(s, "Y") != 0 && strcmp(s, "N") != 0)
      hd_parse_error("Invalid boolean value");
   return strcmp(s, "Y") == 0;
}

/* Split a mutable string on commas.  Returns token count. */
#define HD_MAX_TOKENS 40
static int hd_split(char *s, char **tokens)
{
   int count = 0;
   char *p = s;
   if (!*p)
      return 0;
   tokens[count++] = p;
   while (*p && count < HD_MAX_TOKENS)
   {
      if (*p == ',')
      {
         *p = '\0';
         tokens[count++] = p + 1;
      }
      p++;
   }
   return count;
}

static void hd_rtrim(char *s)
{
   size_t len = strlen(s);
   while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' ||
         s[len - 1] == '\r' || s[len - 1] == '\n'))
      s[--len] = '\0';
}

/* hires.txt paths routinely use Windows separators
 * ("Backdrops\\file.png"); normalize to forward slashes, which every
 * supported platform accepts through the VFS layer. */
static void hd_normalize_path(char *p)
{
   while (*p)
   {
      if (*p == '\\')
         *p = '/';
      p++;
   }
}

/* All pack file access goes through libretro-common's filestream so
 * it is routed via the frontend's VFS interface when one is provided
 * (fceumm initialises it with filestream_vfs_init at startup). */
static int hd_load_file(const char *filename, uint8_t **data, size_t *size)
{
   char path[1600];
   RFILE *f;
   int64_t len;
   uint8_t *buf;

   snprintf(path, sizeof(path), "%s/%s", hd.pack_dir, filename);
   hd_normalize_path(path);
   f = filestream_open(path, RETRO_VFS_FILE_ACCESS_READ,
         RETRO_VFS_FILE_ACCESS_HINT_NONE);
   if (!f)
      return 0;
   len = filestream_get_size(f);
   if (len < 0)
   {
      filestream_close(f);
      return 0;
   }
   buf = (uint8_t*)malloc(len ? (size_t)len : 1);
   if (!buf)
   {
      filestream_close(f);
      return 0;
   }
   if (len > 0 && filestream_read(f, buf, len) != len)
   {
      free(buf);
      filestream_close(f);
      return 0;
   }
   filestream_close(f);
   *data = buf;
   *size = (size_t)len;
   return 1;
}

int hd_pack_read_file(const char *filename, uint8_t **data, size_t *size)
{
   return hd_load_file(filename, data, size);
}

static int hd_file_exists(const char *filename)
{
   char path[1600];
   snprintf(path, sizeof(path), "%s/%s", hd.pack_dir, filename);
   hd_normalize_path(path);
   return filestream_exists(path) ? 1 : 0;
}

/* ---- tile key hashing ------------------------------------------------ */

uint32_t hd_tile_key_hash(const hd_tile_key *key)
{
   if (key->is_chr_ram)
   {
      /* Rolling hash over palette_colors + tile_data, identical to
       * Mesen's HdTileKey::CalculateHash so packs behave the same. */
      uint32_t result = 0;
      uint8_t tmp[4 + 16];
      size_t i;
      memcpy(tmp, &key->palette_colors, 4);
      memcpy(tmp + 4, key->tile_data, 16);
      for (i = 0; i < sizeof(tmp); i += 4)
      {
         uint32_t chunk;
         memcpy(&chunk, tmp + i, 4);
         result += chunk;
         result = (result << 2) | (result >> 30);
      }
      return result;
   }
   return (uint32_t)key->tile_index ^ key->palette_colors;
}

int hd_tile_key_equal(const hd_tile_key *a, const hd_tile_key *b)
{
   if (a->is_chr_ram != b->is_chr_ram)
      return 0;
   if (a->is_chr_ram)
      return a->palette_colors == b->palette_colors &&
             memcmp(a->tile_data, b->tile_data, 16) == 0;
   return a->tile_index == b->tile_index &&
          a->palette_colors == b->palette_colors;
}

/* ---- tile hash map ---------------------------------------------------- */

static int hd_map_alloc(hd_tile_map *map, uint32_t min_entries)
{
   uint32_t cap = 16;
   while (cap < min_entries * 2)
      cap <<= 1;
   map->entries = (hd_map_entry*)calloc(cap, sizeof(hd_map_entry));
   if (!map->entries)
      return 0;
   map->capacity = cap;
   map->count = 0;
   return 1;
}

static void hd_map_free(hd_tile_map *map)
{
   if (map->entries)
      free(map->entries);
   map->entries = NULL;
   map->capacity = 0;
   map->count = 0;
}

int32_t hd_map_find(const hd_tile_map *map, const hd_tile_key *key)
{
   uint32_t mask;
   uint32_t idx;
   if (!map->entries)
      return -1;
   mask = map->capacity - 1;
   idx = hd_tile_key_hash(key) & mask;
   for (;;)
   {
      const hd_map_entry *e = &map->entries[idx];
      if (!e->used)
         return -1;
      if (hd_tile_key_equal(&e->key, key))
         return e->first;
      idx = (idx + 1) & mask;
   }
}

/* Insert tile `tile_idx` under `key`, appending to the chain tail so
 * definition order is preserved (Mesen matches conditions in the order
 * the <tile> tags appear). */
static void hd_map_insert(hd_tile_map *map, const hd_tile_key *key,
      int32_t tile_idx, int use_default_chain)
{
   uint32_t mask = map->capacity - 1;
   uint32_t idx = hd_tile_key_hash(key) & mask;
   for (;;)
   {
      hd_map_entry *e = &map->entries[idx];
      if (!e->used)
      {
         e->used = 1;
         e->key = *key;
         e->first = tile_idx;
         map->count++;
         return;
      }
      if (hd_tile_key_equal(&e->key, key))
      {
         int32_t cur = e->first;
         for (;;)
         {
            int32_t next = use_default_chain
               ? hd.tiles[cur].next_in_default
               : hd.tiles[cur].next_in_key;
            if (next < 0)
               break;
            cur = next;
         }
         if (use_default_chain)
            hd.tiles[cur].next_in_default = tile_idx;
         else
            hd.tiles[cur].next_in_key = tile_idx;
         return;
      }
      idx = (idx + 1) & mask;
   }
}

/* ---- fallback map ------------------------------------------------------ */

int32_t hd_get_fallback_tile(int32_t tile_index)
{
   uint32_t lo = 0;
   uint32_t hi = hd.fallback_map_count;
   while (lo < hi)
   {
      uint32_t mid = (lo + hi) >> 1;
      if (hd.fallback_map[mid].tile_index < tile_index)
         lo = mid + 1;
      else
         hi = mid;
   }
   if (lo < hd.fallback_map_count &&
         hd.fallback_map[lo].tile_index == tile_index)
      return hd.fallback_map[lo].fallback_index;
   return -1;
}

static int hd_fallback_cmp(const void *a, const void *b)
{
   const hd_fallback *fa = (const hd_fallback*)a;
   const hd_fallback *fb = (const hd_fallback*)b;
   if (fa->tile_index < fb->tile_index) return -1;
   if (fa->tile_index > fb->tile_index) return 1;
   return 0;
}

/* ---- growable arrays --------------------------------------------------- */

static hd_pack_tile *hd_new_tile(void)
{
   hd_pack_tile *t;
   if (hd.tile_count == hd.tile_alloc)
   {
      uint32_t na = hd.tile_alloc ? hd.tile_alloc * 2 : 64;
      hd_pack_tile *nt = (hd_pack_tile*)realloc(hd.tiles,
            na * sizeof(hd_pack_tile));
      if (!nt)
         return NULL;
      hd.tiles = nt;
      hd.tile_alloc = na;
   }
   t = &hd.tiles[hd.tile_count++];
   memset(t, 0, sizeof(*t));
   t->next_in_key = -1;
   t->next_in_default = -1;
   t->need_init = 1;
   return t;
}

static hd_condition *hd_new_condition(void)
{
   hd_condition *c;
   if (hd.cond_count == hd.cond_alloc)
   {
      uint32_t na = hd.cond_alloc ? hd.cond_alloc * 2 : 32;
      hd_condition *nc = (hd_condition*)realloc(hd.conds,
            na * sizeof(hd_condition));
      if (!nc)
         return NULL;
      hd.conds = nc;
      hd.cond_alloc = na;
   }
   c = &hd.conds[hd.cond_count++];
   memset(c, 0, sizeof(*c));
   c->result_cache = -1;
   c->tile_index = -1;
   c->watch_slot_a = -1;
   c->watch_slot_b = -1;
   return c;
}

static int32_t hd_find_condition(const char *name)
{
   uint32_t i;
   for (i = 0; i < hd.cond_count; i++)
   {
      if (strcmp(hd.conds[i].name, name) == 0)
         return (int32_t)i;
   }
   return -1;
}

static void hd_watch_add(uint32_t addr)
{
   uint32_t i;
   for (i = 0; i < hd.watch_count; i++)
   {
      if (hd.watch_addrs[i] == addr)
         return;
   }
   if (hd.watch_count < 64)
      hd.watch_addrs[hd.watch_count++] = addr;
   else
      hd_parse_error("Too many watched memory addresses (max 64)");
}

static int16_t hd_watch_slot(uint32_t addr)
{
   uint32_t i;
   for (i = 0; i < hd.watch_count; i++)
   {
      if (hd.watch_addrs[i] == addr)
         return (int16_t)i;
   }
   return -1;
}

/* ---- built-in / global conditions -------------------------------------- */

static void hd_add_global_condition(const char *name, int type,
      uint32_t operand_a)
{
   int neg;
   for (neg = 0; neg < 2; neg++)
   {
      hd_condition *c = hd_new_condition();
      if (!c)
         return;
      snprintf(c->name, sizeof(c->name), "%s%s", neg ? "!" : "", name);
      c->type = (uint8_t)type;
      c->negate = (uint8_t)neg;
      c->use_cache = 0;
      c->operand_a = operand_a;
   }
}

static void hd_init_global_conditions(void)
{
   hd_add_global_condition("hmirror", HD_COND_HMIRROR, 0);
   hd_add_global_condition("vmirror", HD_COND_VMIRROR, 0);
   hd_add_global_condition("bgpriority", HD_COND_BGPRIORITY, 0);
   if (hd.version >= 107)
   {
      hd_add_global_condition("sppalette0", HD_COND_SPRITE_PALETTE, 0);
      hd_add_global_condition("sppalette1", HD_COND_SPRITE_PALETTE, 1);
      hd_add_global_condition("sppalette2", HD_COND_SPRITE_PALETTE, 2);
      hd_add_global_condition("sppalette3", HD_COND_SPRITE_PALETTE, 3);
   }
}

/* ---- condition string on tag lines: [name&name2&...] -------------------- */

static uint16_t *hd_parse_condition_prefix(char *cond_str, uint16_t *count)
{
   uint16_t *list = NULL;
   uint16_t n = 0;
   char name[HD_COND_NAME_LEN];
   size_t name_len = 0;
   size_t i;
   size_t len = strlen(cond_str);

   for (i = 0; i <= len; i++)
   {
      char c = cond_str[i];
      if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
         continue;
      if (c == '&' || c == '\0')
      {
         if (name_len > 0)
         {
            int32_t idx;
            name[name_len] = '\0';
            idx = hd_find_condition(name);
            if (idx >= 0)
            {
               uint16_t *nl = (uint16_t*)realloc(list,
                     (n + 1) * sizeof(uint16_t));
               if (nl)
               {
                  list = nl;
                  list[n++] = (uint16_t)idx;
               }
            }
            else
            {
               char msg[128];
               snprintf(msg, sizeof(msg), "Condition not found: %s", name);
               hd_parse_error(msg);
            }
            name_len = 0;
         }
      }
      else if (name_len + 1 < sizeof(name))
         name[name_len++] = c;
   }

   *count = n;
   return list;
}

/* ---- tag processing ------------------------------------------------------ */

static int hd_stricmp(const char *a, const char *b)
{
   while (*a && *b)
   {
      char ca = *a;
      char cb = *b;
      if (ca >= 'A' && ca <= 'Z') ca = (char)(ca - 'A' + 'a');
      if (cb >= 'A' && cb <= 'Z') cb = (char)(cb - 'A' + 'a');
      if (ca != cb)
         return (int)(unsigned char)ca - (int)(unsigned char)cb;
      a++;
      b++;
   }
   return (int)(unsigned char)*a - (int)(unsigned char)*b;
}

static char *hd_strdup(const char *s)
{
   size_t len = strlen(s) + 1;
   char *p = (char*)malloc(len);
   if (p)
      memcpy(p, s, len);
   return p;
}

static const char *hd_path_ext(const char *name)
{
   const char *dot = strrchr(name, '.');
   return dot ? dot : "";
}

static int hd_ext_supported(const char *name)
{
   const char *ext = hd_path_ext(name);
   return hd_stricmp(ext, ".png") == 0 ||
          hd_stricmp(ext, ".dds") == 0 ||
          hd_stricmp(ext, ".webp") == 0;
}

/* <img>filename  -- failure aborts the pack (like Mesen). */
static int hd_process_img(const char *src)
{
   hd_bitmap *nb;
   hd_bitmap *bmp;

   if (!hd_ext_supported(src))
      hd_parse_error("Unrecognized image extension (expected .png/.dds/.webp)");

   nb = (hd_bitmap*)realloc(hd.bitmaps,
         (hd.bitmap_count + 1) * sizeof(hd_bitmap));
   if (!nb)
      return 0;
   hd.bitmaps = nb;
   bmp = &hd.bitmaps[hd.bitmap_count];
   memset(bmp, 0, sizeof(*bmp));

   if (!hd_load_file(src, &bmp->file_data, &bmp->file_size))
   {
      char msg[1200];
      snprintf(msg, sizeof(msg),
            "Error loading HDPack: image file %s could not be read.", src);
      hd_parse_error(msg);
      return 0;
   }
   bmp->name = hd_strdup(src);
   hd.bitmap_count++;
   return 1;
}

/* Shared with <tile>/<addition>/<condition>: reads either a 32-char hex
 * CHR RAM tile-data blob or a tile index, plus 8-char hex palette. */
static void hd_read_tile_data(hd_tile_key *key, const char *tile_data,
      const char *pal_data)
{
   if (strlen(tile_data) >= 32)
   {
      int i;
      for (i = 0; i < 16; i++)
      {
         char b[3];
         b[0] = tile_data[i * 2];
         b[1] = tile_data[i * 2 + 1];
         b[2] = '\0';
         key->tile_data[i] = (uint8_t)hd_from_hex(b);
      }
      key->tile_index = -1;
      key->is_chr_ram = 1;
   }
   else
   {
      if (hd.version <= 102)
         key->tile_index = atoi(tile_data);
      else
         key->tile_index = (int32_t)hd_from_hex(tile_data);
      key->is_chr_ram = 0;
   }
   key->palette_colors = hd_from_hex(pal_data);
}

static void hd_process_tile(char **tok, int ntok, uint16_t *cond_idx,
      uint16_t cond_count)
{
   hd_pack_tile *tile;
   int index = 0;
   uint16_t i;

   tile = hd_new_tile();
   if (!tile)
      return;

   if (hd.version < 100)
   {
      if (ntok < 7)
      {
         hd_parse_error("Tile tag requires more parameters");
         hd.tile_count--;
         return;
      }
      tile->key.tile_index = atoi(tok[index]); index++;
      tile->bitmap_index = (uint32_t)atoi(tok[index]); index++;
      tile->key.palette_colors = (uint32_t)atoi(tok[index + 2]) |
            ((uint32_t)atoi(tok[index + 1]) << 8) |
            ((uint32_t)atoi(tok[index]) << 16);
      index += 3;
   }
   else
   {
      if (ntok < 7)
      {
         hd_parse_error("Tile tag requires more parameters");
         hd.tile_count--;
         return;
      }
      tile->bitmap_index = (uint32_t)atoi(tok[index]); index++;
      hd_read_tile_data(&tile->key, tok[index], tok[index + 1]);
      index += 2;
   }

   tile->x = (uint32_t)atoi(tok[index]); index++;
   tile->y = (uint32_t)atoi(tok[index]); index++;

   tile->cond_count = cond_count;
   if (cond_count)
   {
      tile->cond_idx = (uint16_t*)malloc(cond_count * sizeof(uint16_t));
      if (tile->cond_idx)
         memcpy(tile->cond_idx, cond_idx, cond_count * sizeof(uint16_t));
      else
         tile->cond_count = 0;
   }

   tile->force_disable_cache = 0;
   for (i = 0; i < tile->cond_count; i++)
   {
      const hd_condition *c = &hd.conds[tile->cond_idx[i]];
      switch (c->type)
      {
         case HD_COND_SPRITE_NEARBY:
         case HD_COND_POS_CHECK_X:
         case HD_COND_POS_CHECK_Y:
         case HD_COND_ORIGIN_POS_CHECK_X:
         case HD_COND_ORIGIN_POS_CHECK_Y:
            tile->force_disable_cache = 1;
            break;
         case HD_COND_TILE_NEARBY:
            if ((c->tile_x % 8) != 0 || (c->tile_y % 8) != 0)
               tile->force_disable_cache = 1;
            break;
         default:
            break;
      }
   }

   if (hd.version >= 105)
      tile->brightness = (int32_t)(atof(tok[index]) * 255.0);
   else if (hd.version > 0)
      tile->brightness = (uint8_t)(atof(tok[index]) * 255.0);
   else
      tile->brightness = 255;
   if (hd.version > 0)
      index++;

   tile->default_tile = (uint8_t)hd_parse_bool(tok[index]); index++;

   /* For CHR ROM tiles the bank id is the 4K bank number. */
   tile->chr_bank_id = (uint32_t)(tile->key.tile_index / 256);

   if (hd.version < 100)
   {
      if (ntok >= 24)
      {
         int k;
         for (k = 0; k < 16; k++)
         {
            tile->key.tile_data[k] = (uint8_t)atoi(tok[index]);
            index++;
         }
         tile->key.is_chr_ram = 1;
      }
      else
         tile->key.is_chr_ram = 0;
   }
   else
   {
      if (tile->key.is_chr_ram && ntok > index)
      {
         tile->chr_bank_id = (uint32_t)strtoul(tok[index], NULL, 10);
         index++;
      }
      if (tile->key.is_chr_ram && ntok > index)
      {
         tile->key.tile_index = atoi(tok[index]);
         index++;
      }
   }

   if (ntok != index)
      hd_parse_error("Tile tag contains too many parameters");
   if (tile->bitmap_index >= hd.bitmap_count)
   {
      hd_parse_error("Invalid bitmap index");
      if (tile->cond_idx)
         free(tile->cond_idx);
      hd.tile_count--;
      return;
   }

   tile->width = 8 * hd.scale;
   tile->height = 8 * hd.scale;
}

static int8_t hd_parse_op(const char *s)
{
   if (strcmp(s, "==") == 0) return HD_OP_EQ;
   if (strcmp(s, "!=") == 0) return HD_OP_NE;
   if (strcmp(s, ">") == 0)  return HD_OP_GT;
   if (strcmp(s, "<") == 0)  return HD_OP_LT;
   if (strcmp(s, "<=") == 0) return HD_OP_LE;
   if (strcmp(s, ">=") == 0) return HD_OP_GE;
   return HD_OP_INVALID;
}

static void hd_process_condition(char **tok, int ntok, int inverted)
{
   hd_condition *c;
   int type = -1;
   int index = 2;

   if (ntok < 4)
   {
      hd_parse_error("Condition tag should contain at least 4 parameters");
      return;
   }
   if (tok[0][0] == '\0')
   {
      hd_parse_error("Condition name may not be empty");
      return;
   }
   if (strchr(tok[0], '!'))
   {
      hd_parse_error("Condition name may not contain '!' characters");
      return;
   }

   if (strcmp(tok[1], "tileAtPosition") == 0)
      type = HD_COND_TILE_AT_POS;
   else if (strcmp(tok[1], "tileNearby") == 0)
      type = HD_COND_TILE_NEARBY;
   else if (strcmp(tok[1], "spriteAtPosition") == 0)
      type = HD_COND_SPRITE_AT_POS;
   else if (strcmp(tok[1], "spriteNearby") == 0)
      type = HD_COND_SPRITE_NEARBY;
   else if (strcmp(tok[1], "memoryCheck") == 0 ||
            strcmp(tok[1], "ppuMemoryCheck") == 0)
      type = HD_COND_MEMORY_CHECK;
   else if (strcmp(tok[1], "memoryCheckConstant") == 0 ||
            strcmp(tok[1], "ppuMemoryCheckConstant") == 0)
      type = HD_COND_MEMORY_CHECK_CONST;
   else if (strcmp(tok[1], "frameRange") == 0)
      type = HD_COND_FRAME_RANGE;
   else if (strcmp(tok[1], "positionCheckX") == 0)
      type = HD_COND_POS_CHECK_X;
   else if (strcmp(tok[1], "positionCheckY") == 0)
      type = HD_COND_POS_CHECK_Y;
   else if (strcmp(tok[1], "originPositionCheckX") == 0)
      type = HD_COND_ORIGIN_POS_CHECK_X;
   else if (strcmp(tok[1], "originPositionCheckY") == 0)
      type = HD_COND_ORIGIN_POS_CHECK_Y;
   else
   {
      if (!inverted)
         hd_parse_error("Invalid condition type");
      return;
   }

   c = hd_new_condition();
   if (!c)
      return;
   c->type = (uint8_t)type;
   c->negate = (uint8_t)(inverted ? 1 : 0);

   {
      /* one byte shorter than c->name so the optional '!' prefix always
       * fits (also silences -Wformat-truncation) */
      char trimmed[HD_COND_NAME_LEN - 1];
      strncpy(trimmed, tok[0], sizeof(trimmed) - 1);
      trimmed[sizeof(trimmed) - 1] = '\0';
      hd_rtrim(trimmed);
      snprintf(c->name, sizeof(c->name), "%s%s", inverted ? "!" : "",
            trimmed);
   }

   switch (type)
   {
      case HD_COND_TILE_NEARBY:
      case HD_COND_TILE_AT_POS:
      case HD_COND_SPRITE_NEARBY:
      case HD_COND_SPRITE_AT_POS:
      {
         int x, y;
         const char *token;
         if (ntok < 6)
         {
            hd_parse_error("Condition tag should contain at least 6 parameters");
            hd.cond_count--;
            return;
         }
         c->use_cache = (type == HD_COND_TILE_AT_POS ||
               type == HD_COND_SPRITE_AT_POS) ? 1 : 0;

         x = atoi(tok[index]); index++;
         y = atoi(tok[index]); index++;
         token = tok[index]; index++;
         c->tile_x = x;
         c->tile_y = y;
         c->pixel_offset = (y * 256) + x;
         if (strlen(token) == 32)
         {
            int k;
            for (k = 0; k < 16; k++)
            {
               char b[3];
               b[0] = token[k * 2];
               b[1] = token[k * 2 + 1];
               b[2] = '\0';
               c->tile_data[k] = (uint8_t)hd_from_hex(b);
            }
            c->tile_index = -1;
         }
         else
         {
            if (hd.version < 104)
               c->tile_index = atoi(token);
            else
               c->tile_index = (int32_t)hd_from_hex(token);
         }
         c->palette_colors = hd_from_hex(tok[index]); index++;
         c->ignore_palette = 0;
         if (ntok >= 7)
         {
            if (hd.version >= 108)
            {
               c->ignore_palette = (uint8_t)hd_parse_bool(tok[index]);
               index++;
            }
            else
               hd_parse_error("Condition ignore palette requires version 108+");
         }
         break;
      }

      case HD_COND_MEMORY_CHECK:
      case HD_COND_MEMORY_CHECK_CONST:
      {
         int use_ppu_memory;
         uint32_t operand_a;
         uint32_t operand_b;
         uint32_t mask = 0xFF;
         int8_t op;

         if (hd.version < 101)
            hd_parse_error("This feature requires version 101+ of HD Packs");
         if (ntok < 5)
         {
            hd_parse_error("Condition tag should contain at least 5 parameters");
            hd.cond_count--;
            return;
         }
         c->use_cache = 1;
         use_ppu_memory = (strncmp(tok[1], "ppu", 3) == 0);
         operand_a = hd_from_hex(tok[index]); index++;
         if (use_ppu_memory)
         {
            if (operand_a > 0x3FFF)
            {
               hd_parse_error("Out of range memoryCheck operand");
               hd.cond_count--;
               return;
            }
            operand_a |= HD_PPU_MEM_MARKER;
         }
         else if (operand_a > 0xFFFF)
         {
            hd_parse_error("Out of range memoryCheck operand");
            hd.cond_count--;
            return;
         }

         op = hd_parse_op(tok[index]); index++;
         if (op == HD_OP_INVALID)
         {
            hd_parse_error("Invalid operator.");
            hd.cond_count--;
            return;
         }

         operand_b = hd_from_hex(tok[index]); index++;
         if (ntok > 5 && hd.version >= 103)
         {
            mask = hd_from_hex(tok[index]); index++;
         }

         if (type == HD_COND_MEMORY_CHECK)
         {
            if (use_ppu_memory)
            {
               if (operand_b > 0x3FFF)
               {
                  hd_parse_error("Out of range memoryCheck operand");
                  hd.cond_count--;
                  return;
               }
               operand_b |= HD_PPU_MEM_MARKER;
            }
            else if (operand_b > 0xFFFF)
            {
               hd_parse_error("Out of range memoryCheck operand");
               hd.cond_count--;
               return;
            }
            hd_watch_add(operand_b);
         }
         else if (operand_b > 0xFF)
         {
            hd_parse_error("Out of range memoryCheckConstant operand");
            hd.cond_count--;
            return;
         }

         hd_watch_add(operand_a);
         c->operand_a = operand_a;
         c->operand_b = operand_b;
         c->op = op;
         c->mask = (uint8_t)mask;
         break;
      }

      case HD_COND_POS_CHECK_X:
      case HD_COND_POS_CHECK_Y:
      case HD_COND_ORIGIN_POS_CHECK_X:
      case HD_COND_ORIGIN_POS_CHECK_Y:
      {
         int8_t op;
         uint32_t operand;
         if (hd.version < 108)
            hd_parse_error("This condition type requires version 108+ of HD Packs");
         c->use_cache = 0;
         op = hd_parse_op(tok[index]); index++;
         if (op == HD_OP_INVALID)
         {
            hd_parse_error("Invalid operator.");
            hd.cond_count--;
            return;
         }
         operand = (uint32_t)atoi(tok[index]); index++;
         if (operand > 0xFFFF)
         {
            hd_parse_error("Out of range positionCheck operand");
            hd.cond_count--;
            return;
         }
         c->op = op;
         c->operand_b = operand;
         break;
      }

      case HD_COND_FRAME_RANGE:
      {
         int32_t operand_a;
         int32_t operand_b;
         if (hd.version < 101)
            hd_parse_error("This condition type requires version 101+ of HD Packs");
         c->use_cache = 1;
         if (hd.version == 101)
         {
            operand_a = (int32_t)hd_from_hex(tok[index]); index++;
            operand_b = (int32_t)hd_from_hex(tok[index]); index++;
         }
         else
         {
            operand_a = atoi(tok[index]); index++;
            operand_b = atoi(tok[index]); index++;
         }
         if (operand_a < 0 || operand_a > 0xFFFF ||
             operand_b < 0 || operand_b > 0xFFFF)
         {
            hd_parse_error("Out of range frameRange operand");
            hd.cond_count--;
            return;
         }
         c->operand_a = (uint32_t)operand_a;
         c->operand_b = (uint32_t)operand_b;
         break;
      }
      default:
         break;
   }
}

static int32_t hd_find_bg_bitmap(const char *name)
{
   uint32_t i;
   for (i = 0; i < hd.bg_bitmap_count; i++)
   {
      if (strcmp(hd.bg_bitmaps[i].name, name) == 0)
         return (int32_t)i;
   }
   return -1;
}

static void hd_process_background(char **tok, int ntok, uint16_t *cond_idx,
      uint16_t cond_count)
{
   int32_t bmp_idx;
   hd_background bg;
   hd_background *nb;
   uint16_t i;
   uint8_t prio;

   if (ntok < 2)
   {
      hd_parse_error("Background tag should contain at least 2 parameters");
      return;
   }

   if (!hd_ext_supported(tok[0]))
      hd_parse_error("Unrecognized background image extension");

   bmp_idx = hd_find_bg_bitmap(tok[0]);
   if (bmp_idx < 0)
   {
      hd_bitmap *nbmp = (hd_bitmap*)realloc(hd.bg_bitmaps,
            (hd.bg_bitmap_count + 1) * sizeof(hd_bitmap));
      hd_bitmap *bmp;
      if (!nbmp)
         return;
      hd.bg_bitmaps = nbmp;
      bmp = &hd.bg_bitmaps[hd.bg_bitmap_count];
      memset(bmp, 0, sizeof(*bmp));
      if (!hd_load_file(tok[0], &bmp->file_data, &bmp->file_size))
      {
         char msg[1200];
         snprintf(msg, sizeof(msg), "Error while loading background: %s",
               tok[0]);
         hd_parse_error(msg);
         return;
      }
      bmp->name = hd_strdup(tok[0]);
      bmp_idx = (int32_t)hd.bg_bitmap_count;
      hd.bg_bitmap_count++;
   }

   memset(&bg, 0, sizeof(bg));
   bg.bitmap_index = (uint32_t)bmp_idx;
   if (hd.version >= 105)
      bg.brightness = (int32_t)(atof(tok[1]) * 255.0);
   else
      bg.brightness = (uint8_t)(atof(tok[1]) * 255.0);
   bg.h_scroll_ratio = 0;
   bg.v_scroll_ratio = 0;
   bg.priority = 10;
   bg.left = 0;
   bg.top = 0;
   bg.blend_mode = HD_BLEND_ALPHA;

   if (cond_count)
   {
      bg.cond_idx = (uint16_t*)malloc(cond_count * sizeof(uint16_t));
      bg.cond_count = 0;
      for (i = 0; i < cond_count && bg.cond_idx; i++)
      {
         switch (hd.conds[cond_idx[i]].type)
         {
            case HD_COND_TILE_AT_POS:
            case HD_COND_SPRITE_AT_POS:
            case HD_COND_MEMORY_CHECK:
            case HD_COND_MEMORY_CHECK_CONST:
            case HD_COND_FRAME_RANGE:
               bg.cond_idx[bg.cond_count++] = cond_idx[i];
               break;
            default:
               hd_parse_error("Invalid condition type for background");
               break;
         }
      }
   }

   if (ntok > 2)
   {
      if (hd.version < 101)
         hd_parse_error("This feature requires version 101+ of HD Packs");
      bg.h_scroll_ratio = (float)atof(tok[2]);
      if (ntok > 3)
         bg.v_scroll_ratio = (float)atof(tok[3]);
      if (ntok > 4)
      {
         if (hd.version < 102)
            hd_parse_error("This feature requires version 102+ of HD Packs");
         if (hd.version >= 106)
         {
            int p = atoi(tok[4]);
            if (p < 0 || p >= HD_BG_LAYER_COUNT)
            {
               hd_parse_error("Invalid background priority value");
               p = 10;
            }
            bg.priority = (uint8_t)p;
         }
         else
            bg.priority = hd_parse_bool(tok[4]) ? 0 : 10;
      }
      if (ntok > 6)
      {
         int l, t;
         if (hd.version < 105)
            hd_parse_error("This feature requires version 105+ of HD Packs");
         l = atoi(tok[5]);
         t = atoi(tok[6]);
         bg.left = (uint32_t)(l < 0 ? 0 : l);
         bg.top = (uint32_t)(t < 0 ? 0 : t);
      }
      if (ntok > 7)
      {
         if (hd.version < 107)
            hd_parse_error("Background blend mode requires version 107+");
         if (strcmp(tok[7], "Alpha") == 0)
            bg.blend_mode = HD_BLEND_ALPHA;
         else if (strcmp(tok[7], "Add") == 0)
            bg.blend_mode = HD_BLEND_ADD;
         else if (strcmp(tok[7], "Subtract") == 0)
            bg.blend_mode = HD_BLEND_SUBTRACT;
         else
            hd_parse_error("Invalid blend mode");
      }
   }

   prio = bg.priority;
   nb = (hd_background*)realloc(hd.bgs[prio],
         (hd.bg_count[prio] + 1) * sizeof(hd_background));
   if (!nb)
   {
      if (bg.cond_idx)
         free(bg.cond_idx);
      return;
   }
   hd.bgs[prio] = nb;
   hd.bgs[prio][hd.bg_count[prio]++] = bg;
}

static void hd_process_addition(char **tok, int ntok)
{
   hd_addition *na;
   hd_addition *a;

   if (hd.version < 107)
   {
      hd_parse_error("Addition tag requires version 107+ of HD Packs");
      return;
   }
   if (ntok < 6)
   {
      hd_parse_error("Addition tag should contain at least 6 parameters");
      return;
   }

   na = (hd_addition*)realloc(hd.additions,
         (hd.addition_count + 1) * sizeof(hd_addition));
   if (!na)
      return;
   hd.additions = na;
   a = &hd.additions[hd.addition_count++];
   memset(a, 0, sizeof(*a));

   hd_read_tile_data(&a->original, tok[0], tok[1]);
   a->offset_x = atoi(tok[2]);
   a->offset_y = atoi(tok[3]);
   hd_read_tile_data(&a->additional, tok[4], tok[5]);
   a->ignore_palette = 0;
   if (ntok >= 7)
   {
      if (hd.version >= 108)
         a->ignore_palette = (uint8_t)hd_parse_bool(tok[6]);
      else
         hd_parse_error("Addition ignore palette requires version 108+");
   }
}

static void hd_process_fallback(char **tok, int ntok)
{
   hd_fallback *nf;
   if (hd.version < 107)
   {
      hd_parse_error("Fallback tag requires version 107+ of HD Packs");
      return;
   }
   if (ntok < 2)
   {
      hd_parse_error("Fallback tag should contain at least 2 parameters");
      return;
   }
   nf = (hd_fallback*)realloc(hd.fallbacks,
         (hd.fallback_count + 1) * sizeof(hd_fallback));
   if (!nf)
      return;
   hd.fallbacks = nf;
   hd.fallbacks[hd.fallback_count].tile_index = (int32_t)hd_from_hex(tok[0]);
   hd.fallbacks[hd.fallback_count].fallback_index =
         (int32_t)hd_from_hex(tok[1]);
   hd.fallback_count++;
}

static int hd_process_sound_track(const char *album_s, const char *track_s,
      const char *filename)
{
   int album = atoi(album_s);
   int track = atoi(track_s);
   if (album < 0 || album > 255)
   {
      hd_parse_error("Invalid album value");
      return -1;
   }
   if (track < 0 || track > 255)
   {
      hd_parse_error("Invalid track value");
      return -1;
   }
   if (!hd_file_exists(filename))
   {
      char msg[1200];
      snprintf(msg, sizeof(msg), "Audio file not found: %s", filename);
      hd_parse_error(msg);
      return -1;
   }
   return album * 256 + track;
}

static char *hd_full_path(const char *filename)
{
   size_t len = strlen(hd.pack_dir) + strlen(filename) + 2;
   char *p = (char*)malloc(len);
   if (p)
   {
      snprintf(p, len, "%s/%s", hd.pack_dir, filename);
      hd_normalize_path(p);
   }
   return p;
}

static void hd_process_bgm(char **tok, int ntok)
{
   int track_id;
   if (ntok < 3)
   {
      hd_parse_error("BGM tag should contain at least 3 parameters");
      return;
   }
   track_id = hd_process_sound_track(tok[0], tok[1], tok[2]);
   if (track_id >= 0)
   {
      hd_track *nt = (hd_track*)realloc(hd.bgm,
            (hd.bgm_count + 1) * sizeof(hd_track));
      if (!nt)
         return;
      hd.bgm = nt;
      hd.bgm[hd.bgm_count].id = track_id;
      hd.bgm[hd.bgm_count].path = hd_full_path(tok[2]);
      hd.bgm[hd.bgm_count].loop_position =
            (ntok > 3) ? (uint32_t)strtoul(tok[3], NULL, 10) : 0;
      hd.bgm_count++;
   }
}

static void hd_process_sfx(char **tok, int ntok)
{
   int track_id;
   if (ntok < 3)
   {
      hd_parse_error("SFX tag should contain at least 3 parameters");
      return;
   }
   track_id = hd_process_sound_track(tok[0], tok[1], tok[2]);
   if (track_id >= 0)
   {
      hd_track *nt = (hd_track*)realloc(hd.sfx,
            (hd.sfx_count + 1) * sizeof(hd_track));
      if (!nt)
         return;
      hd.sfx = nt;
      hd.sfx[hd.sfx_count].id = track_id;
      hd.sfx[hd.sfx_count].path = hd_full_path(tok[2]);
      hd.sfx[hd.sfx_count].loop_position = 0;
      hd.sfx_count++;
   }
}

static void hd_process_patch(char **tok, int ntok)
{
   hd_patch *np;
   hd_patch *p;
   const char *sha;
   int i;

   if (ntok < 2)
   {
      hd_parse_error("Patch tag requires 2 parameters");
      return;
   }
   sha = tok[1];
   if (strlen(sha) != 40)
   {
      hd_parse_error("Invalid SHA1 hash for patch");
      return;
   }
   if (!hd_file_exists(tok[0]))
   {
      char msg[1200];
      snprintf(msg, sizeof(msg), "Patch file not found: %s", tok[0]);
      hd_parse_error(msg);
      return;
   }

   np = (hd_patch*)realloc(hd.patches,
         (hd.patch_count + 1) * sizeof(hd_patch));
   if (!np)
      return;
   hd.patches = np;
   p = &hd.patches[hd.patch_count];
   p->filename = hd_strdup(tok[0]);
   if (!p->filename)
      return;
   for (i = 0; i < 20; i++)
   {
      int hi = hd_hex_digit(sha[i * 2]);
      int lo = hd_hex_digit(sha[i * 2 + 1]);
      if (hi < 0 || lo < 0)
      {
         hd_parse_error("Invalid SHA1 hash for patch");
         free(p->filename);
         return;
      }
      p->sha1[i] = (uint8_t)((hi << 4) | lo);
   }
   hd.patch_count++;
}

static void hd_process_options(char **tok, int ntok)
{
   int i;
   for (i = 0; i < ntok; i++)
   {
      if (strcmp(tok[i], "disableSpriteLimit") == 0)
         hd.option_flags |= HD_OPT_NO_SPRITE_LIMIT;
      else if (strcmp(tok[i], "alternateRegisterRange") == 0)
         hd.option_flags |= HD_OPT_ALT_REGISTER_RANGE;
      else if (strcmp(tok[i], "disableCache") == 0)
         hd.option_flags |= HD_OPT_DISABLE_CACHE;
      else if (strcmp(tok[i], "disableOriginalTiles") == 0)
         hd.option_flags |= HD_OPT_NO_ORIGINAL_TILES;
      else if (strcmp(tok[i], "automaticFallbackTiles") == 0)
         hd.option_flags |= HD_OPT_AUTO_FALLBACK_TILES;
      else if (strcmp(tok[i], "disableContours") == 0)
         { /* removed feature; silently accepted like Mesen */ }
      else
         hd_parse_error("Invalid option");
   }
}

static void hd_load_custom_palette(void)
{
   uint8_t *data = NULL;
   size_t size = 0;
   if (hd_load_file("palette.dat", &data, &size))
   {
      if (size / 3 == 0x40)
      {
         int i;
         for (i = 0; i < 0x40; i++)
         {
            hd.custom_palette[i] = 0xFF000000u |
                  ((uint32_t)data[i * 3] << 16) |
                  ((uint32_t)data[i * 3 + 1] << 8) |
                  (uint32_t)data[i * 3 + 2];
         }
         hd.has_custom_palette = 1;
      }
      free(data);
   }
}

static int hd_build_tile_maps(void)
{
   uint32_t i;
   if (!hd_map_alloc(&hd.tile_map, hd.tile_count ? hd.tile_count : 1))
      return 0;
   if (!hd_map_alloc(&hd.default_map, hd.tile_count ? hd.tile_count : 1))
      return 0;
   for (i = 0; i < hd.tile_count; i++)
   {
      hd_pack_tile *t = &hd.tiles[i];
      hd_map_insert(&hd.tile_map, &t->key, (int32_t)i, 0);
      if (t->default_tile)
      {
         hd_tile_key dk = t->key;
         dk.palette_colors = 0xFFFFFFFFu;
         hd_map_insert(&hd.default_map, &dk, (int32_t)i, 1);
      }
   }
   return 1;
}

/* ---- main parse loop ------------------------------------------------------ */

static int hd_parse_definition(uint8_t *def, size_t len)
{
   size_t pos = 0;
   char line[4096];

   hd_init_global_conditions();

   while (pos < len)
   {
      size_t start = pos;
      size_t line_len;
      uint16_t *cond_idx = NULL;
      uint16_t cond_count = 0;
      char *content;
      char *tok[HD_MAX_TOKENS];
      int ntok;

      while (pos < len && def[pos] != '\n')
         pos++;
      line_len = pos - start;
      if (pos < len)
         pos++;
      hd_current_line++;

      if (line_len == 0)
         continue;
      if (line_len >= sizeof(line))
         line_len = sizeof(line) - 1;
      memcpy(line, def + start, line_len);
      line[line_len] = '\0';
      hd_rtrim(line);
      if (line[0] == '\0')
         continue;

      content = line;
      if (content[0] == '[')
      {
         char *end = strchr(content + 1, ']');
         if (!end)
         {
            hd_parse_error("Invalid condition tag");
            continue;
         }
         *end = '\0';
         cond_idx = hd_parse_condition_prefix(content + 1, &cond_count);
         content = end + 1;
      }

      if (strncmp(content, "<tile>", 6) == 0)
      {
         ntok = hd_split(content + 6, tok);
         hd_process_tile(tok, ntok, cond_idx, cond_count);
      }
      else if (strncmp(content, "<background>", 12) == 0)
      {
         ntok = hd_split(content + 12, tok);
         hd_process_background(tok, ntok, cond_idx, cond_count);
      }
      else if (strncmp(content, "<condition>", 11) == 0)
      {
         char copy[4096];
         strncpy(copy, content + 11, sizeof(copy) - 1);
         copy[sizeof(copy) - 1] = '\0';
         ntok = hd_split(content + 11, tok);
         hd_process_condition(tok, ntok, 0);
         ntok = hd_split(copy, tok);
         hd_process_condition(tok, ntok, 1);
      }
      else if (strncmp(content, "<img>", 5) == 0)
      {
         if (!hd_process_img(content + 5))
         {
            if (cond_idx)
               free(cond_idx);
            return 0;
         }
      }
      else if (strncmp(content, "<addition>", 10) == 0)
      {
         ntok = hd_split(content + 10, tok);
         hd_process_addition(tok, ntok);
      }
      else if (strncmp(content, "<fallback>", 10) == 0)
      {
         ntok = hd_split(content + 10, tok);
         hd_process_fallback(tok, ntok);
      }
      else if (strncmp(content, "<bgm>", 5) == 0)
      {
         ntok = hd_split(content + 5, tok);
         hd_process_bgm(tok, ntok);
      }
      else if (strncmp(content, "<sfx>", 5) == 0)
      {
         ntok = hd_split(content + 5, tok);
         hd_process_sfx(tok, ntok);
      }
      else if (strncmp(content, "<ver>", 5) == 0)
      {
         hd.version = (uint32_t)atoi(content + 5);
         if (hd.version > HD_CURRENT_VERSION)
         {
            hd_parse_error("This HD Pack was built with a more recent "
                  "version of Mesen than this core supports.");
            if (cond_idx)
               free(cond_idx);
            return 0;
         }
      }
      else if (strncmp(content, "<scale>", 7) == 0)
      {
         hd.scale = (uint32_t)atoi(content + 7);
         if (hd.scale > HD_MAX_SCALE || hd.scale == 0)
         {
            hd_parse_error("Scale ratios higher than 10 are not supported.");
            if (cond_idx)
               free(cond_idx);
            return 0;
         }
      }
      else if (strncmp(content, "<overscan>", 10) == 0)
      {
         ntok = hd_split(content + 10, tok);
         if (ntok >= 4)
         {
            hd.overscan[0] = (uint16_t)atoi(tok[0]);
            hd.overscan[1] = (uint16_t)atoi(tok[1]);
            hd.overscan[2] = (uint16_t)atoi(tok[2]);
            hd.overscan[3] = (uint16_t)atoi(tok[3]);
            hd.has_overscan = 1;
         }
      }
      else if (strncmp(content, "<patch>", 7) == 0)
      {
         ntok = hd_split(content + 7, tok);
         hd_process_patch(tok, ntok);
      }
      else if (strncmp(content, "<options>", 9) == 0)
      {
         ntok = hd_split(content + 9, tok);
         hd_process_options(tok, ntok);
      }

      if (cond_idx)
         free(cond_idx);
   }

   hd_load_custom_palette();
   if (!hd_build_tile_maps())
      return 0;

   if (hd_error_count > 0)
      hd_log("Loaded with %d errors", hd_error_count);

   return 1;
}

/* ---- public API ------------------------------------------------------------ */

static void hd_reset_data(void)
{
   memset(&hd, 0, sizeof(hd));
   hd.scale = 1;
   memset(&hd_screen, 0, sizeof(hd_screen));
   hd_current_line = 0;
   hd_error_count = 0;
}

uint32_t HDNes_GetScale(void)
{
   return hdnes_active ? hd.scale : 1;
}

int HDNes_HasAudio(void)
{
   return hdnes_active && (hd.bgm_count > 0 || hd.sfx_count > 0);
}

static int hd_pack_loaded = 0;

int HDNes_PackLoaded(void)
{
   return hd_pack_loaded;
}

int HDNes_LoadPack(const char *system_dir, const char *rom_path)
{
   const char *base;
   const char *dot;
   char rom_name[512];
   size_t name_len;
   uint8_t *def = NULL;
   size_t def_size = 0;
   int ok;

   HDNes_Unload();
   hd_reset_data();

   if (!system_dir || !rom_path || !*rom_path)
      return 0;

   /* base name of the ROM without extension */
   base = strrchr(rom_path, '/');
   {
      const char *base2 = strrchr(rom_path, '\\');
      if (base2 && (!base || base2 > base))
         base = base2;
   }
   base = base ? base + 1 : rom_path;
   dot = strrchr(base, '.');
   name_len = dot ? (size_t)(dot - base) : strlen(base);
   if (name_len >= sizeof(rom_name))
      name_len = sizeof(rom_name) - 1;
   memcpy(rom_name, base, name_len);
   rom_name[name_len] = '\0';

   snprintf(hd.pack_dir, sizeof(hd.pack_dir), "%s/HdPacks/%s",
         system_dir, rom_name);

   if (!hd_load_file("hires.txt", &def, &def_size))
      return 0;

   hd_log("Loading HD pack: %s/hires.txt", hd.pack_dir);
   ok = hd_parse_definition(def, def_size);
   free(def);

   if (!ok)
   {
      hd_log("HD pack failed to load.");
      HDNes_Unload();
      return 0;
   }

   hd_log("HD pack loaded: scale %ux, %u tiles, %u conditions, "
         "%u bgm, %u sfx", hd.scale, hd.tile_count, hd.cond_count,
         hd.bgm_count, hd.sfx_count);
   hd_pack_loaded = 1;
   return 1;
}

void HDNes_PostLoadInit(void)
{
   uint32_t i;
   uint32_t n = 0;

   hd.is_chr_ram_game = (VROM == NULL || VROM_size == 0) ? 1 : 0;

   /* Drop <addition> entries whose tile type does not match the game's
    * CHR configuration (Mesen CleanupInvalidRules). */
   for (i = 0; i < hd.addition_count; i++)
   {
      if (hd.additions[i].additional.is_chr_ram == hd.is_chr_ram_game)
         hd.additions[n++] = hd.additions[i];
   }
   hd.addition_count = n;

   /* Build the runtime fallback map: automatic fallbacks (identical CHR
    * ROM tiles mapping onto tiles the pack defines) plus the explicit
    * <fallback> entries, which take precedence. */
   if (hd.fallback_map)
   {
      free(hd.fallback_map);
      hd.fallback_map = NULL;
   }
   hd.fallback_map_count = 0;

   if (!hd.is_chr_ram_game)
   {
      uint32_t alloc = hd.fallback_count + 64;
      hd_fallback *map = (hd_fallback*)malloc(alloc * sizeof(hd_fallback));
      uint32_t count = 0;

      if (map && (hd.option_flags & HD_OPT_AUTO_FALLBACK_TILES))
      {
         int32_t chr_bytes = (int32_t)(VROM_size * 8192);
         int32_t tile_count = chr_bytes / 16;
         int32_t t;
         static const uint8_t blank[16] = {0};

         for (t = 0; t < tile_count; t++)
         {
            const uint8_t *td = VROM + t * 16;
            uint32_t j;
            if (memcmp(td, blank, 16) == 0)
               continue;
            for (j = 0; j < hd.tile_count; j++)
            {
               int32_t ti = hd.tiles[j].key.tile_index;
               if (ti < 0 || ti == t || (uint32_t)(ti * 16 + 16) >
                     (uint32_t)chr_bytes)
                  continue;
               if (memcmp(VROM + ti * 16, td, 16) == 0)
               {
                  if (count == alloc)
                  {
                     hd_fallback *nm = (hd_fallback*)realloc(map,
                           alloc * 2 * sizeof(hd_fallback));
                     if (!nm)
                        break;
                     map = nm;
                     alloc *= 2;
                  }
                  map[count].tile_index = t;
                  map[count].fallback_index = ti;
                  count++;
                  break;
               }
            }
         }
      }

      if (map)
      {
         for (i = 0; i < hd.fallback_count; i++)
         {
            uint32_t j;
            int found = 0;
            for (j = 0; j < count; j++)
            {
               if (map[j].tile_index == hd.fallbacks[i].tile_index)
               {
                  map[j].fallback_index = hd.fallbacks[i].fallback_index;
                  found = 1;
                  break;
               }
            }
            if (!found)
            {
               if (count == alloc)
               {
                  hd_fallback *nm = (hd_fallback*)realloc(map,
                        alloc * 2 * sizeof(hd_fallback));
                  if (!nm)
                     break;
                  map = nm;
                  alloc *= 2;
               }
               map[count++] = hd.fallbacks[i];
            }
         }
         qsort(map, count, sizeof(hd_fallback), hd_fallback_cmp);
         hd.fallback_map = map;
         hd.fallback_map_count = count;
      }
   }

   /* Resolve memory-condition operands to watch slots. */
   for (i = 0; i < hd.cond_count; i++)
   {
      hd_condition *c = &hd.conds[i];
      if (c->type == HD_COND_MEMORY_CHECK ||
            c->type == HD_COND_MEMORY_CHECK_CONST)
      {
         c->watch_slot_a = hd_watch_slot(c->operand_a);
         if (c->type == HD_COND_MEMORY_CHECK)
            c->watch_slot_b = hd_watch_slot(c->operand_b);
      }
   }

   if (!hd_ppu_init() || !hd_render_init())
   {
      hd_log("Out of memory allocating HD buffers; disabling pack.");
      HDNes_Unload();
      return;
   }

   if (hd.option_flags & HD_OPT_NO_SPRITE_LIMIT)
      FCEUI_DisableSpriteLimitation(1);

   hd_audio_init();
   hdnes_active = 1;
}

void HDNes_Unload(void)
{
   uint32_t i;

   hd_pack_loaded = 0;

   hdnes_active = 0;
   hd_audio_free();
   hd_render_free();
   hd_ppu_free();

   for (i = 0; i < hd.bitmap_count; i++)
      hd_bitmap_free(&hd.bitmaps[i]);
   if (hd.bitmaps)
      free(hd.bitmaps);
   for (i = 0; i < hd.bg_bitmap_count; i++)
      hd_bitmap_free(&hd.bg_bitmaps[i]);
   if (hd.bg_bitmaps)
      free(hd.bg_bitmaps);

   for (i = 0; i < hd.tile_count; i++)
   {
      if (hd.tiles[i].cond_idx)
         free(hd.tiles[i].cond_idx);
      if (hd.tiles[i].hd_data)
         free(hd.tiles[i].hd_data);
   }
   if (hd.tiles)
      free(hd.tiles);
   if (hd.conds)
      free(hd.conds);

   for (i = 0; i < HD_BG_LAYER_COUNT; i++)
   {
      uint32_t j;
      for (j = 0; j < hd.bg_count[i]; j++)
      {
         if (hd.bgs[i][j].cond_idx)
            free(hd.bgs[i][j].cond_idx);
      }
      if (hd.bgs[i])
         free(hd.bgs[i]);
   }

   if (hd.additions)
      free(hd.additions);
   if (hd.fallbacks)
      free(hd.fallbacks);
   if (hd.fallback_map)
      free(hd.fallback_map);

   for (i = 0; i < hd.bgm_count; i++)
      free(hd.bgm[i].path);
   if (hd.bgm)
      free(hd.bgm);
   for (i = 0; i < hd.sfx_count; i++)
      free(hd.sfx[i].path);
   if (hd.sfx)
      free(hd.sfx);

   for (i = 0; i < hd.patch_count; i++)
      free(hd.patches[i].filename);
   if (hd.patches)
      free(hd.patches);

   hd_map_free(&hd.tile_map);
   hd_map_free(&hd.default_map);

   if (hd_screen.pixels)
      free(hd_screen.pixels);

   memset(&hd, 0, sizeof(hd));
   memset(&hd_screen, 0, sizeof(hd_screen));
   hd.scale = 1;
}
