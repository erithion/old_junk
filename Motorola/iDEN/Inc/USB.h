#ifndef _PORT_USB_H_
#define _PORT_USB_H_

#include <exception>
#include <string>
#include <windows.h>
#include <setupapi.h>
#include "Pt_IPort.h"
#include "trace.h"

#include "AutoHandle.h"

namespace Port
{
    GUID guidMobile = {0x4E1E1E15L, 0x52D7, 0x11D4, {0x85, 0x2D, 0x00, 0x10, 0x5A, 0xA6, 0xF8, 0x0B}};


    class USB : public IPort
    {
    public:


        USB(const std::string& name) :
          fileHandle_(CreatePortHandle())
        {
        }

        bool Write(const uchar_vt& data)
        {
            Packet packet = 
            {
                constWriteCommand, 
                uchar_vt(sizeof(P2KHeader) + data.size() + 1, 0),
                uchar_vt(0),
                0xFFFF
            };

            P2KHeader header = 
            {
                2, 
                static_cast<unsigned int>(data.size())
            };

            std::copy((unsigned char*)&header, (unsigned char*)&header + sizeof(P2KHeader), packet.inBuffer_.begin());
            std::copy(data.begin(), data.end(), packet.inBuffer_.begin() + sizeof(P2KHeader));

            if (data.size())
            {
                _SERIAL_LOG_SEND((char*)&data.front(), (int)data.size());
            }

            return CommandDevice(fileHandle_, packet);
        }

        bool Read(uchar_vt& data, unsigned int timeout)
        {
            Packet packet = 
            {
                constReadCommand, 
                uchar_vt(sizeof(P2KHeader), 0),
                uchar_vt(constMaxSize, 0),
                (unsigned int)timeout
            };
            
            P2KHeader* header = (P2KHeader*)&packet.inBuffer_.front();
            header->mode_ = 2;
            header->size_ = 0xffff;
         
            if (CommandDevice(fileHandle_, packet))
            {
                unsigned int size = *(unsigned int*)&packet.outBuffer_.front();
                data.assign(packet.outBuffer_.begin() + sizeof(size), packet.outBuffer_.begin() + sizeof(size) + size);
                if (data.size())
                {
                    _SERIAL_LOG_RECIEVE((char*)&data.front(), (int)data.size());
                }

                return true;
            }
            return false;
        }
        
        virtual ~USB()
        {
        }

        void SetBaudRate(int baudRate)
        {
        }

        bool IsUsb()
        {
            return true;
        }


    private:

#pragma pack(push)
#pragma pack(1)


        struct P2KHeader
        {
            unsigned char mode_;
            unsigned int  size_;
        };


#pragma pack(pop)

        struct Packet
        {
            unsigned long command_;
            uchar_vt inBuffer_;
            uchar_vt outBuffer_;
            unsigned long timeout_;
        };



    private:

        static const unsigned long constWriteCommand    = 0x80002018L;
        static const unsigned long constReadCommand     = 0x8000201CL;
        static const unsigned long constInfoCommand     = 0x80002010L;
        static const unsigned int  constMaxSize         = 0x100000;  


