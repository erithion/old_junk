#ifndef _PORT_MOUNT_POINTS_H_
#define _PORT_MOUNT_POINTS_H_
/************************************************************************/
/* In order to reduce header dependencies requires                      */
/*   Default headers:                                                   */
/*      Windows.h                                                       */
/*      String                                                          */
/*   Custom headers:                                                    */
/*      PortBaseStrategy.h                                              */
/*      AutoHandle.h                                                    */
/*      Types.h                                                         */
/*      P2k1.h                                                          */
/************************************************************************/

/************************************************************************/
/* Various Mount Points                                                 */
/* Theirs logical meaning is just to open a device and be able          */
/* to return an exclusive device handle to the caller at any time       */
/* At last one class means one action!!!                                */
/* Copy protected                                                       */
/************************************************************************/
namespace port
{
	/************************************************************************/
	/* Default serial (COM) mount point - opens a serial port               */
	/* with backing up its previous settings and setting up default ones    */
	/************************************************************************/
	class pointSerial : 
		public strategyBase<Serial, void>
	{
	public:
		explicit pointSerial(const std::string& pointName) : handle_(create(pointName)), bckDcb_(state()), bckTmo_(time())
		{
		}

		autoHandle& point()
		{
			return handle_;
		}

		compatibilityType type() const
		{
			return compatibilityType(Value);
		}

		~pointSerial()
		{
			destroy();
		}
	private:
		autoHandle      handle_;
		::DCB           bckDcb_;
		::COMMTIMEOUTS  bckTmo_; 
	private:
		::HANDLE create(const std::string& name)
		{
			return ::CreateFile(name.c_str(), 
				GENERIC_READ | GENERIC_WRITE, 
				0, 
				0, 
				OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
				0);
		}

		void destroy()
		{
			::SetCommMask(handle_, 0);
			::PurgeComm(handle_, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			::EscapeCommFunction(handle_, CLRDTR);

			::SetCommState(handle_, &bckDcb_);
			::SetCommTimeouts(handle_, &bckTmo_);
		}

		::DCB state() const
		{
			::DCB dcb;
			memset(&dcb, 0, sizeof ::DCB);
			dcb.DCBlength = sizeof ::DCB;

			::GetCommState(handle_, &dcb);
			return dcb;
		}

		void state(::DCB dcb) const
		{
			if (::SetCommState(handle_, &dcb) == FALSE ||
				::PurgeComm(handle_, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE ||
				::SetCommMask(handle_, EV_RXCHAR) == FALSE)
			{
				throw iDenException("Mount Points setting Dcb failure");
			}
		}

		::COMMTIMEOUTS time() const
		{
			::COMMTIMEOUTS tm;

			::GetCommTimeouts(handle_, &tm);
			return tm;
		}
		void time(::COMMTIMEOUTS tm) const
		{
			if (::SetCommTimeouts(handle_, &tm) == FALSE)
			{
				throw iDenException("Mount Points setting Timeouts failure");
			}
		}
	private:
		pointSerial();
		pointSerial(const pointSerial&);
		pointSerial& operator =(const pointSerial&);
	};
	/************************************************************************/
	/* Default p2k1 mount point - opens a p2k1 Motorola device              */
	/************************************************************************/
	class pointUsb : 
		public strategyBase<Usb, void>
	{
	public:
		explicit pointUsb(const std::string& pointName) : handle_(p2k1::get())
		{
		}

		autoHandle& point()
		{
			return handle_;
		}

		compatibilityType type() const
		{
			return compatibilityType(Value);
		}

		~pointUsb()
		{
		}
	private:
		autoHandle      handle_;
	private:
		pointUsb();
		pointUsb(const pointUsb&);
		pointUsb& operator =(const pointUsb&);
	};
}
#endif // _PORT_MOUNT_POINTS_H_