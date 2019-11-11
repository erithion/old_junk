#ifndef	_Imalloc
#define	_Imalloc

#include <xmacros.h>

_C_LIB_DECL
_C_STD_BEGIN

#define M_MXFAST	1
#define M_NLBLKS	2
#define M_GRAIN		3
#define M_KEEP		4

struct mallinfo  {
	int	arena;
	int	ordblks;
	int	smblks;
	int	hblks;
	int	hblkhd;
	int	usmblks;
	int	fsmblks;
	int	uordblks;
	int	fordblks;
	int	keepcost;
};	

#ifndef __size_t
#define	__size_t
_TYPE_size_t;
#endif

extern void *malloc(size_t);
extern void free(void *);
extern void *realloc(void *, size_t);
extern void *calloc(size_t, size_t);
extern int mallopt(int, int);
extern struct mallinfo mallinfo(void);

_C_STD_END
_END_C_LIB_DECL

#endif

