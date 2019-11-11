#include "Pt_ATModemStrategy.h"

using namespace Port;

ATModemStrategy::ATModemStrategy(AutoHandle& fileHandle)
    : PortStrategy(fileHandle)
{
    SetState(GetState());
    SetTimeouts(GetTimeouts());
}

COMMTIMEOUTS ATModemStrategy::GetTimeouts()
{
    COMMTIMEOUTS result; 
    memset(&result, 0, sizeof(COMMTIMEOUTS));

    result.ReadIntervalTimeout = 100;
    result.ReadTotalTimeoutConstant = 1000;
    result.ReadTotalTimeoutMultiplier = 5;
    result.WriteTotalTimeoutConstant = 150;
    result.WriteTotalTimeoutMultiplier = 16;

    return result;
}

DCB ATModemStrategy::GetState()
{
    DCB result;
    memset(&result, 0, sizeof(DCB));
    result.DCBlength = sizeof(DCB);

    result.BaudRate = 19200;
    result.ByteSize = 8;
    result.Parity = NOPARITY; 
    result.StopBits = ONESTOPBIT;
    result.fRtsControl = RTS_CONTROL_ENABLE;
    result.fDtrControl = DTR_CONTROL_ENABLE;
    result.XoffLim = 512;
    result.XonLim = 2048;

    return result;
}
