#ifndef __BSP_GPIO__H__
#define __BSP_GPIO__H__
#include "includes.h"
// GPIO口定义.

// 8路数字输入.

// RB12 INPUT11
// RB13 INPUT21
// RB14 INPUT31
// RB15 INPUT41
// RD14- Input51
// RD15- Input61
// RF4 - INPUT71
// RF5 - INPUT81

#define DI_INPUT0 _RB12
#define DI_INPUT1 _RB13
#define DI_INPUT2 _RB14
#define DI_INPUT3 _RB15
#define DI_INPUT4 _RD14
#define DI_INPUT5 _RD15
#define DI_INPUT6 _RF4
#define DI_INPUT7 _RF5
#define DI_INPUT8 _RF8

// 8路数字输出.
// RB7 OUT4.
// RB8 OUT3.
// RB9 OUT2.
// RB10 OUT1.
// RB11 OUT8
// RA1 - OUT7
// RF12- OUT5
// RF13- OUT6

#define DO_OUT1 _LATB10
#define DO_OUT2 _LATB9
#define DO_OUT3 _LATB8
#define DO_OUT4 _LATB7
#define DO_OUT5 _LATF12
#define DO_OUT6 _LATF13
#define DO_OUT7 _LATA1
#define DO_OUT8 _LATB11

#define SET_DO_VALUE(ucValue) \
    {\
        DO_OUT1= ((ucValue >> 0) &0x01);\
        DO_OUT2= ((ucValue >> 1) &0x01);\
        DO_OUT3= ((ucValue >> 2) &0x01);\
        DO_OUT4= ((ucValue >> 3) &0x01);\
        DO_OUT5= ((ucValue >> 4) &0x01);\
        DO_OUT6= ((ucValue >> 5) &0x01);\
        DO_OUT7= ((ucValue >> 6) &0x01);\
        DO_OUT8= ((ucValue >> 7) &0x01);\
    }


// RG6 - COM
// RG7 - ALARM
// RG8 - SYS.

#define LED_COM _LATG6
#define LED_SYS _LATG8
#define LED_ALM _LATG7

// 驱动器设置接口.
// RE0 - SW8
// RE1 - SW7
// RE2 - SW3
// RE3 - SW2
// RE4 - SW1
// RG12- SW5
// RG13- SW4
// RG14 -SW6

#define DRV_SW1 _LATE4
#define DRV_SW2 _LATE3
#define DRV_SW3 _LATE2
#define DRV_SW4 _LATG13
#define DRV_SW5 _LATG12
#define DRV_SW6 _LATG14
#define DRV_SW7 _LATE1
#define DRV_SW8 _LATE0

#define SET_MOTOR_DRIVER(ucValue) \
    {\
        DRV_SW1= ((ucValue >> 0) &0x01);\
        DRV_SW2= ((ucValue >> 1) &0x01);\
        DRV_SW3= ((ucValue >> 2) &0x01);\
        DRV_SW4= ((ucValue >> 3) &0x01);\
        DRV_SW5= ((ucValue >> 4) &0x01);\
        DRV_SW6= ((ucValue >> 5) &0x01);\
        DRV_SW7= ((ucValue >> 6) &0x01);\
        DRV_SW8= ((ucValue >> 7) &0x01);\
    }

// 绝对式编码器接口.

// RD7 - N0
// RD6 - N1
// RD5 - N2
// RD4 - N3
// RD13- N4
// RD3 - N5
// RD2 - N6
// RD1 - N7
// RC13- N8
// RD0 - N9
// RD11- N10
// RD10- N11
// RD9 - N12
// RD8 - N13


#define ENCODER_ABS_0 _RD7
#define ENCODER_ABS_1 _RD6
#define ENCODER_ABS_2 _RD5
#define ENCODER_ABS_3 _RD5
#define ENCODER_ABS_4 _RD13
#define ENCODER_ABS_5 _RD3
#define ENCODER_ABS_6 _RD2
#define ENCODER_ABS_7 _RD1
#define ENCODER_ABS_8 _RC13
#define ENCODER_ABS_9 _RD0
#define ENCODER_ABS_10 _RD11
#define ENCODER_ABS_11 _RD10
#define ENCODER_ABS_12 _RD9
#define ENCODER_ABS_13 _RD8

#define ENCODER_ABS_BIT(x) ENCODER_ABS_##x


#define ENCODER_ABS_VALUE \
    (((ENCODER_ABS_BIT(0) << 0)\
    |(ENCODER_ABS_BIT(1) << 1)\
    |(ENCODER_ABS_BIT(2) << 2)\
    |(ENCODER_ABS_BIT(3) << 3)\
    |(ENCODER_ABS_BIT(4) << 4)\
    |(ENCODER_ABS_BIT(5) << 5)\
    |(ENCODER_ABS_BIT(6) << 6)\
    |(ENCODER_ABS_BIT(7) << 7)\
    |(ENCODER_ABS_BIT(8) << 8)\
    |(ENCODER_ABS_BIT(9) << 9)\
    |(ENCODER_ABS_BIT(10)<<10)\
    |(ENCODER_ABS_BIT(11)<<11)\
    |(ENCODER_ABS_BIT(12)<<12)\
    |(ENCODER_ABS_BIT(13)<<13)))
    

// 电机控制接口-输出.
#define MOTOR_EN  _LATE6
#define MOTOR_DIR _LATE7
#define MOTOR_STEP _LATE5


extern void GPIO_Init(void);
#endif //
// end of file

