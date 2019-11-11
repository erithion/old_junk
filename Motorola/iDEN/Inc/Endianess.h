#ifndef _ENDIANESS_H_
#define _ENDIANESS_H_

#include "types.h"

namespace Algorithm
{
    template <class T>
    T Revert(T x)
    {
        T result = 0;
        for (unsigned char i = sizeof(x); i > 0; --i)
        {
            T temp = static_cast<unsigned char>((x >> (8 * (i - 1))) & 0xff);
            result |= temp << ((sizeof(x) - i) * 8);
        }
        return result;
    }


    template <class T>
    uchar_vt Serialize(T x)
    {
        return uchar_vt((unsigned char*)&x, (unsigned char*)&x + sizeof(x));
    }

    
}

#endif // _ENDIANESS_H_
