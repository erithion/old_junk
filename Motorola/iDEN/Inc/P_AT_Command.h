#ifndef _AT_COMMAND_H_
#define _AT_COMMAND_H_

#include "P_Command.h"
#include <string>

namespace Protocol
{

    namespace AT
    {
        class Command : public Protocol::Command
        {
        public:
            Command(const std::string& message, const std::string& footer = "\r");
            ~Command();
        };
    }
}

#endif // _AT_COMMAND_H_