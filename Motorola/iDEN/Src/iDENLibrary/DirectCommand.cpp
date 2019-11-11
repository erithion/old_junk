
#include "DirectCommand.h"
#include <algorithm>
#include "DirectCRC.h"

using namespace Protocol::Direct;



Command::Command(const uchar_vt& message)
{
    const unsigned char ProtocolId = 0x10; 
    const unsigned char GenReq     = 0x02; 
    const unsigned char CrcByte    = 0x03; 



    unsigned char header[] = {ProtocolId, GenReq};
    unsigned char footer[] = {ProtocolId, CrcByte, 0, 0};

    buffer_.assign(header, header + sizeof(header));

    for(uchar_vt::const_iterator iter = message.begin(); iter != message.end(); ++iter)
    {
        if (*iter == ProtocolId)
        {
            buffer_.push_back(ProtocolId);
        }
        buffer_.push_back(*iter);
    }

    uchar_vt crcSequence(message);
    crcSequence.push_back(CrcByte);

    unsigned short checksum = CRC::Get(crcSequence); 

    footer[2] = (checksum >> 8) & 0xff;
    footer[3] = checksum & 0xff;

    std::copy(footer, footer + sizeof(footer), std::back_inserter(buffer_));
}


Command::~Command()
{
}

