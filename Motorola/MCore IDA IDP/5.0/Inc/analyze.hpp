#ifndef _FN_ANALYZE_HPP
#define _FN_ANALYZE_HPP

// IDA types
//     op_t - See IDA SDK ua.hpp
//     insn_t - See IDA SDK ua.hpp
class op_t;
class insn_t;

namespace mcore_n
{
	/************************************************************************/
	/* It is op_t::specflag1                                                */
	/************************************************************************/
#define opFlag specflag1
	/************************************************************************/
	/* First stage of disassembly - analysis                                */
	/* The class implements analysis of a single instruction with operands  */
	/************************************************************************/
	class analyze_c
	{
	public:
		struct opflags_s
		{
		public:
			typedef enum phrase_e
			{
				ptNone = 0,
				ptExpression = 2,    // Expression that should be generated in parentheses
				ptXoredImmediate = 1 // Immediate value, that should be XORed on output (~ImmVal)
			};
		public:
			uchar                _isWriteBack        : 1; // Typically it is operand!
			uchar                _isRegistersListSet : 1; // Is the register specified?
			uchar                _isControlRegister  : 1; // Is the register(list) number is a control register(list) number?
			/*phrase_e*/ uchar   _phraseType         : 2; // Phrase type
		};
	public:
		/************************************************************************/
		/* Analyzes instruction and fills 'cmd' structure                       */
		/* It is a callback (see processor_t::u_ana) for IDA kernel to step into*/
		/* while instructions are being analyzed one by one                     */
		/************************************************************************/
		static int handlerMain();

		static bool allowOperandType(op_t& x);

