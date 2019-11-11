#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <string>
#include <bitset>
#include <algorithm>

#include "iDenException.h"
#include "Pt_IPort.h"
#include "./IProgress.h"
#include "Pt_Serial.h"
#include "Pt_ATModemStrategy.h"
#include "P_AT_Command.h"
#include "P_AT_PortReader.h"
#include "P_AT_ReplyParser.h"
#include "I55Command.h"
#include "I55PortReader.h"
#include "I55ReplyParser.h"
#include "DirectCommand.h"
#include "DirectPortReader.h"
#include "DirectReplyParser.h"

#include "FlashStrapCommand.h"
#include "FlashStrapPortReader.h"

#include "S3_Dump.h"
#include "IDevice.h"

#include "P_PortWriter.h"
#include "Utils.h"
#include "resource.h"

//#include "exception.h"

//#include "../Phone/Interrupter.h"

#include "MCUParser.h"

#include "tracing.h"


struct ModelConfig
{
    enum ModelType {Condor, Falcon};


    std::string Firmware;
    ModelType Type;
    std::string ModelName;
    long UsbFileName;
    long SerialFileName;
    Region ROM;
    Region RAM;
    Region FLEX;
    size_t CrapHeaderAddress;
    std::string ReadFirmware;
};


static ModelConfig config[] = 
{
    {"R87.00",    ModelConfig::Condor,   "Motorola iDEN i35/i30",        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),      Region(0x12000000, 0x12100000),   Region(0x10006000, 0x10242762), 0x100000D8, ""},
    {"R32.02",    ModelConfig::Condor,   "Motorola iDEN i50/i55/i85",    IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),      Region(0x12000000, 0x12100000),	  Region(0x10006000, 0x1030D89A), 0x100000D8, ""},
    {"R88.02",    ModelConfig::Condor,   "Motorola iDEN i58/i88",        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),      Region(0x12000000, 0x12100000),	  Region(0x10006000, 0x102F374E), 0x100000D8, ""},
    {"R74.00",    ModelConfig::Condor,   "Motorola iDEN i60", 	        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),      Region(0x12000000, 0x12100000),   Region(0x10006000, 0x10247532), 0x100000D8, ""},
    {"R76.02",    ModelConfig::Condor,   "Motorola iDEN i90",            IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),      Region(0x12000000, 0x12100000),   Region(0x10006000, 0x1030E952), 0x100000D8, ""},
    {"R85.01",    ModelConfig::Condor,   "Motorola iDEN i95/i99",	    IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),      Region(0x12000000, 0x12200000),   Region(0x10006000, 0x103A1832), 0x100000D8, ""},
    {"R1F.00",    ModelConfig::Falcon,   "Motorola iDEN i205/i215/i305", IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),	  Region(0x12000000, 0x12200000),   Region(0x10006000, 0x1033C742), 0x100000D8, ""},
    {"R86.00",    ModelConfig::Falcon,   "Motorola iDEN i205",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),	  Region(0x12000000, 0x12200000),   Region(0x10006000, 0x1033C742), 0x100000D8, ""},
    {"R3B.01",    ModelConfig::Falcon,   "Motorola iDEN i265",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10006000, 0x1068E282), 0x100000D8, ""},
    {"R3F.01",    ModelConfig::Falcon,   "Motorola iDEN i275",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10010000, 0x10774BC2), 0x100000D8, ""},
    {"R2D.00",    ModelConfig::Falcon,   "Motorola iDEN i285",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10006000, 0x1049C922), 0x100000D8, ""},
    {"R1B.00",    ModelConfig::Falcon,   "Motorola iDEN i315",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000),  	  Region(0x12000000, 0x12200000),   Region(0x10006000, 0x1037B3C2), 0x100000D8, ""},
    {"R1A.01",    ModelConfig::Falcon,   "Motorola iDEN i325",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10006000, 0x104ED762), 0x100000D8, ""},
    {"R4A.01",    ModelConfig::Falcon,   "Motorola iDEN i355/i415",      IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10010000, 0x10774BC2), 0x100000D8, ""},
    {"R4A.00",    ModelConfig::Falcon,   "Motorola iDEN i355/i415",      IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10010000, 0x10774BC2), 0x100000D8, ""},
    {"R0E.00",    ModelConfig::Falcon,   "Motorola iDEN i530",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12200000),   Region(0x10006000, 0x102D1BE2), 0x100000D8, ""},
    {"R4C.00",    ModelConfig::Falcon,   "Motorola iDEN i560",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10010000, 0x10778C7A), 0x100000D8, ""},
    {"R2F.01",    ModelConfig::Falcon,   "Motorola iDEN i605",	        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x13000000),   Region(0x10010000, 0x10C2E73A), 0x100000D8, ""},
    {"R2B.02",    ModelConfig::Falcon,   "Motorola iDEN i710",	        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10006000, 0x10634D02), 0x100000D8, ""},
    {"R0A.00",    ModelConfig::Falcon,   "Motorola iDEN i730/i733/i736", IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),	  Region(0x10006000, 0x105A9932), 0x100000D8, ""},
    {"R2B.01",    ModelConfig::Falcon,   "Motorola iDEN i730",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),	  Region(0x10006000, 0x105A9932), 0x100000D8, ""},
    {"R3E.01",    ModelConfig::Falcon,   "Motorola iDEN i760",	        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12800000),   Region(0x10010000, 0x11FEB938), 0x100000D8, ""},
    {"R2B.00.04", ModelConfig::Falcon,   "Motorola iDEN i830",	        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10006000, 0x1063F072), 0x100000D8, ""},
    {"R2B.00.03", ModelConfig::Falcon,   "Motorola iDEN i833",	        IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12400000),   Region(0x10006000, 0x1063ECD2), 0x100000D8, ""},
    {"R3D.01",	  ModelConfig::Falcon,   "Motorola iDEN i850/i855",      IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12800000),   Region(0x10010000, 0x11FEB938), 0x100000D8, ""},
    {"R1C.00",	  ModelConfig::Falcon,   "Motorola iDEN i860",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12800000),   Region(0x10006000, 0x10A84202), 0x100000D8, ""},
    {"R1C.02",	  ModelConfig::Falcon,   "Motorola iDEN i860",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x12800000),   Region(0x10006000, 0x10A84202), 0x100000D8, ""},
    {"R5B.00",	  ModelConfig::Falcon,   "Motorola iDEN i870",           IDR_DUMPER_USB, IDR_DUMPER_SERIAL, Region(0, 0x40000), 	  Region(0x12000000, 0x13000000),   Region(0x10010000, 0x11FEB938), 0x100000D8, ""}
};                                                                                                                
                                                                                                                  
                                                                                                                  
template <class PORT>                                                                                             
class Device : public IDevice                                                                                     
{                                                                                                                 
public:                                                                                                           
                                                                                                                  
