
#include "P_AT_ReplyParser.h"
#include <algorithm>

using namespace Protocol::AT;

ReplyParser::ReplyParser()
{
}

uchar_vt ReplyParser::Parse(const uchar_vt& buffer, const std::string& separator)
{
    const std::string endOfCommand = "\x0D\x0AOK\x0D\x0A";

    uchar_vt::const_iterator begMessage = std::search(buffer.begin(), buffer.end(), 
        separator.begin(), separator.end());
    
    if(begMessage == buffer.end())
    {
        return buffer;
    }
    
    uchar_vt::const_iterator endMessage = std::find_end(buffer.begin(), buffer.end(), 
        endOfCommand.begin(), endOfCommand.end());

    if (endMessage != begMessage)
    {
        begMessage += separator.size();
    }

    return uchar_vt(begMessage, endMessage);
}

ReplyParser::~ReplyParser()
{
}
