#ifndef _PORT_WRITER_H_
#define _PORT_WRITER_H_

#include "Types.h"

namespace Protocol
{
	template <typename Port>
    class PortWriter
    {
    public:
        PortWriter(Port& port)
            : port_(port)
		{
		}
        void Write(const uchar_vt& buffer)
        {
            port_.Write(buffer);
        }
        virtual ~PortWriter()
		{
		}
    protected:
        Port& port_;
    };
}

#endif // _PORT_WRITER_H_