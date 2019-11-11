#ifndef __Imon
#define __Imon

#ifdef	__cplusplus
extern "C" {
#endif

struct hdr {
	char	*lpc;
	char	*hpc;
	int	nfns;
};

struct cnt {
	char	*fnpc;
	long	mcnt;
};

typedef unsigned short WORD;

#define MON_OUT	"mon.out"
#define MPROGS0	(150 * sizeof(WORD))
#define MSCALE0	4
#ifndef NULL
#define NULL	0
#endif

extern void monitor(int (*)(), int (*)(), WORD *, int, int);

#ifdef	__cplusplus
}
#endif
#endif
