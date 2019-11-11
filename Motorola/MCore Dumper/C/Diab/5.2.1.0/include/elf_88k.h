/*	Definition module : elf_88k.h

	Copyright 1994 Diab Data AB, Sweden

	Description :
	elf_88k.h standard header file used by ld, ar, & as
	Defines flags used by 88K ABI.

	History :
	When	Who	What
	941216	dalle	initial
*/

/**************	Exported data, types and macros	*****************/

#define R_88K_NONE		0
#define R_88K_COPY		1
#define R_88K_GOTP_ENT		2
#define R_88K_8			4
#define R_88K_8S		5
#define R_88K_16S		7
#define R_88K_DISP16		8
#define R_88K_DISP26		10
#define R_88K_PLT_DISP26	14
#define R_88K_BBASED_32		16
#define R_88K_BBASED_32UA	17
#define R_88K_BBASED_16H	18
#define R_88K_BBASED_16L	19
#define R_88K_ABDIFF_32		24
#define R_88K_ABDIFF_32UA	25
#define R_88K_ABDIFF_16H	26
#define R_88K_ABDIFF_16L	27
#define R_88K_ABDIFF_16		28
#define R_88K_32		32
#define R_88K_32UA		33
#define R_88K_16H		34
#define R_88K_16L		35
#define R_88K_16		36
#define R_88K_GOT_32		40
#define R_88K_GOT_32UA		41
#define R_88K_GOT_16H		42
#define R_88K_GOT_16L		43
#define R_88K_GOT_16		44
#define R_88K_GOTP_32		48
#define R_88K_GOTP_32UA		49
#define R_88K_GOTP_16H		50
#define R_88K_GOTP_16L		51
#define R_88K_GOTP_16		52
#define R_88K_PLT_32		56
#define R_88K_PLT_32UA		57
#define R_88K_PLT_16H		58
#define R_88K_PLT_16L		59
#define R_88K_PLT_16		60
#define R_88K_ABREL_32		64
#define R_88K_ABREL_32UA	65
#define R_88K_ABREL_16H		66
#define R_88K_ABREL_16L		67
#define R_88K_ABREL_16		68
#define R_88K_GOT_ABREL_32	72
#define R_88K_GOT_ABREL_32UA	73
#define R_88K_GOT_ABREL_16H	74
#define R_88K_GOT_ABREL_16L	75
#define R_88K_GOT_ABREL_16	76
#define R_88K_GOTP_ABREL_32	80
#define R_88K_GOTP_ABREL_32UA	81
#define R_88K_GOTP_ABREL_16H	82
#define R_88K_GOTP_ABREL_16L	83
#define R_88K_GOTP_ABREL_16	84
#define R_88K_PLT_ABREL_32	88
#define R_88K_PLT_ABREL_32UA	89
#define R_88K_PLT_ABREL_16H	90
#define R_88K_PLT_ABREL_16L	91
#define R_88K_PLT_ABREL_16	92
#define R_88K_SREL_32		96
#define R_88K_SREL_32UA		97
#define R_88K_SREL_16H		98
#define R_88K_SREL_16L		99

#define PT_88K_DEBINFADDR	0x70000001

#define DT_88K_ADDRBASE		0x70000001
#define DT_88K_PLTSTART		0x70000002
#define DT_88K_PLTEND		0x70000003
#define DT_88K_TDESC		0x70000004
