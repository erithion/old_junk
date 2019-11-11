#ifndef __Iregexp
#define __Iregexp

#ifdef	__cplusplus
extern "C" {
#endif

/*
 *	Simple Regular Expression Compiler and Executor.
 *	------------------------------------------------
 *
 *	This code is written in accordance with the
 *	specification in SVID.
 *
 *	Written by Felix Burton, 1991-03-10.
 *	CopyRight 1991 Diab Data AB
 */

char *loc1;
char *loc2;
char *locs;

#define	RegExp_End	((ExpType)0)
#define	RegExp_Char	((ExpType)1)
#define	RegExp_Bol	((ExpType)2)
#define	RegExp_Eol	((ExpType)3)
#define	RegExp_Any	((ExpType)4)
#define	RegExp_Set	((ExpType)5)
#define	RegExp_LPar	((ExpType)6)
#define	RegExp_RPar	((ExpType)7)
#define	RegExp_Ptr	((ExpType)8)
#define	RegExp_Range	((ExpType)16)

#define	ExpType		unsigned char
#define	MaxPar		10
#define	ValMax		((ValType)(-1))
#define	ValType		unsigned long
#define	ValSize		(sizeof(ValType))
#define	BitPerChar	8
#define	SetSize		((1 << BitPerChar) / BitPerChar)
#define	InSet(c, s)	((s)[c / BitPerChar] & (1 << (c % BitPerChar)))
#define	SetSet(c, s)	((s)[c / BitPerChar] |= (1 << (c % BitPerChar)))

#define	getval(ep)	(((ValType)(ep)[0] << (3*BitPerChar)) | \
			 ((ValType)(ep)[1] << (2*BitPerChar)) | \
			 ((ValType)(ep)[2] << (1*BitPerChar)) | \
			 ((ValType)(ep)[3] << (0*BitPerChar)))
#define	putval(ep, val)	(void)(((ep)[0] = (ExpType)((val) >> (3*BitPerChar))), \
			       ((ep)[1] = (ExpType)((val) >> (2*BitPerChar))), \
			       ((ep)[2] = (ExpType)((val) >> (1*BitPerChar))), \
			       ((ep)[3] = (ExpType)((val) >> (0*BitPerChar))))

static ExpType *
__evaluate(ExpType *sp, ExpType *ep, int end)
{
	int c;
	ValType min;
	ValType max;
	ExpType *sp1;
	ExpType *minsp;

	while((c = *ep++) != end) {
		switch(c) {
		case RegExp_Char:
			if(*sp++ != *ep++) {
				return (ExpType *)0;
			}
			continue;

		case RegExp_Char | RegExp_Range:
			min = getval(ep + 1);
			max = getval(ep + 1 + ValSize) - min;
			c = *ep++;
			while(min--) {
				if(*sp++ != c) {
					return (ExpType *)0;
				}
			}
			minsp = sp;
			while(*sp++ == c && max--) ;
			break;

		case RegExp_Bol:
			continue;

		case RegExp_Eol:
			if(*sp != '\0') {
				return (ExpType *)0;
			}
			continue;

		case RegExp_Any:
			if(*sp++ == '\0') {
				return (ExpType *)0;
			}
			continue;

		case RegExp_Any | RegExp_Range:
			min = getval(ep);
			max = getval(ep + ValSize) - min;
			while(min--) {
				if(*sp++ == '\0') {
					return (ExpType *)0;
				}
			}
			minsp = sp;
			while(*sp++ != '\0' && max--) ;
			break;

		case RegExp_Set:
			if(!InSet(*sp, ep)) {
				return (ExpType *)0;
			}
			ep += SetSize;
			sp++;
			continue;

		case RegExp_Set | RegExp_Range:
			min = getval(ep + SetSize);
			max = getval(ep + SetSize + ValSize) - min;
			while(min--) {
				if(!InSet(*sp, ep)) {
					return (ExpType *)0;
				}
				sp++;
			}
			minsp = sp;
			while(InSet(*sp, ep) && max--) {
				sp++;
			}
			ep += SetSize;
			sp++;
			break;

		case RegExp_LPar:
			continue;

		case RegExp_RPar:
			continue;

		case RegExp_Ptr:
			if((sp = __evaluate(sp, ep - getval(ep), RegExp_RPar)) == (ExpType *)0) {
				return (ExpType *)0;
			}
			ep += ValSize;
			continue;

		case RegExp_Ptr | RegExp_Range:
			min = getval(ep + ValSize);
			max = getval(ep + ValSize + ValSize) - min;
			while(min--) {
				if((sp = __evaluate(sp, ep - getval(ep), RegExp_RPar)) == (ExpType *)0) {
					return (ExpType *)0;
				}
			}
			minsp = sp;
			while((sp1 = __evaluate(sp, ep - getval(ep), RegExp_RPar)) != (ExpType *)0 && max--) {
				sp = sp1;
			}
			ep += ValSize;
			sp++;
			break;

		default:
			write(2, "unknown regular expression type\n", 32);
			return (ExpType *)0;
		}
		ep += ValSize + ValSize;
		if((ExpType *)locs > minsp && (ExpType *)locs < sp) {
			minsp = (ExpType *)locs;
		}
		while(--sp >= minsp) {
			if((sp1 = __evaluate(sp, ep, end)) != (ExpType *)0) {
				return(sp1);
			}
		}
		return (ExpType *)0;
	}
	return sp;
}

