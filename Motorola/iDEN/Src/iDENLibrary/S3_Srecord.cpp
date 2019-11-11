
#include "S3_Srecord.h"
#include "S3_Record.h"

using namespace S3Dump;

Srecord::Srecord(const uchar_vt& rowData) :
    InputFile(rowData),
    data_count(0),
    garbage_warning(false),
    seen_some_input(false),
    header_seen(false),
    termination_seen(false),
    address_shift(0)
{
}


Srecord::~Srecord()
{
}

int Srecord::read_inner(Record &record)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
        {
            return 0;
        }
        if (c == 'S')
        {
            break;
        }
        if (c == '\n')
        {
            continue;
        }
        if (!garbage_warning)
        {
            throw std::exception("ignoring garbage lines");
            garbage_warning = true;
        }
        for (;;)
        {
            c = get_char();
            if (c < 0)
            {
                return 0;
            }
            if (c == '\n')
            {
                break;
            }
        }
    }
    int tag = get_nibble();
    checksum_reset();
    int line_length = get_byte();
    if (line_length < 1)
    {
        throw std::exception("line length invalid");
    }
    unsigned char buffer[256];
    for (int j = 0; j < line_length; ++j)
    {
        buffer[j] = get_byte();
    }
    int n = checksum_get();
    if (n != 0xFF)
    {
        char text[256];
        sprintf(text, "checksum mismatch (%02X != FF)", n);
        throw std::exception(text);
    }
    if (get_char() != '\n')
    {
        throw std::exception("end-of-line expected");
    }
    --line_length;

    int naddr = 2;
    Record::type_t type = Record::type_unknown;
    switch (tag)
    {
    case 0:
        // header
        type = Record::type_header;
        if (line_length < naddr)
        {
            // Some programs write Very short headers.
            naddr = line_length;
        }
        break;

    case 1:
        // data
        type = Record::type_data;
        break;

    case 2:
        // data
        type = Record::type_data;
        naddr = 3;
        break;

    case 3:
        // data
        type = Record::type_data;
        naddr = 4;
        break;

    case 5:
        // data count
        type = Record::type_data_count;
        //
        // This is documented as having 2 address bytes and
        // no data bytes.  The Green Hills toolchain happily
        // generates records with 4 address bytes.  We cope
        // with this silently.
        //
        if (line_length >= 2 && line_length <= 4)
            naddr = line_length;
        break;

    case 6:
        // data count
        type = Record::type_data_count;
        //
        // Just in case some smarty-pants uses the Green Hills
        // trick, we cope with address size crap the same as S5.
        //
        naddr = 3;
        if (line_length == 4)
            naddr = line_length;
        break;

    case 7:
        // termination
        type = Record::type_start_address;
        naddr = 4;
        break;

    case 8:
        // termination
        type = Record::type_start_address;
        naddr = 3;
        break;

    case 9:
        // termination
        type = Record::type_start_address;
        break;
    }
    if (line_length < naddr)
    {
        char text[256];
        sprintf(text, "data length too short (%d < %d) for data type (%x)", line_length, naddr, tag);
        throw std::exception(text);
    }
    long tmp_addr = Record::decode_big_endian(buffer, naddr);
    if (address_shift && type != Record::type_data_count)
        tmp_addr <<= address_shift;
    record = Record(type, tmp_addr, buffer + naddr, line_length - naddr);
    return 1;
}


int Srecord::read(Record &record)
{
    for (;;)
    {
	if (!read_inner(record))
	{
	    if (!seen_some_input && garbage_warning)
        {
            throw std::exception("file contains no data");
        }
	    if (!header_seen)
	    {
		    throw std::exception("no header record");
		    header_seen = true;
	    }
	    if (data_count <= 0)
        {
            throw std::exception("file contains no data");
        }
	    if (!termination_seen)
	    {
		    throw std::exception("no termination record");
		    termination_seen = true;
	    }
	    return 0;
	}
	seen_some_input = true;
	if (record.get_type() != Record::type_header && !header_seen)
	{
	    throw std::exception("no header record");
	    header_seen = true;
	}
	if
	(
	    record.get_type() != Record::type_start_address
	&&
	    termination_seen
	)
	{
	    throw std::exception("termination record should be last");
	    termination_seen = false;
	}
	switch (record.get_type())
	{
	case Record::type_unknown:
	        throw std::exception("record type not recognized");
	    break;

	case Record::type_header:
	    if (header_seen)
        {
            throw std::exception("redundant header record");
        }
	    if (record.get_address())
	    {
		    throw std::exception("address in header record ignored");
		    record.set_address(0);
	    }
	    header_seen = true;
	    break;

	case Record::type_data:
	    ++data_count;
	    if (record.get_length() == 0)
	    {
		    throw std::exception("empty data record ignored");
		    continue;
	    }
	    break;

	case Record::type_data_count:
	    {
		Record::address_t addr = record.get_address();
		    Record::address_t mask = 0xFFFF;
		    while (addr > mask)
            {
                mask = ~(~mask << 8);
            }
		    mask &= data_count;
		    if (addr != mask)
		    {
                char text[250];
                sprintf(text, "data record count mismatch (file %ld, read %ld)", addr, mask);
                throw std::exception(text);
		    }
	    }
	    continue;

	case Record::type_start_address:
	    if (record.get_length() > 0)
	    {
		    throw std::exception("data in termination record ignored");
		    record.set_length(0);
	    }
	    if (termination_seen)
        {
            throw std::exception("redundant termination record");
        }
	    termination_seen = true;
	    break;
	}
	break;
    }
    return 1;
}


