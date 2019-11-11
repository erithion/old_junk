#include "Hardware.h"
#include "Ctl.h"

asm void ctlDisableInt()
{
	mfcr    r1, psr
	bclri   r1, 6
	mtcr    r1, psr
}
asm void ctlSupervisorWait()
{
	wait
}
void ctlReboot()
{
	ushort_t val = 0;
	ctlDisableInt();

	val = ctlWdt;

	ctlWdt |= 0x204;

	ctlSupervisorWait();
}