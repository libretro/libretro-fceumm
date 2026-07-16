/* Internal data structures for the Mesen HD pack implementation.
 * Shared between hdpack_load.c / hdpack_ppu.c / hdpack_render.c /
 * hdpack_image.c / hdpack_audio.c.  Not for use outside src/hdpack. */

#ifndef _FCEU_HDPACK_DATA_H
#define _FCEU_HDPACK_DATA_H

#include <stdint.h>
#include <stddef.h>

#define HD_CURRENT_VERSION    109
#define HD_MAX_SCALE          10
#define HD_SCREEN_WIDTH       256
#define HD_SCREEN_HEIGHT      240
#define HD_PIXEL_COUNT        (HD_SCREEN_WIDTH * HD_SCREEN_HEIGHT)
#define HD_NO_TILE            (-1)
#define HD_BG_LAYER_COUNT     40
#define HD_PRIO_PER_LAYER     10
#define HD_MAX_SPRITES_PIXEL  4
#define HD_MAX_LINE_SPRITES   64
#define HD_BG_FETCH_SLOTS     40
#define HD_BG_GROUPS          34

/* <options> flags (Mesen HdPackOptions) */
#define HD_OPT_NO_SPRITE_LIMIT       1
#define HD_OPT_ALT_REGISTER_RANGE    2
#define HD_OPT_DISABLE_CACHE         8
#define HD_OPT_NO_ORIGINAL_TILES     16
#define HD_OPT_AUTO_FALLBACK_TILES   32

/* ---- tile keys ---------------------------------------------------- */

/* Field order matters: palette_colors must immediately precede
 * tile_data so CHR RAM keys can be compared/hashed with a single
 * memcmp/rolling hash over both, exactly like Mesen's HdTileKey. */
typedef struct hd_tile_key
{
   uint32_t palette_colors;
   uint8_t  tile_data[16];
   int32_t  tile_index;
   uint8_t  is_chr_ram;
} hd_tile_key;

/* Per-pixel tile/sprite info recorded by the PPU hooks. */
typedef struct hd_ppu_tile_info
{
   uint32_t palette_colors;      /* keep adjacent to tile_data */
   uint8_t  tile_data[16];
   int32_t  tile_index;
   uint8_t  is_chr_ram;

   uint8_t  offset_x;
   uint8_t  offset_y;
   uint8_t  hmirror;
   uint8_t  vmirror;
   uint8_t  bg_priority;

   uint8_t  bg_color_index;
   uint8_t  sprite_color_index;
   uint8_t  bg_color;
   uint8_t  sprite_color;
   uint8_t  ppu_bg_color;
   uint8_t  palette_offset;
} hd_ppu_tile_info;

typedef struct hd_ppu_pixel_info
{
   hd_ppu_tile_info tile;
   hd_ppu_tile_info sprite[HD_MAX_SPRITES_PIXEL];
   uint8_t sprite_count;
} hd_ppu_pixel_info;

typedef struct hd_screen_info
{
   hd_ppu_pixel_info *pixels;             /* HD_PIXEL_COUNT entries */
   uint16_t line_tmp_vram_addr[HD_SCREEN_HEIGHT];
   uint8_t  line_x_scroll[HD_SCREEN_HEIGHT];
   uint8_t  line_grayscale[HD_SCREEN_HEIGHT];
   uint8_t  line_emphasis[HD_SCREEN_HEIGHT];
   uint32_t frame_number;
   uint8_t  watch_values[64];             /* parallel to hd.watch_addrs */
} hd_screen_info;

/* ---- conditions ---------------------------------------------------- */

enum hd_cond_type
{
   HD_COND_HMIRROR = 0,
   HD_COND_VMIRROR,
   HD_COND_BGPRIORITY,
   HD_COND_FRAME_RANGE,
   HD_COND_MEMORY_CHECK,
   HD_COND_MEMORY_CHECK_CONST,
   HD_COND_TILE_NEARBY,
   HD_COND_TILE_AT_POS,
   HD_COND_SPRITE_AT_POS,
   HD_COND_SPRITE_NEARBY,
   HD_COND_SPRITE_PALETTE,
   HD_COND_POS_CHECK_X,
   HD_COND_POS_CHECK_Y,
   HD_COND_ORIGIN_POS_CHECK_X,
   HD_COND_ORIGIN_POS_CHECK_Y
};

