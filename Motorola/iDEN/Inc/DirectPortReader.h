#ifndef _DIRECT_PORT_READER_H_
#define _DIRECT_PORT_READER_H_

#include "Pt_IPort.h"
#include "P_PortReader.h"

namespace Protocol
{
    namespace Direct
    {
        class PortReader : public Protocol::PortReader
        {
        public:

            enum Status { Ok, Resend, Error };

            PortReader(Port::IPort& port);
            Status GetStatus();
            static void ResetAcknolage();
            void SendAck();
            uchar_vt Read(unsigned int timeout);

        private:

            void SendRepeator();

            bool FindCRCPacket(const uchar_vt& data);

            bool IsConnectPacket(const uchar_vt& data);


            bool FindResult(const uchar_vt& data);

            Status status_;

        private:
            static unsigned char acknowledge; 
        };
    }

}

#endif // _DIRECT_PORT_READER_H_