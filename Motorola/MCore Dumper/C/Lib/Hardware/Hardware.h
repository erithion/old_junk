#ifndef _REGDEFS_H
#define _REGDEFS_H

typedef unsigned int   uint_t;
typedef unsigned short ushort_t;
typedef unsigned char  uchar_t;
/************************************************************************/
/* Reboot Control Register (Watchdog???)                                */
/************************************************************************/
#define ctlWdt (*(volatile ushort_t*) (0x204000))
/************************************************************************/
/* Unknown                                                              */
/************************************************************************/
#define sciCReg (*(volatile ushort_t*) (0x218000))
#define sciCReg8 (*(volatile ushort_t*) (0x218008))
#define sciCReg10 (*(volatile ushort_t*) (0x21800A))
#define sciCReg12 (*(volatile ushort_t*) (0x21800C))
#define sciCReg14 (*(volatile ushort_t*) (0x21800E))
#define sciCReg16 (*(volatile ushort_t*) (0x218010))
#define sciCReg18 (*(volatile ushort_t*) (0x218012))

/************************************************************************/
/* USB                                                                  */
/************************************************************************/
#define USB_DEVICE_ADDRESS (*(volatile ushort_t*) (0x209000))
#define USB_RESET (*(volatile ushort_t*) (0x209002))

typedef enum
{
	USB_EP0_IN = 8,                  // In detected on End Point 0
	USB_EP0_OUT = 0x10,              // Out on End Point 0
	USB_EP1_IN = 0x20,               // In on End Point 1
	USB_EP1_OUT = 0x40               // Out on End Point 1
}usbControlBits;

#define USB_CONTROL (*(volatile ushort_t*) (0x209004))

typedef enum
{
	USB_ERROR = 1,
	USB_BUSY = 0x40,
	USB_ACTIVE = 0x400,     // Seems to be an activation, at least this is true for output
	USB_IGNORE = 0x800,     // A kind of ignore flag in response to any incorrect situations on either input or output
	USB_ENABLE = 0x2000
}usbStatusBits;

#define USB_EP0_INPUT_STATUS (*(volatile ushort_t*) (0x209006))
#define USB_EP0_OUTPUT_STATUS (*(volatile ushort_t*) (0x209008))
#define USB_EP1_INPUT_STATUS (*(volatile ushort_t*) (0x20900A))
#define USB_EP1_OUTPUT_STATUS (*(volatile ushort_t*) (0x20900C))

#define USB_EP0_INPUT ((volatile uchar_t*) 0x209080)
#define USB_EP0_OUTPUT ((volatile uchar_t*) 0x209088)
#define USB_EP1_INPUT ((volatile uchar_t*) 0x209090)
#define USB_EP1_OUTPUT ((volatile uchar_t*) 0x2090A0)

#endif // #ifndef _REGDEFS_H