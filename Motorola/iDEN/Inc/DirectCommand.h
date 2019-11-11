#ifndef _DIRECT_COMMAND_H_
#define _DIRECT_COMMAND_H_

#include "P_Command.h"

namespace Protocol
{

    namespace Direct
    {
        class Command : public Protocol::Command
        {
        public:
           
            Command(const uchar_vt& message);
            ~Command();

        };
    }
}

#endif // _DIRECT_COMMAND_H_