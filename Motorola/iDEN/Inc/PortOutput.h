#ifndef _PORT_OUTPUT_H_
#define _PORT_OUTPUT_H_
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
/*   - a writing operation to the mount point                           */
/************************************************************************/
namespace port
{
	/************************************************************************/
	/* Default write operation for the serial (COM) mount points            */
	/************************************************************************/
	template <typename Point>
	class outputSerial : 
		public strategyBase<Serial, Point>
	{
	public:
		outputSerial(Point& point) : point_(point), event_(::CreateEvent(0, 1, 0, 0))
		{
			memset(&ovl_, 0, sizeof ::OVERLAPPED);
			ovl_.hEvent = event_;
		}

		bool out(const uchar_vt& in)
		{
			::DWORD size = 0;
			if (!in.empty() &&
				!::WriteFile(point_.point(), &in.front(), (::DWORD)in.size(), &size, &ovl_))
			{
				::DWORD error = ::GetLastError();
				switch (error)
				{
				case ERROR_IO_PENDING:
				case ERROR_IO_INCOMPLETE:
					{
						if (WAIT_OBJECT_0 == ::WaitForSingleObject(ovl_.hEvent, 5000))
						{
							::GetOverlappedResult(point_.point(), &ovl_, &size, TRUE);
							break;
						}
					}
					break;
				default:
					return false;
				}
			}
			if (!in.empty())
			{
				_SERIAL_LOG_SEND((char*)&in.front(), (int)in.size());
			}
			return true;
		}

		~outputSerial()
		{
		}
	private:
		Point&          point_;
		autoHandle      event_;
		::OVERLAPPED    ovl_;
	};
	/************************************************************************/
	/* Default write operation for the p2k1 Motorola mount points           */
	/************************************************************************/
	template <typename Point>
	class outputUsb : 
		public strategyBase<Usb, Point>
	{
	public:
		outputUsb(Point& point) : point_(point)
		{
		}

		bool out(const uchar_vt& in)
		{
			if (in.size())
			{
				_SERIAL_LOG_SEND((char*)&in.front(), (int)in.size());
			}
			return p2k1::write(point_.point(), in);
		}

		~outputUsb()
		{
		}
	private:
		Point& point_;
	};
}

#endif // _PORT_OUTPUT_H_
