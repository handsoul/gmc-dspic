#include "includes.h"
// to use some predefined functions.
#define FCY     40000000UL
#include <libpic30.h>

// Calc CRC16.
u16 CRC16(u8* pchMsg, u16 wDataLen)
{
	static const u8 chCRCHTalbe[] =
	{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40
	};

	static const u8 chCRCLTalbe[] =                                 // CRC ??????
	{
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
		0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
		0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
		0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
		0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
		0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
		0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
		0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
		0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
		0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
		0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
		0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
		0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
		0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
		0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
		0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
		0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
		0x41, 0x81, 0x80, 0x40
	};


	u8 chCRCHi = 0xFF;
	u8 chCRCLo = 0xFF;
	u16 wIndex;

	while (wDataLen--)
	{
		wIndex  = chCRCLo ^ *pchMsg++;
		chCRCLo = chCRCHi ^ chCRCHTalbe[wIndex];
		chCRCHi = chCRCLTalbe[wIndex];
	}

	return ((chCRCHi << 8) | chCRCLo);
}


/***********************************************************
 * 函数名称: MemorySet
 * 函数功能: 内存赋值
 * 输入参数: pvDstAddr:     目标地址
 *           ucSetValue:    待赋的值
 *           ulSize:        待赋值区域字节长度
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 避免调用库函数造成的开销.
***********************************************************/
void MemorySet(void *pvDstAddr, u8 ucSetValue, u32 ulSize)
{
    u8 *pucDstAddr = (u8 *)pvDstAddr;
    
    while (ulSize-- > 0)
    {
        *pucDstAddr++ = ucSetValue;
    }
}

/***********************************************************
 * 函数名称: MemoryCopy
 * 函数功能: 内存复制
 * 输入参数: pvDstAddr:     目标地址
 *           pvSrcAddr:     源地址
 *           ulSize:        待复制区域字节长度
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 注意对源地址和目标地址进行比较, 避免数据覆盖/丢失.
***********************************************************/
void MemoryCopy(void *pvDstAddr, void *pvSrcAddr, u32 ulSize)
{
    u8 *pucDstAddr = (u8 *)pvDstAddr;
    u8 *pucSrcAddr = (u8 *)pvSrcAddr;
    
    // 源/目标地址重合, 不需要复制
    if ((pucDstAddr == pucSrcAddr)
        || (ulSize == 0))
    {
        return ;
    }
    // 目标地址较低, 直接复制
    else if (pucDstAddr < pucSrcAddr)
    {
        while (ulSize-- > 0)
        {
            *pucDstAddr++ = *pucSrcAddr++;
        }
    }
    // 目标地址较高, 反向复制
    else
    {
        pucDstAddr += (ulSize - 1);
        pucSrcAddr += (ulSize - 1);
        while (ulSize-- > 0)
        {
            *pucDstAddr-- = *pucSrcAddr--;
        }               
    }    
}

/***********************************************************
 * 函数名称: MemoryCompare
 * 函数功能: 内存比较
 * 输入参数: pvMemAddr1:    内存地址1
 *           pvMemAddr2:    内存地址2
 *           ulSize:        待比较区域字节长度
 * 输出参数: 无.
 * 返 回 值: -1:    Addr1 < Add2(按字节大小比较);
 *           0:     Addr1 == Addr2;
 *           1:     Addr1 > Addr2.
 * 备    注: 
***********************************************************/
s16 MemoryCompare(void *pvMemAddr1, void *pvMemAddr2, u32 ulSize)
{
    u8 *pucMemAddr1 = (u8 *)pvMemAddr1;
    u8 *pucMemAddr2 = (u8 *)pvMemAddr2;
    u32 i = 0;
    
    while (ulSize-- > 0)
    {
        if (pucMemAddr1[i] < pucMemAddr2[i])
        {
            return -1;
        }
        else if (pucMemAddr1[i] > pucMemAddr2[i])
        {
            return 1;
        }
        ++i;
    }
    return 0;
}

