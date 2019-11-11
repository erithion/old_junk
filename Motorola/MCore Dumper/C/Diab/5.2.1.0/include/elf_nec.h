/*	Definition module : elf_nec.h

	Copyright 1996 Diab Data, Inc.

	Description :
	elf_nec.h standard header file used by ld, ar, & as

	History :
	When	Who	What
	990113	sanjay	Initial
*/

/**************	Exported data, types and macros	*****************/

/*
 * Relocation types
 */

#define R_NEC_FIRST		0
#define R_NEC_NONE		0

#define R_NEC_PCREL22		2
#define R_NEC_ADDR16_HI		3
/* #define R_NEC_PCREL32		4 */
#define R_NEC_ADDR16_LO		5
#define R_NEC_ADDR32		6
#define R_NEC_16		7
#define R_NEC_8		8
#define R_NEC_SDA_16_16_OFFSET 9
#define R_NEC_SDA_15_16_OFFSET	10
#define R_NEC_ZDA_16_16_OFFSET 	11
#define R_NEC_ZDA_15_16_OFFSET	12
#define R_NEC_TDA_6_8_OFFSET	13
#define R_NEC_TDA_7_8_OFFSET	14
#define R_NEC_TDA_7_7_OFFSET	15
#define R_NEC_TDA_16_16_OFFSET	16
#define R_NEC_ADDR16_HIADJ	17
#define R_NEC_SDA_16_16_SPLIT_OFFSET 	18
#define R_NEC_16_16_SPLIT_OFFSET 	19
#define R_NEC_PCREL26		20
#define R_NEC_LAST		21

#define R_NEC_TFIRST		0
#define R_NEC_N			(R_NEC_LAST-R_NEC_TFIRST)


#define R_NEC_DIAB_FIRST	180
#define R_NEC_DIAB_ADDR0	180
#define R_NEC_DIAB_OVERRIDE0	181
#define R_NEC_DIAB_VTBL32	182
#define R_NEC_DIAB_LAST		183

#define R_NEC_DIAB_TFIRST	(R_NEC_TFIRST+R_NEC_N)
#define R_NEC_DIAB_N		(R_NEC_DIAB_LAST-R_NEC_DIAB_FIRST)
