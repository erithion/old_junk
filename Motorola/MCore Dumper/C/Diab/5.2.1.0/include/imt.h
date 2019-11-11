/*	Definition module : imt.h

	Copyright 1996 Diab Data, Inc.

	Description :
	Contains definitions for IMT shared libraries

	History :
	When	Who	What
	960530	teve	initial
*/

#ifndef D_imt
#define D_imt
/**************	Imports to declare exports **********************/

#undef EXTERN
#ifdef M_imt
#define EXTERN
#else
#define EXTERN extern
#endif

/**************	Exported data, types and macros	*****************/

/* The IMT Info code contains a 1 byte opcode with optional 
   operands. The lower 3 bits defines the length of the operand,
   so that the byte code can be expanded without making the readers
   outdated.
   All operands are big-endian.
*/

/* sizes */
#define IMT_SMASK	0x7	/* size mask			*/
#define IMT_0WORD	0x0
#define IMT_1WORD	0x1	/* 1 32 bit word follows	*/
#define IMT_2WORD	0x2	/* 2 32 bit words follows	*/
#define IMT_BSIZE	0x4	/* 1 byte with real size follows*/
#define IMT_WSIZE	0x5	/* 1 word with real size follows*/
#define IMT_ZTERM	0x6	/* null terminated string	*/
#define IMT_ZWORD	0x7	/* null string + word		*/

/* commands */
/*
   Create a DA with the size CONST.
*/
#define IMT_DASIZE	(0x10 | IMT_1WORD)

/*
   Copy initialized data from address CA+CONST1 to DA with the
   size CONST2.
*/
#define IMT_DAINIT	(0x18 | IMT_2WORD)

/*
   Calculate the base address of the SDA by adding the DA
   address with CONST. Put this address in the DAT.
*/
#define IMT_SDABASE	(0x20 | IMT_1WORD)

/*
   Store the address to the SCA base at the fixed offset
   __SCAbase in the SDA. This address is calculated by adding
   the address of CA with CONST.				
*/
#define IMT_SCABASE	(0x28 | IMT_1WORD)

/*
   Create an IMT table with the size CONST. Store the address
   to the IMT in the SDA at the fixed address __IMTbase.
*/
#define IMT_IMTSIZE	(0x30 | IMT_1WORD)

/*
   Fill the next IMT entry with the information about the
   external function STRING. This is done as follows:
   - Search all other modules for the name STRING.
   - If found:
    + Fill the first instruction in the IMT entry with a 
      "lwz  r13,__DATbase(r13)" to load the DAT pointer.
    + Fill the second instruction with a "lwz  r13,__DAToffs(r13)",
      where __DAToffs is the module number of the module times four.
    + Fill the third instruction with a "lwz  r2,__SCAbase(r13)"
      to get the SCA pointer.
    + Fill the fourth instruction with a "b  function".
   - If not found:
    + Fill the first instruction with a "b  __IMTfind".
      The __IMTfind will search for the expected function
      (the IMT entry address is still in r12, so the current CA can be scanned
      to get the name of the function), since it might be in a module loaded
      after the current module was loaded. If so, the IMT entry is patched,
      otherwise the system loader is called to either load a new module, or
      to give an error.
   There is one IMTNAME command for each external function.
*/
#define IMT_IMTNAME	(0x38 | IMT_ZTERM)

/*
   Fill the next IMT entry with the information about a local function at
   address CONST (relative to CA).
   + Fill the first instruction in the IMT entry with a 
     "lwz  r13,__DATbase(r13)" to load the DAT pointer.
   + Fill the second instruction with a "lwz  r13,__DAToffs(r13)",
     where __DAToffs is the current module number of the module times four.
   + Fill the third instruction with a "lwz  r2,__SCAbase(r13)"
     to get the SCA pointer.
   + Fill the fourth instruction slot with a "b  CA+CONST"
*/
#define IMT_IMTADDR	(0x40 | IMT_1WORD)

/*
   Exports the function NAME. This command is used when other modules are
   looking for a specific function in this module. There is one EXPORT 
   for each externally visible function. The function is located at address
   CA+CONST.
*/
#define IMT_EXPORT	(0x48 | IMT_ZWORD)

/*
   END of IMT info
*/
#define IMT_END		(0xf8 | IMT_0WORD)


/**************	Exported functions ******************************/


#endif /* D_imt */
