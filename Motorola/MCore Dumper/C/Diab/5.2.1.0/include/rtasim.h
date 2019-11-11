/*      Definition module : rtasim.h
 
        Copyright 1999 Diab Data, USA
 
        Description :
	RTAsim definitions
 
	6/17/99
*/

#ifndef __Irtasimh
#define __Irtasimh

#ifdef	__cplusplus
extern "C" {
#endif


/*
 * RTASIM control function and constants
 */
#ifndef ___SKIP_FDECL
extern int __simctrl(int opr, int par, void* arg);
#endif

/* operations */
#define SIMCTRL_OP_EXCTRL  0	/* exception control, 
				   par: handled by 0=simulator, 1=application
				   (int)arg: exception#, -1 = all */
#define SIMCTRL_OP_CLOSE   1	/* close all open host files & sockets 
				   par: 0 (reserved), arg: -not used- */
#define SIMCTRL_OP_ABORT   2	/* return abort flag, 1 if user ctrl-C 
				   par: 0 (reserved), arg: -not used- */
#define SIMCTRL_OP_MEM	   3	/* return start, size and type of memblock 
				   par: 0 (reserved)
				   arg: struct simctrl_mem pointer */
#define SIMCTRL_OP_ICOUNT  4	/* return current instruction count */

#define SIMCTRL_OP_SETMASK 5	/* set address mask
				   par: 0 (reserved)
				   arg: new mask (set if non null)
				   returns: previous mask */
#define SIMCTRL_OP_POLLCTRL 6	/* set poll parameters
				   par: 0 = set readpoll delay count (polls we have to do before
				          we get a successful poll, default=50 needed by pROBE)
					1 = set readpoll sleep-holdoff count (nbr of un-successful
					  polls before we sleep to save CPU, default=1000)
					2 = set select sleep-holdoff count (nbr of un-successfull
					  selects before we sleep to save CPU, default=1000)
				    arg: new count (0 = no delay or sleep)
				    returns: old count */
#define SIMCTRL_OP_INTTYPE 7	/* return the interrupt type 
				   par: 0 (reserved)
				   arg: not used
				   returns: type of last interrupt event, see __addevent below */
#define SIMCTRL_OP_CPUFREQ 8	/* return CPU clock freq in MHz (set w/ -s switch, default=10)*/
#define SIMCTRL_OP_MAINPAR 9	/* return main() parameters; argc, argv & envp */
#define SIMCTRL_OP_SETPRI  10	/* set rtasim priority */
#define SIMCTRL_OP_CLKTICK 11	/* get clock tick time of host (mS) */

/* parameters */
#define SIMCTRL_EXCTRL_SIM 0	/* simulator handles exception (default) */
#define SIMCTRL_EXCTRL_APP 1	/* application handles exception */

#define SIMCTRL_POLLCTRL_RDPOLL_DLY       0
#define SIMCTRL_POLLCTRL_RDPOLL_SLPHLDOFF 1
#define SIMCTRL_POLLCTRL_SELECT_SLPHLDOFF 2

#define SIMCTRL_MAINPAR_ARGC 		0
#define SIMCTRL_MAINPAR_ARGV 		1
#define SIMCTRL_MAINPAR_ENVP 		2

#define SIMCTRL_SETPRIPAR_LOWEST	0
#define SIMCTRL_SETPRIPAR_LOW		1
#define SIMCTRL_SETPRIPAR_NORMAL 	2
#define SIMCTRL_SETPRIPAR_HIGH		3
#define SIMCTRL_SETPRIPAR_HIGHEST	4

/* memblock inquiry structure, note: start is also input */
struct simctrl_mem {
    unsigned long start;/* address, input: address, output: start */
    unsigned long size;	/* size in bytes (0 = 4G bytes) */
    int type;		/* memory type, see below */
};

/* memory types (in struct simctrl_mem.type) */
#define SIMCTRL_MEM_INV	  0	/* invalid block */
#define SIMCTRL_MEM_READ  4	/* read */
#define SIMCTRL_MEM_WRITE 2	/* write */
#define SIMCTRL_MEM_EXEC  1	/* execute */
#define SIMCTRL_MEM_RW 	  6	/* read/write */
#define SIMCTRL_MEM_RWX	  7	/* read/write/execute */


/*
 * RTASIM help functions
 */
#ifndef ___SKIP_FDECL
extern int __readpoll(int fd);	/* returns 1 if a character is available */
extern int __rta_timer(int op);	/* op: 1=start, 0=stop RTA timer, ret: 0=OK */
extern int __simsleep(long ms);	/* ms: sleep for ms milli seconds, ret: == 0 */
extern int __addevent(		/* add (cyclic) exception event, ret: 0=OK */
		      void* handler,	/* handler address, or vector number */
		      int type, 	/* handler type, 0=host address (don't use),
					   1=target address, 2=vector number
					   bit 32..16 are ignored, but returned by
					   __simctrl(SIMCTRL_OP_INTTYPE, 0, 0) */
		      int interval,	/* interval cycle (nano seconds) */
		      int level,	/* int level, 0=any time, 1=at int lvl (68k=1-7,mips=2-7)*/
		      			/* ...for MIPS, 1 will interrupt without IM mask check */
		      int count);	/* times to execute, 0=forever, -1=stop, N:count */
extern int __simctrl(int opr, int par, void* arg); /* simulator control */
#endif
#define ADDEVENT_INTTYPE_SHIFT 8	/* upper 24 bits of type for user */


/*
 * Native RTASIM error codes
 * Mathches Diab definitions in errno.h
 */
#define	___EPERM  1	/* Not super-user			*/
#define	___ENOENT 2	/* No such file or directory		*/
#define	___ESRCH  3	/* No such process			*/
#define	___EINTR  4	/* interrupted system call		*/
#define	___EIO	  5	/* I/O error				*/
#define	___ENXIO  6	/* No such device or address		*/
#define	___E2BIG  7	/* Arg list too long			*/
#define	___ENOEXEC 8	/* Exec format error			*/
#define	___EBADF  9	/* Bad file number			*/
#define	___ECHILD 10	/* No children				*/
#define	___EAGAIN 11	/* No more processes			*/
#define	___ENOMEM 12	/* Not enough core			*/
#define	___EACCES 13	/* Permission denied			*/
#define	___EFAULT 14	/* Bad address				*/
#define	___ENOTBLK 15	/* Block device required		*/
#define	___EBUSY  16	/* Mount device busy			*/
#define	___EEXIST 17	/* File exists				*/
#define	___EXDEV  18	/* Cross-device link			*/
#define	___ENODEV 19	/* No such device			*/
#define	___ENOTDIR 20	/* Not a directory			*/
#define	___EISDIR 21	/* Is a directory			*/
#define	___EINVAL 22	/* Invalid argument			*/
#define	___ENFILE 23	/* File table overflow			*/
#define	___EMFILE 24	/* Too many open files			*/
#define	___ENOTTY 25	/* Not a typewriter			*/
#define	___ETXTBSY 26	/* Text file busy			*/
#define	___EFBIG  27	/* File too large			*/
#define	___ENOSPC 28	/* No space left on device		*/
#define	___ESPIPE 29	/* Illegal seek				*/
#define	___EROFS  30	/* Read only file system		*/
#define	___EMLINK 31	/* Too many links			*/
#define	___EPIPE  32	/* Broken pipe				*/
#define	___EDOM	  33	/* Math arg out of domain of func	*/
#define	___ERANGE 34	/* Math result not representable	*/
#define	___ENOMSG 35	/* No message of desired type		*/
#define	___EIDRM  36	/* Identifier removed			*/
#define	___ECHRNG 37	/* Channel number out of range		*/
#define	___EL2NSYNC 38	/* Level 2 not synchronized		*/
#define	___EL3HLT 39	/* Level 3 halted			*/
#define	___EL3RST 40	/* Level 3 reset			*/
#define	___ELNRNG 41	/* Link number out of range		*/
#define	___EUNATCH 42	/* Protocol driver not attached		*/
#define	___ENOCSI 43	/* No CSI structure available		*/
#define	___EL2HLT 44	/* Level 2 halted			*/
#define	___EDEADLK 45	/* Deadlock condition.			*/
#define	___ENOLCK 46	/* No record locks available.		*/
#define	___ECANCELED 47	/* canceled system call			*/
#define	___ENOTSUP 48	/* operation not supported		*/
#define	___EDQUOT 49	/* Disc quota exceeded			*/

/* Convergent Error Returns */
#define	___EBADE  50	/* invalid exchange			*/
#define	___EBADR  51	/* invalid request descriptor		*/
#define	___EXFULL 52	/* exchange full			*/
#define	___ENOANO 53	/* no anode				*/
#define	___EBADRQC 54	/* invalid request code			*/
#define	___EBADSLT 55	/* invalid slot				*/
#define	___EDEADLOCK 56	/* file locking deadlock error		*/
#define	___EBFONT 57	/* bad font file fmt			*/

/* stream problems */
#define	___ENOSTR 60	/* Device not a stream			*/
#define	___ENODATA 61	/* no data (for no delay io)		*/
#define	___ETIME  62	/* timer expired			*/
#define	___ENOSR  63	/* out of streams resources		*/
#define	___ENONET 64	/* Machine is not on the network	*/
#define	___ENOPKG 65	/* Package not installed		*/
#define	___EREMOTE 66	/* The object is remote			*/
#define	___ENOLINK 67	/* the link has been severed		*/
#define	___EADV	  68	/* advertise error			*/
#define	___ESRMNT 69	/* srmount error			*/
#define	___ECOMM  70	/* Communication error on send		*/
#define	___EPROTO 71	/* Protocol error			*/
#define	___EMULTIHOP 74	/* multihop attempted			*/
#define	___EBADMSG 77	/* trying to read unreadable message	*/
#define	___ENAMETOOLONG 78 /* path name is too long		*/
#define	___EOVERFLOW 79	/* value too large to be stored in data type */
#define	___ENOTUNIQ 80	/* given log. name not unique		*/
#define	___EBADFD   81	/* f.d. invalid for this operation	*/
#define	___EREMCHG  82	/* Remote address changed		*/

/* shared library problems */
#define	___ELIBACC  83	/* Can't access a needed shared lib.	*/
#define	___ELIBBAD  84	/* Accessing a corrupted shared lib.	*/
#define	___ELIBSCN  85	/* .lib section in a.out corrupted.	*/
#define	___ELIBMAX  86	/* Attempting to link in too many libs.	*/
#define	___ELIBEXEC 87	/* Attempting to exec a shared library.	*/
#define	___EILSEQ   88	/* Illegal byte sequence.		*/
#define	___ENOSYS   89	/* Unsupported file system operation	*/
#define	___ELOOP    90	/* Symbolic link loop			*/
#define	___ERESTART 91	/* Restartable system call		*/
#define	___ESTRPIPE 92	/* if pipe/FIFO, don't sleep in stream head */
#define	___ENOTEMPTY 93	/* directory not empty			*/
#define	___EUSERS   94	/* Too many users (for UFS)		*/

/* BSD Networking Software - argument errors */
#define	___ENOTSOCK	95	/* Socket operation on non-socket */
#define	___EDESTADDRREQ	96	/* Destination address required */
#define	___EMSGSIZE	97	/* Message too long */
#define	___EPROTOTYPE	98	/* Protocol wrong type for socket */
#define	___ENOPROTOOPT	99	/* Protocol not available */
#define	___EPROTONOSUPPORT 120	/* Protocol not supported */
#define	___ESOCKTNOSUPPORT 121	/* Socket type not supported */
#define	___EOPNOTSUPP	122	/* Operation not supported on socket */
#define	___EPFNOSUPPORT	123	/* Protocol family not supported */
#define	___EAFNOSUPPORT	124	/* Address family not supported by protocol family */
#define	___EADDRINUSE	125	/* Address already in use */
#define	___EADDRNOTAVAIL 126	/* Can't assign requested address */
/* - operational errors */
#define	___ENETDOWN	127	/* Network is down */
#define	___ENETUNREACH	128	/* Network is unreachable */
#define	___ENETRESET	129	/* Network dropped connection on reset */
#define	___ECONNABORTED	130	/* Software caused connection abort */
#define	___ECONNRESET	131	/* Connection reset by peer */
#define	___ENOBUFS	132	/* No buffer space available */
#define	___EISCONN	133	/* Socket is already connected */
#define	___ENOTCONN	134	/* Socket is not connected */
/* XENIX has 135 - 142 */
#define	___ESHUTDOWN	143	/* Can't send after socket shutdown */
#define	___ETOOMANYREFS	144	/* Too many references: can't splice */
#define	___ETIMEDOUT	145	/* Connection timed out */
#define	___ECONNREFUSED	146	/* Connection refused */
#define	___EHOSTDOWN	147	/* Host is down */
#define	___EHOSTUNREACH	148	/* No route to host */
#define	___EWOULDBLOCK	EAGAIN
#define	___EALREADY	149	/* operation already in progress */
#define	___EINPROGRESS	150	/* operation now in progress */

#define	___ESTALE	151	/* Stale NFS file handle */


/*
 * Native RTASIM posix calls, types and structures
 * Matches Diab definitions in sys/types.h, sys/stat.h, sys/times.h, 
 * sys/time.h, unistd.h and fcntl.h
 */
#define	___O_RDONLY	00000	/* read		*/
#define	___O_WRONLY	00001	/* write	*/
#define	___O_RDWR	00002	/* read/write	*/
#define ___O_ACCMODE	00003	/* rw-mask	*/
#define	___O_NDELAY	00004	/* no buffering	*/
#define ___O_APPEND	00010
#define ___O_SYNC	00020	/* sync writes	*/
#define ___O_NONBLOCK	00100
#define	___O_CREAT	00400	/* open create	*/
#define	___O_TRUNC	01000	/* open trunc	*/
#define	___O_EXCL	02000	/* open exclus.	*/
#define ___O_NOCTTY	04000
#define ___O_BINARY	010000

#define	___S_IRWXU	 00700	/* read, write, execute: owner		*/
#define	___S_IRUSR	 00400	/* read permission: owner		*/
#define	___S_IWUSR	 00200	/* write permission: owner		*/
#define	___S_IXUSR	 00100	/* execute permission: owner		*/
#define	___S_IRWXG	 00070	/* read, write, execute: group		*/
#define	___S_IRGRP	 00040	/* read permission: group		*/
#define	___S_IWGRP	 00020	/* write permission: group		*/
#define	___S_IXGRP	 00010	/* execute permission: group		*/
#define	___S_IRWXO	 00007	/* read, write, execute: other		*/
#define	___S_IROTH	 00004	/* read permission: other		*/
#define	___S_IWOTH	 00002	/* write permission: other		*/
#define	___S_IXOTH	 00001	/* execute permission: other		*/

#define	___R_OK	4	/* Test for read permission		*/
#define	___W_OK	2	/* Test for write permission		*/
#define	___X_OK	1	/* Test for execute permission		*/
#define	___F_OK	0	/* Test for existence of File		*/

#define	___SEEK_SET	0
#define	___SEEK_CUR	1
#define	___SEEK_END	2

typedef	unsigned short	___ino_t;
typedef	int 		___mode_t;
typedef	short		___dev_t;
typedef	long		___off_t;
typedef unsigned int	___size_t;
typedef	___size_t	___time_t;
typedef ___size_t	___clock_t;
typedef short		___pid_t;

struct	___stat {
	___dev_t	st_dev;
	___ino_t	st_ino;
	unsigned short	st_mode;
	short		st_nlink;
	unsigned short	st_uid;
	unsigned short	st_gid;
	___dev_t	st_rdev;
	___off_t	st_size;
	___time_t	st_atim;
	___time_t	st_mtim;
	___time_t	st_ctim;
};

struct ___tms {
	___clock_t	tms_utime;
	___clock_t	tms_stime;
	___clock_t	tms_cutime;
	___clock_t	tms_cstime;
};

struct ___timeval {
	___time_t	tv_sec;
	unsigned long	tv_usec;
};

#ifndef ___SKIP_FDECL
extern int __access(const char *path, int amode);
extern int __close(int fildes);
extern int __creat(const char *path, ___mode_t mode);
extern void ___exit(int status);
extern int __fcntl(int fildes, int cmd, int arg1, int arg2);
extern int __fstat(int fildes, struct ___stat *buf);
extern char *__getcwd(char *buf, ___size_t size);
extern ___pid_t __getpid(void);
extern int __isatty(int fildes);
extern int __link(const char *existingp, const char *newp);
extern ___off_t __lseek(int fildes, ___off_t offset, int whence);
extern int __mkdir(const char *path, ___mode_t mode);
extern int __open(const char *path, int oflag, ___mode_t mode);
extern int __read(int fildes, void *buf, ___size_t nbyte);
extern int __stat(const char *path, struct ___stat *buf);
extern ___time_t __time(___time_t *tloc);
extern ___clock_t __times(struct ___tms *buffer);
extern int __unlink(const char *path);
extern int __write(int fildes, const void *buf, ___size_t nbyte);
#endif

/*
 * native RTASIM socket functions and and used structures
 * Matches Diab definitions in sys/socket.h
 */
#define	___SOCK_STREAM	1		/* stream socket */
#define	___SOCK_DGRAM	2		/* datagram socket */
#define	___SOCK_RAW	3		/* raw-protocol interface */

#define	___SO_DEBUG	0x0001		/* turn on debugging info recording */
#define	___SO_ACCEPTCONN	0x0002		/* socket has had listen() */
#define	___SO_REUSEADDR	0x0004		/* allow local address reuse */
#define	___SO_KEEPALIVE	0x0008		/* keep connections alive */
#define	___SO_DONTROUTE	0x0010		/* just use interface addresses */
#define	___SO_BROADCAST	0x0020		/* permit sending of broadcast msgs */
#define	___SO_USELOOPBACK 0x0040		/* bypass hardware when possible */
#define	___SO_LINGER	0x0080		/* linger on close if data present */
#define	___SO_OOBINLINE	0x0100		/* leave received OOB data in line */

#define ___SO_SNDBUF	0x1001		/* send buffer size */
#define ___SO_RCVBUF	0x1002		/* receive buffer size */
#define	___SO_SNDLOWAT	0x1003		/* send low-water mark */
#define	___SO_RCVLOWAT	0x1004		/* receive low-water mark */
#define	___SO_SNDTIMEO	0x1005		/* send timeout */
#define	___SO_RCVTIMEO	0x1006		/* receive timeout */
#define	___SO_ERROR	0x1007		/* get error status and clear */
#define	___SO_TYPE	0x1008		/* get socket type */
#define	___SO_PROTOTYPE	0x1009		/* get/set protocol type */

struct	___linger {	/* Structure used for manipulating linger option */
	int	l_onoff;		/* option on/off */
	int	l_linger;		/* linger time */
};

#define	___SOL_SOCKET	0xffff		/* options for socket level */

#define	___AF_UNSPEC	0		/* unspecified */
#define	___AF_UNIX	1		/* local to host (pipes, portals) */
#define	___AF_INET	2		/* internetwork: UDP, TCP, etc. */
#define	___PF_UNIX	AF_UNIX
#define	___PF_INET	AF_INET

struct ___sockaddr {
	unsigned short sa_family;	/* address family */
	char	sa_data[14];		/* up to 14 bytes of direct address */
};

#define	___MSG_OOB	0x1		/* process out-of-band data */
#define	___MSG_PEEK	0x2		/* peek at incoming message */
#define	___MSG_DONTROUTE 0x4		/* send without using routing tables */

#ifndef ___SKIP_FDECL
extern int __accept(int s, struct ___sockaddr *addr, int *addrlen);
extern int __bind(int s, struct ___sockaddr *name, int namelen);
extern int __connect(int s, struct ___sockaddr *name, int namelen);
extern int __getpeername(int s, struct ___sockaddr *name, int *namelen);
extern int __getsockname(int s, struct ___sockaddr *name, int *namelen);
extern int __getsockopt(int s, int level, int optname, char *optval, int *optlen);
extern int __listen(int s, int backlog);
extern int __recv(int s, char *buf, int len, int flags);
extern int __recvfrom(int s, char *buf, int len, int flags, struct ___sockaddr *from, int *fromlen);
extern int __send(int s, const char *msg, int len, int flags);
extern int __sendto(int s, char *msg, int len, int flags, struct ___sockaddr *to, int tolen);
extern int __setsockopt(int s, int level, int optname, char *optval, int optlen);
extern int __socket(int domain, int type, int protocol);
extern int __shutdown(int s, int how);
#endif

/*
 * Native RTASIM select function definition and and used structures
 * Matches Diab definitions in sys/select.h
 */
typedef	long ___fd_mask;
#define	___FD_SETSIZE	1024
#define	___NBBY 	8
#define	___NFDBITS	(sizeof (___fd_mask) * ___NBBY)	/* bits per mask */
#define	___howmany(x, y) (((x)+((y)-1))/(y))
#ifndef ___h2tl
#define ___h2tl(x) (x)	/* endian conversion: long, host->target */
#endif
#ifndef ___t2hl
#define ___t2hl(x) (x)	/* endian conversion: long, target->host */
#endif

typedef	struct ___fd_set {
	___fd_mask fds_bits[___howmany(___FD_SETSIZE, ___NFDBITS)];
} ___fd_set;

#define	___FD_SET(n, p)	((p)->fds_bits[(n)/___NFDBITS] = \
	h2tl( t2hl((p)->fds_bits[(n)/___NFDBITS]) | ((unsigned)1 << ((n) % ___NFDBITS))))
