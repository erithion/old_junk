#ifndef _IDEN_CRC_H_
#define _IDEN_CRC_H_

#include "types.h"
#include <numeric>

namespace Algorithm
{
    template <class ITERATOR>
    unsigned char GetI55Checksum(ITERATOR begin, ITERATOR end, unsigned char startCrc = 0)
    {
        unsigned char sum = startCrc;
        sum = std::accumulate(begin, end, sum);
        return 0xff - sum;
    }

    //unsigned char GetI55EmptyChecksum()
    //{
    //    return 0x100 - 1;
    //}

}

#endif // _IDEN_CRC_H_
