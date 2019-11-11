
#include "P_Dir_CommandBuilder.h"
#include "DirectCommand.h"

using namespace Protocol::Direct;

CommandBuilder::CommandBuilder()
{
}

Command CommandBuilder::GetImei()
{
    unsigned char buffer[] = {0x06, 0x04, 0x01};
    return Command(uchar_vt(buffer, buffer + sizeof(buffer)));
}

Command CommandBuilder::GetMemInfo()
{
    unsigned char buffer[] = {0x04, 0x05, 0x01};
    return Command(uchar_vt(buffer, buffer + sizeof(buffer)));
}

Command CommandBuilder::GetPhnEntity(unsigned char type, unsigned char id)
{
    unsigned char buffer[] = {0x04, 0x05, 0x02, 0x00, type, id};
    return Command(uchar_vt(buffer, buffer + sizeof(buffer)));
}

Command CommandBuilder::GetSMSEntity(unsigned char id)
{
    unsigned char buffer[] = {0x01, 0x06, 0x02, id};
    return Command(uchar_vt(buffer, buffer + sizeof(buffer)));
}

Command CommandBuilder::RSSSwitch()
{
    unsigned char buffer[] = {0x06, 0x06, 0x4B, 0x00};
    return Command(uchar_vt(buffer, buffer + sizeof(buffer)));
}

CommandBuilder::~CommandBuilder()
{
}