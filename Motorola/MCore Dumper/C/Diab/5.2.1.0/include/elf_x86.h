/*	Definition module : elf_x86.h

	Copyright 2000 Wind RIver, Inc.

	Description :
	elf_x86.h standard header file used by ld, ar, & as

	History :
	When	        Who	What
	02-06-2001	igor	Initial
*/

/**************	Exported data, types and macros	*****************/

/*
 * Relocation types for x86/Pentium Family (ABI)
 */


#define R_386_FIRST		0
#define R_386_NONE		0
#define R_386_32		1
#define R_386_PC32              2
#define R_386_GOT32             3
#define R_386_PLT32             4
#define R_386_COPY              5
#define R_386_GLOB_DAT          6
#define R_386_JMP_SLOT          7
#define R_386_RELATIVE          8
#define R_386_GOTOFF            9
#define R_386_GOTPC             10
#define R_386_LAST		11

#define R_386_TFIRST		0
#define R_386_N			(R_386_LAST-R_386_TFIRST)


#define R_386_DIAB_FIRST	11
#define R_386_DIAB_ADDR0	11
#define R_386_DIAB_OVERRIDE0	12
#define R_386_DIAB_VTBL32	13
#define R_386_DIAB_LAST         14

#define R_386_DIAB_TFIRST	(R_386_TFIRST+R_386_N)
#define R_386_DIAB_N		(R_386_DIAB_LAST-R_386_DIAB_FIRST)
