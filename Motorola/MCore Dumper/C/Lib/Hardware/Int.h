#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#define INT_USB_RESET 29
#define INT_USB_READ  58

typedef void (*intHandler)();
void intsSetupHandler(unsigned char intNumber, intHandler handler);

#endif // _INTERRUPTS_H_