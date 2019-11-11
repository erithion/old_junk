#define USE_STANDARD_FILE_FUNCTIONS

#include "idaidp.hpp"
#include "frame.hpp"
#include "struct.hpp"

#include "types.hpp"
#include "reg.hpp"
#include "ins.hpp"
#include "emulate.hpp"

using namespace mcore_n;

/*
ToDo:
f) implement IDP options, i.e. endianess
g) check the segments
e) implement macro commands
*/

uint emulate_c::_trackedRegisters[regs_c::rgFake] = {0};
/************************************************************************/
/*                                                                      */
/************************************************************************/
void emulate_c::handleOperandMem(const insn_t& ins, const op_t& x)
{
	ea_t    mem = ::toEA(ins.cs, x.addr);
	ea_t    target = ::get_long(mem);
	// Determining whether the operand is being update by the command
	dref_t  type = (x.n == 0 && ::InstrIsSet(ins.itype, CF_CHG1)) ? dr_W : dr_R;
	// Adding the data reference
	::ua_dodata(mem, x.dtyp);
	::ua_add_dref(x.offb, mem, type);
	::op_offset(mem, x.n, REF_OFF32, target, ins.cs);
}
void emulate_c::handleOperandFar(const insn_t& ins, const op_t& x)
{
	emulate_c::handleOperandMem(ins, x);
	// JMPI
	ea_t    mem = ::toEA(ins.cs, x.addr);
	ea_t    target = ::get_long(mem);
	if (ins.itype == ins_c::pcJmpi) // ::InstrIsSet(ins.itype, CF_JUMP))
	{
		// Telling the kernel to put the target
		// into the code auto-analysis queue

		// The fact is that JMPI instruction can be used both
		// in a function call and in a memory jump within a single function

		// And we must determine it in order to get more accurate function borders and 
		// therefore more accurate switch recognition
		// As well as we should add a fl_C.../fl_J code reference in
		// order to get a typeinfo for a functions with memory-referenced calls

//		if (ins.ea == 0x1002215A)
//		{
//			atype_t state = auto_state;
//			::msg("Ins Addr %X, Ref Addr %X : state %d\n", ins.ea, target, state);
//		}

//		if (may_trace_sp())//auto_state == AU_USED)
//		{
			// However, if we have reached the function reanalysis stage,
			// the instruction(s) at the referenced address is(are) already present
			// So, we can check the address in order to tell if it is a part of the
			// current function or it is a new function
			// is_call_insn actually yields the control to the notify::is_call_insn.
			// And if the instructions at the referenced address make a stack frame,
			// in that case definitely the target address is a new function
			if (::is_call_insn(ins.ea))
			{
				::add_cref(ins.ea, target, fl_CN);
				::auto_apply_type(ins.ea, target);
			}
			// Otherwise we make it a part of the current function
			else
			{
				::add_cref(ins.ea, target, fl_JN);
			}
//		}
		// Thus, during the code analysis we're just planning the conversion of the referenced address into the code
		// as far as we are not able to check the target address before the analysis falls into that address
//		else
//		{
//			::auto_make_code(target);
//		}
	}
	// JSRI
	else if (::InstrIsSet(ins.itype, CF_CALL))
	{
		::add_cref(ins.ea, target, fl_CN);
		::auto_apply_type(ins.ea, target);
	}
}
void emulate_c::handleOperand(const insn_t& ins, const op_t& x)
{
	switch (x.type) 
	{
	case o_imm:
		// Processing the immediate operand value
		{
			flags_t flags = ::getFlags(ins.ea);
			
			if (!::isDefArg(flags, x.n))
			{
				// Tell the kernel that the instruction has an immediate
				::doImmd(ins.ea);
				::op_num(ins.ea, x.n);
			}
		}
		break;
	case o_near:
		// Processing the near reference operand value
		{
			ea_t target = ::toEA(ins.cs, x.addr);
			if (::isEnabled(target))
			{
				bool isCall = ::InstrIsSet(ins.itype, CF_CALL);

				::ua_add_cref(x.offb, target, isCall ? fl_CN : fl_JN);

				return;
			}
			::msg("EMULATION: Bad Operand Address\nSee emulate_c::handleOperand for address %a", ins.ea);
		}
		break;
	case o_mem:
		// Processing the memory reference operand value
		{
			emulate_c::handleOperandMem(ins, x);
		}
	case o_far:
		// Processing the memory reference operand value
		{
			emulate_c::handleOperandFar(ins, x);
		}
		break;
	}
}
/************************************************************************/
/* Returns previously tracked register value                            */
/************************************************************************/
uint emulate_c::getTrackedRegister(regs_c::general_e regNum)
{
	return _trackedRegisters[regNum];
}
/************************************************************************/
/* Register savings by simple operations into tracked registers array   */
/* (Used only in SP tracing, not recommended in any other case)         */
/************************************************************************/
void emulate_c::trackRegisters(const insn_t& ins)
{
	if (::InstrIsSet(ins.itype, CF_USE1) &&
		::InstrIsSet(ins.itype, CF_CHG1) &&
		ins.Op1.type == o_reg)
	{
		switch (ins.itype)
		{
			// Macro command ANDI
		case ins_c::pcMacroAnd:
			{
				_trackedRegisters[ins.Op1.reg] &= ins.Op2.value;
			}
			break;
			// Macro command ORI
		case ins_c::pcMacroOr:
			{
				_trackedRegisters[ins.Op1.reg] |= ins.Op2.value;
			}
			break;
			// Macro command MOVI
		case ins_c::pcMacroMov:
			{
				_trackedRegisters[ins.Op1.reg] = ins.Op2.value;
			}
			break;
		case ins_c::pcMovi:
			{
				_trackedRegisters[ins.Op1.reg] = ins.Op2.value;
			}
			break;
		case ins_c::pcBseti:
			{
				size_t shifted = (1 << ins.Op2.value);
				_trackedRegisters[ins.Op1.reg] |= shifted;
			}
			break;
		case ins_c::pcRotli:
		case ins_c::pcLsli:
			{
				_trackedRegisters[ins.Op1.reg] <<= ins.Op2.value;
			}
			break;
		case ins_c::pcBgeni:
		case ins_c::pcBgeniImm5:
		case ins_c::pcBgeniImm3:
			{
				_trackedRegisters[ins.Op1.reg] = (1 << ins.Op2.value);
			}
			break;
		case ins_c::pcBmaskiImm5:
		case ins_c::pcBmaskiImm3:
		case ins_c::pcBmaski32Set:
			{			
				_trackedRegisters[ins.Op1.reg] = (1 << ins.Op2.value) - 1;
			}
			break;
		case ins_c::pcSubi:
			{
				_trackedRegisters[ins.Op1.reg] -= ins.Op2.value;
			}
			break;
		case ins_c::pcAddi:
			{
				_trackedRegisters[ins.Op1.reg] += ins.Op2.value;
			}
			break;
		case ins_c::pcLrw:
			{
				_trackedRegisters[ins.Op1.reg] = ::get_long(::toEA(ins.cs, ins.Op2.addr));
			}
			break;
		case ins_c::pcBclri:
			{
				_trackedRegisters[ins.Op1.reg] &= ~(1 << ins.Op2.value);
			}
			break;
		case ins_c::pcNot:
			{
				_trackedRegisters[ins.Op1.reg] = ~_trackedRegisters[ins.Op1.reg];
			}
			break;
		}
	}
}
/************************************************************************/
/* Makes full SP tracing                                                */
/* Determines stack points changing, stack frame size changing,         */
/* stack variables creation                                             */
/************************************************************************/
void emulate_c::traceSP(const insn_t& ins)
{
	func_t* fn = ::get_func(ins.ea);
	// Preventing the function working till the 
	// moment the stack frame has been created, 
	// i.e. createFrame callback was called and 
	// it had determined the stack frame presence
	if (fn != 0)
	{
		// We need to track registers in case of some complex stack allocation
		// with participation of the register values.
		// As far as compilers typically don't implement too sophisticated algorithms
		// for stack alloc/dealloc, it matters to track the latest register values only,
		// just before theirs direct usage in stack alloc/dealloc.
		// However, the most complex compiler allocations still must be handled.
		// For an instance, such as 
		//     movi    r1, 0x39 ; lsli    r1, 3; subu    sp, r1    (! Allocation via left shifted register value)
		// or
		//     lrw     r1, 0x668; subu    sp, r1 (! Allocation via loaded register value)
		// etc.
		// In any case, all those instructions with allocation via register value have one thing in common:
		// First there always goes some immediate value into the register, then optionally there is
		// some additional operation with the register and only after there is allocation
		// Thus, we always are able to get the required register value
		emulate_c::trackRegisters(ins);
		// For a stack alloc/dealloc an instruction must
		// increase/decrease the stack pointer value
		// So it must at least contain one operand, 
		// must modify one somehow and this operand 
		// must be a stack pointer register value
		if (::InstrIsSet(ins.itype, CF_CHG1) &&
			ins.Op1.is_reg(regs_c::rgSp)) // emulate_c::isRegisterOperand(ins.Op1, regs_c::rgSp))
		{
			// Check all instructions which are able to increase/decrease its operands
			switch (ins.itype)
			{
				// Haven't seen these instructions to be applied to the SP ever
			case ins_c::pcSubc:
			case ins_c::pcRsubi:
			case ins_c::pcRsub:
			case ins_c::pcAddc:
				break;
			case ins_c::pcSubu:
				{
					uint value = emulate_c::getTrackedRegister(regs_c::general_e(ins.Op2.reg));
					if (value != 0)
					{
						// Setting a new stack point for allocated stack
						::add_auto_stkpnt(ins.ea + ins.size, -(int)value);
						// Current frame size
						size_t frameSize = ::get_frame_size(fn);
						// Required frame size for the current stack point
						size_t val = abs(::get_spd(fn, ins.ea + ins.size));
						// In case the new stack point greater than current stack frame size
						// we must set a valid stack frame size
						if (val > frameSize)
						{
							::set_frame_size(fn, val, 0, 0);
						}
					}
				}
				break;
				// We must meet an ordinal immediate value only
				// accordingly to the M-Core assembly lang. manual
			case ins_c::pcSubi:
				{
					// Setting a new stack point for allocated stack
					::add_auto_stkpnt(ins.ea + ins.size, -(int)ins.Op2.value);
					// Current frame size
					size_t frameSize = ::get_frame_size(fn);
					// Required frame size for the current stack point
					size_t val = abs(::get_spd(fn, ins.ea + ins.size));
					// In case the new stack point greater than current stack frame size
					// we must set a valid stack frame size
					if (val > frameSize)
					{
						::set_frame_size(fn, val, 0, 0);
					}
				}
				break;
			case ins_c::pcAddu:
				{
					uint value = emulate_c::getTrackedRegister(regs_c::general_e(ins.Op2.reg));
					if (value != 0)
					{
						// Setting a new stack point for allocated stack
						::add_auto_stkpnt(ins.ea + ins.size, value);
					}
				}
				break;
				// We must meet an ordinal immediate value only
				// accordingly to the M-Core assembly lang. manual
			case ins_c::pcAddi:
				{
					// Setting a new stack point for deallocated stack
					::add_auto_stkpnt(ins.ea + ins.size, ins.Op2.value);
				}
				break;
			}
		}
		// We don't care about checking an operand size: 4,2,1 bytes
		// while creating stack variables,
		// as far as ua_stkvar cares about it by analyzing op_t::dtyp
		else if (::InstrIsSet(ins.itype, CF_USE1) && 
			emulate_c::isSPBasedOperand(ins.Op1) &&
			::may_create_stkvars() &&
			::ua_stkvar(ins.Op1, ins.Op1.addr))
		{
			::op_stkvar(ins.ea, ins.Op1.n);
		}
		// We don't care about checking an operand size: 4,2,1 bytes
		// while creating stack variables,
		// as far as ua_stkvar cares about it by analyzing op_t::dtyp
		else if (::InstrIsSet(ins.itype, CF_USE2) && 
			emulate_c::isSPBasedOperand(ins.Op2) &&
			::may_create_stkvars() &&
			::ua_stkvar(ins.Op2, ins.Op2.addr))
		{
			::op_stkvar(ins.ea, ins.Op2.n);
		}
	}
}
/************************************************************************/
/* Is called each time when there is an attempt to create a function    */
/* Must check whether any stack frame allocation exists within          */
/* a specified code area.                                               */
/* It relies on a fact that at least a link register must be saved in   */
/* stack. Thus there must be something like                             */
/* ...                                                                  */
/* subi sp, 0x...     ; // first stack has to be allocated for registers*/
/* ...                                                                  */
/* stm {r...-r15}, (sp) ; // stm stores only into stack and             */
/*                      ; // doesn't contain any sp modification affects*/
/* So, we'll check any references to r15 and will try to determine      */
/* whether it was saved into stack at least once                        */
/* However, as far as we're not interested in a precise stack frame size*/
/* as well as we do not need to create any stack points, we will only   */
/* check the stack frame presence                                       */
/* The rest work will be done by SP tracing                             */
/************************************************************************/
// Accordingly to mcoreabi.pdf                                          */
// The function prologue uses only the following classes of instructions.
// — subi r0,imm (Note that this might appear multiple times in a prologue)
// — stm rn-r15,(r0)
// — st.w rx,(r0,disp)
// — instructions that set and modify r1.
// — These are presumed to establish values for a relatively large frame. This
//   sequence includes one of the following instructions:
//     • lr.w r1, imm
//     • movi r1, imm
//     • bgeni r1, imm
//     • bmaski r1, imm
//   followed by zero or more of:
//     • addi r1, imm
//     • subi r1, imm
//     • rsubi r1, imm
//     • not r1
//     • rotli r1, imm
//     • bseti r1, imm
//     • bclri r1, imm
//     • ixh r1,r1
//     • ixw r1,r1
//   followed by:
//     • sub r0, r1
bool emulate_c::createFunctionFrame(func_t* fn)
{
	ea_t end = fn->endEA;
	ea_t start = fn->startEA;
	uint stackSize = 0;
	
	// First we must find SUBI SP, ... 
	// and after
	//   - STM ...-R15, (SP)
	//   - ST.W ..., (SP)
	while (start != BADADDR &&
		start < end)
	{
		if (::isCode(::getFlags(start)))
		{
			::ua_ana0(start);
			insn_t ins = cmd;

			switch (ins.itype)
			{
			case ins_c::pcStm:
				{
					// For STM instruction r15 always exists
					// But it is possible to recognize all 
					// other register saving as well
					reglist_c::listtype_e type;
					uchar                 code;
					reglist_c::decodeList(ins.Op1.reg, type, code);
					uchar                 reglistSize = 15 - code;

					if (reglistSize <= stackSize)
					{
						::update_func(fn);

						if (::add_frame(fn, stackSize, 0, 0))
						{
							struc_t* frame = ::get_frame(fn);
							uint     regPos = 0;
							while (code <= 15)
							{
								char  name[10] = {0};
								::qsnprintf(name, sizeof(name), "_r%d", code);

								::add_struc_member(frame, name, regPos, ::dwrdflag(), NULL, 4);
								++code;
								regPos += 4;
							}
							return true;
						}
					}
				}
				break;
			case ins_c::pcSt:
				{
					// r15 is saved with ST instruction
					if (ins.Op1.is_reg(regs_c::rgLr) &&//emulate_c::isRegisterOperand(ins.Op1, regs_c::rgLr) &&
						emulate_c::isSPBasedOperand(ins.Op2) &&
						ins.Op2.addr <= stackSize)
					{
						::update_func(fn);

						if (::add_frame(fn, stackSize, 0, 0))
						{
							char     name[10] = {0};
							struc_t* frame = ::get_frame(fn);

							::qsnprintf(name, sizeof(name), "_r%d", ins.Op1.reg);
							::add_struc_member(frame, name, ins.Op2.addr, ::dwrdflag(), NULL, 4);

							return true;
						}
					}
				}
				break;
			case ins_c::pcSubi:
				{
					// In order to signal that any further stack usage 
					// is quite valid, as far as there's a stack allocation
					if (ins.Op1.is_reg(regs_c::rgSp))//emulate_c::isRegisterOperand(ins.Op1, regs_c::rgSp))
					{
						stackSize = ins.Op2.value;
					}
				}
				break;
			}
		}
		start = ::get_item_end(start);
	}
	return false;
}
/************************************************************************/
/* Checks whether the instruction doesn't yield execution further       */
/************************************************************************/
bool emulate_c::isFlowStopInstruction(const insn_t& ins)
{
	bool   yieldFlow = ::InstrIsSet(ins.itype, CF_JUMP);
	if (yieldFlow)
	{
		yieldFlow = ins.itype != ins_c::pcBf &&
			ins.itype != ins_c::pcBt &&
			ins.itype != ins_c::pcLoopt;
	}

	return ::InstrIsSet(ins.itype, CF_STOP) || 
		ins.itype == ins_c::pcBkpt ||
		yieldFlow;
}

