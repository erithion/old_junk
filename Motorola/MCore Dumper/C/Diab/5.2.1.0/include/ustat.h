#ifndef __Iustat
#define __Iustat

#ifdef	__cplusplus
extern "C" {
#endif

struct  ustat {
	daddr_t	f_tfree;
	ino_t	f_tinode;
	char	f_fname[6];
	char	f_fpack[6];
};

extern int ustat(int, struct ustat *);

#ifdef	__cplusplus
}
#endif
#endif
