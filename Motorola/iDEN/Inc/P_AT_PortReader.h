#ifndef _AT_PORT_READER_H_
#define _AT_PORT_READER_H_

#include "Pt_IPort.h"
#include "P_PortReader.h"
#include <string>
#include <map>

namespace Protocol
{
    namespace AT
    {
        class PortReader : public Protocol::PortReader
        {
        public:

            enum Status {Ok,  Error, Busy, Next};

            PortReader(Port::IPort& port);

            uchar_vt Read(unsigned int timeout);

            Status GetStatus();

        private:

            bool FindResult(const uchar_vt& data);

            Status status_;

            typedef std::map <Status, std::string> Result;
            Result statuses_;

        };
    }

}

#endif // _AT_PORT_READER_H_