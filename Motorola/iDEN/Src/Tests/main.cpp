// interface.cpp : Defines the entry point for the console application.
//

#include <tchar.h>

#include "types.h"
#include "autohandle.h"
#include "trace.h"
#include "p2k1.h"
#include "PortBaseStrategy.h"
#include "PortMountPoints.h"
#include "PortConfig.h"
#include "PortInput.h"
#include "PortOutput.h"

class testClass
{
public:
	testClass()
	{
	}
	~testClass()
	{
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	using port::pointSerial;
	using port::configSerialAT;

	pointSerial at("COM1");
	configSerialAT<pointSerial> setup(at);

	//using port::pointUsb;
	//pointUsb at2("COM2");
	//configSerialAT<pointUsb> setup2(at2);

	using port::configSerialSpeed;
	configSerialSpeed<pointSerial>(at, 115200);

	using port::inputSerial;
	inputSerial<pointSerial> reader(at);
	reader.in(uchar_vt(), 1);

	using port::outputSerial;
	outputSerial<pointSerial> writer(at);
	writer.out(uchar_vt());

	testClass test;
	inputSerial<testClass> testReader(test);

	return at.type();
}

