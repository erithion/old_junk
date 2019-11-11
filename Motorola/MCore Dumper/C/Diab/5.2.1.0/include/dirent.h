#ifndef __Idirent
#define __Idirent

#ifdef	__cplusplus
extern "C" {
#endif

#include	<sys/types.h>
#include	<sys/dirent.h>

#ifdef	__mot68
#define	MAXNAMLEN	512
#define	DIRBUF		1024
#else
#define	DIRBUF		8192
#endif

typedef struct {
	int	dd_fd;			/* file descriptor	*/
	int	dd_loc;			/* location in buffer	*/
	int	dd_size;		/* bytes in buffer	*/
	char	*dd_buf;		/* points to buffer	*/
}	DIR;

extern int closedir(DIR *);
extern DIR *opendir(const char *);
extern struct dirent *readdir(DIR *);
extern long telldir(DIR *);
extern void seekdir(DIR *, long);
extern void rewinddir(DIR *);

#ifdef	__cplusplus
}
#endif
#endif
