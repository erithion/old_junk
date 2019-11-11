/*	Definition module : elf_68k.h

	Copyright 1995 Diab Data AB, Sweden

	Description :
	elf_68k.h standard header file used by ld, ar, & as
	Defines flags used by Motorola 68000 ABI.

	History :
	When	Who	What
	950110	fb	initial
*/

/**************	Exported data, types and macros	*****************/

/* 
 * Relocation types
 */

#define R_68K_FIRST	0
#define R_68K_NONE	0
#define R_68K_32	1
#define R_68K_16	2
#define R_68K_8		3
#define R_68K_PC32	4
#define R_68K_PC16	5
#define R_68K_PC8	6
#define R_68K_LAST	7

#define R_68K_TFIRST	0
#define R_68K_N		(R_68K_LAST-R_68K_TFIRST)

#define R_68K_GOT32	7
#define R_68K_GOT16	8
#define R_68K_GOT8	9
#define R_68K_GOT320	10
#define R_68K_GOT160	11
#define R_68K_GOT80	12
#define R_68K_PLT32	13
#define R_68K_PLT16	14
#define R_68K_PLT8	15
#define R_68K_PLT320	16
#define R_68K_PLT160	17
#define R_68K_PLT80	18
#define R_68K_COPY	19
#define R_68K_GLOB_DAT	20
#define R_68K_JMP_SLOT	21
#define R_68K_RELATIVE	22

#define R_68K_DIAB_FIRST 180
#define R_68K_SDA32	180
#define R_68K_SDA16	181
#define R_68K_SDA8	182
#define R_68K_0		183
#define R_68K_OVERRIDE0	184
#define R_68K_VTBL32	185
#define R_68K_DIAB_LAST 186

#define R_68K_DIAB_TFIRST	(R_68K_TFIRST+R_68K_N)
#define R_68K_DIAB_N		(R_68K_DIAB_LAST-R_68K_DIAB_FIRST)
