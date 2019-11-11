#ifndef _Isetjmp
#define _Isetjmp
#include <xmacros.h>

_C_LIB_DECL
#if defined(m88k) || defined(__m88k)

#define _JBLEN	40

#elif defined(__ppc)

/* Define ONLY_SOFTWARE_JMPBUF if you are only using software floating point and want
   to save space.  */

#ifdef __EVEC__
#define _JBLEN 23
#else
#ifdef __VEC__
#define _JBLEN 112
#else
#ifdef ONLY_SOFTWARE_JMPBUF
#define _JBLEN 26
#else
#define _JBLEN 64
#endif
#endif
#endif

#elif defined(__m68k)

#ifdef ONLY_SOFTWARE_JMPBUF
#define _JBLEN	13
#else
#define _JBLEN	34
#endif

#elif defined(__rce)

#define _JBLEN 9

#elif defined(__nec)

#define _JBLEN 12

#elif defined(__m32r)

#define _JBLEN 8

#elif defined(__mips)

/* Define ONLY_SOFTWARE_JMPBUF if you are only using software floating point and want
   to save space.  */

#ifdef ONLY_SOFTWARE_JMPBUF
#define _JBLEN 11
#else
#define _JBLEN 23
#endif

#elif defined(sparc) || defined(__sparc)
#define	_JBLEN		5

#elif defined(sh) || defined(__sh)
#define	_JBLEN		9
#elif defined(__arm)
#define	_JBLEN		11

#elif defined(__386)

#define _JBLEN 32 

#elif defined(__sc)
#define _JBLEN 32
#else

#error "_JBLEN not set!"

#endif

#ifdef __EVEC__
typedef __ev64_opaque__  jmp_buf[_JBLEN] ;
#elif defined(__VEC__)
typedef vector signed int jmp_buf[_JBLEN/4] ;

#elif defined(__MIPS64BIT__)
typedef long long jmp_buf[_JBLEN];
#else
typedef int jmp_buf[_JBLEN];
#endif

extern int setjmp(jmp_buf);
extern void longjmp(jmp_buf, int);

#if !defined(__STRICT_ANSI__)
#define sigjmp_buf jmp_buf

#ifdef __mot68
#define sigsetjmp(a,b) setjmp(a)
#define siglongjmp(a,b) longjmp(a,b)
#else
extern int sigsetjmp(sigjmp_buf,int);
extern void siglongjmp(sigjmp_buf, int);
#endif
#endif
_END_C_LIB_DECL
#endif

#ifdef _STD_USING
using	std::longjmp;
using	std::setjmp;
using	std::jmp_buf;
#endif

