
#include "DirectPortReader.h"
#include <algorithm>

using namespace Protocol::Direct;

unsigned char PortReader::acknowledge = 0x30; 


PortReader::PortReader(Port::IPort& port)
: Protocol::PortReader(port), status_(Error)
{
}

PortReader::Status PortReader::GetStatus()
{
    return status_;
}

void PortReader::SendAck()
{
    const unsigned char ProtocolId   = 0x10; 
    const unsigned char acknowledge0 = 0x30;
    const unsigned char acknowledge1 = 0x31;

    PortReader::acknowledge = (PortReader::acknowledge == acknowledge0) ? acknowledge1 : acknowledge0;

    unsigned char header[] = {ProtocolId, acknowledge};

    port_.Write(uchar_vt(header, header + sizeof(header)));
}

void PortReader::SendRepeator()
{
    const unsigned char ProtocolId   = 0x10; 
    const unsigned char Repeat       = 0x05;

    unsigned char buffer[] = {ProtocolId, Repeat};

    port_.Write(uchar_vt(buffer, buffer + sizeof(buffer)));
}


void PortReader::ResetAcknolage()
{
    PortReader::acknowledge = 0x30; 
}

bool PortReader::IsConnectPacket(const uchar_vt& data)
{
    const unsigned char ProtocolId   = 0x10; 
    const unsigned char acknowledge0 = 0x30;
    const unsigned char acknowledge1 = 0x31;
    const unsigned char GenReq       = 0x02; 
    const unsigned char asc1[] = {ProtocolId, acknowledge0}; 
    const unsigned char asc2[] = {ProtocolId, acknowledge1}; 
    const unsigned char reqId[] = {ProtocolId, GenReq, 0x05, 0x83}; 


    uchar_vt::const_iterator begMessage = data.begin();
    for (; begMessage < data.end() - sizeof(reqId); begMessage += sizeof(asc1))
    {
        if (!std::equal(begMessage, begMessage + sizeof(asc1), asc1) 
            && !std::equal(begMessage, begMessage + sizeof(asc2), asc2))
        {
            break;
        }
    }

    if (*(begMessage + 2) == 0)
    {
        return true;
    }

    return std::equal(begMessage, begMessage + sizeof(reqId), reqId);
}

uchar_vt PortReader::Read(unsigned int timeout)
{
    uchar_vt result;
    uchar_vt part;
    int tries = 5;
    unsigned int bTimeout = timeout;
    while (tries-- != 0)
    {
        while (port_.Read(part, bTimeout))
        {
            result.insert(result.end(), part.begin(), part.end());

            if (FindResult(result))
            {
                SendAck();
                if (!IsConnectPacket(result))
                {
                    return result;
                }
                result.clear();
            }
        }
        SendRepeator();
        bTimeout = 500;
    }

    return result;
}

bool PortReader::FindCRCPacket(const uchar_vt& data)
{
    const unsigned char ProtocolId   = 0x10; 
    const unsigned char CrcByte      = 0x03;
    const unsigned char CrcCommand[] = {ProtocolId, CrcByte};
    uchar_vt::const_iterator endIter = data.end() - 2;
    uchar_vt::const_iterator iter = std::find_end(data.begin(), endIter, CrcCommand, CrcCommand + sizeof(CrcCommand));
    if (endIter != iter)
    {
        if (iter != data.begin() && *(iter - 1) == ProtocolId)
        {
            return false;
        }
        return true;
    }
    return false;
}

bool PortReader::FindResult(const uchar_vt& data)
{
    const unsigned char ProtocolId  = 0x10; 
    const unsigned char ResendByte  = 0x05; 
    const unsigned char CrcByte     = 0x03;
    
    const unsigned char ResendCommand[] = {ProtocolId, ResendByte};
    const unsigned char CrcCommand[]    = {ProtocolId, CrcByte};
    
    status_ = Error;

    if (data.size() == 2)
    {
        if (std::equal(data.begin(), data.end(), ResendCommand))
        {
            status_ = Resend;
            return true;
        }
        return false;
    }
    if (data.size() < 4)
    {
        return false;
    }

    if (FindCRCPacket(data))
    {
        status_ = Ok;
        return true;
    }
    return false;
}

