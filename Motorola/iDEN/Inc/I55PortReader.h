#ifndef _I55_PORT_READER_H_
#define _I55_PORT_READER_H_

#include "Pt_IPort.h"
#include "P_PortReader.h"

namespace Protocol
{
    namespace I55
    {
        class PortReader : public Protocol::PortReader
        {
        public:

            enum Status { Ok, Error };
     
            PortReader(Port::IPort& port);

            Status GetStatus() const;

        private:

            bool FindResult(const uchar_vt& data);

            Status status_;

        };
    }

}

#endif // _I55_PORT_READER_H_