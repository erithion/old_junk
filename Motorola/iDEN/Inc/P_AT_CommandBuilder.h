#ifndef _P_AT_COMMAND_BUILDER_H_
#define _P_AT_COMMAND_BUILDER_H_

#include <string>

namespace Protocol
{
    namespace AT
    {
        class Command;

        class CommandBuilder
        {
        public:
            CommandBuilder();
            Command Init() const;
            Command Echo() const;
            Command GetManufacturer() const;
            Command GetFirmware() const;
            Command GetModelDate() const;
            Command InitFS() const;
            Command GetFileListening(const std::string& name) const;
            Command GetFolderListening(const std::string& name) const;
            Command SelectDirectory(const std::string& path) const;
            Command ReadFile(const std::string& name) const;
            virtual ~CommandBuilder();
        };
    }
}

#endif //_P_AT_COMMAND_BUILDER_H_