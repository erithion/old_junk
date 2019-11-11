#include "S3_Record.h"
#include <memory.h>

using namespace S3Dump;

Record::Record() :
	type(type_unknown),
	address(0),
	length(0)
{
}


Record::Record(const Record &arg) :
	type(arg.type),
	address(arg.address),
	length(arg.length)
{
	if (arg.length > 0)
		memcpy(data, arg.data, arg.length);
}


Record::Record(type_t arg) :
	type(arg),
	address(0),
	length(0)
{
}


Record::Record(type_t a1, address_t a2) :
	type(a1),
	address(a2),
	length(0)
{
}

Record::Record(type_t a1, address_t a2, const data_t *a3, int a4) :
	type(a1),
	address(a2),
	length(a4 > 0 ? a4 : 0)
{
	if (length > 0)
		memcpy(data, a3, length);
}


Record &
Record::operator=(const Record &arg)
{
	if (this != &arg)
	{
		type = arg.type;
		address = arg.address;
		length = arg.length;
		if (arg.length > 0)
			memcpy(data, arg.data, arg.length);
	}
	return *this;
}

Record::~Record()
{
}


Record::address_t
Record::decode_big_endian(data_t *buffer, int length)
{
	address_t result = 0;
	while (length-- > 0)
		result = (result << 8) | *buffer++;
	return result;
}




