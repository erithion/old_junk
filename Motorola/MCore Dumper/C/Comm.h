#ifndef _COMM_H_
#define _COMM_H_

#define maxBufferSize 0x20

extern void commInit();
extern unsigned char commReadByte();
extern unsigned int commReadInt();
extern unsigned int commWrite(const void* data, unsigned int size);

#endif // _COMM_H_