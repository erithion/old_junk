#include "s3_parser.h"
#include <stdlib.h>

using namespace parser_n;

parser_c::parser_c() :
     _hfile(0)
{
}
parser_c::~parser_c()
{
	this->close();
}
bool parser_c::open(const std::string& name)
{
	this->close();
	this->_hfile = ::fopen(name.c_str(), "rb");
	return this->_hfile != 0;
}
void parser_c::close()
{
	if (this->_hfile != 0)
	{
		::fclose(this->_hfile);
		this->_hfile = 0;
	}
}
bool parser_c::first()
{
	if (this->_hfile != 0)
	{
		::rewind(this->_hfile);
		while (!feof(this->_hfile))
		{
			unsigned char s0[2];
			::fread((void*)s0, 2, 1, this->_hfile);
			if (memcmp((void*)s0, "\x53\x30", 2) == 0)
			{
				return true;
			}
		}
	}
	return false;
}
bool parser_c::next(raw_t& raw)
{
	raw.resize(2);
	if (this->_hfile != 0)
	{
		raw[0] = 0x53;
		raw[1] = 0x30;
		while (!feof(this->_hfile))
		{
			size_t size = raw.size();
			raw.resize(size + 2);
			::fread((void*)(&raw.front() + size), 2, 1, this->_hfile);
			if (::memcmp((void*)(&raw.front() + size), "\x53\x30", 2) == 0)
			{
				raw.resize(size);
				return true;
			}
		}
	}
	return false;
}