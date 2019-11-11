
#include "P_AT_PortReader.h"
#include <algorithm>

using namespace Protocol::AT;

PortReader::PortReader(Port::IPort& port)
: Protocol::PortReader(port), status_(Busy)
{
    statuses_.insert(std::make_pair(Ok, "\r\nOK\r\n"));
    statuses_.insert(std::make_pair(Error, "\r\nERROR\r\n"));
    statuses_.insert(std::make_pair(Next, "#OK#\r\n"));
    statuses_.insert(std::make_pair(Next, "#OK#\r"));
}

PortReader::Status PortReader::GetStatus()
{
    return status_;
}

uchar_vt PortReader::Read(unsigned int timeout)
{
    uchar_vt result;
    uchar_vt part;
    while (port_.Read(part, timeout))
    {
        result.insert(result.end(), part.begin(), part.end());

        if (FindResult(result))
        {
            if (status_ == Next)
            {
                char nextCommand[] = {'#', '#', '>', '\r', '\n'};
                port_.Write(uchar_vt(nextCommand, nextCommand + sizeof(nextCommand)));
            }
            else
            {
                break;
            }
        }
    }
    return result;
}


bool PortReader::FindResult(const uchar_vt& data)
{
    status_ = Busy;
    for (Result::iterator i = statuses_.begin(); i != statuses_.end(); ++i)
    {
        if ((data.size() >= i->second.size()) && (std::find_end(data.end() - i->second.size(), data.end(), i->second.begin(), i->second.end()) != data.end()))
        {
            status_ = i->first;
            return true;
        }
    }
    return false;
}

