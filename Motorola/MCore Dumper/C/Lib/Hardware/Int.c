#include "Hardware.h"
#include "Int.h"

#pragma section CODE

void ret()
{
	return;
}

extern void usbEP0Reset();
extern void usbEPxIn();


/************************************************************************/
/* Interrupt's Auto-Vector                                              */
/************************************************************************/
intHandler vAutovectorNonNestedInterrupHandlers[65] = 
{
	        ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		usbEP0Reset,//ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		usbEPxIn,//ret,
		ret,
		ret,
		ret,
		ret,
		ret,
		ret
};


void intsSetupHandler(uchar_t intNumber, intHandler handler)
{
	vAutovectorNonNestedInterrupHandlers[intNumber] = handler;
}

