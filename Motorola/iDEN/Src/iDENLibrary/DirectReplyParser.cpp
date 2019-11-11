
#include "DirectReplyParser.h"
#include <algorithm>
#include <exception>

using namespace Protocol::Direct;

ReplyParser::ReplyParser()
{
}

uchar_vt ReplyParser::Parse(const uchar_vt& buffer)
{
    const unsigned char ProtocolId = 0x10; 
    const unsigned char GenReq     = 0x02; 
    const unsigned char CrcByte    = 0x03;

    const unsigned char asc1[] = {ProtocolId, 0x30}; 
    const unsigned char asc2[] = {ProtocolId, 0x31}; 
    const unsigned char wait[] = {ProtocolId, 0x05}; 
    const unsigned char reqId[] = {ProtocolId, GenReq}; 
    
    uchar_vt result;
    
    uchar_vt::const_iterator begMessage = buffer.begin();
    for (; begMessage != buffer.end(); begMessage += 2)
    {
        if (!std::equal(begMessage, begMessage + 2, asc1) 
            && !std::equal(begMessage, begMessage + 2, asc2)
            && !std::equal(begMessage, begMessage + 2, wait))
        {
            break;
        }
    }
    
    if (!std::equal(begMessage, begMessage + 2, reqId))
    {
        throw std::exception("Protocol ID was not found.");
    }
    
    begMessage += 2;
    
    uchar_vt::const_iterator iter = begMessage; 
    for(; iter != buffer.end(); ++iter)
    {
        if (*iter == ProtocolId)
        {
            if (*(iter + 1) == CrcByte)
            {
                break;
            }
            if (*(iter + 1) == ProtocolId)
            {
                ++iter;
            }
            else
            {
                throw std::exception("Unexpected symbol found");
            }
        }
        result.push_back(*iter);
    }

    if (iter == buffer.end())
    {
        throw std::exception("CRC not found.");
    }

    // TODO: check CRC;
    
    return result;
}

ReplyParser::~ReplyParser()
{
}
