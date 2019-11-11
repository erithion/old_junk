/*	Definition module : elf_sh.h

	Copyright 1999 Diab Data, Inc.

	Description :
	elf_sc.h standard header file used by ld, ar, & as
	Defines flags used by SC ABI.

	History :
	When	Who	What
	000227	ds	Initial
*/

/**************	Exported data, types and macros	*****************/

/*
 * e_flags
 */
#define EF_SC_EFLAGS	0x80
/*
* Relocation types
 */

#define R_STARCORE_FIRST	0
#define R_STARCORE_NONE		0	/* same as R_PPC_NONE */
#define R_STARCORE_DIRECT_8	1
#define R_STARCORE_DIRECT_16	2
#define R_STARCORE_DIRECT_32    3
#define R_STARCORE_R9_1_1	4
#define R_STARCORE_R11_1_1	5
#define R_STARCORE_R17_1_1	6
#define R_STARCORE_R21_1_1	7
#define R_STARCORE_S7_0_0	8
#define R_STARCORE_S15_0_0	9
#define R_STARCORE_S15_1_0	10
#define R_STARCORE_S15_2_0	11
#define R_STARCORE_S16_0_0	12
#define R_STARCORE_S16_1_0	13
#define R_STARCORE_T16_0_0	14
#define R_STARCORE_S32_0_0	15
#define R_STARCORE_U4_1_1	16
#define R_STARCORE_U5_2_2	17
#define R_STARCORE_U5_0_0	18
#define R_STARCORE_U6_1_1	19
#define R_STARCORE_U6_0_0	20
#define R_STARCORE_U7_1_1	21
#define R_STARCORE_U8_2_2	22
#define R_STARCORE_V6_0_0	23
#define R_STARCORE_W6_0_0	24
#define R_STARCORE_U16_0_0	25
#define R_STARCORE_U16_1_0	26
#define R_STARCORE_U16_2_0	27
#define R_STARCORE_V16_0_0	28
#define R_STARCORE_N16_0_0	29
#define R_STARCORE_O16_0_0	30
#define R_STARCORE_U32_0_0	31
#define R_STARCORE_U32_1_0	32
#define R_STARCORE_U32_2_0	33
#define R_STARCORE_U32_16_16	33
#define R_STARCORE_LAST		34

#define R_STARCORE_TFIRST	0
#define R_STARCORE_N		(R_STARCORE_LAST-R_STARCORE_TFIRST)


#define R_STARCORE_DIAB_FIRST		180
#define R_STARCORE_DIAB_ADDR0		180
#define R_STARCORE_DIAB_OVERRIDE0	181
#define R_STARCORE_DIAB_VTBL32		182
#define R_STARCORE_DIAB_LAST		183

#define R_STARCORE_DIAB_TFIRST		(R_STARCORE_TFIRST+R_STARCORE_N)
#define R_STARCORE_DIAB_N		(R_STARCORE_DIAB_LAST-R_STARCORE_DIAB_FIRST)
