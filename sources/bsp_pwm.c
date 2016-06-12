#include "includes.h"

void PWM_Init(void)
{

	P1OVDCONbits.POVD3H            = 1;

    PWM1CON1bits.PMOD1  = 0;  // PWM1�Ƕ������.
    PWM1CON1bits.PMOD2  = 0;  // PWM2�Ƕ������.
    PWM1CON1bits.PMOD3  = 0;  // PWM3�Ƕ������.
    PWM1CON1bits.PMOD4  = 0;  // PWM4�Ƕ������.
    

    PWM1CON1bits.PEN1L  = 0;      //PWM1L ��ͨI/Oģʽ
	PWM1CON1bits.PEN2L  = 0;      //PWM2L ��ͨI/Oģʽ
	PWM1CON1bits.PEN3L  = 0;      //PWM3L ��ͨI/Oģʽ
	PWM1CON1bits.PEN4L  = 0;      //PWM4L ��ͨI/Oģʽ	
	
	PWM1CON1bits.PEN1H  = 0;      //PWM1H ��ͨI/Oģʽ
	PWM1CON1bits.PEN2H  = 0;      //PWM2H ��ͨI/Oģʽ
	PWM1CON1bits.PEN3H  = 1;      //PWM3H PWM���
	PWM1CON1bits.PEN4H  = 0;      //PWM4H ��ͨI/Oģʽ
	
	PWM1CON1bits.PMOD1  = 0;      //PWM1  �������ģʽ
	PWM1CON1bits.PMOD2  = 0;      //PWM2  �������ģʽ
	PWM1CON1bits.PMOD3  = 1;      //PWM3  �������ģʽ
	PWM1CON1bits.PMOD4  = 0;      //PWM4  �������ģʽ

    P1SECMPbits.SEVTDIR = 0;      //PWM ʱ���������������¼��ź�
	
	P1TPER            = 10000;   //PWM ���ڼĴ���ֵ(10M->10k)
	
	P1TMRbits.PTMR    = 0x0000;   //PWM ʱ������

    P1DC3             = 10000;
  
    
	// PWMʱ��
    P1TCONbits.PTSIDL   = 0; // ֹͣģʽ������.
    P1TCONbits.PTCKPS   = 1; // Tcy/4 = 10M.
    P1TCONbits.PTMOD    = 0;
    P1TCONbits.PTOPS    = 16U;// 
    P1TCONbits.PTEN     = 1; // ����.

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
