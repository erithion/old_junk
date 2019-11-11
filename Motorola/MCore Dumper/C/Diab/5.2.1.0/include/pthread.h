/*	Implementation module : pthread.h

	Copyright 1998-99 Diab Data, Inc.

	Description :
	pthreads declatations
*/

#ifndef _PTHREAD_H
#define	_PTHREAD_H

#include <sys/types.h>
#include <time.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef	unsigned int pthread_t;
typedef	unsigned int pthread_key_t;
typedef	unsigned int pthread_mutex_t;
typedef	unsigned int pthread_cond_t;
typedef	unsigned int clockid_t;

/* definition per POSIX.4 */
typedef struct  timespec {	
    time_t	tv_sec;		/* seconds */
    long	tv_nsec;	/* and nanoseconds */
} timespec_t;

#define	CLOCK_REALTIME 0	/* only clock supported */
#define	PTHREAD_CANCELED  -19	/* return status from canceled thread */

/* attributes for threads */
typedef struct _pthread_attr {
    void* stackaddr;		/* stack addr */
    unsigned int stacksize;	/* stacksize */
    unsigned int priority; 	/* thread priority */
    unsigned int start_state;	/* initial thread state */
    int attr_pad[8];		/* future extensions */
} pthread_attr_t;


/* schedule parameter (prio) */
struct sched_param {
    unsigned int sched_priority;/* process execution scheduling priority */
    int	sched_pad[8];		/* future extensions */
};


/*
 * attributes for mutex and cond variables
 * NOTE: not supported and must be NULL in call
 */
typedef struct _pthread_mutexattr {
    int dummy;
} pthread_mutexattr_t;

typedef struct _pthread_condattr {
    int dummy;
} pthread_condattr_t;


/*
 * start_state bitmasks
 */
#define	PTHREAD_CREATE_JOINABLE	 0	/* joinable (default) */
#define PTHREAD_CREATE_DETACHED  0x1	/* detached thread (non joinable) */


/* info structure, returned by the pthread_thread_info - diab extension */
struct pthread_tinfo
{
    pthread_t id;		/* thread id */
    unsigned int prio;		/* thread prio */
    unsigned int state;		/* thread state */
    void* start;		/* start address */
    void* argument;		/* start argument */
    void* stack;		/* start of stack */
    unsigned long stacksize;	/* size of stack */		
    unsigned int timer;		/* thread schedule count */
    unsigned int sched;		/* thread tick count */
    void* tcb;			/* tcb address */
    void* res1;			/* reserved */
    void* res2;			/* reserved */
};

/* thread info state constants - diab extension */
#define PTHREAD_STATE_SMUTEX	0x1		/* suspended on mutex */
#define PTHREAD_STATE_SCOND	0x2		/* suspended on cond variable */
#define PTHREAD_STATE_SSEMA	0x4		/* suspended on semaphore */
#define PTHREAD_STATE_SSUSPEND	0x8		/* suspended explicitly */
#define PTHREAD_STATE_SJOIN	0x10		/* suspended on join */
#define PTHREAD_STATE_SSLEEP	0x40		/* sleeping or timeout */
#define PTHREAD_STATE_READY	0x80		/* ready */
#define PTHREAD_STATE_FREESTACK	0x800		/* thread canceled, on free-stack list */
#define PTHREAD_STATE_CANCELED	0x1000		/* thread canceled, waiting for join */
#define PTHREAD_STATE_DETACHED	0x2000		/* detached thread (non joinable) */
#define PTHREAD_STATE_SSTACK	0x4000		/* stack allocated by system */



/*
 * prototypes - thread related calls
 */
extern int pthread_attr_init(pthread_attr_t *attr);
extern int pthread_attr_destroy(pthread_attr_t *attr);
extern int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
extern int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
extern int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
extern int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
extern int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
extern int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
extern int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
extern int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
extern int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
extern int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);

extern int pthread_create(pthread_t*, const pthread_attr_t*, void* (*)(void*), void*);
extern int pthread_join(pthread_t, void**);
extern int pthread_cancel(pthread_t);
extern int pthread_kill(pthread_t, int);
extern void pthread_exit(void*);
extern pthread_t pthread_self(void);