        // outBuffer is supposed to be already resized upon entering to the method.
        // Thus DeviceIoControl uses an already allocated memory for output
        bool CommandDevice(HANDLE fileHandle, Packet& packet)
        {
            //-------create overlapped structure---------
            AutoHandle event(::CreateEvent(0, TRUE, FALSE, 0));
            OVERLAPPED overlapped;
            memset(&overlapped, 0, sizeof(overlapped));
            overlapped.hEvent = event;

            DWORD bytesReturned = 0;
          
            //-------issue device io control read command-------
            if (DeviceIoControl(fileHandle, packet.command_, 
                (unsigned char*)&(packet.inBuffer_.front()), (DWORD)packet.inBuffer_.size(), 
                (unsigned char*)&(packet.outBuffer_.front()), (DWORD)packet.outBuffer_.size(), 
                &bytesReturned, 
                &overlapped) == FALSE)
            {
                if (::GetLastError() == ERROR_IO_PENDING)
                {
                    // Wait for the overlapped operation to complete
                    switch (::WaitForSingleObject(overlapped.hEvent, packet.timeout_))
                    {
                    case WAIT_OBJECT_0:
                        {
                            // The overlapped operation has completed
                            return ::GetOverlappedResult(fileHandle, &overlapped, &bytesReturned, TRUE) == TRUE; 
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



        HANDLE CreatePortHandle()
        {
            int nCounter = 10;
            HANDLE result = INVALID_HANDLE_VALUE;
            do
            {
                result = AcquireP2K();
                if (result != INVALID_HANDLE_VALUE)
                {
                    return result;
                }
                ::Sleep(1000);
            }
            while (nCounter--);

            throw std::exception ("Cant find P2K Flash Interface");
        }

        HANDLE AcquireP2K()
        {
            HANDLE result = INVALID_HANDLE_VALUE;
            GUID *guidDev = &Port::guidMobile;

            HDEVINFO hDevInfo =  ::SetupDiGetClassDevsW( guidDev , NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
            if( hDevInfo == INVALID_HANDLE_VALUE ) 
            {
                return result;
            }
            BOOL bResult = TRUE;
            DWORD index = 0;
            std::wstring deviceName;
            while (GetInterfaceDetail(hDevInfo, index++, deviceName))
            {
                if (IsMotorolaFlash(deviceName, result))
                {
                    break;
                }
            }
       
            SetupDiDestroyDeviceInfoList(hDevInfo);
            return result;
        }


        bool GetInterfaceDetail(HDEVINFO& deviceInfo, DWORD index, std::wstring& deviceName)
        {
            GUID *guidDev = &guidMobile;

            SP_DEVICE_INTERFACE_DATA ifcData;
            memset(&ifcData, 0, sizeof(SP_DEVICE_INTERFACE_DATA));
            ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

            DWORD dwSizeOfBuffer = 256;
            if (::SetupDiEnumDeviceInterfaces(deviceInfo, 0, guidDev, index, &ifcData))
            {
                if (::SetupDiGetDeviceInterfaceDetailW(deviceInfo, &ifcData, 0, 0, &dwSizeOfBuffer, 0) ||
                    ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                {
                    std::vector <char> DetData(dwSizeOfBuffer);
                    SP_DEVICE_INTERFACE_DETAIL_DATA_W * pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA_W *)&DetData.front();
                    pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
                    if (::SetupDiGetDeviceInterfaceDetailW(deviceInfo, &ifcData, pDetData, dwSizeOfBuffer, 0, 0)) 
                    {
                        deviceName.assign(pDetData->DevicePath);
                        return true;
                    }
                }
            }
            return false;
        }

        bool IsMotorolaFlash(std::wstring& deviceName, HANDLE& handle)
        {
            handle = ::CreateFileW(
                deviceName.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_WRITE | FILE_SHARE_READ,
                0,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                NULL);

            if (INVALID_HANDLE_VALUE == handle)
            {
                return false;
            }

            Packet packet = 
            {
                constInfoCommand,
                uchar_vt(2, 0),
                uchar_vt(constMaxSize, 0),
                0xFFFF
            };
            
            packet.inBuffer_[0] = 2;

            if (CommandDevice(handle, packet))
            {
                wchar_t* szAccessory = L"Motorola Flash";
                wchar_t* szInterfaceName = (wchar_t*)(&packet.outBuffer_.front() + 4);
                if (wcscmp(szAccessory, szInterfaceName) == 0)
                {
                    return true;
                }
            }
            ::CloseHandle(handle);
            handle = INVALID_HANDLE_VALUE;
            return false;
        }


    private:

        AutoHandle fileHandle_;

    };

}

#endif // _PORT_USB_H_