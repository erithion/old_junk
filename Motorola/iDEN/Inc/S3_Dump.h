#ifndef _S3_DUMP_H_
#define _S3_DUMP_H_

#include "types.h"

namespace S3Dump
{
    class Dump
    {
    public:
        public:
            explicit Dump(const uchar_vt& rowDump);
            const uchar_vt& GetBinary() const;
            unsigned long  GetAddress() const;
            ~Dump();
        private:
            uchar_vt  data_;
            unsigned long  address_;
    };
}

#endif // _S3_DUMP_H_