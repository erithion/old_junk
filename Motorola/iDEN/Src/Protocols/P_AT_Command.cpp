
#include "P_AT_Command.h"

using namespace Protocol::AT;

Command::Command(const std::string& message, const std::string& footer)
{
    const std::string header = "AT";
    buffer_.assign(header.begin(), header.end());
    std::copy(message.begin(), message.end(), std::back_inserter(buffer_));
    std::copy(footer.begin(), footer.end(), std::back_inserter(buffer_));
}

Command::~Command()
{
}

