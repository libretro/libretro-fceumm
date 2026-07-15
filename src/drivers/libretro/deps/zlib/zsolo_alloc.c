/* Default allocators for the Z_SOLO build of the bundled zlib subset.
 *
 * Z_SOLO strips the gz* file API and, with it, the default zcalloc /
 * zcfree implementations from zutil.c (upstream expects Z_SOLO users to
 * always pass explicit zalloc/zfree callbacks).  libretro-common's
 * trans_stream_zlib.c initialises its z_streams with Z_NULL allocators,
 * so provide the plain malloc/free versions here.  zutil.h hides the
 * prototypes under !Z_SOLO, hence the local declarations. */

#include <stdlib.h>

#define ZLIB_INTERNAL
#include "zutil.h"

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
