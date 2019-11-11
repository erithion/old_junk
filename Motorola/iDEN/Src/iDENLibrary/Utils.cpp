
#include "Utils.h"
#include "P_AT_PortReader.h"
#include "P_AT_ReplyParser.h"
#include "P_AT_Command.h"
#include "P_PortWriter.h"
#include "iDenException.h"
#include "DirectPortReader.h"
#include "DirectReplyParser.h"
#include "DirectCommand.h"

using namespace Port;

void Utils::InitModemMode(IPort& port)
{
    using namespace Protocol;
    AT::PortReader reader(port);
    PortWriter writer(port);

    AT::Command commandAT("");
    writer.Write(commandAT.GetBuffer());
    reader.Read(3000);

    writer.Write(commandAT.GetBuffer());
    reader.Read(3000);

    if (reader.GetStatus() != AT::PortReader::Ok)
    {
        throw iDenException("Plugin.Error.InitializeModem", "Plugin.Advice.CheckCable");
    }

    AT::Command commandEcho("E1");
    writer.Write(commandEcho.GetBuffer());
    reader.Read(3000);

    if (reader.GetStatus() != AT::PortReader::Ok)
    {
        throw iDenException("Plugin.Error.InitializeEcho", "Plugin.Advice.CheckCable");
    }
}

void Utils::SwitchToDirect(Port::IPort& port, bool ignoreCard)
{
    using namespace Protocol;
    AT::PortReader readerAT(port);
    PortWriter writerAT(port);

    AT::Command command("&FE0&D2V1Q0");
    writerAT.Write(command.GetBuffer());

    readerAT.Read(3000);

    if (readerAT.GetStatus() == AT::PortReader::Busy)
    {
        throw iDenException("Plugin.Error.SwitchDirect", "Plugin.Advice.SendLogs");
    }

    if (readerAT.GetStatus() == AT::PortReader::Error && !ignoreCard)
    {
        throw iDenException("Plugin.Error.SwitchDirect", "Plugin.Advice.CardAbsent");
    }
    else if (readerAT.GetStatus() == AT::PortReader::Error)
    {
        writerAT.Write(command.GetBuffer());
        readerAT.Read(300);
    }

    Direct::PortReader::ResetAcknolage();

    Direct::PortReader readerDirect(port);
    PortWriter writerDirect(port);

//    readerDirect.SendAck();
    unsigned char command_1[] = {0x06, 0x02, 0x01, 0x01, 0x00}; 
    Direct::Command commandRes1(uchar_vt(command_1, command_1 + sizeof(command_1)));
    writerDirect.Write(commandRes1.GetBuffer());
    readerDirect.Read(300);

    unsigned char command_2[] = {0x02, 0x01, 0x00}; 
    Direct::Command commandRes2(uchar_vt(command_2, command_2 + sizeof(command_2)));
    writerDirect.Write(commandRes1.GetBuffer());
    readerDirect.Read(300);

    if (readerDirect.GetStatus() == AT::PortReader::Busy)
    {
        throw iDenException("Plugin.Error.SwitchDirect", "Plugin.Advice.CheckCable");
    }
}

