#ifndef	__Iunistd
#define	__Iunistd
#include <xmacros.h>

_C_LIB_DECL

#ifndef NULL
#define NULL 0
#endif

#define F_ULOCK	0	/* unlock locked sections		*/
#define F_LOCK	1	/* lock a section for exclusive use	*/
#define F_TLOCK	2	/* test and lock a section		*/
#define F_TEST	3	/* test a section for locks		*/

#define	R_OK	4	/* Test for read permission		*/
#define	W_OK	2	/* Test for write permission		*/
#define	X_OK	1	/* Test for execute permission		*/
#define	F_OK	0	/* Test for existence of File		*/

#define	SEEK_SET	0
#define	SEEK_CUR	1
#define	SEEK_END	2

#define	STDIN_FILENO	0
#define	STDOUT_FILENO	1
#define	STDERR_FILENO	2

#define	_POSIX_DIR_DOTS		1
#define	_POSIX_EXIT_SIGHUP	0
#define	_POSIX_KILL_PID_NEG1	0	
#define	_POSIX_KILL_SAVED	0
#define	_POSIX_SAVED_IDS	1
#define	_POSIX_UTIME_OWNER	1	/* Allow file time set by owner */
#define	_POSIX_VERSION		198808L

#define _SC_ARG_MAX	1
#define _SC_CHILD_MAX	2
#define _SC_CLK_TCK	3
#define _SC_NGROUPS_MAX	4
#define _SC_OPEN_MAX	5
#define _SC_JOB_CONTROL	6
#define _SC_SAVED_IDS	7
#define _SC_VERSION	8
#define _SC_BCS_VERSION	9
#define _SC_BCS_VENDOR_STAMP	10
#define _SC_BCS_SYS_ID	11
#define _SC_MAXUMEMV	12
#define _SC_MAXUPROC	13
#define _SC_MAXMSGSZ	14
#define _SC_NMSGHDRS	15
#define _SC_SHMMAXSZ	16
#define _SC_SHMMINSZ	17
#define _SC_SHMSEGS	18
#define _SC_NMSYSSEM	19
#define _SC_MAXSEMVL	20
#define _SC_NSEMMAP	21
#define _SC_NSEMMSL	22
#define _SC_NSHMMNI	23
#define _SC_ITIMER_VIRT	24
#define _SC_ITIMER_PROF	25
#define _SC_TIMER_GRAN	26
#define _SC_PHYSMEM	27
#define _SC_AVAILMEM	28
#define _SC_NICE	29
#define _SC_MEMCTL_UNIT	30
#define _SC_SHMLBA	31
#define _SC_SVSTREAMS	32
#define _SC_CPUID	33
#define	_SC_PASS_MAX		1001
#define	_SC_PID_MAX		1002
#define	_SC_UID_MAX		1003
#define	_SC_EXIT_SIGHUP		1004
#define	_SC_KILL_PID_NEG1	1005
#define	_SC_KILL_SAVED		1006
#define	_SC_PGID_CLEAR		1007

#define _PC_LINK_MAX	1
#define _PC_MAX_CANON	2
#define _PC_MAX_INPUT	3
#define _PC_NAME_MAX	4
#define _PC_PATH_MAX	5
#define _PC_PIPE_BUF	6
#define _PC_CHOWN_RESTRICTED	7
#define _PC_NO_TRUNC	8
#define _PC_VDISABLE	9
#define _PC_BLKSIZE	10
#define	_PC_FCHR_MAX		1001
#define	_PC_CHOWN_SUP_GRP	1002
#define	_PC_DIR_DOTS		1003
#define	_PC_GROUP_PARENT	1004
#define	_PC_LINK_DIR		1005
#define	_PC_UTIME_OWNER		1006

#define GET_ULIMIT	1
#define SET_ULIMIT	2
#define GET_BREAK	3
#define GET_OPEN_MAX	4

#define	GF_PATH	"/etc/group"
#define	PF_PATH	"/etc/passwd"

#include <sys/types.h>

#ifndef DPORT

/* change here for dinkum */
#ifdef __cplusplus
namespace std {
extern	char *		ctermid(char *);
}
#endif

extern	void		_exit(int);
extern	int		access(const char *, int);
extern	unsigned int 	alarm(unsigned int);
extern	int		chdir(const char *);
extern	int		chown(const char *, int, int);
extern 	int		close(int);
extern	char *		cuserid(char *);
extern 	int		dup(int);
extern	int 		dup2(int, int);
extern 	int		execl(const char *, const char *, ...);
extern 	int		execle(const char *, const char *, ...);
extern	int		execlp(const char *, const char *, ...);
extern	int		execv(const char *, char *const *);
extern	int 		execve(const char *, char *const *, char *const *);
extern	int		execvp(const char *, char *const *);
extern	pid_t		fork(void);
extern	long		fpathconf(int, int);
extern	char * 		getcwd(char *, int);
extern	gid_t		getegid(void);
extern	uid_t		geteuid(void);
extern	gid_t		getgid(void);
extern 	int 		getgroups(int, gid_t *);
extern	char *		getlogin(void);
extern	pid_t		getpgrp(void);
extern	pid_t 		getpid(void);
extern	pid_t 		getppid(void);
extern	uid_t		getuid(void);
extern	int		isatty(int);
extern	int		link(const char *, const char *);
extern	off_t		lseek(int, off_t, int);
extern	long		pathconf(const char *, int);
extern	int		pause(void);
extern	int		pipe(int *);
extern	int		read(int, void *, unsigned);
extern 	int		rmdir(const char *);
extern	int		setgid(gid_t);
extern	pid_t		setsid(void);
extern	int		setpgid(pid_t, pid_t);
extern	int		setuid(uid_t);
extern	unsigned int 	sleep(unsigned);
extern	long		sysconf(int);
extern	pid_t		tcgetpgrp(int);
extern	int		tcsetpgrp(int, pid_t);
extern	char *		ttyname(int);
extern	int		unlink(const char *);
extern	int		write(int, const void *, unsigned int);
extern 	int		gethostname(char*, int);

/* unix functions not defined in POSIX	*/
#if !defined(_POSIX_SOURCE)
extern	int		brk(void *);
extern	int		chroot(char *);
extern	void *		sbrk(int);
extern	pid_t		setpgrp(void);
extern	int		lockf(int, int, long);
extern	int		nice(int);
extern	void		sync(void);
extern	void		profil(unsigned short *, unsigned, unsigned, unsigned);
extern	int		ptrace(int, int, int, int);
extern	long		ulimit(int, long);

#endif
#endif
_END_C_LIB_DECL
#endif