    Device (const std::string& portName) 
        : model_(Initialize(portName)), port_(portName)
    {
        UploadDumper(GetS3Dump());
    }

    uchar_vt GetRAM(Interrupter& interapter, IProgressIden& progress)
    {
        progress.SetText(L"Plugin.Action.ReadRAM");
        return GetRegion(model_.RAM, interapter, progress);
    }

    uchar_vt GetFLEX(Interrupter& interapter, IProgressIden& progress)
    {
        progress.SetText(L"Plugin.Action.ReadFlex");
        return GetRegion(model_.FLEX, interapter, progress);
    }

    uchar_vt GetFS(Interrupter& interapter, IProgressIden& progress)
    {
        using MCUParser::SCLPType;
        using MCUParser::headerCrap;
        using MCUParser::headerSclpV1;
        using MCUParser::headerSclpV2;
        using MCUParser::headerSclpV3;
        using MCUParser::headerSclpV4;
        using MCUParser::headerSclpV5;

        progress.SetText(L"Plugin.Action.ReadFS");

        Region   crapHeaderSpace(model_.CrapHeaderAddress, model_.CrapHeaderAddress + sizeof(MCUParser::headerCrap));
        headerCrap  crapHeader =  mcuParser_.GetCrap(GetRegion(crapHeaderSpace, interapter, progress));

        size_t sclpHeaderAddress = GetSCLPAddress(crapHeader, interapter, progress);

        Region minSCLPRegion(sclpHeaderAddress, sclpHeaderAddress + sizeof(MCUParser::headerSclpMinimal));
        SCLPType type =  mcuParser_.GetSCLPType(GetRegion(minSCLPRegion, interapter, progress));

        Region userDataRegion;
        
        _TRW1("Type of SCLP structire = %u", type);

        switch (type)
        {
        case MCUParser::SCLPV1:
            {

                Region sclpRegion(sclpHeaderAddress, sclpHeaderAddress + sizeof(headerSclpV1));
                headerSclpV1 header = mcuParser_.GetSCLPHeader<headerSclpV1>(GetRegion(sclpRegion, interapter, progress));

                unsigned int* first = (unsigned int *)header.aCPEncodedBlocks;
                unsigned int* last = (unsigned int *)header.aCPEncodedBlocks  + sizeof(header.aCPEncodedBlocks) / sizeof(unsigned int);
                Region border = mcuParser_.DecodeBlock(uint_vt(first, last));           
                
                _TRW1("UDS description block bounds [0x%08X; 0x%08X]", border.first, border.second);
                size_t size = (border.second - border.first + 1) * header.nCPChunkSize;                     

                _TRW1("UDS size = 0x%08X", size);
                _TRW1("UDS chunk size = 0x%08X", header.nCPChunkSize);

                userDataRegion = Region(header.nCPAddress, header.nCPAddress + size);      

            }
            break;
        case MCUParser::SCLPV2:
            {
                Region sclpRegion(sclpHeaderAddress, sclpHeaderAddress + sizeof(headerSclpV2));
                headerSclpV2 header = mcuParser_.GetSCLPHeader<headerSclpV2>(GetRegion(sclpRegion, interapter, progress));

                unsigned int* first = (unsigned int *)header.aCPEncodedBlocks;
                unsigned int* last = (unsigned int *)header.aCPEncodedBlocks  + sizeof(header.aCPEncodedBlocks) / sizeof(unsigned int);
                Region border = mcuParser_.DecodeBlock(uint_vt(first, last));           

                _TRW1("UDS description block bounds [0x%08X; 0x%08X]", border.first, border.second);
                size_t size = (border.second - border.first + 1) * header.nCPChunkSize;                     

                _TRW1("UDS size = 0x%08X", size);
                _TRW1("UDS chunk size = 0x%08X", header.nCPChunkSize);

                userDataRegion = Region(header.nCPAddress, header.nCPAddress + size);      
            }
            break;
        case MCUParser::SCLPV3:
            {
                Region sclpRegion(sclpHeaderAddress, sclpHeaderAddress + sizeof(headerSclpV3));
                headerSclpV3 header = mcuParser_.GetSCLPHeader<headerSclpV3>(GetRegion(sclpRegion, interapter, progress));
                unsigned int* first = (unsigned int *)header.aCPEncodedBlocks;
                unsigned int* last = (unsigned int *)header.aCPEncodedBlocks  + sizeof(header.aCPEncodedBlocks) / sizeof(unsigned int);
                Region border = mcuParser_.DecodeBlock(uint_vt(first, last));           
                _TRW1("UDS description block bounds [0x%08X; 0x%08X]", border.first, border.second);
                size_t size = (border.second - border.first + 1) * header.nCPChunkSize;                     

                _TRW1("UDS size = 0x%08X", size);
                _TRW1("UDS chunk size = 0x%08X", header.nCPChunkSize);

                userDataRegion = Region(header.nCPAddress, header.nCPAddress + size);      
            }
            break;
        case MCUParser::SCLPV4:
            {
                Region sclpRegion(sclpHeaderAddress, sclpHeaderAddress + sizeof(headerSclpV4));
                headerSclpV4 header = mcuParser_.GetSCLPHeader<headerSclpV4>(GetRegion(sclpRegion, interapter, progress));
                unsigned int* first = (unsigned int *)header.aCPEncodedBlocks;
                unsigned int* last = (unsigned int *)header.aCPEncodedBlocks  + sizeof(header.aCPEncodedBlocks) / sizeof(unsigned int);
                Region border = mcuParser_.DecodeBlock(uint_vt(first, last));           
                _TRW1("UDS description block bounds [0x%08X; 0x%08X]", border.first, border.second);
                size_t size = (border.second - border.first + 1) * header.nCPChunkSize;                     

                _TRW1("UDS size = 0x%08X", size);
                _TRW1("UDS chunk size = 0x%08X", header.nCPChunkSize);

                userDataRegion = Region(header.nCPAddress, header.nCPAddress + size);      
            }
            break;
        case MCUParser::SCLPV5:
        case MCUParser::SCLPV6:
            {
                Region sclpRegion(sclpHeaderAddress, sclpHeaderAddress + sizeof(headerSclpV5));
                headerSclpV5 header = mcuParser_.GetSCLPHeader<headerSclpV5>(GetRegion(sclpRegion, interapter, progress));
                headerSclpV5::blockSclpV5 block = mcuParser_.FindBlock(header, "CP");

                size_t size = block.nChunksCount * block.nChunkSize;                     

                _TRW1("UDS size = 0x%08X", size);
                _TRW1("UDS chunk size = 0x%08X", block.nChunkSize);

                userDataRegion = Region(block.nAddress, block.nAddress + size);      
            
            }
            break;
        }


        return GetRegion(userDataRegion, interapter, progress);          
    }                                                                                

