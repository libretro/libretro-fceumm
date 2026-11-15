/* nes_ntsc 0.2.2. http://www.slack.net/~ant/ */

#include <string.h>

#include "nes_ntsc.h"

/* Copyright (C) 2006-2007 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

nes_ntsc_setup_t const nes_ntsc_monochrome = { 0,-1, 0, 0,.2,  0,.2,-.2,-.2,-1, 1, 0, 0, 0, 0 };
nes_ntsc_setup_t const nes_ntsc_composite  = { 0, 0, 0, 0, 0,  0, 0,  0,  0, 0, 1, 0, 0, 0, 0 };
nes_ntsc_setup_t const nes_ntsc_svideo     = { 0, 0, 0, 0,.2,  0,.2, -1, -1, 0, 1, 0, 0, 0, 0 };
nes_ntsc_setup_t const nes_ntsc_rgb        = { 0, 0, 0, 0,.2,  0,.7, -1, -1,-1, 1, 0, 0, 0, 0 };

#define alignment_count 3
#define burst_count     3
#define rescale_in      8
#define rescale_out     7

#define artifacts_mid   1.0f
#define fringing_mid    1.0f
#define std_decoder_hue -15

#define STD_HUE_CONDITION( setup ) !(setup->base_palette || setup->palette)

#include "nes_ntsc_impl.h"

/* 3 input pixels -> 8 composite samples */
pixel_info_t const nes_ntsc_pixels [alignment_count] = {
	{ PIXEL_OFFSET( -4, -9 ), { 1, 1, .6667f, 0 } },
	{ PIXEL_OFFSET( -2, -7 ), {       .3333f, 1, 1, .3333f } },
	{ PIXEL_OFFSET(  0, -5 ), {                  0, .6667f, 1, 1 } },
};

static void merge_kernel_fields( nes_ntsc_rgb_t* io )
{
	int n;
	for ( n = burst_size; n; --n )
	{
		nes_ntsc_rgb_t p0 = io [burst_size * 0] + rgb_bias;
		nes_ntsc_rgb_t p1 = io [burst_size * 1] + rgb_bias;
		nes_ntsc_rgb_t p2 = io [burst_size * 2] + rgb_bias;
		/* merge colors without losing precision */
		io [burst_size * 0] =
				((p0 + p1 - ((p0 ^ p1) & nes_ntsc_rgb_builder)) >> 1) - rgb_bias;
		io [burst_size * 1] =
				((p1 + p2 - ((p1 ^ p2) & nes_ntsc_rgb_builder)) >> 1) - rgb_bias;
		io [burst_size * 2] =
				((p2 + p0 - ((p2 ^ p0) & nes_ntsc_rgb_builder)) >> 1) - rgb_bias;
		++io;
	}
}

static void correct_errors( nes_ntsc_rgb_t color, nes_ntsc_rgb_t* out )
{
	int n;
	for ( n = burst_count; n; --n )
	{
		unsigned i;
		for ( i = 0; i < rgb_kernel_size / 2; i++ )
		{
			nes_ntsc_rgb_t error = color -
					out [i    ] - out [(i+12)%14+14] - out [(i+10)%14+28] -
					out [i + 7] - out [i + 5    +14] - out [i + 3    +28];
			DISTRIBUTE_ERROR( i+3+28, i+5+14, i+7 );
		}
		out += alignment_count * rgb_kernel_size;
	}
}

#ifdef NES_NTSC_SIMD
/* Entry of a kernel's slot that output lane x (0-6) of the pixel's own
 * chunk reads (kernel0/1/2 in NES_NTSC_RGB_OUT_14_), and the entry the
 * same lane of the next chunk reads (kernelx0/1/2). */
static int lane_kernel_index( int slot, int x )
{
	return slot == 0 ? x : slot == 1 ? (x + 12) % 7 + 14 : (x + 10) % 7 + 28;
}

static int lane_kernelx_index( int slot, int x )
{
	return slot == 0 ? (x + 7) % 14 : slot == 1 ? (x + 5) % 7 + 21 : (x + 3) % 7 + 35;
}

