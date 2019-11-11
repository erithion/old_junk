#ifndef _FN_GENERATE_HPP
#define _FN_GENERATE_HPP

// IDA types
//     op_t - see IDA SDK ua.hpp
//     ea_t - see IDA SDK pro.h

class op_t;
class insn_t;

namespace mcore_n
{
	/************************************************************************/
	/* The last stage of disassembly - output                               */
	/* The class implements the output of a single instruction with operands*/
	/************************************************************************/
	class generate_c
	{
	public:
		/************************************************************************/
		/* Output a single instruction pseudo code                              */
		/* This is a callback for processor_t::u_out                            */
		/************************************************************************/
		static void generateInstructionPseudoCode();

		/************************************************************************/
		/* Output a single instruction operand                                  */
		/* This is a callback for processor_t::u_outop                          */
		/************************************************************************/
		static bool generateInstructionOperands(op_t& x);

		/************************************************************************/
		/* Generate the disassembly header                                      */
		/************************************************************************/
		static void generateDocumentHeader();

		/************************************************************************/
		/* Generate the disassembly footer                                      */
		/************************************************************************/
		static void generateDocumentFooter();

		/************************************************************************/
		/* Generate text to denote thumb <-> normal mode switches               */
		/* (in general this function generates text                             */
		/* for the segment register change points)                              */
		/************************************************************************/
		static void generateSegmentAssume(ea_t ea);

		/************************************************************************/
		/* Generate the segment header text                                     */
		/************************************************************************/
		static void generateSegmentHeader(ea_t ea);

		/************************************************************************/
		/* Generate the segment footer text                                     */
		/************************************************************************/
		static void generateSegmentFooter(ea_t ea);

		/************************************************************************/
		/* Generate the special segment text (external, absolute                */
		/*  and communal symbols)                                               */
		/************************************************************************/
		static bool generateSegmentSpecialItem(ea_t ea, uchar segtype);

	protected:
	private:
		static const string _constPredefinedSegments[];
	private:
		static bool outOperandRegister(const insn_t& ins, const op_t& x);
		static bool outOperandImmediate(const insn_t& ins, const op_t& x);
		static bool outOperandDisplacement(const insn_t& ins, const op_t& x);
		static bool outOperandMemory(const insn_t& ins, const op_t& x);
		static bool outOperandNear(const insn_t& ins, const op_t& x);
		static bool outOperandFar(const insn_t& ins, const op_t& x);
	private:
		generate_c();
	};
}
#endif // _FN_GENERATE_HPP