#define	___FD_CLR(n, p)	((p)->fds_bits[(n)/___NFDBITS] = \
	h2tl( t2hl((p)->fds_bits[(n)/___NFDBITS]) & ~((unsigned)1 << ((n) % ___NFDBITS))))
#define	___FD_ISSET(n, p)  \
	      (t2hl((p)->fds_bits[(n)/___NFDBITS]) & ((unsigned)1 << ((n) % ___NFDBITS)))
#define	___FD_ZERO(p)	memset((char *)(p), 0, sizeof (*(p)))

#ifndef ___SKIP_FDECL
extern int __select(int, ___fd_set*, ___fd_set*, ___fd_set*, struct ___timeval *);
#endif

/*
 * native RTASIM socket support functions and and used structures
 * All addresses are supplied in host order, and returned 
 * in network order (suitable for use in system calls).
 * Matches Diab definitions in netdb.h
 */

#define	___MAXHOSTNAMELEN 64	/* maximum length of hostname */
#define	___MAXALIASES	5	/* maximum number of aliases returned (-1) */
#define	___MAXADDRS	10	/* maximum number of addresses returned (-1) */
#define	___MAXLINELEN	128	/* maximum linelength of serv_fp file */
#define ___MAXSERVLEN	64	/* maximum length of service name */
#define ___MAXPROTOLEN	64	/* maximum length of protocol name */

