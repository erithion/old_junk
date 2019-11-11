#ifndef _DIRECT_CRC_H_
#define _DIRECT_CRC_H_

#include "types.h"

namespace Protocol
{

    namespace Direct
    {
        class CRC
        {
        public:
            static unsigned short Get(const uchar_vt& sequence);
        private:
            CRC() {}
            static const int table_[];
        };
    }
}

#endif // _DIRECT_COMMAND_H_