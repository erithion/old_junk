#ifndef _PORT_READER_H_
#define _PORT_READER_H_

#include "Types.h"

namespace Protocol
{
	template <typename Port>
	class PortReader
	{
	public:
		PortReader(Port& port)
			: port_(port) 
		{
		}
		uchar_vt Read(unsigned int timeout)
		{
			uchar_vt result;
			uchar_vt part;

			while (port_.Read(part, timeout))
			{
				result.insert(result.end(), part.begin(), part.end());

				if (FindResult(result))
				{
					break;
				}
			}
			return result;
		}
		virtual ~PortReader()
		{
		}
	protected:
		virtual bool FindResult(const uchar_vt& data) = 0;
	private:
		Port& port_;
	};
}

#endif // _PORT_READER_H_