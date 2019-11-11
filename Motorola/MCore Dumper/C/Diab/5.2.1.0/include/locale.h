#ifndef __Ilocale
#define __Ilocale
#include <xmacros.h>

_C_LIB_DECL

struct lconv {
	char *decimal_point;
	char *thousands_sep;
	char *grouping;
	char *int_curr_symbol;
	char *currency_symbol;
	char *mon_decimal_point;
	char *mon_thousands_sep;
	char *mon_grouping;
	char *positive_sign;
	char *negative_sign;
	char int_frac_digits;
	char frac_digits;
	char p_cs_precedes;
	char p_sep_by_space;
	char n_cs_precedes;
	char n_sep_by_space;
	char p_sign_posn;
	char n_sign_posn;
};

#define LC_CTYPE	0
#define LC_COLLATE	1
#define LC_TIME		2
#define LC_NUMERIC	3
#define LC_MONETARY	4
#define LC_ALL		6

#ifndef NULL
#define NULL		0
#endif

extern char *setlocale(int, const char *);
extern struct lconv *localeconv(void);

_END_C_LIB_DECL

#ifdef _STD_USING
using	std::lconv;
using	std::localeconv;
using	std::setlocale;
#endif
#endif
