#ifndef _REPLY_PARSER_H_
#define _REPLY_PARSER_H_

#include "types.h"

typedef std::vector<unsigned char> uchar_vt;

namespace Protocol
{

    class ReplyParser
    {
    public:
        virtual uchar_vt Parse(const uchar_vt& buffer) 
        {
            return buffer;
        }
        virtual ~ReplyParser(){}

    };
}

#endif // _REPLY_PARSER_H_