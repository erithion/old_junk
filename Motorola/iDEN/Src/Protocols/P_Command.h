#ifndef _COMMAND_H_
#define _COMMAND_H_

namespace Protocol
{
	template <typename Buffer>
    class Command
    {
    public:
        Command()
        {
        }
        Command(const Buffer& rth) : buffer_(rth.buffer_)
        {
        }
        Command(const Buffer& buffer) : buffer_(buffer)
        {
        }
        Command& operator = (const Command& rth)
        {
            Buffer = rth.buffer_;
            return *this;
        }
        Buffer GetBuffer() const
        {
            return Buffer;
        }
        virtual ~Command() = 0;
    protected:
        Buffer buffer_;
    };
}

#endif // _COMMAND_H_