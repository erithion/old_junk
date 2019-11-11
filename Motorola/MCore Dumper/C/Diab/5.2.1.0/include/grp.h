#ifndef __Igrp
#define __Igrp

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __Itypes
#include <sys/types.h>
#endif
#ifndef __Istdio
#include <stdio.h>
#endif

struct group {
	char	*gr_name;	/* name of the group	*/
	char	*gr_passwd;	/* group password	*/
	int	gr_gid;		/* group ID		*/
	char	**gr_mem;	/* array of members	*/
};

extern struct group *getgrgid(gid_t), *getgrnam(const char *);

/* Not in POSIX: */
extern struct group *getgrent(void), *fgetgrent(FILE *);
extern void endgrent(void);
extern void setgrent(void);

#ifdef	__cplusplus
}
#endif
#endif
