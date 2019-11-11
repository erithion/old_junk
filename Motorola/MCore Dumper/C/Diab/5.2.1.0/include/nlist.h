#ifndef __Inlist
#define __Inlist

#ifdef	__cplusplus
extern "C" {
#endif

struct nlist
{
	char		*n_name;	/* symbol name		*/
	long		n_value;	/* value of symbol	*/
	short		n_scnum;	/* section number	*/
	unsigned short	n_type;		/* type and derived type*/
	char		n_sclass;	/* storage class	*/
	char		n_numaux;	/* # of aux. entries	*/
};

extern int nlist(const char *, struct nlist *);

#ifdef	__cplusplus
}
#endif
#endif
