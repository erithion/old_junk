
#include "S3_Dump.h"
#include "S3_Record.h"
#include "S3_Srecord.h"

using namespace S3Dump;

Dump::Dump(const uchar_vt& rowDump)
: address_(0)
{
    using Record::data_t;

    Record  rec;
    Srecord srec(rowDump);
    bool    gotAddr = false;
    while (srec.read(rec) != 0)
    {
        Record::type_t type = rec.get_type();
        switch (type) 
        {
        case Record::type_start_address:
            {
                if (!gotAddr)
                {
                    this->address_ = rec.get_address();
                    gotAddr = true;
                }
            }
            break;
        case Record::type_data:
            {
                const data_t*           data = rec.get_data();
                size_t                  size = (size_t)rec.get_length();

                data_.insert(data_.end(), data, data + size);
                if (!gotAddr)
                {
                    this->address_ = rec.get_address();
                    gotAddr = true;
                }
            }
            break;
        }
    }
    if (!gotAddr)
    {
        throw std::exception ("Invalid S3Record file");
    }
}

const uchar_vt& Dump::GetBinary() const
{
    return data_;
}

unsigned long Dump::GetAddress() const
{
    return address_;
}

Dump::~Dump()
{
}

