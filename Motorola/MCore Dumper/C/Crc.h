#ifndef _CRC_H
#define _CRC_H

/************************************************************************/
/* Resets the CRC value. Should be called at the start for the entire   */
/* block.                                                               */
/************************************************************************/
void crcReset();
/************************************************************************/
/* Recalculates the CRC value for the entire block plus the new value   */
/************************************************************************/
void crcUpdate(unsigned char value);
/************************************************************************/
/* Returns the CRC value of the entire block                            */
/************************************************************************/
unsigned char crcGet();

#endif // #ifndef _CRC_H