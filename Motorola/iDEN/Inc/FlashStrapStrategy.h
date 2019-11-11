#ifndef _PORT_FLASHSTRAP_STRATEGY_H_
#define _PORT_FLASHSTRAP_STRATEGY_H_

#include "Pt_PortStrategy.h"

namespace Port
{

    class FlashStrapStrategy : public PortStrategy
    {
    public:

        FlashStrapStrategy(AutoHandle& fileHandle);

        DCB GetState();

        ~FlashStrapStrategy() {}

    private:
        
        COMMTIMEOUTS GetTimeouts();

    };

}

#endif // _PORT_FLASHSTRAP_STRATEGY_H_
