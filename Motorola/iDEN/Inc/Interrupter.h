#ifndef _INTERRUPTER_H_
#define _INTERRUPTER_H_

class Interrupter
{
public:
    explicit Interrupter(volatile long& abort_connect)
        : abort_connect_(abort_connect)
    {
    }

    bool DoInterrupt() const
    {
        return abort_connect_ != 0;
    }

private:
    volatile long& abort_connect_;
};

#endif //_INTERRUPTER_H_