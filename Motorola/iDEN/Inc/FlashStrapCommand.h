#ifndef _FLASHSTRAP_COMMAND_H_
#define _FLASHSTRAP_COMMAND_H_

#include "P_Command.h"

namespace Protocol
{

    namespace FlashStrap
    {
        class Command : public Protocol::Command
        {
        public:
            Command();
            Command(const uchar_vt& dump, unsigned char startCrc);
            Command(size_t dumpSize, unsigned long address, bool isUsb);
            Command(const uchar_vt& dump, unsigned long address, bool isUsb);
            ~Command();

            unsigned char GetCurrentCrc() const
            {
                return currentCrc_;
            }

        private:
            unsigned char currentCrc_;

        };
    }
}

#endif // _FLASHSTRAP_COMMAND_H_