#include "Hardware.h"
#include "USBType.h"
#include "Int.h"
#include "USB.h"

#pragma section CODE

#define CONF_INITIAL 0
#define CONF_UNCONFIGURED 1
#define CONF_CONFIGURED 2
static unsigned char vUsbConfiguration;

static rxHandler vRxHandler = 0;

/************************************************************************/
/* USB descriptors accordingly to USB spec.                             */
/************************************************************************/
typedef enum
{
	LANG_ID = 0,
	MANUFACTURER = 1,
	PRODUCT = 2,
	SERIAL_NUMBER = 3,
	CONFIGURATION = 4,
	INTERFACE = 5
} 
usbStringDescriptorRequest;

static usbDeviceDescriptor vDevice = 
{
	sizeof (usbDeviceDescriptor),  // Length
	TYPE_DEVICE_DESCRIPTOR,        // Descriptor Type
	0x110,                         // bcdUSB
	0,                             // Device Class
	0,                             // Device Sub Class
	0,                             // Device Protocol
	8,                             // Max Packet Size
	0xC44,                         // ID Vendor
	0x22,                          // ID Product
	0x1,                           // bcdDevice
	MANUFACTURER,                  // Manufacturer Index
	PRODUCT,                       // Product Index
	SERIAL_NUMBER,                 // Serial Number Index
	1                              // Total Device Configurations Number
};
static usbConfigData vConfig = 
{
	// usbConfigurationDescriptor
	{
		sizeof (usbConfigurationDescriptor),            // Length
		TYPE_CONFIGURATION_DESCRIPTOR,                  // Descriptor Type
		0x20,                                           // Total Length
		1,                                              // Total Interfaces Number
		1,                                              // Configuration Value
		CONFIGURATION,                                  // Configuration Index
		SELF_POWERED | RESERVED_USB10_BUS_POWERED,      // Attributes
		5                                               // Max Voltage Power
	},
	// usbInterfaceDescriptor
	{
		sizeof (usbInterfaceDescriptor),                // Length
		TYPE_INTERFACE_DESCRIPTOR,                      // Descriptor Type
		0,                                              // Interface Number
		0,                                              // Alternate Interface Number to Switch to
		2,                                              // Total Number of Endpoints
		0xFF,                                           // Interface Class
		0xA,                                            // Interface Sub Class
		0xFF,                                           // Interface Protocol
		INTERFACE                                       // Interface Index
	},
	// usbEndpointDescriptor 1
	{
		sizeof (usbEndpointDescriptor),                 // Length
		TYPE_ENDPOINT_DESCRIPTOR,                       // Descriptor Type
		DIRECTION_OUTPUT | 1,                           // Direction and Endpoint Number
		TRANSFER_TYPE_BULK,                             // Transfer Type
		0x10,                                           // Max Packet Size
		0                                               // Interval
	},
	// usbEndpointDescriptor 1
	{
		sizeof (usbEndpointDescriptor),                 // Length
		TYPE_ENDPOINT_DESCRIPTOR,                       // Descriptor Type
		DIRECTION_INPUT | 2,                            // Direction and Endpoint Number
		TRANSFER_TYPE_BULK,                             // Transfer Type
		0x10,                                           // Max Packet Size
		0                                               // Interval
	}
};
static usbLangIdDescriptor vLangId = 
{
	sizeof (usbLangIdDescriptor),                       // Length
	TYPE_STRING_DESCRIPTOR,                             // Descriptor Type
	US_ENGLISH                                          // Language ID
};
#define MOTOROLA "M\0o\0t\0o\0r\0o\0l\0a\0"
static usbManufacturerStringDescriptor vManufacturer = 
{
	sizeof (usbManufacturerStringDescriptor),           // Length
	TYPE_STRING_DESCRIPTOR,                             // Descriptor Type
	MOTOROLA                                            // String
};
#define FLASH_P2K_PATRIOT "F\0l\0a\0s\0h\0 \0P\0\x32\0K\0 \0P\0a\0t\0r\0i\0o\0t\0"
static usbProductStringDescriptor vProduct = 
{
	sizeof (usbProductStringDescriptor),                // Length
	TYPE_STRING_DESCRIPTOR,                             // Descriptor Type
	FLASH_P2K_PATRIOT                                   // String
};
#define D83_00_0 "D\0\x38\0\x33\0.\0\x30\0\x30\0.\0\x30\0"
static usbSerialNumberStringDescriptor vSerialNumber = 
{
	sizeof (usbSerialNumberStringDescriptor),           // Length
	TYPE_STRING_DESCRIPTOR,                             // Descriptor Type
	D83_00_0                                            // String
};
#define MOTOROLA_FLASH "M\0o\0t\0o\0r\0o\0l\0a\0 \0F\0l\0a\0s\0h\0"
static usbInterfaceStringDescriptor vInterface = 
{
	sizeof (usbInterfaceStringDescriptor),              // Length
	TYPE_STRING_DESCRIPTOR,                             // Descriptor Type
	MOTOROLA_FLASH                                      // String
};

