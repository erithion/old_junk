#pragma once

#include <stdarg.h>
#include <string>
//#include <common.h>
//#include "../include/phone/phn_types.h"


namespace Tracer
{
    typedef int LogType;
    enum LogTypeEnum
    {
        LT_PLUGIN = 0x01,
        LT_SERIAL = 0x02
    };
}
#ifdef	_LOG
void initLog( char* szFileName, int logLevel = 5, Tracer::LogType type = Tracer::LT_PLUGIN );
void doneLog(Tracer::LogType type = (Tracer::LT_PLUGIN | Tracer::LT_SERIAL));
//void writeHeader(const std::string& pluginName, const common::api_s &ver);

void serialLogSend(void* msg, int len);
void serialLogRecieve(void* msg, int len);

void dprintf( char *format, va_list args );

extern int mLevel;
char* ToHex( void *msg, int len );

void ErrorA(char *format, ...);
void Warning1(char *format, ...);
void Warning2(char *format, ...);
void Warning3(char *format, ...);
void Warning4(char *format, ...);
void Warning5(char *format, ...);
void DumpMemory(int Tabs, void *msg, int len);
void DumpMemoryEx(int Tabs, void *msg, int len);

void SetLevel(int level);
#endif

#ifdef	_LOG
#define _INITLOG initLog
#define _DONELOG doneLog
#define _HEADER1 writeHeader

#define _SERIAL_LOG_SEND serialLogSend
#define _SERIAL_LOG_RECIEVE serialLogRecieve

#define _TRE ErrorA

#define _TRW1	Warning1
#define _TRW2	Warning2
#define _TRW3	Warning3
#define _TRW4	Warning4
#define _TRW5	Warning5
#define	_Dump	DumpMemory
#define	_DM		DumpMemory
#define	_DMX	DumpMemoryEx
#define _TRL	SetLevel
#define _TRL0	SetLevel(0);
#define _TRL1	SetLevel(1);
#define _TRL2	SetLevel(2);
#define _TRL3	SetLevel(3);
#define _TRL4	SetLevel(4);
#define _TRL5	SetLevel(5);

#else

#define _INITLOG
#define _DONELOG
#define _HEADER1

#define _SERIAL_LOG_SEND 
#define _SERIAL_LOG_RECIEVE 

#define _TRE
#define _TRW1
#define _TRW2
#define _TRW3
#define _TRW4
#define _TRW5
#define	_Dump
#define	_DM
#define	_DMX
#define _TRL
#define _TRL0
#define _TRL1
#define _TRL2
#define _TRL3
#define _TRL4
#define _TRL5
#endif