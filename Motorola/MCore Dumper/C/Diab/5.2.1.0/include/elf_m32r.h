/*	Definition module : elf_m32r.h

	Copyright 1997 Diab Data, Inc.

	Description :
	elf_m32r.h standard header file used by ld, ar, & as
	Defines flags used by M32R relocations.
	
	File is in both tools/include and dlib/include/include.unx

	History :
	When	Who	What
	970806	paulb	Initial
*/

/**************	Exported data, types and macros	*****************/

/*
 * Relocation types
 */

#define R_M32R_FIRST		0
#define R_M32R_NONE		0
#define R_M32R_ADDR16		1
#define R_M32R_ADDR32		2
#define R_M32R_ADDR24		3
#define R_M32R_DISP8		5
#define R_M32R_DISP24		6
#define R_M32R_LAST		7

#define R_M32R_TFIRST		0
#define R_M32R_N		(R_M32R_LAST-R_M32R_TFIRST)


#define R_M32R_DIAB_FIRST	180
#define R_M32R_DIAB_ADDR0	180
#define R_M32R_DIAB_OVERRIDE0	181
#define R_M32R_DIAB_VTBL32	182
#define R_M32R_DIAB_ADDR16_HA	183	/* @ha */
#define R_M32R_DIAB_ADDR16_HI	184	/* @hi */
#define R_M32R_DIAB_ADDR16_LO	185	/* @l */
#define R_M32R_DIAB_SCA_16	186	/* @code */
#define R_M32R_DIAB_SCA_16_HA	187	/* @code@ha */
#define R_M32R_DIAB_SCA_16_HI	188	/* @code@h */
#define R_M32R_DIAB_SCA_16_LO	189	/* @code@l */
#define R_M32R_DIAB_SDA_16	190	/* @data */
#define R_M32R_DIAB_SDA_16_HA	191	/* @data@ha */
#define R_M32R_DIAB_SDA_16_HI	192	/* @data@h */
#define R_M32R_DIAB_SDA_16_LO	193	/* @data@l */
#define R_M32R_DIAB_LAST	194

#define R_M32R_DIAB_TFIRST	(R_M32R_TFIRST+R_M32R_N)
#define R_M32R_DIAB_N		(R_M32R_DIAB_LAST-R_M32R_DIAB_FIRST)

/* 
 * Processor specific sh_flags 
 */

#define	SHF_M32R_READ		0x80000000
