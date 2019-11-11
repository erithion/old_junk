#ifndef __Istdio
#define __Istdio
#include <xmacros.h>
_C_STD_BEGIN
_C_LIB_DECL

#if __GNUC__
#ifndef __VA_LIST
#define __VA_LIST
typedef __builtin_va_list va_list;
#endif
#else
#include "diab/va_list.h"
#endif

#define _IOFBF		0000
#define _IOREAD		0001
#define _IOWRT		0002
#define _IONBF		0004
#define _IOMYBUF	0010
#define _IOEOF		0020
#define _IOERR		0040
#define _IOLBF		0100
#define _IORW		0200
#define _IOBIN		0400
#define _IOUSED		01000

#define EOF		(-1)

#define BUFSIZ		1024
#define _FBUFSIZ	16

#define L_tmpnam	(sizeof("/usr/tmp/") + 15)
#define TMP_MAX		17576

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#if defined(__ppc)
#define FOPEN_MAX	30
#elif	defined(__mot68)
#define FOPEN_MAX	100
#else
#define FOPEN_MAX	72
#endif

#define _NFILE		FOPEN_MAX

#define FILENAME_MAX	512

#ifndef NULL
#define NULL		0
#endif

typedef struct {			/* buffering control structure	*/
	int		_cnt;		/* chars left before flush/read	*/
	unsigned char	*_ptr;		/* next character		*/
	unsigned char	*_base;		/* ptr to buffer base		*/
	unsigned short	_flag;		/* flags, see _IOxxx		*/
	short		_file;		/* file descriptor		*/
	int		_fcount;	/* size of _base buffer		*/
	void		*_flock;	/* semaphore for flockfile	*/
	unsigned char	_ubuf[4];	/* unget buffer			*/
} FILE;

typedef long fpos_t;			/* file pointer type		*/
#ifndef __size_t
#define	__size_t
_TYPE_size_t;
#endif

#if defined(__STRICT_ANSI__)
#define __FLSBUF	__flsbuf
#define __FILBUF	__filbuf
#else
#define __FLSBUF	_flsbuf
#define __FILBUF	_filbuf
#endif

#define stdin		(&__std_file_table[0])
#define stdout		(&__std_file_table[1])
#define stderr		(&__std_file_table[2])

extern FILE		__std_file_table[3];

extern int getc(FILE *);
extern int getc_unlocked(FILE *);
extern int putc(int, FILE *);
extern int putc_unlocked(int, FILE *);
extern int getchar(void);
extern int getchar_unlocked(void);
extern int putchar(int);
extern int putchar_unlocked(int);
extern void clearerr(FILE *);
extern int feof(FILE *);
extern int ferror(FILE *);

#if !defined(__OCS_88000__)
#define getc_unlocked(stream)	(--(stream)->_cnt < 0 ? __FILBUF(stream) : (int)*(stream)->_ptr++)
#define putc_unlocked(c, stream) (--(stream)->_cnt < 0 ? __FLSBUF((c),(stream)) : (int)(*(stream)->_ptr++ = (c)))
#define getchar_unlocked()	getc(stdin)
#define putchar_unlocked(c)	putc((c), stdout)

#define feof(stream)	((stream)->_flag & _IOEOF)
#define ferror(stream)	((stream)->_flag & _IOERR)
#endif	/* __OCS_88000__ */

extern int fclose(FILE *);
extern int fflush(FILE *);
extern int fgetc(FILE *);
extern int fgetpos(FILE *, fpos_t *);
extern char *fgets(char *, int, FILE *);
extern void flockfile(FILE *);
extern int ftrylockfile(FILE *);
extern void funlockfile(FILE *);
extern FILE *fopen(const char *, const char *);
extern int fprintf(FILE *, const char *, ...);
extern int fputc(int, FILE *);
extern int fputs(const char *, FILE *);
extern size_t fread(void *, size_t, size_t, FILE *);
extern FILE *freopen(const char*, const char *, FILE *);
extern int fscanf(FILE *, const char *, ...);
extern int fseek(FILE *, long, int);
extern int fsetpos(FILE *, const fpos_t *);
extern long ftell(FILE *);
extern size_t fwrite(const void *, size_t, size_t, FILE *);
extern char *gets(char *);
extern void perror(const char *);
extern int printf(const char *, ...);
extern int puts(const char *);
extern int remove(const char *);
extern int rename(const char *, const char *);
extern void rewind(FILE *);
extern int scanf(const char *, ...);
extern void setbuf(FILE *, char *);
extern int setvbuf(FILE *, char *, int, size_t);
extern int sprintf(char *, const char *, ...);
extern int sscanf(const char *, const char *, ...);
extern FILE *tmpfile(void);
extern char *tmpnam(char *s);
extern int ungetc(int, FILE *);

extern int vprintf(const char *, va_list);
extern int vfprintf(FILE *, const char *, va_list);
extern int vsprintf(char *, const char *, va_list);
extern int vscanf(const char *, va_list);
extern int vfscanf(FILE *, const char *, va_list);
extern int vsscanf(const char *, const char *, va_list);


extern int __FLSBUF(int c, FILE *);
extern int __FILBUF(FILE *);

#if !defined(__STRICT_ANSI__)
#define P_tmpdir	"/usr/tmp/"
#define L_ctermid	9
#define L_cuserid	9


extern int fileno(FILE *);
#define fileno(stream)	((stream)->_file)

extern char *ctermid(char *);
extern FILE *fdopen(int, const char *);
extern int getw(FILE *);
extern int pclose(FILE *);
extern FILE *popen(const char *, const char *);
extern int putw(int, FILE *);
extern char *tempnam(const char *, const char *);
extern char *mktemp(char *);
extern int getopt(int, char *const *, const char *);
#if	defined(__Istdarg) || defined(__Ivarargs)
extern int vscanf(const char *, va_list);
extern int vfscanf(FILE *, const char *, va_list);
extern int vsscanf(const char *, const char *, va_list);
#endif

#endif	/* __STRICT_ANSI__ */

_END_C_LIB_DECL
_C_STD_END
#endif
