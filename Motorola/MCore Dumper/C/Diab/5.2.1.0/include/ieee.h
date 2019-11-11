#ifndef __Iieee
#define __Iieee

#ifdef __cplusplus
extern "C" {
#endif


#if	defined(__LITTLE_ENDIAN__)
typedef union ieee_bits {
	struct	{
		unsigned m2:32;
		unsigned m1:20;
		unsigned exp:11;
		unsigned sign:1;
	} b;
	struct	{
		unsigned i2:32;
		unsigned i1:32;
	} i;
	double d;
} *dptr, dnan;
#else
typedef union ieee_bits {
	struct	{
		unsigned sign:1;
		unsigned exp:11;
		unsigned m1:20;
		unsigned m2:32;
	} b;
	struct	{
		unsigned i1:32;
		unsigned i2:32;
	} i;
	double d;
} *dptr, dnan;
#endif


#if	defined(__LITTLE_ENDIAN__)
typedef union ieee_bits_f {
	struct	{
		unsigned m1:23;
		unsigned exp:8;
		unsigned sign:1;
	} b;
	unsigned i;
	float f;
} *fptr, fnan;
#else
typedef union ieee_bits_f {
	struct	{
		unsigned sign:1;
		unsigned exp:8;
		unsigned m1:23;
	} b;
	unsigned i;
	float f ;
} *fptr, fnan; 
#endif

extern double _logb(double) ;
extern double _copysign(double, double) ;
extern double _nextafter(double, double) ;
extern double scalbn(double, int) ;
extern double scalbln(double, long int) ;
extern double _chgsign(double) ;
extern double _scalb(double, int) ;
extern int _finite(double) ;
extern int _isinf(double) ;
extern int _isnan(double) ;
extern int _isnorm(double) ;
extern int _lessgreater(double, double) ;
extern int _unordered(double, double) ;
extern int _is_inf_or_nan(double) ;

#ifdef __STRICT_ANSI__
#define __inline__ 
#endif

__inline__ int _signbit(double x)
{
#ifdef __SINGLE_ONLY
        fnan x_union ;
        x_union.f = x ;
        return (int)x_union.b.sign;
#else
        dnan x_union ;
        x_union.d = x ;
        return (int)x_union.b.sign;
#endif
}

#ifdef __STRICT_ANSI__
#undef __inline__
#endif

#define _make_inf() (1.0/0.0)
#define _make_nan() (0.0/0.0)

extern float _logbf(float) ;
extern float _copysignf(float, float) ;
extern float _nextafterf(float, float) ;
extern float scalbnf(float, int);
extern float scalblnf(float, long int);
extern int _finitef(float) ;
extern int _isinff(float) ;
extern int _isnanf(float) ;
extern int _isnormf(float) ;
extern int _signbitf(float) ;

#define _make_inff() (1.0F/0.0F)
#define _make_nanf() (0.0F/0.0F)

#define isnan(a)	\
 ( (sizeof(a)==sizeof(float)) ? _isnanf(a) : _isnan((double)(a)) )

#define isinf(a)	\
 ( (sizeof(a)==sizeof(float)) ? _isinff(a) : _isinf((double)(a)) )

#define isfinite(a)	\
 ( (sizeof(a)==sizeof(float)) ? _finitef(a) : _finite((double)(a)) )

#define isnormal(a)	\
 ( (sizeof(a)==sizeof(float)) ? _isnormf(a) : _isnorm((double)(a)) )

#define signbit(a)	\
 ( (sizeof(a)==sizeof(float)) ? _signbitf(a) : _signbit((double)(a)) )

#ifdef __cplusplus
}
#endif
#endif
