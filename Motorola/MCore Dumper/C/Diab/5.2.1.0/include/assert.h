#include <xmacros.h>

_C_STD_BEGIN

#ifdef NDEBUG

#define assert(expr)    ((void)0)
#else
#define assert(expr)	((void)(!(expr) ? __assert(#expr,__FILE__,__LINE__):(void)0))
_C_LIB_DECL
extern void __assert(const char *, const char *, int);
_END_C_LIB_DECL

#endif

_C_STD_END
