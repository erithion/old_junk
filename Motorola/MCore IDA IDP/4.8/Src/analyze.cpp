#include "idaidp.hpp"

#include "types.hpp"
#include "ins.hpp"
#include "analyze.hpp"
#include "reg.hpp"

using namespace mcore_n;

bool analyze_c::_useMacroCommands = true;
/************************************************************************/
/* analyze_c                                                            */
/************************************************************************/
bool analyze_c::groupFirst(ushort code, insn_t& ins)
{
#pragma pack(push)
#pragma pack(1)
	struct opnd_s
	{
	public:
		union
		{
		public:
			struct  
			{
			public:
				ushort   _dddddddd : 8;
				ushort   _zzzz     : 4;
			} _zzzz_dddddddd;
			struct  
			{
			public:
				ushort   _eeeeeeee : 8;
	            char               : 2;
				ushort   _uu       : 2;
			} _uu_eeeeeeee;
			struct  
			{
			public:
				ushort   _eeee  : 4;
				ushort   _nnn   : 3;
                char            : 3;
				ushort   _uu    : 2;
			} _uu_nnn_eeee;
			struct  
			{
			public:
				ushort   _rrrr : 4;
				ushort   _ii   : 2;
				ushort   _p    : 1;
                char           : 3;
				ushort   _uu   : 2;
			} _uu_p_ii_rrrr;
			struct 
			{
			public:
				ushort   _rrrr : 4;
				ushort   _iiii : 4;
				ushort   _zzzz : 4;
			} _zzzz_iiii_rrrr;
		};
	};
#pragma pack(pop)

	opnd_s* op = (opnd_s*)&code;

	/* Determining whether the opcode is from the first instruction set group of 12 bit long operands
	mcLrw = 0x7000,         //  b111     zzzz dddddddd
	mcH_exec = 0x4000,      //  b100     uu 00 eeeeeeee
	mcH_ret = 0x4100,       //  b100     uu 010 nnn eeee
	mcH_call = 0x4180,      //  b100     uu 011 nnn eeee
	mcH_ld = 0x4200,        //  b100     uu 100 p ii rrrr
	mcH_st = 0x4280,        //  b100     uu 101 p ii rrrr
	mcH_ldh = 0x4300,       //  b100     uu 110 p ii rrrr
	mcH_sth = 0x4380,       //  b100     uu 111 p ii rrrr
	mcLd = 0x8000,          // b1000     zzzz iiii rrrr
	mcSt = 0x9000,          // b1001     zzzz iiii rrrr
	mcLdb = 0xA000,         // b1010     zzzz iiii rrrr
	mcStb = 0xB000,         // b1011     zzzz iiii rrrr
	mcLdh = 0xC000,         // b1100     zzzz iiii rrrr
	mcSth = 0xD000,         // b1101     zzzz iiii rrrr
	*/

	ushort  id = (code >> 12) << 12;

	switch (id) 
	{
	case mcH_exec:
		{
			/*
			Syntax:
			H_EXEC #UU, #CODE

			#UU:
			00 - Block 0
			01 - Block 1
			10 - Block 2
			11 - Block 3

			#CODE:
			Operation code for hardware block
			*/

			ins.itype = ins_c::pcH_exec;

			ins.Op1.value = op->_uu_eeeeeeee._eeeeeeee;
			ins.Op1.type = o_imm;
			ins.Op1.dtyp = dt_byte;

			ins.Op2.value = op->_uu_eeeeeeee._uu;
			ins.Op2.type = o_imm;
			ins.Op2.dtyp = dt_byte;

			return true;
		}
		break;
	case mcH_call:
	case mcH_ret:
		{
			/*
			Syntax:
			H_CALL #UU, r4-rLast, #CODE

			#UU:
			00 - Block 0
			01 - Block 1
			10 - Block 2
			11 - Block 3

			r4-rLast:

			000 - Reserved. Do not use.
			001 - pass r4
			010 - pass r4, r5
			...
			111 - pass r4 - r10
			*/

			switch (id)
			{
			case mcH_ret: ins.itype = ins_c::pcH_ret; break;
			case mcH_call: ins.itype = ins_c::pcH_call; break;
			}


			ins.Op1.value = op->_uu_nnn_eeee._eeee;
			ins.Op1.type = o_imm;
			ins.Op1.dtyp = dt_byte;

			ins.Op2.reg = op->_uu_nnn_eeee._nnn;
			opflags_s* flag = (opflags_s*)&ins.Op2.opFlag;
			flag->_isRegistersListSet = true;
			ins.Op2.dtyp = dt_void;

			ins.Op3.value = op->_uu_nnn_eeee._uu;
			ins.Op3.type = o_imm;
			ins.Op3.dtyp = dt_byte;

			return true;
		}
		break;
	case mcH_sth:
	case mcH_ldh:
	case mcH_st:
	case mcH_ld:
		{
			/*
			Syntax:
			H_LD [u] #UU, (rx, disp)

			[u]:
			0 - Base register should NOT be updated
			1 - Base register should be updated

			#UU:
			00 - Block 0
			01 - Block 1
			10 - Block 2
			11 - Block 3

			rx - base address in register X to be added with displacement

			disp - displacement to be added with base address value
			*/

			switch (id)
			{
			case mcH_ld: ins.itype = ins_c::pcH_ld; break;
			case mcH_st: ins.itype = ins_c::pcH_st; break;
			case mcH_ldh: ins.itype = ins_c::pcH_ldh; break;
			case mcH_sth: ins.itype = ins_c::pcH_sth; break;
			}



			ins.Op1.reg = op->_uu_p_ii_rrrr._rrrr;
			ins.Op1.addr = (id == mcH_sth || id == mcH_ldh) ? op->_uu_p_ii_rrrr._ii << 1 : op->_uu_p_ii_rrrr._ii;
			opflags_s* flag = (opflags_s*)&ins.Op1.opFlag;
			flag->_isWriteBack = op->_uu_p_ii_rrrr._p;
			flag->_phraseType = opflags_s::ptExpression;
			ins.Op1.type = o_displ;
			ins.Op1.dtyp = (id == mcH_sth || id == mcH_ldh) ? dt_byte : dt_word;

			ins.Op2.value = op->_uu_p_ii_rrrr._uu;
			ins.Op2.type = o_imm;
			ins.Op2.dtyp = dt_byte;

			return true;
		}
		break;
	case mcLrw:
		{
			/*
			Syntax:

			rz <- MEM(pc + 2 + unsigned_imm8 << 2) & 0xFFFFFFFC

			LRW rz, [label]
			LRW rz, label
			LRW rz, 0x...
			*/

			if (op->_zzzz_dddddddd._zzzz < 1 ||
				op->_zzzz_dddddddd._zzzz > 14)
			{
				return false;
			}

			ins.itype = ins_c::pcLrw;

			ins.Op2.addr = ins.ea + 2 + (op->_zzzz_dddddddd._dddddddd << 2);
			ins.Op2.addr &= 0xFFFFFFFC; 
			ins.Op2.type = o_mem;
			ins.Op2.dtyp = dt_dword;
			opflags_s* flag = (opflags_s*)&ins.Op2.opFlag;
			flag->_phraseType = opflags_s::ptExpression;

			ins.Op1.reg = op->_zzzz_dddddddd._zzzz;
			ins.Op1.type = o_reg;
			ins.Op1.dtyp = dt_dword;

			return true;
		}
		break;
	case mcSth:
	case mcLdh:
	case mcStb:
	case mcLdb:
	case mcSt:
	case mcLd:
		{
			/*
			Syntax:

			rz <- MEM[rx + unsigned_imm4 << {0, 1, 2}]

			LD{w,h,b} rz, (rx, disp)

			*/
			size_t     scale = 0;

			opflags_s* flag = (opflags_s*)&ins.Op2.opFlag;

			switch (id) 
			{
			case mcLd: 
			case mcSt: 
				{
					ins.itype = id == mcLd ? ins_c::pcLd : ins_c::pcSt; 
					ins.Op1.dtyp = ins.Op2.dtyp = dt_dword;
					scale = 2;
				}
				break;
			case mcSth: 
			case mcLdh: 
				{
					ins.itype = id == mcLdh ? ins_c::pcLdh : ins_c::pcSth;
					ins.Op1.dtyp = ins.Op2.dtyp = dt_word;
					scale = 1;
				}
				break;
			case mcStb: 
			case mcLdb: 
				{
					ins.itype = id == mcLdb ? ins_c::pcLdb : ins_c::pcStb;
					ins.Op1.dtyp = ins.Op2.dtyp = dt_byte;
				}
				break;
			}

			ins.Op2.reg = op->_zzzz_iiii_rrrr._rrrr;
			ins.Op2.type = o_displ;
			ins.Op2.addr = op->_zzzz_iiii_rrrr._iiii << scale;
			flag->_phraseType = opflags_s::ptExpression;

			ins.Op1.reg = op->_zzzz_iiii_rrrr._zzzz;
			ins.Op1.type = o_reg;

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::groupSecond(ushort code, insn_t& ins)
{
#pragma pack(push)
#pragma pack(1)
	struct opnd_s
	{
	public:
		union
		{
		public:
			struct  
			{
			public:
				ushort   _rrrr    : 4;
				ushort   _iiiiiii : 7;
			} _iiiiiii_rrrr;
			struct 
			{
			public:
				short    _ddddddddddd : 11;
			} _ddddddddddd;
		};
	};
#pragma pack(pop)

	opnd_s* op = (opnd_s*)&code;

	/* Determining whether the opcode is from the first instruction set group of 11 bit long operands
	mcMovi = 0x6000,        // b1100      iiiiiii rrrr
	mcBt = 0xE000,          // b11100     ddddddddddd
	mcBf = 0xE800,          // b11101     ddddddddddd
	mcBr = 0xF000,          // b11110     ddddddddddd
	mcBsr = 0xF800,         // b11111     ddddddddddd
	*/

	ushort  id = (code >> 11) << 11;

	switch (id) 
	{
	case mcMovi:
		{
			/*
			Syntax:

			RX <- unsigned_imm7

			MOVI rx, imm7
			*/
			ins.itype = ins_c::pcMovi;

			ins.Op1.reg = op->_iiiiiii_rrrr._rrrr;
			ins.Op1.type = o_reg;
			ins.Op1.dtyp = dt_dword;

			ins.Op2.value = op->_iiiiiii_rrrr._iiiiiii;
			ins.Op2.type = o_imm;
			ins.Op2.dtyp = dt_byte;

			return true;
		}
		break;
	case mcBf:
	case mcBsr:
	case mcBr:
	case mcBt:
		{
			/*
			Syntax: (Common)

			r15 <- pc + 2
			pc <- pc + 2 + (signed extended 11 bit displacement << 1)

			bsr label

			*/

			switch (id)
			{
			case mcBt: ins.itype = ins_c::pcBt; break;
			case mcBf: ins.itype = ins_c::pcBf; break;
			case mcBr: ins.itype = ins_c::pcBr; break;
			case mcBsr: ins.itype = ins_c::pcBsr; break;
			}


			ins.Op1.addr = ins.ea + 2 + (op->_ddddddddddd._ddddddddddd << 1);
			ins.Op1.type = o_near;
			ins.Op1.dtyp = dt_code;

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::groupThird(ushort code, insn_t& ins)
{
#pragma pack(push)
#pragma pack(1)
	struct opnd_s
	{
	public:
		union
		{
		public:
			struct  
			{
			public:
				ushort   _rrrr    : 4;
				ushort   _ccccc   : 5;
			} _ccccc_rrrr;
			struct 
			{
			public:
				ushort   _rrrr    : 4;
				ushort   _iiiii   : 5;
			} _iiiii_rrrr;
		};
	};
#pragma pack(pop)

	opnd_s* op = (opnd_s*)&code;

	/* Determining whether the opcode is from the first instruction set group of 9 bit long operands
	mcMfcr = 0x1000,        //   b1000      ccccc rrrr
	mcMtcr = 0x1800,        //   b1100      ccccc rrrr
	mcAddi = 0x2000,        //  b10000      iiiii rrrr
	mcCmplti = 0x2200,      //  b10001      iiiii rrrr
	mcSubi = 0x2400,        //  b10010      iiiii rrrr
	mcRsubi = 0x2800,       //  b10100      iiiii rrrr
	mcCmpnei = 0x2A00,      //  b10101      iiiii rrrr
	mcAndi = 0x2E00,        //  b10111      iiiii rrrr
	mcBclri = 0x3000,       //  b11000      iiiii rrrr
	mcBseti = 0x3400,       //  b11010      iiiii rrrr
	mcBtsti = 0x3600,       //  b11011      iiiii rrrr
	mcRotli = 0x3800,       //  b11100      iiiii rrrr
	mcAsri = 0x3A00,        //  b11101      iiiii rrrr
	mcLsli = 0x3C00,        //  b11110      iiiii rrrr
	mcLsri = 0x3E00,        //  b11111      iiiii rrrr
	mcBmaskiImm5 = 0x2D00,  //  b101101     iiiii rrrr
	mcBgeniImm5 = 0x3300,   //  b110011     iiiii rrrr
	*/

	ushort  id = (code >> 9) << 9;

	switch (id) 
	{
	case mcMtcr:
	case mcMfcr:
		{
			/*
			Syntax: (Common)

			rx <- cry

			mfcr rx, cry

			rx:
			0000 - r0
			0001 - r1
			... 
			1111 - r15

			cry:
			00000 - cr0
			00001 - cr1
			...
			11111 - cr31
			*/
			switch (id)
			{
			case mcMfcr: ins.itype = ins_c::pcMfcr; break;
			case mcMtcr: ins.itype = ins_c::pcMtcr; break;
			}


			ins.Op1.reg = op->_ccccc_rrrr._rrrr;
			ins.Op1.type = o_reg;
			ins.Op1.dtyp = dt_dword;

			ins.Op2.reg = op->_ccccc_rrrr._ccccc;
			ins.Op2.type = o_reg;
			opflags_s* flag = (opflags_s*)&ins.Op2.opFlag;
			flag->_isControlRegister = true;
			ins.Op2.dtyp = dt_dword;

			return true;
		}
		break;
	case mcLsri:
	case mcAsri:
	case mcLsli:
	case mcRotli:
		{
			if (op->_iiiii_rrrr._iiiii == 0)
			{
				// It is (ASR/LSL/LSR)C command then
				return false;
			}
		}
		// Break is missed intentionally
	case mcRsubi:
	case mcSubi:
	case mcBtsti:
	case mcBseti:
	case mcBclri:
	case mcAndi:
	case mcCmpnei:
	case mcCmplti:
	case mcAddi:
	case mcBgeniImm5:
	case mcBmaskiImm5:
		{
			int correction = 0;
			/*
			Syntax: (Common)

			rx <- Op(Rx) by Imm5

			lsri rx, imm5
			*/
			switch (id)
			{
			case mcLsri: ins.itype = ins_c::pcLsri; break;
			case mcLsli: ins.itype = ins_c::pcLsli; break;
			case mcAsri: ins.itype = ins_c::pcAsri; break;
			case mcBgeniImm5: 
				{
					// Operand value 0-6 re-maps the command on to movi
					if (op->_iiiii_rrrr._iiiii < 7)
					{
						return false;
					}
					ins.itype = ins_c::pcBgeniImm5; 
				}
				break;
			case mcBmaskiImm5:
				{
					// Operand value 1-7 re-maps the command on to movi
					if (op->_iiiii_rrrr._iiiii < 8 &&
						op->_iiiii_rrrr._iiiii > 0)
					{
						return false;
					}
					else if (op->_iiiii_rrrr._iiiii == 0)
					{
						// 0 means 32 bits
						correction = 32;
					}
					ins.itype = ins_c::pcBmaskiImm5; 
				}
				break;
			case mcRotli: ins.itype = ins_c::pcRotli; break;
			case mcBtsti: ins.itype = ins_c::pcBtsti; break;
			case mcBseti: ins.itype = ins_c::pcBseti; break;
			case mcBclri: ins.itype = ins_c::pcBclri; break;
			case mcAndi: ins.itype = ins_c::pcAndi; break;
			case mcCmpnei: ins.itype = ins_c::pcCmpnei; break;
			case mcRsubi: ins.itype = ins_c::pcRsubi; break;
			case mcSubi: 
				{
					ins.itype = ins_c::pcSubi;
					correction = 1;
				}
				break;
			case mcCmplti:
				{
					ins.itype = ins_c::pcCmplti; 
					correction = 1;
				}
				break;
			case mcAddi:
				{
					ins.itype = ins_c::pcAddi; 
					correction = 1;
				}
				break;
			}


			ins.Op1.reg = op->_iiiii_rrrr._rrrr;
			ins.Op1.type = o_reg;
			ins.Op1.dtyp = dt_dword;

			ins.Op2.value = op->_iiiii_rrrr._iiiii + correction;
			ins.Op2.type = o_imm;
			ins.Op2.dtyp = dt_dword;

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::groupFourth(ushort code, insn_t& ins)
{
#pragma pack(push)
#pragma pack(1)
	struct opnd_s
	{
	public:
		union
		{
		public:
			struct  
			{
			public:
				ushort   _rrrr    : 4;
				ushort   _ssss    : 4;
			} _ssss_rrrr;
			struct 
			{
			public:
				ushort   _dddddddd: 8;
			} _dddddddd;
			struct  
			{
			public:
				ushort   _bbbb    : 4;
				ushort   _ssss    : 4;
			} _ssss_bbbb;
		};
	};
#pragma pack(pop)

	opnd_s* op = (opnd_s*)&code;

	/* Determining whether the opcode is from the first instruction set group of 8 bit long operands
	mcMovt = 0x200,         //      b10      ssss rrrr
	mcMult = 0x300,         //      b11      ssss rrrr
	mcSubu = 0x500,         //     b101      ssss rrrr
	mcAddc = 0x600,         //     b110      ssss rrrr
	mcSubc = 0x700,         //     b111      ssss rrrr
	mcMovf = 0xA00,         //    b1010      ssss rrrr
	mcLsr = 0xB00,          //    b1011      ssss rrrr
	mcCmphs = 0xC00,        //    b1100      ssss rrrr
	mcCmplt = 0xD00,        //    b1101      ssss rrrr
	mcTst = 0xE00,          //    b1110      ssss rrrr
	mcCmpne = 0xF00,        //    b1111      ssss rrrr
	mcMov = 0x1200,         //   b10010      ssss rrrr
	mcBgenr = 0x1300,       //   b10011      ssss rrrr
	mcRsub = 0x1400,        //   b10100      ssss rrrr
	mcIxw = 0x1500,         //   b10101      ssss rrrr
	mcAnd = 0x1600,         //   b10110      ssss rrrr
	mcXor = 0x1700,         //   b10111      ssss rrrr
	mcAsr = 0x1A00,         //   b11010      ssss rrrr
	mcLsl = 0x1B00,         //   b11011      ssss rrrr
	mcAddu = 0x1C00,        //   b11100      ssss rrrr
	mcIxh = 0x1D00,         //   b11101      ssss rrrr
	mcOr = 0x1E00,          //   b11110      ssss rrrr
	mcAndn = 0x1F00,        //   b11111      ssss rrrr
	mcLoopt = 0x400,        //     b100      ssss bbbb
	mcJmpi = 0x7000,        //  b1110000     dddddddd
	mcJsri = 0x7F00,        //  b1111111     dddddddd
	*/

	ushort  id = (code >> 8) << 8;

	switch (id) 
	{
	case mcLoopt:
		{
			ins.Op2.reg = op->_ssss_bbbb._ssss;
			ins.Op2.type = o_reg;
			ins.Op2.dtyp = dt_dword;

			ins.Op1.addr = op->_ssss_bbbb._bbbb * 2 - 32;
			ins.Op1.type = o_near;
			ins.Op1.dtyp = dt_code;
		}
		break;
	case mcAnd:
	case mcXor:
	case mcAsr:
	case mcLsl:
	case mcAddu:
	case mcIxh:
	case mcOr:
	case mcIxw:
	case mcRsub:
	case mcBgenr:
	case mcMov:
	case mcCmpne:
	case mcTst:
	case mcCmplt:
	case mcCmphs:
	case mcLsr:
	case mcMovf:
	case mcSubc:
	case mcAddc:
	case mcSubu:
	case mcMult:
	case mcMovt:
	case mcAndn:
		{
			switch (id) 
			{
			case mcAnd: ins.itype = ins_c::pcAnd; break;
			case mcXor: ins.itype = ins_c::pcXor; break;
			case mcAsr: ins.itype = ins_c::pcAsr; break;
			case mcLsl: ins.itype = ins_c::pcLsl; break;
			case mcAddu: ins.itype = ins_c::pcAddu; break;
			case mcIxh: ins.itype = ins_c::pcIxh; break;
			case mcOr: ins.itype = ins_c::pcOr; break;
			case mcIxw: ins.itype = ins_c::pcIxw; break;
			case mcRsub: ins.itype = ins_c::pcRsub; break;
			case mcBgenr: ins.itype = ins_c::pcBgenr; break;
			case mcMov: ins.itype = ins_c::pcMov; break;
			case mcCmpne: ins.itype = ins_c::pcCmpne; break;
			case mcTst: ins.itype = ins_c::pcTst; break;
			case mcCmplt: ins.itype = ins_c::pcCmplt; break;
			case mcCmphs: ins.itype = ins_c::pcCmphs; break;
			case mcLsr: ins.itype = ins_c::pcLsr; break;
			case mcMovf: ins.itype = ins_c::pcMovf; break;
			case mcSubc: ins.itype = ins_c::pcSubc; break;
			case mcAddc: ins.itype = ins_c::pcAddc; break;
			case mcSubu: ins.itype = ins_c::pcSubu; break;
			case mcLoopt: ins.itype = ins_c::pcLoopt; break;
			case mcMult: ins.itype = ins_c::pcMult; break;
			case mcMovt: ins.itype = ins_c::pcMovt; break;
			case mcAndn: ins.itype = ins_c::pcAndn; break;
			}


			ins.Op1.reg = op->_ssss_rrrr._rrrr;
			ins.Op1.type = o_reg;
			ins.Op1.dtyp = dt_dword;

			ins.Op2.reg = op->_ssss_rrrr._ssss;
			ins.Op2.type = o_reg;
			ins.Op2.dtyp = dt_dword;

			return true;
		}
		break;
	case mcJsri:
	case mcJmpi:
		{
			/*
			Syntax: (Common)

			r15 <- pc + 2
			pc <- MEM[(pc + 2 + unsigned_disp8 << 2) & 0xfffffffc]

			JSRI [label]
			*/
			switch (id) 
			{
			case mcJsri: ins.itype = ins_c::pcJsri; break;
			case mcJmpi: ins.itype = ins_c::pcJmpi; break;
			}

			ins.Op1.addr = ins.ea + 2 + (op->_dddddddd._dddddddd << 2);
			ins.Op1.addr &= 0xFFFFFFFC; 
			//ins.Op1.type = o_mem;
			ins.Op1.type = o_far;
			ins.Op1.dtyp = dt_dword;
			opflags_s* flag = (opflags_s*)&ins.Op1.opFlag;
			flag->_phraseType = opflags_s::ptExpression;

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::groupFifth(ushort code, insn_t& ins)
{
#pragma pack(push)
#pragma pack(1)
	struct opnd_s
	{
	public:
		union
		{
		public:
			struct  
			{
			public:
				ushort   _rrrr    : 4;
				ushort   _iii     : 3;
			} _iii_rrrr;
		};
	};
#pragma pack(pop)

	opnd_s* op = (opnd_s*)&code;

	/* Determining whether the opcode is from the first instruction set group of 7 bit long operands
	mcBmaskiImm3 = 0x2C80,  //  b1011001      iii rrrr
	mcBgeniImm3 = 0x3280,   //  b1100101      iii rrrr
	*/

	ushort  id = (code >> 7) << 7;

	switch (id) 
	{
	case mcBgeniImm3:
	case mcBmaskiImm3:
		{
			switch (id) 
			{
			case mcBmaskiImm3: ins.itype = ins_c::pcBmaskiImm3; break;
			case mcBgeniImm3: ins.itype = ins_c::pcBgeniImm3; break;
			}


			ins.Op1.reg = op->_iii_rrrr._rrrr;
			ins.Op1.type = o_reg;
			ins.Op1.dtyp = dt_dword;

			ins.Op1.value = op->_iii_rrrr._iii;
			ins.Op1.type = o_imm;
			ins.Op1.dtyp = dt_byte;

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::groupSixth(ushort code, insn_t& ins)
{
#pragma pack(push)
#pragma pack(1)
	struct opnd_s
	{
	public:
		union
		{
		public:
			struct  
			{
			public:
				ushort   _rrrr    : 4;
			} _rrrr;
		};
	};
#pragma pack(pop)

	opnd_s* op = (opnd_s*)&code;

	/* Determining whether the opcode is from the first instruction set group of 4 bit long operands
	mcMvc = 0x10,           //           b10     rrrr
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
	*/

	ushort  id = (code >> 4) << 4;

	switch (id) 
	{
	case mcMvcv:
	case mcLdq:
	case mcStq:
	case mcLdm:
	case mcStm:
	case mcDect:
	case mcDecf:
	case mcInct:
	case mcIncf:
	case mcJmp:
	case mcJsr:
	case mcFf1:
	case mcBrev:
	case mcXtrb0:
	case mcXtrb1:
	case mcXtrb2:
	case mcXtrb3:
	case mcZextb:
	case mcSextb:
	case mcZexth:
	case mcSexth:
	case mcDeclt:
	case mcTstnbz:
	case mcDecgt:
	case mcDecne:
	case mcClrt:
	case mcClrf:
	case mcAbs:
	case mcNot:
	case mcBmaski32Set:
	case mcDivu:
	case mcDivs:
	case mcBgeni:
	case mcXsr:
	case mcAsrc:
	case mcLslc:
	case mcLsrc:
	case mcMvc:
		{

			bool overloaded = false;

			switch (id) 
			{
			case mcMvc: ins.itype = ins_c::pcMvc; break;
			case mcMvcv: ins.itype = ins_c::pcMvcv; break;
			case mcLdq: 
				{
					overloaded = true;

					ins.itype = ins_c::pcLdq; 

					ins.Op1.reg = reglist_c::encodeList(reglist_c::ltR4_R7, 0);
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
					opflags_s* op1 = (opflags_s*)&ins.Op1.opFlag;
					op1->_isRegistersListSet = true;

					ins.Op2.reg = op->_rrrr._rrrr;
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;
					opflags_s* op2 = (opflags_s*)&ins.Op2.opFlag;
					op2->_phraseType = opflags_s::ptExpression;
				}
				break;
			case mcStq: 
				{
					overloaded = true;

					ins.itype = ins_c::pcStq; 

					ins.Op1.reg = reglist_c::encodeList(reglist_c::ltR4_R7, 0);
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
					opflags_s* op1 = (opflags_s*)&ins.Op1.opFlag;
					op1->_isRegistersListSet = true;

					ins.Op2.reg = op->_rrrr._rrrr;
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;
					opflags_s* op2 = (opflags_s*)&ins.Op2.opFlag;
					op2->_phraseType = opflags_s::ptExpression;
				}
				break;
			case mcLdm: 
				{
					overloaded = true;

					ins.itype = ins_c::pcLdm; 

					ins.Op2.phrase = regs_c::rgR0; // Register R0
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;
					opflags_s* op2 = (opflags_s*)&ins.Op2.opFlag;
					op2->_phraseType = opflags_s::ptExpression;

					ins.Op1.reg = reglist_c::encodeList(reglist_c::ltRf_R15, op->_rrrr._rrrr);
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
					opflags_s* op1 = (opflags_s*)&ins.Op1.opFlag;
					op1->_isRegistersListSet = true;
				}
				break;
			case mcStm:	
				{
					overloaded = true;

					ins.itype = ins_c::pcStm; 

					ins.Op2.reg = regs_c::rgR0; // Register R0
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;
					opflags_s* op2 = (opflags_s*)&ins.Op2.opFlag;
					op2->_phraseType = opflags_s::ptExpression;

					ins.Op1.reg = reglist_c::encodeList(reglist_c::ltRf_R15, op->_rrrr._rrrr);
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
					opflags_s* op1 = (opflags_s*)&ins.Op1.opFlag;
					op1->_isRegistersListSet = true;
				}
				break;
			case mcDect: ins.itype = ins_c::pcDect; break;
			case mcDecf: ins.itype = ins_c::pcDecf; break;
			case mcInct: ins.itype = ins_c::pcInct; break;
			case mcIncf: ins.itype = ins_c::pcIncf; break;
			case mcJmp: ins.itype = ins_c::pcJmp; break;
			case mcJsr: ins.itype = ins_c::pcJsr; break;
			case mcFf1: ins.itype = ins_c::pcFf1; break;
			case mcBrev: ins.itype = ins_c::pcBrev; break;
			case mcXtrb0:
				{
					overloaded = true;

					ins.itype = ins_c::pcXtrb0;

					ins.Op2.reg = op->_rrrr._rrrr;
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;

					ins.Op1.reg = regs_c::rgR1;
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
				}
				break;
			case mcXtrb1:
				{
					overloaded = true;

					ins.itype = ins_c::pcXtrb1;

					ins.Op2.reg = op->_rrrr._rrrr;
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;

					ins.Op1.reg = regs_c::rgR1;
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
				}
				break;

			case mcXtrb2:
				{
					overloaded = true;

					ins.itype = ins_c::pcXtrb2;

					ins.Op2.reg = op->_rrrr._rrrr;
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;

					ins.Op1.reg = regs_c::rgR1;
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
				}
				break;

			case mcXtrb3:
				{
					overloaded = true;

					ins.itype = ins_c::pcXtrb3;

					ins.Op2.reg = op->_rrrr._rrrr;
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;

					ins.Op1.reg = regs_c::rgR1;
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
				}
				break;

			case mcZextb: ins.itype = ins_c::pcZextb; break;
			case mcSextb: ins.itype = ins_c::pcSextb; break;
			case mcZexth: ins.itype = ins_c::pcZexth; break;
			case mcSexth: ins.itype = ins_c::pcSexth; break;
			case mcDeclt: ins.itype = ins_c::pcDeclt; break;
			case mcTstnbz: ins.itype = ins_c::pcTstnbz; break;
			case mcDecgt: ins.itype = ins_c::pcDecgt; break;
			case mcDecne: ins.itype = ins_c::pcDecne; break;
			case mcClrt: ins.itype = ins_c::pcClrt; break;
			case mcClrf: ins.itype = ins_c::pcClrf; break;
			case mcAbs: ins.itype = ins_c::pcAbs; break;
			case mcNot: ins.itype = ins_c::pcNot; break;
			case mcBmaski32Set: ins.itype = ins_c::pcBmaski32Set; break;
			case mcDivu: 
				{
					overloaded = true;

					ins.itype = ins_c::pcDivu;

					ins.Op2.reg = regs_c::rgR1; // Register R1
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;

					ins.Op1.reg = op->_rrrr._rrrr;
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
				}
				break;
			case mcDivs: 
				{
					overloaded = true;

					ins.itype = ins_c::pcDivs; 
					
					ins.Op2.reg = regs_c::rgR1; // Register R1
					ins.Op2.type = o_reg;
					ins.Op2.dtyp = dt_dword;

					ins.Op1.reg = op->_rrrr._rrrr;
					ins.Op1.type = o_reg;
					ins.Op1.dtyp = dt_dword;
				}
				break;
			case mcBgeni: ins.itype = ins_c::pcBgeni; break;
			case mcXsr: ins.itype = ins_c::pcXsr; break;
			case mcAsrc: ins.itype = ins_c::pcAsrc; break;
			case mcLslc: ins.itype = ins_c::pcLslc; break;
			case mcLsrc: ins.itype = ins_c::pcLsrc; break;
			}

			if (!overloaded)
			{
				ins.Op1.reg = op->_rrrr._rrrr;
				ins.Op1.type = o_reg;
				ins.Op1.dtyp = dt_dword;
			}

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::groupSeventh(ushort code, insn_t& ins)
{
#pragma pack(push)
#pragma pack(1)
	struct opnd_s
	{
	public:
		union
		{
		public:
			struct  
			{
			public:
				ushort   _ii    : 2;
			} _ii;
		};
	};
#pragma pack(pop)

	opnd_s* op = (opnd_s*)&code;

	/* Determining whether the opcode is from the first instruction set group of 2 bit long operands
	mcTrap = 0x8,           //             b10     ii
	*/
	switch ((code >> 2) << 2) 
	{
	case mcTrap:
		{
			ins.itype = ins_c::pcTrap;

			ins.Op1.value = op->_ii._ii;
			ins.Op1.type = o_imm;
			ins.Op1.dtyp = dt_byte;

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::groupEighth(ushort code, insn_t& ins)
{
	/* Determining whether the opcode is from the first instruction set group with no operands
	mcBkpt = 0x0,
	mcSync = 0x1,
	mcRte = 0x2,
	mcRfi = 0x3,
	mcStop = 0x4,
	mcWait = 0x5,
	mcDoze = 0x6,
	*/
	switch (code) 
	{
	case mcSync:
	case mcRte:
	case mcRfi:
	case mcStop:
	case mcWait:
	case mcDoze:
	case mcBkpt:
		{
			switch (code) 
			{
			case mcBkpt: ins.itype = ins_c::pcBkpt; break;
			case mcSync: ins.itype = ins_c::pcSync; break;
			case mcRte: ins.itype = ins_c::pcRte; break;
			case mcRfi: ins.itype = ins_c::pcRfi; break;
			case mcStop: ins.itype = ins_c::pcStop; break;
			case mcWait: ins.itype = ins_c::pcWait; break;
			case mcDoze: ins.itype = ins_c::pcDoze; break;
			}

			return true;
		}
		break;
	}

	return false;
}
bool analyze_c::splitCommand(ushort code, insn_t& ins)
{
	// The order of commands checking is very important
	// as far as they are combined to be checked for equality 
	// from the highest opcodes to lowest
	if (analyze_c::groupFirst(code, ins) ||
		analyze_c::groupSecond(code, ins) ||
		analyze_c::groupThird(code, ins) ||
		analyze_c::groupFourth(code, ins) ||
		analyze_c::groupFifth(code, ins) ||
		analyze_c::groupSixth(code, ins) ||
		analyze_c::groupSeventh(code, ins) ||
		analyze_c::groupEighth(code, ins))
	{
		ins.size = 2;

		return true;
	}

	ins.itype = ins_c::pcFake;
	ins.size = 0;

	return false;
}
bool analyze_c::allowOperandType(op_t &x)
{
	switch (x.type)
	{
		case o_void:        // No Operand
		case o_reg:         // General Register (al,ax,es,ds...)
		case o_near:
		case o_far:
		return false;
	}
	return 1;
}
void analyze_c::useMacro(bool use)
{
	analyze_c::_useMacroCommands = use;
}
bool analyze_c::useMacro()
{
	return analyze_c::_useMacroCommands;
}
// The function exists in private in order to be accessed only during the analysis stage.
// It must take the next instruction after the specified one and
// check whether it fits the macro criteria.
// As far as it is analysis stage, it is impossible to take into account
// the code flow of the subsequent instructions or even to check whether
// the next instruction exists.
// So the function just get the next instruction without any checks
bool analyze_c::isNextInstructionFitMacro(const insn_t& ins, insn_t& next)
{
	next = ins;
	next.ea += next.size;
	// Check in order to avoid the situations when
	// there is any reference between two instructions
	// If it is true, the instructions cannot be united into a single MACRO command
	if (!::hasRef(::getFlags(next.ea)))
	{
		ushort code = ::get_word(next.ea);

		analyze_c::splitCommand(code, next);

		return ins.Op1.type == o_reg &&
			next.Op1.type == o_reg &&
			ins.Op1.reg == next.Op1.reg;
	}
	return false;
}
void analyze_c::createMacroImmediateValueSize4(insn_t& ins, ins_c::ins_e code, longlong value)
{
	ins.flags |= INSN_MACRO;

	ins.size = 4;

	ins.itype = code;

	ins.Op2.value = (uint)value;
	ins.Op2.type = o_imm;
	ins.Op2.dtyp = dt_dword;
}
void analyze_c::setXORedImmediateOperandFlag(op_t& op)
{
	analyze_c::opflags_s* flag = (analyze_c::opflags_s*)&op.opFlag;
	flag->_phraseType = opflags_s::ptXoredImmediate;
}
void analyze_c::createMacroImmediateValueSize2(insn_t& ins, ins_c::ins_e code, longlong value)
{
	ins.flags |= INSN_MACRO;

	ins.size = 2;

	ins.itype = code;

	ins.Op2.value = (uint)value;
	ins.Op2.type = o_imm;
	ins.Op2.dtyp = dt_dword;
}
bool analyze_c::doMacro(insn_t& ins)
{
//	return false;
	// We'll try to convert each "bits-oriented" operations into a macro command
	// in order to get it looked more convenient for reading
	// Something like BGENI r2, 1 => MOVI r2, 0x02
	// We could fall into the following cases:
	// 1. "Bits-oriented" instruction group. For an instance
	//         BGENI r0, 0x10
	//         BSETI r0, 0x5
	//     could be converted into one
	//         MOVI r0, 0x10020
	// 2. "Bits-oriented" single instruction. For an instance
	//         BGENI r0, 0x10
	//     could be converted into one
	//         MOVI r0, 0x10000
	switch (ins.itype)
	{
		// For compound register values starting with movi rX, ...
	case ins_c::pcMovi:
		{
			//         movi r6, 1
            // locxxx:  
			//         addi r6, 0x18
//			if (ins.ea == 0x10001BE8)
//			{
//				ea_t e = ins.ea;
//			}

			insn_t next;
			// Start checking if the instruction falls into the case of the instruction group
			// If we meet one, we'll convert it and exit with true
			// Otherwise with false
			if (analyze_c::isNextInstructionFitMacro(ins, next))
			{
				longlong value = 0;

				// All following group of instructions will become a single MOVI rx, 0x...
				switch (next.itype)
				{
					// MOVI rx, 0x...   ; BSETI rx, 0x...
				case ins_c::pcBseti:
					{
						value = longlong(ins.Op2.value) | (1 << next.Op2.value);
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// MOVI rx, 0x...   ; BCLRI rx, 0x...
				case ins_c::pcBclri:
					{
						value = longlong(ins.Op2.value) & (~(1 << next.Op2.value));
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// MOVI rx, 0x...   ; LSLI rx, 0x...
				case ins_c::pcRotli:
				case ins_c::pcLsli:
					{
						value = longlong(ins.Op2.value) << next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// MOVI rx, 0x...   ; LSRI rx, 0x...
				case ins_c::pcLsri:
				case ins_c::pcAsri:
					{
						value = longlong(ins.Op2.value) >> next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// MOVI rx, 0x...   ; ADDI rx, 0x...
				case ins_c::pcAddi:
					{
						value = longlong(ins.Op2.value) + next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// MOVI rx, 0x...   ; SUBI rx, 0x...
				case ins_c::pcSubi:
					{
						value = longlong(ins.Op2.value) - next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
				}
			}
		}
		break;
		// For compound register values starting with movi rX, ...
	case ins_c::pcBgeni:
	case ins_c::pcBgeniImm3:
	case ins_c::pcBgeniImm5:
		{
			// bseti ...
			// bgeni ...
//			if (ins.ea == 0x10010C5E)
//			{
//				ea_t e = ins.ea;
//			}

			insn_t next;
			// Start checking if the instruction falls into the case of the instruction group
			// If we meet one, we'll convert it and exit with true
			// Otherwise we'll convert it anyway but into a single instruction and exit with false
			if (analyze_c::isNextInstructionFitMacro(ins, next))
			{
				longlong value = 0;

				// All following groups of instructions becomes MOVI rx, 0x...
				switch (next.itype)
				{
					// BGENI rx, 0x... ; BSETI rx, 0x...
				case ins_c::pcBseti:
					{
						value = (longlong(1) << ins.Op2.value) | (1 << next.Op2.value);
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BGENI rx, 0x... ; BCLRI rx, 0x...
				case ins_c::pcBclri:
					{
						value = (longlong(1) << ins.Op2.value) & (~(1 << next.Op2.value));
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BGENI rx, 0x... ; ADDI rx, 0x...
				case ins_c::pcAddi:
					{
						value = (longlong(1) << ins.Op2.value) + next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BGENI rx, 0x... ; SUBI rx, 0x...
				case ins_c::pcSubi:
					{
						value = (longlong(1) << ins.Op2.value) - next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BGENI rx, 0x... ; ROTLI rx, 0x...
				case ins_c::pcLsli:
				case ins_c::pcRotli:
					{
						value = (longlong(1) << ins.Op2.value) << next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BGENI rx, 0x... ; LSRI rx, 0x...
				case ins_c::pcLsri:
				case ins_c::pcAsri:
					{
						value = (longlong(1) << ins.Op2.value) >> next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
				}
			}
			// Converting into a single convenient macro instruction
			analyze_c::createMacroImmediateValueSize2(ins, ins_c::pcMacroMov, longlong(1) << ins.Op2.value);

			return true;
		}
		break;
	case ins_c::pcBmaskiImm3:
	case ins_c::pcBmaskiImm5:
		{
			insn_t next;
			// Start checking if the instruction falls into the case of the instruction group
			// If we meet one, we'll convert it and exit with true
			// Otherwise we'll convert it anyway but into a single instruction and exit with false
			if (analyze_c::isNextInstructionFitMacro(ins, next))
			{
				longlong value = 0;

				switch (next.itype)
				{
					// BMASKI rx, 0x... ; BSETI rx, 0x...
				case ins_c::pcBseti:
					{
						value = ((longlong(1) << ins.Op2.value) - 1) | (1 << next.Op2.value);
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BMASKI rx, 0x... ; BCLRI rx, 0x...
				case ins_c::pcBclri:
					{
						value = ((longlong(1) << ins.Op2.value) - 1) & (~(1 << next.Op2.value));
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BMASKI rx, 0x... ; ADDI rx, 0x...
				case ins_c::pcAddi:
					{
						value = ((longlong(1) << ins.Op2.value) - 1) + next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BMASKI rx, 0x... ; SUBI rx, 0x...
				case ins_c::pcSubi:
					{
						value = ((longlong(1) << ins.Op2.value) - 1) - next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BMASKI rx, 0x... ; LSLI rx, 0x...
				case ins_c::pcLsli:
				case ins_c::pcRotli:
					{
						value = ((longlong(1) << ins.Op2.value) - 1) << next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
					// BMASKI rx, 0x... ; LSRI rx, 0x...
				case ins_c::pcLsri:
				case ins_c::pcAsri:
					{
						value = ((longlong(1) << ins.Op2.value) - 1) >> next.Op2.value;
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroMov, value);

						return true;
					}
					break;
				}
			}
			// Converting into a single convenient macro instruction
			analyze_c::createMacroImmediateValueSize2(ins, ins_c::pcMacroMov, (longlong(1) << ins.Op2.value) - 1);

			return true;
		}
		break;
	case ins_c::pcBseti:
		{
			insn_t next;
			// Start checking if the instruction falls into the case of the instruction group
			// If we meet one, we'll convert it and exit with true
			// Otherwise we'll convert it anyway but into a single instruction and exit with false
			if (analyze_c::isNextInstructionFitMacro(ins, next))
			{
				longlong value = 0;

				switch (next.itype)
				{
					// BSETI rx, 0x... ; BSETI rx, 0x...
				case ins_c::pcBseti:
					{
						value = (longlong(1) << ins.Op2.value) | (1 << next.Op2.value);
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroOr, value);

						return true;
					}
					break;
				}
			}
			// Converting into a single convenient macro instruction
			analyze_c::createMacroImmediateValueSize2(ins, ins_c::pcMacroOr, longlong(1) << ins.Op2.value);

			return true;
		}
		break;
	case ins_c::pcBclri:
		{
			insn_t next;
			// Start checking if the instruction falls into the case of the instruction group
			// If we meet one, we'll convert it and exit with true
			// Otherwise we'll convert it anyway but into a single instruction and exit with false
			if (analyze_c::isNextInstructionFitMacro(ins, next))
			{
				longlong value = 0;

				switch (next.itype)
				{
					// BSETI rx, 0x... ; BSETI rx, 0x...
				case ins_c::pcBclri:
					{
						value = (longlong(1) << ins.Op2.value) | (1 << next.Op2.value);
						//analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroAnd, ~value);
						analyze_c::createMacroImmediateValueSize4(ins, ins_c::pcMacroAnd, value);
						analyze_c::setXORedImmediateOperandFlag(ins.Op2);

						return true;
					}
					break;
				}
			}
			// Converting into a single convenient macro instruction
			//analyze_c::createMacroImmediateValueSize2(ins, ins_c::pcMacroAnd, ~(longlong(1) << ins.Op2.value));
			analyze_c::createMacroImmediateValueSize2(ins, ins_c::pcMacroAnd, longlong(1) << ins.Op2.value);
			analyze_c::setXORedImmediateOperandFlag(ins.Op2);

			return true;
		}
		break;
	}
	return false;
}
int analyze_c::handlerMain()
{
	// 0x1019FC
	if (cmd.ip & 1)
	{
		return 0;           // alignment error
	}

	ushort code = ::ua_next_word();

	analyze_c::splitCommand(code, cmd);

	if (analyze_c::_useMacroCommands)
	{
		analyze_c::doMacro(cmd);
	}
	// In order to reanalyze instruction in case of
	// switching on/off using macro commands.
	// If the instruction was already defined at least once,
	// the information about its item size would be stored in the database
	// and at the next time, when the analysis would be required, 
	// IDA would organize addresses beating in accordance with theirs prev. definitions and sizes
	if (::isHead(::getFlags(cmd.ea)) && // if not the first analyze
		cmd.size != ::get_item_size(cmd.ea)) // and the size of the instruction just changed
	{
		// reanalyze
		ea_t ea = cmd.ea;
		::do_unknown_range(cmd.ea, cmd.size, false); // undefine preceding instructions
		cmd.ea = ea;
		::autoMark(cmd.ea, AU_CODE);
	}
	return cmd.size;
}