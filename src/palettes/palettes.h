#define EMPTY_PALETTE_1          { 0, 0, 0},
#define EMPTY_PALETTE_4          EMPTY_PALETTE_1 EMPTY_PALETTE_1 EMPTY_PALETTE_1 EMPTY_PALETTE_1
#define EMPTY_PALETTE_16         EMPTY_PALETTE_4 EMPTY_PALETTE_4 EMPTY_PALETTE_4 EMPTY_PALETTE_4
#define EMPTY_PALETTE_64         EMPTY_PALETTE_16 EMPTY_PALETTE_16 EMPTY_PALETTE_16 EMPTY_PALETTE_16
#define EMPTY_PALETTE_DEEMPH_X_7 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64 EMPTY_PALETTE_64

pal rp2c04_0001[512] = {
 #include "rp2c04-0001.h"
 EMPTY_PALETTE_DEEMPH_X_7
};

pal rp2c04_0002[512] = {
 #include "rp2c04-0002.h"
 EMPTY_PALETTE_DEEMPH_X_7
};

pal rp2c04_0003[512] = {
 #include "rp2c04-0003.h"
 EMPTY_PALETTE_DEEMPH_X_7
};
pal rp2c04_0004[512] = {
 #include "rp2c04-0004.h"
 EMPTY_PALETTE_DEEMPH_X_7
};

pal rp2c03[512] = {
 #include "rp2c03.h"
 EMPTY_PALETTE_DEEMPH_X_7
};


pal unvpalette[7] = {
	{ 0, 0, 0 }, /* Black */
	{ 255, 255, 211 }, /* White */
	{ 0, 0, 0 }, /* Black */
	{ 117, 117, 146 }, /* Greyish */
	{ 190, 0, 0 }, /* Redish */
	{ 51, 255, 51 }, /* Bright green */
	{ 49, 14, 200 },
};


/* Default palette */
pal palette[512] = {
	{ 117, 117, 117 }, /* Value 0 */
	{ 36, 24, 142 }, /* Value 1 */
	{ 0, 0, 170 }, /* Value 2 */
	{ 69, 0, 158 }, /* Value 3 */
	{ 142, 0, 117 }, /* Value 4 */
	{ 170, 0, 16 }, /* Value 5 */
	{ 166, 0, 0 }, /* Value 6 */
	{ 125, 8, 0 }, /* Value 7 */
	{ 65, 44, 0 }, /* Value 8 */
	{ 0, 69, 0 }, /* Value 9 */
	{ 0, 81, 0 }, /* Value 10 */
	{ 0, 60, 20 }, /* Value 11 */
	{ 24, 60, 93 }, /* Value 12 */
	{ 0, 0, 0 }, /* Value 13 */
	{ 0, 0, 0 }, /* Value 14 */
	{ 0, 0, 0 }, /* Value 15 */
	{ 190, 190, 190 }, /* Value 16 */
	{ 0, 113, 239 }, /* Value 17 */
	{ 32, 56, 239 }, /* Value 18 */
	{ 130, 0, 243 }, /* Value 19 */
	{ 190, 0, 190 }, /* Value 20 */
	{ 231, 0, 89 }, /* Value 21 */
	{ 219, 40, 0 }, /* Value 22 */
	{ 203, 77, 12 }, /* Value 23 */
	{ 138, 113, 0 }, /* Value 24 */
	{ 0, 150, 0 }, /* Value 25 */
	{ 0, 170, 0 }, /* Value 26 */
	{ 0, 146, 56 }, /* Value 27 */
	{ 0, 130, 138 }, /* Value 28 */
	{ 0, 0, 0 }, /* Value 29 */
	{ 0, 0, 0 }, /* Value 30 */
	{ 0, 0, 0 }, /* Value 31 */
	{ 255, 255, 255 }, /* Value 32 */
	{ 60, 190, 255 }, /* Value 33 */
	{ 93, 150, 255 }, /* Value 34 */
	{ 207, 138, 255 }, /* Value 35 */
	{ 247, 121, 255 }, /* Value 36 */
	{ 255, 117, 182 }, /* Value 37 */
	{ 255, 117, 97 }, /* Value 38 */
	{ 255, 154, 56 }, /* Value 39 */
	{ 243, 190, 60 }, /* Value 40 */
	{ 130, 211, 16 }, /* Value 41 */
	{ 77, 223, 73 }, /* Value 42 */
	{ 89, 251, 154 }, /* Value 43 */
	{ 0, 235, 219 }, /* Value 44 */
	{ 121, 121, 121 }, /* Value 45 */
	{ 0, 0, 0 }, /* Value 46 */
	{ 0, 0, 0 }, /* Value 47 */
	{ 255, 255, 255 }, /* Value 48 */
	{ 170, 231, 255 }, /* Value 49 */
	{ 199, 215, 255 }, /* Value 50 */
	{ 215, 203, 255 }, /* Value 51 */
	{ 255, 199, 255 }, /* Value 52 */
	{ 255, 199, 219 }, /* Value 53 */
	{ 255, 190, 178 }, /* Value 54 */
	{ 255, 219, 170 }, /* Value 55 */
	{ 255, 231, 162 }, /* Value 56 */
	{ 227, 255, 162 }, /* Value 57 */
	{ 170, 243, 190 }, /* Value 58 */
	{ 178, 255, 207 }, /* Value 59 */
	{ 158, 255, 243 }, /* Value 60 */
	{ 199, 199, 199 }, /* Value 61 */
	{ 0, 0, 0 }, /* Value 62 */
	{ 0, 0, 0 }, /* Value 63 */

	EMPTY_PALETTE_DEEMPH_X_7
};
