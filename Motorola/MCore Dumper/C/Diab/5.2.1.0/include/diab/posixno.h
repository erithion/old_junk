/* POSIX calls */

#define	POSIX_exit	1
#define	POSIX_fork	2
#define	POSIX_read	3
#define	POSIX_write	4
#define	POSIX_open	5
#define	POSIX_close	6
#define	POSIX_wait	7
#define	POSIX_creat	8
#define	POSIX_link	9
#define	POSIX_unlink	10
#define	POSIX_exec	11
#define	POSIX_chdir	12
#define	POSIX_time	13
#define	POSIX_chmod	15
#define	POSIX_chown	16
#define	POSIX_brk	17
#define	POSIX_stat	18
#define	POSIX_lseek	19
#define	POSIX_getpid	20
#define	POSIX_setuid	23
#define	POSIX_getuid	24
#define	POSIX_stime	25
#define	POSIX_alarm	27
#define	POSIX_fstat	28
#define	POSIX_pause	29
#define	POSIX_utime	30
#define	POSIX_stty	31
#define	POSIX_gtty	32
#define	POSIX_access	33
#define	POSIX_nice	34
#define	POSIX_statfs	35
#define	POSIX_sync	36
#define	POSIX_kill	37
#define	POSIX_fstatfs	38
#define	POSIX_dup	41
#define	POSIX_pipe	42
#define	POSIX_times	43
#define	POSIX_profil	44
#define	POSIX_plock	45
#define	POSIX_setgid	46
#define	POSIX_getgid	47
#define	POSIX_signal	48
#define	POSIX_fdsync	58
#define	POSIX_execve	59
#define	POSIX_umask	60
#define	POSIX_chroot	61
#define	POSIX_fcntl	62
#define	POSIX_ulimit	63
#define	POSIX_rmdir	79
#define	POSIX_mkdir	80
#define	POSIX_getdents	81
#define	POSIX_lstat	88
#define	POSIX_readpoll	89

/* simulator support */
#define POSIX_isatty	120
#define POSIX_rta_timer	121
#define POSIX_getcwd	122
#define POSIX_addevent	123
#define POSIX_simctrl	124
#define POSIX_simsleep	125

/* sockets */
#define POSIX_accept	  200	/*(int, struct sockaddr *, int *)*/
#define POSIX_bind	  201	/*(int, struct sockaddr *, int)*/
#define POSIX_connect	  202	/*(int, struct sockaddr *, int)*/
#define POSIX_getpeername 203	/*(int, struct sockaddr *, int *)*/
#define POSIX_getsockname 204	/*(int, struct sockaddr *, int *)*/
#define POSIX_getsockopt  205	/*(int, int, int, char *, int *)*/
#define POSIX_listen	  206	/*(int, int)*/
#define POSIX_recv	  207	/*(int, char *, int, int)*/
#define POSIX_recvfrom	  208	/*(int, char *, int, int, struct sockaddr *, int *)*/
#define POSIX_send	  209	/*(int, const char *, int, int)*/
#define POSIX_sendto	  210	/*(int, const char *, int,int, const struct sockaddr*,int)*/
#define POSIX_setsockopt  211	/*(int, int, int, const char *, int)*/
#define POSIX_socket	  212	/*(int, int, int)*/
#define POSIX_shutdown	  216	/*(int, int)*/
#define POSIX_gethostname 217	/*(char *, int)*/
#define POSIX_gethostbyname_r 218 /*(char *, struct hostent *, struct hostent_data)*/
#define POSIX_gethostbyaddr_r 219 /*(char *, int, int, struct hostent *, struct hostent_data)*/
#define POSIX_select	      220 /*(int, fd_set *, fd_set *, fd_set *, struct timeval *)*/
#define POSIX_getservbyname_r 221 /*(const char *, const char*, struct servent *, struct servent_data *)*/
#define POSIX_getservbyport_r 222 /*(int, const char*, struct servent *, struct servent_data *)*/
#define POSIX_inet_addr	    223	/*unsigned long (const char *cp)*/
#define POSIX_inet_ntoa_r   224	/*int (struct in_addr in, char *buf, int buflen)*/
