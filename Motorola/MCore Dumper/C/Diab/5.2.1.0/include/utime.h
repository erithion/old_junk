#ifndef __Iutime
#define __Iutime

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/types.h>

#if defined(m88k) || defined(__m88k)

struct	utimbuf {
	time_t		actime;
	unsigned long	acusec;
	time_t		modtime;
	unsigned long	modusec;
};

#else

struct	utimbuf {
	time_t	actime;
	time_t	modtime;
};

#endif

extern	int	utime(const char *, const struct utimbuf *);

#ifdef	__cplusplus
}
#endif
#endif

