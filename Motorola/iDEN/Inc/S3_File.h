#ifndef _S3_FILE_H_
#define _S3_FILE_H_

#include <string>
#include "types.h"

namespace S3Dump
{

    class InputFile
    {
    public:

        virtual ~InputFile();

    protected:

        InputFile(const uchar_vt& rowData);

        virtual int get_char();
        int get_nibble();
        virtual int get_byte();
        int checksum_get() const;
        int checksum_get16() const;
        virtual void checksum_add(unsigned char n);
        void checksum_reset();

    private:

        const uchar_vt& data_;
        uchar_vt::const_iterator dataPointer_;
        int line_number;
        bool prev_was_newline;

    protected:
        int checksum;


    private:

        int getc();
        void ungetc();


        InputFile(const InputFile &);
        InputFile &operator=(const InputFile &);
    };
}

#endif // _S3_FILE_H_
