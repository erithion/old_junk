#ifndef __I_fcntl
#define __I_fcntl

#ifdef	__cplusplus
extern "C" {
#endif

/* Flags used by open and fcntl */
#define	O_RDONLY	00000	/* read		*/
#define	O_WRONLY	00001	/* write	*/
#define	O_RDWR		00002	/* read/write	*/
#define O_ACCMODE	00003	/* rw-mask	*/
#define	O_NDELAY	00004	/* no buffering	*/
#define	O_APPEND	00010	/* at end	*/
#define O_SYNC		00020	/* sync writes	*/
#ifdef	__mot68
#define O_NONBLOCK	4
#else
#define O_NONBLOCK	00100
#endif
#define	O_CREAT		00400	/* open create	*/
#define	O_TRUNC		01000	/* open trunc	*/
#define	O_EXCL		02000	/* open exclus.	*/
#ifdef	__mot68
#define O_NOCTTY	0x10000000
#else
#define O_NOCTTY	04000
#endif
#define O_BINARY	010000

/* fcntl flags */
#define	F_DUPFD	 0	/* dup fd	*/
#define	F_GETFD	 1	/* get fd flags	*/
#define	F_SETFD	 2	/* set fd flags */
#define	F_GETFL	 3	/* get file flg */
#define	F_SETFL	 4	/* set file flg */
#define	F_GETLK	 5	/* get lock	*/
#define	F_SETLK	 6	/* set lock	*/
#define	F_SETLKW 7	/* set and wait	*/
#define F_CHKFL	 8	/* check flags	*/
#define F_ALLOCSP 10
#define F_FREESP 11
#ifdef	__mot68
#define	F_INSALLOC	253	/* insure allocated space */
#define	F_GETPALLOC	254	/* return amount of pre-allocated space */
#define	F_GETFSBSIZE	255	/* return file system block size */
#endif

/* file locking structure */
typedef struct flock {
	short	l_type;
	short	l_whence;
	long	l_start;
	long	l_len;
#if defined(m88k) || defined(__mc88k)
	long	l_pid;
	short	l_sysid;
#else
	short	l_sysid;
	short	l_pid;
#endif
} flock_t;

/* l_type */
#define	F_RDLCK	1	/* read lock	*/
#define	F_WRLCK	2	/* write lock	*/
#define	F_UNLCK	3	/* remove lock	*/

#define FD_CLOEXEC 1

#ifndef __Itypes
#include <sys/types.h>
#endif

extern  int     creat(const char *, mode_t);
extern  int     fcntl(int, int, ...);
extern  int     open(const char *, int, ...);

#ifdef	__cplusplus
}
#endif
#endif
