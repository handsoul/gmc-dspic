#include "includes.h"

// 设置GPIO的输入/输出.

void GPIO_Init(void)
{
    // RA.
    // RA0 - T1 - DS18B20 - 温度检测.
    // RA1 - OUT7 - DO.
    _TRISA1 = 0;
    _LATA1  = 0;


    // RB.
    // RB0 - ICDDA.
    // RB1 - ICDCK
    // RB3 / RB4 / RB5 QEI接口.
    // RB6 - 
    // RB7 OUT4.
    // RB8 OUT3.
    // RB9 OUT2.
    // RB10 OUT1.
    // RB11 OUT8
    // RB12 INPUT11
    // RB13 INPUT21
    // RB14 INPUT31
    // RB15 INPUT41
    _TRISB7 = 0;
    _LATB7  = 0;

    _TRISB8 = 0;
    _LATB8  = 0;
    
    _TRISB9 = 0;
    _LATB9  = 0;
    
    _TRISB10 = 0;
    _LATB10  = 0;
    
    _TRISB11 = 0;
    _LATB11  = 0;


    _TRISB12 = 1;
    _TRISB13 = 1;
    _TRISB14 = 1;
    _TRISB15 = 1;

    // RC1 - AI0
    // RC2 - AI1
    // RC3 - AI2
    // RC4 - AI3

    // RC13- N8 - 绝对值编码器接口.
    _TRISC13 = 1;
    // RD0 - N9
    _TRISD0  = 1;
    // RD1 - N7
    // RD2 - N6
    // RD3 - N5
    // RD4 - N3
    // RD5 - N2
    // RD6 - N1
    // RD7 - N0
    // RD8 - N13
    // RD9 - N12
    // RD10- N11
    // RD11- N10
    // RD12- 空.
    // RD13- N4
    // RD14- Input51
    // RD15- Input61
    TRISB = 0xFFFF; // 全部设置为输入.

    // RE (0 - 7 通道均为输出)

    // RE0 - SW8
    // RE1 - SW7
    // RE2 - SW3
    // RE3 - SW2
    // RE4 - SW1
    // RE5 - STEP // 电机PWM输出.
    // RE6 - DIR
    // RE7 - EN

    TRISE = 0xFF00;
    LATE  = 0x00;

    // RF.
    // RF0 - CAN
    // RF1 - CAN
    // RF2 - UART
    // RF3 - UART
    // RF4 - INPUT71
    // RF5 - INPUT81
    // RF6
    // RF7 - RE/DE.
    // RF12- OUT5
    // RF13- OUT6
    _TRISE7 = 0; // 输出.
    _LATE7  = 1; // 485收发使能.

    _TRISF12 = 0;
    _TRISF13 = 0;
    _LATF12  = 0;
    _LATF13  = 0;
    
    // LED.
    // RG6 - COM
    // RG7 - ALARM
    // RG8 - SYS.

    // RG12- SW5
    // RG13- SW4
    // RG14 -SW6
    _TRISG6 = 0;
    _TRISG7 = 0;
    _TRISG8 = 0;

    _LATG6  = 1;
    _LATG7  = 1;
    _LATG8  = 1;

    _TRISG12= 0;
    _TRISG13= 0;
    _TRISG14= 0;

    _LATG12 = 0;
    _LATG13 = 0;
    _LATG14 = 0;
    // 
}

//end of file

