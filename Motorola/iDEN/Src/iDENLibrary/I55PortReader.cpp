
#include "I55PortReader.h"
#include "Endianess.h"

using namespace Protocol::I55;

PortReader::PortReader(Port::IPort& port)
: Protocol::PortReader(port), status_(Error)
{
}

bool PortReader::FindResult(const uchar_vt& data)
{
    status_ = PortReader::Error;
    if (data.size() < 8)
    {
        return false;
    }
   
    unsigned long size = *(unsigned long*)&data[3];
    size = Algorithm::Revert(size);
    
    if (data.size() < size + 8)
    {
        return false;
    }

    status_ = PortReader::Ok;
    return true;
}

PortReader::Status PortReader::GetStatus() const
{
    return status_;
}

