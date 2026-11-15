/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _FCEU_STATE_H_
#define _FCEU_STATE_H_

#include <stddef.h>

#include "fceu-memory.h"

int FCEUSS_Load_Mem(const void *buf, size_t size);
size_t FCEUSS_Save_Mem(void *buf, size_t size);

typedef struct {
	void *v;
	uint32_t s;
	char desc[5];
} SFORMAT;

/* A chunk kept in the state for state outside the SFORMAT tables, such
 * as a frontend filter or a sound mode's history. Each slot has its own
 * chunk type. size() is 0 when there is nothing to keep, and nothing is
 * written.
 * save() writes size() bytes, little-endian, straight into the state
 * buffer; load() reads the chunk where it lies in the loaded buffer and
 * checks len itself; absent() runs when a loaded state has no chunk. */
typedef struct
{
	size_t (*size)(void);
	void (*save)(uint8_t *dst);
	void (*load)(const uint8_t *src, size_t len);
	void (*absent)(void);
} fceuss_host_chunk_t;

enum
{
	FCEUSS_CHUNK_FRONTEND = 0,  /* the frontend driver's filters */
	FCEUSS_CHUNK_SOUND,         /* high-quality sound history */
	FCEUSS_CHUNKS
};

void FCEUSS_SetHostChunk(int slot, const fceuss_host_chunk_t *hc);

void ResetExState(void (*PreSave)(void), void (*PostSave)(void));
void AddExState(void *v, uint32_t s, int type, char *desc);

/* SFORMAT 's' field encoding:
 *   bit 31     (0x80000000): FCEUSTATE_RLSB - byte-swap on save/load when
 *                            the host endianness differs from on-disk LE.
 *   bits 24-30 (0x7F000000): FCEUSTATE_STRIDE - element size in bytes
 *                            for multi-element arrays, encoded directly
 *                            (1, 2, 4, 8 ...). 0 means "legacy" - take
 *                            the whole buffer as one element of size
 *                            equal to the byte count. The legacy
 *                            interpretation is correct only when the
 *                            entry is a single primitive (size <= 8) -
 *                            the previous savestate code only ever set
 *                            RLSB on single primitives, so legacy is
 *                            safe for old code.
 *   bits 0-23  (0x00FFFFFF): byte size of the entry. 16 MiB max, well
 *                            beyond any actual save state field.
 *
 * Use FCEUSTATE_RLSB_ARRAY(stride) to express "array of N elements,
 * each of `stride` bytes, byte-swap each element independently". For
 * single primitives use the bare FCEUSTATE_RLSB as before.
 */
#define FCEUSTATE_RLSB           0x80000000u
#define FCEUSTATE_STRIDE_MASK    0x7F000000u
#define FCEUSTATE_STRIDE_SHIFT   24
#define FCEUSTATE_SIZE_MASK      0x00FFFFFFu
/* Compose the stride bits for an N-byte element. */
#define FCEUSTATE_STRIDE(n)      (((uint32_t)(n) << FCEUSTATE_STRIDE_SHIFT) & FCEUSTATE_STRIDE_MASK)
/* Convenience: byte-swap-each-element flag for an array. */
#define FCEUSTATE_RLSB_ARRAY(n)  (FCEUSTATE_RLSB | FCEUSTATE_STRIDE(n))

void FCEU_DrawSaveStates(uint8_t *XBuf);

#endif
