#include "includes.h"

void PWM_Init(void)
{

	P1OVDCONbits.POVD3H            = 1;

    PWM1CON1bits.PMOD1  = 0;  // PWM1是独立输出.
    PWM1CON1bits.PMOD2  = 0;  // PWM2是独立输出.
    PWM1CON1bits.PMOD3  = 0;  // PWM3是独立输出.
    PWM1CON1bits.PMOD4  = 0;  // PWM4是独立输出.
    

    PWM1CON1bits.PEN1L  = 0;      //PWM1L 普通I/O模式
	PWM1CON1bits.PEN2L  = 0;      //PWM2L 普通I/O模式
	PWM1CON1bits.PEN3L  = 0;      //PWM3L 普通I/O模式
	PWM1CON1bits.PEN4L  = 0;      //PWM4L 普通I/O模式	
	
	PWM1CON1bits.PEN1H  = 0;      //PWM1H 普通I/O模式
	PWM1CON1bits.PEN2H  = 0;      //PWM2H 普通I/O模式
	PWM1CON1bits.PEN3H  = 1;      //PWM3H PWM输出
	PWM1CON1bits.PEN4H  = 0;      //PWM4H 普通I/O模式
	
	PWM1CON1bits.PMOD1  = 0;      //PWM1  互补输出模式
	PWM1CON1bits.PMOD2  = 0;      //PWM2  互补输出模式
	PWM1CON1bits.PMOD3  = 1;      //PWM3  独立输出模式
	PWM1CON1bits.PMOD4  = 0;      //PWM4  互补输出模式

    P1SECMPbits.SEVTDIR = 0;      //PWM 时基递增产生特殊事件信号
	
	P1TPER            = 10000;   //PWM 周期寄存器值(10M->10k)
	
	P1TMRbits.PTMR    = 0x0000;   //PWM 时基计数

    P1DC3             = 10000;
  
    
	// PWM时基
    P1TCONbits.PTSIDL   = 0; // 停止模式下运行.
    P1TCONbits.PTCKPS   = 1; // Tcy/4 = 10M.
    P1TCONbits.PTMOD    = 0;
    P1TCONbits.PTOPS    = 16U;// 
    P1TCONbits.PTEN     = 1; // 开启.

	// P1OVDCONbits.POUT3H = 1;
	
}


void SetMotorPwm(u8 ucDir, u8 ucFreq)
{


#if 0
    static u16 s_ausPeriod[] = 
    {
        0x1000,0x800,0x600,0x400,0x200,0x100
    };

    if (ucFreq == 0)
    {
        MOTOR_EN = 0;
    }
    else
    {
        MOTOR_EN = 1;
    }


    MOTOR_DIR = ucDir;

    if (ucFreq > 5)
        ucFreq = 5;

    if (ucFreq != 0)
    {
        PWM1CON2bits.OSYNC = 1;

        P1OVDCONbits.POVD3H = 0;
        
        P1TMRbits.PTMR = s_ausPeriod[ucFreq];

        P1OVDCONbits.POUT3H = 1;
        
    }

#endif	

}