char *
compile(char *instring, char *expbuf, char *endbuf, int eof)
{
	INIT
	ExpType *bp;
	ExpType *ep;
	ExpType *lp;
	ExpType *etab[MaxPar];
	int c;
	int i;
	int lc;
	int lpcnt;
	int rpcnt;
	ValType val;

	bp = (ExpType *)expbuf;
	ep = (ExpType *)endbuf;

	if(PEEKC() == '^') {
		c = GETC();
		if(bp + 1 > ep) {
			ERROR(50);
		}
		*bp++ = RegExp_Bol;
	}
	lp = bp;

	lpcnt = 0;
	rpcnt = 0;
	while((c = (ExpType)GETC()) != eof && c != '\0') {
		switch(c) {
		case '$':
			if(PEEKC() != eof && PEEKC() != '\0') {
				break;
			}
			if(bp + 1 > ep) {
				ERROR(50);
			}
			*bp++ = RegExp_Eol;
			lp = bp;
			continue;

		case '*':
			if(bp == lp) {
				break;
			}
			if(bp + ValSize + ValSize > ep) {
				ERROR(50);
			}
			*lp |= RegExp_Range;
			putval(bp, 0);
			bp += ValSize;
			putval(bp, ValMax);
			bp += ValSize;
			lp = bp;
			continue;

		case '.':
			if(bp + 1 > ep) {
				ERROR(50);
			}
			lp = bp;
			*bp++ = RegExp_Any;
			continue;

		case '[':
			if(bp + 1 + SetSize > ep) {
				ERROR(50);
			}

			lp = bp;
			*bp++ = RegExp_Set;

			for(i = 0; i < SetSize; i++) {
				bp[i] = 0;
			}

			if((c = PEEKC()) == '^') {
				c = GETC();
				i = 1;
			} else {
				i = 0;
			}

			lc = 0;
			while((c = (ExpType)GETC()) != '\0' && c != ']') {
				if(c == '-' && lc != 0 && PEEKC() != ']') {
					c = GETC();
					while(lc < c) {
						SetSet(lc, bp);
						lc++;
					}
				}
				SetSet(c, bp);
				lc = c;
			}

			if(c != ']') {
				ERROR(49);
			}

			if(i) {
				SetSet('\0', bp);
				for(i = 0; i < SetSize; i++) {
					bp[i] = ~bp[i];
				}
			}
			bp += SetSize;
			continue;

		case '\\':
			switch(c = GETC()) {
			case '{':
				if(bp == lp) {
					break;
				}
				if(bp + ValSize + ValSize > ep) {
					ERROR(50);
				}
				i = 0;
				val = 0;
				while((c = GETC()) != ',' && c != '\\') {
					if(c < '0' || c > '9') {
						ERROR(16);
					}
					val = val*10 + c - '0';
					i = 1;
				}
				if(i == 0) {
					ERROR(16);
				}
				putval(bp, val);
				bp += ValSize;

				if(c == ',') {
					i = 0;
					val = 0;
					while((c = GETC()) != '\\') {
						if(c < '0' || c > '9') {
							ERROR(16);
						}
						val = val*10 + c - '0';
						i = 1;
					}
					if(i == 0) {
						val = ValMax;
					}
				}
				putval(bp, val);
				bp += ValSize;

				if(GETC() != '}') {
					ERROR(45);
				}
				if(getval(bp - 2*ValSize) > getval(bp - ValSize)) {
					ERROR(46);
				}
				*lp |= RegExp_Range;
				lp = bp;
				continue;

			case '(':
				if(lpcnt >= MaxPar) {
					ERROR(43);
				}
				if(bp + 1 > ep) {
					ERROR(50);
				}
				etab[lpcnt++] = bp;
				*bp++ = RegExp_LPar;
				lp = bp;
				continue;

			case ')':
				if(lpcnt != rpcnt + 1) {
					ERROR(42);
				}
				if(bp + 1 > ep) {
					ERROR(50);
				}
				*bp++ = RegExp_RPar;
				rpcnt++;
				lp = bp;
				continue;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				c -= '1';
				if(c >= rpcnt) {
					ERROR(25);
				}
				if(bp + 1 + ValSize > ep) {
					ERROR(50);
				}
				lp = bp;
				*bp++ = RegExp_Ptr;
				putval(bp, bp - etab[c]);
				bp += ValSize;
				continue;

			default:
				break;
			}
			break;
		}

		if(bp + 2 > ep) {
			ERROR(50);
		}
		lp = bp;
		*bp++ = RegExp_Char;
		*bp++ = c;
	}

	if(bp + 1 > ep) {
		ERROR(50);
	}
	*bp++ = RegExp_End;
	RETURN((char *)bp);
}

