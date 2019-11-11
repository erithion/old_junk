#ifndef _I55_COMMAND_H_
#define _I55_COMMAND_H_

#include "P_Command.h"
#include "IdenCRC.h"
#include "Endianess.h"

namespace Protocol
{
    namespace I55
    {
        enum  CommandId
        {
            Dump    = 0x01,
            Reboot  = 0x02
        };

        class Command : public Protocol::Command
        {
        public:
           
            Command(CommandId id)
                : Protocol::Command(GetHeader(id))
            {
                uchar_vt size = Algorithm::Serialize<unsigned long>(0);
                buffer_.insert(buffer_.end(), size.begin(), size.end());
                buffer_.push_back(0xff);
            }

            template <class ITERATOR>
            Command(CommandId id, ITERATOR begin, ITERATOR end)
                : Protocol::Command(GetHeader(id))
            {
                unsigned long size = static_cast<unsigned long>(end - begin);
                size = Algorithm::Revert<unsigned long>(size);
                uchar_vt sizeArray = Algorithm::Serialize(size);
                
                buffer_.insert(buffer_.end(), sizeArray.begin(), sizeArray.end());
                buffer_.insert(buffer_.end(), begin, end);
                buffer_.push_back(Algorithm::GetI55Checksum(begin, end));
            }

            ~Command()
            {
            }
        private:
            
            uchar_vt GetHeader(CommandId id)
            {
                const unsigned char ProtocolID = 0x55;
                unsigned char header[] = {ProtocolID, id};
                return uchar_vt(header, header + sizeof(header));
            }


        };
    }
}

#endif // _I55_COMMAND_H_