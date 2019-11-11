/*	Definition module : elftypes.h

	Copyright 1994 Diab Data AB, Sweden

	Description :
	elftypes.h standard header file used by ld, ar, & as
	Defines standard types used by Elf headers

	History :
	When	Who	What
	941129	dalle	initial
*/

#ifndef D_elftypes
#define D_elftypes

/**************	Exported data, types and macros	*****************/

typedef unsigned long   Elf32_Addr;
typedef unsigned short  Elf32_Half;
typedef unsigned long   Elf32_Off;
typedef long            Elf32_Sword;
typedef unsigned long   Elf32_Word;

#endif
