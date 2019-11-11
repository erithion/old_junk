#ifndef _PORT_STRATEGY_H_
#define _PORT_STRATEGY_H_

#include "AutoHandle.h"
#include <windows.h>


namespace Port
{

    class PortStrategy
    {
    public:

        void SetState(DCB& state);
        virtual ~PortStrategy();

    protected:

        void SetTimeouts(COMMTIMEOUTS& timeouts);
        PortStrategy(AutoHandle& fileHandle);

    private:

        AutoHandle&    fileHandle_;
        DCB             backupDCB_;
        COMMTIMEOUTS    backupTimeouts_; 
    };

}

#endif // _PORT_STRATEGY_H_
