/*	Implementation module : sched.h

	Copyright 1998 Diab Data, Inc.

	Description :
	sched declarations (pthreads)
*/

#ifndef _SCHED_H
#define	_SCHED_H

#ifdef	__cplusplus
extern "C" {
#endif


/*
 * posix scheduling policies
 */
#define	SCHED_OTHER	0
#define	SCHED_FIFO	1
#define	SCHED_RR	2


/*
 * prototypes
 */
extern int sched_yield(void);
extern int sched_get_priority_max(int);
extern int sched_get_priority_min(int);

/*
 * unsupported calls 
 */
#if 0
extern int sched_get_rr_get_interval(pid_t, struct timespec*);
extern int sched_getparam(pid_t, struct sched_param*);
extern int sched_setparam(pid_t, const struct sched_param*);
extern int sched_getscheduler(pid_t);
extern int sched_setscheduler(pid_t, int, const struct sched_param*);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _SCHED_H */
