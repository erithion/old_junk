
#include <vector>
#include <string>

namespace Port
{
    enum DeviceType
    {
        TypeSerial,
        TypeUsb
    };

    struct ConnectionInfo
    {
        DeviceType  type_;
        std::string	name_;
        std::string	path_;
    };

    typedef std::vector<ConnectionInfo>     ConnectionInfo_vt;
    ConnectionInfo_vt GetDevices(const std::string& pattern);
}