/* A pixel in chunk slot s starts adding at lane 2s of its chunk. As
 * kernelN it covers the rest of that chunk and, until the next pixel in
 * its slot arrives, the first 2s lanes of the next; as kernelxN the
 * rest of the next chunk and the first 2s lanes of the one after. */
static void build_lanes( nes_ntsc_t* ntsc )
{
	int burst, color, slot, t;

	for ( burst = 0; burst < nes_ntsc_burst_count; burst++ )
	for ( color = 0; color < nes_ntsc_palette_size; color++ )
	for ( slot = 0; slot < 3; slot++ )
	{
		nes_ntsc_rgb_t const* kernel = &ntsc->table [color] [burst * nes_ntsc_burst_size];
		nes_ntsc_lane_t* lane = ntsc->lanes [burst] [color] [slot];
		int start = slot * 2;

		memset( lane, 0, sizeof (ntsc->lanes [0] [0] [0]) );
		for ( t = 0; t < 14; t++ )
		{
			int at = start + t;
			int idx;
			if ( at < 7 )
				idx = lane_kernel_index( slot, at );
			else if ( at < 7 + start )
				idx = lane_kernel_index( slot, at - 7 );
			else if ( at < 14 )
				idx = lane_kernelx_index( slot, at - 7 );
			else
				idx = lane_kernelx_index( slot, at - 14 );
			lane [nes_ntsc_lane_lead + t] = (nes_ntsc_lane_t) kernel [idx];
		}
	}
}
#endif

