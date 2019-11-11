/*	Definition module : elf_arm.h

	Copyright 2000 Diab Data, Inc.

	Description :
	elf_arm.h standard header file used by ld, ar, & as

	History :
	When	Who	What
	000203	sandeep	Initial
*/

/**************	Exported data, types and macros	*****************/

/*
 * Relocation types for ARM Family (ABI)
 */


#define R_ARM_FIRST		0
#define R_ARM_NONE		0
#define R_ARM_PC24		1
#define R_ARM_ABS32             2
#define R_ARM_REL32             3
#define R_ARM_PC13              4
#define R_ARM_ABS16             5
#define R_ARM_ABS12             6
#define R_ARM_THM_ABS5          7
#define R_ARM_ABS8              8
#define R_ARM_SBREL32           9
#define R_ARM_THM_PC22          10
#define R_ARM_THM_PC8           11
#define R_ARM_AMP_VCALL9        12
#define R_ARM_SWI24             13
#define R_ARM_THM_SWI8          14
#define R_ARM_XPC25             15
#define R_ARM_THM_XPC22         16
#define R_ARM_LAST		17

#define R_ARM_TFIRST		0
#define R_ARM_N			(R_ARM_LAST-R_ARM_TFIRST)


#define R_ARM_DIAB_FIRST	17
#define R_ARM_DIAB_ADDR0	17
#define R_ARM_DIAB_OVERRIDE0	18
#define R_ARM_DIAB_VTBL32	19
#define R_ARM_DIAB_LAST         20

#define R_ARM_DIAB_TFIRST	(R_ARM_TFIRST+R_ARM_N)
#define R_ARM_DIAB_N		(R_ARM_DIAB_LAST-R_ARM_DIAB_FIRST)
