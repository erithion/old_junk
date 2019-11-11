#ifndef _AUTO_HANDLE_H_
#define _AUTO_HANDLE_H_

#include <windows.h>
#include <exception>

class autoHandle
{
public:
	explicit autoHandle(::HANDLE handle) : handle_(handle)
	{
		if (handle == INVALID_HANDLE_VALUE)
		{
			throw std::exception("Invalid handle");
		}
	}

	~autoHandle()
	{
		::CloseHandle(handle_);
	}

	operator ::HANDLE() const 
	{
		return handle_;
	}
private:
	::HANDLE handle_;
private:
	autoHandle();
	autoHandle(const autoHandle&);
	autoHandle& operator=(const autoHandle&);
};

#endif //_AUTO_HANDLE_H_