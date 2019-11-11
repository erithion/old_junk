
#include "FlashStrapCommand.h"
#include "S3_Dump.h"
#include "IdenCRC.h"
#include "Endianess.h"

using namespace Protocol::FlashStrap;

Command::Command()
: currentCrc_(0)
{
    char command[] = {'a', 'A'};
    buffer_.assign(command, command + sizeof(command));
}


Command::Command(size_t dumpSize, unsigned long address, bool isUsb)
{

    const unsigned char ProtocolId = 0x55; 

    const static unsigned char header[] = {ProtocolId, ProtocolId, ProtocolId, 0xaa};
    const static unsigned char footer[] = {0, 0x08};

    buffer_.assign(header, header + sizeof(header));

    uchar_vt addressArrary = Algorithm::Serialize(Algorithm::Revert<unsigned long>(address));
    std::copy(addressArrary.begin(), addressArrary.end(), std::back_inserter(buffer_));

    unsigned short size = static_cast<unsigned short> (dumpSize);
    size += isUsb ? 1 : 0;
    uchar_vt sizeArrary = Algorithm::Serialize(Algorithm::Revert(size));
    std::copy(sizeArrary.begin(), sizeArrary.end(), std::back_inserter(buffer_));
    std::copy(footer, footer + sizeof(footer), std::back_inserter(buffer_));

    currentCrc_ = Algorithm::GetI55Checksum(buffer_.begin() + 4, buffer_.end());

}

Command::Command(const uchar_vt& dump, unsigned char startCrc)
{
    std::copy(dump.begin(), dump.end(), std::back_inserter(buffer_));
    buffer_.push_back(Algorithm::GetI55Checksum(buffer_.begin(), buffer_.end(), 0xff - startCrc));
}


Command::Command(const uchar_vt& dump, unsigned long address, bool isUsb)
{

    const unsigned char ProtocolId = 0x55; 

    const static unsigned char header[] = {ProtocolId, ProtocolId, ProtocolId, 0xaa};
    const static unsigned char footer[] = {0, 0x08};

    buffer_.assign(header, header + sizeof(header));

    uchar_vt addressArrary = Algorithm::Serialize(Algorithm::Revert<unsigned long>(address));
    std::copy(addressArrary.begin(), addressArrary.end(), std::back_inserter(buffer_));

    unsigned short size = static_cast<unsigned short> (dump.size());
    size += isUsb ? 1 : 0;
    uchar_vt sizeArrary = Algorithm::Serialize(Algorithm::Revert(size));
    std::copy(sizeArrary.begin(), sizeArrary.end(), std::back_inserter(buffer_));
    std::copy(footer, footer + sizeof(footer), std::back_inserter(buffer_));
    std::copy(dump.begin(), dump.end(), std::back_inserter(buffer_));
    buffer_.push_back(Algorithm::GetI55Checksum(buffer_.begin() + 4, buffer_.end()));
}


Command::~Command()
{
}

