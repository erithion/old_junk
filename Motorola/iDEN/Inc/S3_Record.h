#ifndef _S3_RECORD_H
#define _S3_RECORD_H

namespace S3Dump
{
    class Record
    {
    public:
        ~Record();
        enum type_t
        {
            type_unknown,
            type_header,
            type_data,
            type_data_count,
            type_start_address
        };

        typedef unsigned long address_t;
        typedef unsigned char data_t;
        Record();
        Record(const Record &);
        Record(type_t);
        Record(type_t, address_t);
        Record(type_t the_type, address_t the_address, const data_t *the_data,
            int the_data_length);
        Record &operator=(const Record &);
        address_t get_address() const { return address; }
        address_t get_address_end() const { return (address + length); }
        void set_address(address_t arg) { address = arg; }
        int get_length() const { return length; }
        void  set_length(int arg)
        {
            if (arg < 0)
            {
                arg = 0;
            }
            if (arg < length)
            {
                length = arg;
            }
        }
        const data_t *get_data() const { return data; }
        type_t get_type() const { return type; }
        static address_t decode_big_endian(data_t *data, int len);
        enum { max_data_length = 255 };

    private:
        type_t type;

        address_t address;

        int length;

        data_t data[max_data_length];
    };
}


#endif // _S3_RECORD_H
