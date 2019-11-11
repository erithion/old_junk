#ifndef _DIRECT_REPLY_PARSER_H_
#define _DIRECT_REPLY_PARSER_H_

#include "P_ReplyParser.h"

namespace Protocol
{

    namespace Direct
    {
        class ReplyParser : public Protocol::ReplyParser
        {
        public:
            ReplyParser();
            uchar_vt Parse(const uchar_vt& buffer);
            ~ReplyParser();
        };
    }
}

#endif // _DIRECT_REPLY_PARSER_H_