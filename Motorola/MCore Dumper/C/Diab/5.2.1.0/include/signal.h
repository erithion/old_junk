#ifndef __Isignal
#define __Isignal
#include <xmacros.h>

_C_STD_BEGIN
_C_LIB_DECL

#include<sys/signal.h>

typedef void (*__sighandler_t)(int);
typedef volatile int sig_atomic_t;

extern __sighandler_t signal(int, __sighandler_t);
extern int raise(int);


#if !defined(__STRICT_ANSI__)

typedef struct sigset {
	unsigned long s[2];
} sigset_t;

struct sigaction {
	__sighandler_t sa_handler;
	sigset_t sa_mask;
	int	 sa_flags;
};

#if defined(m88k) || defined(__m88k)
#define	sigmask(s)	(1L << (31  - (((s) - 1) & 0x1f)))
#define	sigoff(s)	(((s) - 1) >> 5)

struct sigframe {
	long	sf_r31;
	long	sf_id;
	long	sf_sxip;
	long	sf_snip;
	long	sf_sfip;
	long	sf_sigsource;
	long	sf_signum;
	long	sf_numexceptblock;
	long	sf_exceptblock[1];
};

struct exceptblock {
	long eb_numarg;
	long eb_arg[1];
};

struct siginfo {
	long	si_version,
		si_r1,
		si_r2,
		si_r3,
		si_r4,
		si_r5,
		si_r6,
		si_r7,
		si_r8,
		si_r9,
		si_r10,
		si_r11,
		si_r12,
		si_r13,
		si_r14,
		si_r15,
		si_r16,
		si_r17,
		si_r18,
		si_r19,
		si_r20,
		si_r21,
		si_r22,
		si_r23,
		si_r24,
		si_r25,
		si_r26,
		si_r27,
		si_r28,
		si_r29,
		si_r30,
		si_psr,
		si_fpsr,
		si_fpcr;
	struct	sigframe	*si_sigframe;
};
#endif

extern int kill(int, int),
	sigemptyset(sigset_t *),
	sigfillset(sigset_t *),
	sigaddset(sigset_t *, int),
	sigdelset(sigset_t *, int),
	sigismember(const sigset_t *, int),
	sigaction(int, const struct sigaction *, struct sigaction *),
	sigprocmask(int, const sigset_t *, sigset_t *),
	sigpending(sigset_t *),
	sigsuspend(sigset_t *);

/* SVID functions: */
extern __sighandler_t sigset(int, __sighandler_t);
extern int sighold(int);
extern int sigrelse(int);
extern int sigignore(int);
extern int sigpause(int);

/* old V.3 functions: */
extern int gsignal(int),
	(*ssignal(int, int (*)()))(void);

#endif	/* __STRICT_ANSI__ */

_END_C_LIB_DECL
_C_STD_END

#ifdef _STD_USING
	using std::raise;
	using std::sig_atomic_t;
	using std::signal;
#endif
#endif