    const std::string& GetModelFirmware()                                            
    {                                                                                
        return model_.ReadFirmware;                                                  
    }                                                                                
                                                                                     
    const std::string& GetModelName()                                                
    {                                                                                
        return model_.ModelName;                                                     
    }                                                                                
                                                                                     
    std::string GetModelType()                                                
    {                                                                                
        return model_.Type == ModelConfig::Condor ? "Condor" : "Falcon";
    }

    virtual ~Device()
    {
        Reboot();
    }

private:

    size_t GetSCLPAddress(const MCUParser::headerCrap& crapStruct, Interrupter& interapter, IProgressIden& progress)
    {
        if (mcuParser_.IsCrap(crapStruct))
        {
            // in case CRAP - first signature present crapStruct.addressSclpHeader_ contains address of the SCLP header
            return crapStruct.addressSclpHeader_;
        }
        else if (mcuParser_.IsIden(crapStruct))
        {
            // in case IDEN - first signature present crapStruct.addressSclpHeader_ contains pointer to address of the SCLP header
            Region pointerRegion(crapStruct.addressSclpHeader_, 
                                 crapStruct.addressSclpHeader_ + sizeof(unsigned long));
            
            uchar_vt pointerBuffer;
            pointerBuffer.swap(GetRegion(pointerRegion, interapter, progress));
            unsigned long pointer = *(unsigned long*)&pointerBuffer.front();
            pointer = Algorithm::Revert<unsigned long>(pointer);
            
            return pointer;
        }
        else
        {
            throw iDenException("Unknown Crap Signature", "Plugin.Advice.SendLogs");
        }
    }


