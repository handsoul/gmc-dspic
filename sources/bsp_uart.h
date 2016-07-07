#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "includes.h"


#define UART_BUAD_RATE 9600

extern void SendByte(u8 ucData);
extern void Uart_Init(void);


#endif // __BSP_UART_H__
// end of file
