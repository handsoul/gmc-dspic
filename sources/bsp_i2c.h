#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "includes.h"

void I2C1_Init(void);
void I2C1_Close(void);
void I2C1_Idle(void);
void I2C1_Start(void);
void I2C1_Restart(void);
void I2C1_Stop(void);
void I2C1_Done(void);
void I2C1_Ack(void);
void I2C1_NotAck(void);
char I2C1_DataRdy(void);
unsigned char I2C1_Read(void) ;
char I2C1_Write(unsigned char WRDATA) ;

void I2C2_Init(void);
void I2C2_Close(void);
void I2C2_Idle(void);
void I2C2_Start(void);
void I2C2_Restart(void);
void I2C2_Stop(void);
void I2C2_Done(void);
void I2C2_Ack(void);
void I2C2_NotAck(void);
char I2C2_DataRdy(void);
unsigned char I2C2_Read(void) ;
char I2C2_Write(unsigned char WRDATA) ;


#endif // __BSP_I2C_H__

// end of file.
