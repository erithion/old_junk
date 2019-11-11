#ifndef _REPLY_PARSER_H_
#define _REPLY_PARSER_H_

#include "Types.h"

namespace Protocol
{
	template <typename>
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