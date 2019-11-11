
#include "P_AT_CommandBuilder.h"
#include "P_AT_Command.h"

#include <sstream>

using namespace Protocol::AT;

CommandBuilder::CommandBuilder()
{
}

Command CommandBuilder::Init() const
{
    return Command("");
}

Command CommandBuilder::Echo() const
{
    return Command("E 1");
}

Command CommandBuilder::GetManufacturer() const
{
    return Command("I 4");
}

Command CommandBuilder::GetFirmware() const
{
    return Command("I 2");
}

Command CommandBuilder::GetModelDate() const
{
    return Command("I 3");
}

Command CommandBuilder::InitFS() const
{
    return Command("+FSIF");
}

Command CommandBuilder::GetFileListening(const std::string& name) const
{
    std::stringstream stream;
    stream << "+FSDL=\"" << name << "\"";
    return Command(stream.str(), "\r");
}

Command CommandBuilder::GetFolderListening(const std::string& name) const
{
    std::stringstream stream;
    stream << "+FSDI=\"" << name << "\"";
    return Command(stream.str(), "\r");
}

Command CommandBuilder::SelectDirectory(const std::string& path) const
{
    std::stringstream stream;
    stream << "+FSCD=\"" << path << "\"";
    return Command(stream.str(), "\r");
}

Command CommandBuilder::ReadFile(const std::string& name) const
{
    std::stringstream stream;
    stream << "+FSFR=-1,\"" << name << "\"";
    return Command(stream.str(), "\r");
}

CommandBuilder::~CommandBuilder()
{
}
