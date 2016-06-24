#include "includes.h"


bool I2C0_StartDataTransmit(void)
{
}

bool I2C0_StopDataTransmit(void)
{
}

bool I2C0_WaitTransDone(void)
{
}

bool I2C0_WaitAck(void)
{
}



/***********************************************************
 * 函数名称: I2C0_WriteBytes
 * 函数功能: I2C写数据
 * 输入参数: ucCtrlByte:    控制字节
 *           usWriteAddr:   待写地址
 *           ucAddrType:    待写地址类型(0: 8bits, 1: 16bits)
 *           pucSrcData:    待写源数据指针
 *           ulDataNum:     待写数据长度字节数
 * 输出参数: 无.
 * 返 回 值: 是否发生超时. 0表示没有发生超时. 1表示发生过超时.
 * 备    注: I2C写数据的过程:
 *           1.启动传输;
 *           2.发送控制字节(方向为写, 等待响应, 下同);
 *           3.发送待写地址;
 *           4.发送待写数据内容;
 *           5.停止传输;
***********************************************************/
u8 I2C0_WriteBytes(u8 ucCtrlByte, u16 usWriteAddr, u8 ucAddrType, 
                         u8 *pucSrcData, u32 ulDataNum)
{
    u32 i;
    
    u32 ulTimeout = 0;    

    // 1.启动数据传输
    I2C0_StartDataTransmit();

    // 2.发送控制字节
    I2C0_DATA = (ucCtrlByte & 0xFE);    // [0]: 0, 写数据
    ulTimeout += I2C0_WaitAck();
    
    // 3.发送待写地址
    if (ucAddrType == I2C_ADDR_TYPE_16BITS)     // 16位地址模式时先发送高8位
    {
        I2C0_DATA = (u8)(usWriteAddr >> 8);
        ulTimeout += I2C0_WaitAck();
    }
    I2C0_DATA = (u8)usWriteAddr;
    ulTimeout += I2C0_WaitAck();

    // 4.发送数据内容
    for (i = 0; i < ulDataNum; ++i)
    {
        I2C0_DATA = pucSrcData[i];
        ulTimeout += I2C0_WaitAck();
    }

    // 5.停止数据传输
    I2C0_StopDataTransmit();
    
    return (u8)(ulTimeout > 0);
}

/***********************************************************
 * 函数名称: I2C0_ReadBytes
 * 函数功能: I2C读数据
 * 输入参数: ucCtrlByte:    控制字节
 *           usReadAddr:    待读地址
 *           ucAddrType:    待读地址类型(0: 8bits, 1: 16bits)
 *           pucDstData:    待读出数据放置的目标指针
 *           ucDataNum:     待读出数据长度字节数
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: I2C读数据的过程:
 *           1.启动传输;
 *           2.发送控制字节(方向为写, 等待响应, 下同);
 *           3.发送待读地址;  ---- 这一点不要忘了, 先写待读地址下去.
 *           4.重复发送起始条件(启动读数据流程), 发送控制字节(方向为读);
 *           5.设置为接收模式, 读出数据(前N-1个数据有应答['0'], 最后一个数据无应答['1']);
 *           6.停止传输;
***********************************************************/
u8 I2C0_ReadBytes(u8 ucCtrlByte, u16 usReadAddr, u8 ucAddrType, 
                        u8 *pucDstData, u32 ulDataNum)
{
    u32 i;
    u32 ulTimeout = 0;
   
    // 1.启动数据传输
    I2C0_StartDataTransmit();
   
    // 2.发送控制字节
    I2C0_DATA = (ucCtrlByte & 0xFE);    // [0]: 0, 写数据
    ulTimeout += I2C0_WaitAck();
    
    // 3.发送待读地址
    if (ucAddrType == I2C_ADDR_TYPE_16BITS)     // 16位地址模式时先发送高8位
    {
        I2C0_DATA = (u8)(usReadAddr >> 8);
        ulTimeout += I2C0_WaitAck();
    }
    I2C0_DATA = (u8)usReadAddr;
    ulTimeout += I2C0_WaitAck();

    // 4. 重复发送起始条件(启动读数据流程)
    I2C0_RPT_ST_AG;
    I2C0_DATA = (ucCtrlByte | 0x01); // 发送控制字节(方向为读)
    ulTimeout += I2C0_WaitAck();
   
    // 5.设置为接收模式, 读出数据
    I2C0_DIR_RX;
    // 主机接收模式下, 读IBDR寄存器来初始化接收(参考数据手册), 实际数据丢弃
    i = I2C0_DATA;
    // 前N-1个字节, 回送应答(I2C0_StartDataTransmit中已使能)
    for (i = 0; i < ulDataNum - 1; ++i)
    {
        ulTimeout += I2C0_WaitTransDone();
        // 取出寄存器中的数据
        pucDstData[i] = I2C0_DATA;
    }
    // 接收最后一个字节不应答
    I2C0_ACK_DISABLE;
    ulTimeout += I2C0_WaitTransDone();
      
    // 5.停止数据传输
    I2C0_StopDataTransmit();
    // 取出接收的最后一个字节
    pucDstData[i] = I2C0_DATA;
    
    return (u8)(ulTimeout > 0);
}


