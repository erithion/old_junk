#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <map>
#include <string>

/************************************************************************/
/* Types                                                                */
/************************************************************************/
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;

typedef std::vector<uchar> uchar_vt;

static const uchar         constMaxByte = 0xFF;
static const ushort        constMaxWord = 0xFFFF;
static const uint          constMaxDword = 0xFFFFFFFF;
/************************************************************************/
/* The following must be retrieved from SCLP structure                  */
/************************************************************************/
// Corresponds to the value which is decoded headerSclp::aCPEncodedBlocks arrays
static const uchar         constMaxDataBlocks = 0x3F;
// Corresponds to the value of headerSclp::nCPChunkSize
static const uint          constBlockSize = 0x10000;
/************************************************************************/
/*                                                                      */
/************************************************************************/
// Granularity depends on a type of the flash chip, i.e 128, 256, 512 etc.
// i730 has i28F128W18 with 128 bytes granularity
static const               constGranularity = 0x80;
/************************************************************************/
/* Structures for applying within the Intel FDI                         */
/************************************************************************/
struct FDIUnitHeader
{
	typedef enum Status
	{
		HDR_INVALID = 0xF000,
		HDR_INVALIDATING = 0xFC00,
		HDR_VALID = 0xFF00,
		HDR_ALLOCATED = 0xFFC0,
		HDR_ALLOCATING = 0xFFFC,
		HDR_EMPTY = 0xFFFF,
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		HDR_NOTALLOCATED = 0xF3FF,
		HDR_INVALIDATED = 0xFFF0
	};
	typedef enum Attribute
	{
		ATTR_SEQ_TABLE = 0,
		ATTR_DATA_FRAG = 2,
		ATTR_MULTI_INST = 0xC,
		ATTR_GRP_TABLE = 0xD,
		ATTR_SINGL_INST = 0xE,

		ATTRIBUTE_MASK = 0xF
	};
	// unique identity per parameter
	ushort  identifier;
	// sequence table number of instance
	ushort  table_number;
	// in multiples of granularity
	ushort  g_unit_size;
	// in granularities from top of the  block information
	ushort  g_unit_offset_bottom;
	// data type and unit attribute: multiple instance, single instance, sequence table, etc.
	// unit attribute describes what exactly should be expected at the pointed data unit
	//    Single instance is just a data within one or more units
	//
	uchar  type_attribute;
	uchar  _unknown;
	// power-off recovery
	ushort  status;
};
struct FDIEntryTable 
{
	typedef enum Status
	{
		GRP_VALID = 0x80,
		SEQ_VALID = GRP_VALID
	};
	// virtual block number
	ushort block_number;
	// the instance of this frag/table
	ushort entry_instance;
	// replaced entry's logical block #
	ushort old_entry_block;
	// the physical unit header index of the header that needs to be invalidated
	ushort physical_index;
	// in multiples of granularity
	ushort entry_size;
	// table entry status byte - POR
	uchar  entry_status;
	uchar  _reserved;
};
struct FDIBlockInfo
{
	typedef enum State
	{
		// block's data integrity
		BLOCK_INTEGRITY = 0xF0F0,
		// invalidated block data integrity
		BLOCK_INTEGRITY_INVALID = 0xF0C0
	};
	typedef enum Status
	{
		BLK_ERASED = 0xFF,
		BLK_RECOVER = 0xFC,
		BLK_ERASING = 0xF0,
		BLK_UPDATING = 0xC0,
		BLK_WRITE = 0x0
	};
	// number of times erased
	uint erase_count;
	// number of times the reclaim block is erased
	uint physical_erase_count;
	// allows for movable spare block
	ushort  logical_block_number;
	// physical block number being copied during reclamation
	ushort  physical_copy;
	// this will demonstrate block integrity
	ushort  state;
	// reclamation information
	uchar  status;
	// 
	uchar  padding_byte;
};
struct FDIMultiInstance
{
	// size of this instance of the data
	ushort  size_of_instance;
	// number of data instances available in this unit
	ushort  number_of_instances;
};
struct FDIFileInfo 
{
	char   ascii_identifier[32];
	uchar  field_20;
	uchar  type;
	ushort is_locked;
	uint   time;
	uint   date;
	uint   size;
	uint   _2;
	ushort _3;
	ushort file_id;
	uint   _4;
	uint   _5;
	uint   _6;
	uint   _7;
	ushort _8;
	wchar_t unicode_name[33];
};
/************************************************************************/
/* Utils                                                                */
/************************************************************************/
template <class T> T Revert(T x)
{
	T result = 0;
	for (unsigned char i = sizeof(x); i > 0; --i)
	{
		T temp = static_cast<unsigned char>((x >> (8 * (i - 1))) & 0xff);
		result |= temp << ((sizeof(x) - i) * 8);
	}
	return result;
}
template <> FDIUnitHeader Revert(FDIUnitHeader x)
{
	x.status = Revert(x.status);
	x.identifier = Revert(x.identifier);
	x.table_number = Revert(x.table_number);
	x.type_attribute = Revert(x.type_attribute);
	x.g_unit_offset_bottom = Revert(x.g_unit_offset_bottom);
	x.g_unit_size = Revert(x.g_unit_size);

	return x;
}
template <> FDIEntryTable Revert(FDIEntryTable x)
{
	x.block_number = Revert(x.block_number);
	x.entry_instance = Revert(x.entry_instance);
	x.entry_size = Revert(x.entry_size);
	x.entry_status = Revert(x.entry_status);
	x.old_entry_block = Revert(x.old_entry_block);
	x.physical_index = Revert(x.physical_index);

	return x;
}
template <> FDIBlockInfo Revert(FDIBlockInfo x)
{
	x.state = Revert(x.state);
	x.erase_count = Revert(x.erase_count);
	x.logical_block_number = Revert(x.logical_block_number);
	x.padding_byte = Revert(x.padding_byte);
	x.physical_copy = Revert(x.physical_copy);
	x.physical_erase_count = Revert(x.physical_erase_count);
	x.status = Revert(x.status);

	return x;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef enum Error
{
	ERR_NONE = 0,
	ERR_READ = 6,
	HW_ERR_EMPTY_LKUP = 7,
	HW_ERR_SYSTEM = 0xA
};

#define GET_STATUS(s) ((s) & 0xAA)

struct FDILogicalBlock
{
	// the amount of free space in the physical block in multiple DWORDs
	uint  free_space;
	// the amount of invalid space in the physical block in multiple DWORDs
	uint  dirty_space;
	// physical block identifier
	uint   physical_block_number;
	// Number of units of FDIUnitHeader
	uint   number_of_units;
};
struct FDILookupTable 
{
	uint logical_block_number;
	uint header_offset;
	uchar  streamId;
	uchar  existanceAttr;
	uchar field_6;
	uchar field_7;
};
struct FDIOpenStream 
{
	uint block_addr;
	uint tabl_block_addr;
	uint frag_block_addr;
	ushort b_total_size;
	uchar field_E;
	uchar field_F;
	uint  b_unit_size;
	ushort b_hdr_offset;
	uchar _1;
	uchar _2;
	uint b_tabl_hdr_offset;
	uint b_frag_hdr_offset;
	uint b_begin_unit_offset;
	uint b_grp_table_entry_offset;
	uint b_begin_tabl_offset;
	uint b_seq_table_entry_offset;
	uint b_current_file_offset;
	uint b_remaining_in_unit;
	ushort identifier;
	ushort logical_block;
	uchar table_number;
	uchar field_3D;
	uchar type_attribute;
	uchar data_streaming;
};
struct FDICmdCtrl
{
	typedef enum type
	{
		DIRECTORY_INFO_TYPE = 0x02,
		FILE_INFO_TYPE = 0xC,
		FILE_CONTENT_TYPE = 0xD
	};
	uchar* buff_ptr;
	uint required;
	uint offset;
	uint aux;
	uint actual;
	ushort field_12;
	ushort id;
	uchar type;
	uchar priority;
	uchar subcommand;
	uchar reserved;
	uchar field_1A;
	uchar field_1B;
};
// Types of the file system
static const uchar         constMaxIdentifiers = 0xF;
static ushort              fdiSpareBlock = constMaxWord;
static FDILogicalBlock     fdiLogicalTable[constMaxDataBlocks];
static FDILookupTable      fdiLookup[constMaxIdentifiers][0x500];
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool FDIScanBlockInfo(const uchar_vt& v)
{
	for (size_t i = 0, offset = 0; i < constMaxDataBlocks + 1; ++i, offset += constBlockSize)
	{
		FDIBlockInfo blockInfo;
		std::copy(v.begin() + offset + constBlockSize - sizeof FDIBlockInfo, 
			v.begin() + offset + constBlockSize, 
			(uchar*)&blockInfo);

		blockInfo = Revert(blockInfo);

		int block = 0;
		if (blockInfo.state != FDIBlockInfo::BLOCK_INTEGRITY)
		{
			if (fdiSpareBlock == constMaxWord ||
				fdiSpareBlock != i)
			{
				fdiSpareBlock = (ushort)i;
			}
			else
			{
				// error
			}
		}
		else
		{
			if (blockInfo.status == FDIBlockInfo::BLK_ERASING ||
				blockInfo.status == FDIBlockInfo::BLK_UPDATING)
			{
			}
			else if (blockInfo.status == FDIBlockInfo::BLK_WRITE)
			{
				if (blockInfo.logical_block_number < constMaxDataBlocks &&
					blockInfo.logical_block_number != constMaxWord)
				{
					fdiLogicalTable[blockInfo.logical_block_number].physical_block_number = (ushort)i;
				}
			}
			else if (blockInfo.status == FDIBlockInfo::BLK_RECOVER)
			{
			}
			else if (blockInfo.status == FDIBlockInfo::BLK_ERASED)
			{
			}
		}
	}
	for (size_t i = 0; i < constMaxDataBlocks; ++i)
	{
		if (fdiLogicalTable[i].physical_block_number == constMaxDword)
			return false;
	}
	return fdiSpareBlock != constMaxWord;
}
void FDIScanHeaders(const uchar_vt& v)
{
	// Not completely restored because there were a lot of recovery and power loss checks,
	// however still must work quite correctly
	for (uint i = 0; i < constMaxDataBlocks; ++i)
	{
		FDIUnitHeader unitHeader;
		uint        address = fdiLogicalTable[i].physical_block_number * constBlockSize;
		do 
		{
			std::copy(v.begin() + address, v.begin() + address + sizeof FDIUnitHeader, (char*)&unitHeader);
			unitHeader = Revert(unitHeader);

			if (unitHeader.status == FDIUnitHeader::HDR_VALID ||
				unitHeader.status == FDIUnitHeader::HDR_ALLOCATED)
			{
				fdiLogicalTable[i].free_space -= (sizeof FDIUnitHeader + unitHeader.g_unit_size * 0x80) / sizeof uint;

				if (/*(unitHeader.type_attribute & FDIUnitHeader::ATTRIBUTE_MASK) != FDIUnitHeader::ATTR_GRP_TABLE &&*/
					(unitHeader.type_attribute & FDIUnitHeader::ATTRIBUTE_MASK) != FDIUnitHeader::ATTR_DATA_FRAG &&
					(unitHeader.type_attribute & FDIUnitHeader::ATTRIBUTE_MASK) != FDIUnitHeader::ATTR_SEQ_TABLE)
				{
					// Modify Lookup
					uchar index = (unitHeader.type_attribute >> 4) & 0xF;
					fdiLookup[index][unitHeader.identifier].logical_block_number = i;
					uint offset = (address - (fdiLogicalTable[i].physical_block_number * constBlockSize)) / sizeof FDIUnitHeader;
					fdiLookup[index][unitHeader.identifier].header_offset = offset;
					fdiLookup[index][unitHeader.identifier].existanceAttr = 0;
					fdiLookup[index][unitHeader.identifier].streamId = constMaxByte;
				}
			}
			else if ((unitHeader.status | FDIUnitHeader::HDR_NOTALLOCATED) == FDIUnitHeader::HDR_NOTALLOCATED)
			{
				fdiLogicalTable[i].dirty_space += (sizeof FDIUnitHeader + unitHeader.g_unit_size * 0x80) / sizeof uint;
			}
			else if (unitHeader.status == FDIUnitHeader::HDR_INVALIDATING)
			{
				fdiLogicalTable[i].free_space -= (sizeof FDIUnitHeader + unitHeader.g_unit_size * 0x80) / sizeof uint;
				// Search for identifier
				// etc.
			}
			else if (unitHeader.status == FDIUnitHeader::HDR_INVALIDATED)
			{
				fdiLogicalTable[i].dirty_space += (sizeof FDIUnitHeader + unitHeader.g_unit_size * 0x80) / sizeof uint;
				// FlashDevReadHeader, UpdateFreeDirty, SearchForIdentifier
				// etc.
			}
			else if (unitHeader.status == FDIUnitHeader::HDR_ALLOCATING)
			{
				fdiLogicalTable[i].dirty_space += 3;
				// UpdateFreeDirty with 3 bytes, with Write and error
			}
			else if (unitHeader.status == FDIUnitHeader::HDR_EMPTY)
			{
				break;
			}
			else if ((unitHeader.status & FDIUnitHeader::HDR_INVALID) == FDIUnitHeader::HDR_INVALID)
			{
				break;
				// FlashDevWrite with possible break
			}
			else
				break;

			address += sizeof FDIUnitHeader;
		} 
		while(unitHeader.status != FDIUnitHeader::HDR_EMPTY);
		fdiLogicalTable[i].number_of_units = (address - fdiLogicalTable[i].physical_block_number * constBlockSize) / sizeof FDIUnitHeader;
	}
}
void FDIClearTables()
{
	// Initialize the Logic_Block_Table.
	for (size_t i = 0; i < constMaxDataBlocks; ++i)
	{
		fdiLogicalTable[i].free_space = (constBlockSize - sizeof FDIUnitHeader - sizeof FDIBlockInfo) / sizeof(uint);
		fdiLogicalTable[i].dirty_space = 0;
		fdiLogicalTable[i].physical_block_number = constMaxDword;
	}

	// Initialize the data lookup table.
	for (size_t i = 0; i < constMaxIdentifiers; ++i)
	{
		memset((char*)fdiLookup[i], constMaxByte, 0x500 * sizeof FDILookupTable);
	}
}
uint FDILookupToHeader(const uchar_vt& v, 
					   uint& offsetBlockStart,     // start of the block
					   uint& offsetDataStart,      // offset of the real data within a block from its beginning
					   ushort& offsetUnitHeader,   // offset of this unit header from the block start
					   FDIUnitHeader& unitHeader,  // unit header from flash
					   ushort& logicalBlock, 
					   ushort id, 
					   uchar type)
{
	if (fdiLookup[type][id].existanceAttr != constMaxByte ||
		fdiLookup[type][id].logical_block_number != constMaxDword ||
		fdiLookup[type][id].streamId != constMaxByte)
	{
		if (fdiLookup[type][id].logical_block_number == constMaxDword)
		{
			offsetDataStart = constMaxDword;
			offsetBlockStart = constMaxDword;
			offsetUnitHeader = constMaxWord;
			logicalBlock = constMaxWord;
		}
		else
		{
			logicalBlock = fdiLookup[type][id].logical_block_number;
			offsetBlockStart = fdiLogicalTable[logicalBlock].physical_block_number * constBlockSize;
			offsetUnitHeader = fdiLookup[type][id].header_offset * sizeof FDIUnitHeader;
			uint address = offsetBlockStart + offsetUnitHeader;

			std::copy(v.begin() + address, v.begin() + address + sizeof FDIUnitHeader, (char*)&unitHeader);
			unitHeader = Revert(unitHeader);

			offsetDataStart = offsetBlockStart + 
				(constBlockSize - sizeof FDIBlockInfo - unitHeader.g_unit_offset_bottom * constGranularity);
			return ERR_NONE;
		}
	}
	return HW_ERR_EMPTY_LKUP;
}
uint FDIFindInstance(const uchar_vt& v,
					 uint& block_addr_ptr, 
					 uint& header_offset_ptr, 
					 uint& unit_offset_ptr, 
					 ushort identifier,
					 ushort table_number,
					 ushort instance,
					 uchar type,
					 uchar attribute,
					 ushort block,
					 ushort hdr_status)
{
	uint physical_block = fdiLogicalTable[block].physical_block_number;
	block_addr_ptr = physical_block * constBlockSize;
	unit_offset_ptr = 0;

	header_offset_ptr = 0;
	FDIUnitHeader header;
	size_t offs = block_addr_ptr;
	std::copy(v.begin() + offs, v.begin() + offs + sizeof FDIUnitHeader, (uchar*)&header);

	header = Revert(header);

	while((header.status & 0xFAAA) != 0xFAAA)
	{
		if (header.identifier == identifier &&
			header.table_number == table_number &&
			((header.type_attribute >> 4) & 0xF) == type &&
			(header.type_attribute & 0xF) == attribute)
		{
			if ((header.status & 0xFAAA) == 0xFA00)
			{
				if (hdr_status == FDIUnitHeader::HDR_VALID)
					--instance;
			}
			if ((header.status & 0xFAAA) == 0xFA80)
			{
				if (hdr_status == FDIUnitHeader::HDR_ALLOCATED || 
					hdr_status == FDIUnitHeader::HDR_VALID)
					instance = 0;
			}
			if (instance == 0)
			{
				if (header.g_unit_size != 0 &&
					header.g_unit_size != constMaxWord)
				{
					unit_offset_ptr = header.g_unit_offset_bottom;
				}
				break;
			}
		}
		header_offset_ptr += sizeof FDIUnitHeader;
		offs += sizeof FDIUnitHeader;

		std::copy(v.begin() + offs, v.begin() + offs + sizeof FDIUnitHeader, (uchar*)&header);
		header = Revert(header);
	} 


	unit_offset_ptr = constBlockSize - sizeof FDIBlockInfo - unit_offset_ptr * 0x80;
	if (instance != 0)
	{
		return HW_ERR_SYSTEM;
	}
	return ERR_NONE;
}
void FDIGetTableEntry(const uchar_vt& v, 
					 FDIOpenStream& stream, 
					 ushort& grp_invalid_ptr, 
					 ushort& seq_invalid_ptr, 
					 ushort& grp_entries_ptr, 
					 ushort& seq_entries_ptr, 
					 uint offset, 
					 ushort entry_limit)
{
#define MAX_SEQ_ENTRY 0xA

	stream.b_remaining_in_unit = constMaxDword;
	stream.logical_block = constMaxWord;
	stream.table_number = 0;
	stream.b_total_size = 0;
	seq_invalid_ptr = 0;
	grp_invalid_ptr = 0;
	seq_entries_ptr = 0;

	ushort grp_entries = 0;
	ushort seq_entries = 0;
	if (entry_limit >= 1)
	{
		FDIEntryTable table;
		for (grp_entries = 0; grp_entries < entry_limit; ++grp_entries)
		{
			size_t offs = stream.block_addr + stream.b_grp_table_entry_offset;
			std::copy(v.begin() + offs, v.begin() + offs + sizeof FDIEntryTable, (uchar*)&table);

			table = Revert(table);

			if (GET_STATUS(table.entry_status) == 0xAA)
			{
				stream.b_grp_table_entry_offset -= sizeof FDIEntryTable;
				--stream.table_number;
				break;
			}
			if (GET_STATUS(table.entry_status) != FDIEntryTable::GRP_VALID)
			{
				stream.b_grp_table_entry_offset += sizeof FDIEntryTable;
				++grp_invalid_ptr;
				continue;
			}

			uchar type = (stream.type_attribute >> 4) & 0xF;
			grp_entries_ptr = table.block_number;
			seq_entries_ptr = FDIUnitHeader::HDR_VALID;

			FDIFindInstance(v,
				stream.tabl_block_addr,
				stream.b_tabl_hdr_offset,
				stream.b_begin_tabl_offset,
				stream.identifier, 
				0,
				table.entry_instance,
				(stream.type_attribute >> 4) & 0xF,
				FDIUnitHeader::ATTR_SEQ_TABLE, 
				table.block_number, 
				FDIUnitHeader::HDR_VALID);
			stream.b_seq_table_entry_offset = stream.b_begin_tabl_offset;
			seq_invalid_ptr = 0;
			while (seq_entries < MAX_SEQ_ENTRY)
			{
				size_t offs = stream.tabl_block_addr + stream.b_seq_table_entry_offset;
				std::copy(v.begin() + offs, v.begin() + offs + sizeof FDIEntryTable, (uchar*)&table);

				table = Revert(table);

				if (GET_STATUS(table.entry_status) != 0xAA)
				{
					if (GET_STATUS(table.entry_status) != FDIEntryTable::SEQ_VALID || 
						offset == constMaxDword)
					{
						stream.b_seq_table_entry_offset += sizeof FDIEntryTable;
						if (GET_STATUS(table.entry_status) == FDIEntryTable::SEQ_VALID && 
							offset == constMaxDword)
						{
							stream.b_total_size += (table.entry_size * 0x80);
							++grp_invalid_ptr;
						}
						++seq_invalid_ptr;
						++seq_entries;
						continue;
					}
					stream.b_total_size += (table.entry_size * constGranularity);
					if (stream.b_total_size > offset)
					{
						stream.logical_block = table.block_number;
						stream.b_remaining_in_unit = stream.b_total_size - offset;
						stream.b_unit_size = table.entry_size * constGranularity;
						stream.b_total_size -= stream.b_unit_size;
						break;
					}
				}
				else
				{
					stream.b_remaining_in_unit = stream.b_total_size <= offset ? 0 : stream.b_total_size - offset;
					stream.b_seq_table_entry_offset -= sizeof FDIEntryTable;
					break;
				}
				stream.b_seq_table_entry_offset += sizeof FDIEntryTable;
				++seq_entries;
			}
			if (stream.logical_block != constMaxWord)
			{
				break;
			}
			stream.b_grp_table_entry_offset += sizeof FDIEntryTable;
			++stream.table_number;
		}		
	}
	grp_entries_ptr = grp_entries;
	seq_entries_ptr = seq_entries;

	if (grp_entries == entry_limit)
	{
		stream.b_grp_table_entry_offset -= sizeof FDIEntryTable;
		--stream.table_number;
	}
	if (stream.logical_block == constMaxWord &&
		seq_entries == MAX_SEQ_ENTRY)
	{
		stream.b_seq_table_entry_offset -= sizeof FDIEntryTable;
		stream.b_remaining_in_unit = 0;
	}
}

uint FDIRead(const uchar_vt& v, FDICmdCtrl& c)
{
	FDIOpenStream stream;
	FDIUnitHeader unitHeader;
	FDIEntryTable seqTable;
	FDIEntryTable grpTable;
	uint bytes = 0;
	uint unit_data_addr = 0;
	uint actual = 0;
	uint frag_unit_offset = 0;
	uint frag_unit_address = 0;
	ushort grp_size = 0;
	ushort seq_size = 0;
	if (fdiLookup[c.type][c.id].streamId == constMaxByte)
	{
		stream.identifier = c.id;
		uchar tp = c.type;
		if (FDILookupToHeader(v, 
			stream.block_addr, 
			unit_data_addr, 
			stream.b_hdr_offset, 
			unitHeader, 
			stream.logical_block, 
			c.id, c.type) == ERR_NONE)
		{
			stream.type_attribute = unitHeader.type_attribute;
			stream.b_unit_size = unitHeader.g_unit_size * constGranularity;
			stream.b_remaining_in_unit = 0;
			stream.b_current_file_offset = 0;
			stream.b_begin_unit_offset = unit_data_addr - stream.block_addr;

			if ((stream.type_attribute & FDIUnitHeader::ATTRIBUTE_MASK) == FDIUnitHeader::ATTR_SINGL_INST)
			{
				stream.b_current_file_offset = c.offset;
				stream.b_grp_table_entry_offset = constMaxDword;
				if (stream.b_unit_size <= c.offset)
				{
					stream.b_remaining_in_unit = 0;
				}
				else
				{
					stream.b_remaining_in_unit = stream.b_unit_size - c.offset;
				}
			}
			else if ((stream.type_attribute & FDIUnitHeader::ATTRIBUTE_MASK) == FDIUnitHeader::ATTR_GRP_TABLE)
			{
				stream.b_grp_table_entry_offset = stream.b_begin_unit_offset;
				grp_size = stream.b_unit_size;
				ushort temp_word = 0;

				ushort logical_block_temp = stream.logical_block;

				FDIGetTableEntry(v, stream, temp_word, temp_word, temp_word, temp_word, c.offset, grp_size / sizeof FDIEntryTable);
				bytes = stream.b_unit_size;
				stream.b_unit_size = grp_size;

				stream.logical_block = logical_block_temp;
				stream.b_current_file_offset = c.offset;

				unit_data_addr = stream.tabl_block_addr + stream.b_seq_table_entry_offset;

				std::copy(v.begin() + unit_data_addr, v.begin() + unit_data_addr + sizeof FDIEntryTable, (char*)&seqTable);

				seqTable = Revert(seqTable);
			}
		}
	}
	FDIMultiInstance instance;
	size_t inst_number = 0;
	if (fdiLookup[c.type][c.id].logical_block_number != constMaxDword)
	{
		if ((stream.type_attribute & FDIUnitHeader::ATTRIBUTE_MASK) == FDIUnitHeader::ATTR_MULTI_INST)
		{
			stream.b_current_file_offset = c.offset;
			stream.b_grp_table_entry_offset = constMaxDword;
			std::copy(v.begin() + unit_data_addr, v.begin() + unit_data_addr + sizeof FDIMultiInstance, (char*)&instance);

			instance.number_of_instances = Revert(instance.number_of_instances);
			instance.size_of_instance = Revert(instance.size_of_instance);

			stream.b_unit_size = instance.size_of_instance;

			unit_data_addr += sizeof FDIMultiInstance;

			for (inst_number = 0; inst_number < instance.number_of_instances; ++inst_number)
			{
				if ((v[unit_data_addr + inst_number] & 0xAA) == 0xA0)
				{
					break;
				}
			}
			unit_data_addr += instance.number_of_instances;
			unit_data_addr += (stream.b_unit_size * inst_number);
			if (stream.b_unit_size < c.offset)
			{
				stream.b_remaining_in_unit = 0;
			}
			else
			{
				stream.b_remaining_in_unit = stream.b_unit_size - c.offset;
			}
			bytes = c.required;
			if (stream.b_remaining_in_unit < c.required)
			{
				bytes = stream.b_remaining_in_unit;
			}

			actual = bytes;
			std::copy(v.begin() + unit_data_addr + c.offset, v.begin() + unit_data_addr + c.offset + bytes, c.buff_ptr);

			c.actual = actual;
			return ERR_NONE;

		}
		else if ((stream.type_attribute & FDIUnitHeader::ATTRIBUTE_MASK) == FDIUnitHeader::ATTR_SINGL_INST)
		{
			bytes = c.required;
			if (stream.b_remaining_in_unit < c.required)
			{
				bytes = stream.b_remaining_in_unit;
			}
			unit_data_addr = stream.block_addr + stream.b_begin_unit_offset;
			c.actual = bytes;
			std::copy(v.begin() + unit_data_addr + c.offset, v.begin() + unit_data_addr + c.offset + bytes, c.buff_ptr);
			return ERR_NONE;
		}
		else
		{
			if (stream.b_remaining_in_unit != 0)
			{
				FDIFindInstance(v, stream.frag_block_addr, 
					stream.b_frag_hdr_offset,
					frag_unit_offset, 
					c.id,
					stream.table_number,
					seqTable.entry_instance, 
					c.type,
					FDIUnitHeader::ATTR_DATA_FRAG, 
					seqTable.block_number, 
					FDIUnitHeader::HDR_VALID);

			}
			grp_size = stream.b_unit_size -	(stream.b_grp_table_entry_offset - stream.b_begin_unit_offset);
			seq_size = constGranularity - (stream.b_seq_table_entry_offset - stream.b_begin_tabl_offset);
			if (stream.b_remaining_in_unit == 0 &&
				seq_size == 0x14)
			{
				seq_size -= sizeof FDIEntryTable;
			}
			while (grp_size >= sizeof FDIEntryTable)
			{
				while (seq_size >= sizeof FDIEntryTable)
				{
					frag_unit_address = stream.frag_block_addr + frag_unit_offset + 
						(seqTable.entry_size * constGranularity) - stream.b_remaining_in_unit;

					if (actual + stream.b_remaining_in_unit > c.required)
						bytes = c.required - actual;
					else
						bytes = stream.b_remaining_in_unit;

					if (bytes != 0)
					{
						std::copy(v.begin() + frag_unit_address, v.begin() + frag_unit_address + bytes, c.buff_ptr + actual);

						actual += bytes;

						stream.b_current_file_offset += bytes;
						stream.b_remaining_in_unit -= bytes;

					}
					if (actual == c.required)
						break;
					do
					{
						stream.b_seq_table_entry_offset += sizeof FDIEntryTable;

						unit_data_addr += sizeof FDIEntryTable;

						std::copy(v.begin() + unit_data_addr, v.begin() + unit_data_addr + sizeof FDIEntryTable, (uchar*)&seqTable);
						seq_size -= sizeof FDIEntryTable;

						seqTable = Revert(seqTable);
					} 
					while (GET_STATUS(seqTable.entry_status) != 0xAA &&
						GET_STATUS(seqTable.entry_status) != FDIEntryTable::SEQ_VALID &&
						seq_size >= sizeof FDIEntryTable);
					if (seq_size < sizeof FDIEntryTable ||
						GET_STATUS(seqTable.entry_status) == 0xAA)
						break;
					stream.b_remaining_in_unit = seqTable.entry_size * constGranularity;
					FDIFindInstance(v, stream.frag_block_addr, 
						stream.b_frag_hdr_offset,
						frag_unit_offset, 
						c.id,
						stream.table_number,
						seqTable.entry_instance, 
						c.type,
						FDIUnitHeader::ATTR_DATA_FRAG, 
						seqTable.block_number, 
						FDIUnitHeader::HDR_VALID);
				}

				if (actual == c.required)
					break;
				do
				{
					stream.b_grp_table_entry_offset += sizeof FDIEntryTable;

					std::copy(v.begin() + stream.block_addr + stream.b_grp_table_entry_offset, 
						v.begin() + stream.block_addr + stream.b_grp_table_entry_offset + sizeof FDIEntryTable, 
						(uchar*)&grpTable);

					grp_size -= sizeof FDIEntryTable;

					grpTable = Revert(grpTable);
				} 
				while (GET_STATUS(grpTable.entry_status) != 0xAA &&
					GET_STATUS(grpTable.entry_status) != FDIEntryTable::GRP_VALID &&
					grp_size >= sizeof FDIEntryTable);
				if (grp_size < sizeof FDIEntryTable ||
					GET_STATUS(grpTable.entry_status) == 0xAA)
					break;
				++stream.table_number;
				FDIFindInstance(v, stream.tabl_block_addr,
					stream.b_tabl_hdr_offset,
					stream.b_begin_tabl_offset,
					c.id, 
					0, 
					grpTable.entry_instance, 
					c.type,
					FDIUnitHeader::ATTR_SEQ_TABLE, 
					grpTable.block_number, 
					FDIUnitHeader::HDR_VALID);
				stream.b_seq_table_entry_offset = stream.b_begin_tabl_offset;

				unit_data_addr = stream.tabl_block_addr + stream.b_begin_tabl_offset;

				seq_size = constGranularity;
				do
				{
					std::copy(v.begin() + unit_data_addr, v.begin() + unit_data_addr + sizeof FDIEntryTable, (uchar*)&seqTable);

					seqTable = Revert(seqTable);
					if (GET_STATUS(seqTable.entry_status) != 0xAA &&
						GET_STATUS(seqTable.entry_status) != FDIEntryTable::SEQ_VALID)
					{
						seq_size -= sizeof FDIEntryTable;
						stream.b_seq_table_entry_offset += sizeof FDIEntryTable;

						unit_data_addr += sizeof FDIEntryTable;
					}
				} 
				while (GET_STATUS(seqTable.entry_status) != 0xAA &&
					GET_STATUS(seqTable.entry_status) != FDIEntryTable::SEQ_VALID &&
					seq_size >= sizeof FDIEntryTable);
				if (seq_size < sizeof FDIEntryTable ||
					GET_STATUS(seqTable.entry_status) == 0xAA)
					break;
				stream.b_remaining_in_unit = seqTable.entry_size * constGranularity;
				FDIFindInstance(v, stream.frag_block_addr, 
					stream.b_frag_hdr_offset,
					frag_unit_offset, 
					c.id,
					stream.table_number,
					seqTable.entry_instance, 
					c.type,
					FDIUnitHeader::ATTR_DATA_FRAG, 
					seqTable.block_number, 
					FDIUnitHeader::HDR_VALID);
			}

			c.actual = actual;
			return ERR_NONE;
		}
	}
	return ERR_READ;
}
void FDIInit(const uchar_vt& v)
{
	fdiSpareBlock = constMaxWord;
	FDIClearTables();
	if (FDIScanBlockInfo(v))
	{
		FDIScanHeaders(v);
	}
}

#define NAMES_COUNT_OFFSET 0x12F
#define NAME_SIZE 0x28
struct FDIDirSubFileInfo 
{
	char name[0x24];
	uint id;
};
#pragma pack(push, 1)
struct FDIDir 
{
	uint _1;
	uint _2;
	char name[33];
	ushort _3;
	ushort subdir_count;
	ushort subdir_id[129];
	ushort files_count;
	//FDIDirSubFileInfo files_array;
};
typedef std::map<uint, uchar_vt> uchar_mvt;
#pragma pack(pop)
uchar_mvt ReadDirInfo(const uchar_vt& v)
{
	FDICmdCtrl cmd;
	uchar_mvt  dirs; 
	for (ushort i = 0; i < 0x200; ++i)
	{
		uchar buffer[0x10000] = {0};

		cmd.priority = 1;
		cmd.buff_ptr = (uchar*)buffer;
		cmd.required = sizeof buffer;
		cmd.type = FDICmdCtrl::DIRECTORY_INFO_TYPE;
		cmd.offset = 0;
		cmd.id = i;

		// Read body
		if (FDIRead(v, cmd) == ERR_NONE)
		{
			dirs[i] = uchar_vt(buffer, buffer + cmd.actual);
		}
	}
	return dirs;
}
struct File 
{
	FDIFileInfo info;
	uchar_vt    data;
};
typedef std::map<uint, File> file_mt;
file_mt ReadFiles(const uchar_vt& v)
{
	FDICmdCtrl               cmd;
	file_mt                  files;

	for (ushort i = 0; i < 255; ++i)
	{
		File                 file;
		cmd.buff_ptr = (uchar*)&file.info;
		cmd.required = sizeof FDIFileInfo;
		cmd.priority = 1;
		cmd.type = FDICmdCtrl::FILE_INFO_TYPE;
		cmd.offset = 0;
		cmd.id = i;

		if (FDIRead(v, cmd) == ERR_NONE)
		{
			file.info.date = Revert(file.info.date);
			file.info.time = Revert(file.info.time);
			file.info.size = Revert(file.info.size);
			file.info.file_id = Revert(file.info.file_id);

			for (size_t j = 0; j < sizeof file.info.unicode_name / sizeof wchar_t; ++j)
			{
				file.info.unicode_name[j] = Revert(file.info.unicode_name[j]);
			}
			
			// Read body
			uchar* buffer = new uchar[file.info.size];
			cmd.priority = 1;
			cmd.buff_ptr = buffer;
			cmd.required = file.info.size;
			cmd.type = FDICmdCtrl::FILE_CONTENT_TYPE;
			cmd.offset = 0;
			if (FDIRead(v, cmd) == ERR_NONE)
			{
				std::copy(buffer, buffer + cmd.actual, std::back_inserter(file.data));
				uint id = atoi(file.info.ascii_identifier + 1);
				files[id] = file;
			}
			delete[] buffer;
		}
	}

	return files;
}
void CreateDirs(const uchar_vt& root, uchar_mvt& dirs, file_mt& files)
{
	FDIDir d;
	std::copy(root.begin(), root.begin() + sizeof FDIDir, (uchar*)&d);

	d.subdir_count = Revert(d.subdir_count);

	char buf_name[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, buf_name);
	std::string name(buf_name);
	name += "\\";
	name += strcmp(d.name, "c:") == 0 ? "root (C_DISK)" : d.name;
	if (::CreateDirectory(name.c_str(), 0))
	{
		for (ushort i = 0; i < d.subdir_count; ++i)
		{
			::SetCurrentDirectory(name.c_str());
			d.subdir_id[i] = Revert(d.subdir_id[i]);
			uint index = d.subdir_id[i];
			CreateDirs(dirs[index], dirs, files);
			::SetCurrentDirectory(buf_name);
		}

		::SetCurrentDirectory(name.c_str());
		d.files_count = Revert(d.files_count);
		for (ushort j = 0; j < d.files_count; ++j)
		{
			FDIDirSubFileInfo sub_file;
			uint offs = sizeof FDIDir + (j * sizeof FDIDirSubFileInfo);
			std::copy(root.begin() + offs, root.begin() + offs + sizeof FDIDirSubFileInfo, (uchar*)&sub_file);

			sub_file.id = Revert(sub_file.id);

			if (files.count(sub_file.id) != 0)
			{
				char buff[255];
				WideCharToMultiByte(CP_ACP, 0, files[sub_file.id].info.unicode_name, -1, buff, 255, NULL, NULL);
				std::string name(strlen(buff) != 0 ? buff : sub_file.name);

				std::ofstream body(name.c_str(), std::ios_base::binary);
				std::copy(files[sub_file.id].data.begin(), files[sub_file.id].data.end(), std::ostream_iterator<uchar>(body));

			}
		}
		::SetCurrentDirectory(buf_name);
	}
}
void ReadUnknown(const uchar_vt& v)
{
	FDICmdCtrl cmd;

	char buf_name[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, buf_name);

	std::string name(buf_name);
	name += "\\_Unknown_Data_";
	if (::CreateDirectory(name.c_str(), 0))
	{
		::SetCurrentDirectory(name.c_str());
		
		for (ushort i = 0; i < 0x499; ++i)
		{
			uchar buffer[0x10000] = {0};

			cmd.priority = 1;
			cmd.buff_ptr = (uchar*)buffer;
			cmd.required = sizeof buffer;
			cmd.type = 0;
			cmd.offset = 0;
			cmd.id = i;

			// Read body
			if (FDIRead(v, cmd) == ERR_NONE)
			{
				char name[20] = {0};
				sprintf(name, "UNKNOWN_%03d.bin", i);
				std::ofstream body(name, std::ios_base::binary);

				std::copy(buffer, buffer + cmd.actual, std::ostream_iterator<uchar>(body));
			}
		}
		::SetCurrentDirectory(buf_name);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 1)
	{
		std::ifstream f(argv[1], std::ios_base::binary);
		if (f.is_open())
		{
			f.flags(f.flags() & ~std::ios_base::skipws);
			uchar_vt v((std::istream_iterator<unsigned char>(f)), 
				(std::istream_iterator<unsigned char>()));
			FDIInit(v);

			uchar_mvt dirs = ReadDirInfo(v);
			file_mt   files = ReadFiles(v);
			if (!dirs.empty())
			{
				CreateDirs(dirs[0], dirs, files);
			}
			ReadUnknown(v);

			// FILE INFO
			{
				std::ofstream f("content.txt");

				size_t total = 0;

				file_mt::iterator iter = files.begin();
				while (iter != files.end())
				{
					char buff[255];
					WideCharToMultiByte(CP_ACP, 0, iter->second.info.unicode_name, -1, buff, 255, NULL, NULL);
					std::string name(strlen(buff) != 0 ? buff : iter->second.info.ascii_identifier);

					f << "   File Id: " << (int)iter->second.info.file_id << std::endl;
					f << "   ASCII Identifier: " << iter->second.info.ascii_identifier << std::endl;
					f << "   Unicode Name: " << name << std::endl;
					f << "   Size: " << iter->second.info.size << std::endl;
					f << "   Date: " << iter->second.info.date << std::endl;
					f << "   Time: " << iter->second.info.time << std::endl;
					f << std::endl;
					f.flush();

					++total;
					++iter;
				}
				f << std::endl << "   Total: " << (int)total << std::endl;
			}
		}
	}
	return 0;
}

