#ifndef _USB_TYPE_H_
#define _USB_TYPE_H_

#pragma pack (1, 1, 0)

/************************************************************************/
/* For incoming requests                                                */
/************************************************************************/
typedef enum
{
	GET_STATUS = 0,
	CLEAR_FEATURE =	1,
	SET_FEATURE = 3,
	SET_ADDRESS = 5,
	GET_DESCRIPTOR = 6,
	SET_DESCRIPTOR = 7,
	GET_CONFIGURATION = 8,
	SET_CONFIGURATION = 9,
	GET_INTERFACE = 10,
	SET_INTERFACE = 11,
	SYNCH_FRAME = 12
} usbRequest;

typedef struct 
{
	uchar_t        bmRequestType;
	uchar_t        bRequest; // usbRequest Type
	union usbRequestData
	{
		struct
		{
			uchar_t  bDevAddressLo;
			uchar_t  bDevAddressHi;
			ushort_t wIndex;
			ushort_t wLength;
		} usbAddress;
		struct
		{
			// Comes the index for requested descriptor, 
			// for an instance when bDescriptorType == TYPE_STRING_DESCRIPTOR, 
			// bIndex is a type of requested string
			uchar_t  bIndex;
			// Comes the requested descriptor value from usbDescriptorType
			uchar_t  bDescriptorType;
			ushort_t wLanguageID;
			uchar_t  bLengthLo;
			uchar_t  bLengthHi;
		} usbDescriptor;
		struct
		{
			uchar_t  bConfValLo;
			uchar_t  bConfValHi;
			ushort_t wIndex;
			uchar_t  bLengthLo;
			uchar_t  bLengthHi;
		} usbConfiguration;
	} data;
} usbSetupRequest;

// Restoring
#pragma pack (0, 0, 0)
// !!! DO NOT REMOVE
// Byte Swapping is being done in order to satisfy Intel Little-Endian
#pragma pack (1, 1, 1)

/************************************************************************/
/* Descriptors                                                          */
/************************************************************************/
typedef enum
{
	TYPE_DEVICE_DESCRIPTOR = 1,
	TYPE_CONFIGURATION_DESCRIPTOR = 2,
	TYPE_STRING_DESCRIPTOR = 3,
	TYPE_INTERFACE_DESCRIPTOR = 4,
	TYPE_ENDPOINT_DESCRIPTOR = 5
} usbDescriptorType;
// For description of the device
typedef struct 
{
	uchar_t           bLength;
	uchar_t           bDescriptorType; // usbDescriptorType
	ushort_t          bcdUSB;
	uchar_t           bDeviceClass;
	uchar_t           bDeviceSubClass;
	uchar_t           bDeviceProtocol;
	uchar_t           bMaxPacketSize0;
	ushort_t          idVendor;
	ushort_t          idProduct;
	ushort_t          bcdDevice;
	uchar_t           iManufacturer;
	uchar_t           iProduct;
	uchar_t           iSerialNumber;
	uchar_t           bNumConfigurations;
} usbDeviceDescriptor;
// For description of the endpoint
typedef enum
{
	TRANSFER_TYPE_CONTROL = 0,
	TRANSFER_TYPE_ISOCHRONOUS = 1,
	TRANSFER_TYPE_BULK = 2,
	TRANSFER_TYPE_INTERRUPT = 3
} usbEndpointAttr;
// Is applied to bEndpointAddress
typedef enum
{
	DIRECTION_OUTPUT = 0,
	DIRECTION_INPUT = 0x80
} usbEndpointDirection;

typedef struct 
{
	uchar_t             bLength;
	uchar_t             bDescriptorType; // usbDescriptorType
	uchar_t             bEndpointAddress;
	uchar_t             bmAttributes;    // usbEndpointAttr
	ushort_t            wMaxPacketSize;
	uchar_t             bInterval;
} usbEndpointDescriptor;
// For description of the configuration
typedef enum
{
	REMOTE_WAKEUP = 0x20,
	SELF_POWERED = 0x40,
	RESERVED_USB10_BUS_POWERED = 0x80
} usbConfAttr;

typedef struct 
{
	uchar_t       bLength;
	uchar_t       bDescriptorType; // usbDescriptorType
	ushort_t      wTotalLength;
	uchar_t       bNumInterfaces;
	uchar_t       bConfigurationValue;
	uchar_t       iConfiguration;
	uchar_t       bmAttributes;
	uchar_t       MaxPower;
} usbConfigurationDescriptor;
// For description of the interface
typedef struct 
{
	uchar_t       bLength;
	uchar_t       bDescriptorType; // usbDescriptorType
	uchar_t       bInterfaceNumber;
	uchar_t       bAlternateSetting;
	uchar_t       bNumEndpoints;
	uchar_t       bInterfaceClass;
	uchar_t       bInterfaceSubClass;
	uchar_t       bInterfaceProtocol;
	uchar_t       iInterface;
} usbInterfaceDescriptor;

typedef struct 
{
	usbConfigurationDescriptor ConfigDescriptor;
	usbInterfaceDescriptor     InterfaceDescriptor;
	usbEndpointDescriptor      EndpointDescriptor0;
	usbEndpointDescriptor      EndpointDescriptor1;
} usbConfigData;
// For description of the language
typedef enum
{
	US_ENGLISH = 0x0409
} usbLanguageIDValues;

typedef struct 
{
	uchar_t        bLenght;
	uchar_t        bDescriptorType; // usbDescriptorType
	ushort_t       wLANGID0;
} usbLangIdDescriptor;
// For strings
typedef struct 
{
	uchar_t bLenght;
	uchar_t bDescriptorType;
	char    bString[0x10];
} usbManufacturerStringDescriptor;
typedef struct
{
	uchar_t bLenght;
	uchar_t bDescriptorType;
	char    bString[0x22];
} usbProductStringDescriptor;
typedef struct
{
	uchar_t bLenght;
	uchar_t bDescriptorType;
	char    bString[0x10];
} usbSerialNumberStringDescriptor;
typedef struct
{
	uchar_t bLenght;
	uchar_t bDescriptorType;
	char    bString[0x1C];
} usbInterfaceStringDescriptor;

#pragma pack (0, 0, 0)

#endif // _USB_TYPE_H_