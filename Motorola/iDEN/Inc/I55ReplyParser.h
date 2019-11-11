#ifndef _I55_REPLY_PARSER_H_
#define _I55_REPLY_PARSER_H_

#include "P_ReplyParser.h"

namespace Protocol
{

    namespace I55
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

#endif // _I55_REPLY_PARSER_H_