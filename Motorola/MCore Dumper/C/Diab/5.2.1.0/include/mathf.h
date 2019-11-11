/* mathf.h -- float package, by aem 910725
 */

#ifndef _rhMATHF_H
#define _rhMATHF_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _lint	/* for lint cleanup */
#include <values.h>
#include <limits.h>
#endif

#define _IEEE_F 1

#define HUGE_VAL_F      9.9e99999F
#define HUGE_F          HUGE_VAL_F

#define M_E		2.7182818284590452353603
#define M_LOG2E		1.4426950408889634073599
#define M_LOG10E	0.4342944819032518276511
#define M_LN2		0.6931471805599453094172
#define M_LN10		2.3025850929940456840180
#define M_PI		3.1415926535897932384626
#define M_PI_2		1.5707963267948966192313
#define M_PI_4		0.7853981633974483096157
#define M_1_PI		0.3183098861837906715377
#define M_2_PI		0.6366197723675813430755
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.4142135623730950488016
#define M_SQRT1_2	0.7071067811865475244008

/* Errors */

#define DOMAIN		1
#define SING		2
#define OVERFLOW	3
#define UNDERFLOW	4
#define TLOSS		5
#define PLOSS		6

/* Error report structure */
struct exceptionf {
	int type;
	const char *name;
	float arg1;
	float arg2;
	float retval;
};

/* Some useful constants for float */

#define X_TLOSS_F (((float)M_PI * FMAXPOWTWO))
#define H_PREC_F  ((FSIGNIF % 2 ? (float)M_SQRT2*(float)(1L << (FSIGNIF/2)) : (float)(1L << (FSIGNIF/2))))
#define X_EPS_F   ((float)(1.0F/H_PREC_F))
#define X_PLOSS_F ((float)(long)((float)M_PI * H_PREC_F))

#define LN_MAXFLOAT     ((float)(M_LN2 * (double)FMAXEXP))
#define LN_MINFLOAT     ((float)(M_LN2 * (double)(FMINEXP - 1)))
#define LN_DENFLOAT     ((float)(M_LN2 * (double)(FMINEXP - 1 - FLT_MANT_DIG)))

/* Function prototypes to avaible functions: */

extern float asinf(float x);
extern float acosf(float x);
extern float atanf(float x);
extern float atan2f(float y, float x);

extern float sinf(float x);
extern float cosf(float x);
extern float tanf(float x);

extern float sinhf(float x);
extern float coshf(float x);
extern float tanhf(float x);

extern float expf(float x);
extern float log10f(float x);
extern float logf(float x);
extern float powf(float x, float y);
extern float sqrtf(float x);

extern float roundf(float);	/* new with C9X */
extern float truncf(float);	/* new with C9X */
extern float floorf(float x);
extern float ceilf(float x);
extern float fabsf(float x);
extern float fmodf(float x, float y);

extern float erff(float x);
extern float erfcf(float x);

extern float lgammaf(float x);	/* was named gammaf */

extern float hypotf(float x, float y);

extern float j0f(float x);
extern float j1f(float x);
extern float jnf(int n, float x);
extern float y0f(float x);
extern float y1f(float x);
extern float ynf(int n, float x);

extern int matherrf(struct exceptionf* _exp);

/* libcf stuff: */

extern float frexpf(float x, int* _exp);
extern float ldexpf(float x, int _exp);
extern float modff(float x, float *iptr);

#ifdef	__cplusplus
}
#endif
#endif

