/* Configure library by modifying this file */

#ifndef NES_NTSC_CONFIG_H
#define NES_NTSC_CONFIG_H

/* Uncomment to enable emphasis support and use a 512 color palette instead
of the base 64 color palette. */
#define NES_NTSC_EMPHASIS 1

/* The following affect the built-in blitter only; a custom blitter can
handle things however it wants. */

/* Bits per pixel of output. Can be 15, 16, 32, or 24 (same as 32). */
#if defined(FRONTEND_SUPPORTS_ARGB888)
#define NES_NTSC_OUT_DEPTH 32
#else
#define NES_NTSC_OUT_DEPTH 16
#endif

/* Type of input pixel values. You'll probably use unsigned short
if you enable emphasis above. */
#define NES_NTSC_IN_T unsigned char

/* Each raw pixel input value is passed through this. You might want to mask
the pixel index if you use the high bits as flags, etc. */
#define NES_NTSC_ADJ_IN( in, deemp ) (( in & 0x3F ) | ((deemp & 0x7) << 6))

/* For each pixel, this is the basic operation:
output_color = color_palette [NES_NTSC_ADJ_IN( NES_NTSC_IN_T )] */

#endif
