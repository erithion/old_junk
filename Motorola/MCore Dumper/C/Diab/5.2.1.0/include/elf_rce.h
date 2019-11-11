/*	Definition module : elf_rce.h

	Copyright 1996 Diab Data, Inc.

	Description :
	elf_rce.h standard header file used by ld, ar, & as
	Defines flags used by RCE ABI.

	History :
	When	Who	What
	960219	fb	Initial
*/

/**************	Exported data, types and macros	*****************/

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
