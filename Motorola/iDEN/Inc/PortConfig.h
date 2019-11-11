#ifndef _PORT_CONFIG_H_
#define _PORT_CONFIG_H_
/************************************************************************/
/* In order to reduce header dependencies requires                      */
/*   Default headers:                                                   */
/*      Windows.h                                                       */
/*   Custom headers:                                                    */
/*      PortBaseStrategy.h                                              */
/*      Types.h                                                         */
/************************************************************************/

/************************************************************************/
/* Mount Points' various Input adapter                                  */
/* As in case of the mount points, the recommended meaning of any       */
/* adapter is "One Class - One Action" as well.                         */
/* Adapter is a thing, that being applied to a specific mount point,    */
/* performs some specific action over it                                */
/* Each one adapter from the following group performs:                  */
/*   - a particular configuration for the mount point                   */
/************************************************************************/
namespace port
{
	/************************************************************************/
	/* Default AT Modem configuration for the serial (COM) mount points     */
	/************************************************************************/
	template <typename Point>
	class configSerialAT : 
		public strategyBase<Serial, Point>
	{
	public:
		configSerialAT(Point& point) : point_(point) 
		{
			state(state());
			time(time());
		}
		~configSerialAT()
		{
		}
	private:
		Point& point_;
	private:
		::COMMTIMEOUTS time() const
		{
			::COMMTIMEOUTS result; 
			memset(&result, 0, sizeof ::COMMTIMEOUTS);

			result.ReadIntervalTimeout = 100;
			result.ReadTotalTimeoutConstant = 1000;
			result.ReadTotalTimeoutMultiplier = 5;
			result.WriteTotalTimeoutConstant = 150;
			result.WriteTotalTimeoutMultiplier = 16;

			return result;
		}

		void time(::COMMTIMEOUTS tm) const
		{
			if (::SetCommTimeouts(point_.point(), &tm) == FALSE)
			{
				throw iDenException("Mount Points setting Timeouts failure");
			}
		}
		::DCB state() const
		{
			::DCB result;
			memset(&result, 0, sizeof ::DCB);
			result.DCBlength = sizeof ::DCB;

			result.BaudRate = 19200;
			result.ByteSize = 8;
			result.Parity = NOPARITY; 
			result.StopBits = ONESTOPBIT;
			result.fRtsControl = RTS_CONTROL_ENABLE;
			result.fDtrControl = DTR_CONTROL_ENABLE;
			result.XoffLim = 512;
			result.XonLim = 2048;

			return result;
		}
		void state(::DCB dcb) const
		{
			if (::SetCommState(point_.point(), &dcb) == FALSE ||
				::PurgeComm(point_.point(), PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE ||
				::SetCommMask(point_.point(), EV_RXCHAR) == FALSE)
			{
				throw iDenException("Mount Points setting Dcb failure");
			}
		}
	};
	/************************************************************************/
	/* Default FlashStrap configuration for the serial (COM) mount points   */
	/************************************************************************/
	template <typename Point>
	class configSerialFlashStrap : 
		public strategyBase<Serial, Point>
	{
	public:
		configSerialFlashStrap(Point& point) : point_(point) 
		{
		}
		~configSerialFlashStrap()
		{
		}
	private:
		Point& point_;
	private:
		::COMMTIMEOUTS time()
		{
			::COMMTIMEOUTS result; 
			memset(&result, 0, sizeof ::COMMTIMEOUTS);

			result.ReadIntervalTimeout = 100;
			result.ReadTotalTimeoutConstant = 300;
			result.ReadTotalTimeoutMultiplier = 150;
			result.WriteTotalTimeoutConstant = 150;
			result.WriteTotalTimeoutMultiplier = 15;

			return result;
		}

		void time(::COMMTIMEOUTS tm) const
		{
			if (::SetCommTimeouts(point_.point(), &tm) == FALSE)
			{
				throw iDenException("Mount Points setting Timeouts failure");
			}
		}
		::DCB state()
		{
			::DCB result;
			memset(&result, 0, sizeof ::DCB);
			result.DCBlength = sizeof ::DCB;

			result.BaudRate = 115200;
			result.ByteSize = 8;
			result.Parity = NOPARITY; 
			result.StopBits = ONESTOPBIT;
			result.fRtsControl = RTS_CONTROL_ENABLE;
			result.fDtrControl = DTR_CONTROL_ENABLE;
			result.XoffLim = 512;
			result.XonLim = 2048;

			return result;
		}

		void state(::DCB dcb) const
		{
			if (::SetCommState(point_.point(), &dcb) == FALSE ||
				::PurgeComm(point_.point(), PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE ||
				::SetCommMask(point_.point(), EV_RXCHAR) == FALSE)
			{
				throw iDenException("Mount Points setting Dcb failure");
			}
		}
	};
	/************************************************************************/
	/* Default Baud Rate configuration for the serial (COM) mount points    */
	/************************************************************************/
	template <typename Point>
	class configSerialSpeed : 
		public strategyBase<Serial, Point>
	{
	public:
		configSerialSpeed(Point& point, int baudRate) : point_(point) 
		{
			setSpeed(baudRate);
		}

		~configSerialSpeed()
		{
		}
	private:
		Point& point_;
	private:
		void setSpeed(int baudRate)
		{
			::DCB dcb = state();

			dcb.fOutxDsrFlow = 0;
			dcb.fDtrControl = DTR_CONTROL_ENABLE;
			dcb.fOutxCtsFlow = 0;
			dcb.fRtsControl = RTS_CONTROL_ENABLE;
			dcb.fInX = 0;
			dcb.fOutX = 0;

			state(dcb);

			dcb.BaudRate = baudRate;

			state(dcb);
		}

		::DCB state() const
		{
			::DCB dcb;
			memset(&dcb, 0, sizeof ::DCB);
			dcb.DCBlength = sizeof ::DCB;

			if (::GetCommState(point_.point(), &dcb) == FALSE)
			{
				throw iDenException("Mount Points getting Dcb failure");
			}
			return dcb;
		}

		void state(::DCB dcb) const
		{
			if (::SetCommState(point_.point(), &dcb) == FALSE ||
				::PurgeComm(point_.point(), PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE ||
				::SetCommMask(point_.point(), EV_RXCHAR) == FALSE)
			{
				throw iDenException("Mount Points setting Dcb failure");
			}
		}
	};
}
#endif // _PORT_CONFIG_H_