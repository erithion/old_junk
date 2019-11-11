#ifndef __Imath
#define __Imath
#include <xmacros.h>

_C_LIB_DECL

#ifndef HUGE_VAL
#define HUGE_VAL	9.9e99999
#endif

extern double
	acos(double),
	asin(double),
	atan(double),
	atan2(double, double),
	cos(double),
	sin(double),
	tan(double),
	cosh(double),
	sinh(double),
	tanh(double),
	exp(double),
	frexp(double, int *),
	ldexp(double, int),
	log(double),
	log10(double),
	modf(double, double *),
	pow(double, double),
	sqrt(double),
	ceil(double),
	fabs(double),
	floor(double),
	fmod(double, double);


#if defined( __C9X__) || (!defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE))

extern double			/* New with C9X */
 	erf(double),
	erfc(double),
	hypot(double, double),
	gamma(double),	
	lgamma(double),		/* was named gamma */
	round(double),
	trunc(double);

#ifndef _rhMATHF_H	/* mathf.h has these */
float acosf(float);
float asinf(float);
float atanf(float);
float atan2f(float,float);
float ceilf(float);
float cosf(float);
float coshf(float);
float expf(float);
float fabsf(float);
float floorf(float);
float fmodf(float,float);
float frexpf(float,int *);
float ldexpf(float,int);
float logf(float);
float log10f(float);
float modff(float,float *);
float powf(float,float);
float sinf(float);
float sinhf(float);
float sqrtf(float);
float tanf(float);
float tanhf(float);

/* New with C9X */
float erff(float);
float erfcf(float);
float hypotf(float,float);
float lgammaf(float);
float roundf(float);
float truncf(float);
#endif

#endif

#if !defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)
extern double
	j0(double), j1(double), jn(int, double),
	y0(double), y1(double), yn(int, double);


extern int signgam;

#ifndef __cplusplus
struct exception {
	int type;
	const char *name;
	double arg1;
	double arg2;
	double retval;
};

extern int matherr(struct exception *);
#endif

#define HUGE	HUGE_VAL

#define DOMAIN		1
#define SING		2
#define OVERFLOW	3
#define UNDERFLOW	4
#define TLOSS		5
#define PLOSS		6

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

#endif /* __STRICT_ANSI__ */

_END_C_LIB_DECL

#ifdef __cplusplus

extern long double long_double_cosl_not_supported(void); 
extern long double long_double_coshl_not_supported(void);
extern long double long_double_logl_not_supported(void);
extern long double long_double_log10l_not_supported(void);
extern long double long_double_cosl_not_supported(void);
extern long double long_double_sinhl_not_supported(void);
extern long double long_double_abs_not_supported(void);
extern long double long_double_acos_not_supported(void);
extern long double long_double_asin_not_supported(void);
extern long double long_double_atan_not_supported(void);
extern long double long_double_atan2_not_supported(void);
extern long double long_double_ceil_not_supported(void);
extern long double long_double_cos_not_supported(void);
extern long double long_double_cosh_not_supported(void);
extern long double long_double_exp_not_supported(void);
extern long double long_double_floor_not_supported(void);
extern long double long_double_fmod_not_supported(void);
extern long double long_double_frexp_not_supported(void);
extern long double long_double_ldexp_not_supported(void);
extern long double long_double_log_not_supported(void);
extern long double long_double_log10_not_supported(void);
extern long double long_double_modf_not_supported(void);
extern long double long_double_pow_not_supported(void);
extern long double long_double_pow_not_supported(void);
extern long double long_double_sin_not_supported(void);
extern long double long_double_cosl_not_supported(void);
extern long double long_double_sqrt_not_supported(void);
extern long double long_double_tan_not_supported(void);
extern long double long_double_tanh_not_supported(void);

inline long double cosl(long double)
	{	
	return ( long_double_cosl_not_supported() );
	}

inline long double coshl(long double)
	{	
	return ( long_double_coshl_not_supported() );
	}

inline long double logl(long double)
	{	
	return ( long_double_logl_not_supported() );
	}

inline long double log10l(long double)
	{	
	return ( long_double_log10l_not_supported() );
	}

inline long double sinl(long double)
	{	
	return ( long_double_cosl_not_supported() );
	}

inline long double sinhl(long double)
	{	
	return ( long_double_sinhl_not_supported() );
	}
#if !__GNUC__

inline long double abs(long double)	 
	{	
	return ( long_double_abs_not_supported() );
	}

inline long double acos(long double)
	{	
	return ( long_double_acos_not_supported() );
	}

inline long double asin(long double)
	{	
	return ( long_double_asin_not_supported() );
	}

inline long double atan(long double)
	{	
	return ( long_double_atan_not_supported() );
	}

inline long double atan2(long double, long double)
	{	
	return ( long_double_atan2_not_supported() );
	}

inline long double ceil(long double)
	{	
	return ( long_double_ceil_not_supported() );
	}

inline long double cos(long double)
	{	
	return ( long_double_cos_not_supported() );
	}

inline long double cosh(long double)
	{	
	return ( long_double_cosh_not_supported() );
	}

inline long double exp(long double)
	{	
	return ( long_double_exp_not_supported() );
	}

inline long double fabs(long double x)
	{	
		if ( x < 0.0 ) {
			return -x;
		}
	return x;
	}

inline long double floor(long double)
	{	
	return ( long_double_floor_not_supported() );
	}

inline long double fmod(long double, long double)
	{	
	return ( long_double_fmod_not_supported() );
	}

inline long double frexp(long double, int *)
	{	
	return ( long_double_frexp_not_supported() );
	}

inline long double ldexp(long double, int)
	{	
	return ( long_double_ldexp_not_supported() );
	}

inline long double log(long double )
	{	
	return ( long_double_log_not_supported() );
	}

inline long double log10(long double)
	{	
	return ( long_double_log10_not_supported() );
	}

inline long double modf(long double, long double *)
	{	
	return ( long_double_modf_not_supported() );
	}

inline long double pow(long double, long double)
	{	
	return ( long_double_pow_not_supported() );
	}

inline long double pow(long double, int)
	{	
	return ( long_double_pow_not_supported() );
	}

inline long double sin(long double)
	{	
	return ( long_double_sin_not_supported() );
	}

inline long double sinh(long double)
	{	
	return ( long_double_cosl_not_supported() );
	}

inline long double sqrt(long double)
	{	
	return ( long_double_sqrt_not_supported() );
	}

inline long double tan(long double)
	{	
	return ( long_double_tan_not_supported() );
	}

inline long double tanh(long double)
	{	
	return ( long_double_tanh_not_supported() );
	}
#endif /* __GNUC__ */

/* long double declarations */
extern long double acosl(long double);
extern long double acoshl(long double);
extern long double asinl(long double);
extern long double atanl(long double);
extern long double atan2l(long double, long double);
extern long double ceill(long double);
extern long double coshl(long double); 
extern long double expl(long double);
extern long double fabsl(long double);
extern long double floorl(long double);
extern long double fmodl(long double, long double);
extern long double frexpl(long double, int *);
extern long double ldexpl(long double, int);
extern long double log10l(long double);
extern long double modfl(long double, long double *);
extern long double powl(long double, long double);
extern long double sinhl(long double);
extern long double sqrtl(long double);
extern long double tanl(long double);
extern long double tanhl(long double);

#endif

#endif
