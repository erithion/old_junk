#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <exception>
#include <string>
#include <windows.h>
#include "Pt_IPort.h"
#include "trace.h"


#include "AutoHandle.h"

namespace Port
{
    template <class STRATEGY>
    class Serial : public IPort
    {
    public:
        Serial(const std::string& name) :
          ovlReadHandle_ (::CreateEvent(0, 1, 0, 0)),
          ovlWriteHandle_(::CreateEvent(0, 1, 0, 0)),
          fileHandle_(CreatePortHandle(name)),
          strategy_(fileHandle_)
        {
            memset(&ovlRead_, 0, sizeof(OVERLAPPED));
            ovlRead_.hEvent = ovlReadHandle_;

            memset(&ovlWrite_, 0, sizeof(OVERLAPPED));
            ovlWrite_.hEvent = ovlWriteHandle_;
        }

        bool Write(const uchar_vt& data)
        {
            DWORD size = 0;
            if(!::WriteFile(fileHandle_, &data.front(), (DWORD)data.size(), &size, &ovlWrite_))
            {
                unsigned int error = ::GetLastError();
                switch(error)
                {
                case ERROR_IO_PENDING:
                case ERROR_IO_INCOMPLETE:
                    {
                        if(WAIT_OBJECT_0 == ::WaitForSingleObject(ovlWrite_.hEvent, 5000))
                        {
                            ::GetOverlappedResult(fileHandle_, &ovlWrite_, &size, TRUE);
                            break;
                        }
                    }
                    break;
                default:
                    return false;
                }
            }
            if (data.size())
            {
                _SERIAL_LOG_SEND((char*)&data.front(), (int)data.size());
            }
            return true;
        }


        bool Read(uchar_vt& data, unsigned int timeout)
        {
            DWORD size = 0;
            DWORD i = GetTickCount();
            while(((GetTickCount() - i)) < timeout && size == 0)
            {
                COMSTAT stat;
                memset(&stat, 0, sizeof (COMSTAT));
                DWORD   status = 0;
                if(ClearCommError(fileHandle_, &status, &stat) == FALSE)
                {
                    _TRW2("ClearCommError fails : ");
                    _TRW2("Error: %d", GetLastError());
                    return false;
                }
                else
                {
                    size = stat.cbInQue;
                    if (0 == size) 
                    {
                        Sleep(10);
                    }
                }
            }
            if (size > 0)
            {
                data.resize(size);
                if(0 == ::ReadFile(fileHandle_, &data.front(), static_cast<DWORD>(data.size()), &size, &ovlRead_))
                {
                    ::GetOverlappedResult(fileHandle_, &ovlRead_, &size, TRUE);
                }
                data.resize(size);
                if (data.size())
                {
                    _SERIAL_LOG_RECIEVE((char*)&data.front(), (int)data.size());
                }
                return true;
            }
            return false;
        }

        void SetBaudRate(int baudRate)
        {
            DCB current = strategy_.GetState();

            current.fOutxDsrFlow = 0;
            current.fDtrControl = DTR_CONTROL_ENABLE;
            current.fOutxCtsFlow = 0;
            current.fRtsControl = RTS_CONTROL_ENABLE;
            current.fInX = 0;
            current.fOutX = 0;

            strategy_.SetState(current);

            current.BaudRate = baudRate;
            strategy_.SetState(current);
        }

        bool IsUsb()
        {
            return false;
        }
        
        virtual ~Serial()
        {
            ::SetCommMask(fileHandle_, 0);
            ::PurgeComm(fileHandle_, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
            ::EscapeCommFunction(fileHandle_, CLRDTR);
        }

    private:

        HANDLE CreatePortHandle(const std::string& name)
        {
            return ::CreateFile(name.c_str(), 
                GENERIC_READ | GENERIC_WRITE, 
                0, 
                0, 
                OPEN_EXISTING, 
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
                0);
        }

        OVERLAPPED      ovlWrite_;
        OVERLAPPED      ovlRead_;

        AutoHandle fileHandle_;

        STRATEGY strategy_;

        AutoHandle ovlReadHandle_;
        AutoHandle ovlWriteHandle_;

    };

}

#endif // _SERIAL_H_