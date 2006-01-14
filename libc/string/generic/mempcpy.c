/* Copy memory to memory until the specified number of bytes
   has been copied, return pointer to following byte.
   Overlap is NOT handled correctly.
*/

/* Ditch the glibc version and just wrap memcpy. */

#define _GNU_SOURCE
#include <string.h>

libc_hidden_proto(mempcpy)
libc_hidden_proto(memcpy)

void *mempcpy (void *dstpp, const void *srcpp, size_t len)
{
  memcpy(dstpp, srcpp, len);
  return (void *)(((char *)dstpp) + len);
}
libc_hidden_def(mempcpy)
