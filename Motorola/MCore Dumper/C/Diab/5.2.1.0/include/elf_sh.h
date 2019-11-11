/*	Definition module : elf_sh.h

	Copyright 1999 Diab Data, Inc.

	Description :
	elf_sh.h standard header file used by ld, ar, & as
	Defines flags used by SH ABI.

	History :
	When	Who	What
	990617	ds	Initial
*/

/**************	Exported data, types and macros	*****************/

/*
* Relocation types
 */

#define R_SH_FIRST		0
#define R_SH_NONE		0
#define R_SH_ADDR32		1	/* .long var */
#define R_SH_PCRELIMM12BY2	3	/* future */
#define R_SH_RELCODE            4       /* code relative */
#define R_SH_RELDATA            5       /* data relative */
#define R_SH_LAST		10

#define R_SH_TFIRST		0
#define R_SH_N		(R_SH_LAST-R_SH_TFIRST)


#define R_SH_DIAB_FIRST	180
#define R_SH_DIAB_ADDR0	180
#define R_SH_DIAB_OVERRIDE0	181
#define R_SH_DIAB_VTBL32	182
#define R_SH_DIAB_LAST	183

#define R_SH_DIAB_TFIRST	(R_SH_TFIRST+R_SH_N)
#define R_SH_DIAB_N		(R_SH_DIAB_LAST-R_SH_DIAB_FIRST)