    void UploadDumper(const uchar_vt& rawDump)
    {
        using namespace Protocol;
        PortWriter writer(port_);

        bool lowSpeed = false;
        if (!port_.IsUsb())
        {
            FlashStrap::Command waitCommand;
            FlashStrap::PortReader fsReader(port_);

            ::Sleep(3800);
            for (int i = 0; i < 2; ++i)
            {
                int tries = 200;
                do
                {
                    writer.Write(waitCommand.GetBuffer());
                    fsReader.Read(30);
                }
                while(fsReader.GetStatus() != FlashStrap::PortReader::Ok && --tries);

                if (fsReader.GetStatus() == FlashStrap::PortReader::Ok)
                {
                    break;
                }
                port_.SetBaudRate(19200);
                lowSpeed = true;
            }

            if (fsReader.GetStatus() != FlashStrap::PortReader::Ok)
            {
                throw iDenException("Plugin.Error.EstablishFlashStrap", "Plugin.Advice.SendLogs");
            }
        }

        S3Dump::Dump dump(rawDump);

        FlashStrap::Command uploadCommand(dump.GetBinary().size(), dump.GetAddress(), port_.IsUsb());
        FlashStrap::Command dumpCommand(dump.GetBinary(), uploadCommand.GetCurrentCrc());

        writer.Write(uploadCommand.GetBuffer());
        if (lowSpeed)
        {
            ::Sleep(3000);
            port_.SetBaudRate(115200);
            ::Sleep(3000);
        }
        FlashStrap::PortReader reader(port_);

        writer.Write(dumpCommand.GetBuffer());
        reader.Read(300);

        if (reader.GetStatus() != FlashStrap::PortReader::Ok)
        {
            throw iDenException("Plugin.Error.EstablishFlashStrap", "Plugin.Advice.SendLogs");
        }
    }

    uchar_vt GetS3Dump()
    {
        if (port_.IsUsb())
        {
            return Resource().GetBinary(model_.UsbFileName, "DUMPERS");
        }
        else
        {
            return Resource().GetBinary(model_.SerialFileName, "DUMPERS");
        }
    }



