#ifndef _P2K1_API_H_
#define _P2K1_API_H_

/************************************************************************/
/* In order to reduce header dependencies requires                      */
/*   Default headers:                                                   */
/*      Windows.h                                                       */
/*   Custom headers:                                                    */
/*      Types.h                                                         */
/************************************************************************/
namespace p2k1
{
	/************************************************************************/
	/* Acquires P2K1 interface handle if it exists within the device list.  */
	/************************************************************************/
	::HANDLE get();
	bool write(::HANDLE handle, const uchar_vt& in);
	uchar_vt read(::HANDLE handle, uint timeout);
}
#endif // _P2K1_API_H_