		static void useMacro(bool use);
		static bool useMacro();
	protected:
	private:
		/************************************************************************/
		/* M-Core opcodes (for internal usage only)                             */
		/* Takes part in the command identification.                            */
		/* Identification is done without taking into account command's operands*/
		/* I.e., all operand's part is assumed to be 0, only opcode is taken    */
		/************************************************************************/
		typedef enum opcode_e 
		{
			/************************************************************************/
			/* rrrr - Rx field                                                      */
			/* ssss - Ry field                                                      */
			/* zzzz - Rz field                                                      */
			/* ffff - Rfirst field                                                  */
			/* cccc - Control Register Specifier                                    */
			/* iiii - Immediate field                                               */
			/* dddd - Branch displacement                                           */
			/* bbbb - Loop displacement                                             */
			/* uuuu - Accelerator unit                                              */
			/* eeee - Execution code                                                */
			/* nnnn - Register count                                                */
			/* pppp - Update option                                                 */
			/* xxxx - Undefined field                                               */
			/************************************************************************/
			// M-Core instruction set
			//-------------------------------------------
			mcBkpt = 0x0,
			mcSync = 0x1,
			mcRte = 0x2,
			mcRfi = 0x3,
			mcStop = 0x4,
			mcWait = 0x5,
			mcDoze = 0x6,
			//-------------------------------------------
			mcTrap = 0x8,           //             b10     ii
			//-------------------------------------------
			mcMvc = 0x20,           //           b10     rrrr
			mcMvcv = 0x30,          //           b11     rrrr
			mcLdq = 0x40,           //          b100     rrrr
			mcStq = 0x50,           //          b101     rrrr
			mcLdm = 0x60,           //          b110     rrrr
			mcStm = 0x70,           //          b111     rrrr
			mcDect = 0x80,          //         b1000     rrrr
			mcDecf = 0x90,          //         b1001     rrrr
			mcInct = 0xA0,          //         b1010     rrrr
			mcIncf = 0xB0,          //         b1011     rrrr
			mcJmp = 0xC0,           //         b1100     rrrr
			mcJsr = 0xD0,           //         b1101     rrrr
			mcFf1 = 0xE0,           //         b1110     rrrr
			mcBrev = 0xF0,          //         b1111     rrrr
			mcXtrb3 = 0x100,        //        b10000     rrrr
			mcXtrb2 = 0x110,        //        b10001     rrrr
			mcXtrb1 = 0x120,        //        b10010     rrrr
			mcXtrb0 = 0x130,        //        b10011     rrrr
			mcZextb = 0x140,        //        b10100     rrrr
			mcSextb = 0x150,        //        b10101     rrrr
			mcZexth = 0x160,        //        b10110     rrrr
			mcSexth = 0x170,        //        b10111     rrrr
			mcDeclt = 0x180,        //        b11000     rrrr
			mcTstnbz = 0x190,       //        b11001     rrrr
			mcDecgt = 0x1A0,        //        b11010     rrrr
			mcDecne = 0x1B0,        //        b11011     rrrr
			mcClrt = 0x1C0,         //        b11100     rrrr
			mcClrf = 0x1D0,         //        b11101     rrrr
			mcAbs = 0x1E0,          //        b11110     rrrr
			mcNot = 0x1F0,          //        b11111     rrrr
			mcBmaski32Set = 0x2C00, //   b1011000000     rrrr
			mcDivu = 0x2C10,        //   b1011000001     rrrr
			mcDivs = 0x3210,        //   b1100100001     rrrr
			mcBgeni = 0x3270,       //   b1100100111     rrrr
			mcXsr = 0x3800,         //   b1110000000     rrrr
			mcAsrc = 0x3A00,        //   b1110100000     rrrr
			mcLslc = 0x3C00,        //   b1111000000     rrrr
			mcLsrc = 0x3E00,        //   b1111100000     rrrr
			//-------------------------------------------
			mcBmaskiImm3 = 0x2C80,  //  b1011001      iiirrrr
			mcBgeniImm3 = 0x3280,   //  b1100101      iiirrrr
			//-------------------------------------------
			mcMovt = 0x200,         //      b10      ssssrrrr
			mcMult = 0x300,         //      b11      ssssrrrr
			mcLoopt = 0x400,        //     b100      ssssbbbb
			mcSubu = 0x500,         //     b101      ssssrrrr
			mcAddc = 0x600,         //     b110      ssssrrrr
			mcSubc = 0x700,         //     b111      ssssrrrr
			mcMovf = 0xA00,         //    b1010      ssssrrrr
			mcLsr = 0xB00,          //    b1011      ssssrrrr
			mcCmphs = 0xC00,        //    b1100      ssssrrrr
			mcCmplt = 0xD00,        //    b1101      ssssrrrr
			mcTst = 0xE00,          //    b1110      ssssrrrr
			mcCmpne = 0xF00,        //    b1111      ssssrrrr
			mcMov = 0x1200,         //   b10010      ssssrrrr
			mcBgenr = 0x1300,       //   b10011      ssssrrrr
			mcRsub = 0x1400,        //   b10100      ssssrrrr
			mcIxw = 0x1500,         //   b10101      ssssrrrr
			mcAnd = 0x1600,         //   b10110      ssssrrrr
			mcXor = 0x1700,         //   b10111      ssssrrrr
			mcAsr = 0x1A00,         //   b11010      ssssrrrr
			mcLsl = 0x1B00,         //   b11011      ssssrrrr
			mcAddu = 0x1C00,        //   b11100      ssssrrrr
			mcIxh = 0x1D00,         //   b11101      ssssrrrr
			mcOr = 0x1E00,          //   b11110      ssssrrrr
			mcAndn = 0x1F00,        //   b11111      ssssrrrr

			mcBmaskiImm5 = 0x2C00,  //  b101101      iiiiirrrr
			mcBgeniImm5 = 0x3200,   //  b110011      iiiiirrrr

			mcJmpi = 0x7000,        //  b1110000     dddddddd
			mcJsri = 0x7F00,        //  b1111111     dddddddd
			//-------------------------------------------
			mcMfcr = 0x1000,        //   b1000      cccccrrrr
			mcMtcr = 0x1800,        //   b1100      cccccrrrr

			mcAddi = 0x2000,        //  b10000      iiiiirrrr
			mcCmplti = 0x2200,      //  b10001      iiiiirrrr
			mcSubi = 0x2400,        //  b10010      iiiiirrrr
			mcRsubi = 0x2800,       //  b10100      iiiiirrrr
			mcCmpnei = 0x2A00,      //  b10101      iiiiirrrr
			mcAndi = 0x2E00,        //  b10111      iiiiirrrr
			mcBclri = 0x3000,       //  b11000      iiiiirrrr
			mcBseti = 0x3400,       //  b11010      iiiiirrrr
			mcBtsti = 0x3600,       //  b11011      iiiiirrrr
			mcRotli = 0x3800,       //  b11100      iiiiirrrr
			mcAsri = 0x3A00,        //  b11101      iiiiirrrr
			mcLsli = 0x3C00,        //  b11110      iiiiirrrr
			mcLsri = 0x3E00,        //  b11111      iiiiirrrr
			//-------------------------------------------
			mcMovi = 0x6000,        // b1100      iiiiiiirrrr
			mcBt = 0xE000,          // b11100     ddddddddddd
			mcBf = 0xE800,          // b11101     ddddddddddd
			mcBr = 0xF000,          // b11110     ddddddddddd
			mcBsr = 0xF800,         // b11111     ddddddddddd
			//-------------------------------------------
			mcLrw = 0x7000,         // b111      zzzzdddddddd

			mcH_exec = 0x4000,      //  b100     uu00eeeeeeee
			mcH_ret = 0x4100,       //  b100     uu010nnneeee
			mcH_call = 0x4180,      //  b100     uu011nnneeee
			mcH_ld = 0x4200,        //  b100     uu100piirrrr
			mcH_st = 0x4280,        //  b100     uu101piirrrr
			mcH_ldh = 0x4300,       //  b100     uu110piirrrr
			mcH_sth = 0x4380,       //  b100     uu111piirrrr

			mcLd = 0x8000,          // b1000     zzzziiiirrrr
			mcSt = 0x9000,          // b1001     zzzziiiirrrr
			mcLdb = 0xA000,         // b1010     zzzziiiirrrr
			mcStb = 0xB000,         // b1011     zzzziiiirrrr
			mcLdh = 0xC000,         // b1100     zzzziiiirrrr
			mcSth = 0xD000,         // b1101     zzzziiiirrrr
			//-------------------------------------------
			mcEnd = 0x10000         // b10000000000000000
		};

