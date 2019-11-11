#ifndef _PORT_INPUT_H_
#define _PORT_INPUT_H_
/************************************************************************/
/* In order to reduce header dependencies requires                      */
/*   Default headers:                                                   */
/*      Windows.h                                                       */
/*   Custom headers:                                                    */
/*      PortBaseStrategy.h                                              */
/*      AutoHandle.h                                                    */
/*      Types.h                                                         */
/*      P2k1.h                                                          */
/*      Trace.h                                                          */
/************************************************************************/

/************************************************************************/
/* Mount Points' various Input adapter                                  */
/* As in case of the mount points, the recommended meaning of any       */
/* adapter is "One Class - One Action" as well.                         */
/* Adapter is a thing, that being applied to a specific mount point,    */
/* performs some specific action over it                                */
/* Each one adapter from the following group performs:                  */
/*   - a reading operation from the mount point                         */
/************************************************************************/
namespace port
{
	/************************************************************************/
	/* Default read operation for the serial (COM) mount points             */
	/************************************************************************/
	template <typename Point>
	class inputSerial : 
		public strategyBase<Serial, Point>
	{
	public:
		inputSerial(Point& point) : point_(point), event_(::CreateEvent(0, 1, 0, 0))
		{
			memset(&ovl_, 0, sizeof ::OVERLAPPED);
			ovl_.hEvent = event_;
		}

		bool in(uchar_vt& out, uint timeout)
		{
			::DWORD size = 0;
			::DWORD i = ::GetTickCount();
			
			while ((::GetTickCount() - i) < timeout && 
				   size == 0)
			{
				::COMSTAT stat;
				::DWORD   status = 0;
				memset(&stat, 0, sizeof ::COMSTAT);

				if (::ClearCommError(point_.point(), &status, &stat) == FALSE)
				{
					_TRW2("ClearCommError fails : ");
					_TRW2("Error: %d", ::GetLastError());
					return false;
				}
				else
				{
					size = stat.cbInQue;
					if (0 == size) 
					{
						::Sleep(10);
					}
				}
			}
			if (size > 0)
			{
				out.resize(size);
				if(::ReadFile(point_.point(), &out.front(), (::DWORD)out.size(), &size, &ovl_) == FALSE)
				{
					::GetOverlappedResult(point_.point(), &ovl_, &size, TRUE);
				}
				out.resize(size);
				
				if (out.size())
				{
					_SERIAL_LOG_RECIEVE((char*)&out.front(), (int)out.size());
				}
				return true;
			}
			return false;
		}

		~inputSerial()
		{
		}
	private:
		Point&          point_;
		autoHandle      event_;
		::OVERLAPPED    ovl_;
	};
	/************************************************************************/
	/* Default read operation for the p2k1 Motorola mount points            */
	/************************************************************************/
	template <typename Point>
	class inputUsb : 
		public strategyBase<Usb, Point>
	{
	public:
		inputUsb(Point& point) : point_(point)
		{
		}

		bool in(uchar_vt& out, uint timeout)
		{
			out.swap(p2k1::read(point_.point(), timeout));

			if (!out.empty())
			{
				_SERIAL_LOG_SEND((char*)&out.front(), (int)out.size());
				return true;
			}
			return false;
		}

		~inputUsb()
		{
		}
	private:
		Point& point_;
	};
}

#endif // _PORT_INPUT_H_
