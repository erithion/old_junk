#ifndef __Istdlib
#define __Istdlib
#include <xmacros.h>
_C_STD_BEGIN

_C_LIB_DECL

#ifndef __size_t
#define	__size_t
_TYPE_size_t;
#endif

#ifndef	__wchar_t
#define	__wchar_t
_TYPE_wchar_t;
#endif

typedef struct {
	int quot;
	int rem;
} div_t;

typedef struct {
	long quot;
	long rem;
} ldiv_t;

#ifndef NULL
#define NULL	0
#endif

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define RAND_MAX 32767
#define MB_CUR_MAX 1

extern double atof(const char *);
extern int atoi(const char *);
extern long atol(const char *);
extern double strtod(const char *, char **);
extern long strtol(const char *, char **, int);
extern unsigned long strtoul(const char *, char **, int);
extern int rand(void);
extern int rand_r(unsigned int *);
extern void srand(unsigned int);
extern void *calloc(size_t, size_t);
extern void free(void *);
extern void *malloc(size_t);
extern void *realloc(void *, size_t);
#ifdef __VEC__
extern void *vec_calloc(size_t, size_t);
extern void vec_free(void *);
extern void *vec_malloc(size_t);
extern void *vec_realloc(void *, size_t);
#endif

extern void abort(void);
extern int atexit(void (*)(void));
extern void exit(int);
extern char *getenv(const char *);
extern int system(const char *);

extern void *bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
extern void qsort(void *, size_t, size_t, int (*)(const void *, const void *));

extern int abs(int);
extern div_t div(int, int);
extern long labs(long);
extern ldiv_t ldiv(long, long);
extern int mblen(const char *, size_t);
extern int mbtowc(wchar_t *, const char *, size_t);
extern int wctomb(char *, wchar_t);
extern size_t mbstowcs(wchar_t *, const char *, size_t);
extern size_t wcstombs(char *, const wchar_t *, size_t);

#if !defined(__STRICT_ANSI__) || defined(__cplusplus)
extern	long		a64l(const char *);
extern	char *		l64a(long l);
extern	double		drand48(void);
extern	double		erand48(unsigned short *);
extern	long		irand48(unsigned short);
extern	long		jrand48(unsigned short *);
extern	long		krand48(unsigned short *, unsigned short);
extern	void		lcong48(unsigned short *);
extern	long		lrand48(void);
extern	long		mrand48(void);
extern	long		nrand48(unsigned short *);
extern	void		srand48(long);
extern	unsigned short *seed48(unsigned short *);
extern	char *		getpass(const char *);
extern	int		putenv(char *);

extern	int		__no_malloc_warning;

#endif /* __STRICT_ANSI__ */

_END_C_LIB_DECL

_C_STD_END

#endif
