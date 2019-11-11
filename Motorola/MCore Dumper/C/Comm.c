#include "Comm.h"
#include "MemMove.h"

#pragma section CODE


#ifdef SERIAL_MODE // SERIAL_MODE

#include "serial.h"
//
#define INIT  serialInit
#define WRITE serialWrite

int i = 0;
while (i < srcSize)
{
	serialWriteByte(srcBuffer[i]);
	++i;
}

#elif USB_MODE // USB_MODE

#include "USB.h"
//
#define INIT  usbInit
#define WRITE usbWrite

#else // Unknown Mode

#error "comm.c : There's no such compilation mode. Please, check the macro definitions. It can be either SERIAL_MODE or USB_MODE"

#endif


static unsigned char curData[maxBufferSize] = {0};
static unsigned char isDataSet = 0;
static unsigned char curPos = 0;
static unsigned char dataSize = 0;

void commMachine(const void* data, unsigned int size)
{
//	size = size <= maxBufferSize ? size : maxBufferSize;
//	memmove(curData, data, size);
//
//	isDataSet = 1;
//	curPos = 0;
//	dataSize = size;
//	char x = 0x50;
//	commWrite(&x, 1);
	commWrite(data, size);
}

void commInit()
{
	static unsigned int initFlag = 0;

	if (initFlag == 0)
	{
		INIT(commMachine);

		initFlag = 1;
	}
}

unsigned char commReadByte()
{
	while (isDataSet == 0);

	return curData[curPos++];
}
unsigned int commReadInt()
{
	unsigned int  result = 0;
	int           i = 0;

	while (isDataSet == 0 && dataSize < 4);

	while (i < 4)
	{
		result <<= 8;
		result |= curData[i];
		++i;
	}
	return result;
}


unsigned int commWrite(const void* data, unsigned int size)
{
	return WRITE(data, size);
}