void nes_ntsc_init( nes_ntsc_t* ntsc, nes_ntsc_setup_t const* setup )
{
	int merge_fields;
	int entry;
	init_t impl;
	float gamma_factor;
	
	if ( !setup )
		setup = &nes_ntsc_composite;
	init( &impl, setup );
	
	/* setup fast gamma */
	{
		float gamma = (float) setup->gamma * -0.5f;
		if ( STD_HUE_CONDITION( setup ) )
			gamma += 0.1333f;
		
		gamma_factor = (float) pow( (float) fabs( gamma ), 0.73f );
		if ( gamma < 0 )
			gamma_factor = -gamma_factor;
	}
	
	merge_fields = setup->merge_fields;
	if ( setup->artifacts <= -1 && setup->fringing <= -1 )
		merge_fields = 1;
	
	for ( entry = 0; entry < nes_ntsc_palette_size; entry++ )
	{
		/* Base 64-color generation */
		static float const lo_levels [4] = { -0.12f, 0.00f, 0.31f, 0.72f };
		static float const hi_levels [4] = {  0.40f, 0.68f, 1.00f, 1.00f };
		int level = entry >> 4 & 0x03;
		float lo = lo_levels [level];
		float hi = hi_levels [level];
		
		int color = entry & 0x0F;
		if ( color == 0 )
			lo = hi;
		if ( color == 0x0D )
			hi = lo;
		if ( color > 0x0D )
			hi = lo = 0.0f;
		
		{
			/* phases [i] = cos( i * PI / 6 ) */
			static float const phases [0x10 + 3] = {
				-1.0f, -0.866025f, -0.5f, 0.0f,  0.5f,  0.866025f,
				 1.0f,  0.866025f,  0.5f, 0.0f, -0.5f, -0.866025f,
				-1.0f, -0.866025f, -0.5f, 0.0f,  0.5f,  0.866025f,
				 1.0f
			};
			#define TO_ANGLE_SIN( color )   phases [color]
			#define TO_ANGLE_COS( color )   phases [(color) + 3]
			
			/* Convert raw waveform to YIQ */
			float sat = (hi - lo) * 0.5f;
			float i = TO_ANGLE_SIN( color ) * sat;
			float q = TO_ANGLE_COS( color ) * sat;
			float y = (hi + lo) * 0.5f;
			
			/* Optionally use base palette instead */
			if ( setup->base_palette )
			{
				unsigned char const* in = &setup->base_palette [(entry & 0x3F) * 3];
				static float const to_float = 1.0f / 0xFF;
				float r = to_float * in [0];
				float g = to_float * in [1];
				float b = to_float * in [2];
				q = RGB_TO_YIQ( r, g, b, y, i );
			}
			
			/* Apply color emphasis */
			#ifdef NES_NTSC_EMPHASIS
			{
				int tint = entry >> 6 & 7;
				if ( tint && color <= 0x0D )
				{
					static float const atten_mul = 0.79399f;
					static float const atten_sub = 0.0782838f;
					
					if ( tint == 7 )
					{
						y = y * (atten_mul * 1.13f) - (atten_sub * 1.13f);
					}
					else
					{
						static unsigned char const tints [8] = { 0, 6, 10, 8, 2, 4, 0, 0 };
						int const tint_color = tints [tint];
						float sat = hi * (0.5f - atten_mul * 0.5f) + atten_sub * 0.5f;
						y -= sat * 0.5f;
						if ( tint >= 3 && tint != 4 )
						{
							/* combined tint bits */
							sat *= 0.6f;
							y -= sat;
						}
						i += TO_ANGLE_SIN( tint_color ) * sat;
						q += TO_ANGLE_COS( tint_color ) * sat;
					}
				}
			}
			#endif
			
			/* Optionally use palette instead */
			if ( setup->palette )
			{
				unsigned char const* in = &setup->palette [entry * 3];
				static float const to_float = 1.0f / 0xFF;
				float r = to_float * in [0];
				float g = to_float * in [1];
				float b = to_float * in [2];
				q = RGB_TO_YIQ( r, g, b, y, i );
			}
			
			/* Apply brightness, contrast, and gamma */
			y *= (float) setup->contrast * 0.5f + 1;
			/* adjustment reduces error when using input palette */
			y += (float) setup->brightness * 0.5f - 0.5f / 256;
			
			{
				float r, g, b = YIQ_TO_RGB( y, i, q, default_decoder, float, r, g );
				
				/* fast approximation of n = pow( n, gamma ) */
				r = (r * gamma_factor - gamma_factor) * r + r;
				g = (g * gamma_factor - gamma_factor) * g + g;
				b = (b * gamma_factor - gamma_factor) * b + b;
				
				q = RGB_TO_YIQ( r, g, b, y, i );
			}
			
			i *= rgb_unit;
			q *= rgb_unit;
			y *= rgb_unit;
			y += rgb_offset;
			
			/* Generate kernel */
			{
				int r, g, b = YIQ_TO_RGB( y, i, q, impl.to_rgb, int, r, g );
				/* blue tends to overflow, so clamp it */
				nes_ntsc_rgb_t rgb = PACK_RGB( r, g, (b < 0x3E0 ? b: 0x3E0) );
				
				if ( setup->palette_out )
					RGB_PALETTE_OUT( rgb, &setup->palette_out [entry * 3] );
				
				if ( ntsc )
				{
					nes_ntsc_rgb_t* kernel = ntsc->table [entry];
					gen_kernel( &impl, y, i, q, kernel );
					if ( merge_fields )
						merge_kernel_fields( kernel );
					correct_errors( rgb, kernel );
				}
			}
		}
	}
#ifdef NES_NTSC_SIMD
	if ( ntsc )
		build_lanes( ntsc );
#endif
}

#ifndef NES_NTSC_NO_BLITTERS

/* The reference blitter: the only one on targets without a vector arm,
 * and what tests/ntsc_simd.c holds the vector arm to. */
