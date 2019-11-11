#define CHAR_BIT	8
#if defined(__CHAR_UNSIGNED__)
#define CHAR_MAX	UCHAR_MAX
#define CHAR_MIN	0
#else
#define CHAR_MAX	SCHAR_MAX
#define CHAR_MIN	SCHAR_MIN
#endif
#define MB_LEN_MAX	5
#define INT_MAX		2147483647
#define INT_MIN		(-2147483647-1)
#define LONG_MAX	2147483647L
#define LONG_MIN	(-2147483647L-1L)
#define SCHAR_MAX	127
#define SCHAR_MIN	(-128)
#define SHRT_MAX	32767
#define SHRT_MIN	(-32768)
#define UCHAR_MAX	255
#define UINT_MAX	4294967295U
#define ULONG_MAX	4294967295LU
#define LLONG_MIN 	(-9223372036854775807LL-1LL)
#define LLONG_MAX	(9223372036854775807LL)
#define ULLONG_MAX 	(18446744073709551615ULL)
#define USHRT_MAX	65535
#if !defined(__RTP__)
#ifndef WCHAR_MIN
#define WCHAR_MIN	0
#endif
#ifndef WCHAR_MAX
#define WCHAR_MAX	USHRT_MAX
#endif
#endif /* !defined(__RTP__) */
#define WINT_MIN	INT_MIN
#define WINT_MAX	INT_MAX

#if !defined(__STRICT_ANSI__)
#ifndef DBL_DIG
#define DBL_DIG		15
#endif
#ifndef DBL_MAX
#define DBL_MAX		1.7976931348623157E308
#endif
#ifndef DBL_MIN
#define DBL_MIN		2.2250738585072016E-308
#endif
#ifndef FLT_DIG
#define FLT_DIG		6
#endif
#ifndef FLT_MAX
#define FLT_MAX		3.40282347E38F
#endif
#ifndef FLT_MIN
#define FLT_MIN		1.17549435E-38F
#endif

#if !defined(__RTP__)
#ifndef HUGE_VAL
#define HUGE_VAL	9.9e99999
#endif
#endif /* !defined(__RTP__) */

#ifdef	__mot68
#define NGROUPS_MAX	0
#define ARG_MAX		(8*1024)
#define CHILD_MAX	75
#define CLK_TCK		60
#define FCHR_MAX	1048576
#define OPEN_MAX	100
#define LINK_MAX	1000
#define NAME_MAX	14
#define PATH_MAX	256
#define PASS_MAX	8
#define PID_MAX		30000
#define PIPE_BUF	8192
#define PIPE_MAX	8192
#define STD_BLK		1024
#define SYS_NMLN	9
#define UID_MAX		60000
#define	USI_MAX		ULONG_MAX
#define WORD_BIT	32
#define MAX_CANON	255
#define MAX_INPUT	255
#else
#define NGROUPS_MAX	0
#define ARG_MAX		8
#define CHILD_MAX	50
#define CLK_TCK		(sysconf(3))
#define FCHR_MAX	ULONG_MAX
#define OPEN_MAX	72
#define LINK_MAX	1000
#define NAME_MAX	14
#define PATH_MAX	1024
#define PASS_MAX	8
#define PID_MAX		30000
#define PIPE_BUF	5120
#define PIPE_MAX	5120
#define STD_BLK		2048
#define SYS_NMLN	9
#define UID_MAX		30000
#define	USI_MAX		ULONG_MAX
#define WORD_BIT	32
#define MAX_CANON	255
#define MAX_INPUT	255
#endif

#if defined(m88k) || defined(__m88k)
#undef NGROUPS_MAX
#undef ARG_MAX
#undef CHILD_MAX
#undef OPEN_MAX
#undef LINK_MAX
#undef NAME_MAX
#undef PATH_MAX
#undef PIPE_BUF
#undef MAX_CANON
#undef MAX_INPUT
#endif
#endif	/* __STRICT_ANSI__ */


#ifdef	__mot68
#else
#define _POSIX_NGROUPS_MAX	0
#define _POSIX_ARG_MAX		4096
#define _POSIX_CHILD_MAX	6
#define _POSIX_OPEN_MAX		16
#define _POSIX_LINK_MAX		8
#define _POSIX_MAX_CANON	255
#define _POSIX_MAX_INPUT	255
#define _POSIX_NAME_MAX		14
#define _POSIX_PATH_MAX		255
#define _POSIX_PIPE_BUF		512
#endif
