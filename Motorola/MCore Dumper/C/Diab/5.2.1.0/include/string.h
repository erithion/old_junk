#ifndef __Istring
#define __Istring
#include <xmacros.h>
_C_STD_BEGIN
_C_LIB_DECL

#ifndef NULL
#define NULL	0
#endif

#ifndef __size_t
#define	__size_t
_TYPE_size_t;
#endif

extern void
	*memcpy(void *, const void *, size_t),
	*memmove(void *, const void *, size_t),
	*memchr(const void *, int, size_t),
	*memset(void *, int, size_t);
extern char
	*strcpy(char *, const char *),
	*strncpy(char *, const char *, size_t),
	*strcat(char *, const char *),
	*strncat(char *, const char *, size_t),
	*strchr(const char *, int),
	*strpbrk(const char *, const char *),
	*strrchr(const char *, int),
	*strstr(const char *, const char *),
	*strtok(char *, const char *),
	*strtok_r(char *, const char *, char **),
	*strerror(int);
extern int
	memcmp(const void *, const void *, size_t),
	strcmp(const char *, const char *),
	strcoll(const char *, const char *),
	strncmp(const char *, const char *, size_t);
extern size_t
	strxfrm(char *, const char *, size_t),
	strcspn(const char *, const char *),
	strspn(const char *, const char *),
	strlen(const char *);

#if !defined(__STRICT_ANSI__)

extern void
	*memccpy(void *, const void *, int, size_t);
extern char
	*strdup(const char *);

#endif /* __STRICT_ANSI__ */
_END_C_LIB_DECL
_C_STD_END

#ifdef	_STD_USING
using	std::memcpy
using	std::memmove
using	std::memchr
using	std::memset
using	std::strcpy
using	std::strncpy
using	std::strcat
using	std::strncat
using	std::strchr
using	std::strpbrk
using	std::strrchr
using	std::strstr
using	std::strtok
using	std::strtok_r
using	std::strerror
using	std::memcmp
using	std::strcmp
using	std::strcoll
using	std::strncmp
using	std::strxfrm
using	std::strcspn
using	std::strspn
using	std::strlen
#endif
#endif
