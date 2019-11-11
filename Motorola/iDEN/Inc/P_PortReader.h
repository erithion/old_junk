#ifndef _PORT_READER_H_
#define _PORT_READER_H_

#include "Pt_IPort.h"

namespace Protocol
{

    class PortReader
    {
    public:

        PortReader(Port::IPort& port);
        virtual uchar_vt Read(unsigned int timeout);
        virtual ~PortReader();

    protected:
        
        virtual bool FindResult(const uchar_vt& data) = 0;

        Port::IPort& port_;

    };
}

#endif // _PORT_READER_H_