/* Default allocators for the Z_SOLO build of the bundled zlib subset.
 *
 * Z_SOLO strips the gz* file API and, with it, the default zcalloc /
 * zcfree implementations from zutil.c (upstream expects Z_SOLO users to
 * always pass explicit zalloc/zfree callbacks).  libretro-common's
 * trans_stream_zlib.c initialises its z_streams with Z_NULL allocators,
 * so provide the plain malloc/free versions here.
 *
 * Deliberately includes zconf.h rather than zutil.h: under Z_SOLO,
 * zutil.h avoids <stddef.h> and typedefs a guessed 'long ptrdiff_t',
 * which conflicts with the real 'long long' ptrdiff_t that <stdlib.h>
 * drags in on LLP64 targets (MinGW-w64).  zconf.h provides voidpf,
 * which is all this file needs; zutil.h hides the zcalloc/zcfree
 * prototypes under !Z_SOLO anyway, hence the local declarations.
 * ZLIB_INTERNAL matches zutil.h's definition (empty unless HAVE_HIDDEN,
 * which this build does not define). */

#include <stdlib.h>

#include "zconf.h"

#ifndef ZLIB_INTERNAL
#define ZLIB_INTERNAL
#endif

voidpf ZLIB_INTERNAL zcalloc(voidpf opaque, unsigned items, unsigned size);
void ZLIB_INTERNAL zcfree(voidpf opaque, voidpf ptr);

voidpf ZLIB_INTERNAL zcalloc(voidpf opaque, unsigned items, unsigned size)
{
   (void)opaque;
   return (voidpf)malloc((size_t)items * (size_t)size);
}

void ZLIB_INTERNAL zcfree(voidpf opaque, voidpf ptr)
{
   (void)opaque;
   free(ptr);
}
