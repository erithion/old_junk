#ifndef _MAIN_HPP
#define _MAIN_HPP

// IDA types
struct processor_t;
struct asm_t;
class netnode;

namespace mcore_n
{
	/************************************************************************/
	/* IDA requires the only one symbol to be exported by any IDP. It is LPH*/
	/* LPH is a processor extended descriptor.                              */
	/* The class initializes LPH for IDA                                    */
	/************************************************************************/
	struct idpentry_c
	{
	public:
		/************************************************************************/
		/* Constructor fills LPH, base processor descriptor, with a valid data  */
		/************************************************************************/
		idpentry_c();

	protected:
	private:
		// User defined M-Core Platform Id for LPH, must be above 0x8000
		static const int   _constPlatformId = 0x9000;

		// Short processor names declaration for LPH
		static const char* _constShortProcessorName[];

		// Long processor names declaration for LPH
		static const char* _constLongProcessorName[];

	private:
		// IDP processor descriptor may have minimum 1 implemented assembler
		// The target assemblers list contains these assemblers. 
		// The first declared assembler in this list will be used by default.
		static asm_t*      _assemblers[];

		// netnode to contain all processor specific info
		static netnode     _helper;

		// The only exported symbol for IDA - processor descriptor
		static processor_t LPH;

	private:
		/************************************************************************/
		/* Kernel notification callback                                         */
		/************************************************************************/
		static int kernelCallback(processor_t::idp_notify msgid, ...);
		static const char* setIDPOptions(const char* keyword, int type, const void* value);
		static bool idaapi isStackArgLoad(int* src, int* dst);
		static int useRegArgType(ea_t ea, const type_t* const* argTypes, const char* const* argNames, const ulong* regsData, int regsSize);
		static bool idaapi setOperandType(op_t &x, const type_t* type, const char* name);
		static int idpentry_c::calcArgLocs(const type_t*& type, ulong* argData, int argSize);
	};

	// Static object to initialize a processor descriptor
	static idpentry_c initializeProcessor;
}
#endif // _MAIN_HPP