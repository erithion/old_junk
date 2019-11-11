#ifndef __Idiab_wchar
#define __Idiab_wchar

#include <xmacros.h>
#include <stdio.h>

#define _WConst_return

_C_LIB_DECL

#ifndef __size_t
#define	__size_t
#ifdef _TYPE_size_t
_TYPE_size_t;
#else
typedef unsigned int size_t;
#endif
#endif

#ifndef	__wchar_t
#define	__wchar_t
#ifdef _TYPE_wchar_t
_TYPE_wchar_t;
#else
typedef long int wchar_t;
#endif
#endif

#ifndef	__mbstate_t
#define	__mbstate_t mbstate_t
typedef struct {
	wchar_t	ch;
	char	fl;
} mbstate_t;
#endif

#if	!defined(__wint_t) && !defined(_WINT_T)
#define	__wint_t
#define _WINT_T
#ifdef _TYPE_wint_t
_TYPE_wint_t;
#else
typedef wchar_t wint_t;
#endif
#endif

#ifndef WEOF
#define WEOF	((wint_t)(-1))
#endif

struct tm;
struct tmx;

int fwprintf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, ...);
int fwscanf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, ...);
int swprintf(wchar_t * __RESTRICT s, size_t n, const wchar_t * __RESTRICT format, ...);
int swscanf(const wchar_t * __RESTRICT s, const wchar_t * __RESTRICT format, ...);
int vfwprintf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, va_list arg);
int vfwscanf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, va_list arg);
int vswprintf(wchar_t * __RESTRICT s, size_t n, const wchar_t * __RESTRICT format, va_list arg);
int vswscanf(const wchar_t * __RESTRICT s, const wchar_t * __RESTRICT format, va_list arg);
int vwprintf(const wchar_t * __RESTRICT format, va_list arg);
int vwscanf(const wchar_t * __RESTRICT format, va_list arg);
int wprintf(const wchar_t * __RESTRICT format, ...);
int wscanf(const wchar_t * __RESTRICT format, ...);

wint_t fgetwc(FILE *);
wchar_t *fgetws(wchar_t *, int, FILE *);
wint_t fputwc(wint_t, FILE *);
int fputws(const wchar_t *, FILE *);
int fwide(FILE *stream, int mode);
wint_t getwc(FILE *);
wint_t getwchar(void);
wint_t putwc(wint_t, FILE *);
wint_t putwchar(wint_t);
wint_t ungetwc(wint_t, FILE *);

double wcstod(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr);
float wcstof(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr);
long double wcstold(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr);

long int wcstol(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);
unsigned long int wcstoul(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);

#if !defined(__DISABLE_LONG_LONG)
long long int wcstoll(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);
unsigned long long int wcstoull(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);
#endif

wchar_t *wcscpy(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2);
wchar_t *wcsncpy(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);
wchar_t *wcscat(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2);
wchar_t *wcsncat(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);

int wcscmp(const wchar_t *s1, const wchar_t *s2);
int wcscoll(const wchar_t *s1, const wchar_t *s2);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
size_t wcsxfrm(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);
wchar_t *wcschr(const wchar_t *s, wchar_t c);
size_t wcscspn(const wchar_t *s1, const wchar_t *s2);
size_t wcslen(const wchar_t *s);
wchar_t *wcspbrk(const wchar_t *s1, const wchar_t *s2);
wchar_t *wcsrchr(const wchar_t *s, wchar_t c);
size_t wcsspn(const wchar_t *s1, const wchar_t *s2);
wchar_t *wcsstr(const wchar_t *s1, const wchar_t *s2);
wchar_t *wcstok(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, wchar_t ** __RESTRICT ptr);

wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n);
int wmemcmp(const wchar_t * s1, const wchar_t * s2, size_t n);
wchar_t *wmemcpy(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);
wchar_t *wmemmove(wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wmemset(wchar_t *s, wchar_t c, size_t n);

size_t wcsftime(wchar_t * __RESTRICT s, size_t maxsize, const wchar_t * __RESTRICT format, const struct tm * __RESTRICT timeptr);
size_t wcsfxtime(wchar_t * __RESTRICT s, size_t maxsize, const wchar_t * __RESTRICT format, const struct tmx * __RESTRICT timeptr);

wint_t btowc(int c);
int wctob(wint_t c);

int mbsinit(const mbstate_t *ps);
size_t mbrlen(const char * __RESTRICT s, size_t n, mbstate_t * __RESTRICT ps);
size_t mbrtowc(wchar_t * __RESTRICT pwc, const char * __RESTRICT s, size_t n, mbstate_t * __RESTRICT ps);
size_t wcrtomb(char * __RESTRICT s, wchar_t wc, mbstate_t * __RESTRICT ps);
size_t mbsrtowcs(wchar_t * __RESTRICT dst, const char ** __RESTRICT src, size_t len, mbstate_t * __RESTRICT ps);
size_t wcsrtombs(char * __RESTRICT dst, const wchar_t ** __RESTRICT src, size_t len, mbstate_t * __RESTRICT ps);

_END_C_LIB_DECL

#endif
