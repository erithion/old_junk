#include "idaidp.hpp"
#include "srarea.hpp"
#include "typeinf.hpp"

#include "allocator.hpp"
#include "types.hpp"

#include "main.hpp"

#include "ins.hpp"
#include "reg.hpp"
#include "emulate.hpp"
#include "analyze.hpp"
#include "generate.hpp"

using namespace mcore_n;

static idpentry_c initializeProcessor;

/************************************************************************/
/* idpentry_c                                                           */
/************************************************************************/
const char* idpentry_c::_constShortProcessorName[] =
{
	"M*Core",
	0
};
const char* idpentry_c::_constLongProcessorName[] =
{
	"Motorola",
	0
};
static asm_t v_gnu = 
{
	ASH_HEXF3 | 
	ASD_DECF0 | 
	ASO_OCTF1 | 
	ASB_BINF3 | 
	AS_N2CHR | 
	AS_LALIGN | 
	AS_1TEXT | 
	AS_ONEDUP | 
	AS_COLON,
	0,                                                       // uflag
	"Motorola M*Core Assembler",                             // name
	0,                                                       // help
	0,                                                       // header
	0,                                                       // bad instructions
	".org",                                                  // origin
	".end",                                                  // end

	"",                                                     // comment string
	'"',                                                     // string delimiter
	'\'',                                                    // char delimiter
	"\"'",                                                   // special symbols in char and string constants

	".ascii",                                                // ascii
	".byte",                                                 // byte
	".short",                                                // word
	".long",                                                 // dword
	".quad",                                                 // qword (no information about this directive)
	0,                                                       // oword  (16 bytes)
	".float",                                                // float
	".double",                                               // double
	0,                                                       // tbyte (no information about this directive)
	0,                                                       // packreal
	".ds.#s(b,w,l,d) #d, #v",                                // arrays (#h,#d,#v,#s(...)
	".ds.b %s",                                              // bss
	".equ",                                                  // equ
	0,                                                       // seg

	0,                                                       // int (*checkarg_preline)(char *argstr, s_preline *S);
	0,                                                       // char *(*checkarg_atomprefix)(char *operand,int *res);
	0,                                                       // char *checkarg_operations;

	0,                                                       // uchar *XlatAsciiOutput
	".",                                                     // char *a_curip;
	0,                                                       // function header
	0,                                                       // function footer
	".globl",     // public
	0,                                                       // weak
	".extrn",    // extrn
	0,                                                       // comdef
	0,                                                       // get name of type
	".align",                                                // align
	'(',                                                     // lbrace
	')',	                                                 // rbrace
	"%",                                                     // mod
	"&",                                                     // and
	"|",                                                     // or
	"^",                                                     // xor
	"~",                                                     // not
	">>",                                                    // shl
	"<<",                                                    // shr
	"sizeof",                                                // sizeof
};
netnode idpentry_c::_helper;
asm_t* idpentry_c::_assemblers[] = 
{ 
	&v_gnu, 
	0
};
processor_t idpentry_c::LPH;
int idpentry_c::calcArgLocs(const type_t*& type, ulong* argData, int argSize)
{
	// format:
	//  cm_t... additional flags;
	//  type_t... return type;
	//  [argloc_t of returned value (if CM_CC_SPECIAL)];
	//  if !CM_CC_VOIDARG:
	//    dt N (N=number of parameters)
	//    if ( N == 0 )
	//      if CM_CC_ELLIPSIS
	//        func(...)
	//      else
	//        parameters are unknown
	//    else
	//      N records:
	//        type_t... (i.e. type of each parameter)
	//        [argloc_t (if CM_CC_SPECIAL)] (i.e. place of each parameter)

	// Ellipsis is not taken into account in the number of parameters
	// ATT4: the return type can not be BT_ARRAY or BT_FUNC
	
	// Passing additional flags
	++type;
	// 
	cm_t cc = ::get_cc(*type);
	// Passing return type
	++type;

	if (cc == CM_CC_UNKNOWN) cc = ::get_cc(inf.cc.cm);
	if (cc == CM_CC_VOIDARG) return 0;

	::get_type_size(idati, type);
	if (cc == CM_CC_SPECIAL) ::skip_argloc(type);

	int n = get_dt(type);
	if (n < 0) return -1;
	if (n >= argSize) 
	{
		n = argSize - 1;
	}
	//
	const type_t* tp = type;
	for (int i = 0; i < n; ++i)
	{
		if (::get_type_size(idati, tp) == BADSIZE) return -1;
		if (cc == CM_CC_SPECIAL) ::skip_argloc(tp);
		// Arguments are passed via r2-r7
		if (i < 6)
		{
			argData[i] = ARGLOC_REG | (2 + i);
		}
		else
		{
			argData[i] = (i - 6) * 4;
		}
	}
	return n;
}
bool idaapi idpentry_c::setOperandType(op_t &x, const type_t* type, const char* name)
{
	insn_t ins = cmd;
	switch (x.type)
	{
	case o_reg:
		{
			func_t* pfn = ::get_func(cmd.ea);
			if (pfn == NULL)
			{
				return false;
			}
			func_item_iterator_t fii;
			ulong                regNum = x.reg;
			bool                 valid = fii.set(pfn, cmd.ea);
			while (valid)
			{
				switch (cmd.itype)
				{
				case ins_c::pcLdm:
				case ins_c::pcLdq:
					{
						assert("LDM | LDQ instruction was met while setting operand type info");
					}
					break;
				}
				if (::get_spoiled_reg(&regNum, 1) != -1)
				{
					::add_regarg(pfn, regNum, type, name);
					break;
				}
				valid = fii.decode_prev_insn();
			}
		}
		break;
	}
	return false;
}
int idpentry_c::useRegArgType(ea_t ea, const type_t* const* argTypes, const char* const* argNames, const ulong* regsData, int regsSize)
{
	int idSpoiledReg = -1;
	if (::ua_ana0(ea))
	{
		insn_t ins = cmd;
		idSpoiledReg = ::get_spoiled_reg(regsData, regsSize);
		// If we met spoiled register, it is our register parameter
		if (idSpoiledReg >= 0)
		{
			const type_t* type = argTypes[idSpoiledReg];
			const char*   name = argNames[idSpoiledReg];

			setOperandType(cmd.Op1, type, name);
		}
	}
	return idSpoiledReg;
}
bool idaapi idpentry_c::isStackArgLoad(int* src, int* dst)
{
	return false;
}
// Probability
static uchar isStackProlog(ea_t start)
{
	uint   funcProbability = 0;
	uint   flowIns = 0;
	ea_t   cur = start;
	while (flowIns < 4 &&
		funcProbability < 100)
	{
		if (::isEnabled(start))//::isCode(::getFlags(start)))
		{
			::ua_ana0(cur);

			if (cmd.itype == ins_c::pcSubi &&
				cmd.Op1.is_reg(regs_c::rgSp))
			{
				funcProbability += 50;
			}
			if (cmd.itype == ins_c::pcStm ||
				(cmd.itype == ins_c::pcSt &&
				cmd.Op2.is_reg(regs_c::rgSp)))
			{
				funcProbability += 50;
			}
		}
		cur = ::get_item_end(cur);
		if (emulate_c::isFlowStopInstruction(cmd))//!::isFlow(::getFlags(cur)))
		{
			break;
		}
		++flowIns;
	}
	return funcProbability;
}
int idpentry_c::kernelCallback(processor_t::idp_notify msgid, ...)
{
	va_list va;
	va_start(va, msgid);

	// A well behaving processor module should call invoke_callbacks()
	// in his notify() function. If this function returns 0, then
	// the processor module should process the notification itself
	// Otherwise the code should be returned to the caller:

	int     code = ::invoke_callbacks(HT_IDP, msgid, va);
	if (code)
	{
		return code;
	}

	switch(msgid)
	{
		// Initiate the newbies when they load new files
	case ph.newfile:
		{
			inf.mf = true;
		}
		break;
		// The first event from the kernel - the processor module is loaded
	case ph.init:
		{
			idpentry_c::_helper.create();
		}
		break;
		// Set DS equal to CS so the user will not worry about it
	case ph.newseg:
		{
			segment_t* seg = va_arg(va, segment_t*);
			seg->defsr[regs_c::rgVds - ph.regFirstSreg] = ::find_selector(seg->sel);
			seg->defsr[regs_c::rgVcs - ph.regFirstSreg] = 0;
		}
		break;
		// An old file has been loaded
	case ph.oldfile:
		{
			// compatibility issue: old databases did not contain the
			// segment registers (thumb mode was not there yet)
			// Here we create segment register areas
			for (int i = 0; i < ::get_segm_qty(); ++i)
			{
				segment_t* s = getnseg(i);
				if (::getSRarea(s->startEA) == 0)
				{
					segreg_t sr;
					sr.startEA = s->startEA;
					sr.endEA = s->endEA;
					sr.reg(regs_c::rgVds) = s->sel;
					sr.settags(SR_autostart);
					SRareas.create_area(&sr);
				}
			}
			// Load the saved module options
			//			idpflags = (ushort)helper.altval(-1);
		}
		break;
		// Is the instruction a "call"?
		// ea_t ea  - instruction address
		// returns: 1-unknown, 0-no, 2-yes
	case ph.is_call_insn:
		{
			ea_t ea = va_arg(va, ea_t);

			::ua_ana0(ea);

			if (cmd.itype == ins_c::pcJmpi)
			{
				return isStackProlog(::get_long(::toEA(cmd.cs, cmd.Op1.addr)));
			}
			return false;
		}
		break;
		// can a function start here?
		// arg: none, the instruction is in 'cmd'
		// returns: probability 0..100
		// 'cmd' structure is filled upon the entrace
		// the idp module is allowed to modify 'cmd'
	case ph.may_be_func:
		{
			if (::isCode(::getFlags(cmd.ea)))
			{
				return isStackProlog(cmd.ea);
			}
			return false;
		}
		break;
		// is the instruction sane for the current file type?
		// arg:  int no_crefs
		// 1: the instruction has no code refs to it.
		//    ida just tries to convert unexplored bytes
		//    to an instruction (but there is no other
		//    reason to convert them into an instruction)
		// 0: the instruction is created because
		//    of some coderef, user request or another
		//    weighty reason.
		// The instruction is in 'cmd'
		// returns: 1-ok, 0-no, the instruction isn't
		// likely to appear in the program
	case ph.is_sane_insn:
		{
			int noCodeRefs = va_arg(va, int);
			return !noCodeRefs;
		}
		break;
	case ph.is_jump_func:
		{
//						const func_t* pfn = va_arg(va, const func_t*);
//						ea_t* jump_target = va_arg(va, ea_t*);
//						return is_jump_func(pfn, jump_target);
			return false;
		}
		break;
		// The kernel has changed a segment register value
		// args:  ea_t startEA
		//        ea_t endEA
		//        int regnum
		//        sel_t value
		//        sel_t old_value
		//        uchar tag (SR_... values)
		// returns: 1-ok, 0-error
	case ph.setsgr:
		{
			ea_t ea1 = va_arg(va, ea_t);
			ea_t ea2 = va_arg(va, ea_t);
			int  reg = va_arg(va, int);
			::msg("Deleting instructions in range %08a..%08a\n", ea1, ea2);
			for (ea_t x = ea1; x < ea2; x = ::nextthat(x, ea2, ::isCode))
			{
				::do_unknown(x, false);
			}
		}
		break;
		// +++ TYPE CALLBACKS
		// Decorate/undecorate a C symbol name
		// const til_t *ti    - pointer to til
		// const char *name   - name of symbol
		// const type_t *type - type of symbol. If NULL then it will try to guess.
		// char *outbuf       - output buffer
		// size_t bufsize     - size of the output buffer
		// bool mangle        - true-mangle, false-unmangle
		// cm_t cc            - real calling convention for VOIDARG functions
		// returns: true if success
	case ph.decorate_name:
		{
			const til_t*  ti = va_arg(va, const til_t*);
			const char*   name = va_arg(va, const char*);
			const type_t* type = va_arg(va, const type_t*);
			char*         outbuf = va_arg(va, char*);
			size_t        bufsize = va_arg(va, size_t);
			bool          mangle = va_arg(va, bool);
			cm_t          real_cc = va_arg(va, cm_t);
			return ::gen_decorate_name(ti, name, type, outbuf, bufsize, mangle, real_cc);
		}
		break;
	case ph.guess_memory_model:
		{
			return CM_M_NN;
		}
		break;
	case ph.max_ptr_size:
		{
			return 4 + 1;
		}
		// get prefix and size of 'segment based' ptr
		// type (something like char _ss *ptr)
		// see description in typeinf.hpp
		// args:  unsigned ptrt
		//        const char **ptrname (output arg)
		// returns: size of type
	case ph.based_ptr:
		{
			return false;
		}
		break;
		// get default enum size
		// args:  cm_t cm
		// returns: sizeof(enum)
	case ph.get_default_enum_size: 
		{
			return inf.cc.size_e;
		}
		break;
		// Fill an array of actual arguments ins arglogs
		// args:    const type_t **type
		//          ulong *arglocs - the result array
		//          int maxn       - number of elements in arglocs
		// returns: int number_of_arguments
	case ph.calc_arglocs:
		{
			const type_t** type = va_arg(va, const type_t**);
			ulong*         arg = va_arg(va, ulong *);
			int            argCount = va_arg(va, int);
			return calcArgLocs(*type, arg, argCount);
		}
		break;
		// use information about callee arguments
		// args:ea_t ea              - address of the call instruction
		//      const type_t * const * - array of all argument types
		//      const char * const * - array of all argument names
		//      const ulong *        - array of argument locations
		//      int n                - number of all arguments
		//      const type_t **      - array of register argument types
		//      const char **        - array of register argument names
		//      ulong *              - array of register numbers
		//      int rn               - number of register arguments
		// returns: new value of rn
		// this callback will be used only if PR_USE_ARG_TYPES is set
	case ph.use_arg_types:
		{
			ea_t                     caller = va_arg(va, ea_t);
			const type_t* const*     argAllTypes = va_arg(va, const type_t* const*);
			const char* const*       argAllNames = va_arg(va, const char* const*);
			const ulong*             argAllLocs = va_arg(va, const ulong*);
			int                      argAllSize = va_arg(va, int);
			const type_t**           argRegTypes = va_arg(va, const type_t**);
			const char**             argRegNames = va_arg(va, const char**);
			ulong*                   argRegLocs = va_arg(va, ulong*);
			int                      argRegSize = va_arg(va, int);
			return ::gen_use_arg_types(caller, 
				argAllTypes, argAllNames, argAllLocs, argAllSize,
				argRegTypes, argRegNames, argRegLocs, argRegSize,
				setOperandType, isStackArgLoad,	0);
		}
		break;
		// use information about register argument
		// args:ea_t ea              - address of the instruction
		//      const type_t * const * - array of argument types
		//      const char * const * - array of argument names
		//      const ulong *        - array of register numbers
		//      int n                - number of register arguments
		// returns: idx of the used argument - if the argument is defined in the current instruction
		//                                     a comment will be applied by the kernel
		//          idx|REG_SPOIL            - argument is spoliled by the instruction
        //                                     stop tracing the register
		//          -1                       - if the instruction doesn't change any registers
		//          -2                       - if the instruction spoils all registers
	case ph.use_regarg_type:
		{
			ea_t                 ea = va_arg(va, ea_t);
			const type_t* const* argTypesArray = va_arg(va, const type_t* const*);
			const char* const*   argNamesArray = va_arg(va, const char* const*);
			const ulong*         regsArray = va_arg(va, const ulong*);
			int                  regsCount = va_arg(va, int);
			return useRegArgType(ea, argTypesArray, argNamesArray, regsArray, regsCount);
		}
		break;
	case ph.use_stkarg_type:
	case ph.get_fastcall_regs:
	case ph.calc_cdecl_purged_bytes:
		assert("Empty notify branches was handled");
		return 0;
	}

	va_end(va);

	return true;
}
const char* idpentry_c::setIDPOptions(const char* keyword, int type, const void* value)
{
	if (keyword == NULL)
	{
		static char form[] =
			"HELP\n"
			"\n"
			" Use macro instructions\n"
			"\n"
			"     When enabled, IDA uses macro instructions instead of\n"
            "     the standard M-Core instructions                    \n"
			"     All macro instructions are displayed with caps      \n"
			"\n"
			"     Transforming instructions list:                     \n"
			"\n"
			"         - bmaski r, v                                   \n"
			"              becomes                                    \n"
			"           MOVI r, (1 << v) - 1                          \n"
			"\n"
			"         - bmaski r, v1                                  \n"
			"           bseti r, v2                                   \n"
			"              becomes                                    \n"
			"           MOVI r, ((1 << v1) - 1) | (1 << v2)           \n"
			"\n"
			"         - bmaski r, v1                                  \n"
			"           bclri r, v2                                   \n"
			"              becomes                                    \n"
			"           MOVI r, ((1 << v1) - 1) & (~(1 << v2))        \n"
			"\n"
			"         - bmaski r, v1                                  \n"
			"           addi r, v2                                    \n"
			"              becomes                                    \n"
			"           MOVI r, ((1 << v1) - 1) + v2                  \n"
			"\n"
			"         - bmaski r, v1                                  \n"
			"           subi r, v2                                    \n"
			"              becomes                                    \n"
			"           MOVI r, ((1 << v1) - 1) - v2                  \n"
			"\n"
			"         - bmaski r, v1                                  \n"
			"           {lsli | rotli} r, v2                          \n"
			"              becomes                                    \n"
			"           MOVI r, ((1 << v1) - 1) << v2                 \n"
			"\n"
			"         - bmaski r, v1                                  \n"
			"           {lsri | asri} r, v2                           \n"
			"              becomes                                    \n"
			"           MOVI r, ((1 << v1) - 1) >> v2                 \n"
			"\n"
			"         - bgeni r, v                                    \n"
			"              becomes                                    \n"
			"           MOVI r, 1 << v                                \n"
			"\n"
			"         - bgeni r, v1                                   \n"
			"           bseti r, v2                                   \n"
			"              becomes                                    \n"
			"           MOVI r, (1 << v1) | (1 << v2)                 \n"
			"\n"
			"         - bgeni r, v1                                   \n"
			"           bclri r, v2                                   \n"
			"              becomes                                    \n"
			"           MOVI r, (1 << v1) & (~(1 << v2))              \n"
			"\n"
			"         - bgeni r, v1                                   \n"
			"           addi r, v2                                    \n"
			"              becomes                                    \n"
			"           MOVI r, (1 << v1) + v2                        \n"
			"\n"
			"         - bgeni r, v1                                   \n"
			"           subi r, v2                                    \n"
			"              becomes                                    \n"
			"           MOVI r, (1 << v1) - v2                        \n"
			"\n"
			"         - bgeni r, v1                                   \n"
			"           {lsli | rotli} r, v2                          \n"
			"              becomes                                    \n"
			"           MOVI r, (1 << v1) << v2                       \n"
			"\n"
			"         - bgeni r, v1                                   \n"
			"           {lsri | asri} r, v2                           \n"
			"              becomes                                    \n"
			"           MOVI r, (1 << v1) >> v2                       \n"
			"\n"
			"         - movi r, v1                                    \n"
			"           bseti r, v2                                   \n"
			"              becomes                                    \n"
			"           MOVI r, v1 | (1 << v2)                        \n"
			"\n"
			"         - movi r, v1                                    \n"
			"           bclri r, v2                                   \n"
			"              becomes                                    \n"
			"           MOVI r, v1 & (~(1 << v2))                     \n"
			"\n"
			"         - movi r, v1                                    \n"
			"           addi r, v2                                    \n"
			"              becomes                                    \n"
			"           MOVI r, v1 + v2                               \n"
			"\n"
			"         - movi r, v1                                    \n"
			"           subi r, v2                                    \n"
			"              becomes                                    \n"
			"           MOVI r, v1 - v2                               \n"
			"\n"
			"         - movi r, v1                                    \n"
			"           {lsli | rotli} r, v2                          \n"
			"              becomes                                    \n"
			"           MOVI r, v1 << v2                              \n"
			"\n"
			"         - movi r, v1                                    \n"
			"           {lsri | asri} r, v2                           \n"
			"              becomes                                    \n"
			"           MOVI r, v1 >> v2                              \n"
			"\n"
			"         - bseti r, v                                    \n"
			"              becomes                                    \n"
			"           ORI r, 1 << v                                 \n"
			"\n"
			"         - bseti r, v1                                   \n"
			"           bseti r, v2                                   \n"
			"              becomes                                    \n"
			"           ORI r, (1 << v1) | (1 << v2)                  \n"
			"\n"
			"         - bclri r, v                                    \n"
			"              becomes                                    \n"
			"           ANDI r, ~(1 << v)                             \n"
			"\n"
			"         - bclri r, v1                                   \n"
			"           bclri r, v2                                   \n"
			"              becomes                                    \n"
			"           ANDI r, ~((1 << v1) | (1 << v2))              \n"
			"\n"
			"ENDHELP\n"
			"M-Core specific options\n"
			"\n"
			" <#If enabled, forces to use convenient macro commands instead of the bit operations"
			" #Analyzer options##"
			"Use ~m~acro instructions:C>>\n"
			"\n";
		uint macro = analyze_c::useMacro();
		
		::AskUsingForm_c(form, &macro);

		analyze_c::useMacro(macro != 0);
		
		return IDPOPT_OK;
	}
	else
	{
		if (type != IDPOPT_BIT)
		{
			return IDPOPT_BADTYPE;
		}
		if (strcmp(keyword, "MCORE_MACRO") == 0)
		{
			analyze_c::useMacro(*(int*)value != 0);
//			::setflag(idpflags, MCORE_MACRO,*(int*)value);
			return IDPOPT_OK;
		}
		return IDPOPT_BADKEY;
	}
}
idpentry_c::idpentry_c()
{
	// Interface version
	idpentry_c::LPH.version = IDP_INTERFACE_VERSION;

	// Platform id
	idpentry_c::LPH.id = idpentry_c::_constPlatformId;

	idpentry_c::LPH.flag = PR_USE32 |       // 32-bit processor
		                   PR_DEFSEG32 |    // Create 32-bit segments by default
		                   PRN_HEX |        // Values are hexadecimal by default
		                   PR_SEGS |        // Has segment registers
		                   PR_SGROTHER |    // The segment register do not contain the segment information
		                   PR_TYPEINFO |//;     // Support the type system notifications
						   PR_USE_ARG_TYPES;

	// 8 bits in a byte for code segments
	idpentry_c::LPH.cnbits = 8;

	// 8 bits in a byte for other segments
	idpentry_c::LPH.dnbits = 8;

	// Short processor names (NULL terminated). 
	// Each name should be shorter than 9 characters
	idpentry_c::LPH.psnames = (char**)idpentry_c::_constShortProcessorName;

	// Long processor names (NULL terminated)
	// No restriction on name lengthes.
	idpentry_c::LPH.plnames = (char**)idpentry_c::_constLongProcessorName;

	// Pointer to array of target
	// assembler definitions. You may
	// change this array when current
	// processor is changed.
	// (NULL terminated)
	idpentry_c::LPH.assemblers = idpentry_c::_assemblers;

	// Various notifications for the idp
	idpentry_c::LPH.notify = idpentry_c::kernelCallback;

	// Function to produce start of disassembled text
	idpentry_c::LPH.header = generate_c::generateDocumentHeader;

	// Function to produce end of disassembled text
	idpentry_c::LPH.footer = generate_c::generateDocumentFooter;

	// Function to produce start of segment
	idpentry_c::LPH.segstart = generate_c::generateSegmentHeader;

	// Function to produce end of segment
	idpentry_c::LPH.segend = generate_c::generateSegmentFooter;

	// Function to produce assume directives
	// when segment register value changes
	// If your processor has no segment
	// registers, you may define it as NULL
	idpentry_c::LPH.assumes = generate_c::generateSegmentAssume;

	// Analyze one instruction and fill 'cmd' structure.
	// cmd.ea contains address of instruction to analyze.
	// Return length of the instruction in bytes, 0 if instruction can't be decoded.
	// This function shouldn't change the database, flags or anything else.
	// All these actions should be performed only by u_emu() function.
	idpentry_c::LPH.u_ana = analyze_c::handlerMain;

	// Emulate instruction, create cross-references, plan to analyze
	// subsequent instructions, modify flags etc. Upon entrance to this function
	// all information about the instruction is in 'cmd' structure.
	// Return length of the instruction in bytes.
	idpentry_c::LPH.u_emu = emulate_c::main;

	// Generate text representation of an instruction in 'cmd' structure.
	// This function shouldn't change the database, flags or anything else.
	// All these actions should be performed only by u_emu() function.
	idpentry_c::LPH.u_out = generate_c::generateInstructionPseudoCode;

	// Generate text representation of an instruction operand.
	// This function shouldn't change the database, flags or anything else.
	// All these actions should be performed only by u_emu() function.
	// The output text is placed in the buffer pointer by 'u_line' pointer.
	// Returns: 1-ok, 0-operand is hidden.
	idpentry_c::LPH.u_outop = generate_c::generateInstructionOperands;

	// Generate text representation of data items
	// This function MAY change the database and create cross-references, etc.
	idpentry_c::LPH.d_out = intel_data;

	// Compare instruction operands.
	// Returns 1-equal,0-not equal operands.
	// This pointer may be NULL.
	idpentry_c::LPH.cmp_opnd = 0;

	// Can the operand have a type as offset, segment, decimal, etc.
	// (for example, a register AX can't have a type, the user can't change its
	// representation. see bytes.hpp for information about types and flags)
	// This pointer may be NULL.
	idpentry_c::LPH.can_have_type = analyze_c::allowOperandType;

	// Number of registers
	idpentry_c::LPH.regsNum = regs_c::getRegGenSize();

	// Register names
	idpentry_c::LPH.regNames = regs_c::getRegGen();

	// Get register value.
	// may be NULL. If specified, will be
	// used in the determining predefined
	// comment based on register value.
	// Should be NULL
	idpentry_c::LPH.getreg = 0;

	// Number of register files
	idpentry_c::LPH.rFiles = 0;

	// Register names for files
	idpentry_c::LPH.rFnames = 0;

	// Description of registers
	idpentry_c::LPH.rFdescs = 0;

	// Pointer to CPU registers
	idpentry_c::LPH.CPUregs = 0;

	// The number of the first segment register
	idpentry_c::LPH.regFirstSreg = regs_c::rgVcs;

	// The number of the last segment register
	idpentry_c::LPH.regLastSreg = regs_c::rgVds;

	// Size of a segment register in bytes
	idpentry_c::LPH.segreg_size = 1;

	// Segment register information (use virtual CS and DS registers if your
	// processor doesn't have segment registers):

	// The number of cs segment register
	idpentry_c::LPH.regCodeSreg = regs_c::rgVcs;

	// The number of ds segment register
	idpentry_c::LPH.regDataSreg = regs_c::rgVds;

	// Array of typical code start sequences
	// This array is used when a new file
	// is loaded to find code starts.
	// This array is terminated with
	// a zero length item.
	idpentry_c::LPH.codestart = 0;

	// Array of 'return' instruction opcodes
	// This array is used to determine
	// form of autogenerated locret_...
	// labels.
	// This array should be defined
	// The last item of it should be { 0, NULL }
	idpentry_c::LPH.retcodes = 0;

	// iCode of the first instruction
	idpentry_c::LPH.instruc_start = ins_c::pcStart;

	// iCode of the Last + 1 instruction
	idpentry_c::LPH.instruc_end = ins_c::pcFake;

	// The instructions array
	idpentry_c::LPH.instruc = ins_c::getIns();

	// Is indirect far jump or call instruction?
	idpentry_c::LPH.is_far_jump = 0;

	// Translation function for offsets
	// Currently used in the offset display functions
	// to calculate the referenced address
	idpentry_c::LPH.translate = 0;

	// Size of long double (tbyte) for this processor
	// (meaningful only if ash.a_tbyte != NULL)
	idpentry_c::LPH.tbyte_size = 0;

	// Floating point -> IEEE
	// Error codes returned by this function (load/store):
	// REAL_ERROR_FORMAT  -1  not supported format for current .idp
	// REAL_ERROR_RANGE   -2 // number too big (small) for store (mem NOT modifyed)
	// REAL_ERROR_BADDATA -3 // illegal real data for load (IEEE data not filled)
	idpentry_c::LPH.realcvt = 0;//notify_c::realcvt;

	// Number of digits in floating numbers after the decimal point.
	// If an element of this array equals 0, then the corresponding
	// floating point data is not used for the processor.
	// This array is used to align numbers in the output.
	//      real_width[0] - number of digits for short floats (only PDP-11 has it)
	//      real_width[1] - number of digits for "float"
	//      real_width[2] - number of digits for "double"
	//      real_width[3] - number of digits for "long double"
	// Example: IBM PC module has { 0,7,15,19 }
	idpentry_c::LPH.real_width[0] = 0;
	idpentry_c::LPH.real_width[1] = 4;
	idpentry_c::LPH.real_width[2] = 8;
	idpentry_c::LPH.real_width[3] = 0;

	//  Find 'switch' idiom
	//      fills 'si' structure with information and returns 1
	//      returns 0 if switch is not found.
	//      input: 'cmd' structure is correct.
	//      this function may use and modify 'cmd' structure
	//      it will be called for instructions marked with CF_JUMP
	idpentry_c::LPH.is_switch = emulate_c::isSwitch;

	//  Generate map file. If this pointer is NULL, the kernel itself
	//  will create the map file.
	//  This function returns number of lines in output file.
	//  0 - empty file, -1 - write error
	idpentry_c::LPH.gen_map_file = 0;

	//  Extract address from a string. Returns BADADDR if can't extract.
	//  Returns BADADDR-1 if kernel should use standard algorithm.
	idpentry_c::LPH.extract_address = 0;

	//  Check whether the operand is relative to stack pointer
	//  This function is used to determine how to output a stack variable
	//  (if it returns 0, then the operand is relative to frame pointer)
	//  This function may be absent. If it is absent, then all operands
	//  are sp based by default.
	//  Define this function only if some stack references use frame pointer
	//  instead of stack pointer.
	//  returns: 1 - yes, 0 - no
	idpentry_c::LPH.is_sp_based = 0;

	//  Create a function frame for a newly created function.
	//  Set up frame size, its attributes etc.
	//  This function may be absent.
	idpentry_c::LPH.create_func_frame = emulate_c::createFunctionFrame;

	// Get size of function return address in bytes
	//      pfn - pointer to function structure, can't be NULL
	// If this function is absent, the kernel will assume
	//      4 bytes for 32-bit function
	//      2 bytes otherwise
	idpentry_c::LPH.get_frame_retsize = emulate_c::getFrameReturnSize;

	//  Generate stack variable definition line
	//  If this function is NULL, then the kernel will create this line itself.
	//  Default line is
	//              varname = type ptr value
	//  where 'type' is one of byte,word,dword,qword,tbyte
	idpentry_c::LPH.gen_stkvar_def = 0;

	// Generate text representation of an item in a special segment
	// i.e. absolute symbols, externs, communal definitions etc.
	// returns: 1-overflow, 0-ok
	idpentry_c::LPH.u_outspec = gen_spcdef;//generate_c::generateSegmentSpecialItem;

	// iCode of return instruction. It is ok to give any of possible return instructions
	idpentry_c::LPH.icode_return = 0;

	// Set IDP-specific option
	//      keyword - keyword encoutered in IDA.CFG file
	//                if NULL, then a dialog form should be displayed
	//      value_type - type of value of the keyword
	// IDPOPT_STR 1     string constant (char *)
	// IDPOPT_NUM 2     number (uval_t *)
	// IDPOPT_BIT 3     bit, yes/no (int *)
	// IDPOPT_FLT 4     float (double *)
	//      value   - pointer to value
	// returns:
	// IDPOPT_OK       NULL             ok
	// IDPOPT_BADKEY   ((char*)1)       illegal keyword
	// IDPOPT_BADTYPE  ((char*)2)       illegal type of value
	// IDPOPT_BADVALUE ((char*)3)       illegal value (bad range, for example)
	//      otherwise return pointer to an error message
	idpentry_c::LPH.set_idp_options = idpentry_c::setIDPOptions;

	//      Is the instruction created only for alignment purposes?
	//      returns: number of bytes in the instruction
	idpentry_c::LPH.is_align_insn = 0;

	//      Micro virtual machine description
	//      If NULL, IDP doesn't support microcodes.
	idpentry_c::LPH.mvm = 0;

	//      If the FIXUP_VHIGH and FIXUP_VLOW fixup types are supported
	//      then the number of bits in the HIGH part. For example,
	//      SPARC will have here 22 because it has HIGH22 and LOW10 relocations.
	//      See also: the description of PR_FULL_HIFXP bit
	idpentry_c::LPH.high_fixup_bits = 0;
}