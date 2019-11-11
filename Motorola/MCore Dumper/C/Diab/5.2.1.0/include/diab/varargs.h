#ifndef __Idvarargs
#define __Idvarargs

#ifdef	__cplusplus
extern "C" {
#endif

#include "va_list.h"

#if defined(__lint)
/* trick lint */

#define va_dcl int va_alist;
#define va_start(list) (list = (va_list) &va_alist)
#define va_end(list) ((void)0)
#define va_arg(list, mode) (((mode *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1])

#elif defined(m88k) || defined(__m88k)

#pragma pure_function __mem_ptr, __reg_ptr

extern int *__mem_ptr(), *__reg_ptr();

#define va_dcl int va_alist;
#define va_start(list) {\
	(list).mem_ptr = __mem_ptr();\
	(list).reg_ptr = __reg_ptr();\
	(list).next_arg =  &va_alist - (list).mem_ptr;\
}
#define va_end(list)
#define va_arg(list, mode) (\
	sizeof(mode,2) <= 3 || sizeof(mode,2) == 44 ? (	/* char-short */\
		(list).next_arg < 8 ? (\
			*(mode*)((char*)(&(list).reg_ptr[++(list).next_arg])-sizeof(mode))\
		) : (\
			*(mode*)((char*)(&(list).mem_ptr[++(list).next_arg])-sizeof(mode))\
		)\
	) : sizeof(mode) == 4 && sizeof(mode,1) == 4 ? (/* 4 byte types */\
		(list).next_arg < 8 ? (\
			*(mode*)&(list).reg_ptr[(list).next_arg++]\
		) : (\
			*(mode*)&(list).mem_ptr[(list).next_arg++]\
		)\
	) : sizeof(mode,2) == 15  ? (			/* double */\
		((list).next_arg & 1) && (list).next_arg++,\
		(list).next_arg < 8 ? (\
			(list).next_arg += 2,\
			*(mode*)&(list).reg_ptr[(list).next_arg-2]\
		) : (\
			(list).next_arg += 2,\
			*(mode*)&(list).mem_ptr[(list).next_arg-2]\
		)\
	) : sizeof(mode,2) == 32 ? (			/* non-aggregates */\
		(list).next_arg < 8 ? (\
			**(mode**)&(list).reg_ptr[(list).next_arg++]\
		) : (\
			**(mode**)&(list).mem_ptr[(list).next_arg++]\
		)\
	) : (						/* structs */\
		(void)(sizeof(mode,1) == 8 && ((list).next_arg & 1) && (list).next_arg++),\
		(list).next_arg += (sizeof(mode)+3)>>2,\
		*(mode*)&(list).mem_ptr[(list).next_arg-((sizeof(mode)+3)>>2)]\
	)\
)

#elif defined(__ppc) && defined(__EABI__)

#define va_dcl int va_alist;

#pragma pure_function __va_mem, __va_reg, __va_gpr, __va_fpr

extern int __va_gpr(void), __va_fpr(void);
extern char *__va_mem(void), *__va_reg(void);
extern void *__va_arg(va_list, int);

#ifdef __softfp
#define __va_one_gpr(mode)	(sizeof(mode,2) <= 7 || sizeof(mode,2) == 14 || sizeof(mode,2) == 19 || sizeof(mode,2) == 34 || sizeof(mode,2) == 44)
#define __va_two_gpr(mode)	(sizeof(mode,2) <= 9 || sizeof(mode,2) == 15 || sizeof(mode,2) == 16)
#else
#define __va_one_gpr(mode)	(sizeof(mode,2) <= 7 || sizeof(mode,2) == 19 || sizeof(mode,2) == 34 || sizeof(mode,2) == 44)
#define __va_two_gpr(mode)	(sizeof(mode,2) <= 9)
#endif

#define va_start(list) {\
	(list[0]).__mem = __va_mem();\
	(list[0]).__reg = __va_reg();\
	(list[0]).__gpr = __va_gpr()-1;\
	(list[0]).__fpr = __va_fpr();\
}
#define va_end(list)
#ifdef __SINGLE_ONLY
#define va_arg(list, mode) (\
	(sizeof(mode,2) == 15) ? (	/* double -> float */\
        	(*(mode *)((char *)__va_arg(list, 1))) \ 
	) : __va_one_gpr(mode) ? (			/* char-ulint + ptr (float if SWFP) */\
		(*(mode *)((char *)__va_arg(list, 1)+sizeof(int)-sizeof(mode)))\
	) : __va_two_gpr(mode) ? (		/* llint-ullint (double if SWFP) */\
		(*(mode *)__va_arg(list, 2))\
	) : (sizeof(mode,2) == 14) ? (		/* float (602, -Xno-double) */\
		(*(mode *)__va_arg(list, 4))\
	) : (sizeof(mode,2) == 15 || sizeof(mode,2) == 16) ? (		/* double */\
		(*(mode *)__va_arg(list, 3))\
	) : (					/* structs and others passed as pointers */\
		(*(mode *)__va_arg(list, 0))\
	)\
)
#else
#define va_arg(list, mode) (\
	 __va_one_gpr(mode) ? (			/* char-ulint + ptr (float if SWFP) */\
		(*(mode *)((char *)__va_arg(list, 1)+sizeof(int)-sizeof(mode)))\
	) : __va_two_gpr(mode) ? (		/* llint-ullint (double if SWFP) */\
		(*(mode *)__va_arg(list, 2))\
	) : (sizeof(mode,2) == 14) ? (		/* float (602, -Xno-double) */\
		(*(mode *)__va_arg(list, 4))\
	) : (sizeof(mode,2) == 15 || sizeof(mode,2) == 16) ? (		/* double */\
		(*(mode *)__va_arg(list, 3))\
	) : (					/* structs and others passed as pointers */\
		(*(mode *)__va_arg(list, 0))\
	)\
)
#endif

