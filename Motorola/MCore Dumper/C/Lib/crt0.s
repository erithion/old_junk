
//   .export intReset
//   .export intStub
//   .export intAutovectorHandler

   .extern ctlReboot

   .extern vAutovectorNonNestedInterrupHandlers
   .extern hrdInit
   .extern main
   
//   .section        .startup,,c   // Locate the code in the ".text2" section.

   .align 2


// Interrupt Pending List register 1
INT_NIPND1 EQU 0x200018
// Interrupt Pending List register 2
INT_NIPND2 EQU 0x20001C

// Reset exception handler for the exception vector
intReset:
    lrw r1, 0x106780
    mov sp, r1

    lrw r1, 0xA0000150
    mtcr  r1, psr

    // Added
    lrw     r2, 0x220000
    ld.w    r4, (r2, 4)
    lrw     r3, 2
    or      r3, r4
    st.w    r3, (r2, 4)
    lrw     r4, 0xFFFFFFFE
    and     r3, r4
    st.w    r3, (r2, 4)
    //    
    lrw     r2, vExceptionVector
    mtcr    r2, vbr

    // Added
    lrw     r2, 0x220000
    lrw     r3, 0x10001A1
    st.w    r3, (r2)
    lrw     r3, 0x1003121
    st.w    r3, (r2, 8)
    lrw     r3, 0x1000C01
    st.w    r3, (r2, 0x14)

    //
    mfcr  r1, psr
    bseti r1, 1
    mtcr  r1, psr
    // Added
    lrw r1, 0x106780
    mov sp, r1
    //
    mfcr  r1, psr
    bclri r1, 1
    mtcr  r1, psr
    // Added
    lrw     r1, 0x2000000
    lrw     r2, 0x200000
    ld.w    r3, (r2, 0x10)
    or      r3, r1
    st.w    r3, (r2, 0x10)
    lrw     r1, 0x10008000
    lrw     r2, 0x200000
    ld.w    r3, (r2, 0x14)
    or      r3, r1
    st.w    r3, (r2, 0x14)
    //
    jsri  hrdInit

    jmpi  main


// INT exception handler, Auto-Vectored, Non-Nested for exception vector
intAutovectorHandler:
	lrw     r3, INT_NIPND1
	ld.w    r1, (r3)
	ff1     r1
	andi    r1, 0x1F
	cmpnei  r1, 0
	movi    r4, 0x20
	jbt     _getVectoIndex
	lrw     r3, INT_NIPND2
	ld.w    r1, (r3)
	ff1     r1
	movi    r4, 0
_getVectoIndex:
	rsubi   r1, 0x1F
	addu    r1, r4
	lrw     r2, vAutovectorNonNestedInterrupHandlers + 4
	lsli    r1, 2
	addu    r2, r1
	ld.w    r1, (r2)
	jsr     r1

       
       
       
       jbsr ctlReboot




	rte


// INT stub for exception vector
intStub:
  jbr intStub

//  .section        .vector,,c   // Locate the code in the ".text2" section.

  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0

vExceptionVector:
	.long intReset
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intAutovectorHandler + 1
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub
	.long intStub


