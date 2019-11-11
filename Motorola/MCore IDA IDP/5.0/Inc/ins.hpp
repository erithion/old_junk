#ifndef _INS_HPP
#define _INS_HPP

// IDA types
//     instruc_t - See idp.hpp of IDA SDK
struct instruc_t;

namespace mcore_n
{
	/************************************************************************/
	/* M-Core pseudo-code list interface                                    */
	/************************************************************************/
	class ins_c
	{
	public:
		/************************************************************************/
		/* Pseudo Codes for IDA. Each position coincides with Pseudo Codes array*/
		/* IDA will use the enum range, first and last + 1 to get the real      */
		/* pseudo code string from _constPseudoCodes list for output            */
		/************************************************************************/
		typedef enum ins_e
		{
			pcStart = 0,
			// Warning: Instructions with 0 opcode 
			// won't work with ::InstrIsSet,
			// because ::InstrIsSet works only with unsigned numerics
			// and contains a check for min-opcode, specified at LPH
			// to be greater, not greater equal
			pcBkpt = 0,
			pcSync,
			pcRte,
			pcRfi,
			pcStop,
			pcWait,
			pcDoze,
			pcTrap,
			pcMvc,
			pcMvcv,
			pcLdq,
			pcStq,
			pcLdm,
			pcStm,
			pcDect,
			pcDecf,
			pcInct,
			pcIncf,
			pcJmp,
			pcJsr,
			pcFf1,
			pcBrev,
			pcXtrb3,
			pcXtrb2,
			pcXtrb1,
			pcXtrb0,
			pcZextb,
			pcSextb,
			pcZexth,
			pcSexth,
			pcDeclt,
			pcTstnbz,
			pcDecgt,
			pcDecne,
			pcClrt,
			pcClrf,
			pcAbs,
			pcNot,
			pcMovt,
			pcMult,
			pcLoopt,
			pcSubu,
			pcAddc,
			pcSubc,
			pcMovf,
			pcLsr,
			pcCmphs,
			pcCmplt,
			pcTst,
			pcCmpne,
			pcMfcr,
			pcMov,
			pcBgenr,
			pcRsub,
			pcIxw,
			pcAnd,
			pcXor,
			pcMtcr,
			pcAsr,
			pcLsl,
			pcAddu,
			pcIxh,
			pcOr,
			pcAndn,
			pcAddi,
			pcCmplti,
			pcSubi,
			pcRsubi,
			pcCmpnei,
			pcBmaski32Set,
			pcDivu,
			pcBmaskiImm5,
			pcBmaskiImm3,
			pcAndi,
			pcBclri,
			pcDivs,
			pcBgeniImm5,
			pcBgeniImm3,
			pcBgeni,
			pcBseti,
			pcBtsti,
			pcXsr,
			pcRotli,
			pcAsrc,
			pcAsri,
			pcLslc,
			pcLsli,
			pcLsrc,
			pcLsri,
			pcH_exec,
			pcH_ret,
			pcH_call,
			pcH_ld,
			pcH_st,
			pcH_ldh,
			pcH_sth,
			pcMovi,
			pcLrw,
			pcJmpi,
			pcJsri,
			pcLd,
			pcSt,
			pcLdb,
			pcStb,
			pcLdh,
			pcSth,
			pcBt,
			pcBf,
			pcBr,
			pcBsr,

			/************************************************************************/
			/* Macro instructions                                                   */
			/************************************************************************/
			pcMacroMov,
			pcMacroOr,
			pcMacroAnd,
			
			pcFake
		};
	public:
		/************************************************************************/
		/* Returns instruction list                                             */
		/************************************************************************/
		static instruc_t* getIns();
		/************************************************************************/
		/* Returns a single instruction                                         */
		/************************************************************************/
		static instruc_t getIns(ins_e i) _throw(out_of_range_x);
	protected:
	private:
		// Pseudo Codes array for IDA to find appropriate instruction string by instruction id
		static instruc_t _constPseudoCodes[];
	private:
		ins_c();
		ins_c(const ins_c&);
		ins_c& operator =(const ins_c&);
	};
}
#endif // _INS_HPP