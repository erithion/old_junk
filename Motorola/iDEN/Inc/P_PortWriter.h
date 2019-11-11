#ifndef _PORT_WRITER_H_
#define _PORT_WRITER_H_

#include "Pt_IPort.h"

namespace Protocol
{

    class PortWriter
    {
    public:
        PortWriter(Port::IPort& port)
            : port_(port)   {}

        virtual void Write(const uchar_vt& buffer)
        {
            port_.Write(buffer);
        }
        virtual ~PortWriter() {}
    
    protected:
        Port::IPort& port_;

    };
}

#endif // _PORT_WRITER_H_