	private:
		static bool _useMacroCommands;
	private:
		/************************************************************************/
		/* Splits the command                                                   */
		/* Arguments:                                                           */
		/*    code - 2-byte opcode of the instruction                           */
		/*    ins - IDA insn_t result of spliting                               */
		/* Results:                                                             */
		/*    Returns logical 1 if the command has been recognized successfully,*/
		/*    logical 0 otherwise                                               */
		/************************************************************************/
		/* Command output:                                                      */
		/*    insn_t::size - command size in bytes                              */
		/*    insn_t::itype - command value in accordance to pseudocode_e enum  */
		/*    insn_t::insnpref - command operands count                         */
		/* Operands output types:                                               */
		/*    o_reg - register                                                  */
		/*       op_t::dtyp - operand type                                      */
		/*       op_t::reg - register number                                    */
		/*       op_t::opFlag - operand flags                                   */
		/*    o_imm - immediate value                                           */
		/*       op_t::dtyp - operand type                                      */
		/*       op_t::value - operand value                                    */
		/*    o_displ - displacement                                            */
		/*       op_t::dtyp - operand type                                      */
		/*       op_t::reg - register part of the operand                       */
		/*       op_t::addr - offset part of the operand                        */
		/*       op_t::opFlag - operand flags                                   */
		/*    o_mem - offset to literal pool data                               */
		/*       op_t::dtyp - operand type                                      */
		/*       op_t::addr - offset value                                      */
		/*    o_near - offset to code                                           */
		/*       op_t::dtyp - operand type                                      */
		/*       op_t::addr - offset value                                      */
		/************************************************************************/
		static inline bool splitCommand(ushort code, insn_t& ins);

		/************************************************************************/
		/* Utility methods for opcodes recognition by group                     */
		/************************************************************************/
		static inline bool groupFirst(ushort code, insn_t& ins);
		static inline bool groupSecond(ushort code, insn_t& ins);
		static inline bool groupThird(ushort code, insn_t& ins);
		static inline bool groupFourth(ushort code, insn_t& ins);
		static inline bool groupFifth(ushort code, insn_t& ins);
		static inline bool groupSixth(ushort code, insn_t& ins);
		static inline bool groupSeventh(ushort code, insn_t& ins);
		static inline bool groupEighth(ushort code, insn_t& ins);

		/************************************************************************/
		/* Utility method for changing word endianess                           */
		/************************************************************************/
		static inline ushort revertByteOrder(ushort word);

		static bool isNextInstructionFitMacro(const insn_t& ins, insn_t& next);
		static bool doMacro(insn_t& ins);
		static void createMacroImmediateValueSize4(insn_t& ins, ins_c::ins_e code, longlong value);
		static void createMacroImmediateValueSize2(insn_t& ins, ins_c::ins_e code, longlong value);
		static void setXORedImmediateOperandFlag(op_t& op);
	private:
		analyze_c();
	};
}
#endif // _FN_ANALIZE_HPP