#if !defined(NES_NTSC_SIMD) || defined(NES_NTSC_TEST)
#ifndef NES_NTSC_TEST
static
#endif
void nes_ntsc_blit_scalar( nes_ntsc_t const* ntsc, NES_NTSC_IN_T const* input, NES_NTSC_IN_T const* inputD, long in_row_width,
		int burst_phase, int in_width, int in_height, void* rgb_out, long out_pitch )
{
	int chunk_count = (in_width - 1) / nes_ntsc_in_chunk;
	for ( ; in_height; --in_height )
	{
		NES_NTSC_IN_T const* line_in = input;
		NES_NTSC_IN_T const* line_inD = inputD;
		NES_NTSC_BEGIN_ROW( ntsc, burst_phase,
				nes_ntsc_black, nes_ntsc_black, NES_NTSC_ADJ_IN( *line_in, *line_inD ) );
		nes_ntsc_out_t* line_out = (nes_ntsc_out_t*) rgb_out;
		int n;
		++line_in;
		
		for ( n = chunk_count; n; --n )
		{
			/* order of input and output pixels must not be altered */
			NES_NTSC_COLOR_IN( 0, NES_NTSC_ADJ_IN( line_in [0], line_inD[0] ) );
			NES_NTSC_RGB_OUT( 0, line_out [0], NES_NTSC_OUT_DEPTH );
			NES_NTSC_RGB_OUT( 1, line_out [1], NES_NTSC_OUT_DEPTH );
			
			NES_NTSC_COLOR_IN( 1, NES_NTSC_ADJ_IN( line_in [1], line_inD[1] ) );
			NES_NTSC_RGB_OUT( 2, line_out [2], NES_NTSC_OUT_DEPTH );
			NES_NTSC_RGB_OUT( 3, line_out [3], NES_NTSC_OUT_DEPTH );
			
			NES_NTSC_COLOR_IN( 2, NES_NTSC_ADJ_IN( line_in [2], line_inD[2] ) );
			NES_NTSC_RGB_OUT( 4, line_out [4], NES_NTSC_OUT_DEPTH );
			NES_NTSC_RGB_OUT( 5, line_out [5], NES_NTSC_OUT_DEPTH );
			NES_NTSC_RGB_OUT( 6, line_out [6], NES_NTSC_OUT_DEPTH );
			
			line_in  += 3;
			line_inD += 3;
			line_out += 7;
		}
		
		/* finish final pixels */
		NES_NTSC_COLOR_IN( 0, nes_ntsc_black );
		NES_NTSC_RGB_OUT( 0, line_out [0], NES_NTSC_OUT_DEPTH );
		NES_NTSC_RGB_OUT( 1, line_out [1], NES_NTSC_OUT_DEPTH );
		
		NES_NTSC_COLOR_IN( 1, nes_ntsc_black );
		NES_NTSC_RGB_OUT( 2, line_out [2], NES_NTSC_OUT_DEPTH );
		NES_NTSC_RGB_OUT( 3, line_out [3], NES_NTSC_OUT_DEPTH );
		
		NES_NTSC_COLOR_IN( 2, nes_ntsc_black );
		NES_NTSC_RGB_OUT( 4, line_out [4], NES_NTSC_OUT_DEPTH );
		NES_NTSC_RGB_OUT( 5, line_out [5], NES_NTSC_OUT_DEPTH );
		NES_NTSC_RGB_OUT( 6, line_out [6], NES_NTSC_OUT_DEPTH );
		
		burst_phase = (burst_phase + 1) % nes_ntsc_burst_count;
		input  += in_row_width;
		inputD += in_row_width;
		rgb_out = (char*) rgb_out + out_pitch;
	}
}
#endif

#ifdef NES_NTSC_SIMD

#if defined(NES_NTSC_SIMD_SSE2)
#include <emmintrin.h>
typedef __m128i nes_ntsc_v4;
#define V4_LOAD( p )      _mm_loadu_si128( (__m128i const*) (p) )
#define V4_STORE( p, v )  _mm_storeu_si128( (__m128i*) (p), (v) )
#define V4_ADD            _mm_add_epi32
#define V4_SUB            _mm_sub_epi32
#define V4_AND            _mm_and_si128
#define V4_OR             _mm_or_si128
#define V4_SRL( v, n )    _mm_srli_epi32( (v), (n) )
#define V4_SET1( x )      _mm_set1_epi32( (int) (x) )
#else
#include <arm_neon.h>
typedef uint32x4_t nes_ntsc_v4;
#define V4_LOAD( p )      vld1q_u32( (uint32_t const*) (p) )
#define V4_STORE( p, v )  vst1q_u32( (uint32_t*) (p), (v) )
#define V4_ADD            vaddq_u32
#define V4_SUB            vsubq_u32
#define V4_AND            vandq_u32
#define V4_OR             vorrq_u32
#define V4_SRL( v, n )    vshrq_n_u32( (v), (n) )
#define V4_SET1( x )      vdupq_n_u32( (uint32_t) (x) )
#endif

