#include "includes.h"


void I2C1_Init(void)         // I2C1初始化
{
	TRISGbits.TRISG2 = 0;    // SCL1 
	TRISGbits.TRISG3 = 1;    // SDA1 
	I2C1CONbits.SCLREL = 1;  // SCL 释放时钟
	I2C1CONbits.DISSLW = 1;  // 禁止斜率控制
	I2C1BRG = 0x395;         // 40MHZ,100K,395; 40Mhz,400K,95.
	I2C1CONbits.I2CEN  = 1;  // 使能I2C1模块
}

//-------------------------------------------------------------------------------//
void I2C1_Close(void)        //关闭 I2C1
{
	I2C1CONbits.I2CEN = 0;
	_SI2C1IE = 0;
	_MI2C1IE = 0;
	_SI2C1IF = 0;
	_MI2C1IF = 0;
}
//-------------------------------------------------------------------------------//
void I2C1_Idle(void)         // 总线空闲检测
{
    while(I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN || 
          I2C1CONbits.RSEN || I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT);	
}
//-------------------------------------------------------------------------------//
void I2C1_Start(void)	     // 启动 I2C1
{
     I2C1CONbits.SEN = 1;
}
//-------------------------------------------------------------------------------//
void I2C1_Restart(void)	     // 重启动I2C1
{ 
    I2C1CONbits.RSEN = 1;
}
//-------------------------------------------------------------------------------//
void I2C1_Stop(void)	     // 停止  I2C1
{
     I2C1CONbits.PEN = 1;
}
//-------------------------------------------------------------------------------//
void I2C1_Done(void)	     // 等待  I2C1
{
	while( !IFS1bits.MI2C1IF );
	IFS1bits.MI2C1IF = 0;
}	

void I2C1_Ack(void)          // 产生应答 ACK
{
	I2C1CONbits.ACKDT = 0;
	I2C1CONbits.ACKEN = 1;
}

void I2C1_NotAck(void)       // 不产生应答 ACK
{
    I2C1CONbits.ACKDT = 1; 
    I2C1CONbits.ACKEN = 1;
}

char I2C1_DataRdy(void)      // 接收缓冲区状态  1:满    0:空
{
     return I2C1STATbits.RBF;
}

unsigned char I2C1_Read(void)          // 主控读
{
    I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN);
    I2C1STATbits.I2COV = 0;
    return(I2C1RCV);
}

char I2C1_Write(unsigned char WRDATA)  // 主控写
{
    I2C1TRN = WRDATA;
    if(I2C1STATbits.IWCOL)   return -1;
    else                     return 0;
}
void I2C2_Init(void)         // I2C2初始化
{
    _TRISA2 = 0; // SCL2.
    _TRISA2 = 1; // SCL3.
    
	I2C2CONbits.SCLREL = 1;  // SCL 释放时钟
	I2C2CONbits.DISSLW = 1;  // 禁止斜率控制
	I2C2BRG = 0x395;         // 40MHZ,100K,395; 40Mhz,400K,95.
	I2C2CONbits.I2CEN  = 1;  // 使能I2C2模块
}

void I2C2_Close(void)        //关闭 I2C2
{
	I2C2CONbits.I2CEN = 0;
	_SI2C2IE = 0;
	_MI2C2IE = 0;
	_SI2C2IF = 0;
	_MI2C2IF = 0;
}

void I2C2_Idle(void)         // 总线空闲检测
{
    while(I2C2CONbits.SEN || I2C2CONbits.PEN || I2C2CONbits.RCEN || 
          I2C2CONbits.RSEN || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT);	
}

void I2C2_Start(void)	     // 启动 I2C2
{
     I2C2CONbits.SEN = 1;
}

void I2C2_Restart(void)	     // 重启动I2C2
{ 
    I2C2CONbits.RSEN = 1;
}

void I2C2_Stop(void)	     // 停止  I2C2
{
     I2C2CONbits.PEN = 1;
}

void I2C2_Done(void)	     // 等待  I2C2
{
	while( !IFS3bits.MI2C2IF );
	IFS3bits.MI2C2IF = 0;
}	

void I2C2_Ack(void)          // 产生应答 ACK
{
	I2C2CONbits.ACKDT = 0;
	I2C2CONbits.ACKEN = 1;
}

void I2C2_NotAck(void)       // 不产生应答 ACK
{
    I2C2CONbits.ACKDT = 1; 
    I2C2CONbits.ACKEN = 1;
}

char I2C2_DataRdy(void)      // 接收缓冲区状态  1:满    0:空
{
     return I2C2STATbits.RBF;
}

unsigned char I2C2_Read(void)          // 主控读
{
    I2C2CONbits.RCEN = 1;
    while(I2C2CONbits.RCEN);
    I2C2STATbits.I2COV = 0;
    return(I2C2RCV);
}

char I2C2_Write(unsigned char WRDATA)  // 主控写
{
    I2C2TRN = WRDATA;
    if(I2C2STATbits.IWCOL)   return -1;
    else                     return 0;
}
