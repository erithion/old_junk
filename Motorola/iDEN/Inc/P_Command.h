#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "Pt_IPort.h"

namespace Protocol
{

    class Command
    {
    public:

        Command()
        {
        }

        Command(const Command& rth)
            : buffer_(rth.buffer_)
        {
        }

        Command(const uchar_vt& buffer)
            : buffer_(buffer)
        {
        }

        Command& operator = (const Command& rth)
        {
            buffer_ = rth.buffer_;
            return *this;
        }


        uchar_vt GetBuffer() const
        {
            return buffer_;
        }
        virtual ~Command() = 0 {}

    protected:

        uchar_vt buffer_;
    };
}

#endif // _COMMAND_H_