/* Four output lanes h..h+3 of a chunk: the six kernel slices summed,
 * clamped and packed as NES_NTSC_RGB_OUT_14_ does. N, P and PP are the
 * lane rows of the chunk's pixels and of the two chunks before. */
#define NES_NTSC_V4_LANES( h, out ) {\
	nes_ntsc_v4 v_ = V4_ADD(\
		V4_ADD( V4_ADD( V4_LOAD( &N0 [nes_ntsc_lane_lead     + (h)] ),\
		                V4_LOAD( &P0 [nes_ntsc_lane_lead + 7 + (h)] ) ),\
		        V4_ADD( V4_LOAD( &N1 [nes_ntsc_lane_lead - 2 + (h)] ),\
		                V4_LOAD( &P1 [nes_ntsc_lane_lead + 5 + (h)] ) ) ),\
		V4_ADD( V4_ADD( V4_LOAD( &PP1 [nes_ntsc_lane_lead + 12 + (h)] ),\
		                V4_LOAD( &N2  [nes_ntsc_lane_lead - 4  + (h)] ) ),\
		        V4_ADD( V4_LOAD( &P2  [nes_ntsc_lane_lead + 3  + (h)] ),\
		                V4_LOAD( &PP2 [nes_ntsc_lane_lead + 10 + (h)] ) ) ) );\
	nes_ntsc_v4 sub_   = V4_AND( V4_SRL( v_, 9 ), clamp_mask );\
	nes_ntsc_v4 clamp_ = V4_SUB( clamp_add, sub_ );\
	v_     = V4_OR( v_, clamp_ );\
	clamp_ = V4_SUB( clamp_, sub_ );\
	v_     = V4_AND( v_, clamp_ );\
	if ( NES_NTSC_OUT_DEPTH == 16 )\
		out = V4_OR( V4_OR( V4_AND( V4_SRL( v_, 13 ), V4_SET1( 0xF800 ) ),\
		                    V4_AND( V4_SRL( v_, 8 ),  V4_SET1( 0x07E0 ) ) ),\
		             V4_AND( V4_SRL( v_, 4 ), V4_SET1( 0x001F ) ) );\
	else\
		out = V4_OR( V4_OR( V4_AND( V4_SRL( v_, 5 ), V4_SET1( 0xFF0000 ) ),\
		                    V4_AND( V4_SRL( v_, 3 ), V4_SET1( 0x00FF00 ) ) ),\
		             V4_AND( V4_SRL( v_, 1 ), V4_SET1( 0x0000FF ) ) );\
}

