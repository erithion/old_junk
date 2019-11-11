#ifndef _FLASHSTRAP_PORT_READER_H_
#define _FLASHSTRAP_PORT_READER_H_

#include "P_PortReader.h"

namespace Protocol
{
    namespace FlashStrap
    {
        class PortReader : public Protocol::PortReader
        {
        public:

            enum Status {Ok, Error};

            PortReader(Port::IPort& port);

            Status GetStatus();

             ~PortReader();
        private:

            bool FindResult(const uchar_vt& data);

            Status status_;

        };
    }

}

#endif // _FLASHSTRAP_PORT_READER_H_