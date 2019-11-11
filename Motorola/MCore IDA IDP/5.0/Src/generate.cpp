#include "idaidp.hpp"

#include "allocator.hpp"
#include "types.hpp"
#include "ins.hpp"
#include "generate.hpp"
#include "analyze.hpp"
#include "reg.hpp"

using namespace mcore_n;

const mcore_n::string generate_c::_constPredefinedSegments[] = 
{
	mcore_n::string(".text"),  // Code generated for functions, global asm statements and string constants.
	mcore_n::string(".data"),  // Static and global non-const variables with initial values. 
		                   // The values may be copied from “rom” to “ram” at startup.
	mcore_n::string(".bss"),   // Static and global non-const variables without initial values. 
	                       // Will be initialized to 0 by startup code.
	mcore_n::string(".rodata") // const variables, and strings.
};

bool generate_c::outOperandRegister(const insn_t& ins, const op_t& x)
{
	analyze_c::opflags_s* flag = (analyze_c::opflags_s*)&x.opFlag;

	mcore_n::string       pref("");
	mcore_n::string       postf("");
	
	switch (flag->_phraseType) 
	{
	case analyze_c::opflags_s::ptExpression:
		{
			pref = "(";
			postf = ")";
		}
		break;
	}

	postf += flag->_isWriteBack ? "!" : "";

	mcore_n::string       reg("");

	if (flag->_isRegistersListSet)
	{
		reg = reglist_c(x.reg).getList();
	}
	else if (flag->_isControlRegister)
	{
		reg = regs_c::getRegCtlItem((regs_c::control_e)x.reg);
	}
	else
	{
		reg = regs_c::getRegGenItem((regs_c::general_e)x.reg);
	}

	out_line(pref.c_str(), COLOR_SYMBOL);

	out_register(reg.c_str());

	out_line(postf.c_str(), COLOR_SYMBOL);

	return true;
}
bool generate_c::outOperandImmediate(const insn_t& ins, const op_t& x)
{
	op_t cp = x;
	
	//out_line("#", COLOR_SYMBOL);

	analyze_c::opflags_s* flag = (analyze_c::opflags_s*)&x.opFlag;
	if (flag->_phraseType == analyze_c::opflags_s::ptXoredImmediate)
	{
		::out_line("~", COLOR_INSN);
	}

	::OutValue(cp, OOFS_IFSIGN | OOFW_IMM);

	return true;
}
bool generate_c::outOperandDisplacement(const insn_t& ins, const op_t& x)
{
	op_t                  cp = x;
	analyze_c::opflags_s* flag = (analyze_c::opflags_s*)&x.opFlag;

	mcore_n::string       reg("");
	mcore_n::string       pref("");
	mcore_n::string       postf("");

	switch (flag->_phraseType) 
	{
	case analyze_c::opflags_s::ptExpression:
		{
			pref = "(";
			postf = flag->_isWriteBack ? ")!" : ")";
		}
		break;
	}
	
	if (flag->_isRegistersListSet)
	{
		reg = reglist_c(x.reg).getList();
	}
	else if (flag->_isControlRegister)
	{
		reg = regs_c::getRegCtlItem((regs_c::control_e)x.reg);
	}
	else
	{
		reg = regs_c::getRegGenItem((regs_c::general_e)x.reg);
	}

	out_line(pref.c_str(), COLOR_SYMBOL);

	out_register(reg.c_str());

	// It is done for (rx, 0) can be converted into structure offsets as well
//	if (cp.addr != 0)
//	{
		out_line(", ", COLOR_SYMBOL);

		OutValue(cp, OOF_ADDR | OOF_SIGNED);
//	}

	out_line(postf.c_str(), COLOR_SYMBOL);

	return true;
}
bool generate_c::outOperandMemory(const insn_t& ins, const op_t& x)
{
	insn_t   i = ins;
	op_t     cp = x;

	ea_t     refEa = ::toEA(ins.cs, cp.addr);
	// Checking the type that has been set during emu action
	if (::hasRef(::getFlags(refEa)))
	{
		ea_t        target = ::toEA(ins.cs, ::get_long(refEa));
		char        ptr[0x200];
		// if the pointer exists, i.e. whether it is a valid referenced address
		// for an instance, in case of jsri xxxxx, address must exist
		if (::get_name_expr(refEa, cp.n, target, BADADDR, ptr, 0x200) != 0 &&
			::hasRef(::getFlags(target)))
		{
			OutLine(ptr);
			return true;
		}
		else if (::InstrIsSet(ins.itype, CF_CALL) ||
			::InstrIsSet(ins.itype, CF_JUMP))
		{
			// Otherwise mark it as invalid
			cp.value = ::get_long(refEa);
			::out_tagon(COLOR_ERROR);
			::OutLong(::get_long(refEa), ::getRadixEA(refEa, x.n));
			::out_tagoff(COLOR_ERROR);
			::QueueMark(Q_noName, ins.ea);
			return true;
		}
	}
	// If it is not an offset, output as value
	cp.value = ::get_long(refEa);
	::OutValue(cp, OOF_NUMBER | OOFW_IMM);
	return true;
}
bool generate_c::outOperandFar(const insn_t& ins, const op_t& x)
{
	insn_t   i = ins;
	op_t     cp = x;

	ea_t     refEa = ::toEA(ins.cs, cp.addr);
	// Checking the type that has been set during emu action
	if (::hasRef(::getFlags(refEa)))
	{
		ea_t        target = ::toEA(ins.cs, ::get_long(refEa));
		char        ptr[0x200];
		// if the pointer exists, i.e. whether it is a valid referenced address
		// for an instance, in case of jsri xxxxx, address must exist
		if (::get_name_expr(refEa, cp.n, target, BADADDR, ptr, 0x200) != 0 &&
			::hasRef(::getFlags(target)))
		{
			OutLine(ptr);
			return true;
		}
		// Otherwise mark it as invalid
		cp.value = ::get_long(refEa);
		::out_tagon(COLOR_ERROR);
		::OutLong(::get_long(refEa), ::getRadixEA(refEa, x.n));
		::out_tagoff(COLOR_ERROR);
		::QueueMark(Q_noName, ins.ea);
		return true;
	}
	// If it is not an offset, output as value
	cp.value = ::get_long(refEa);
	::OutValue(cp, OOF_NUMBER | OOFW_IMM);
	return true;
}
bool generate_c::outOperandNear(const insn_t& ins, const op_t& x)
{
	op_t                  cp = x;

	ea_t                  ea = toEA(ins.cs, cp.addr);

	char                  ptr[0x200];

	if (::get_name_expr(ins.ea + cp.offb, cp.n, ea, cp.addr, ptr, 0x200) == 0)
	{
		out_tagon(COLOR_ERROR);

		OutLong(x.addr, 16);

		out_tagoff(COLOR_ERROR);

		QueueMark(Q_noName, ins.ea);

		return false;
	}

	OutLine(ptr);

	return true;
}
bool generate_c::generateInstructionOperands(op_t &x)
{
	switch (x.type)
	{
	case o_reg:
		{
			return generate_c::outOperandRegister(cmd, x);
		}
		break;
	case o_imm:
		{
			return generate_c::outOperandImmediate(cmd, x);
		}
		break;
	case o_displ:
		{
			return generate_c::outOperandDisplacement(cmd, x);
		}
		break;
	case o_mem:
		{
			return generate_c::outOperandMemory(cmd, x);
		}
		break;
	case o_far:
		{
			return generate_c::outOperandFar(cmd, x);
		}
		break;
	case o_near:
		{
			return generate_c::outOperandNear(cmd, x);
		}
		break;
	default:
		{
			warning("out: %a: bad optype", x.type);
		}
		break;
	}
	return false;
}
void generate_c::generateInstructionPseudoCode()
{
	char buf[MAXSTR];
	ea_t ea = cmd.ea;

	init_output_buffer(buf, sizeof(buf));

	// Output the instruction name with the suffix
	OutMnem();

	// Output the operands
	if (::InstrIsSet(cmd.itype, CF_USE1))
	{
		out_one_operand(0);
	}
	if (::InstrIsSet(cmd.itype, CF_USE2))
	{
		out_symbol(',');
		out_symbol(' ');
		out_one_operand(1);
	}

	// terminate the output line
	term_output_buffer();

	gl_comm = 1;                  // generate a user defined comment on this line
	MakeLine(buf);                // be done with the instruction line
}
void generate_c::generateSegmentHeader(ea_t ea)
{
	segment_t*           seg = getseg(ea);
	if (is_spec_segm(seg->type))
	{
		return;
	}
	
	char buf[0x200];
	::get_true_segm_name(seg, buf, 0x200);
	mcore_n::string      sname(buf);
	::get_segm_class(seg, buf, 0x200);
	mcore_n::string      sclass(buf);
	bool                 sfound = false;
	
	for (size_t i = 0; i < qnumber(generate_c::_constPredefinedSegments); ++i)
	{
		sfound |= (sname == generate_c::_constPredefinedSegments[i]);
	}
	if (sfound)
	{
		printf_line(-1, COLSTR("%s", SCOLOR_ASMDIR), sname);
	}
	else
	{
		printf_line(-1, COLSTR(".section %s", SCOLOR_ASMDIR) "" COLSTR("%s %s", SCOLOR_AUTOCMT),
			sname.c_str(),
			ash.cmnt,
			sclass.c_str());
	}
}
void generate_c::generateSegmentFooter(ea_t ea)
{
	segment_t* seg = getseg(ea - 1);
	
	if (is_spec_segm(seg->type))
	{
		return;
	}
	char buf[0x200];
	get_segm_name(seg, buf, 0x200);
	gen_cmt_line("%-*s ends", inf.indent - strlen(ash.cmnt) - 2, buf);
}
void generate_c::generateSegmentAssume(ea_t ea)
{
	//if ( !inf.s_assume ) return;

	//segreg_t *d  = getSRarea(ea);
	//segment_t *s = getseg(ea);
	//if ( s != NULL
	//	&& s->type == SEG_CODE
	//	&& d != NULL
	//	&& (s->startEA == ea || d->startEA == ea) )
	//{
	//	if ( ash.uflag & UAS_GNU )
	//		printf_line(-1, COLSTR("%s", SCOLOR_ASMDIR), d->reg(T) ? ".code 16" : ".arm");
	//	else
	//		printf_line(-1, COLSTR("CODE%d", SCOLOR_ASMDIR), d->reg(T) ? 16 : 32);

	//}
}
void generate_c::generateDocumentHeader()
{
	gen_cmt_line("Processor       : %s", inf.procName);
	gen_cmt_line("Target assembler: %s", ash.name);

	if (ash.header != 0)
	{
		for (const char** ptr = ash.header; *ptr != 0; ++ptr)
		{
			printf_line(0, COLSTR("%s", SCOLOR_ASMDIR), *ptr);
		}
	}
}
void generate_c::generateDocumentFooter(void)
{
	char name[MAXSTR];
	get_colored_name(BADADDR, inf.beginEA, name, sizeof(name));
	printf_line(-1, COLSTR("%s", SCOLOR_ASMDIR) " %s", ash.end, name);
}