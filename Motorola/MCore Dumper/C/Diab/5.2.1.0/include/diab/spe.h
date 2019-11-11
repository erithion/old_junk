#ifndef __Idiab_spe
#define __Idiab_spe
#include <xmacros.h>

_C_STD_BEGIN
_C_LIB_DECL


/* According to the PIM, these are equivalent to the strtosifx
 * calls EXCEPT for the behaviour on error, about which the PIM 
 * also says that these functions "need not affect the integer
 * expression errno". The other difference is that their behaviour
 * on unrepresentable values is undefined (by the PIM). So we decide
 * to establish the equivalence in all cases. If the PIM changes, 
 * we'll also change.
 *
 * atof translates to the special case of strtod.
 */

#define atosfix16(str)     strtosfix16 ((str), (char **) 0)
#define atosfix32(str)     strtosfix32 ((str), (char **) 0)
#define atosfix64(str)     strtosfix64 ((str), (char **) 0)
#define atoufix16(str)     strtoufix16 ((str), (char **) 0)
#define atoufix32(str)     strtoufix32 ((str), (char **) 0)
#define atoufix64(str)     strtoufix64 ((str), (char **) 0)

extern signed short int		strtosfix16 (const char *, char **) ;
extern signed int		strtosfix32 (const char *, char **) ;
extern signed long long int	strtosfix64 (const char *, char **) ;

extern unsigned short int	strtoufix16 (const char *, char **) ;
extern unsigned int		strtoufix32 (const char *, char **) ;
extern unsigned long long int	strtoufix64 (const char *, char **) ;


_END_C_LIB_DECL
_C_STD_END
#endif            /* __Idiab_spe */
