#ifndef __Idiab_wctype
#define __Idiab_wctype

#include <xmacros.h>

_C_LIB_DECL

#ifndef	__wint_t
#define	__wint_t
typedef wchar_t wint_t;
#endif

#ifndef	__wctrans_t
#define	__wctrans_t
typedef size_t wctrans_t;
#endif

#ifndef	__wctype_t
#define	__wctype_t
typedef size_t wctype_t;
#endif

#ifndef WEOF
#define WEOF	((wint_t)(-1))
#endif

extern int iswalnum(wint_t);
extern int iswalpha(wint_t);
extern int iswcntrl(wint_t);
extern int iswdigit(wint_t);
extern int iswgraph(wint_t);
extern int iswlower(wint_t);
extern int iswprint(wint_t);
extern int iswpunct(wint_t);
extern int iswspace(wint_t);
extern int iswupper(wint_t);
extern int iswxdigit(wint_t);
extern int iswctype(wint_t, wctype_t);
extern wctype_t wctype(const char *);
extern wint_t towlower(wint_t);
extern wint_t towupper(wint_t);
wint_t towctrans(wint_t wc, wctrans_t desc);
wctrans_t wctrans(const char *property);

_END_C_LIB_DECL

#endif