/***********************************************************
 * 函数名称: IsMemory0xFF
 * 函数功能: 判断存储区是否为0xFF
 * 输入参数: pvMemAddr:     内存地址
 *           ulLen:         待判断区域字节长度
 * 输出参数: 无.
 * 返 回 值: TRUE:  存储区都是0xFF.
 *           FALSE: 存储区不全是0xFF.
 * 备    注: 
***********************************************************/
bool IsMemory0xFF(void *pvMemAddr, u32 ulLen)
{
    u8 *pucMemAddr = (u8 *)pvMemAddr;
    u32 i = 0;
    while (ulLen-- > 0)
    {
        if (pucMemAddr[i++] != 0xFF)
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

/***********************************************************
 * 函数名称: SwapBigLittleEndian
 * 函数功能: 字节序转换
 * 输入参数: pvDataAddr:    数据地址
 *           ucSize:        待转换数据长度
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 只接受2/4字节的转换
***********************************************************/
void SwapBigLittleEndian(void *pvDataAddr, u8 ucSize)
{
    u8 *pucDataTemp = (u8 *)pvDataAddr;
    u8  ucTemp;

    if (ucSize == 2)
    {
        ucTemp = pucDataTemp[0];
        pucDataTemp[0] = pucDataTemp[1];
        pucDataTemp[1] = ucTemp;
    }
    else if (ucSize == 4)
    {
        ucTemp = pucDataTemp[0];
        pucDataTemp[0] = pucDataTemp[3];
        pucDataTemp[3] = ucTemp;
        ucTemp = pucDataTemp[2];
        pucDataTemp[2] = pucDataTemp[1];
        pucDataTemp[1] = ucTemp;
    }
}

/***********************************************************
 * 函数名称: GetStrLen
 * 函数功能: 获取字符串长度
 * 输入参数: pscString:     目标字符串
 * 输出参数: 无.
 * 返 回 值: 目标字符串长度
 * 备    注: 注意长度最多为2个字节.
***********************************************************/
u16 GetStrLen(s8 *pscString)
{
    u16 i = 0;
    while (pscString[i] != '\0')
    {
        ++i;
    }
    return i;
}

/***********************************************************
 * 函数名称: GetSum
 * 函数功能: 计算字节序列的累加和取最低字节后的补码
 * 输入参数: pucData:   输入序列指针
 *           ulLen:     输入序列长度
 * 输出参数: 无.
 * 返 回 值: 输入序列累加和(保留最低字节)
 * 备    注: 对累加和的最低字节按位取反.
***********************************************************/
u8 GetSum(u8 *pucData, u32 ulLen)
{
    u8  ucSum = 0;
    u32 i;
    for (i = 0; i < ulLen; ++i)
    {
        ucSum += pucData[i];
    }
    return ucSum;
}

/***********************************************************
 * 函数名称: GetAverageForU16
 * 函数功能: 计算输入序列(U16)的平均值
 * 输入参数: pusData:   输入序列指针
 *           ulLen:     输入序列长度
 *           bDelLimit: 是否去掉极值
 * 输出参数: 无.
 * 返 回 值: 输入序列均值
 * 备    注: 去极值求平均.
***********************************************************/
u16 GetAverageForU16(u16 *pusData, u32 ulDataNum, bool bDelLimit)
{
    u16 usMaxValue = pusData[0];
    u16 usMinValue = pusData[0];
    u32 i;
    u32 ulSum = 0;

    if (ulDataNum == 0)
    {
        return 0;
    }
    
    for (i = 0; i < ulDataNum; ++i)
    {        
        ulSum += pusData[i];
        if (pusData[i] > usMaxValue)
        {
            usMaxValue = pusData[i];
        }
        else if (pusData[i] < usMinValue)
        {
            usMinValue = pusData[i];
        }
    }
    if ((bDelLimit == TRUE)
        && (ulDataNum > 2))
    {
        return (u16)((ulSum - usMaxValue - usMinValue) / (ulDataNum - 2));
    }
    else
    {
        return (u16)(ulSum / ulDataNum);
    }
}

/***********************************************************
 * 函数名称: GetAverageForS32
 * 函数功能: 计算输入序列(S32)的平均值
 * 输入参数: pslData:   输入序列指针
 *           ulLen:     输入序列长度
 *           bDelLimit: 是否去掉极值
 * 输出参数: 无.
 * 返 回 值: 输入序列均值
 * 备    注: 去极值求平均.
***********************************************************/
s32 GetAverageForS32(s32 *pslData, u32 ulDataNum, bool bDelLimit)
{
    s32 slMaxValue = pslData[0];
    s32 slMinValue = pslData[0];
    s32 i;
    s32 slDataNum = (s32)ulDataNum;
    s32 slSum = 0;

    if (slDataNum == 0)
    {
        return 0;
    }
    
    for (i = 0; i < slDataNum; ++i)
    {        
        slSum += pslData[i];
        if (pslData[i] > slMaxValue)
        {
            slMaxValue = pslData[i];
        }
        else if (pslData[i] < slMinValue)
        {
            slMinValue = pslData[i];
        }
    }
    if ((bDelLimit == TRUE)
        && (slDataNum > 2))
    {
        return (s32)((slSum - slMaxValue - slMinValue) / (slDataNum - 2));
    }
    else
    {
        return (s32)(slSum / slDataNum);
    }
}

/***********************************************************
 * 函数名称: DelayTick
 * 函数功能: 延迟一定节拍数
 * 输入参数: usCount:   延时数
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: DelayTime(T) = 7 * n + 9.
 *           加上调用开销9, T = (7 * n + 18)(Tick).
 *           对于本工程的Fcpu = 40MHz, T = 450 + 175 * n(ns).
 *           如果形参为8bit, 则调用开销为8, 其余不变. T = (7 * n + 17).
***********************************************************/
void DelayTick(u16 usCount)
{
    while (usCount--)
    {
    }
}

/***********************************************************
 * 函数名称: DelayMS
 * 函数功能: 延迟n毫秒
 * 输入参数: usCount:   延时数
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: T = n * T1.
 *           相比T1时间, 调用开销可以忽略. 
 *           T1 = 450 + 175 * 5711(ns) = 1000050(ns) ~= 1(ms).
***********************************************************/



void DelayMS(u16 usCount)
{
    while (usCount--)
    {
        __delay_ms(1);
		ClearWdt();
    }
}  

/***********************************************************
 * 函数名称: BcdToDec
 * 函数功能: BCD码转化成十进制
 * 输入参数: ucValue:   待转换的BCD码值
 * 输出参数: 无.
 * 返 回 值: 转换后的十进制值
 * 备    注: 
***********************************************************/
u8   BcdToDec(u8 ucValue)
{
  	return (((ucValue >> 4) & 0xF) * 10 + (ucValue & 0xF));
}

/***********************************************************
 * 函数名称: DecToBcd
 * 函数功能: 十进制转化成BCD码
 * 输入参数: ucValue:   待转换的十进制值
 * 输出参数: 无.
 * 返 回 值: 转换后的BCD码值
 * 备    注: 
***********************************************************/
u8   DecToBcd(u8 ucValue)
{
    if (ucValue >= 100)
    {
        return 0;
    }
  	return (((ucValue / 10) << 4) + (ucValue % 10));
}

// End Of File.

