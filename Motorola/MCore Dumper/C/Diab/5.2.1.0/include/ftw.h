#ifndef __Iftw
#define __Iftw

#ifdef	__cplusplus
extern "C" {
#endif

/* type of file in ftw */
#define	FTW_F	0	/* file		*/
#define	FTW_D	1	/* directory	*/
#define	FTW_DNR	2	/* dir. no read	*/
#define	FTW_NS	3	/* stat failed	*/

extern int ftw(const char *, int (*)(), int);

#ifdef	__cplusplus
}
#endif
#endif