struct	___hostent {
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
	int	h_addrtype;	/* host address type */
	int	h_length;	/* length of address */
	char	**h_addr_list;	/* list of addresses from name server */
};

struct	___servent {
	char	*s_name;	/* official service name */
	char	**s_aliases;	/* alias list */
	int	s_port;		/* port # */
	char	*s_proto;	/* protocol to use */
};

struct ___in_addr {
	unsigned long __s_addr;
};

/*
 * The fields in the __hostent_data structure are referenced by
 * entries in the __hostent data structure after a successful
 * call to __gethostent_r(), __gethostbyname_r() or __gethostbyaddr_r().
 * __hostent_data also maintains state information between calls
 * to __gethostent_r().  The contents of this structure should not
 * be referenced directly except to initialize the hostf field to NULL.
 */
struct	___hostent_data {
	struct ___in_addr host_addr;		/* host address */
	void	*hostf;				/* hosts file (really FILE*) */
	short	_nsw_src;			/* source of answer */
	short	_flags;				/* internal flags */
	char	*current;			/* current YP entry */
	int	currentlen;			/* len of current YP entry */
	char	*host_aliases[___MAXALIASES];	/* list of aliases, NULL=end */
	char	*h_addr_ptrs[___MAXADDRS];	/* list of addresses,NULL=end*/
	char	hostbuf[___MAXHOSTNAMELEN * ___MAXALIASES];/* host name info buff*/
	struct ___in_addr hostaddr[___MAXADDRS];	/* host address list */
};

