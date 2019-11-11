#include "EnumDevices.h"
#include <windows.h>

using namespace Port;

ConnectionInfo_vt Port::GetDevices(const std::string& pattern)
{
    // Querying all DOS available devices
    std::vector<char>  ospath;
    ospath.resize(65535);
    size_t            oslen = ::QueryDosDevice(0, (LPSTR)&ospath.front(), (DWORD)ospath.size());
    size_t            i = 0;

    ConnectionInfo_vt result;
    while (i < oslen)
    {
        std::string dev = &ospath[i];
        // Saving to output in case it satisfies the specified pattern
        if (pattern.size() < dev.size() &&
            dev.compare(0, pattern.size(), pattern) == 0)
        {
            ConnectionInfo info;
            info.path_ = "\\\\.\\" + dev;
            info.name_ = dev;
            HANDLE file = ::CreateFile(info.path_.c_str(), FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
            ::GetLastError();
            COMMPROP         prop;
            // Saving the name in case opening operation succeeded and 
            // the device type is MODEM
            if (file != INVALID_HANDLE_VALUE &&
                ::GetCommProperties(file, &prop))
            {
                switch(prop.dwProvSubType )
                {
                case PST_MODEM:
                    info.name_ = "Modem Device (" + info.name_ + ")";
                    info.type_ = TypeUsb;
                    result.push_back(info);
                    break;
                case PST_RS232:
                    info.name_ = "Communications Port (" + info.name_ + ")";
                    info.type_ = TypeSerial;
                    result.push_back(info);
                    break;
                default:
                    break;
                }
            }
            ::CloseHandle(file);
        }
        i += dev.size() + 1;
    }
    return result;
}
