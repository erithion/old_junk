#ifndef _MCU_PARSER_H_
#define _MCU_PARSER_H_

#include "Types.h"
#include "Endianess.h"
#include "iDenException.h"
#include "tracing.h"
#include <algorithm>
#include <functional>



class MCUParser
{
public:

    enum SCLPType
    {
        SCLPUnknown = 0,
        SCLPV1 = 1,
        SCLPV2 = 2,
        SCLPV3 = 3,
        SCLPV4 = 4,
        SCLPV5 = 5,
        SCLPV6 = 6
    };


    ///************************************************************************/
    ///* Describes "CRAP" header, which is stored at the start of MCU area    */
    ///************************************************************************/
    struct headerCrap
    {
        // Must contain CRAP word or IDEN word
        char         signatureCrap1_[4];
        // Must contain CRAP word or IDEN word
        char         signatureCrap2_[4];
        // Address points on to Copyright string within the MCU
        unsigned int addressCopyrightString_;
        unsigned int unk_;
        // Address points on to Sclp header within the MCU
        unsigned int addressSclpHeader_;

    };


    /************************************************************************/
    /* Describes "SCLP" header of MCU area                                  */
    /************************************************************************/
    struct headerSclpMinimal 
    {
        unsigned int nType;
        char         szSignature[4];
        unsigned int _unk1;
        unsigned int _unk2;
    };

    struct headerSclpV1
    {
        headerSclpMinimal minimal;
        unsigned int nCPChunkSize;
        unsigned int nCPAddress;
        unsigned int aCPEncodedBlocks[8];
        unsigned int nBCKPAddress;
        unsigned int aBCKPEncodedBlocks[8];
        unsigned int nCODEAddress;
        unsigned int aCODEEncodedBlocks[8];
    };

    // Applicable when SCLP type is 2
    struct headerSclpV2 
    {
        headerSclpMinimal minimal;
        unsigned int nCPChunkSize;
        unsigned int nCPAddress;
        unsigned int aCPEncodedBlocks[8];
        unsigned int nBCKPAddress;
        unsigned int aBCKPEncodedBlocks[8];
        unsigned int nCODEAddress;
        unsigned int aCODEEncodedBlocks[8];
        unsigned int nCSDChunkSize;
        unsigned int nCSDAddress;
        unsigned int nMMAPAddress;
        unsigned int aMMAPEncodedBlocks[8];
    };

    // Applicable when SCLP type is 3
    struct headerSclpV3 
    {
        headerSclpMinimal minimal;
        unsigned int nCPChunkSize;
        unsigned int nCPAddress;
        unsigned int aCPEncodedBlocks[8];
        unsigned int nBCKPAddress;
        unsigned int aBCKPEncodedBlocks[8];
        unsigned int nCODEAddress;
        unsigned int aCODEEncodedBlocks[8];
        unsigned int nCSDChunkSize;
        unsigned int nCSDAddress;
        unsigned int nMMAPAddress;
        unsigned int aMMAPEncodedBlocks[8];
        unsigned int nDAVAddress;
        unsigned int aDAVEncodedBlocks[8];
        unsigned int nUnk2Address;
        unsigned int aUnk2EncodedBlocks[8];
        unsigned int nUnk3Address;
        unsigned int aUnk3EncodedBlocks[8];
    };

    // Applicable when SCLP type is 4
    struct headerSclpV4 
    {
        headerSclpMinimal minimal;
        unsigned int nCPChunkSize;
        unsigned int nCPAddress;
        unsigned int aCPEncodedBlocks[16];
        unsigned int nBCKPAddress;
        unsigned int aBCKPEncodedBlocks[16];
        unsigned int nCODEAddress;
        unsigned int aCODEEncodedBlocks[16];
        unsigned int nCSDChunkSize;
        unsigned int nCSDAddress;
        unsigned int nMMAPAddress;
        unsigned int aMMAPEncodedBlocks[16];
        unsigned int nDAVAddress;
        unsigned int aDAVEncodedBlocks[16];
        unsigned int nUnk2Address;
        unsigned int aUnk2EncodedBlocks[16];
        unsigned int nUnk3Address;
        unsigned int aUnk3EncodedBlocks[16];
        unsigned int nFPAddress;
        unsigned int aFPEncodedBlocks[16];
    };

