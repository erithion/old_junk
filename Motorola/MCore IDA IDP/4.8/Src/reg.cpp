#include "idaidp.hpp"

#include "allocator.hpp"
#include "types.hpp"
#include "reg.hpp"

using namespace mcore_n;

/************************************************************************/
/* regs_c                                                               */
/************************************************************************/
char* regs_c::_constRegGen[] =
{
	// User mode registers set
	"sp",   // Stack Pointer
	"r1",   // Scratch
	"r2",   // 1st Argument
	"r3",   // 2nd Argument
	"r4",   // 3rd Argument
	"r5",   // 4th Argument
	"r6",   // 5th Argument
	"r7",   // 6th Argument
	"r8",   // Local
	"r9",   // Local
	"r10",  // Local
	"r11",  // Local
	"r12",  // Local
	"r13",  // Local
	"r14",  // Local
	"r15",  // Link/Scratch
	"pc",   // Program Counter
	"CS", "DS",           // cs and ds registers are required by the IDA kernel
};
char* regs_c::_constRegCtl[] = 
{
/* Processor Status Register
bit  0   1    2   3   4    5   6    7    8    9    10  11   12   13   14 15  16 - 22 23   24   25   26   27  28 29  30  31
    -----------------------------------------------------------------------------------------------------------------------
   | C | AF | 0 | 0 | FE | 0 | IE | IC | EE | MM | SC | 0 | TC | TP1 |  TM  |  VEC  | 0 | U0 | U1 | U2 | U3 |  SP  | 0 | S | 
	-----------------------------------------------------------------------------------------------------------------------
*/
	"psr",      // cr0 - Processor Status Register
	"vbr",      // cr1 - Vector Base Register
	"epsr",     // cr2 - Exception Processor Status Register (shadowing)
	"fpsr",     // cr3 - Fast interrupt Processor Status Register (shadowing)
	"epc",      // cr4 - Exception Program Counter (shadowing)
	"fpc",      // cr5 - Fast interrupt Program Counter (shadowing)
	"ss0",      // cr6 - Supervisor Storage Register
	"ss1",      // cr7 - Supervisor Storage Register
	"ss2",      // cr8 - Supervisor Storage Register
	"ss3",      // cr9 - Supervisor Storage Register
	"ss4",      // cr10 - Supervisor Storage Register
	"gcr",      // cr11 - Global Control Register
	"gsr",      // cr12 - Global Status Register
	"cr13",
	"cr14",
	"cr15",
	"cr16",
	"cr17",
	"cr18",
	"cr19",
	"cr20",
	"cr21",
	"cr22",
	"cr23",
	"cr24",
	"cr25",
	"cr26",
	"cr27",
	"cr28",
	"cr29",
	"cr30",
	"cr31"
};
char** regs_c::getRegGen()
{
	return regs_c::_constRegGen;
}
mcore_n::string regs_c::getRegGenItem(general_e id)
{
	if (id >= rgR0 && 
		id <= rgPc)
	{
		return mcore_n::string(regs_c::_constRegGen[id]);
	}
	throw out_of_range_x();
}
size_t regs_c::getRegGenSize()
{
	return sizeof(regs_c::_constRegGen) / sizeof(char*);
}
mcore_n::string regs_c::getRegCtlItem(control_e id)
{
	if (id >= rcPsr && 
		id <= rcCr31)
	{
		return mcore_n::string(regs_c::_constRegCtl[id]);
	}
	throw out_of_range_x();
}
/************************************************************************/
/* reglist_c                                                            */
/************************************************************************/
void reglist_c::decodeList(ushort code, listtype_e& type, uchar& id)
{
	type = (listtype_e)(code >> 8);
	id = code & 0xFF;
}
ushort reglist_c::encodeList(listtype_e type, uchar id)
{
	return (type << 8) | id;
}
reglist_c::reglist_c(ushort code)
{
	reglist_c::decodeList(code, this->_type, this->_id);

	switch (this->_type)
	{
	case ltR4_R10:
		{
			/*
			typedef enum rtlR4_R10_e
			{
			rlR4 = 1,          // Registers list includes only R4
			rlR4_R5,           // Registers list includes only R4, R5
			rlR4_R6,           // Registers list includes only R4 - R6
			rlR4_R7,           // Registers list includes only R4 - R7
			rlR4_R8,           // Registers list includes only R4 - R8
			rlR4_R9,           // Registers list includes only R4 - R9
			rlR4_R10           // Registers list includes only R4 - R10
			};
			*/
			if (this->_id >= 1 &&
				this->_id < 8)
			{
				return;
			}
		}
		break;
	case ltRf_R15:
		{
			/*
			typedef enum rtlRf_R15_e
			{
			rlR1_R15 = 1,
			rlR2_R15,
			rlR3_R15,
			rlR4_R15,
			rlR5_R15,
			rlR6_R15,
			rlR7_R15,
			rlR8_R15,
			rlR9_R15,
			rlR10_R15,
			rlR11_R15,
			rlR12_R15,
			rlR13_R15,
			rlR14_R15
			};
			*/
			if (this->_id >= 1 &&
				this->_id < 15)
			{
				return;
			}
		}
	case ltR4_R7:
		{
			return;
		}
		break;
	}

	throw out_of_range_x();
}
mcore_n::string reglist_c::getList() const
{
	switch (this->_type)
	{
	case ltR4_R10:
		{
			/*
			typedef enum rtlR4_R10_e
			{
			rlR4 = 1,          // Registers list includes only R4
			rlR4_R5,           // Registers list includes only R4, R5
			rlR4_R6,           // Registers list includes only R4 - R6
			rlR4_R7,           // Registers list includes only R4 - R7
			rlR4_R8,           // Registers list includes only R4 - R8
			rlR4_R9,           // Registers list includes only R4 - R9
			rlR4_R10           // Registers list includes only R4 - R10
			};
			*/
			int                  id = this->_id + 3;
			char                 buf[15];

			if (id > 4)
			{
				::qsnprintf(buf, sizeof(buf), "r4 - r%d", id);
			}
			else
			{
				::qsnprintf(buf, sizeof(buf), "r4");
			}

			return mcore_n::string(buf);
		}
		break;
	case ltRf_R15:
		{
			/*
			typedef enum rtlRf_R15_e
			{
			rlR1_R15 = 1,
			rlR2_R15,
			rlR3_R15,
			rlR4_R15,
			rlR5_R15,
			rlR6_R15,
			rlR7_R15,
			rlR8_R15,
			rlR9_R15,
			rlR10_R15,
			rlR11_R15,
			rlR12_R15,
			rlR13_R15,
			rlR14_R15
			};
			*/
			int                  id = this->_id;
			char                 buf[15];

			::qsnprintf(buf, sizeof(buf), "r%d - r15", id);

			return mcore_n::string(buf);
		}
	case ltR4_R7:
		{
			return mcore_n::string("r4 - r7");
		}
		break;
	}

	return mcore_n::string("");
}