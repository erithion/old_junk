/* mathe.h -- long double package, by aem 910729
 * This include file contains function prototypes for libme.a and the error
 * structure and constants for it.
 * It also contains the constant HUGE_E, but all other constants are to
 * be found in the header valuese.h
 */

#ifndef _rhMATHE_H
#define _rhMATHE_H

#ifdef	__cplusplus
extern "C" {
#endif
#define _IEEE_E 1

typedef union _h_val_e {
  unsigned short i[8];
  long double    e;
} _h_val_e;

extern _h_val_e __huge_val_e;

#define HUGE_E          __huge_val_e.e

/* Error report structure */
struct exceptione {
	int type;		/* See error types below 	*/
	char *name;		/* Name of failing function 	*/
	long double arg1;	/* First argument to function 	*/
        long double arg2;	/* Second arg, if present 	*/
	long double retval;	/* Value to be returned 	*/
};

/* Error types */
#define DOMAIN		1
#define SING		2
#define OVERFLOW	3
#define UNDERFLOW	4
#define TLOSS		5
#define PLOSS		6

/* Function prototypes to avaible functions: */

extern long double asine(long double x);
extern long double acose(long double x);
extern long double atane(long double x);
extern long double atan2e(long double y, long double x);

extern long double sine(long double x);
extern long double cose(long double x);
extern long double tane(long double x);

extern long double sinhe(long double x);
extern long double coshe(long double x);
extern long double tanhe(long double x);

extern long double expe(long double x);
extern long double log10e(long double x);
extern long double loge(long double x);
extern long double powe(long double x, long double y);
extern long double sqrte(long double x);

extern long double floore(long double x);
extern long double ceile(long double x);
extern long double fabse(long double x);
extern long double fmode(long double x, long double y);

extern long double erfe(long double x);
extern long double erfce(long double x);

extern long double gammae(long double x);

extern long double hypote(long double x, long double y);

extern long double j0e(long double x);
extern long double j1e(long double x);
extern long double jne(int n, long double x);
extern long double y0e(long double x);
extern long double y1e(long double x);
extern long double yne(int n, long double x);

extern int matherre(struct exceptione* exp);

/* libce stuff: */

long double frexpe(long double x, int* exp);
long double ldexpe(long double x, int exp);
long double modfe(long double x, long double *iptr);

#ifdef	__cplusplus
}
#endif
#endif
