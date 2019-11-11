#include "FlashStrapStrategy.h"

using namespace Port;

FlashStrapStrategy::FlashStrapStrategy(AutoHandle& fileHandle)
    : PortStrategy(fileHandle)
{
    SetState(GetState());
    SetTimeouts(GetTimeouts());
}

COMMTIMEOUTS FlashStrapStrategy::GetTimeouts()
{
    COMMTIMEOUTS result; 
    memset(&result, 0, sizeof(COMMTIMEOUTS));

    result.ReadIntervalTimeout = 100;
    result.ReadTotalTimeoutConstant = 300;
    result.ReadTotalTimeoutMultiplier = 150;
    result.WriteTotalTimeoutConstant = 150;
    result.WriteTotalTimeoutMultiplier = 15;

    return result;
}

DCB FlashStrapStrategy::GetState()
{
    DCB result;
    memset(&result, 0, sizeof(DCB));
    result.DCBlength = sizeof(DCB);

    result.BaudRate = 115200;
    result.ByteSize = 8;
    result.Parity = NOPARITY; 
    result.StopBits = ONESTOPBIT;
    result.fRtsControl = RTS_CONTROL_ENABLE;
    result.fDtrControl = DTR_CONTROL_ENABLE;
    result.XoffLim = 512;
    result.XonLim = 2048;

    return result;
}