/************************************************************************/
/* Utility                                                              */
/************************************************************************/
// Writes the specified data into address by correct words
void WriteWordAlignedAddress(volatile void* address, const void* data, uchar_t size)
{
	uchar_t            pairedSize = size / 2;
	uchar_t            remainedSize = size % 2;
	uchar_t            i = 0;
	volatile ushort_t* out = address;
	const ushort_t*    in = data;

	while (i < pairedSize)
	{
		out[i] = in[i];
		++i;
	}
	if (remainedSize != 0)
	{
		const uchar_t* in = data;
		ushort_t       word = (in[i * 2]) << 8;
		out[i] = word;
	}
}
// Resets pending EP0 Out                                               */
void usbEP0Activate()
{
	USB_EP0_OUTPUT_STATUS &= 0xFFF0;
	USB_EP0_OUTPUT_STATUS = USB_EP0_OUTPUT_STATUS;
	USB_EP0_OUTPUT_STATUS |= USB_ENABLE;
	USB_EP0_OUTPUT_STATUS |= USB_ACTIVE;
}
// Serves as an interrupt handler as well
void usbEP0Reset()
{
	USB_EP0_INPUT_STATUS |= 0x2600;
	USB_EP0_OUTPUT_STATUS |= (USB_ENABLE | 0x200);
	USB_EP1_INPUT_STATUS |= USB_ENABLE;
	USB_EP1_OUTPUT_STATUS |= USB_ENABLE;
	USB_RESET |= 0x4000;
	USB_RESET |= 0x79;
	vUsbConfiguration = CONF_INITIAL;
}
/************************************************************************/
/* Writers                                                              */
/************************************************************************/
// To default endpoint 0, max size is 8
void usbEP0Write(void* data, uint_t size, uint_t maxSize)
{
	uint_t   segmentsCount = size >= maxSize ? maxSize / 8 : size / 8;
	uint_t   remainedCount = size >= maxSize ? maxSize % 8 : size % 8;
	uint_t   i = 0;
	uchar_t* out = data;

	while (i < segmentsCount)
	{
		WriteWordAlignedAddress(USB_EP0_OUTPUT, 
			                    &out[i * 8], 
								8);
		++i;

		USB_EP0_OUTPUT_STATUS &= 0xFFF0;
		USB_EP0_OUTPUT_STATUS |= 8;
		USB_EP0_OUTPUT_STATUS |= USB_ENABLE;
		USB_EP0_OUTPUT_STATUS |= USB_ACTIVE;

		while ((USB_CONTROL & USB_EP0_OUT) == 0);

		if (USB_CONTROL & USB_ERROR)
		{
			usbEP0Reset();
			return;
		}

		if ((USB_CONTROL & USB_EP0_IN) &&
			(USB_EP0_INPUT_STATUS & 0xF) == 0)
		{
			USB_EP0_OUTPUT_STATUS &= ~USB_ACTIVE;
			return;
		}
	}
	if (remainedCount != 0)
	{
		WriteWordAlignedAddress((void*)USB_EP0_OUTPUT, &out[i * 8], remainedCount);

		USB_EP0_OUTPUT_STATUS &= 0xFFF0;
		USB_EP0_OUTPUT_STATUS |= remainedCount;
		USB_EP0_OUTPUT_STATUS |= USB_ENABLE;
		USB_EP0_OUTPUT_STATUS |= USB_ACTIVE;
	}
	else
	{
		usbEP0Activate();
	}
	while ((USB_CONTROL & USB_EP0_OUT) == 0);
}
void usbEP1MissCycle(unsigned char time)
{
	unsigned char t2 = time / 8;
	time -= t2;
	while (time != 0)
	{
		asm("mov r2, r2");
		asm("mov r2, r2");
		asm("mov r2, r2");
		asm("mov r2, r2");
		asm("mov r2, r2");
		asm("mov r2, r2");
		--time;
	}
}
// To endpoint 1, max size is 31
uint_t usbEP1Write(const void* data, uint_t size)
{
	if (vUsbConfiguration == CONF_CONFIGURED)
	{
		uchar_t packetSize = (size & 0x1F);
		
		while (1)
		{
			if ((USB_CONTROL & USB_EP1_OUT) != 0) break;
			if ((USB_EP1_OUTPUT_STATUS & USB_ACTIVE) != 0) 
			{
				// usbEP1MissCycle(1);
			}
			else break;
		}
		WriteWordAlignedAddress(USB_EP1_OUTPUT, data, packetSize);

		USB_EP1_OUTPUT_STATUS &= 0xFFE0;
		USB_EP1_OUTPUT_STATUS |= packetSize;

		USB_EP1_OUTPUT_STATUS |= USB_ENABLE;
		USB_EP1_OUTPUT_STATUS |= USB_ACTIVE;

		return packetSize;
	}
	return 0;
}
/************************************************************************/
/* Readers                                                              */
/************************************************************************/
// From default endpoint 0, handles all system requests
void usbEP0In()
{
	if ((USB_EP0_INPUT_STATUS & USB_BUSY) == 0)
	{
		// If there was an interrupt
		if (USB_CONTROL & USB_EP0_IN)
		{
			usbSetupRequest request;
			uint_t          i = 0;
			uchar_t*        ptr = (uchar_t*)&request;
			// Getting Packet Size
			uchar_t size = USB_EP0_INPUT_STATUS & 0xF;

			if (size == 0)
			{
				USB_EP0_INPUT_STATUS |= USB_ENABLE;
				return;
			}

			while (i < sizeof (usbSetupRequest))
			{
				ptr[i] = USB_EP0_INPUT[i];
				++i;
			}
			USB_EP0_INPUT_STATUS |= USB_ENABLE;

			switch (request.bRequest)
			{
			case GET_DESCRIPTOR:
				{
					usbDescriptorType type = request.data.usbDescriptor.bDescriptorType;
					ushort_t          length = (request.data.usbDescriptor.bLengthHi << 8) | request.data.usbDescriptor.bLengthLo;

					switch (type)
					{
					case TYPE_DEVICE_DESCRIPTOR:
						{
							usbEP0Write(&vDevice, sizeof (usbDeviceDescriptor), length); 
							return;
						}
						break;
					case TYPE_CONFIGURATION_DESCRIPTOR:
						{
							usbEP0Write(&vConfig, sizeof (usbConfigData), length); 
							return;
						}
						break;
					case TYPE_STRING_DESCRIPTOR:
						{
							usbStringDescriptorRequest type = request.data.usbDescriptor.bIndex;

							switch (type)
							{
							case LANG_ID:
								{
									usbEP0Write(&vLangId, vLangId.bLenght, length); 
									return;
								}
								break;
							case MANUFACTURER:
								{
									usbEP0Write(&vManufacturer, vManufacturer.bLenght, length); 
									return;
								}
								break;
							case PRODUCT:
								{
									usbEP0Write(&vProduct, vProduct.bLenght, length); 
									return;
								}
								break;
							case SERIAL_NUMBER:
								{
									usbEP0Write(&vSerialNumber, vSerialNumber.bLenght, length); 
									return;
								}
								break;
							case INTERFACE:
								{
									usbEP0Write(&vInterface, vInterface.bLenght, length); 
									return;
								}
								break;
							}
						}
						break;
					}
				}
				break;
			case SET_ADDRESS:
				{
					ushort_t address = (request.data.usbAddress.bDevAddressLo) |
						(request.data.usbAddress.bDevAddressHi << 8);

					if (request.data.usbAddress.wIndex == 0 &&
						request.data.usbAddress.wLength == 0 &&
						address < 255)
					{
						switch (vUsbConfiguration)
						{
						case CONF_INITIAL:
							{
								usbEP0Activate();

								while ((USB_CONTROL & USB_EP0_OUT) == 0);

								if (address != 0)
								{
									USB_DEVICE_ADDRESS |= (address & 0x7F);
									vUsbConfiguration = CONF_CONFIGURED;
								}
								return;
							}
							break;
						case CONF_UNCONFIGURED:
							{
								usbEP0Activate();

								while ((USB_CONTROL & USB_EP0_OUT) == 0);

								if (address != 0)
								{
									USB_DEVICE_ADDRESS |= (address & 0x7F);
								}
								else
								{
									USB_DEVICE_ADDRESS = USB_DEVICE_ADDRESS;
									vUsbConfiguration = CONF_INITIAL;
								}
								return;
							}
							break;
						case CONF_CONFIGURED:
							{
								usbEP0Activate();

								while ((USB_CONTROL & USB_EP0_OUT) == 0);

								if (address == 0)
								{
									USB_DEVICE_ADDRESS = USB_DEVICE_ADDRESS;
									vUsbConfiguration = CONF_INITIAL;
									return;
								}
							}
							break;
						default: return;
						}
					}
				}
				break;
			case GET_CONFIGURATION:
				{
					if (vUsbConfiguration == CONF_CONFIGURED &&
						request.data.usbConfiguration.bConfValHi == 0 &&
						request.data.usbConfiguration.bConfValLo == 0 &&
						request.data.usbConfiguration.wIndex == 0 &&
						request.data.usbConfiguration.bLengthLo == 1)
					{
						usbEP0Write(&vConfig.ConfigDescriptor.bConfigurationValue, 
							sizeof (vConfig.ConfigDescriptor.bConfigurationValue),
							request.data.usbConfiguration.bLengthLo);
						return;
					}
				}
				break;
			case SET_CONFIGURATION:
				{
					if (request.data.usbConfiguration.wIndex == 0 &&
						request.data.usbConfiguration.bLengthLo == 0 &&
						request.data.usbConfiguration.bLengthHi == 0 &&
						request.bmRequestType == 0 &&
						request.data.usbConfiguration.bConfValHi)
					{
						uchar_t confValue = request.data.usbConfiguration.bConfValLo;

						switch (vUsbConfiguration)
						{
						case CONF_UNCONFIGURED:
							{
								if (confValue == 1)
								{
									USB_EP1_INPUT_STATUS |= 0x4000;
									USB_EP1_OUTPUT_STATUS |= 0x4000;

									USB_EP1_INPUT_STATUS |= 0x200;
									USB_EP1_OUTPUT_STATUS |= 0x200;

									USB_EP1_INPUT_STATUS |= USB_ACTIVE;

									usbEP0Activate();

									while ((USB_CONTROL & USB_EP0_OUT) == 0);

									vUsbConfiguration = CONF_CONFIGURED;

									return;
								}
								else if (confValue == 0)
								{
									usbEP0Activate();

									while ((USB_CONTROL & USB_EP0_OUT) == 0);

									return;
								}
							}
							break;
						case CONF_CONFIGURED:
							{
								if (confValue == 1)
								{
									usbEP0Activate();

									while ((USB_CONTROL & USB_EP0_OUT) == 0);

									return;
								}
								else if (confValue == 0)
								{
									USB_EP1_INPUT_STATUS |= 0xFDFF;
									USB_EP1_OUTPUT_STATUS |= 0xFDFF;

									usbEP0Activate();

									while ((USB_CONTROL & USB_EP0_OUT) == 0);

									vUsbConfiguration = CONF_UNCONFIGURED;

									return;
								}
							}
							break;
						}
					}
				}
				break;
			}
		}
		else
		{
			return;
		}
	}
	// make ignore
	USB_EP0_OUTPUT_STATUS |= USB_IGNORE;
}
// From endpoint 1, handles data input and dispatches the read data to the user handler
static uchar_t           data[0x1F] = {0};

