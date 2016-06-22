#include "includes.h"


// 串口初始化. 硬件上使用了UART1.



void Uart_Init(void)
{
    U1BGR = (FREQ_FCY/UART_BUAD_RATE)/16 - 1;

    // 115200 的误差: 低速模式: U1BGR = 20.7 = 21,   实际值: 113636 , 误差 1.35%.
    //                高速模式: U1BGR = 86.81-1 = 86.实际值: 114942 , 误差0.22%
    
    U1MODEbits.STSEL = 0; // 1-stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U1MODEbits.BRGH  = 1;  // High Speed mode

    // 允许发送和接收中断.

    IEC0bits.U1TXIE = 1; // 允许发送中断.
    IEC0Bits.U1RXIE = 1; // 允许接收中断.

    // TODO: 修改为DMA方式,避免CPU频繁中断.
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN   = 1;
    
    // 
}
