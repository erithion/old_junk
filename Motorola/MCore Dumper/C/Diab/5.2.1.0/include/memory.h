#ifndef __Imemory
#define __Imemory

#include <xmacros.h>

_C_LIB_DECL
_C_STD_BEGIN

#ifndef __size_t
#define	__size_t
_TYPE_size_t;
#endif

extern void
	*memcpy(void *, const void *, size_t),
	*memccpy(void *, const void *, int, size_t),
	*memmove(void *, const void *, size_t),
	*memchr(const void *, int, size_t),
	*memset(void *, int, size_t);
extern int
	memcmp(const void *, const void *, size_t);

_C_STD_END
_END_C_LIB_DECL

#endif