static void nes_ntsc_blit_simd( nes_ntsc_t const* ntsc, NES_NTSC_IN_T const* input, NES_NTSC_IN_T const* inputD, long in_row_width,
		int burst_phase, int in_width, int in_height, void* rgb_out, long out_pitch )
{
	int const chunk_count = (in_width - 1) / nes_ntsc_in_chunk;
	nes_ntsc_v4 const clamp_mask = V4_SET1( nes_ntsc_clamp_mask );
	nes_ntsc_v4 const clamp_add  = V4_SET1( nes_ntsc_clamp_add );

	for ( ; in_height; --in_height )
	{
		nes_ntsc_lane_t const (*lanes) [3] [nes_ntsc_lane_size] = ntsc->lanes [burst_phase];
		NES_NTSC_IN_T const* line_in  = input + 1;
		NES_NTSC_IN_T const* line_inD = inputD;
		nes_ntsc_out_t* line_out = (nes_ntsc_out_t*) rgb_out;
		/* As NES_NTSC_BEGIN_ROW: the chunk before the first holds black,
		 * black and the first pixel, and the one before that black. */
		nes_ntsc_lane_t const* P0  = lanes [nes_ntsc_black] [0];
		nes_ntsc_lane_t const* P1  = lanes [nes_ntsc_black] [1];
		nes_ntsc_lane_t const* P2  = lanes [NES_NTSC_ADJ_IN( input [0], inputD [0] )] [2];
		nes_ntsc_lane_t const* PP1 = lanes [nes_ntsc_black] [1];
		nes_ntsc_lane_t const* PP2 = lanes [nes_ntsc_black] [2];
		int c;

		for ( c = 0; c <= chunk_count; c++ )
		{
			nes_ntsc_lane_t const *N0, *N1, *N2;
			nes_ntsc_v4 lo, hi;

			/* The chunk after the last pixels is black. */
			if ( c < chunk_count )
			{
				N0 = lanes [NES_NTSC_ADJ_IN( line_in [0], line_inD [0] )] [0];
				N1 = lanes [NES_NTSC_ADJ_IN( line_in [1], line_inD [1] )] [1];
				N2 = lanes [NES_NTSC_ADJ_IN( line_in [2], line_inD [2] )] [2];
				line_in  += 3;
				line_inD += 3;
			}
			else
			{
				N0 = lanes [nes_ntsc_black] [0];
				N1 = lanes [nes_ntsc_black] [1];
				N2 = lanes [nes_ntsc_black] [2];
			}

			NES_NTSC_V4_LANES( 0, lo );
			NES_NTSC_V4_LANES( 4, hi );

			/* Lane 7 is not an output; the next chunk overwrites it, and
			 * the last chunk of a row writes only its seven. */
#if NES_NTSC_OUT_DEPTH == 16
			{
				unsigned short px [8];
#if defined(NES_NTSC_SIMD_SSE2)
				lo = _mm_srai_epi32( _mm_slli_epi32( lo, 16 ), 16 );
				hi = _mm_srai_epi32( _mm_slli_epi32( hi, 16 ), 16 );
				_mm_storeu_si128( (__m128i*) px, _mm_packs_epi32( lo, hi ) );
#else
				vst1q_u16( (uint16_t*) px, vcombine_u16( vmovn_u32( lo ), vmovn_u32( hi ) ) );
#endif
				if ( c < chunk_count )
					memcpy( line_out, px, 8 * sizeof (unsigned short) );
				else
					memcpy( line_out, px, 7 * sizeof (unsigned short) );
			}
#else
			V4_STORE( line_out, lo );
			if ( c < chunk_count )
				V4_STORE( line_out + 4, hi );
			else
			{
				nes_ntsc_lane_t px [4];
				V4_STORE( px, hi );
				memcpy( line_out + 4, px, 3 * sizeof (nes_ntsc_lane_t) );
			}
#endif
			PP1 = P1;
			PP2 = P2;
			P0  = N0;
			P1  = N1;
			P2  = N2;
			line_out += 7;
		}

		burst_phase = (burst_phase + 1) % nes_ntsc_burst_count;
		input  += in_row_width;
		inputD += in_row_width;
		rgb_out = (char*) rgb_out + out_pitch;
	}
}
#endif

void nes_ntsc_blit( nes_ntsc_t const* ntsc, NES_NTSC_IN_T const* input, NES_NTSC_IN_T const* inputD, long in_row_width,
		int burst_phase, int in_width, int in_height, void* rgb_out, long out_pitch )
{
#ifdef NES_NTSC_SIMD
	nes_ntsc_blit_simd( ntsc, input, inputD, in_row_width, burst_phase,
			in_width, in_height, rgb_out, out_pitch );
#else
	nes_ntsc_blit_scalar( ntsc, input, inputD, in_row_width, burst_phase,
			in_width, in_height, rgb_out, out_pitch );
#endif
}

#endif
