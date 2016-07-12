#include "includes.h"

void EE_Init(void)
{
	I2C2_Init(); 	// EEPROM 使用的是I2C2.
	DISABLE_EE_WP;	// disable write protection function.
}

void EE_Write_Byte( u32 addr,unsigned char WRDATA )   //写 24C64 1字节
{
	unsigned char AddrH = 0;         // 高地址
	unsigned char AddrL = 0;         // 低地址
    unsigned char ucCtrlByte = 0;
    // I2C_EEPROM控制字节结构:
    // [7] [6] [5] [4] [3] [2] [1] [0]
    //  1   0   1   0  A2  A1  A0  R/W_
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // 高地址  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    ucCtrlByte |= 0xA0;     // 控制码


	AddrL = (unsigned char)(addr);   // 地些转换
	AddrH = (unsigned char)((addr >>8)&0xFF);// 2M. = 
	
	I2C2_Idle();                     // 总线空闲？
  	I2C2_Start();                    // I2C2 启动
	I2C2_Done();                     // 清零 MI2C2IF
				
	I2C2_Write(ucCtrlByte);                // Write Control+Write
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(AddrH);               // 写 EEPROM 高地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(AddrL);               // 写 EEPROM 低地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(WRDATA);              // Write Data to EEPROM
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Stop();                     // I2C总线停止
	I2C2_Done();                     // 清零 MI2C2IF
}

//-------------------------------------------------------------------------------//
// I2C2 连续写多个字节
//-------------------------------------------------------------------------------//
void EE_SEQU_Write(u32 addr,unsigned char length,unsigned char *dptr)
{	
	unsigned char AddrH = 0;         // 高地址
	unsigned char AddrL = 0;         // 低地址
    unsigned char ucCtrlByte = 0;
    
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // 高地址  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    
    ucCtrlByte |= 0xA0;     // 控制码
    

	AddrL = (unsigned char)(addr);   // 地些转换
	AddrH = (unsigned char)((addr >>8)&0x1F);//8K
	
	I2C2_Idle();                     // 总线空闲？
  	I2C2_Start();                    // I2C2 启动
	I2C2_Done();                     // 清零 MI2C2IF

    
	I2C2_Write(ucCtrlByte);          // Write Control+Write
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(AddrH);               // 写 EEPROM 高地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF
	
	I2C2_Write(AddrL);               // 写 EEPROM 低地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	while (length != 0)              // Check write completed ?
	{
		I2C2_Write(*dptr);           // Write data to EEPROM
		while(I2C2STATbits.ACKSTAT); // wait Acknowledge from EEPROM
		I2C2_Done();	             // 清零 MI2C2IF
		dptr++;                      // Point to next byte
		length--;
	}
	
	I2C2_Stop();                     // I2C总线停止
	I2C2_Done();                     // 清零 MI2C2IF
}
//-------------------------------------------------------------------------------//
// I2C2 连续 "读" 1个字节
//-------------------------------------------------------------------------------//
unsigned char EE_Read_Byte(u32 addr)
{
	unsigned char RD_DATA;           // 临时变量
	unsigned char AddrH = 0;         // 高地址
	unsigned char AddrL = 0;         // 低地址
    unsigned char ucCtrlByte = 0;
    // I2C_EEPROM控制字节结构:
    // [7] [6] [5] [4] [3] [2] [1] [0]
    //  1   0   1   0  A2  A1  A0  R/W_
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // 高地址  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    ucCtrlByte |= 0xA0;     // 控制码


	AddrL = (unsigned char)(addr);   // 地些转换
	AddrH = (unsigned char)((addr >>8)&0xFF);
	
	I2C2_Idle();                     // 总线空闲？
  	I2C2_Start();                    // I2C2 启动
	I2C2_Done();                     // 清零 MI2C2IF
				
	I2C2_Write(ucCtrlByte);                // Write Control+Write

	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(AddrH);               // 写 EEPROM 高地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(AddrL);               // 写 EEPROM 低地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

   	I2C2_Restart();                  // initiate Restart condition
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(ucCtrlByte | 0x01);   // Write Control+Read
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	RD_DATA = I2C2_Read();           // 读取1字节数据
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_NotAck();                   // 无应答  NACK
	I2C2_Done();	                 // 清零 MI2C2IF
                     
	I2C2_Stop();                     // I2C总线停止
	I2C2_Done();                     // 清零 MI2C2IF

	return(RD_DATA);                 // 返回读取数据
}
//-------------------------------------------------------------------------------//
// I2C2 连续 "读" 多个字节
//-------------------------------------------------------------------------------//
void EE_SEQU_Read(u32 addr,unsigned char length,unsigned char *dptr)
{
	unsigned char AddrH = 0;         // 高地址
	unsigned char AddrL = 0;         // 低地址
    unsigned char ucCtrlByte;
    // I2C_EEPROM控制字节结构:
    // [7] [6] [5] [4] [3] [2] [1] [0]
    //  1   0   1   0  A2  A1  A0  R/W_
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // 高地址  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    ucCtrlByte |= 0xA0;     // 控制码


	AddrL = (unsigned char)(addr);   // 地些转换
	AddrH = (unsigned char)((addr >>8)&0xFF);// 2M. = 
	
	I2C2_Idle();                     // 总线空闲？
  	I2C2_Start();                    // I2C2 启动
	I2C2_Done();                     // 清零 MI2C2IF
				
	I2C2_Write(ucCtrlByte);                // Write Control+Write

	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	I2C2_Write(AddrH);               // 写 EEPROM 高地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF
	
	I2C2_Write(AddrL);               // 写 EEPROM 低地址
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

   	I2C2_Restart();                  // initiate Restart condition
	I2C2_Done();                     // 清零 MI2C2IF

    // 数据方向为读.
	I2C2_Write(ucCtrlByte|0x01);                // Write Control+Read
	while( I2C2STATbits.ACKSTAT );   // 等待 ACK
	I2C2_Done();                     // 清零 MI2C2IF

	while (length!=0)
	{
		*dptr = I2C2_Read();         // 连续读取放入缓冲区
		I2C2_Done();	
		dptr++;	
		length--;
				
		if (length == 0) I2C2_NotAck();   // 无应答 NACK
		else             I2C2_Ack();      // 应答   ACK
		
		I2C2_Done();
	}

	I2C2_Stop();                     // I2C总线停止
	I2C2_Done();                     // 清零 MI2C2IF
}


