#ifndef __Ifilehdr
#define __Ifilehdr

struct filehdr {
	unsigned short	f_magic;	/* magic		*/
	unsigned short	f_nscns;	/* number of sections	*/
	long		f_timdat;	/* date stamp		*/
	long		f_symptr;	/* fileptr to symtab	*/
	long		f_nsyms;	/* symtab count		*/
	unsigned short	f_opthdr;	/* sizeof(optional hdr) */
	unsigned short	f_flags;	/* flags		*/
};


#define  F_RELFLG	0x0001		/* reloc info stripped		*/
#define  F_EXEC		0x0002		/* file is executable		*/
#define  F_LNNO		0x0004		/* lnno stripped		*/
#define  F_LSYMS	0x0008		/* locals stripped		*/
#define  F_MINMAL	0x0010
#define  F_UPDATE	0x0020
#define  F_SWABD	0x0040
#define  F_DEMO		0x0040		/* demo executable		*/
#define  F_AR16WR	0x0080
#define  F_AR32WR	0x0100		/* VAX byte ordering		*/
#define  F_AR32W	0x0200		/* Motorola byte ordering	*/
#define  F_PATCH	0x0400
#define  F_NODF		0x0400
#define  F_EXTSYM	0x1000		/* -Xextern-in-place used	*/
#define  F_PIC		0x4000		/* Position independent refs	*/


/*	Magic Numbers	*/


/*	Motorola 68000	*/
#define	MC68MAGIC	0520
#define	MC68TVMAGIC	0521
#define MC68WRMAGIC	0620			
#define MC68ROMAGIC	0625
#define MC68KWRMAGIC	0620			
#define MC68KPGMAGIC	0620			
#define MC68KROMAGIC	0625
#define	MC68020MAGIC	0630
#define	M68MAGIC	0210
#define	M68TVMAGIC	0211

/*	Motorola 88k	*/
#define MC88MAGIC	0555	/* BCS july 88 */

#define MC88MAGOLD	0540

/*	PowerPC COFF	*/
#define	PPCCOFFMAGIC	0560

/*	GCC ARM COFF	*/
#define	ARMCOFFMAGIC	0x000a


#define ISCOFF(x) ((x)==MC88MAGIC || (x)==MC68020MAGIC || (x)==MC68ROMAGIC || (x)==MC68WRMAGIC || (x)==PPCCOFFMAGIC || (x) == ARMCOFFMAGIC)

#define	FILHDR	struct filehdr
#define	FILHSZ	sizeof(FILHDR)

#endif
