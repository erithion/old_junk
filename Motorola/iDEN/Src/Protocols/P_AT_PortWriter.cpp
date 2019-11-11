#include "P_AT_PortWriter.h"

using namespace Protocol::AT;

PortWriter::PortWriter(Port::IPort& port)
    : Protocol::PortWriter(port)
{
}

void PortWriter::WriteNext()
{
    char nextCommand[] = "##>";
    Write(uchar_vt(nextCommand, nextCommand + sizeof(nextCommand)));
}

PortWriter::~PortWriter()
{
}