enum hd_cond_op
{
   HD_OP_INVALID = -1,
   HD_OP_EQ = 0,
   HD_OP_NE,
   HD_OP_GT,
   HD_OP_LT,
   HD_OP_LE,
   HD_OP_GE
};

#define HD_PPU_MEM_MARKER 0x80000000u
#define HD_COND_NAME_LEN  64

typedef struct hd_condition
{
   char name[HD_COND_NAME_LEN];
   uint8_t type;                 /* enum hd_cond_type */
   uint8_t negate;               /* name started with '!' */
   uint8_t use_cache;
   int8_t  result_cache;         /* -1 = not evaluated this frame */

   /* tile/sprite conditions */
   int32_t  tile_x;
   int32_t  tile_y;
   int32_t  pixel_offset;
   uint32_t palette_colors;
   uint8_t  tile_data[16];
   int32_t  tile_index;
   uint8_t  ignore_palette;

   /* memory conditions */
   uint32_t operand_a;           /* may carry HD_PPU_MEM_MARKER */
   uint32_t operand_b;
   int8_t   op;                  /* enum hd_cond_op */
   uint8_t  mask;
   int16_t  watch_slot_a;        /* resolved watch indices */
   int16_t  watch_slot_b;

   /* frameRange (reuses operand_a/operand_b) / position checks
    * (reuses op/operand_b) / sprite palette (reuses operand_a) */
} hd_condition;

/* ---- images -------------------------------------------------------- */

typedef struct hd_bitmap
{
   char     *name;
   uint8_t  *file_data;          /* raw file bytes until first decode */
   size_t    file_size;
   uint32_t *pixels;             /* premultiplied ARGB after decode */
   uint32_t  width;
   uint32_t  height;
   uint8_t   init_done;
   uint8_t   failed;
   /* Per-row non-transparent extent in *bitmap* pixel columns (0-based),
    * computed once at decode. row_min[r] is the first column with a
    * non-zero pixel and row_max[r] the last; row_min[r] > row_max[r]
    * (specifically row_min=width, row_max=0) marks a fully transparent
    * row. Lets the background span blit skip transparent runs, which
    * dominate HUD/parallax overlay rows. NULL until computed. */
   uint32_t *row_min;
   uint32_t *row_max;
} hd_bitmap;

/* ---- tiles ---------------------------------------------------------- */

typedef struct hd_pack_tile
{
   hd_tile_key key;

   uint32_t x;
   uint32_t y;
   uint32_t width;
   uint32_t height;
   uint32_t bitmap_index;
   int32_t  brightness;          /* 0..255+, version >= 105 may exceed */
   uint8_t  default_tile;
   uint32_t chr_bank_id;

   uint16_t *cond_idx;
   uint16_t  cond_count;
   uint8_t   force_disable_cache;

   /* lazily initialised HD pixel data */
   uint32_t *hd_data;            /* width * height ARGB */
   uint8_t   need_init;
   uint8_t   blank;
   uint8_t   has_transparent;
   uint8_t   fully_transparent;

   int32_t   next_in_key;        /* chain within the key hash bucket */
   int32_t   next_in_default;    /* chain within default-key bucket */
} hd_pack_tile;

/* ---- backgrounds ---------------------------------------------------- */

enum hd_blend_mode
{
   HD_BLEND_ALPHA = 0,
   HD_BLEND_ADD,
   HD_BLEND_SUBTRACT
};

typedef struct hd_background
{
   uint32_t bitmap_index;        /* into hd.bg_bitmaps */
   int32_t  brightness;
   uint16_t *cond_idx;
   uint16_t  cond_count;
   float    h_scroll_ratio;
   float    v_scroll_ratio;
   uint8_t  priority;            /* 0..39 */
   uint32_t left;
   uint32_t top;
   uint8_t  blend_mode;
} hd_background;

