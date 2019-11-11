#ifndef __Idcc
#define __Idcc

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/types.h>

extern	void *		alloca(size_t);
extern	void *		__alloca(size_t);
extern	char *		ecvt(double, int, int *, int *);
extern	char *		fcvt(double, int, int *, int *);
extern	char *		gcvt(double, int, char *);
extern	char *		index(const char *, int);
extern	char *		rindex(const char *, int);
extern	void		swab(const char *, char*, int);
extern	long		tell(int);
extern	void		l3tol(long *, char *, int);
extern	void		ltol3(char *, long *, int);

#ifdef	__cplusplus
}
#endif
#endif