int
step(char *string, char *expbuf)
{
	ExpType *sp1;
	ExpType *sp;
	ExpType *ep;
	int c;

	sp = (ExpType *)string;
	ep = (ExpType *)expbuf;
	switch(*ep) {
	case RegExp_Bol:
		if((sp1 = __evaluate(sp, ep, RegExp_End)) != (ExpType *)0) {
			loc1 = (char *)sp;
			loc2 = (char *)sp1;
			return 1;
		}
		break;

	case RegExp_Char:
		c = ep[1];
		do {
			if(*sp == c && (sp1 = __evaluate(sp, ep, RegExp_End)) != (ExpType *)0) {
				loc1 = (char *)sp;
				loc2 = (char *)sp1;
				return 1;
			}
		} while(*sp++ != '\0');
		break;

	default:
		do {
			if((sp1 = __evaluate(sp, ep, RegExp_End)) != (ExpType *)0) {
				loc1 = (char *)sp;
				loc2 = (char *)sp1;
				return 1;
			}
		} while(*sp++ != '\0');
		break;
	}
	return 0;
}

int
advance(char *string, char *expbuf)
{
	ExpType *sp;

	if((sp = __evaluate((ExpType *)string, (ExpType *)expbuf, RegExp_End)) == (ExpType *)0) {
		return 0;
	}
	loc2 = (char *)sp;
	return 1;
}

#undef	RegExp_End
#undef	RegExp_Char
#undef	RegExp_Bol
#undef	RegExp_Eol
#undef	RegExp_Any
#undef	RegExp_Set
#undef	RegExp_LPar
#undef	RegExp_RPar
#undef	RegExp_Ptr
#undef	RegExp_Range
#undef	ExpType
#undef	MaxPar
#undef	ValMax
#undef	ValType
#undef	ValSize
#undef	BitPerChar
#undef	SetSize
#undef	InSet
#undef	SetSet
#undef	getval
#undef	putval

#ifdef	__cplusplus
}
#endif
#endif
