#include "Hardware.h"

void hrdInit()
{
	unsigned short data = 0xF7FF;

	sciCReg10 |= 0xC6;

	sciCReg12 = sciCReg12;
	sciCReg14 = sciCReg14;

	sciCReg8 |= 0x20;

	sciCReg16 |= 0x2001;

	data &= sciCReg18;
	data |= 0x500;
	sciCReg18 = data;
}