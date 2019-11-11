#ifndef __Ictype
#define __Ictype
#include <xmacros.h>
_C_STD_BEGIN
_C_LIB_DECL

#if defined(__STRICT_ANSI__) || defined(__m88k)
#define __CTYPE __ctype
#else
#define __CTYPE _ctype
#endif

#define	_U	1	/* A-Z */
#define	_L	2	/* a-z */
#define	_N	4	/* 0-9 */
#define	_S	8	/* white space */
#define	_P	16	/* !(' ' | isalnum) & isprint */
#define	_C	32	/* control character */
#define	_B	64	/* ' ' */
#define	_X	128	/* 0-9, A-F, a-f */

extern const unsigned char __CTYPE[];

#ifndef __cplusplus
extern int isalnum(int);
extern int isalpha(int);
extern int iscntrl(int);
extern int isdigit(int);
extern int isgraph(int);
extern int islower(int);
extern int isprint(int);
extern int ispunct(int);
extern int isspace(int);
extern int isupper(int);
extern int isxdigit(int);
extern int tolower(int);
extern int toupper(int);
#ifndef __STRICT_ANSI__
extern int isascii(int);
extern int toascii(int);
extern int setchrclass(char *);
#endif /* __STRICT_ANSI__ */

#define	isalnum(c)	(__CTYPE[(c)+1] & (_U|_L|_N))
#define	isalpha(c)	(__CTYPE[(c)+1] & (_U|_L))
#define	iscntrl(c)	(__CTYPE[(c)+1] & _C)
#define	isdigit(c)	(__CTYPE[(c)+1] & _N)
#define	isgraph(c)	(__CTYPE[(c)+1] & (_U|_L|_N|_P))
#define	islower(c)	(__CTYPE[(c)+1] & _L)
#define	isprint(c)	(__CTYPE[(c)+1] & (_U|_L|_N|_P|_B))
#define	ispunct(c)	(__CTYPE[(c)+1] & _P)
#define	isspace(c)	(__CTYPE[(c)+1] & _S)
#define	isupper(c)	(__CTYPE[(c)+1] & _U)
#define	isxdigit(c)	(__CTYPE[(c)+1] & _X)
#ifndef __STRICT_ANSI__
#define	toascii(c)	((c) & 0x7f)
#define	isascii(c)	(!((c) & ~0x7f))
#if defined(m88k) || defined (__m88k)
#define	_toupper(c)	(__CTYPE[(c)+258])
#define	_tolower(c)	(__CTYPE[(c)+258])
#else
#define	_toupper(c)	((c) - 'a' + 'A')
#define	_tolower(c)	((c) - 'A' + 'a')
#endif
#endif /* __STRICT_ANSI__ */

#else /* __cplusplus */

inline int isalnum(int c)	{ return(__CTYPE[(c)+1] & (_U|_L|_N)); }
inline int isalpha(int c)	{ return(__CTYPE[(c)+1] & (_U|_L)); }
inline int iscntrl(int c)	{ return(__CTYPE[(c)+1] & _C); }
inline int isdigit(int c)	{ return(__CTYPE[(c)+1] & _N); }
inline int isgraph(int c)	{ return(__CTYPE[(c)+1] & (_U|_L|_N|_P)); }
inline int islower(int c)	{ return(__CTYPE[(c)+1] & _L); }
inline int isprint(int c)	{ return(__CTYPE[(c)+1] & (_U|_L|_N|_P|_B)); }
inline int ispunct(int c)	{ return(__CTYPE[(c)+1] & _P); }
inline int isspace(int c)	{ return(__CTYPE[(c)+1] & _S); }
inline int isupper(int c)	{ return(__CTYPE[(c)+1] & _U); }
inline int isxdigit(int c)	{ return(__CTYPE[(c)+1] & _X); }
#if defined(m88k) || defined (__m88k)
inline int toupper(int c)	{ return(__CTYPE[(c)+258]); }
inline int tolower(int c)	{ return(__CTYPE[(c)+258]); }
#else
inline int toupper(int c)       { if(islower(c)) return c-'a'+'A'; return c; }
inline int tolower(int c)       { if(isupper(c)) return c-'A'+'a'; return c; }
#endif

#endif /* __cplusplus */

_END_C_LIB_DECL
_C_STD_END

#ifdef _STD_USING
using std::isalnum; 
using std::isalpha; 
using std::iscntrl;
using std::isdigit; 
using std::isgraph; 
using std::islower;
using std::isprint; 
using std::ispunct; 
using std::isspace;
using std::isupper; 
using std::isxdigit; 
using std::tolower;
using std::toupper;
#endif

#endif
