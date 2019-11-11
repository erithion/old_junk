#ifndef _USB_H
#define _USB_H

/************************************************************************/
/* Initialization of USB ports.                                         */
/* Should be called before any attempts                                 */
/* of reading/writing from/to UCB ports                                 */
/* Specified RX handler will be called each time when incoming request  */
/* has arrived                                                          */
/************************************************************************/
typedef void (*rxHandler)(const void* data, unsigned int size);
extern void usbInit(rxHandler);
/************************************************************************/
/* Writes a packet of the specified size into the USB output            */
/* However, the packet size can't be greater than maxPacketSize         */
/************************************************************************/
extern unsigned int usbWrite(const void* data, unsigned int size);

#endif // #ifndef _USB_H