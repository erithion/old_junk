#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <Windows.h>
#include "Types.h"
#include "iDenException.h"

class Resource
{
public:
	Resource()
	{
		MEMORY_BASIC_INFORMATION mbi;
		static int               dummy;

		::VirtualQuery(&dummy, &mbi, sizeof(mbi));

		module_ = (HANDLE)mbi.AllocationBase;
	}
	uchar_vt GetBinary(long id, const std::string& type)
	{
		HRSRC hr = FindResource((HMODULE)module_, (LPCSTR)id, type.c_str());
		if (hr == 0)
		{
			throw iDenException("Resource not found");
		}
		HGLOBAL hres = LoadResource((HMODULE)module_, hr);
		return uchar_vt((unsigned char*)hres, (unsigned char*)hres + ::SizeofResource((HMODULE)module_, hr));
	}

	~Resource(){}
private:
	HANDLE  module_;
};
#endif
