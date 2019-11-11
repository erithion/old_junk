#ifndef _I_DEVICE_H_
#define _I_DEVICE_H_

#include "types.h"
#include <string>

class Interrupter;


class IDevice
{
public:
    virtual uchar_vt GetRAM(Interrupter& interapter, IProgressIden& progress) = 0;
    virtual uchar_vt GetFLEX(Interrupter& interapter, IProgressIden& progress) = 0;
    virtual uchar_vt GetFS(Interrupter& interapter, IProgressIden& progress) = 0;
    virtual const std::string& GetModelFirmware() = 0;
    virtual const std::string& GetModelName() = 0;
    virtual std::string GetModelType() = 0;
    virtual ~IDevice() = 0 
    {}
};

#endif // _I_DEVICE_H_