/* ---- additions / fallbacks ------------------------------------------ */

typedef struct hd_addition
{
   hd_tile_key original;
   hd_tile_key additional;
   int32_t offset_x;
   int32_t offset_y;
   uint8_t ignore_palette;
} hd_addition;

typedef struct hd_fallback
{
   int32_t tile_index;
   int32_t fallback_index;
} hd_fallback;

/* ---- audio ----------------------------------------------------------- */

typedef struct hd_track
{
   int32_t  id;                  /* album * 256 + track */
   char    *path;
   uint32_t loop_position;       /* BGM only */
} hd_track;

typedef struct hd_patch
{
   char   *filename;
   uint8_t sha1[20];
} hd_patch;

/* ---- hash tables ------------------------------------------------------ */

typedef struct hd_map_entry
{
   hd_tile_key key;
   int32_t first;                /* first tile index in chain, -1 empty */
   uint8_t used;
} hd_map_entry;

typedef struct hd_tile_map
{
   hd_map_entry *entries;
   uint32_t capacity;            /* power of two */
   uint32_t count;
} hd_tile_map;

/* ---- top-level pack data ---------------------------------------------- */

typedef struct hd_pack
{
   uint32_t version;
   uint32_t scale;
   uint32_t option_flags;

   hd_bitmap *bitmaps;           /* <img> sheets */
   uint32_t   bitmap_count;
   hd_bitmap *bg_bitmaps;        /* <background> images */
   uint32_t   bg_bitmap_count;

   hd_pack_tile *tiles;
   uint32_t      tile_count;
   uint32_t      tile_alloc;

   hd_condition *conds;
   uint32_t      cond_count;
   uint32_t      cond_alloc;

   hd_background *bgs[HD_BG_LAYER_COUNT];
   uint32_t       bg_count[HD_BG_LAYER_COUNT];

   hd_addition *additions;
   uint32_t     addition_count;

   hd_fallback *fallbacks;       /* from <fallback> tags */
   uint32_t     fallback_count;

   uint32_t watch_addrs[64];
   uint32_t watch_count;

   hd_track *bgm;
   uint32_t  bgm_count;
   hd_track *sfx;
   uint32_t  sfx_count;

   hd_patch *patches;
   uint32_t  patch_count;

   uint32_t custom_palette[64];
   uint8_t  has_custom_palette;

   uint8_t  has_overscan;
   uint16_t overscan[4];         /* top,right,bottom,left (parsed only) */

   hd_tile_map tile_map;         /* exact keys */
   hd_tile_map default_map;      /* palette = 0xFFFFFFFF keys */

   /* resolved after game load */
   uint8_t  is_chr_ram_game;

   /* runtime fallback map (auto + explicit); simple sorted array */
   hd_fallback *fallback_map;
   uint32_t     fallback_map_count;

   char pack_dir[1024];
} hd_pack;

extern hd_pack hd;
extern hd_screen_info hd_screen;

/* hdpack_image.c */
int  hd_bitmap_init(hd_bitmap *bmp);          /* lazy decode; 1 on ok */
void hd_bitmap_free(hd_bitmap *bmp);

/* hdpack_load.c helpers shared with other units */
void hd_log(const char *fmt, ...);
int  hd_pack_read_file(const char *filename, uint8_t **data, size_t *size);
int32_t hd_get_fallback_tile(int32_t tile_index);
uint32_t hd_tile_key_hash(const hd_tile_key *key);
int hd_tile_key_equal(const hd_tile_key *a, const hd_tile_key *b);
int32_t hd_map_find(const hd_tile_map *map, const hd_tile_key *key);

/* hdpack_render.c */
int  hd_render_init(void);
void hd_render_free(void);

/* hdpack_ppu.c */
int  hd_ppu_init(void);
void hd_ppu_free(void);

/* hdpack_audio.c */
void hd_audio_init(void);
void hd_audio_free(void);

#endif /* _FCEU_HDPACK_DATA_H */
