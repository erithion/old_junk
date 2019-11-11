#ifndef __Ierrno
#define __Ierrno
#include <xmacros.h>
#include <sys/errno.h>

_C_LIB_DECL
#define errno	(*__errno_fn())
extern int *__errno_fn(void);
_END_C_LIB_DECL

#endif
