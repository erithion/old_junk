#ifndef _PORT_BASE_STRATEGY_H_
#define _PORT_BASE_STRATEGY_H_

/************************************************************************/
/* Base Strategy - Has to be derived by the all mount point adapters    */
/************************************************************************/
namespace port
{
	typedef enum compatibilityType {Serial = 1, Usb};
	/************************************************************************/
	/* Base class for any strategy                                          */
	/* Introduces a compatibility trait for the                             */
	/* derived adapter classes with compile time checks.                    */
	/* Created in order to prevent using of classes with different traits.  */
	/* Compiles with an error if there were different IDs (traits)          */
	/* For an instance, there is no way to manipulate with a serial device  */
	/* in a manner of the USB device. Thus, in case of having serial mount  */
	/* point, USB reader with DeviceIoControl could not be used             */
	/* See pointSerial, inputSerial and inputUsb for illustration           */
	/************************************************************************/
	template <int CompatibilityId, typename CheckedType> struct strategyBase
	{
	public:
		enum {Value = CompatibilityId};

		template <bool> struct compatibilityTraitsAssert;
		template<> struct compatibilityTraitsAssert<true> {};

		compatibilityTraitsAssert<CompatibilityId == CheckedType::Value> errorIncompatibleStrategyType;
	};
	/************************************************************************/
	/* Partial specialization with void type omits compile time checking    */
	/************************************************************************/
	template <int CompatibilityId> struct strategyBase<CompatibilityId, void>
	{
	public:
		enum {Value = CompatibilityId};
	};
}
#endif // _PORT_BASE_STRATEGY_H_