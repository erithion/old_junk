#ifndef __Iutmp
#define __Iutmp

#ifdef	__cplusplus
extern "C" {
#endif

#define	UTMP_FILE	"/etc/utmp"
#define	WTMP_FILE	"/etc/wtmp"
#define	ut_name	ut_user

struct utmp {
	char	ut_user[8];
	char	ut_id[4];
	char	ut_line[12];
	pid_t	ut_pid;
	short	ut_type;
#if defined(m88k) || defined(__m88k)
	char	ut_pad1[2];
#endif
	struct	exit_status {
		short	e_termination;
		short	e_exit;
	} ut_exit;
	time_t ut_time;
#if defined(m88k) || defined(__m88k)
	char	ut_pad2[24];
#endif
#ifdef	__mot68
	char	ut_host[16];
#endif
};

/* ut_type */
#define	EMPTY		0
#define	RUN_LVL		1
#define	BOOT_TIME	2
#define	OLD_TIME	3
#define	NEW_TIME	4
#define	INIT_PROCESS	5
#define	LOGIN_PROCESS	6
#define	USER_PROCESS	7
#define	DEAD_PROCESS	8
#define	ACCOUNTING	9
#define	UTMAXTYPE	9
#define FTP		128
#define REMOTE_LOGIN	129
#define REMOTE_PROCESS	130

/* ut_line specials */
#define	RUNLVL_MSG	"run-level %c"
#define	BOOT_MSG	"system boot"
#define	OTIME_MSG	"old time"
#define	NTIME_MSG	"new time"

extern struct utmp *getutent(void);
extern struct utmp *getutid(const struct utmp *);
extern struct utmp *getutline(const struct utmp *);
extern void pututline(const struct utmp *);
extern void setutent(void);
extern void endutent(void);
extern void utmpname(const char *);
extern int ttyslot(void);

#ifdef	__cplusplus
}
#endif
#endif
