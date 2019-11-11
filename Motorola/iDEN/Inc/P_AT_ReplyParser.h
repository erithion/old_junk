#ifndef _AT_REPLY_PARSER_H_
#define _AT_REPLY_PARSER_H_

#include "P_ReplyParser.h"

namespace Protocol
{

    namespace AT
    {
        class ReplyParser : public Protocol::ReplyParser
        {
        public:
            ReplyParser();
            uchar_vt Parse(const uchar_vt& buffer, const std::string& separator = "\x0D\x0A");
            ~ReplyParser();
        };
    }

}

#endif // _AT_REPLY_PARSER_H_