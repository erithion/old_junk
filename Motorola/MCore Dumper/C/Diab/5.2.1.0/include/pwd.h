#ifndef __Ipwd
#define __Ipwd

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __Itypes
#include <sys/types.h>
#endif
#ifndef __Istdio
#include <stdio.h>
#endif

struct passwd {
	char	*pw_name;
	char	*pw_passwd;
#ifdef __mot68
	int	pw_uid;
	int	pw_gid;
#else
	uid_t	pw_uid;
	gid_t	pw_gid;
#endif
	char	*pw_age;
	char	*pw_comment;
	char	*pw_gecos;
	char	*pw_dir;
	char	*pw_shell;
};

struct comment {
	char	*c_dept;
	char	*c_name;
	char	*c_acct;
	char	*c_bin;
};

extern struct passwd *getpwuid(uid_t);
extern struct passwd *getpwnam(const char *);

/* Not in POSIX: */
extern struct passwd *getpwent(void);
extern struct passwd *fgetpwent(FILE *);
extern void setpwent(void);
extern void endpwent(void);
extern int putpwent(const struct passwd *, FILE *);

extern int getpw(int, char *);

#ifdef	__cplusplus
}
#endif
#endif