/*
 * __servent_data structure for calls to __getservent_r(), __getservbyname_r()
 * or __getservbyport_r().  The contents of this structure should not be
 * referenced directly except to initialize the serv_fp field to NULL.
 */
struct  ___servent_data {
	void	*serv_fp;			/* serv file (really FILE*) */
	char	line[___MAXLINELEN+1];
	char	*serv_aliases[___MAXALIASES];	/* list of aliases (NULL=end)*/
	short	_nsw_src;			/* source of answer */
	short	_flags;				/* internal flags */
	char	*current;			/* current YP entry */
	int	currentlen;			/* len of current YP entry */
	char	hostbuf[___MAXSERVLEN * (___MAXALIASES+1) + 
		       ___MAXPROTOLEN];		/* host name info buff*/
};

#ifndef ___SKIP_FDECL
extern int __gethostname(char *name, int maxlen);
extern int __gethostbyname_r(const char *name, struct ___hostent *ent, 
			     struct ___hostent_data *ent_data);
extern int __gethostbyaddr_r(const char *addr, int len, int type, 
			     struct ___hostent *ent, struct ___hostent_data *ent_data);
extern int __getservbyname_r(const char *name, const char *proto, 
			     struct ___servent *ent, struct ___servent_data *ent_data);
extern int __getservbyport_r(int port, const char *proto, 
			     struct ___servent *ent, struct ___servent_data *ent_data);
extern unsigned long ___inet_addr(const char *addr);
extern int __inet_ntoa_r(struct ___in_addr *in, char *buf, int buflen);
#endif

#ifdef	__cplusplus
}
#endif
#endif