    std::string ReadFirmware(Port::IPort& port)
    {
        using namespace Protocol;

        AT::PortReader reader(port);
        PortWriter writer(port);
        AT::ReplyParser parser;

        AT::Command command("I 2");
        writer.Write(command.GetBuffer());
        uchar_vt readBuffer = reader.Read(300);

        if (reader.GetStatus() != AT::PortReader::Ok)
        {
            throw iDenException("Plugin.Error.ReadFirmware", "Plugin.Advice.CheckCable");
        }

        readBuffer = parser.Parse(readBuffer);

        return std::string(readBuffer.begin(), readBuffer.end() - 2);
    }
    ModelConfig CheckFirmware(const std::string& firmware)
    {
        for (int i = 0; i < sizeof(config) / sizeof(config[0]); ++i)
        {
            if (std::equal(config[i].Firmware.begin(), config[i].Firmware.end(), firmware.begin()))
            {
                config[i].ReadFirmware = firmware;
                return config[i];
            }
        }
        throw iDenException("Plugin.Error.Unsupported");
    }
    uchar_vt GetRegion(Region& region, Interrupter& interapter, IProgressIden& progress)
    {
        __tracing_method__(status);
        _TRW2("Reading memory region [0x%08X - 0x%08X]", region.first, region.second);
        
		using namespace Protocol;
        size_t size = region.second - region.first;

        const BLOCK_SIZE = 0x1000;

        I55::PortReader reader(port_);
        PortWriter writer(port_);
        I55::ReplyParser parser;


        size_t count = size / BLOCK_SIZE;

        uchar_vt result;

        _TRW2("Total %d blocks ", count);
        for (size_t i = 0; i <= count; ++i)
        {
            progress.SetPercent(static_cast<int>(i * 100 / (count + 1)));
            if (interapter.DoInterrupt())
            {
//                throw common::AbortException();
            }

            size_t sAddr = i * BLOCK_SIZE + region.first;
            size_t eAddr = (i + 1) * BLOCK_SIZE + region.first;
            eAddr = ((eAddr < region.second) ? eAddr : region.second); 

            _TRW2(" -- Region [0x%08X - 0x%08X] ", sAddr, eAddr);

            uchar_vt addres1(Algorithm::Serialize(Algorithm::Revert<unsigned long>((unsigned long)sAddr)));
            uchar_vt addres2(Algorithm::Serialize(Algorithm::Revert<unsigned long>((unsigned long)eAddr)));

            uchar_vt buffer(addres1);
            buffer.insert(buffer.end(), addres2.begin(), addres2.end());

            I55::Command command(I55::Dump, buffer.begin(), buffer.end());
            writer.Write(command.GetBuffer());
            uchar_vt data = reader.Read(3000);
            if (reader.GetStatus() != I55::PortReader::Ok)
            {
                _TRE("ERROR GET REGION");
                throw iDenException("Plugin.Error.GettingMemoryRegion");
            }
            uchar_vt answer(parser.Parse(data));
            result.insert(result.end(), answer.begin(), answer.end());
        }

        return result;
    }

    void Reboot()
    {
        using namespace Protocol;
        I55::PortReader reader(port_);
        PortWriter writer(port_);
        I55::ReplyParser parser;
        I55::Command command(I55::Reboot);
        writer.Write(command.GetBuffer());
        reader.Read(300);
    }


    void SwitchToFlashStrap(Port::IPort& port)
    {
        using namespace Protocol;
        Direct::PortReader reader(port);
        PortWriter writer(port);

        unsigned char  p2k[] = {0x06, 0x05};
        Direct::Command command(uchar_vt(p2k, p2k + sizeof(p2k)));
        writer.Write(command.GetBuffer());

        reader.Read(400);

        if (reader.GetStatus() != Direct::PortReader::Ok)
        {
            throw iDenException("Plugin.Error.SwitchFlashStrap", "Plugin.Advice.CheckCable");
        }
    }

    ModelConfig Initialize(const std::string& portName)
    {
        using namespace Port;
        Serial <ATModemStrategy> port(portName);
        Utils::InitModemMode(port);

        ModelConfig model = CheckFirmware(ReadFirmware(port));

        Utils::SwitchToDirect(port, true);
        SwitchToFlashStrap(port);

        return model;
    }

    typedef std::pair<size_t, size_t> pair_t;

    ModelConfig model_;

//    static const unsigned int CrapHeaderAddress_ = 0x100000D8;

    PORT  port_; 

    MCUParser mcuParser_;

};


#endif // _DEVICE_H_