#elif defined(__mips) || defined(__mcore) || defined(__sparc)

#define va_dcl int va_alist;
#define va_start(list) (list = (va_list) &va_alist)
#define va_end(list)
#if defined(__LITTLE_ENDIAN__)
#define va_arg(list, mode) (\
	sizeof(mode,2) == 32 ? (			/* non-aggregates. */\
		*((mode **)(list += ((sizeof(mode *)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (\
	sizeof(mode,1) >= 8 ? (				/* align on double */\
		list = (va_list)((int)((char *)list + sizeof(double) - 1) & -(int)sizeof(double)),\
		((mode *)(list += ((sizeof(mode)+sizeof(mode,1)-1) & ~(sizeof(mode,1)-1))))[-1]\
	) : (\
	sizeof(mode) >= sizeof(int) ? (			/* align on double */\
		((mode *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (\
		((mode *)((int *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))) - 1))[0]\
	)))\
)
#else
#define va_arg(list, mode) (\
	sizeof(mode,2) == 32 ? (			/* non-aggregates. */\
		*((mode **)(list += ((sizeof(mode *)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (\
	sizeof(mode,1) >= 8 ? (			/* align on double */\
		list = (va_list)((int)((char *)list + sizeof(double) - 1) & -(int)sizeof(double)),\
		((mode *)(list += ((sizeof(mode)+sizeof(mode,1)-1) & ~(sizeof(mode,1)-1))))[-1]\
	) : (\
		((mode *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	))\
)
#endif
#elif defined(__m32r) || defined(__sh)

#define va_dcl int va_alist;
#define va_start(list) (list = (va_list) &va_alist)
#define va_end(list)
#define va_arg(list, mode) (\
	sizeof(mode,2) == 32 ? (			/* non-aggregates. */\
		*((mode **)(list += ((sizeof(mode *)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (\
	sizeof(mode,1) >= 8 ? (			/* align on double */\
		list += (unsigned)list & 4,\
		((mode *)(list += ((sizeof(mode)+sizeof(mode,1)-1) & ~(sizeof(mode,1)-1))))[-1]\
	) : (\
		((mode *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	))\
)
#elif defined(__v850)

#define va_dcl int va_alist;
#define va_start(list) (list = (va_list) &va_alist)
#define va_end(list)
#define va_arg(list, mode) (\
	sizeof(mode,2) == 32 ? (			/* non-aggregates. */\
		*((mode **)(list += ((sizeof(mode *)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (\
	sizeof(mode,1) >= 8 ? (				/* align on double */\
		list = (va_list)((int)((char *)list + sizeof(double) - 1) & -(int)sizeof(double)),\
		((mode *)(list += ((sizeof(mode)+sizeof(mode,1)-1) & ~(sizeof(mode,1)-1))))[-1]\
	) : (\
	sizeof(mode) >= sizeof(int) ? (			/* align on double */\
		((mode *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (\
		((mode *)((int *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))) - 1))[0]\
	)))\
)

#elif defined(__m68k)

#define va_dcl int va_alist;
#define va_start(list) (list = (va_list) &va_alist)
#define va_end(list)
#define va_arg(list, mode) (\
	(sizeof(mode,2) == 32) ? (		/* non-aggregates. */\
		*((mode **)(list += ((sizeof(mode *)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (sizeof(mode,2) == 23 || sizeof(mode,2) == 24) ? ( 	/* align structs/unions on int */\
		((mode *)((list += ((sizeof(mode) + 3) & -4)) - ((sizeof(mode) + 3) & -4)))[0]\
	) : (\
		((mode *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	)\
)
#else

#define va_dcl int va_alist;
#define va_start(list) (list = (va_list) &va_alist)
#define va_end(list)
#define va_arg(list, mode) (\
	(sizeof(mode,2) == 32) ? (		/* non-aggregates. */\
		*((mode **)(list += ((sizeof(mode *)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	) : (\
		((mode *)(list += ((sizeof(mode)+sizeof(int)-1) & ~(sizeof(int)-1))))[-1]\
	)\
)
#endif

#ifdef	__cplusplus
}
#endif
#endif
