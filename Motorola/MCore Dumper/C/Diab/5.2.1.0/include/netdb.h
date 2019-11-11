/*      Definition module : netdb.h
 
        Copyright 1999 Diab Data, USA
 
        Description :
        Berkeley socket standard header file
 
	4/14/99
*/

#ifndef _NETDB_H
#define	_NETDB_H

#include <netinet/in.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* note: must match defines in posix.h (rtasim) */
#define	_MAXHOSTNAMELEN	64	/* maximum length of hostname */
#define	_MAXALIASES	5	/* maximum number of aliases returned (-1) */
#define	_MAXADDRS	10	/* maximum number of addresses returned (-1) */
#define	_MAXLINELEN	128	/* maximum linelength of serv_fp file */
#define _MAXSERVLEN	64	/* maximum length of service name */
#define _MAXPROTOLEN	64	/* maximum length of protocol name */


/*
 * Structures returned by network
 * data base library.  All addresses
 * are supplied in host order, and
 * returned in network order (suitable
 * for use in system calls).
 */
struct	hostent {
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
	int	h_addrtype;	/* host address type */
	int	h_length;	/* length of address */
	char	**h_addr_list;	/* list of addresses from name server */
#define	h_addr	h_addr_list[0]	/* address, for backward compatiblity */
};

/*
 * The fields in the hostent_data structure are referenced by
 * entries in the hostent data structure after a successful
 * call to gethostent_r(), gethostbyname_r() or gethostbyaddr_r().
 * hostent_data also maintains state information between calls
 * to gethostent_r().  The contents of this structure should not
 * be referenced directly except to initialize the hostf field
 * to NULL.
 */
struct	hostent_data {
	struct in_addr host_addr;		/* host address */
	void	*hostf;				/* hosts file (really FILE*) */
	short	_nsw_src;			/* source of answer */
	short	_flags;				/* internal flags */
	char	*current;			/* current YP entry */
	int	currentlen;			/* len of current YP entry */
	char	*host_aliases[_MAXALIASES];	/* list of aliases, NULL=end */
	char	*h_addr_ptrs[_MAXADDRS];	/* list of addresses,NULL=end*/
	char	hostbuf[_MAXHOSTNAMELEN * _MAXALIASES];/* host name info buff*/
	struct in_addr hostaddr[_MAXADDRS];	/* host address list */
};

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct	netent {
	char		*n_name;	/* official name of net */
	char		**n_aliases;	/* alias list */
	int		n_addrtype;	/* net address type */
	unsigned long	n_net;		/* network # */
};

struct	servent {
	char	*s_name;	/* official service name */
	char	**s_aliases;	/* alias list */
	int	s_port;		/* port # */
	char	*s_proto;	/* protocol to use */
};

/*
 * servent_data structure for calls to getservent_r(), getservbyname_r()
 * or getservbyport_r().  The contents of this structure should not
 * be referenced directly except to initialize the serv_fp field
 * to NULL.
 */
struct  servent_data {
	void	*serv_fp;			/* serv file (really FILE*) */
	char	line[_MAXLINELEN+1];
	char	*serv_aliases[_MAXALIASES];	/* list of aliases (NULL=end)*/
	short	_nsw_src;			/* source of answer */
	short	_flags;				/* internal flags */
	char	*current;			/* current YP entry */
	int	currentlen;			/* len of current YP entry */
	char	hostbuf[_MAXSERVLEN * (_MAXALIASES+1) + 
		       _MAXPROTOLEN];		/* host name info buff*/
};

struct	protoent {
	char	*p_name;	/* official protocol name */
	char	**p_aliases;	/* alias list */
	int	p_proto;	/* protocol # */
};

/* re-entrant functions */
extern int gethostbyname_r(const char *, struct hostent *, 
			   struct hostent_data *);
extern int gethostbyaddr_r(const char *, int, int, struct hostent *,
			   struct hostent_data *);
extern int getservbyname_r(const char *, const char *, 
			   struct servent *, struct servent_data *);
extern int getservbyport_r(int, const char *, struct servent *,
			   struct servent_data *);

/* old interfaces that return a pointer to a static area; non-reentrant */
extern struct hostent	*gethostbyname(const char *);
extern struct hostent	*gethostbyaddr(const char *, int, int);
extern struct hostent	*gethostent(void);
extern struct netent	*getnetbyname(const char *);
extern struct netent	*getnetbyaddr(long, int);
extern struct netent	*getnetent(void);
extern struct servent	*getservbyname(const char *, const char *);
extern struct servent	*getservbyport(int, const char *);
extern struct servent	*getservent(void);
extern struct protoent	*getprotobyname(const char *);
extern struct protoent	*getprotobynumber(int);
extern struct protoent	*getprotoent(void);
extern int		 getnetgrent(char **, char **, char **);

extern int sethostent(int);
extern int endhostent(void);
extern int setnetent(int);
extern int endnetent(void);
extern int setservent(int);
extern int endservent(void);
extern int setprotoent(int);
extern int endprotoent(void);
extern int setnetgrent(const char *);
extern int endnetgrent(void);


/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * (left in extern int h_errno).
 */
extern  int h_errno;

#define	HOST_NOT_FOUND	1 /* Authoritative Answer Host not found */
#define	TRY_AGAIN	2 /* Non-Authoritive Host not found, or SERVERFAIL */
#define	NO_RECOVERY	3 /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define	NO_DATA		4 /* Valid name, no data record of requested type */
#define	NO_ADDRESS	NO_DATA		/* no address, look for MX record */

#ifdef	__cplusplus
}
#endif

#endif	/* _NETDB_H */