    // Applicable when SCLP type is 5
    struct headerSclpV5 
    {
        struct blockSclpV5 
        {
            char         szFirstName[4];
            char         szSecondName[4];
            unsigned int nAddress;
            unsigned int nChunkSize;
            unsigned int nChunksCount;
        };

        typedef std::vector<blockSclpV5> block_vt;
        headerSclpMinimal minimal;
        block_vt blocks;
    };


    MCUParser();
    headerCrap GetCrap(const uchar_vt& buffer) const;
    bool IsCrap(const headerCrap& header) const;
    bool IsIden(const headerCrap& header) const;
    bool IsSclp(const headerSclpMinimal& header) const;
    SCLPType GetSCLPType(const uchar_vt& buffer) const;
    Region DecodeBlock(const uint_vt& block)  const;
    bool IsEmptyBlockV5(const headerSclpV5::blockSclpV5& block) const;
    headerSclpV5::blockSclpV5 FindBlock(const headerSclpV5& header, const std::string& name) const;

    template <class SCLPHeader>
    SCLPHeader GetSCLPHeader(const uchar_vt& buffer)  const
    {
        __tracing_method__(status);
        using std::transform;
        using std::ptr_fun;
        using Algorithm::Revert;

        if (buffer.size() < sizeof(SCLPHeader))
        {
            throw iDenException("SCLP header retrieving has failed");
        }

        SCLPHeader result = *(SCLPHeader*)&buffer.front();

        unsigned int *forRevert = (unsigned int*)&result;

        const size_t start = sizeof(headerSclpMinimal) / sizeof(unsigned int);
        const size_t end = sizeof(SCLPHeader) / sizeof(unsigned int);

        for(size_t i = start; i < end; ++i)
        {
            forRevert[i] = Revert<unsigned int>(forRevert[i]);
        }
        //transform(forRevert + start, 
        //          forRevert + start + end,
        //          forRevert + start,
        //          ptr_fun());

        return result;
    }

   
    template <>
    headerSclpV5 GetSCLPHeader<headerSclpV5>(const uchar_vt& buffer)  const
    {
        using std::transform;
        using std::ptr_fun;
        using Algorithm::Revert;

        headerSclpV5 result;
        if (buffer.size() < sizeof(headerSclpMinimal) + sizeof(headerSclpV5::blockSclpV5)
            || (((buffer.size() - sizeof(headerSclpMinimal)) % sizeof(headerSclpV5::blockSclpV5)) != 0))
        {
            throw iDenException("SCLP header retrieving has failed");
        }

        result.minimal = *(headerSclpMinimal*)&buffer.front();
        headerSclpV5::blockSclpV5 *block = (headerSclpV5::blockSclpV5*)(&buffer.front() + sizeof(headerSclpMinimal));
        while(!IsEmptyBlockV5(*block))
        {
            headerSclpV5::blockSclpV5 blockToAdd = *block;
            blockToAdd.nAddress = Revert<unsigned int>(blockToAdd.nAddress);
            blockToAdd.nChunksCount = Revert<unsigned int>(blockToAdd.nChunksCount);
            blockToAdd.nChunkSize = Revert<unsigned int>(blockToAdd.nChunkSize);
            result.blocks.push_back(blockToAdd);
            ++block;
        }


        //SCLPHeader result = *(SCLPHeader*)&buffer.front();

        //unsigned int *forRevert = (unsigned int*)&result;

        //const size_t start = sizeof(headerSclpMinimal) / sizeof(unsigned int);
        //const size_t end = sizeof(SCLPHeader) / sizeof(unsigned int);

        //transform(forRevert + start, 
        //    forRevert + start + end,
        //    forRevert + start,
        //    ptr_fun(Revert<unsigned int>));

        return result;
    }

    ~MCUParser();
};

#endif //_MCU_PARSER_H_