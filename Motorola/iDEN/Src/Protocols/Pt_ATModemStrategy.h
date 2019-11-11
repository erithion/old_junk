#ifndef _PORT_AT_MODEM_STRATEGY_H_
#define _PORT_AT_MODEM_STRATEGY_H_

#include "Pt_PortStrategy.h"

namespace Port
{

    class ATModemStrategy : public PortStrategy
    {
    public:

        ATModemStrategy(AutoHandle& fileHandle);

        DCB GetState();
        
        ~ATModemStrategy() {}

    private:
        
        COMMTIMEOUTS GetTimeouts();

    };

}

#endif // _PORT_AT_MODEM_STRATEGY_H_
