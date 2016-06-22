#ifndef __PUBLIC_FUNCTIONS_H__
#define __PUBLIC_FUNCTIONS_H__

#include "includes.h"


#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define ABS(a)          ((a) >= 0 ? (a) : (-(a)))

#define MAKEWORD(ucLow, ucHigh) ((u16)((u8)(ucLow)) | (((u16)(u8)(ucHigh))<<8))

#define MAKELONG(usLow, usHigh) ((u32)((u16)(usLow)) | (((u32)(u16)(usHigh))<<16))

#define HIBYTE(usData)  ((u8)(usData >> 8))

#define LOBYTE(usData)  ((u8)(usData))

#define HIWORD(ulData)  ((u16)(ulData >> 16))

#define LOWORD(ulData)  ((u16)(ulData))

#define ITEM_OF(array)  (sizeof(array)/sizeof(array[0]))

#define UPPER_LIMIT(value, upper)   if ((value) > (upper)) (value) = (upper)
#define LOWER_LIMIT(value, lower)   if ((value) < (lower)) (value) = (lower)                         


extern void MemorySet(void *pvDstAddr, u8 ucSetValue, u32 ulSize);
extern void MemoryCopy(void *pvDstAddr, void *pvSrcAddr, u32 ulSize);
extern s16  MemoryCompare(void *pvMemAddr1, void *pvMemAddr2, u32 ulSize);
extern bool IsMemory0xFF(void *pvMemAddr, u32 ulLen);
extern void SwapBigLittleEndian(void *pvDataAddr, u8 ucSize);
extern u16  GetStrLen(s8 *pscString);
extern u8   GetSum(u8 *pucData, u32 ulLen);
extern u16  GetAverageForU16(u16 *pusData, u32 ulDataNum, bool bDelLimit);
extern s32  GetAverageForS32(s32 *pslData, u32 ulDataNum, bool bDelLimit);
extern void DelayTick(u16 usCount);
extern void DelayMS(u16 usCount);
extern u8   BcdToDec(u8 ucValue);
extern u8   DecToBcd(u8 ucValue);


// CRC16 
extern u16 CRC16(u8* pchMsg, u16 wDataLen);

#endif 
