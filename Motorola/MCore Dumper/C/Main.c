
#include "Crc.h"

#include "Comm.h"
#include "Ctl.h"


typedef unsigned int   uint_t;
typedef unsigned short ushort_t;
typedef unsigned char  uchar_t;
typedef uint_t         bool_t;


/************************************************************************/
/* Protocol Elements                                                    */
/************************************************************************/
#define idProtocol			0x55
#define idCmdDump			0x01
#define idCmdReboot			0x02
#define idCmdWelcome		0x5a

/************************************************************************/
/* Status Codes                                                         */
/************************************************************************/
#define statOk				0x0
#define statBadCommand		0xF1
#define statBadCrc			0xF2
#define statBadProtocol		0xF3
#define statBadDumpInterval 0xF4


/************************************************************************/
/************************************************************************/
/* Protocol Handlers                                                    */
/************************************************************************/
/************************************************************************/

/************************************************************************/
/* Notifies about the error accordingly to the protocol rules           */
/************************************************************************/
void handlerError(uchar_t code)
{
	// Creation of the packet to be sent on error
	uchar_t packet[] = {idProtocol, 0, 0, 0, 0, 0, 0, 0};
	// Sending out the error code
	packet[2] = code;
	// Reset CRC
	crcReset();
	// Sending out the CRC of a message body (0xff)
	packet[7] = crcGet();
	commWrite(packet, sizeof (packet));
}
/************************************************************************/
/* Reboots the phone                                                    */
/************************************************************************/
void handlerReboot()
{
	// Creation of the packet to be sent on reboot
	uchar_t packet[] = {idProtocol, idCmdReboot, statOk, 0, 0, 0, 0, 0};
	// Reset CRC
	crcReset();
	// Sending out the CRC of a message body (0xff)
	packet[7] = crcGet();
	commWrite(packet, sizeof (packet));
	// Rebooting the phone
	ctlReboot();
}
/************************************************************************/
/* Dumps the specified range of the phone memory and                    */
/* sends out the content accordingly to the protocol rules              */
/************************************************************************/
void handlerDump(const uchar_t* start, const uchar_t* end)
{
	uchar_t packet[] = {idProtocol, idCmdDump, statOk};
	// Sending out the size of the message body
	uint_t  size = (end - start);
	uint_t  i = 0;
	uchar_t crc = 0;

	if (start > end)
	{
		handlerError(statBadDumpInterval);
		return;
	}
	commWrite(packet, sizeof (packet));
	commWrite((uchar_t*)&size, sizeof(size));
	crcReset();
	for (i = 0; i < size; ++i)
	{
		// Sending out the next byte
		crcUpdate(start[i]);
	}
	commWrite(start, size);
	// Sending out the CRC of a message body
	crc = crcGet();
	commWrite(&crc, sizeof(crc));
}

void main()
{
//	char    welcomeMessage[] = "Paraben's iDEN DumperZ";
	char    welcomeMessage[2] = {0x50, 0x50};
	commInit();

   	commWrite(welcomeMessage, sizeof(welcomeMessage) - 1);

	// Forever
	while (1)
	{
		//if (commReadByte() == idProtocol)
		//{
		//	uint_t  i = 0;
		//	uint_t  idCmd = commReadByte();
		//	uint_t  size = commReadInt();
		//	uchar_t buffer[maxBufferSize];

		//	crcReset();
		//	while (i < size)
		//	{
		//		buffer[i] = commReadByte();
		//		crcUpdate(buffer[i]);
		//		++i;
		//	}
		//	// Checking whether CRC values are equal
		//	if (commReadByte() == crcGet())
		//	{
		//		switch(idCmd) 
		//		{
		//		case idCmdDump: 
		//			{
		//				// Getting the start border of interval for dumping
		//				uint_t start = (*(uint_t*)(buffer + 0));
		//				// Getting the end border of interval for dumping
		//				uint_t end = (*(uint_t*)(buffer + 4));
		//				handlerDump((uchar_t*)start, (uchar_t*)end);
		//			}
		//			break;
		//		case idCmdReboot: 
		//			{
		//				handlerReboot();
		//			}
		//			break;
		//		default:
		//			{
		//				handlerError(statBadCommand);
		//			}
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		handlerError(statBadCrc);
		//	}
		//}
		//else
		//{
		//	handlerError(statBadProtocol);
		//}
	}
	exit(0);
}
