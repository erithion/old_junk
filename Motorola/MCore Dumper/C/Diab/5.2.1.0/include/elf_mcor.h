/*	Definition module : elf_mcor.h

	Copyright 1996 Diab Data, Inc.

	Description :
	elf_mcor.h standard header file used by ld, ar, & as
	Defines flags used by RCE ABI.

	History :
	When	Who	What
	970918	paulb	Initial
*/

/**************	Exported data, types and macros	*****************/

/* The ABI specifies the RCE versions still but the MCORE versions
   are here for the future?
   */

/*
 * Relocation types
 */

#define R_MCORE_FIRST		0
#define R_MCORE_NONE		0
#define R_MCORE_ADDR32		1
#define R_MCORE_PCRELIMM11BY2	3
#define R_MCORE_PCREL32		5
#define R_MCORE_RELCODE		7
#define R_MCORE_RELDATA		9
#define R_MCORE_LAST		10

#define R_MCORE_TFIRST		0
#define R_MCORE_N		(R_MCORE_LAST-R_MCORE_TFIRST)


#define R_MCORE_DIAB_FIRST	180
#define R_MCORE_DIAB_ADDR0	180
#define R_MCORE_DIAB_OVERRIDE0	181
#define R_MCORE_DIAB_VTBL32	182
#define R_MCORE_DIAB_LAST	183

#define R_MCORE_DIAB_TFIRST	(R_MCORE_TFIRST+R_MCORE_N)
#define R_MCORE_DIAB_N		(R_MCORE_DIAB_LAST-R_MCORE_DIAB_FIRST)


/* 
 * Processor specific sh_flags 
 */

#define	SHF_MCORE_READ		0x80000000

/*
 * Relocation types
 */

#define R_RCE_FIRST		0
#define R_RCE_NONE		0
#define R_RCE_ADDR32		1
#define R_RCE_PCRELIMM11BY2	3
#define R_RCE_PCREL32		5
#define R_RCE_RELCODE		7
#define R_RCE_RELDATA		9
#define R_RCE_LAST		10

#define R_RCE_TFIRST		0


/* 
 * Processor specific sh_flags 
 */

#define	SHF_RCE_READ		0x80000000
