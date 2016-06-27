#ifndef __PUBLIC_FUNCTIONS_H__
#define __PUBLIC_FUNCTIONS_H__

#include "includes.h"

// 互斥赋值(例如: 主程序中访问"与ISR共享"的变量), 确保赋值过程不因被ISR打断而得到无效值
#define MUTEX_SET_VALUE(a, b)       \
    do          \
    {           \
        a = b;  \
    } while (a != b)


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


#define UNPACK16_BE(pbuf,usData) \
    ((u8*)(pbuf))[0] = (u8)((usData) >> 8);\
    ((u8*)(pbuf))[1] = (u8)((usData) >> 0);

#define UNPACK32_BE(pbuf,usData) \
    ((u8*)(pbuf))[0] = (u8)((usData) >> 24);\
    ((u8*)(pbuf))[1] = (u8)((usData) >> 16);\
    ((u8*)(pbuf))[2] = (u8)((usData) >> 8);\
    ((u8*)(pbuf))[3] = (u8)((usData) >> 0);


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
