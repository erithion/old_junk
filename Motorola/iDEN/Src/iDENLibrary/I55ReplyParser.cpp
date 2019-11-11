
#include "I55ReplyParser.h"
#include "IdenCRC.h"
#include <exception>

using namespace Protocol::I55;

ReplyParser::ReplyParser()
{
}

uchar_vt ReplyParser::Parse(const uchar_vt& buffer)
{
    if (buffer.size() < 8)
    {
        throw std::exception("Incomplete packet");
    }
    uchar_vt result(buffer.begin() + 7, buffer.end() - 1);

    unsigned char crc = Algorithm::GetI55Checksum(result.begin(), result.end());
    if (crc != buffer.back())
    {
        throw std::exception("Check sum mismatch");
    }
    return result;
}

ReplyParser::~ReplyParser()
{
}
