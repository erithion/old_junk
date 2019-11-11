#include "Pt_PortStrategy.h"

using namespace Port;

void PortStrategy::SetState(DCB& state)
{
    ::SetCommState(fileHandle_, &state);
    ::PurgeComm(fileHandle_, PURGE_TXCLEAR | PURGE_RXCLEAR);
    ::SetCommMask(fileHandle_, EV_RXCHAR);
}

void PortStrategy::SetTimeouts(COMMTIMEOUTS& timeouts)
{
    ::SetCommTimeouts(fileHandle_, &timeouts);
}

PortStrategy::PortStrategy(AutoHandle&  fileHandle)  : fileHandle_(fileHandle)
{
    backupDCB_.DCBlength = sizeof(DCB);
    ::GetCommState(fileHandle_, &backupDCB_);
    ::GetCommTimeouts(fileHandle_, &backupTimeouts_);
}

PortStrategy::~PortStrategy()
{
    ::SetCommMask(fileHandle_, 0);
    ::PurgeComm(fileHandle_, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    ::EscapeCommFunction(fileHandle_, CLRDTR);
    ::SetCommState(fileHandle_, &backupDCB_);
    ::SetCommTimeouts(fileHandle_, &backupTimeouts_);
}