void usbEP1In()
{
	if (vUsbConfiguration == CONF_CONFIGURED)
	{
		if ((USB_EP1_INPUT_STATUS & USB_BUSY) == 0)
		{
			uchar_t           size = (USB_EP1_INPUT_STATUS & 0x1F);
			volatile uchar_t* in = USB_EP1_INPUT;
			uint_t            i = 0;
			while (i < size)
			{
				data[i] = in[i];
			}
			USB_EP1_INPUT_STATUS |= USB_ENABLE;
			
			if (vRxHandler != 0)
			{
				vRxHandler(data, size);
			}
		}
		else
		{
			USB_EP1_INPUT_STATUS |= USB_IGNORE;
		}
	}
}
/************************************************************************/
/* Interrupt handlers                                                   */
/************************************************************************/
// For handling all IN requests on all endpoints
void usbEPxIn()
{
	ushort_t control = USB_CONTROL & (USB_EP0_IN | USB_EP1_IN);

	switch (control)
	{
	case USB_EP0_IN:
		{
			USB_EP0_OUTPUT_STATUS &= ~USB_ACTIVE;
			// making ep0 read
			usbEP0In();
		}
		break;
	case USB_EP1_IN:
		{
			// making ep1 read
			usbEP1In();
		}
		break;
	}
}
/************************************************************************/
/* API                                                                  */
/************************************************************************/
// Initializer
void usbInit(rxHandler handler)
{
//	intsSetupHandler(INT_USB_RESET, usbEP0Reset);
//	intsSetupHandler(INT_USB_READ, usbEPxIn);

	vRxHandler = handler;
	USB_RESET |= 0x79;
	vUsbConfiguration = CONF_CONFIGURED;

}
// Writer
uint_t usbWrite(const void* data, uint_t size)
{
	uint_t         segmentsCount = size / 16;
	uint_t         remainedCount = size % 16;
	uint_t         i = 0;
	uint_t         res = 0;
	const uchar_t* out = data;

	while (i < segmentsCount)
	{
		res += usbEP1Write(&out[i * 16], 16);
		++i;
	}
	if (remainedCount != 0)
	{
		res += usbEP1Write(&out[i * 16], remainedCount);
	}
	return res;
}
