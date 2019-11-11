#ifndef _REG_HPP
#define _REG_HPP

namespace mcore_n
{
	/************************************************************************/
	/* Processor registers, i.e. General and Control Registers              */
	/************************************************************************/
	class regs_c
	{
	public:
		/************************************************************************/
		/* General registers. Numeric ids of the each general registers         */
		/************************************************************************/
		typedef enum general_e
		{
			rgSp,
			rgR0 = rgSp, 
			rgR1,
			rgR2,
			rgR3,
			rgR4,
			rgR5,
			rgR6,
			rgR7,
			rgR8,
			rgR9,
			rgR10,
			rgR11,
			rgR12,
			rgR13,
			rgR14,
			rgLr,
			rgR15 = rgLr,
			rgPc,
			rgVcs, rgVds,            // virtual registers for code and data segments

			rgFake
		};

		/************************************************************************/
		/* Control registers. Numeric ids of the each control registers         */
		/************************************************************************/
		typedef enum control_e
		{
			rcPsr = 0,
			rcVbr,
			rcEpsr,
			rcFpsr,
			rcEpc,
			rcFpc,
			rcSs0,
			rcSs1,
			rcSs2,
			rcSs3,
			rcSs4,
			rcGcr,
			rcGsr,
			rcCr13,
			rcCr14,
			rcCr15,
			rcCr16,
			rcCr17,
			rcCr18,
			rcCr19,
			rcCr20,
			rcCr21,
			rcCr22,
			rcCr23,
			rcCr24,
			rcCr25,
			rcCr26,
			rcCr27,
			rcCr28,
			rcCr29,
			rcCr30,
			rcCr31,

			rcFake
		};
	public:
		/************************************************************************/
		/* Returns the array of the general register name strings               */
		/* Method was created mostly to fill the processor_t::regNames          */
		/************************************************************************/
		static char** getRegGen();

		/************************************************************************/
		/* Returns the total number of general registers                        */
		/* Method was created mostly to fill processor_t::regNums               */
		/************************************************************************/
		static size_t getRegGenSize();

		/************************************************************************/
		/* Returns the general register name string by its numeric id           */
		/************************************************************************/
		static string getRegGenItem(general_e id) _throw(out_of_range_x);

		/************************************************************************/
		/* Returns the control register name string by id                       */
		/************************************************************************/
		static string getRegCtlItem(control_e id) _throw(out_of_range_x);
	protected:
	private:
		// M-Core general registers (User mode)
		static char* _constRegGen[];

		// M-Core control registers (Supervisor mode)
		static char* _constRegCtl[];
	private:
		regs_c();
	};
	/************************************************************************/
	/* M-Core command set uses various register's list in such a commands   */
	/* as LDM, LDQ etc.                                                     */
	/* This is a single interface to get any list of registers for command  */
	/************************************************************************/
	class reglist_c
	{
	public:
		/************************************************************************/
		/* The types of the register lists                                      */
		/************************************************************************/
		typedef enum listtype_e
		{
			ltR4_R10 = 0,   // Applicable for commands, where register list is set only
			                // within a range of R4 (value = 0) - R10 (value = 7)
			ltRf_R15 = 1,   // Applicable for commands, where register list is set only
			                // by the first register number, such as R[n] - R15
			ltR4_R7 = 2     // Applicable for commands, where register list is static R4 - R7
		};
	public:
		/************************************************************************/
		/* Encodes the type of the list and its value into a single ushort value*/
		/************************************************************************/
		static ushort encodeList(listtype_e type, uchar id);

		/************************************************************************/
		/* Decodes the type of the list and its value from a single ushort value*/
		/************************************************************************/
		static void decodeList(ushort code, listtype_e& type, uchar& id);
	public:
		/************************************************************************/
		/* code - encoded single short value of the list type and value         */
		/* see reglist_c::encodeList method                                     */
		/************************************************************************/
		reglist_c(ushort code) _throw(out_of_range_x);

		/************************************************************************/
		/* Gets the string representation of the register list                  */
		/* For example, "r7 - r15"                                              */
		/************************************************************************/
		string getList() const;
	protected:
	private:
		// Register list type of the constructed object
		listtype_e _type;

		// Register list id
		uchar      _id;
	};
}
#endif // _REG_HPP