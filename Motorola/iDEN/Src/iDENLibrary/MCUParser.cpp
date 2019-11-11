// #include "MCUParser.h"
// #include "trace.h"
// #include "tracing.h"
// #include <bitset>
// 
// MCUParser::MCUParser()
// {
// }
// 
// MCUParser::~MCUParser()
// {
// }
// 
// MCUParser::headerCrap MCUParser::GetCrap(const uchar_vt& buffer) const
// {
//     using Algorithm::Revert;
// 
//     if (buffer.size() != sizeof headerCrap )
//     {
//         throw iDenException("CRAP header retrieving has failed");
//     }
// 
//     headerCrap result = *((headerCrap*)&buffer.front());
//     if (!IsCrap(result) && !IsIden(result))
//     {
//         throw iDenException("Unknown Crap Signature", "Plugin.Advice.SendLogs");
//     }
// 
//     result.addressCopyrightString_ = Revert<unsigned int> (result.addressCopyrightString_);
//     result.addressSclpHeader_ = Revert<unsigned int> (result.addressSclpHeader_);
//     result.unk_ = Revert<unsigned int> (result.unk_);
// 
//     return result;
// }
// 
// bool MCUParser::IsCrap(const headerCrap& header) const
// {
//     using std::string;
//     const string crapSignature = "CRAP";
//     string sign(header.signatureCrap1_, sizeof(header.signatureCrap1_));
//     return (sign == crapSignature);
// }
// 
// bool MCUParser::IsIden(const headerCrap& header) const
// {
//     using std::string;
//     const string idenSignature = "IDEN";
//     string sign(header.signatureCrap1_, sizeof(header.signatureCrap1_));
//     return (sign == idenSignature);
// }
// 
// bool MCUParser::IsSclp(const headerSclpMinimal& header) const
// {
//     using std::string;
//     const string sclpSignature = "SCLP";
//     string sign(header.szSignature, sizeof(header.szSignature));
//     return (sign == sclpSignature);
// }
// 
// 
// MCUParser::SCLPType MCUParser::GetSCLPType(const uchar_vt& buffer) const
// {
//     if (buffer.size() < sizeof(headerSclpMinimal))
//     {
//         throw iDenException("SCLP header retrieving has failed");
//     }
// 
//     headerSclpMinimal header = *(headerSclpMinimal*)&buffer.front();
// 
//     const std::string sclpSignature("SCLP");
//     if (!IsSclp(header))
//     {
//         throw iDenException("Unknown SCLP Signature", "Plugin.Advice.SendLogs");
//     }
//     
//     header.nType = Algorithm::Revert<unsigned int>(header.nType);
// 
//     if (header.nType > 6 || header.nType < 1)
//     {
//         throw iDenException("Unknown SCLP Type", "Plugin.Advice.SendLogs");
//     }
// 
//     return (SCLPType)header.nType;
// }
// 
// // The borders are encoded within the array of 8 bit-field values
// // First met value would a first border 
// // Second met value would a last border 
// Region MCUParser::DecodeBlock(const uint_vt& block) const
// {
//     __tracing_method__(status);
// 
//     Region borders(0, 0);
//     
//     for (size_t i = 0; i < block.size(); ++i)
//     {
//         if (block[i] != 0)
//         {
//             std::bitset<sizeof size_t * 8> d(block[i]);
// 
//             unsigned long size = static_cast<unsigned long>(d.size());
//             for (unsigned long j = 0; j < size; ++j)
//             {
//                 // if not empty
//                 if (d.any())
//                 {
//                     unsigned long pos = size - 1 - j;
//                     // Checking whether the first border was already set
//                     // and if the most significant bit is set,
//                     // then all indexes are ready for calculation of the value
//                     if (borders.first == 0 && borders.second == 0 && d.test(pos))
//                     {
//                         borders.first = (i << 5) + j;
//                     }
//                     // Checking whether the last border was already set
//                     // and if the least significant bit is set,
//                     // then all indexes are ready for calculation of the value
//                     else if (borders.first != 0 && borders.second == 0 && !d.test(pos))
//                     {
//                         borders.second = (i << 5) + j - 1;
//                     }
//                 }
//             }
//         }
//     }
//     return borders;
// }
// 
// bool MCUParser::IsEmptyBlockV5(const headerSclpV5::blockSclpV5& block) const
// {
//     return 
//         block.nAddress == 0 && 
//         block.nChunkSize == 0 && 
//         block.nChunksCount == 0 && 
//         *(unsigned int*)block.szFirstName == 0 &&
//         *(unsigned int*)block.szSecondName == 0;
// }
// 
// 
// MCUParser::headerSclpV5::blockSclpV5 MCUParser::FindBlock(const headerSclpV5& header, const std::string& name) const
// {
//     for(size_t i = 0; i < header.blocks.size(); ++i)
//     {
//         if (name == std::string(header.blocks[i].szFirstName, name.size()))
//         {
//             return header.blocks[i];
//         }
//     }
//     throw iDenException("Specified block not found", "Plugin.Advice.SendLogs");
// }
// 
