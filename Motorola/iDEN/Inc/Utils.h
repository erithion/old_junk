#ifndef _UTILS_UTILS_H_
#define _UTILS_UTILS_H_

#include "Pt_IPort.h"

namespace Utils
{
    void InitModemMode(Port::IPort& port);
    void SwitchToDirect(Port::IPort& port, bool ignoreCard);
}

#endif // _UTILS_UTILS_H_