
#include "FlashStrapPortReader.h"
#include <algorithm>

using namespace Protocol::FlashStrap;

PortReader::PortReader(Port::IPort& port)
: Protocol::PortReader(port), status_(Error)
{
}

PortReader::~PortReader()
{
}


PortReader::Status PortReader::GetStatus()
{
    return status_;
}


bool PortReader::FindResult(const uchar_vt& data)
{
    const unsigned char idCmdWelcome = 0x5a;

    if (std::find(data.rbegin(), data.rend(), idCmdWelcome) != data.rend())
    {
        status_ = PortReader::Ok;
        return true;
    }

    status_ = PortReader::Error;
    return false;
}

