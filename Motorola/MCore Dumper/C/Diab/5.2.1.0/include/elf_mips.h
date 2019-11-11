/*	Definition module : elf_mips.h

	Copyright 1996 Diab Data, USA

	Description :
	elf_mips.h standard header file used by ld, ar, & as
	Defines flags used by 

	History :
	When	Who	What
	960411	paulb	initial
*/

/**************	Exported data, types and macros	*****************/

/*
 * Relocation types
 */
/* ABI/DIAB reloc modes */

#define R_MIPS_NONE		0
#define R_MIPS_16		1	/* .short var */
#define R_MIPS_32		2	/* .long var */
#define R_MIPS_26		4	/* jal func */
#define R_MIPS_HI16		5	/* %hi(var) */
#define R_MIPS_LO16		6	/* %lo(var) */
#define R_MIPS_PC16		10	/* bal func */

/* Diab reloc modes is a superset */
#define R_MIPS_DIAB_FIRST	0
#define R_MIPS_DIAB_ADDR16	1	/* .short var */
#define R_MIPS_DIAB_ADDR32	2	/* .long var */
#define R_MIPS_DIAB_ADDR26	4	/* jal func */
#define R_MIPS_DIAB_ADDR16_HA	5	/* %hi(var) */
#define R_MIPS_DIAB_ADDR16_LO	6	/* %lo(var) */
#define R_MIPS_DIAB_PC16	10	/* bal func */
#define R_MIPS_DIAB_ADDR16_HI	12
#define R_MIPS_DIAB_ADDR0	13
#define R_MIPS_DIAB_OVERRIDE0	14
#define R_MIPS_DIAB_VTBL32	15
#define R_MIPS_DIAB_LAST	16

#define R_MIPS_DIAB_TFIRST	0
#define R_MIPS_DIAB_N		(R_MIPS_DIAB_LAST-R_MIPS_DIAB_FIRST)

#define R_MIPS_SDA_FIRST	245
#define R_MIPS_SCA_16		245	/* %picoff(var) */
#define R_MIPS_SCA_16_HA	246	/* %pichi(var) */
#define R_MIPS_SCA_16_HI	247
#define R_MIPS_SCA_16_LO	248	/* %piclo(var) */
#define R_MIPS_SDA_16		251	/* %sdaoff(var) */
#define R_MIPS_SDA_16_HA	252	/* %pidhi(var) */
#define R_MIPS_SDA_16_HI	253
#define R_MIPS_SDA_16_LO	254	/* %pidlo(var) */
#define R_MIPS_SDA_LAST		255

#define R_MIPS_SDA_TFIRST	(R_MIPS_DIAB_TFIRST+R_MIPS_DIAB_N)
#define R_MIPS_SDA_N		(R_MIPS_SDA_LAST-R_MIPS_SDA_FIRST)

/* mips16 relocations */

#define	R_MIPS16_FIRST		100 
#define	R_MIPS16_26		100  	/* mips16 jal */
#define	R_MIPS16_SDA		101  	/* %sdaoff(var) */
#define	R_MIPS16_LAST		102 

#define R_MIPS16_TFIRST		(R_MIPS_SDA_TFIRST+R_MIPS_SDA_N)
#define R_MIPS16_N		(R_MIPS16_LAST-R_MIPS16_FIRST)

#define STO_MIPS16_SYM		240

/*
  SGI relocation modes. Not supported
*/
#if 0
#define R_MIPS_FIRST		0
#define R_MIPS_NONE		0
#define R_MIPS_REFHALF       1
#define R_MIPS_REFWORD       2
#define R_MIPS_JMPADDR       3
#define R_MIPS_REFHI         4
#define R_MIPS_REFLO         5
#define R_MIPS_GPREL         6
#define R_MIPS_LITERAL       7
#define R_MIPS_REL32         8
#define R_MIPS_REFGOT        R_MIPS_REL32     /* alias for compatibility */
#define R_MIPS_REFHI_64      9
#define R_MIPS_REFLO_64      10
#define R_MIPS_REFWORD_64    11
#define R_MIPS_PC16          12
#define R_MIPS_RELHI         13
#define R_MIPS_RELLO         14
#define R_MIPS_REFSHFT       15
#define R_MIPS_REFHI_ADDEND  16          /* lo value is in immed of inst */
#define R_MIPS_SHFTCNT       17
#define R_MIPS_MULTRELHI     18
#define R_MIPS_MULTRELLO     19
#define R_MIPS_DATA16        20
#define R_MIPS_LAST	21
#endif
