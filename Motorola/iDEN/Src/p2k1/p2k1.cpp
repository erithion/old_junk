#include <Windows.h>
#include <setupapi.h>
#include <string>

#include "AutoHandle.h"
#include "Types.h"
#include "p2k1.h"

using namespace p2k1;

typedef ::HANDLE                     handle;
typedef ::DWORD                      dword;
typedef ::SP_DEVICE_INTERFACE_DATA   devInterface;
typedef ::SP_DEVICE_INTERFACE_DETAIL_DATA_W    devInterfaceDetail;
typedef ::HDEVINFO                   devInfo;
typedef ::OVERLAPPED                 overlapped;
typedef ::GUID                       guid;

#pragma pack(push)
#pragma pack(1)
struct p2k1Header
{
	uchar mode_;
	uint  size_;
};
#pragma pack(pop)

struct p2k1Packet
{
	ulong    command_;
	uchar_vt inBuffer_;
	uchar_vt outBuffer_;
	ulong    timeout_;
};

static const guid  constP2k1 = 
{
	0x4E1E1E15L, 
	0x52D7, 
	0x11D4, 
	{0x85, 0x2D, 0x00, 0x10, 0x5A, 0xA6, 0xF8, 0x0B}
};
static const ulong constWriteCommand    = 0x80002018L;
static const ulong constReadCommand     = 0x8000201CL;
static const ulong constInfoCommand     = 0x80002010L;
static const uint  constMaxSize         = 0x100000;  

// outBuffer is supposed to be already resized upon entering to the method.
// Thus DeviceIoControl uses an already allocated memory for output
bool controlDevice(handle fileHandle, p2k1Packet& packet)
{
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		try
		{
			//-------create overlapped structure---------
			autoHandle event(::CreateEvent(0, TRUE, FALSE, 0));
			overlapped ovl;
			dword      nRead = 0;

			memset(&ovl, 0, sizeof(ovl));
			ovl.hEvent = event;
			//-------issue device io control read command-------
			uchar* in = (uchar*)&(packet.inBuffer_.front());
			uchar* out = (uchar*)&(packet.outBuffer_.front());
			dword  inSize = (dword)packet.inBuffer_.size();
			dword  outSize = (dword)packet.outBuffer_.size();
			if (::DeviceIoControl(fileHandle, packet.command_, in, inSize, out, outSize, &nRead, &ovl) == FALSE)
			{
				if (::GetLastError() == ERROR_IO_PENDING)
				{
					// Wait for the overlapped operation to complete
					switch (::WaitForSingleObject(ovl.hEvent, packet.timeout_))
					{
					case WAIT_OBJECT_0:
						{
							// The overlapped operation has completed
							return ::GetOverlappedResult(fileHandle, &ovl, &nRead, TRUE) == TRUE; 
						}

					case WAIT_TIMEOUT:
						{
							// Cancel the I/O operation
							CancelIo(fileHandle);
							return false;
						}
					}
				}
			}
			return true;
		}
		catch (const std::exception&)
		{		
		}
	}
	return false;
}

std::wstring getInterfaceDetail(devInfo deviceInfo, dword index)
{
	std::wstring deviceName(L"");
	devInterface ifcData;
	dword        dwSizeOfBuffer = 256;
	
	memset(&ifcData, 0, sizeof devInterface);
	ifcData.cbSize = sizeof devInterface;

	if (::SetupDiEnumDeviceInterfaces(deviceInfo, 0, &constP2k1, index, &ifcData))
	{
		if (::SetupDiGetDeviceInterfaceDetailW(deviceInfo, &ifcData, 0, 0, &dwSizeOfBuffer, 0) ||
			::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			char_vt DetData(dwSizeOfBuffer);
			devInterfaceDetail* pDetData = (devInterfaceDetail*)&DetData.front();
			pDetData->cbSize = sizeof devInterfaceDetail;
			if (::SetupDiGetDeviceInterfaceDetailW(deviceInfo, &ifcData, pDetData, dwSizeOfBuffer, 0, 0)) 
			{
				deviceName.assign(pDetData->DevicePath);
			}
		}
	}
	return deviceName;
}

handle getMotorolaFlash(const std::wstring& deviceName)
{
	handle handle = ::CreateFileW(
		deviceName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);

	if (INVALID_HANDLE_VALUE != handle)
	{
		p2k1Packet packet = 
		{
			constInfoCommand,
			uchar_vt(2, 0),
			uchar_vt(constMaxSize, 0),
			0xFFFF
		};

		packet.inBuffer_[0] = 2;

		if (controlDevice(handle, packet))
		{
			wchar_t* szAccessory = L"Motorola Flash";
			wchar_t* szInterfaceName = (wchar_t*)(&packet.outBuffer_.front() + 4);
			if (wcscmp(szAccessory, szInterfaceName) == 0)
			{
				return handle;
			}
		}
		::CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
	return handle;
}

handle acquireP2k1()
{
	handle   result = INVALID_HANDLE_VALUE;

	devInfo  hDevInfo = ::SetupDiGetClassDevsW(&constP2k1, 
		                   NULL, 
						   NULL, 
						   DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return result;
	}

	dword        index = 0;
	std::wstring deviceName(getInterfaceDetail(hDevInfo, index++));
	
	while (!deviceName.empty())
	{
		result = getMotorolaFlash(deviceName);
		if (INVALID_HANDLE_VALUE != result)
		{
			break;
		}
	}

	::SetupDiDestroyDeviceInfoList(hDevInfo);
	return result;
}

handle p2k1::get()
{
	int    nCounter = 10;
	handle result = INVALID_HANDLE_VALUE;
	do
	{
		result = acquireP2k1();
		if (result != INVALID_HANDLE_VALUE)
		{
			return result;
		}
		::Sleep(1000);
	}
	while (nCounter--);

	return INVALID_HANDLE_VALUE;
}

bool p2k1::write(handle handle, const uchar_vt& in)
{
	p2k1Packet packet = 
	{
		constWriteCommand, 
		uchar_vt(sizeof p2k1Header + in.size() + 1, 0),
		uchar_vt(0),
		0xFFFF
	};

	p2k1Header header = {2, static_cast<uint>(in.size())};

	std::copy((uchar*)&header, (uchar*)&header + sizeof p2k1Header, packet.inBuffer_.begin());
	std::copy(in.begin(), in.end(), packet.inBuffer_.begin() + sizeof p2k1Header);

	return controlDevice(handle, packet);
}

uchar_vt p2k1::read(handle handle, uint timeout)
{
	p2k1Packet packet = 
	{
		constReadCommand, 
		uchar_vt(sizeof p2k1Header, 0),
		uchar_vt(constMaxSize, 0),
		timeout
	};

	p2k1Header* header = (p2k1Header*)&packet.inBuffer_.front();
	header->mode_ = 2;
	header->size_ = 0xffff;

	if (controlDevice(handle, packet))
	{
		uint size = *(uint*)&packet.outBuffer_.front();

		return uchar_vt(packet.outBuffer_.begin() + sizeof(size), 
			            packet.outBuffer_.begin() + sizeof(size) + size);
	}
	return uchar_vt(0);
}
