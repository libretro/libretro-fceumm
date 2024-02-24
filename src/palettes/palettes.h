#define EMPTY_PALETTE_1          { 0, 0, 0},
#define EMPTY_PALETTE_4          EMPTY_PALETTE_1 EMPTY_PALETTE_1 EMPTY_PALETTE_1 EMPTY_PALETTE_1
#define EMPTY_PALETTE_16         EMPTY_PALETTE_4 EMPTY_PALETTE_4 EMPTY_PALETTE_4 EMPTY_PALETTE_4
#define EMPTY_PALETTE_64         EMPTY_PALETTE_16 EMPTY_PALETTE_16 EMPTY_PALETTE_16 EMPTY_PALETTE_16
#define EMPTY_PALETTE_DEEMPH_X_7 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64

pal rp2c04_0001[PALETTE_ARRAY_SIZE] = {
	#include "rp2c04-0001.h"
	EMPTY_PALETTE_DEEMPH_X_7
};

pal rp2c04_0002[PALETTE_ARRAY_SIZE] = {
	#include "rp2c04-0002.h"
	EMPTY_PALETTE_DEEMPH_X_7
};

pal rp2c04_0003[PALETTE_ARRAY_SIZE] = {
	#include "rp2c04-0003.h"
	EMPTY_PALETTE_DEEMPH_X_7
};

pal rp2c04_0004[PALETTE_ARRAY_SIZE] = {
	#include "rp2c04-0004.h"
	EMPTY_PALETTE_DEEMPH_X_7
};

pal rp2c03[PALETTE_ARRAY_SIZE] = {
	#include "rp2c03.h"
	EMPTY_PALETTE_DEEMPH_X_7
};

pal unvpalette[7] = {
	{ 0x00, 0x00, 0x00 }, /*  0 = Black */
	{ 0xFF, 0xFF, 0xD3 }, /*  1 = White */
	{ 0x00, 0x00, 0x00 }, /*  2 = Black */
	{ 0x75, 0x75, 0x92 }, /*  3 = Greyish */
	{ 0xBE, 0x00, 0x00 }, /*  4 = Reddish */
	{ 0x33, 0xFF, 0x33 }, /*  5 = Bright green */
	{ 0x31, 0x0E, 0xC8 }  /*  6 = Ultramarine Blue */
};

#define P64(x) (((x) << 2) | ((x >> 4) & 3))

/* Default palette */
pal palette[PALETTE_ARRAY_SIZE] = {
	{ P64(0x1D), P64(0x1D), P64(0x1D) }, /* Value 0 */
	{ P64(0x09), P64(0x06), P64(0x23) }, /* Value 1 */
	{ P64(0x00), P64(0x00), P64(0x2A) }, /* Value 2 */
	{ P64(0x11), P64(0x00), P64(0x27) }, /* Value 3 */
	{ P64(0x23), P64(0x00), P64(0x1D) }, /* Value 4 */
	{ P64(0x2A), P64(0x00), P64(0x04) }, /* Value 5 */
	{ P64(0x29), P64(0x00), P64(0x00) }, /* Value 6 */
	{ P64(0x1F), P64(0x02), P64(0x00) }, /* Value 7 */
	{ P64(0x10), P64(0x0B), P64(0x00) }, /* Value 8 */
	{ P64(0x00), P64(0x11), P64(0x00) }, /* Value 9 */
	{ P64(0x00), P64(0x14), P64(0x00) }, /* Value 10 */
	{ P64(0x00), P64(0x0F), P64(0x05) }, /* Value 11 */
	{ P64(0x06), P64(0x0F), P64(0x17) }, /* Value 12 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 13 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 14 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 15 */
	{ P64(0x2F), P64(0x2F), P64(0x2F) }, /* Value 16 */
	{ P64(0x00), P64(0x1C), P64(0x3B) }, /* Value 17 */
	{ P64(0x08), P64(0x0E), P64(0x3B) }, /* Value 18 */
	{ P64(0x20), P64(0x00), P64(0x3C) }, /* Value 19 */
	{ P64(0x2F), P64(0x00), P64(0x2F) }, /* Value 20 */
	{ P64(0x39), P64(0x00), P64(0x16) }, /* Value 21 */
	{ P64(0x36), P64(0x0A), P64(0x00) }, /* Value 22 */
	{ P64(0x32), P64(0x13), P64(0x03) }, /* Value 23 */
	{ P64(0x22), P64(0x1C), P64(0x00) }, /* Value 24 */
	{ P64(0x00), P64(0x25), P64(0x00) }, /* Value 25 */
	{ P64(0x00), P64(0x2A), P64(0x00) }, /* Value 26 */
	{ P64(0x00), P64(0x24), P64(0x0E) }, /* Value 27 */
	{ P64(0x00), P64(0x20), P64(0x22) }, /* Value 28 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 29 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 30 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 31 */
	{ P64(0x3F), P64(0x3F), P64(0x3F) }, /* Value 32 */
	{ P64(0x0F), P64(0x2F), P64(0x3F) }, /* Value 33 */
	{ P64(0x17), P64(0x25), P64(0x3F) }, /* Value 34 */
	{ P64(0x33), P64(0x22), P64(0x3F) }, /* Value 35 */
	{ P64(0x3D), P64(0x1E), P64(0x3F) }, /* Value 36 */
	{ P64(0x3F), P64(0x1D), P64(0x2D) }, /* Value 37 */
	{ P64(0x3F), P64(0x1D), P64(0x18) }, /* Value 38 */
	{ P64(0x3F), P64(0x26), P64(0x0E) }, /* Value 39 */
	{ P64(0x3C), P64(0x2F), P64(0x0F) }, /* Value 40 */
	{ P64(0x20), P64(0x34), P64(0x04) }, /* Value 41 */
	{ P64(0x13), P64(0x37), P64(0x12) }, /* Value 42 */
	{ P64(0x16), P64(0x3E), P64(0x26) }, /* Value 43 */
	{ P64(0x00), P64(0x3A), P64(0x36) }, /* Value 44 */
	{ P64(0x1E), P64(0x1E), P64(0x1E) }, /* Value 45 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 46 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 47 */
	{ P64(0x3F), P64(0x3F), P64(0x3F) }, /* Value 48 */
	{ P64(0x2A), P64(0x39), P64(0x3F) }, /* Value 49 */
	{ P64(0x31), P64(0x35), P64(0x3F) }, /* Value 50 */
	{ P64(0x35), P64(0x32), P64(0x3F) }, /* Value 51 */
	{ P64(0x3F), P64(0x31), P64(0x3F) }, /* Value 52 */
	{ P64(0x3F), P64(0x31), P64(0x36) }, /* Value 53 */
	{ P64(0x3F), P64(0x2F), P64(0x2C) }, /* Value 54 */
	{ P64(0x3F), P64(0x36), P64(0x2A) }, /* Value 55 */
	{ P64(0x3F), P64(0x39), P64(0x28) }, /* Value 56 */
	{ P64(0x38), P64(0x3F), P64(0x28) }, /* Value 57 */
	{ P64(0x2A), P64(0x3C), P64(0x2F) }, /* Value 58 */
	{ P64(0x2C), P64(0x3F), P64(0x33) }, /* Value 59 */
	{ P64(0x27), P64(0x3F), P64(0x3C) }, /* Value 60 */
	{ P64(0x31), P64(0x31), P64(0x31) }, /* Value 61 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 62 */
	{ P64(0x00), P64(0x00), P64(0x00) }, /* Value 63 */

	#undef P64

	EMPTY_PALETTE_DEEMPH_X_7
};
