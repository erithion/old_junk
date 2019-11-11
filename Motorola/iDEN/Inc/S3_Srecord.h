#ifndef _S3_SRECORD_H_
#define _S3_SRECORD_H_

#include "S3_File.h"
#include "S3_Record.h"

namespace S3Dump
{

    class Srecord : public InputFile
    {
    public:

        virtual ~Srecord();

        Srecord(const uchar_vt& rowData);
        int read(Record &);
        const char *get_file_format_name() const;

    private:

        unsigned long data_count;
        int read_inner(Record &);
        bool garbage_warning;
        bool seen_some_input;
        bool header_seen;
        bool termination_seen;
        unsigned address_shift;
        Srecord();
        Srecord(const Srecord &);
        Srecord &operator=(const Srecord &);
    };

}

#endif // _S3_SRECORD_H_
