#ifndef _P_DIR_COMMAND_BUILDER_H_
#define _P_DIR_COMMAND_BUILDER_H_

namespace Protocol
{
    namespace Direct
    {
        class Command;

        class CommandBuilder
        {
        public:
            CommandBuilder();
            Command GetImei();
            Command GetMemInfo();
            Command GetPhnEntity(unsigned char type, unsigned char id);
            Command GetSMSEntity(unsigned char id);
            Command RSSSwitch();
            virtual ~CommandBuilder();
        };
    }
}

#endif //_P_DIR_COMMAND_BUILDER_H_