#ifndef _FN_EMULATE_HPP
#define _FN_EMULATE_HPP

#include <fstream>
#include <set>

namespace mcore_n
{
	class emulate_c
	{
	public:
		/************************************************************************/
		/* Emulate one instruction                                              */
		/************************************************************************/
		static int main();

		static bool createFunctionFrame(func_t* frame);
		static int getFrameReturnSize(func_t* fn);
		static bool isSwitch(switch_info_t* si);
		static bool isFlowStopInstruction(const insn_t& ins);
	protected:
	private:
		// Is called by the handleOperand for the memory type of the operand
		static void handleOperandMem(const insn_t& ins, const op_t& x);
		static void handleOperandFar(const insn_t& ins, const op_t& x);
		static void handleOperand(const insn_t& ins, const op_t& x);
		// Checks whether the specified instruction doesn't yield an execution to any next one
		static void trackRegisters(const insn_t& ins);
		static uint getTrackedRegister(regs_c::general_e regNum);
//		static bool isFlowStopInstruction(const insn_t& ins);
		static bool peekPrevInstruction(const insn_t& ins, insn_t& prev);
		static bool isSPBasedOperand(const op_t& op);
		//static bool isRegisterOperand(const op_t& op, regs_c::general_e regNum);
		static void traceSP(const insn_t& ins);
		//
		static bool checkSwitchIdiom(const insn_t& ins, switch_info_t& si);
	private:
		static uint _trackedRegisters[regs_c::rgFake];
		emulate_c();
	};
}
#endif // _FN_EMULATE_HPP