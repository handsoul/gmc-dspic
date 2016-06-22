#include "includes.h"


// ���ڳ�ʼ��. Ӳ����ʹ����UART1.



void Uart_Init(void)
{
    U1BGR = (FREQ_FCY/UART_BUAD_RATE)/16 - 1;

    // 115200 �����: ����ģʽ: U1BGR = 20.7 = 21,   ʵ��ֵ: 113636 , ��� 1.35%.
    //                ����ģʽ: U1BGR = 86.81-1 = 86.ʵ��ֵ: 114942 , ���0.22%
    
    U1MODEbits.STSEL = 0; // 1-stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U1MODEbits.BRGH  = 1;  // High Speed mode

    // �����ͺͽ����ж�.

    IEC0bits.U1TXIE = 1; // �������ж�.
    IEC0Bits.U1RXIE = 1; // ��������ж�.

    // TODO: �޸�ΪDMA��ʽ,����CPUƵ���ж�.
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN   = 1;
    
    // 
}