/***********************************************************
 * 函数名称: EEPROM_WriteBytes
 * 函数功能: 向EEPROM中直接写入一定字节数
 * 输入参数: ulMemAddr:     EEPROM器件内部地址
 *           pucSrcData:     源数据地址(为兼容不同数据类型, 使用void *)
 *           ulDataNum:     待写数据长度字节数
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 本函数完成直接对EEPROM的写操作, 注意:
 *           由于EEPROM的内部写逻辑需要10ms延迟, 因此建议使用队列写.
 *           函数仅用在初始化或判断EEPROM好坏等需要立即返回结果场合.
***********************************************************/
// #pragma MESSAGE DISABLE C12056      // 禁止"类似代码需要合并"的告警(页写调用)
void EEPROM_WriteBytes(u32 ulMemAddr, u8 *pucSrcData, u32 ulDataNum)
{
    u16 usWriteNum;         // 每次写入字节数
    
    if ((ulMemAddr + ulDataNum >= EEPROM_SIZE)
        || (ulDataNum == 0))
    {
        return ;
    }

    // 满足页写地址区间的, 直接采用页写
    if (ulMemAddr / EEPROM_PAGE_SIZE == (ulMemAddr + ulDataNum - 1) / EEPROM_PAGE_SIZE)
    {
        EE_SEQU_Write(ulMemAddr, (u16)ulDataNum, pucSrcData);
        DelayMS(5);
        ClearWdt();
    }
    // 不满足页写区间的, 分成多页进行页写
    else
    {
        // 写第1页
        usWriteNum = (u16)(EEPROM_PAGE_SIZE - ulMemAddr % EEPROM_PAGE_SIZE);
        EE_SEQU_Write(ulMemAddr,usWriteNum ,pucSrcData);
        DelayMS(5);
        ClearWdt();
        ulMemAddr  += usWriteNum;
        pucSrcData += usWriteNum;
        ulDataNum  -= usWriteNum;
            
        // 写中间页
        while (ulDataNum >= EEPROM_PAGE_SIZE)
        {
            EE_SEQU_Write(ulMemAddr,EEPROM_PAGE_SIZE,pucSrcData);
            DelayMS(5);
            ClearWdt();
            ulMemAddr  += EEPROM_PAGE_SIZE;
            pucSrcData += EEPROM_PAGE_SIZE;
            ulDataNum  -= EEPROM_PAGE_SIZE;
        }

        // 写最后1页
        if (ulDataNum > 0)
        {
            EE_SEQU_Write(ulMemAddr,(u16)ulDataNum,pucSrcData);
            DelayMS(5);
            ClearWdt();
        }                  
    }        
}

// void EE_SEQU_Read(u32 addr,unsigned char length,unsigned char *dptr)
void EEPROM_ReadBytes(u32 ulMemAddr, u8 *pucDstData, u32 ulDataNum)
{
    EE_SEQU_Read(ulMemAddr,ulDataNum,pucDstData);
}