extern int pthread_key_create(pthread_key_t*, void (*)(void*));
extern int pthread_key_delete(pthread_key_t);
extern int pthread_setspecific(pthread_key_t, const void*);
extern void *pthread_getspecific(pthread_key_t);
extern int pthread_setschedparam(pthread_t, int, const struct sched_param*);
extern int pthread_getschedparam(pthread_t, int*, struct sched_param*);
/* diab extensions */
extern int pthread_thread_info(pthread_t, struct pthread_tinfo*);
extern int pthread_variable_create(void**, void*);
extern void* pthread_variable_get(pthread_t, void**);
struct pthread_stackinfo;
extern int pthread_getstacks(pthread_t, struct pthread_stackinfo *, int);
extern int pthread_get_gregs(pthread_t, void *, int);	/*obsolete*/
extern void *pthread_get_sp_reg(pthread_t, void *);	/*obsolete*/
extern void *pthread_get_sp_start(pthread_t);		/*obsolete*/
extern void *pthread_get_sp_end(pthread_t);		/*obsolete*/
extern int pthread_suspend(pthread_t);
extern int pthread_continue(pthread_t);
extern int pthread_interrupt(pthread_t);
extern void pthread_lock(void);
extern void pthread_unlock(void);
extern void pthread_kernel_preinit(void);
extern int pthread_kernel_init(unsigned int threads, unsigned long ns_per_tick, 
  int rr_quanta, unsigned int objects, unsigned int keys, unsigned int static_mem);
extern void pthread_kernel_start(void);
extern void pthread_kernel_tick(void);
extern void pthread_kernel_isr_enter(void);
extern void pthread_kernel_isr_exit(void);

/*
 * prototypes - synchronization related calls
 */
extern int pthread_mutex_init(pthread_mutex_t*, const pthread_mutexattr_t*);
extern int pthread_mutex_destroy(pthread_mutex_t*);
extern int pthread_mutex_lock(pthread_mutex_t*);
extern int pthread_mutex_trylock(pthread_mutex_t*);
extern int pthread_mutex_unlock(pthread_mutex_t*);
extern int pthread_cond_init(pthread_cond_t*, const pthread_condattr_t*);
extern int pthread_cond_destroy(pthread_cond_t*);
extern int pthread_cond_broadcast(pthread_cond_t*);
extern int pthread_cond_signal(pthread_cond_t*);
extern int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*);
extern int pthread_cond_timedwait(pthread_cond_t*, pthread_mutex_t*, const struct timespec*);

/*
 * prototypes - time related calls
 */
extern int clock_settime(clockid_t, struct timespec*);
extern int clock_gettime(clockid_t, struct timespec*);
extern int clock_getres(clockid_t, struct timespec*);
extern int nanosleep(const struct timespec*, struct timespec*);


/*
 * unsupported calls 
 */
#if 0
extern int pthread_attr_setscope(pthread_attr_t*, int);
extern int pthread_attr_getscope(const pthread_attr_t*,	int*);
extern int pthread_attr_setinheritsched(pthread_attr_t *attr, int);
extern int pthread_attr_getinheritsched(const pthread_attr_t*, int*);
extern int pthread_mutexattr_init(pthread_mutexattr_t*);
extern int pthread_mutexattr_destroy(pthread_mutexattr_t*);
extern int pthread_mutexattr_setpshared(pthread_mutexattr_t*, int);
extern int pthread_mutexattr_getpshared(const pthread_mutexattr_t*, int*);
extern int pthread_mutexattr_setprotocol(pthread_mutexattr_t*, int);
extern int pthread_mutexattr_getprotocol(const pthread_mutexattr_t*, int*);
extern int pthread_mutexattr_setprioceiling(pthread_mutexattr_t*, int);
extern int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t*, int*);
extern int pthread_mutex_setprioceiling(pthread_mutex_t*, int, int*);
extern int pthread_mutex_getprioceiling(pthread_mutex_t*, int *ceiling);
extern int pthread_condattr_init(pthread_condattr_t*);
extern int pthread_condattr_destroy(pthread_condattr_t*);
extern int pthread_condattr_setpshared(pthread_condattr_t*, int);
extern int pthread_condattr_getpshared(const pthread_condattr_t*, int*);
extern int pthread_detach(pthread_t);
extern int pthread_setcancelstate(int, int*);
extern int pthread_setcanceltype(int, int*);
extern void pthread_testcancel(void);
extern int pthread_equal(pthread_t, pthread_t);
extern int pthread_atfork(void (*)(void), void (*)(void), void (*)(void));
extern int pthread_sigmask(int how, const sigset_t*, sigset_t*);
#endif

/*
 * callout pointers 
 */
extern void (* __dk_thread_switch_co)(pthread_t, pthread_t);
extern void (* __dk_thread_create1_co)(pthread_t);
extern void (* __dk_thread_create2_co)(void);
extern void (* __dk_thread_cancel_co)(void*);
extern void (* __dk_thread_signal_co)(int);
extern void (* __dk_idle_co)(void);
extern void (* __dk_init_co)(unsigned long);

#if defined(__ppc) || defined(__mips) || defined(__mcore) || \
    defined(__nec) || defined(__m32r)
  /* make sure the interrupt/exception frame can handle nesting */
# pragma option -Xnested-interrupts
#endif

#if defined(__mcore)
  /* for MCORE make sure we use normal interrupts & not use the alternate reg set */
# pragma option -Xinterrupt-rte
# pragma option -Xinterrupt-alt-reg-off
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _PTHREAD_H */
