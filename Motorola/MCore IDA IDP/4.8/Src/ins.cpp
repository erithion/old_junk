#include "idaidp.hpp"

#include "types.hpp"
#include "ins.hpp"

using namespace mcore_n;

// M-Core instruction set
instruc_t ins_c::_constPseudoCodes[] = 
{
	{ "bkpt",       CF_STOP                                           },   // Breakpoint instruction exception
	{ "sync",       0                                                 },   // Synchronize
	{ "rte",        0                                                 },   // pc <- epc, psr <- epsr
	{ "rfi",        0                                                 },   // pc <- fpc, psr <- fpsr
	{ "stop",       0                                                 },   // Stop untill any exception occurs
	{ "wait",       0                                                 },   // Stop untill any exception occurs
	{ "doze",       0                                                 },   // Enter power low doze mode
	{ "trap",       0                                                 },   // Causes a trap exception to occur (trap #number)
	{ "mvc",        CF_USE1 | CF_CHG1                                 },   // rx <- c bit (mvc rx)
	{ "mvcv",       CF_USE1 | CF_CHG1                                 },   // rx <- !c bit (mvcv rx)
	{ "ldq",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // Destination registers <- memory (ldq r4-r7, rx)
	{ "stq",        CF_USE1 |                     CF_USE2 | CF_CHG2   },   // memory <- Destination registers (stq r4-r7, rx)
	{ "ldm",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // Destination registers <- memory (ldm rf-r15, (r0))
	{ "stm",        CF_USE1 |                     CF_USE2 | CF_CHG2   },   // memory <- Destination registers (stm rf-r15, (r0))
	{ "dect",       CF_USE1 | CF_CHG1                                 },   // c == 1 ? rx - 1 : rx (dect rx)
	{ "decf",       CF_USE1 | CF_CHG1                                 },   // c == 0 ? rx - 1 : rx (decf rx)
	{ "inct",       CF_USE1 | CF_CHG1                                 },   // c == 1 ? rx + 1 : rx (inct rx)
	{ "incf",       CF_USE1 | CF_CHG1                                 },   // c == 0 ? rx + 1 : rx (incf rx)
	{ "jmp",        CF_USE1 |           CF_JUMP                       },   // pc <- rx (jmp rx)
	{ "jsr",        CF_USE1 |           CF_CALL                       },   // r15 <- pc + 2, pc <- (rx) (jsr rx)
	{ "ff1",        CF_USE1 | CF_CHG1                                 },   // rx <- ff1(rx) (ff1 rx)
	{ "brev",       CF_CHG1 | CF_USE1                                 },   // Reverse bits of rx (brev rx)
	{ "xtrb3",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // r1 <- byte 3 of rx zero extended to 32 (xtrb3 r1, rx)
	{ "xtrb2",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // r1 <- byte 2 of rx zero extended to 32 (xtrb2 r1, rx)
	{ "xtrb1",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // r1 <- byte 1 of rx zero extended to 32 (xtrb1 r1, rx)
	{ "xtrb0",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // r1 <- byte 0 of rx zero extended to 32 (xtrb0 r1, rx)
	{ "zextb",      CF_USE1 | CF_CHG1                                 },   // rx <- low-order byte of rx zero extended to 32 (zextb rx)
	{ "sextb",      CF_USE1 | CF_CHG1                                 },   // rx <- rx[7:0] sign extended to 32 (sextb rx)
	{ "zexth",      CF_USE1 | CF_CHG1                                 },   // rx <- low-order half of rx zero extended to 32 (zexth rx)
	{ "sexth",      CF_USE1 | CF_CHG1                                 },   // rx <- rx[15:0] sign extended to 32 (sexth rx)
	{ "declt",      CF_USE1 | CF_CHG1                                 },   // rx = rx - 1, rx < 0 ? c = 1 : c = 0 (declt rx)
	{ "tstnbz",     CF_USE1                                           },   // if no byte of rx is 0 ? c = 1 : c = 0 (tstnbz rx)
	{ "decgt",      CF_USE1 | CF_CHG1                                 },   // rx = rx - 1, rx > 0 ? c = 1 : c = 0 (decgt rx)
	{ "decne",      CF_USE1 | CF_CHG1                                 },   // rx = rx - 1, rx != 0 ? c = 1 : c = 0 (decne rx)
	{ "clrt",       CF_USE1 | CF_CHG1                                 },   // if c == 1 rx <- 0 (clrf rx)
	{ "clrf",       CF_USE1 | CF_CHG1                                 },   // if c == 0 rx <- 0 (clrf rx)
	{ "abs",        CF_USE1 | CF_CHG1                                 },   // rx <- |rx| (abs rx)
	{ "not",        CF_CHG1 | CF_USE1                                 },   // rx <- !rx (not rx)
	{ "movt",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // if (c == 1) rx <- ry (movt rx, ry)
	{ "mult",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx * ry (mult rx, ry)
	{ "loopt",      CF_USE1 | CF_CHG1 | CF_JUMP | CF_USE2             },   // if (c == 1) {pc <- pc + 2 + (one's extended 4-bit displacement << 1); ry <- ry - 1; Update c bit;} else {ry <- ry - 1; Update c bit;} (loopt ry, label)
	{ "subu",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx - ry (subu rx, ry)
	{ "addc",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx + ry + c, c - carry out (addc rx, ry)
	{ "subc",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx - ry - !c , c <- carryout (subc rx, ry)
	{ "movf",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // if (c == 0) rx <- ry (movf rx, ry)
	{ "lsr",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- lsr(rx) by ry[5:0] bits (lsr rx, ry)
	{ "cmphs",      CF_USE1 |                     CF_USE2             },   // rx >= ry ? c = 1 : c = 0 (cmphs rx, ry)
	{ "cmplt",      CF_USE1 |                     CF_USE2             },   // rx < ry ? c = 1 : c = 0 (cmplt rx, ry)
	{ "tst",        CF_USE1 |                     CF_USE2             },   // rx & ry != 0 ? c = 1 : c = 0 (tst rx, ry)
	{ "cmpne",      CF_USE1 |                     CF_USE2             },   // rx != ry ? c = 1 : c = 0 (cmpne rx, ry)
	{ "mfcr",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- Control Register(cry) (mfcr rx, cry)
	{ "mov",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- ry (mov rx, ry)
	{ "bgenr",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // (bgenr rx, ry)
	{ "rsub",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- ry - rx (rsub rx, ry)
	{ "ixw",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx + (ry << 2) (ixw rx, ry)
	{ "and",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx & ry (and rx, ry)
	{ "xor",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx xor ry (xor rx, ry)
	{ "mtcr",       CF_USE1 |                     CF_USE2 | CF_CHG2   },   // cry <- rx (mtcr rx, cry)
	{ "asr",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- asr(rx) by ry[5:0] bits (asr rx, ry)
	{ "lsl",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- lsl(rx) by ry[5:0] bits (lsl rx, ry)
	{ "addu",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx + ry (addi rx, ry)
	{ "ixh",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx + (ry << 1) (ixh rx, ry)
	{ "or",         CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx | ry (or rx, ry)
	{ "andn",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx && !ry (andn rx, ry)
	{ "addi",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx + unsigned OIMM5 (addi rx, oimm5)
	{ "cmplti",     CF_USE1 |                     CF_USE2             },   // rx < IMM5 ? c = 1 : c = 0 (cmplti rx, imm5)
	{ "subi",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx - unsigned OIMM5 (subi rx, oimm5)
	{ "rsubi",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- unsigned IMM5 - rx (rsubi rx, imm5)
	{ "cmpnei",     CF_USE1 |                     CF_USE2             },   // rx != IMM5 ? c = 1 : c = 0 (cmpnei rx, imm5)
	{ "bmaski32",   CF_USE1 | CF_CHG1                                 },   // rx <- (2) ^ (32) - 1 (bmaski rx)
	{ "divu",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx / r1 (divu rx, r1)
	{ "bmaski",     CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- (2) ^ (IMM5) - 1 (bmaski rx, imm5)
	{ "bmaski",     CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- (2) ^ (IMM5) - 1 (bmaski rx, imm5)
	{ "andi",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx && IMM5 (andi rx, imm5)
	{ "bclri",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // Clear bit [IMM5] of rx (bclri rx, imm5)
	{ "divs",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx / r1 (divs rx, r1)
	{ "bgeni",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- (2) ^ IMM% (bgeni rx, imm5)
	{ "bgeni",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- (2) ^ IMM% (bgeni rx, imm5)
	{ "bgeni",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- (2) ^ IMM% (bgeni rx, imm5)
	{ "bseti",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // Set bit[IMM5] of rx (bseti rx, imm5)
	{ "btsti",      CF_USE1 |                     CF_USE2             },   // c <- rx[IMM5] (btsti rx, imm5)
	{ "xsr",        CF_USE1 | CF_CHG1                                 },   // rx <- rx >> 1 (xsr rx)
	{ "rotli",      CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rotl(rx) by IMM5 bits (1..31) (rotli rx, imm5)
	{ "asrc",       CF_USE1 | CF_CHG1                                 },   // c <- rx[0], rx <- asr(rx) by 1 (asrc rx)
	{ "asri",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- asr(rx) by IMM5 bits (asri rx, imm5)
	{ "lslc",       CF_USE1 | CF_CHG1                                 },   // c <- rx[31], rx <- rx << 1 (lslc rx)
	{ "lsli",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx << IMM5 bits (1..31) (lsli rx, imm5)
	{ "lsrc",       CF_USE1 | CF_CHG1                                 },   // c <- rx[0], rx <- rx >> 1 (lsrc rx)
	{ "lsri",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- rx >> IMM5 bits (1..31) (lsri rx, imm5)
	{ "h_exec",     0                                                 },
	{ "h_ret",      0                                                 },
	{ "h_call",     0                                                 },
	{ "h_ld",       0                                                 },
	{ "h_st",       0                                                 },
	{ "h_ldh",      0                                                 },
	{ "h_sth",      0                                                 },
	{ "movi",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- IMM5 (mov rx, imm5)
	{ "lrw",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // rz <- MEM(pc + 2 + unsigned_imm8 << 2) & 0xfffffffc (lrw rz, [label])
	{ "jmpi",       CF_USE1 |           CF_JUMP                       },   // pc <- MEM(pc + 2 + unsigned_imm8 << 2) & 0xfffffffc (jmpi [label])
	{ "jsri",       CF_USE1 |           CF_CALL                       },   // r15 <- pc + 2, pc <- MEM(pc + 2 + unsigned_imm8 << 2) & 0xfffffffc (jsri [label])
	{ "ld.w",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rz <- MEM(rx + unsigned_imm4 << {0, 1, 2}) ({ld/ld.w} rz, (rx, disp))
	{ "st.w",       CF_USE1 |                     CF_USE2 | CF_CHG2   },   // MEM(rx + unsigned_imm4 << {0, 1, 2}) <- rz ({st/st.w} rz, (rx, disp))
	{ "ld.b",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rz <- MEM(rx + unsigned_imm4 << {0, 1, 2})[7:0] ({ldb/ld.b} rz, (rx, disp))
	{ "st.b",       CF_USE1 |                     CF_USE2 | CF_CHG2   },   // MEM(rx + unsigned_imm4 << {0, 1, 2}) <- rz ({stb/st.b} rz, (rx, disp))
	{ "ld.h",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rz <- MEM(rx + unsigned_imm4 << {0, 1, 2}) ({ldh/ld.h} rz, (rx, disp))
	{ "st.h",       CF_USE1 |                     CF_USE2 | CF_CHG2   },   // MEM(rx + unsigned_imm4 << {0, 1, 2}) <- rz ({sth/st.h} rz, (rx, disp))
	{ "jbt",        CF_USE1 |           CF_JUMP                       },   // c == 1 ? pc = pc + 2 + (signed extended 11-bit displacement << 1) : pc = pc + 2 (bt label)
	{ "jbf",        CF_USE1 |           CF_JUMP                       },   // c == 0 ? pc = pc + 2 + (signed extended 11-bit displacement << 1) : pc = pc + 2 (bf label)
	{ "jbr",        CF_USE1 |           CF_JUMP                       },   // pc = pc + 2 + (signed extended 11-bit displacement << 1) (br label)
	{ "jbsr",       CF_USE1 |           CF_CALL                       },   // r15 <- pc + 2, pc = pc + 2 + (signed extended 11-bit displacement << 1) (bsr label)

	/************************************************************************/
	/* Macro instructions                                                   */
	/************************************************************************/
	{ "MOVI",       CF_USE1 | CF_CHG1 |           CF_USE2             },   // rx <- IMM (mov rx, imm)
	{ "ORI",        CF_USE1 | CF_CHG1 |           CF_USE2             },   // 
	{ "ANDI",       CF_USE1 | CF_CHG1 |           CF_USE2             }   // 
};
instruc_t* ins_c::getIns()
{
	return ins_c::_constPseudoCodes;
}
instruc_t ins_c::getIns(ins_e i)
{
	if (i >= ins_c::pcStart &&
		i < ins_c::pcFake)
	{
		return ins_c::_constPseudoCodes[i];
	}
	throw out_of_range_x();
}