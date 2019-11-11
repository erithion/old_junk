#ifndef _P_AT_PORT_WRITER_H_
#define _P_AT_PORT_WRITER_H_

#include "P_PortWriter.h"

namespace Protocol
{
    namespace AT
    {
        class PortWriter : public Protocol::PortWriter
        {
        public:
            PortWriter(Port::IPort& port);
            void WriteNext();
            ~PortWriter();
        };
    }
}


#endif //_P_AT_PORT_WRITER_H_