bool emulate_c::peekPrevInstruction(const insn_t& ins, insn_t& prev)
{
	insn_t bckp = cmd;

	ea_t ea = ::decode_prev_insn(ins.ea);

	prev = cmd;

	cmd = bckp;

	return ea != BADADDR;
}
/************************************************************************/
/* Checks for a presence of SP relative expression                      */
/************************************************************************/
bool emulate_c::isSPBasedOperand(const op_t& op)
{
	return (op.type == o_displ || op.type == o_phrase) &&
		   op.reg == regs_c::rgSp;
}
/************************************************************************/
/* Checks for a presence a specified register in specified operand      */
/************************************************************************/
//bool emulate_c::isRegisterOperand(const op_t& op, regs_c::general_e regNum)
//{
//	return op.type == o_reg &&
//		op.reg == regNum;
//}
// M-Core doesn't use stack for return addresses
int emulate_c::getFrameReturnSize(func_t* fn)
{
	return 0;
}
bool emulate_c::checkSwitchIdiom(const insn_t& ins, switch_info_t& si)
{
	// Checks the presence of a group of instructions
	// lrw rx, [JumpTable]
	// ixw rx, ry
	// ld  rx, (rx)
	// jmp rx

	insn_t insLd;
	insn_t insIxw;
	insn_t insLrw;
	insn_t insJb;

	bool isPrev = emulate_c::peekPrevInstruction(ins, insLd) &&
		          emulate_c::peekPrevInstruction(insLd, insIxw) &&
				  emulate_c::peekPrevInstruction(insIxw, insLrw) &&
				  emulate_c::peekPrevInstruction(insLrw, insJb);
	if (isPrev &&

		insLd.itype == ins_c::pcLd &&
		insIxw.itype == ins_c::pcIxw &&
		insLrw.itype == ins_c::pcLrw &&
		::InstrIsSet(insJb.itype, CF_JUMP) &&
		
		ins.Op1.reg == insLd.Op1.reg &&
		insLd.Op2.reg == insIxw.Op1.reg &&
		insIxw.Op1.reg == insLrw.Op1.reg)
	{
		si.jumps = ::get_long(insLrw.Op2.addr);
		si.startea = insLrw.ea;
		si.defjump = insJb.Op1.addr;
		si.flags = SWI_J32 | SWI_DEFAULT;

		return true;
	}
	return false;
}
bool emulate_c::isSwitch(switch_info_t* si)
{
	insn_t    ins = cmd;

	func_t* fn = ::get_func(ins.ea);

	// We'll start a recognition of switch idiom only 
	// if the instruction already is a part of a some function.
	// In this case we are able to check all cases of the jump table much easier 
	// by checking whether it is a same function frame at specified offset.
	if (fn != 0 &&
		ins.itype == ins_c::pcJmp &&
		emulate_c::checkSwitchIdiom(ins, *si))
	{
		uint size = 0;
		ea_t next = si->jumps;
		// Calculation of the size of the jump table
		while (true)
		{
			ea_t offs = ::get_long(next);
			// We would exit only in case if we met an offset to
			// anything else except current function
			if (::can_be_off32(offs) == BADADDR &&
				::get_func(offs) != fn)
			{
				break;
			}
			next += 4;
			++size;
		}
		si->ncases = size;

		return true;
	}
	return false;
}
int emulate_c::main()
{
	insn_t  ins = cmd;
	// Generally speaking, there're two important stages of the auto analysis:
	// AU_CODE
	//    Used for the instruction conversion. During this stage
	//    there must be provided an ordinal beat, instruction by instruction.
	//    As well as any procedure or label jumps must be planned.
	// AU_USED
	//    In case there was any attempt to convert any address into procedure
	//    during any stage by specifying auto_make_proc or whatever like this, 
	//    the specified address would be pushed into AU_USED queue as 
	//    a start address of the future procedure.
	//    And further, all addresses till the first met flow stop event/instruction will
	//    become the part of this procedure.
	//    In case if the next address was not analyzed yet, this would bring us 
	//    again to AU_CODE stage and only we would be returned to AU_USED stage.

	// Instruction conversion and operands handling
	// The current stage is unimportant at all
	// First operand
	if (::InstrIsSet(ins.itype, CF_USE1))
	{
		emulate_c::handleOperand(ins, ins.Op1);
	}
	// Second operand
	if (::InstrIsSet(ins.itype, CF_USE2))
	{
		emulate_c::handleOperand(ins, ins.Op2);
	}
	// Making SP tracing.
	// As far as the therm SP tracing is meaningful only for 
	// the procedures, may_trace_sp takes it into account by
	// checking whether the current stage is AU_USED
	if (::may_trace_sp())
	{
		// Checking if we've been met any instruction 
		// which doesn't yield the execution further
		if (emulate_c::isFlowStopInstruction(ins) &&
			::is_call_insn(ins.ea))
		{
			::recalc_spd(ins.ea);
		}
		else
		{
			traceSP(ins);
		}
	}
	// Planning the next instruction just after the current
	// The current stage is unimportant at all,
	// as far as in any case we must plan the next instruction
	if (!emulate_c::isFlowStopInstruction(ins))
	{
		::ua_add_cref(0, ins.ea + ins.size, fl_F);
	}
	return 2;
}