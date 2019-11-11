#include "S3_File.h"

using namespace S3Dump;

InputFile::InputFile(const uchar_vt& rowData) :
    data_(rowData),
    dataPointer_(data_.begin()),
    line_number(1),
    prev_was_newline(false),
    checksum(0)
{
}


InputFile::~InputFile()
{
}


int InputFile::getc()
{
    if (dataPointer_ != data_.end())
    {
        return *dataPointer_++;
    }
    return EOF;
}

void InputFile::ungetc()
{
    --dataPointer_;
}

int InputFile::get_char()
{
    if (prev_was_newline)
    {
        ++line_number;
    }
    int c = getc();
    if (c == EOF)
    {
	    //
	    // If this is a text file, but the last character wasn't
	    // a newline, insert one.
	    //
	    c = ((!prev_was_newline) ? '\n' : -1);
    }
    else if (c == '\r')
    {
	    //
	    // If this is a text file, turn CRLF into LF.
	    // Leave all other sequences containing CR alone.
	    //
	    c = getc();
	    if (c == EOF)
	    {
	        c = '\r';
	    }
	    else if (c != '\n')
	    {
	        ungetc();
	        c = '\r';
	    }
    }
    prev_was_newline = (c == '\n');
    return c;
}


int
InputFile::get_nibble()
{
    int c = get_char();
    switch (c)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
	    return (c - '0');

    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	    return (c - 'a' + 10);

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
	    return (c - 'A' + 10);
    }
    throw std::exception("hexadecimal digit expected");
    // NOTREACHED
    return -1;
}


int
InputFile::get_byte()
{
    int c1 = get_nibble();
    int c2 = get_nibble();
    int n = ((c1 << 4) | c2);
    checksum_add(n);
    return n;
}


int
InputFile::checksum_get()
    const
{
    return (checksum & 0xFF);
}


int
InputFile::checksum_get16()
    const
{
    return (checksum & 0xFFFF);
}


void
InputFile::checksum_reset()
{
    checksum = 0;
}


void
InputFile::checksum_add(unsigned char n)
{
    checksum += n;
}

